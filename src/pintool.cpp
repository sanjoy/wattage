#include "estimators.hpp"
#include "processor-traits.hpp"


#include <stdint.h>
#include <fstream>
#include <iostream>

using namespace std;
using namespace wattage;

namespace {
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
                            "output", "wattage.out",
                            "specify output file name");
KNOB<string> KnobTraitsFile(KNOB_MODE_WRITEONCE, "pintool",
                            "traits-file", "default.traits",
                            "specify the file to read processor traits from");
KNOB<bool> KnobPrintTraitNames(KNOB_MODE_WRITEONCE, "pintool",
                               "print-trait-names", "false",
                               "print the trait field names");
KNOB<bool> KnobPrintTraitValues(KNOB_MODE_WRITEONCE, "pintool",
                                "print-trait-values", "false",
                                "read the trait values from the traits file "
                                "and print them out");

int ShowUsage() {
  cerr << "wattage (sanjoy@playingwithpointers.com)" << endl;
  cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
  return -1;
}

class Baton {
 public:
  static Baton *create(const char *traits_file_name,
                       const char *output_file_name) {
    FILE *output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
      cerr << "error: could not open output file `" << output_file_name << "`!"
           << endl;
      return NULL;
    }

    Baton *baton = new Baton;
    baton->output_file_ = output_file;

    FILE *traits_file = fopen(traits_file_name, "r");

    if (traits_file == NULL) {
      cerr << "error: could not open traits file `" << traits_file_name
           << "`!" << endl;
      return NULL;
    }

    char *error_string = NULL;
    bool result = baton->traits_.read_from(traits_file, &error_string);
    fclose(traits_file);

    if (!result) {
      cerr << "error: " << error_string << "!" << endl;
      delete baton;
      return NULL;
    }

    baton->estimator_ = new Estimator(&baton->counter_, &baton->traits_);
    return baton;
  }

  Estimator *estimator() const { return estimator_; }

  void save() const {
    power_counters()->print(output_file_);
    fclose(output_file_);
  }
  const PowerCounter *power_counters() const { return &counter_; }

  ~Baton() {
    fclose(output_file_);
    delete estimator_;
  }

 private:
  ProcessorTraits traits_;
  PowerCounter counter_;
  Estimator *estimator_;
  FILE *output_file_;
};

class PINCallbacks {
 public:
  static void setup(Baton *baton) {
    assert("use teardown before setup!" && baton_ == NULL);
    baton_ = baton;
    INS_AddInstrumentFunction(instrument_INS, NULL);
    PIN_AddFiniFunction(finish, NULL);
  }

  static void teardown() {
    finish(0, NULL);
  }

 private:
  static void finish(int code, void *) {
    baton()->save();
    delete baton();
    baton_ = NULL;
  }

  static void instrument_INS(INS ins, void *) {
    if (INS_IsMemoryRead(ins)) {
      INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(compute_cost_INS),
                     IARG_ADDRINT, ins, IARG_CONTEXT, IARG_MEMORYREAD_EA,
                     IARG_MEMORYREAD_SIZE, IARG_ADDRINT, INS_Address(ins),
                     IARG_UINT32, INS_Size(ins), IARG_END);
    } else if (INS_IsMemoryWrite(ins)) {
      INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(compute_cost_INS),
                     IARG_ADDRINT, ins, IARG_CONTEXT, IARG_MEMORYWRITE_EA,
                     IARG_MEMORYWRITE_SIZE, IARG_ADDRINT, INS_Address(ins),
                     IARG_UINT32, INS_Size(ins), IARG_END);
    } else {
      INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(compute_cost_INS),
                     IARG_ADDRINT, ins, IARG_CONTEXT, IARG_ADDRINT, NULL,
                     IARG_UINT32, 0, IARG_ADDRINT, INS_Address(ins),
                     IARG_UINT32, INS_Size(ins), IARG_END);
    }
  }

  static void compute_cost_INS(INS ins, const CONTEXT *ctx, intptr_t daddr,
                               uint32_t dsize, intptr_t iaddr, uint32_t isize) {
    baton()->estimator()->compute_cost(ins, ctx, daddr, dsize, iaddr, isize);
  }

  static Baton *baton() { return baton_; }
  static Baton *baton_;
};

Baton *PINCallbacks::baton_ = NULL;

void PrintTraitValues(FILE *output) {
  string file_name = KnobTraitsFile.Value();
  fprintf(output, "using traits file `%s`\n", file_name.c_str());

  FILE *file = fopen(file_name.c_str(), "r");
  if (file == NULL) {
    fprintf(output, "error: could not open traits file.\n");
    return;
  }

  ProcessorTraits traits;
  char *error_msg = NULL;
  if (traits.read_from(file, &error_msg)) traits.dump(output);
  else fprintf(output, "error: %s.\n", error_msg);

  fclose(file);
}

}

int main(int argc, char **argv) {
  if (PIN_Init(argc, argv)) return ShowUsage();
  if (KnobPrintTraitNames.Value()) {
    ProcessorTraits::print_fields(stdout);
    return 0;
  }
  if (KnobPrintTraitValues.Value()) {
    PrintTraitValues(stdout);
    return 0;
  }

  Baton *baton = Baton::create(KnobTraitsFile.Value().c_str(),
                               KnobOutputFile.Value().c_str());
  if (!baton) return 1;
  PINCallbacks::setup(baton);
  PIN_StartProgram();
  return 0;
}

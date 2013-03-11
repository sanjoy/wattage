#include "intel-xed-opcodes.hpp"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Signals.h"
#include "llvm/TableGen/Error.h"
#include "llvm/TableGen/Main.h"
#include "llvm/TableGen/Record.h"

#include <cassert>
#include <iostream>

using namespace llvm;
using namespace std;

namespace {

class InstrItineraryEmitter {
 public:
  InstrItineraryEmitter(RecordKeeper &record_keeper, raw_ostream &output)
      : record_keeper_(record_keeper),
        output_(output) {
    instructions_ = record_keeper.getAllDerivedDefinitions("I");
  }

  bool run();

 private:
  RecordKeeper &record_keeper_;
  raw_ostream &output_;
  vector<Record *> instructions_;

  Record *find_inst_for_name(const char *name);
  void emit_for_xed(const char *xed);
};

Record *InstrItineraryEmitter::find_inst_for_name(const char *name) {
  vector<Record *>::const_iterator index = instructions_.begin();
  size_t position = 0;

  while (name[position] != 0 && index != instructions_.end()) {
    if ((*index)->getName().length() <= position) {
      index++;
      continue;
    }
    char current_char = (*index)->getName()[position];
    if (current_char == name[position]) {
      position++;
    } else if (current_char > name[position]) {
      return NULL;
    } else {
      index++;
    }
  }

  return *index;
}

void InstrItineraryEmitter::emit_for_xed(const char *xed) {
  Record *inst = find_inst_for_name(xed);
  string iic_value;
  if (inst == NULL) {
    iic_value = "IIC_UNKNOWN";
  } else {
    iic_value =
        inst->getValueAsDef(StringRef("Itinerary"))->getNameInitAsString();
  }

  output_ << "  itinerary_table_[XED_ICLASS_" << xed << "] = "
          << iic_value << ";\n";
}

bool InstrItineraryEmitter::run() {
  output_ << "void ProcessorTraits::populate_itinerary_table() {\n";

#define PROCESS_XED_OPCODE(xed_name) emit_for_xed( # xed_name);
  INTEL_XED_OPCODES(PROCESS_XED_OPCODE);
#undef PROCESS_XED_OPCODE

  output_ << "}\n";

  return false;
}

bool TableGenCallback(raw_ostream &output, RecordKeeper &keeper) {
  return InstrItineraryEmitter(keeper, output).run();
}

}

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);
  return TableGenMain(argv[0], &TableGenCallback);
}

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

enum ActionType {
  GenItineraryEnum,
  GenItineraryTable
};

cl::opt<ActionType>
Action(cl::desc("Action to perform:"),
       cl::values(clEnumValN(GenItineraryEnum, "gen-itinerary-enum",
                             "Generate enum values for Itineraries"),
                  clEnumValN(GenItineraryTable, "gen-itinerary-table",
                             "Generate a function to populate itinerary "
                             "tables"),
                  clEnumValEnd));

class InstrItineraryTableEmitter {
 public:
  InstrItineraryTableEmitter(RecordKeeper &record_keeper, raw_ostream &output)
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

Record *InstrItineraryTableEmitter::find_inst_for_name(const char *name) {
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

void InstrItineraryTableEmitter::emit_for_xed(const char *xed) {
  Record *inst = find_inst_for_name(xed);
  string iic_value;
  if (inst == NULL) {
    iic_value = "UNKNOWN";
  } else {
    string name =
        inst->getValueAsDef(StringRef("Itinerary"))->getNameInitAsString();
    iic_value = string(name.begin() + 4, name.end());
  }

  output_ << "  F(" << xed << ", " << iic_value << ") \\\n";
}

bool InstrItineraryTableEmitter::run() {
  output_ << "#define IIC_INST_PAIRS(F) \\\n";

#define PROCESS_XED_OPCODE(xed_name) emit_for_xed( # xed_name);
  INTEL_XED_OPCODES(PROCESS_XED_OPCODE);
#undef PROCESS_XED_OPCODE

  output_ << "\n";

  return false;
}

class ItineraryEnumEmitter {
 public:
  ItineraryEnumEmitter(RecordKeeper &record_keeper, raw_ostream &output)
      : record_keeper_(record_keeper),
        output_(output) { }

  bool run();

 private:
  RecordKeeper &record_keeper_;
  raw_ostream &output_;
};

bool ItineraryEnumEmitter::run() {
  vector<Record *> itineraries =
      record_keeper_.getAllDerivedDefinitions("InstrItinClass");
  output_ << "#define INSTR_ITIN_CLASSES(F) \\\n";

  for (vector<Record *>::const_iterator I = itineraries.begin(),
                                        E = itineraries.end();
       I != E;
       ++I) {
    string name = (*I)->getName();
    if (name != "NoItinerary") {
      output_ << "  F(" << string(name.begin() + 4, name.end()) << ") \\\n";
    }
  }
  output_ << "\n";

  return false;
}

bool TableGenCallback(raw_ostream &output, RecordKeeper &keeper) {
  switch (Action) {
    case GenItineraryEnum:
      return ItineraryEnumEmitter(keeper, output).run();
    case GenItineraryTable:
      return InstrItineraryTableEmitter(keeper, output).run();
  }
  return true;
}

}

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);
  return TableGenMain(argv[0], &TableGenCallback);
}

#ifndef __INSTRUCTION_COST_HPP
#define __INSTRUCTION_COST_HPP

#include <map>
#include <string>

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Debug.h"
#include "llvm/TableGen/Record.h"
#include "CodeGenInstruction.h"

#include "processor-traits.hpp"

namespace wattage {

class InstructionCosts {
 public:
  explicit InstructionCosts(const ProcessorTraits &traits);

  int get_base_cpu_cost(llvm::Record *Op,
                        const llvm::CodeGenInstruction &II) const {
    assert(Op->isSubClassOf("Instruction") && "contract");

    string itinerary_name = Op->getValueAsDef(llvm::StringRef(
        "Itinerary"))->getNameInitAsString();

    std::map<std::string, int>::const_iterator i =
        cost_map_.find(itinerary_name);

    if (i == cost_map_.end()) {
      llvm::dbgs() << "could not find itinerary `" << itinerary_name << "`\n";
      llvm::dbgs() << "cost_map_ = {\n";
      for (map<string, int>::const_iterator I = cost_map_.begin(),
                                            E = cost_map_.end();
           I != E;
           ++I) {
        llvm::dbgs() << "  " << I->first << " : " << I->second << "\n";
      }
      llvm::dbgs() << "}\n";
      abort();
    }
    assert(i->second != 0 && "we multiply costs!");
    int base_cost = i->second;

    if (II.isMoveImm) base_cost *= 2;
    if (II.mayLoad) base_cost *= 2;
    if (II.mayStore) base_cost *= 2;

    return base_cost;
  }

 private:
  std::map<std::string, int> cost_map_;
};

}

#endif

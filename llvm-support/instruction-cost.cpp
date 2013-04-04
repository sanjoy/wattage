#include "instruction-cost.hpp"
#include "processor-iic-categories.hpp"

#include <cassert>

using namespace llvm;
using namespace wattage;

enum {
#define CAT_ENUM(CAPS, small) CAT_ ## CAPS,
  INSTRUCTION_CATEGORIES(CAT_ENUM)
#undef CAT_ENUM
  CAT_MAX
};

InstructionCosts::InstructionCosts(const ProcessorTraits &traits) {
  int category_costs[CAT_MAX];

#define ASSIGN_CAT_COSTS(CAPS, small)                                   \
  category_costs[CAT_ ## CAPS] = static_cast<int>(                      \
      traits.category_ ## small ## _weight() / 10.0);
  INSTRUCTION_CATEGORIES(ASSIGN_CAT_COSTS);
#undef ASSIGN_CAT_COSTS

  int current_cost = 0;

#define CHANGE_CAT(CAT_NAME) current_cost = category_costs[CAT_ ## CAT_NAME];
#define FOR_EACH_IIC(NAME) cost_map_[ "IIC_" # NAME ] = current_cost;
  PROCESSOR_IIC_CATEGORIES(FOR_EACH_IIC, CHANGE_CAT);
#undef FOR_EACH_IIC
#undef CHANGE_CAT
  cost_map_["IIC_DEFAULT"] = traits.category_default_weight();
  cost_map_["NoItinerary"] = traits.category_default_weight();
}

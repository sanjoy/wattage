#ifndef __PROCESSOR_TRAITS__HPP
#define __PROCESSOR_TRAITS__HPP

#include "processor-traits-itinerary-types.inc"
#include "processor-traits-instruction-to-itinerary.inc"

#include "pin.H"

#include <cassert>
#include <cstdio>

#define UNDIRECTED_PROPERTIES(F)                \
  F(imm_operand_cpu_wt)                         \
  F(reg_cpu_wt)                                 \
  F(reg_value_cpu_wt)                           \
  F(inst_addr_cpu_wt)                           \
                                                \
  F(data_addr_cpu_wt)                           \
                                                \
  F(inst_addr_mem_wt)                           \
  F(inst_value_mem_wt)                          \
  F(data_addr_mem_wt)

#define DIRECTED_PROPERTIES(F)                  \
  F(data_value_cpu_wt)                          \
  F(data_value_mem_wt)

#define INSTRUCTION_CATEGORIES(F)                               \
  F(DEFAULT, default)                                           \
  F(GENERIC_ALU, generic_alu)                                   \
  F(INTEGER_MULTIPLICATION, integer_multiplication)             \
  F(INTEGER_DIVISION, integer_division)                         \
  F(FLOAT_DIVISION, float_division)                             \
  F(UNARY_ARITHMETIC_OPERATION, unary_arithmetic_operation)     \
  F(BINARY_ARITHMETIC_OPERATION, binary_arithmetic_operation)   \
  F(SHIFT_ROTATE, shift_rotate)                                 \
  F(DOUBLE_SHIFT, double_shift)                                 \
  F(CONDITIONAL_MOVE, conditional_move)                         \
  F(SET, set)                                                   \
  F(JUMP, jump)                                                 \
  F(LOOP, loop)                                                 \
  F(CALL, call)                                                 \
  F(RETURN, return)                                             \
  F(SIGN_EXTENSION, sign_extension)                             \
  F(ZERO_EXTENSION, zero_extension)                             \
  F(SSE, sse)                                                   \
  F(MMX, mmx)                                                   \
  F(SYSTEM, system)                                             \
  F(NOP, nop)

namespace wattage {

class ProcessorTraits {
 public:
  float base_cpu(OPCODE opcode) {
    return get_iic_weight(static_cast<xed_iclass_enum_t>(opcode));
  }

  float base_mem(INS ins) { return 10.0f; }
  float functional_unit_change(INS i0, INS i1) { return 0.0f; }

#define DEFINE_PROPERTY(name)                   \
 private:                                       \
  float name ## _;                              \
 public:                                        \
  float name() const { return name ## _; }

#define DEFINE_UNDIRECTED_PROPERTY(name)        \
  DEFINE_PROPERTY(name ## _individual)          \
  DEFINE_PROPERTY(name ## _hamming)

#define DEFINE_DIRECTED_PROPERTY(name)                  \
  DEFINE_PROPERTY(name ## _individual_read)             \
  DEFINE_PROPERTY(name ## _individual_write)            \
  DEFINE_PROPERTY(name ## _hamming_read)                \
  DEFINE_PROPERTY(name ## _hamming_write)

#define DEFINE_INSTR_CATEGORY(caps_name, lower_name)    \
  DEFINE_PROPERTY(category_ ## lower_name ## _weight)

  UNDIRECTED_PROPERTIES(DEFINE_UNDIRECTED_PROPERTY);
  DIRECTED_PROPERTIES(DEFINE_DIRECTED_PROPERTY);
  INSTRUCTION_CATEGORIES(DEFINE_INSTR_CATEGORY);

#undef DEFINE_UNDIRECTED_PROPERTY
#undef DEFINE_DIRECTED_PROPERTY
#undef DEFINE_INSTR_CATEGORY

 public:
  ProcessorTraits();

  // Returns false on error
  bool read_from(std::FILE *file, char **error);
  void dump(std::FILE *file);

  static void print_fields(std::FILE *file);

 private:
  enum InstrItinClass {
    IIC_UNKNOWN,
#define DECL_IIC_ENUM(name) IIC_ ## name ,
    INSTR_ITIN_CLASSES(DECL_IIC_ENUM)
#undef DECL_IIC_ENUM
    IIC_COUNT
  };

  enum InstCategory {
    CATEGORY_UNKNOWN,
#define DECLARE_CATEGORY(caps_name, lower_name) CATEGORY_ ## caps_name ,
    INSTRUCTION_CATEGORIES(DECLARE_CATEGORY)
#undef DECLARE_CATEGORY
    CATEGORY_COUNT
  };

  void initialize_iic_table();
  void initialize_category_table();
  float get_iic_weight(xed_iclass_enum_t opcode);

  uint8_t iic_category_table_lookup(int index) {
    assert(index >= IIC_UNKNOWN && index < IIC_COUNT);
    return iic_table_[index];
  }

  uint16_t iic_table_lookup(xed_iclass_enum_t opcode) {
    int opc = static_cast<int>(opcode);
    assert(opc > XED_ICLASS_INVALID && opc < XED_ICLASS_LAST);
    return iic_table_[opc];
  }

  uint16_t iic_table_[XED_ICLASS_LAST];
  uint8_t iic_category_table_[IIC_COUNT];
};

};

#endif // __PROCESSOR_TRAITS__HPP

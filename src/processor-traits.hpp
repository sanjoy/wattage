#ifndef __PROCESSOR_TRAITS__HPP
#define __PROCESSOR_TRAITS__HPP

#include "pin.H"

#include <cassert>
#include <cstdio>

namespace wattage {

class ProcessorTraits {
 public:
  float base_cpu(OPCODE opcode) { return 100.0f; }
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

  UNDIRECTED_PROPERTIES(DEFINE_UNDIRECTED_PROPERTY)
  DIRECTED_PROPERTIES(DEFINE_DIRECTED_PROPERTY)

#undef DEFINE_UNDIRECTED_PROPERTY
#undef DEFINE_DIRECTED_PROPERTY

 public:
  ProcessorTraits();

  bool read_from(std::FILE *file, char **error);
};

};

#endif // __PROCESSOR_TRAITS__HPP

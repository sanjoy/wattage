#include "estimators.hpp"

#include <iostream>

using namespace wattage;

PowerCounter::PowerCounter() {
  for (iterator I = begin(), E = end(); I != E; ++I) {
    *I = 0.0f;
  }
}

void PowerCounter::print(FILE *stream) const {
  for (int i = 0; i < COUNTER_TYPE_LEN; i++) {
    fprintf(stream, "[%s] = %f\n", counter_type_to_cstring(i), counters_[i]);
  }
}

const char *PowerCounter::counter_type_to_cstring(int type) const {
  switch (type) {
    case CPU_INSTR: return "cpu_instr";
    case CPU_DATA: return "cpu_data";
    case MEM_INSTR: return "mem_instr";
    case MEM_DATA: return "mem_data";
  }
  assert("unhandled powercounter type" && 0);
  return NULL;
}

int RegfileBuffer::xor_and_assign(const CONTEXT *context) {
  static const REG regs[] = {
    REG_RAX, REG_RBX, REG_RCX, REG_RDX,
    REG_RSP, REG_RBP,
    REG_SEG_ES, REG_SEG_FS, REG_SEG_GS, REG_SEG_SS,
    REG_R8, REG_R9, REG_R10, REG_R11, REG_R12, REG_R13,
    REG_R14, REG_R15
  };

  assert(kTrackedRegistersLen == (sizeof(regs) / sizeof(REG)));

  int xor_count = 0;

  for (int i = 0; i < kTrackedRegistersLen; i++) {
    intptr_t current =
        static_cast<intptr_t>(PIN_GetContextReg(context, regs[i]));
    xor_count =
        Bitwise::pop_count_64(static_cast<uint64_t>(buffer_[i] ^ current));
    buffer_[i] = current;
  }

  return xor_count;
}

void Estimator::compute_cost(INS ins, const CONTEXT *context,
                             intptr_t memory_address,
                             uint32_t dsize) {
  if (!INS_IsOriginal(ins)) return;

  current_ins_ = ins;
  current_isize_ = INS_Size(ins);
  reg_context_ = context;
  current_daddr_ = memory_address;
  current_dsize_ = dsize;
  current_iaddr_ = INS_Address(ins);

  process();

  previous_ins_ = current_ins_;
  previous_isize_ = current_isize_;
  previous_daddr_ = current_daddr_;
  previous_iaddr_ = current_iaddr_;
}

void Estimator::process() {
  process_constants();
  process_imm();
  process_reg();
  process_regfile();
  process_iaddr();
  process_ivalue();
  process_daddr();
  process_dvalue();
}

void Estimator::process_constants() {
  power_counter_->accumulate(PowerCounter::CPU_INSTR,
                             traits_->base_cpu(INS_Opcode(current_ins_)));
}

void Estimator::process_imm() {
  int operand_count = INS_OperandCount(current_ins_);

  BitBuffer<64> buffer;
  for (int i = 0; i < operand_count; i++) {
    if (INS_OperandIsImmediate(current_ins_, i)) {
      buffer.add_uint64(INS_OperandImmediate(current_ins_, i));
    }
  }

  float individual_cost =
      traits_->imm_operand_cpu_wt_individual() * buffer.pop_count();
  float hamming_cost =
      traits_->imm_operand_cpu_wt_hamming() * imm_buffer_.xor_count(buffer);
  imm_buffer_.store(buffer);

  power_counter_->accumulate(PowerCounter::CPU_INSTR,
                             individual_cost + hamming_cost);
}

void Estimator::process_reg() {
  const int operand_count = INS_OperandCount(current_ins_);

  BitBuffer<64> buffer;
  for (int i = 0; i < operand_count; i++) {
    if (INS_OperandIsReg(current_ins_, i)) {
      buffer.add_uint64(INS_OperandReg(current_ins_, i));
    }
  }

  float individual_cost =
      traits_->reg_cpu_wt_individual() * buffer.pop_count();
  float hamming_cost =
      traits_->reg_cpu_wt_hamming() * reg_buffer_.xor_count(buffer);
  reg_buffer_.store(buffer);

  power_counter_->accumulate(PowerCounter::CPU_INSTR,
                             individual_cost + hamming_cost);
}

void Estimator::process_regfile() {
  float individual_cost = 0.0f;
  const int operand_count = INS_OperandCount(current_ins_);

  for (int i = 0; i < operand_count; i++) {
    if (INS_OperandIsReg(current_ins_, i)) {
      REG reg = INS_OperandReg(current_ins_, i);
      if (!REG_is_fr_for_get_context(reg)) continue;
      intptr_t reg_value =
          static_cast<intptr_t>(PIN_GetContextReg(reg_context_, reg));
      individual_cost += Bitwise::pop_count_ptr(reg_value) *
                         traits_->reg_value_cpu_wt_individual();
    }
  }

  float hamming_cost = regfile_buffer_.xor_and_assign(reg_context_) *
                       traits_->reg_value_cpu_wt_hamming();

  power_counter_->accumulate(PowerCounter::CPU_INSTR,
                             individual_cost + hamming_cost);
}

void Estimator::process_iaddr() {
  const int base_count = Bitwise::pop_count_ptr(current_iaddr_);
  const int hamming_count =
      Bitwise::pop_count_ptr(current_iaddr_ ^ previous_iaddr_);

  power_counter_->accumulate(
      PowerCounter::CPU_INSTR,
      base_count * traits_->inst_addr_cpu_wt_individual() +
      hamming_count * traits_->inst_addr_cpu_wt_hamming());

  power_counter_->accumulate(
      PowerCounter::MEM_INSTR,
      base_count * traits_->inst_addr_mem_wt_individual() +
      hamming_count * traits_->inst_addr_mem_wt_hamming());
}

void Estimator::process_ivalue() {
  const uint8_t *prev_inst_buffer =
      reinterpret_cast<uint8_t *>(INS_Address(previous_ins_));
  const uint8_t *current_inst_buffer =
      reinterpret_cast<uint8_t *>(INS_Address(current_ins_));

  const int base_count =
      Bitwise::pop_count_buffer(current_inst_buffer, current_isize_);
  const int hamming_count =
      INS_Valid(previous_ins_) ?
      0 :
      Bitwise::xor_count_buffer(prev_inst_buffer, previous_isize_,
                                current_inst_buffer, current_isize_);

  power_counter_->accumulate(
      PowerCounter::MEM_INSTR,
      traits_->inst_value_mem_wt_individual() * base_count +
      traits_->inst_value_mem_wt_hamming() * hamming_count);
}

void Estimator::process_daddr() {
  const int base_count =
      Bitwise::pop_count_ptr(current_daddr_);
  const int hamming_count =
      Bitwise::pop_count_ptr(current_daddr_ ^ previous_daddr_);

  power_counter_->accumulate(
      PowerCounter::CPU_DATA,
      traits_->data_addr_cpu_wt_individual() * base_count +
      traits_->data_addr_cpu_wt_hamming() * hamming_count);
  power_counter_->accumulate(
      PowerCounter::CPU_DATA,
      traits_->data_addr_mem_wt_individual() * base_count +
      traits_->data_addr_mem_wt_hamming() * hamming_count);
}

void Estimator::process_dvalue() {
  float individual_wt_cpu, individual_wt_mem, hamming_wt_cpu, hamming_wt_mem;
  if (INS_IsMemoryRead(current_ins_)) {
    individual_wt_cpu = traits_->data_value_cpu_wt_individual_read();
    hamming_wt_cpu = traits_->data_value_cpu_wt_hamming_read();
    individual_wt_mem = traits_->data_value_mem_wt_individual_read();
    hamming_wt_mem = traits_->data_value_mem_wt_hamming_read();
  } else if (INS_IsMemoryWrite(current_ins_)) {
    individual_wt_cpu = traits_->data_value_cpu_wt_individual_write();
    hamming_wt_cpu = traits_->data_value_cpu_wt_hamming_write();
    individual_wt_mem = traits_->data_value_mem_wt_individual_write();
    hamming_wt_mem = traits_->data_value_mem_wt_hamming_write();
  } else {
    return;
  }

  BitBuffer<64> buffer;
  const uint8_t *raw_buffer = reinterpret_cast<uint8_t *>(current_daddr_);
  for (int i = 0; i < current_dsize_; i++) {
    buffer.add_uint8(raw_buffer[i]);
  }
  const int base_count = buffer.pop_count();
  const int hamming_count = dvalue_buffer_.xor_count(buffer);
  dvalue_buffer_.store(buffer);

  power_counter_->accumulate(
      PowerCounter::CPU_DATA,
      base_count * individual_wt_cpu + hamming_count * hamming_wt_cpu);
  power_counter_->accumulate(
      PowerCounter::MEM_DATA,
      base_count * individual_wt_mem + hamming_count * hamming_wt_mem);
}

#ifndef __WATTAGE__ESTIMATORS_HPP
#define __WATTAGE__ESTIMATORS_HPP

#include "processor-traits.hpp"
#include "pin.H"

#include <stdint.h>


namespace wattage {

class PowerCounter {
 public:
  enum Type {
    CPU_INSTR,
    CPU_DATA,
    MEM_INSTR,
    MEM_DATA,
    COUNTER_TYPE_LEN
  };

  float read(Type type) { return counters_[type]; }
  void accumulate(Type type, float value) {
    counters_[type] += value;
  }

  typedef float *iterator;
  typedef const float *const_iterator;

  iterator begin() { return &counters_[0]; }
  iterator end() { return &counters_[COUNTER_TYPE_LEN]; }

  const_iterator begin() const { return &counters_[0]; }
  const_iterator end() const { return &counters_[COUNTER_TYPE_LEN]; }

  PowerCounter();
  void print(FILE *stream) const;

 private:
  float counters_[COUNTER_TYPE_LEN];
  const char *counter_type_to_cstring(int type) const;
};

class Bitwise {
 public:
  static int pop_count_8(uint8_t value) {
    value = value - ((value >> 1) & 0x55);
    value = (value & 0x33) + ((value >> 2) & 0x33);
    return (value + (value >> 4)) & 0x0F;
  }

  static int pop_count_32(uint32_t i) {
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
  }

  static int pop_count_64(uint64_t i) {
    i = i - ((i >> 1) & 0x5555555555555555ULL);
    i = (i & 0x3333333333333333ULL) + ((i >> 2) & 0x3333333333333333ULL);
    return (((i + (i >> 4)) & 0x0F0F0F0F0F0F0F0FULL) *
            0x0101010101010101ULL) >> 56;
  }

  static int pop_count_ptr(intptr_t i) {
    if (sizeof(intptr_t) == 4) {
      return pop_count_32(static_cast<uint32_t>(i));
    } else {
      assert("only 32 and 64 bits are allowed!" && sizeof(intptr_t) == 8);
      return pop_count_64(static_cast<uint64_t>(i));
    }
  }

  static int pop_count_buffer(const uint8_t *buffer, int len) {
    int result = 0;
    for (int i = 0; i < len; i++) {
      result += pop_count_8(buffer[i]);
    }
    return result;
  }

  static int xor_count_buffer(const uint8_t *buffer_s, int len_s,
                              const uint8_t *buffer_l, int len_l) {
    if (len_s > len_l) {
      return xor_count_buffer(buffer_l, len_l, buffer_s, len_s);
    }

    int xor_count = 0;
    for (int i = 0; i < len_s; i++) {
      xor_count += Bitwise::pop_count_8(buffer_s[i] ^ buffer_l[i]);
    }

    for (int i = len_s; i < len_l; i++) {
      xor_count += Bitwise::pop_count_8(buffer_l[i]);
    }
    return xor_count;
  }
};

template<int N>
class BitBuffer {
 public:
  BitBuffer() : length_(0) { }

  void store(const BitBuffer<N> &other) {
    length_ = other.length_;
    for (int i = 0; i < length_; i++) {
      buffer_[i] = other.buffer_[i];
    }
  }

  void add_uint8(uint8_t value) {
    assert("exceeded capacity!" && length_ < N);
    buffer_[length_++] = value;
  }

  void add_uint64(uint64_t value) {
    for (int i = 0; i < 8; i++) {
      add_uint8(value % 256ULL);
      value /= 256ULL;
    }
  }

  int xor_count(const BitBuffer<N> &other) const {
    return Bitwise::xor_count_buffer(buffer_, length_,
                                     other.buffer_, other.length_);
  }

  int pop_count() const {
    return Bitwise::pop_count_buffer(buffer_, length_);
  }

 private:
  uint8_t buffer_[N];
  int length_;

  BitBuffer(BitBuffer &);
  BitBuffer &operator=(BitBuffer &);
};

class RegfileBuffer {
 public:
  RegfileBuffer() {
    for (int i = 0; i < kTrackedRegistersLen; i++) {
      buffer_[i] = 0;
    }
  }

  int xor_and_assign(const CONTEXT *context);

 private:
  static const int kTrackedRegistersLen = 18;
  intptr_t buffer_[kTrackedRegistersLen];
};

class Estimator {
 public:
  Estimator(PowerCounter *power_counter, ProcessorTraits *traits) :
      power_counter_(power_counter),
      traits_(traits),
      previous_ins_(INS_Invalid()),
      current_ins_(INS_Invalid()),
      previous_isize_(0),
      current_isize_(0),
      reg_context_(NULL),
      previous_daddr_(0),
      current_daddr_(0),
      current_dsize_(0),
      previous_iaddr_(0),
      current_iaddr_(0) { }

  void compute_cost(INS ins, const CONTEXT *context, intptr_t memory_address,
                    uint32_t dsize);

 private:
  void process();

  void process_imm();
  void process_reg();
  void process_regfile();
  void process_iaddr();
  void process_ivalue();
  void process_daddr();
  void process_dvalue();

  PowerCounter *power_counter_;
  ProcessorTraits *traits_;

  INS previous_ins_;
  INS current_ins_;
  int previous_isize_;
  int current_isize_;

  const CONTEXT *reg_context_;

  intptr_t previous_daddr_;
  intptr_t current_daddr_;
  int current_dsize_;

  intptr_t previous_iaddr_;
  intptr_t current_iaddr_;

  BitBuffer<64> imm_buffer_;
  BitBuffer<64> reg_buffer_;
  BitBuffer<64> dvalue_buffer_;
  RegfileBuffer regfile_buffer_;
};

}

#endif

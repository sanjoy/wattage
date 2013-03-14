#include "processor-traits.hpp"

#include "processor-iic-categories.hpp"

#include <cstring>
#include <map>
#include <string>

using namespace wattage;
using namespace std;

ProcessorTraits::ProcessorTraits() {
#define INIT_UNDIRECTED_PROPERTY(name)           \
  name ## _individual_ = 0.0f;                   \
  name ## _hamming_ = 0.0f;                      \

#define INIT_DIRECTED_PROPERTY(name)            \
  name ## _individual_read_ = 0.0f;             \
  name ## _individual_write_ = 0.0f;            \
  name ## _hamming_read_ = 0.0f;                \
  name ## _hamming_write_ = 0.0f;

  UNDIRECTED_PROPERTIES(INIT_UNDIRECTED_PROPERTY);
  DIRECTED_PROPERTIES(INIT_DIRECTED_PROPERTY);

#undef INIT_UNDIRECTED_PROPERTY
#undef INIT_DIRECTED_PROPERTY

  initialize_iic_table();
}

bool ProcessorTraits::read_from(FILE *fptr, char **error) {
  const int kBufferLen = 1024;
  char buffer[kBufferLen];
  int current_line = 0;

  map<string, float *> dict;

#define INSERT_UNDIRECTED_PROPERTY(name)                \
  dict[#name "_individual"] = &name ## _individual_;    \
  dict[#name "_hamming"] = &name ## _hamming_;

#define INSERT_DIRECTED_PROPERTY(name)                                  \
  dict[#name "_individual_read"] = &name ## _individual_read_;          \
  dict[#name "_individual_write"] = &name ## _individual_write_;        \
  dict[#name "_hamming_read"] = &name ## _hamming_read_;                \
  dict[#name "_hamming_write"] = &name ## _hamming_write_;

#define INSERT_INSTR_CATEGORY(caps_name, lower_name) do {       \
    float *address = &category_ ## lower_name ## _weight_;      \
    dict["category_" #lower_name "_weight"] = address;          \
  } while(false);

  UNDIRECTED_PROPERTIES(INSERT_UNDIRECTED_PROPERTY);
  DIRECTED_PROPERTIES(INSERT_DIRECTED_PROPERTY);
  INSTRUCTION_CATEGORIES(INSERT_INSTR_CATEGORY);

#undef INSERT_DIRECTED_PROPERTY
#undef INSERT_UNDIRECTED_PROPERTY
#undef INSERT_INSTR_CATEGORY

  while (true) {
    buffer[kBufferLen - 2] = '\n';
    fgets(buffer, kBufferLen, fptr);
    if (feof(fptr)) break;

    current_line++;

    if (buffer[kBufferLen - 2] != '\n') {
      if (error != NULL) {
        *error = new char[500];
        sprintf(*error, "line %d too long", current_line);
      }
      return false;
    }

    buffer[strlen(buffer) - 1] = 0;
    if (buffer[0] == 0 || buffer[0] == '#') continue;

    char name[kBufferLen];
    float value = 0.0f;
    int items_read = sscanf(buffer, "%s %f", name, &value);
    if (items_read != 2) {
      if (error != NULL) {
        *error = new char[500 + kBufferLen];
        sprintf(*error, "could not parse line %d: `%s`", current_line, buffer);
      }
      return false;
    }

    if (float *address = dict[string(name)]) {
      *address = value;
    } else {
      if (error != NULL) {
        *error = new char[500 + kBufferLen];
        sprintf(*error, "unknown property: `%s`", name);
      }
      return false;
    }
  }

  return true;
}

void ProcessorTraits::dump(FILE *file) {
#define PRINT_PROPERTY(name) fprintf(file, "%s %f\n", #name, name());
#define PRINT_UNDIRECTED_PROPERTY(name)         \
  PRINT_PROPERTY(name ## _individual);          \
  PRINT_PROPERTY(name ## _hamming);
#define PRINT_DIRECTED_PROPERTY(name)           \
  PRINT_PROPERTY(name ## _individual_read);     \
  PRINT_PROPERTY(name ## _individual_write);    \
  PRINT_PROPERTY(name ## _hamming_read);        \
  PRINT_PROPERTY(name ## _hamming_write);
#define PRINT_INST_CATEGORY_WT(caps_name, small_name)   \
  PRINT_PROPERTY(category_ ## small_name ## _weight)

  UNDIRECTED_PROPERTIES(PRINT_UNDIRECTED_PROPERTY);
  DIRECTED_PROPERTIES(PRINT_DIRECTED_PROPERTY);
  INSTRUCTION_CATEGORIES(PRINT_INST_CATEGORY_WT);

#undef PRINT_INST_CATEGORY_WT
#undef PRINT_DIRECTED_PROPERTY
#undef PRINT_UNDIRECTED_PROPERTY
#undef PRINT_PROPERTY
}

void ProcessorTraits::initialize_iic_table() {
#define ASSIGN_IIC(inst, iic) iic_table_[XED_ICLASS_ ## inst] = IIC_ ## iic;
  IIC_INST_PAIRS(ASSIGN_IIC);
#undef ASSIGN_IIC
}

void ProcessorTraits::initialize_category_table() {
#define CHANGE_CATEGORY(new_cat) current_category = CATEGORY_ ## new_cat;
#define ASSIGN_IIC(iic_name)                                    \
  iic_category_table_[IIC_ ## iic_name] = current_category;

  uint8_t current_category = 0;
  int i;

  for (i = 0; i < IIC_COUNT; i++) {
    iic_category_table_[i] = IIC_DEFAULT;
  }

  PROCESSOR_IIC_CATEGORIES(ASSIGN_IIC, CHANGE_CATEGORY);

#undef ASSIGN_IIC
#undef CHANGE_CATEGORY
}

void ProcessorTraits::print_fields(FILE *file) {
#define PRINT_TRAITS_UNDIRECTED(name)           \
  fprintf(file,                                 \
          "%s_individual\n"                     \
          "%s_hamming\n",                       \
          # name, # name);
#define PRINT_TRAITS_DIRECTED(name)             \
  fprintf(file,                                 \
          "%s_individual_read\n"                \
          "%s_individual_write\n"               \
          "%s_hamming_read\n"                   \
          "%s_hamming_write\n",                 \
          # name, # name, # name, # name);
#define PRINT_TRAITS_INST_CATEGORY(caps_name, lower_name)       \
  fprintf(file, "category_%s_weight\n", # lower_name);

  UNDIRECTED_PROPERTIES(PRINT_TRAITS_UNDIRECTED);
  DIRECTED_PROPERTIES(PRINT_TRAITS_DIRECTED);
  INSTRUCTION_CATEGORIES(PRINT_TRAITS_INST_CATEGORY);

#undef PRINT_TRAITS_DIRECTED
#undef PRINT_TRAITS_UNDIRECTED
#undef PRINT_TRAITS_INST_CATEGORY
}

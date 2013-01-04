#include "processor-traits.hpp"

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

  UNDIRECTED_PROPERTIES(INSERT_UNDIRECTED_PROPERTY);
  DIRECTED_PROPERTIES(INSERT_DIRECTED_PROPERTY);

#undef INSERT_DIRECTED_PROPERTY
#undef INSERT_UNDIRECTED_PROPERTY

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

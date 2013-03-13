include pintool.config

HEADERS=src/estimators.hpp src/processor-traits.hpp processor-traits-itinerary-types.inc

ifeq ($(mode), release)
	CXXFLAGS=-Wall -Werror -O3 -DNDEBUG -I./
else
	CXXFLAGS=-Wall -Werror -g3 -I./
endif


all:: wattage.so wattage-tblgen

wattage.so: estimators.o pintool.o processor-traits.o
	$(PIN_LD) $(PIN_LDFLAGS) $(LINK_DEBUG) $^ $(PIN_LPATHS) $(PIN_LIBS) $(DBG) -o $@

%.o: src/%.cpp $(HEADERS)
	$(CXX) $(COPT) $(CXXFLAGS) $(PIN_CXXFLAGS) $< -o $@

processor-traits-itinerary-types.inc: wattage-tblgen
	./run-tblgen.sh -gen-itinerary-enum > $@

LLVM_CXXFLAGS=$(shell $(llvm_config) --cxxflags)
LLVM_LIBDIR=$(shell $(llvm_config) --libdir)
LLVM_LDFLAGS=$(shell $(llvm_config) --ldflags)
LLVM_LIBS=$(shell $(llvm_config) --libs)
LLVM_INCLUDE_DIR=$(shell $(llvm_config) --includedir)
LLVM_INCLUDE_DIR_GENERATED="$(shell $(llvm_config) --obj-root)/include"

wattage-tblgen: tblgen/wattage-tblgen.cpp tblgen/intel-xed-opcodes.hpp
	g++ $< $(CXXFLAGS) $(LLVM_CXXFLAGS) -I$(LLVM_INCLUDE_DIR) -I$(LLVM_INCLUDE_DIR_GENERATED) $(LLVM_LIBS) -L$(LLVM_LIBDIR) $(LLVM_LDFLAGS) -fno-rtti -o $@

clean:
	rm wattage.so *.o wattage-tblgen processor-traits-itinerary-types.inc

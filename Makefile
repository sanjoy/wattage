include pintool.config

HEADERS=src/estimators.hpp src/processor-traits.hpp

ifeq ($(mode), release)
	CXXFLAGS=-Wall -Werror -O3 -DNDEBUG
else
	CXXFLAGS=-Wall -Werror -g3
endif

wattage.so: estimators.o pintool.o processor-traits.o
	$(PIN_LD) $(PIN_LDFLAGS) $(LINK_DEBUG) $^ $(PIN_LPATHS) $(PIN_LIBS) $(DBG) -o $@

%.o: src/%.cpp $(HEADERS)
	$(CXX) $(COPT) $(CXXFLAGS) $(PIN_CXXFLAGS) $< -o $@

clean:
	rm wattage.so estimators.o pintool.o
all:: wattage.so

DBG=-g
STD=-std=c++20

BGREPO := $(shell git rev-parse --show-toplevel)
BGSRCS := main GameState Battle Character Dice
BGOBJS := $(BGSRCS:%=$(BGREPO)/build/%.o)
BGTGT=$(BGREPO)/o
all: $(BGTGT)

# Temporary
$(BGTGT): $(BGOBJS) /usr/local/lib/libbicycle.a
	g++ $(DBG) $(STD) $(BGOBJS) -lbicycle $(shell ncursesw6-config --libs) -lfluidsynth -o $@

$(BGREPO)/build/%.o: $(BGREPO)/src/%.cpp 
	g++ $(DBG) $(STD) -c $< -Iinclude $(shell ncursesw6-config --cflags ) -o $@

$(BGREPO)/build/:
	mkdir -p build

.PHONY: clean
clean:
	  rm -rf $(BGREPO)/build/*.o
		rm -f $(BGREPO)/o

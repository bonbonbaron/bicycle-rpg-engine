DBG=-g
STD=-std=c++20

BGREPO := $(shell git rev-parse --show-toplevel)

BGSRCS := ports actions example-actions

BGOBJS := $(BGSRCS:%=$(BGREPO)/build/%.o)
BGTGT=$(BGREPO)/bin/o.so
all: $(BGTGT)

# Temporary
$(BGTGT): $(BGOBJS) 
	g++ -fPIC -shared $(DBG) $(STD) $(BGOBJS) -o $@

$(BGREPO)/build/%.o: $(BGREPO)/src/%.cpp $(BGREPO)/include/%.h 
	g++ -fPIC $(DBG) $(STD) -c $< -Iinclude -I/usr/local/include/ -o $@

$(BGREPO)/build/%.o: $(BGREPO)/src/%.cpp 
	g++ -fPIC $(DBG) $(STD) -c $< -Iinclude -I/usr/local/include/ -o $@

$(BGREPO)/build/:
	mkdir -p build

install: $(BGTGT)
	sudo cp $(BGTGT) /usr/local/games

.PHONY: clean
clean:
	  rm -rf $(BGREPO)/build/*.o
		rm -f $(BGREPO)/o

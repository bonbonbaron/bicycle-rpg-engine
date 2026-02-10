DBG=-g
STD=-std=c++20

BGREPO := $(shell git rev-parse --show-toplevel)

BGSRCS := blackboards ports actions example-actions 

BGOBJS := $(BGSRCS:%=$(BGREPO)/build/%.o)
BGTGT=$(BGREPO)/bin/o.so
all: $(BGTGT)

# Temporary
$(BGTGT): $(BGOBJS) 
	g++ -Wall -fPIC -shared $(DBG) $(STD) $(BGOBJS) -o $@

$(BGREPO)/build/%.o: $(BGREPO)/src/%.cpp $(BGREPO)/include/%.h 
	g++ -Wall -fPIC $(DBG) $(STD) -c $< -Iinclude -I/usr/local/include/ -o $@

$(BGREPO)/build/%.o: $(BGREPO)/src/%.cpp 
	g++ -Wall -fPIC $(DBG) $(STD) -c $< -Iinclude -I/usr/local/include/ -o $@

$(BGREPO)/build/:
	mkdir -p build

install: $(BGTGT)
	sudo cp $(BGTGT) /usr/local/games

.PHONY: clean
clean:
	  rm -rf $(BGREPO)/build/*.o
		rm -f $(BGREPO)/o

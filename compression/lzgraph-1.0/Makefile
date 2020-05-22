CPP=g++

#CPPFLAGS=-g3 -Wall -DVDEBUG -m32
#CPPFLAGS=-g3 -Wall -m32
#CPPFLAGS=-O9 -Wall -DNDEBUG -m32
#CPPFLAGS=-g3 -Wall -DVDEBUG 
#CPPFLAGS=-g3 -Wall
#CPPFLAGS=-O9 -Wall -DNDEBUG
CPPFLAGS=-O1

OBJECTS=BLArray.o LempelZiv.o bitrankw32int.o
EOBJECTS=BLArray.o bitrankw32int.o LZGraph.o
BINS=compressor use rebuild 

%.o: %.cpp
	@$(CPP) $(CPPFLAGS) -c $< -o $@
	@echo "[CPP] Compiling $<"

all: msg $(OBJECTS) $(EOBJECTS) $(BINS)

msg:
	@echo "[MSG] Using CPP=$(CPP)"
	@echo "[MSG] Using CPPFLAGS=$(CPPFLAGS)"
  
compressor: $(OBJECTS)
	@$(CPP) $(CPPFLAGS) -o compressor compressor.cpp $(OBJECTS)
	@echo "[LNK] Linking compressor"

use: $(EOBJECTS) use.cpp
	@$(CPP) $(CPPFLAGS) -o use use.cpp $(EOBJECTS)
	@echo "[LNK] Linking use"

rebuild: $(EOBJECTS)
	@$(CPP) $(CPPFLAGS) -o rebuild rebuild.cpp $(EOBJECTS)
	@echo "[LNK] Linking rebuild"

clean:
	@echo "[CLN] Removing binary files"
	@rm -f $(BINS) 
	@echo "[CLN] Removing object files"
	@rm -f $(OBJECTS) $(EOBJECTS)


CPP=g++
CPPFLAGS=-g3 -Wall 
#CPPFLAGS=-pg -Wall
#CPPFLAGS=-O9 -Wall -DNDEBUG 
OBJECTS=Heap.o HashTablePos.o HashTablePairs.o RePair.o bitrankw32int.o
EOBJECTS=bitrankw32int.o RepairGraph.o MultiGraph.o
BINS=buildIndex compressDictionary use rebuild cutGraph useMulti rebuildMulti


%.o: %.cpp
	@echo "[C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) -c $< -o $@

all: msg $(BINS)

msg:
	@echo "[MSG] Using CPP=$(CPP)"
	@echo "[MSG] Using CPPFLAGS=$(CPPFLAGS)"
	
buildIndex: $(OBJECTS)
	@echo "[LNK] Linking buildIndex"
	@$(CPP) $(CPPFLAGS) -o buildIndex buildIndex.cpp $(OBJECTS)

compressDictionary: $(OBJECTS)
	@echo "[LNK] Linking compressDictionary"
	@$(CPP) $(CPPFLAGS) -o compressDictionary compressDictionary.cpp $(OBJECTS)

use: $(EOBJECTS)
	@echo "[LNK] Linking use"
	@$(CPP) $(CPPFLAGS) -o use use.cpp $(EOBJECTS)
	
rebuild: $(EOBJECTS)
	@echo "[LNK] Linking rebuild"
	@$(CPP) $(CPPFLAGS) -o rebuild rebuild.cpp $(EOBJECTS)

useMulti: $(EOBJECTS)
	@echo "[LNK] Linking useMulti"
	@$(CPP) $(CPPFLAGS) -o useMulti useMulti.cpp $(EOBJECTS)	

rebuildMulti: $(EOBJECTS)
	@echo "[LNK] Linking rebuildMulti"
	@$(CPP) $(CPPFLAGS) -o rebuildMulti rebuildMulti.cpp $(EOBJECTS)

cutGraph: $(EOBJECTS)
	@echo "[LNK] Linking cutGraph"
	@$(CPP) $(CPPFLAGS) -o cutGraph cutGraph.cpp
	
clean:
	@echo "[CLN] Removing binary files"
	@rm -f $(BINS) 
	@echo "[CLN] Removing object files"
	@rm -f $(OBJECTS) 
	@rm -f $(EOBJECTS)

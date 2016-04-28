CXX = g++
CC = g++
FLAGS = -std=c++11

all:		main run

symbolTableEntries.o: symbolTableEntries.cpp symbolTableEntries.h
	$(CXX) $(FLAGS) -c symbolTableEntries.cpp

mncScan.o: mncScan.cpp mncScan.h
	$(CXX) $(FLAGS) -c mncScan.cpp

mncParser.o: mncParser.cpp mncParser.h
	$(CXX) $(FLAGS) -c mncParser.cpp

mncCode.o: mncCode.cpp mncCode.h symbolTableEntries.o
	$(CXX) $(FLAGS) -c mncCode.cpp

main.o: main.cpp mncScan.o mncParser.o mncCode.o symbolTableEntries.o
	$(CXX) $(FLAGS) -c main.cpp

main: main.o
	$(CXX) $(FLAGS) main.o mncScan.o mncParser.o mncCode.o symbolTableEntries.o -o testIt

run: main
	./testIt
clobber:
	rm -f *.o *.exe *.asm *.lst *.tok core testIt

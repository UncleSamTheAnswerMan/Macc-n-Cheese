CXX = g++
CC = g++
FLAGS = -std=c++11

all:		main run

mncScan.o: mncScan.cpp mncScan.h
	$(CXX) $(FLAGS) -c mncScan.cpp

mncScanTest.o: mncScanTest.cpp mncScan.h mncScan.cpp
	$(CXX) $(FLAGS) -c mncScanTest.cpp

main: mncScanTest.o mncScan.o
	$(CXX) $(FLAGS) mncScanTest.o mncScan.o -o testIt

run: main
	./testIt
clobber:
	rm -f *.o *.exe core testIt

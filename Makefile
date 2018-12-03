CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
INC=include
SRC=src

ROOTFLAG=`root-config --cflags --libs`
CPPFLAG=-I$(INC)

all: runRandomConePtSpectrum

RandomCone.o: $(SRC)/RandomCone.C
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CPPFLAG) $(ROOTFLAG)

runRandomConePtSpectrum: runRandomConePtSpectrum.C RandomCone.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CPPFLAG) $(ROOTFLAG)

.PHONY:
	clean
	all

clean:
	rm -f runRandomConePtSpectrum

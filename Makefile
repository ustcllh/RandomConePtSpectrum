CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
INC=include
SRC=src

ROOTFLAG=`root-config --cflags --libs`
CPPFLAG=-I$(INC)

all: runRandomConePtSpectrum

runRandomConePtSpectrum: runRandomConePtSpectrum.C
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CPPFLAG) $(ROOTFLAG)

.PHONY:
	clean
	all

clean:
	rm -f runRandomConePtSpectrum

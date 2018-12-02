CC=g++
INC=include
SRC=src

ROOTFLAG=`root-config --cflags --libs`
CPPFLAG=-I$(INC)

all: runRandomConePtSpectrum

runRandomConePtSpectrum: runRandomConePtSpectrum.C
	$(CC) -o $@ $^ $(CPPFLAG) $(ROOTFLAG)

.PHONY:
	clean
	all

clean:
	rm -f runRandomConePtSpectrum

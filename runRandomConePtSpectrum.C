#include "RandomCone.h"

#include <iostream>

int main(int argc, char* argv[]){
  if(argc!=4){
    std::cout << "./runRandomConePtSpectrum <inputfile> <outputfile> <useCentTab>" << std::endl;
    exit(-1);
  }
  return 0;
}

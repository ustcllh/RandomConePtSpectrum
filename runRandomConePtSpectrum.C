#include "RandomCone.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]){
  if(argc!=4){
    std::cout << "./runRandomConePtSpectrum <inputfile> <outputfile> <useCentTab>" << std::endl;
    exit(-1);
  }
  std::string input_str = std::string(argv[1]);
  std::string output_str = std::string(argv[2]);
  int useCentTab = atoi(argv[3]);

  RandomCone rc(input_str, output_str, useCentTab);
  rc.Init();
  rc.Execute();
  rc.End();
  return 0;
}

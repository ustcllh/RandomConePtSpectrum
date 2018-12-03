#ifndef __RANDOMCONE_H__
#define __RANDOMCONE_H__

#include <vector>
#include <string>

// root dependencies
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"

#define PI 3.1415926

struct randomcone{
  float eta;
  float phi;
  float pt;

  // constructor
  randomcone(float rcEta, float rcPhi): eta(rcEta), phi(rcPhi), pt(0){};
  // compare function
  bool operator<(randomcone other){
    return pt<other.pt;
  };
};

// cent tab
enum centTab{
  Data=0, Drum5F=1, Cymbal5F=2, Cymbal5Ev8=3
};

class RandomCone{
public:
  RandomCone(std::string& input, std::string& output, int& useCentTab){
    input_str = TString(input);
    output_str = TString(output);
    centTable_id = (centTab) useCentTab;
  };

  void Init();
  void Execute();
  void End();

  int gethiBinfromhiHF(float& EHF);

  float deltaR(float& eta1, float& phi1, float& eta2, float& phi2){
    float dr = std::sqrt(std::pow(eta1-eta2, 2) + std::pow(phi1-phi2, 2));
    return dr;
  };

  bool globalSelection(){
    return pprimaryVertexFilter && phfCoincFilter2Th4 && pclusterCompatibilityFilter;
  };

private:
  TString input_str;
  TString output_str;

  TFile* input_f;
  TFile* output_f;

  int nEvents;

  // random number generator
  TRandom3* rng;

  // random cone selection config
  static const int nrc;
  static const float etaMax;
  static const float etaMin;
  static const float phiMax;
  static const float phiMin;
  static const float rcSize;

  // pf cand tree
  TTree* pfcand_t;
  int nPFpart;
  std::vector<int>* pfId;
  std::vector<float>* pfPt;
  std::vector<float>* pfEta;
  std::vector<float>* pfPhi;

  // skim tree
  TTree* skim_t;
  int pprimaryVertexFilter;
  int phfCoincFilter2Th4;
  int pclusterCompatibilityFilter;

  // event tree
  TTree* evt_t;
  int hiBin;
  float hiHF;

  // output tree
  TTree* tout;
  float rcPt;
  float rcPhi;
  float rcEta;


  static const std::vector<std::string> centTabnames;
  centTab centTable_id;

  static const int nbinscentTab_data = 201;
  static const std::vector<float> centTab_data;

  static const int nbinscentTab_MC = 21;
  static const std::vector<float> centTab_MC_Drum5F;
  static const std::vector<float> centTab_MC_Cymbal5F;
  static const std::vector<float> centTab_MC_Cymbal5Ev8;


};



#endif

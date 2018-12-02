#ifndef __RANDOMCONE_H__
#define __RANDOMCONE_H__

#include <vector>
#include <string>

// root dependencies
#include "TString.h"
#include "TFile.h"
#include "TTree.h"

class RandomCone{
public:
  RandomCone(std::string& input, std::string& output, int& useCentTab){
    input_str = TString(input);
    output_str = TString(output);
    centTable_id = useCentTab;
  };

  void Init(){};
  void Execute(){};
  void End(){};

  int gethiBinfromhiHF(float EHF){return hiBin;};

  float deltaR(float eta1, float phi1, float eta2, float phi2){
    float dr = std::sqrt(std::pow(eta1-eta2, 2) + std::pow(phi1-phi2, 2));
    return dr;
  };

private:
  TString input_str;
  TString output_str;
  int centTable_id;

  TFile* input_f;
  TFile* output_f;

  int nEvents;

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

  static const int nbinscentTab_data = 201;
  const std::vector<float> centTab_data;

  static const int nbinscentTab_MC = 21;
  const std::vector<float> centTab_MC_Drum5F;
  const std::vector<float> centTab_MC_Cymbal5F;
  const std::vector<float> centTab_MC_Cymbal5Ev8;


};

const std::vector<float> RandomCone::centTab_data = {0, 11.4416, 12.1458, 12.7896, 13.4615, 14.0845, 14.7092, 15.3469, 16.0376, 16.686, 17.4067, 18.1092, 18.85, 19.6033, 20.3848, 21.1547, 21.9537, 22.7789, 23.6197, 24.5323, 25.5301, 26.5972, 27.6021, 28.6306, 29.6685, 30.8588, 32.1082, 33.3274, 34.5643, 35.9569, 37.2992, 38.7241, 40.22, 41.8977, 43.6106, 45.3499, 47.1777, 49.0889, 51.0998, 53.1554, 55.2419, 57.4508, 59.7639, 62.1935, 64.7132, 67.2647, 69.9241, 72.7119, 75.6242, 78.6836, 81.782, 85.0615, 88.4518, 91.9302, 95.5374, 99.261, 103.063, 107.193, 111.313, 115.654, 120.229, 125.022, 129.784, 134.883, 140.165, 145.465, 150.958, 156.669, 162.492, 168.484, 174.725, 181.162, 187.794, 194.748, 201.75, 209.116, 216.782, 224.48, 232.246, 240.531, 248.861, 257.506, 266.394, 275.631, 285.197, 294.963, 305.005, 315.031, 325.839, 336.69, 347.625, 358.913, 370.586, 382.313, 394.694, 407.319, 420.059, 432.928, 446.156, 459.654, 473.284, 487.875, 502.63, 517.402, 532.581, 548.157, 563.809, 580.161, 596.567, 613.599, 630.764, 648.503, 666.645, 685.007, 703.55, 722.396, 741.724, 761.388, 781.623, 801.823, 822.656, 844.047, 865.768, 887.74, 910.113, 933.149, 955.614, 979.289, 1003.03, 1027.94, 1053.22, 1078.65, 1104.92, 1131.32, 1158.85, 1186.17, 1213.87, 1242.34, 1270.73, 1300.3, 1330.7, 1360.76, 1391.64, 1422.88, 1454.82, 1487.43, 1520.32, 1554.38, 1589, 1623.8, 1658.38, 1693.92, 1729.78, 1765.83, 1802.72, 1839.96, 1877.58, 1916.96, 1956.59, 1997.45, 2039.41, 2081.63, 2124.22, 2166.32, 2210.45, 2255.92, 2302.07, 2347.31, 2394.12, 2443.6, 2492.33, 2541.85, 2592.7, 2643.83, 2696.46, 2750.58, 2804.85, 2861.01, 2918.08, 2975.71, 3034.69, 3094.43, 3155.59, 3219.19, 3284.14, 3350.76, 3417.36, 3486.82, 3557.98, 3630.9, 3706.12, 3780.93, 3860.1, 3942.22, 4027.03, 4114.06, 4203.13, 4298.1, 4398.53, 4524.65, 5166.42};

const std::vector<float> RandomCone::centTab_MC_Drum5F = {0, 19.340349, 29.62356, 44.732491, 67.390396, 103.568878, 156.940445, 225.768677, 328.517792, 451.307648, 601.051514, 806.734863, 1021.947327, 1316.270386, 1666.708984, 2081.608643, 2538.543213, 3090.073486, 3775.013428, 4608.974121, 50000};

const std::vector<float> RandomCone::centTab_MC_Cymbal5F = {0, 19.340349, 29.62356, 44.732491, 67.390396, 103.568878, 156.940445, 225.768677, 328.517792, 451.307648, 601.051514, 806.734863, 1021.947327, 1316.270386, 1666.708984, 2081.608643, 2538.543213, 3090.073486, 3775.013428, 4608.974121, 50000};

const std::vector<float> RandomCone::centTab_MC_Cymbal5Ev8 = {0, 14.085408, 17.91843, 23.530363, 34.372017, 54.325417, 80.162933, 119.569427, 174.41449, 249.967499, 344.683929, 461.308899, 619.797974, 808.329163, 1041.43396, 1327.012085, 1648.500854, 2058.486572, 2539.696289, 3145.067139, 50000};


#endif

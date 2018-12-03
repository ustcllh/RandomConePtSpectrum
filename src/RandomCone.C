#include "RandomCone.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TRandom3.h"


void RandomCone::Init(){
  input_f = new TFile(input_str, "READ");
  output_f = new TFile(output_str, "RECREATE");
  if(!input_f || !output_f){
    std::cout << "File pointer: Null!" << std::endl;
    exit(-1);
  }

  std::cout << "Init(): Processing " << input_str << std::endl;
  std::cout << "Init(): Output " << output_str << std::endl;
  std::cout << "Init(): centTab " << centTabnames[centTable_id] << std::endl;

  pfcand_t = (TTree*) input_f->Get("pfcandAnalyzer/pfTree");
  skim_t = (TTree*) input_f->Get("skimanalysis/HltTree");
  evt_t = (TTree*) input_f->Get("hiEvtAnalyzer/HiTree");

  if(!pfcand_t || !skim_t || !evt_t){
    std::cout << "Tree pointer: Null!" << std::endl;
    exit(-1);
  }

  nEvents = (int) evt_t->GetEntries();

  pfcand_t->SetBranchStatus("*", 0);
  pfcand_t->SetBranchStatus("nPFpart", 1);
  pfcand_t->SetBranchStatus("pfId", 1);
  pfcand_t->SetBranchStatus("pfPt", 1);
  pfcand_t->SetBranchStatus("pfEta", 1);
  pfcand_t->SetBranchStatus("pfPhi", 1);

  pfId = new std::vector<int>();
  pfPt = new std::vector<float>();
  pfPhi = new std::vector<float>();
  pfEta = new std::vector<float>();

  pfcand_t->SetBranchAddress("nPFpart", &nPFpart);
  pfcand_t->SetBranchAddress("pfId", &pfId);
  pfcand_t->SetBranchAddress("pfPt", &pfPt);
  pfcand_t->SetBranchAddress("pfEta", &pfEta);
  pfcand_t->SetBranchAddress("pfPhi", &pfPhi);

  skim_t->SetBranchStatus("*", 0);
  skim_t->SetBranchStatus("pprimaryVertexFilter", 1);
  skim_t->SetBranchStatus("phfCoincFilter2Th4", 1);
  skim_t->SetBranchStatus("pclusterCompatibilityFilter", 1);
  skim_t->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);
  skim_t->SetBranchAddress("phfCoincFilter2Th4", &phfCoincFilter2Th4);
  skim_t->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter);

  evt_t->SetBranchStatus("*", 0);
  evt_t->SetBranchStatus("hiBin", 1);
  evt_t->SetBranchStatus("hiHF", 1);
  evt_t->SetBranchStatus("hiBin", &hiBin);
  evt_t->SetBranchStatus("hiHF", &hiHF);

  tout = new TTree("randomCone", "randomCone");
  tout->Branch("hiBin", &hiBin, "hiBin/I");
  tout->Branch("rcPt", &rcPt, "rcPt/F");
  tout->Branch("rcEta", &rcEta, "rcEta/F");
  tout->Branch("rcPhi", &rcPhi, "rcPhi/F");

  rng = new TRandom3();

  std::cout << "Init(): nEvents " << nEvents << std::endl;
  std::cout << "Init(): Initialization Complete! " << nEvents << std::endl;

}

void RandomCone::Execute(){
  std::vector<randomcone> rc_v;
  for(int ievt=0; ievt<nEvents; ievt++){
    rc_v.clear();

    pfcand_t->GetEntry(ievt);
    skim_t->GetEntry(ievt);
    evt_t->GetEntry(ievt);

    if(!globalSelection()) continue;

    hiBin = gethiBinfromhiHF(hiHF);

    for(int i=0; i<nrc; i++){
      float eta = (float) rng->Rndm()*(etaMax-etaMin) + etaMin;
      float phi = (float) rng->Rndm()*(phiMax-phiMin) + phiMin;
      rc_v.emplace_back(randomcone(eta, phi));
    }

    for(auto&& rc : rc_v){
      for(int i=0; i<nPFpart; i++){
        float dr = deltaR(rc.eta, rc.phi, pfEta->at(i), pfPhi->at(i));
        if(dr<rcSize){
          rc.pt += pfPt->at(i);
        }
      }
    }

    std::sort(rc_v.begin(), rc_v.end());

    // median index
    int midx = nrc/2 + 1;
    rcEta = rc_v[midx].eta;
    rcPhi = rc_v[midx].phi;
    rcPt = rc_v[midx].pt;

    tout->Fill();
  }

  std::cout << "Execute(): Execution Complete!" << std::endl;
}

void RandomCone::End(){
  delete pfId;
  delete pfPt;
  delete pfEta;
  delete pfPhi;
  delete rng;

  output_f->cd();
  tout->Write();
  input_f->Close();
  output_f->Close();

  std::cout<< "End(): Process End!" << std::endl;

}

int RandomCone::gethiBinfromhiHF(float& EHF){
  int binpos = 0;
  switch(centTable_id){
    case Data:
      binpos = std::upper_bound(centTab_data.begin(), centTab_data.end(), EHF) - centTab_data.begin();
      return 200-binpos;
    case Drum5F:
      binpos = std::upper_bound(centTab_MC_Drum5F.begin(), centTab_MC_Drum5F.end(), EHF) - centTab_MC_Drum5F.begin();
      binpos = nbinscentTab_MC - 1 - binpos;
      return (int) (200.*((double)binpos)/((double)nbinscentTab_MC));
    case Cymbal5F:
      binpos = std::upper_bound(centTab_MC_Cymbal5F.begin(), centTab_MC_Cymbal5F.end(), EHF) - centTab_MC_Cymbal5F.begin();
      binpos = nbinscentTab_MC - 1 - binpos;
      return (int) (200.*((double)binpos)/((double)nbinscentTab_MC));
    case Cymbal5Ev8:
      binpos = std::upper_bound(centTab_MC_Cymbal5Ev8.begin(), centTab_MC_Cymbal5Ev8.end(), EHF) - centTab_MC_Cymbal5Ev8.begin();
      binpos = nbinscentTab_MC - 1 - binpos;
      return (int) (200.*((double)binpos)/((double)nbinscentTab_MC));
  }
}

// random cone selection config
const int RandomCone::nrc = 23;
const float RandomCone::etaMax = 1.3;
const float RandomCone::etaMin = -1.3;
const float RandomCone::phiMax = PI;
const float RandomCone::phiMin = -PI;
const float RandomCone::rcSize = 0.4;

// centrality tables
const std::vector<std::string> RandomCone::centTabnames = {"Data", "Drum5F", "Cymbal5F", "Cymbal5Ev8"};

const std::vector<float> RandomCone::centTab_data = {0, 11.4416, 12.1458, 12.7896, 13.4615, 14.0845, 14.7092, 15.3469, 16.0376, 16.686, 17.4067, 18.1092, 18.85, 19.6033, 20.3848, 21.1547, 21.9537, 22.7789, 23.6197, 24.5323, 25.5301, 26.5972, 27.6021, 28.6306, 29.6685, 30.8588, 32.1082, 33.3274, 34.5643, 35.9569, 37.2992, 38.7241, 40.22, 41.8977, 43.6106, 45.3499, 47.1777, 49.0889, 51.0998, 53.1554, 55.2419, 57.4508, 59.7639, 62.1935, 64.7132, 67.2647, 69.9241, 72.7119, 75.6242, 78.6836, 81.782, 85.0615, 88.4518, 91.9302, 95.5374, 99.261, 103.063, 107.193, 111.313, 115.654, 120.229, 125.022, 129.784, 134.883, 140.165, 145.465, 150.958, 156.669, 162.492, 168.484, 174.725, 181.162, 187.794, 194.748, 201.75, 209.116, 216.782, 224.48, 232.246, 240.531, 248.861, 257.506, 266.394, 275.631, 285.197, 294.963, 305.005, 315.031, 325.839, 336.69, 347.625, 358.913, 370.586, 382.313, 394.694, 407.319, 420.059, 432.928, 446.156, 459.654, 473.284, 487.875, 502.63, 517.402, 532.581, 548.157, 563.809, 580.161, 596.567, 613.599, 630.764, 648.503, 666.645, 685.007, 703.55, 722.396, 741.724, 761.388, 781.623, 801.823, 822.656, 844.047, 865.768, 887.74, 910.113, 933.149, 955.614, 979.289, 1003.03, 1027.94, 1053.22, 1078.65, 1104.92, 1131.32, 1158.85, 1186.17, 1213.87, 1242.34, 1270.73, 1300.3, 1330.7, 1360.76, 1391.64, 1422.88, 1454.82, 1487.43, 1520.32, 1554.38, 1589, 1623.8, 1658.38, 1693.92, 1729.78, 1765.83, 1802.72, 1839.96, 1877.58, 1916.96, 1956.59, 1997.45, 2039.41, 2081.63, 2124.22, 2166.32, 2210.45, 2255.92, 2302.07, 2347.31, 2394.12, 2443.6, 2492.33, 2541.85, 2592.7, 2643.83, 2696.46, 2750.58, 2804.85, 2861.01, 2918.08, 2975.71, 3034.69, 3094.43, 3155.59, 3219.19, 3284.14, 3350.76, 3417.36, 3486.82, 3557.98, 3630.9, 3706.12, 3780.93, 3860.1, 3942.22, 4027.03, 4114.06, 4203.13, 4298.1, 4398.53, 4524.65, 5166.42};

const std::vector<float> RandomCone::centTab_MC_Drum5F = {0, 19.340349, 29.62356, 44.732491, 67.390396, 103.568878, 156.940445, 225.768677, 328.517792, 451.307648, 601.051514, 806.734863, 1021.947327, 1316.270386, 1666.708984, 2081.608643, 2538.543213, 3090.073486, 3775.013428, 4608.974121, 50000};

const std::vector<float> RandomCone::centTab_MC_Cymbal5F = {0, 19.340349, 29.62356, 44.732491, 67.390396, 103.568878, 156.940445, 225.768677, 328.517792, 451.307648, 601.051514, 806.734863, 1021.947327, 1316.270386, 1666.708984, 2081.608643, 2538.543213, 3090.073486, 3775.013428, 4608.974121, 50000};

const std::vector<float> RandomCone::centTab_MC_Cymbal5Ev8 = {0, 14.085408, 17.91843, 23.530363, 34.372017, 54.325417, 80.162933, 119.569427, 174.41449, 249.967499, 344.683929, 461.308899, 619.797974, 808.329163, 1041.43396, 1327.012085, 1648.500854, 2058.486572, 2539.696289, 3145.067139, 50000};

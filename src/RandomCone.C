#include "RandomConePtSpectrum.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TRandom3.h"


RandomCone::Init(){
  input_f = TFile(input_str, "READ");
  output_f = TFile(output_str, "RECREATE");
  if(!input || !output){
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
  tout->SetBranchAddress("hiBin", &hiBin, "hiBin/I");
  tout->SetBranchAddress("rcPt", &rcpt, "rcPt/F");
  tout->SetBranchAddress("rcEta", &rcEta, "rcEta/F");
  tout->SetBranchAddress("rcPhi", &rcPhi, "rcPhi/F");

  rng = new TRandom3();

  std::cout << "Init(): nEvents " << nEvents << std::endl;
  std::cout << "Init(): Initialization Complete! " << nEvents << std::endl;

};

void RandomCone::Execute(){
  std::vector<randomcone> rc_v;
  for(int ievt=0; ievt<nEvents; ievt++){
    rc_v->clear();

    pfcand_t->GetEntry(ievt);
    skim_t->GetEntry(ievt);
    evt_t->GetEntry(ievt);

    if(!globalSelection) continue;

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
    rcEta = rc_v[midx];
    rcPhi = rc_v[midx];
    rcPt = rc_v[midx];

    tout->Fill();
  }

  std::cout << "Execute(): Execution Complete!" << std::endl;
};

void RandomCone::End(){
  delete pfId;
  delete pfPt;
  delete pfEta;
  delete pfPhi;
  delete rng;

  output_f->cd();
  tout->Write();
  input_f->Close();
  output_f->Close()

  std::cout<< "End(): Process End!" << std::endl;

};

int RandomCone::gethiBinfromhiHF(float& EHF){
  int binpos = 0;
  switch(centTab){
    case 0:
      auto it =  std::upper_bound(centTab_data.begin(), centTab_data.end(), EHF);
      binpos = it - centTab_data.begin();
      return 200-binpos;
    case 1:
      auto it = std::upper_bound(centTab_MC_Drum5F.begin(), centTab_MC_Drum5F.end(), EHF);
      binpos = it - centTab_MC_Drum5F.begin();
      binpos = nbinscentTab_MC - 1 - binpos;
      return (int) (200.*((double)binPos)/((double)nbinscentTab_MC);
    case 2:
      auto it = std::upper_bound(centTab_MC_Cymbal5F.begin(), centTab_MC_Cymbal5F.end(), EHF);
      binpos = it - centTab_MC_Cymbal5F.begin();
      binpos = nbinscentTab_MC - 1 - binpos;
      return (int) (200.*((double)binPos)/((double)nbinscentTab_MC);
    case 3:
      auto it = std::upper_bound(centTab_MC_Cymbal5Ev8.begin(), centTab_MC_Cymbal5Ev8.end(), EHF);
      binpos = it - centTab_MC_Cymbal5Ev8.begin();
      binpos = nbinscentTab_MC - 1 - binpos;
      return (int) (200.*((double)binPos)/((double)nbinscentTab_MC);
  }
};

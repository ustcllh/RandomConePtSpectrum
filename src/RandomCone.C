#include "RandomConePtSpectrum.h"

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"


RandomCone::Init(){
  input_f = TFile(input_str, "READ");
  output_f = TFile(output_str, "RECREATE");
  if(!input || !output){
    std::cout << "File pointer: Null!" << std::endl;
    exit(-1);
  }

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
  tout->SetBranchAddress("rcPt", &rcpt, "rcPt/F");
  tout->SetBranchAddress("rcEta", &rcEta, "rcEta/F");
  tout->SetBranchAddress("rcPhi", &rcPhi, "rcPhi/F");

}

int plot(){
	gStyle->SetOptStat(0);
	TString data_str = "../RandomCone_data.root";
	TString mc1_str = "../RandomCone_Cymbal5F.root";
	TString mc2_str = "../RandomCone_Cymbal5Ev8.root";
	TString mc3_str = "../RandomCone_Drum5F.root";

	TFile* data_f = new TFile(data_str, "READ");
	TFile* mc1_f = new TFile(mc1_str, "READ");
	TFile* mc2_f = new TFile(mc2_str, "READ");
	TFile* mc3_f = new TFile(mc3_str, "READ");

	TTree* data_t = (TTree*) data_f->Get("randomCone");
	TTree* mc1_t = (TTree*) mc1_f->Get("randomCone");
	TTree* mc2_t = (TTree*) mc2_f->Get("randomCone");
	TTree* mc3_t = (TTree*) mc3_f->Get("randomCone");

	data_t->Draw("rcPt:hiBin>>data_h2(100, 0,200, 100, 0, 200)", "", "");
	mc1_t->Draw("rcPt:hiBin>>mc1_h2(100, 0,200, 100, 0, 200)", "", "");
	mc2_t->Draw("rcPt:hiBin>>mc2_h2(100, 0,200, 100, 0, 200)", "", "");
	mc3_t->Draw("rcPt:hiBin>>mc3_h2(100, 0,200, 100, 0, 200)", "", "");

	TH2D* data_h2 = (TH2D*) gDirectory->Get("data_h2");
	TH2D* mc1_h2 = (TH2D*) gDirectory->Get("mc1_h2");
	TH2D* mc2_h2 = (TH2D*) gDirectory->Get("mc2_h2");
	TH2D* mc3_h2 = (TH2D*) gDirectory->Get("mc3_h2");
/*	
	data_h1->Scale(1./data_h1->Integral());
	mc1_h1->Scale(1./mc1_h1->Integral());
	mc2_h1->Scale(1./mc2_h1->Integral());
	mc3_h1->Scale(1./mc3_h1->Integral());

    data_h1->SetMarkerStyle(20);
    data_h1->SetMarkerColor(1);
    data_h1->SetLineColor(1);
    mc1_h1->SetMarkerStyle(21);
    mc1_h1->SetMarkerColor(2);
    mc1_h1->SetLineColor(2);
    mc2_h1->SetMarkerStyle(22);
    mc2_h1->SetMarkerColor(3);
    mc2_h1->SetLineColor(3);
    mc3_h1->SetMarkerStyle(23);
    mc3_h1->SetMarkerColor(4);
    mc3_h1->SetLineColor(4);
    data_h1->SetMarkerSize(0.5);
    mc1_h1->SetMarkerSize(0.5);
    mc2_h1->SetMarkerSize(0.5);
    mc3_h1->SetMarkerSize(0.5);

	TLegend* lg = new TLegend(0.6, 0.7, 0.9, 0.9);
	lg->AddEntry(data_h1, "Data Run 326587");
	lg->AddEntry(mc1_h1, "MC_Cymbal5F");
	lg->AddEntry(mc2_h1, "MC_Cymbal5Ev8");
	lg->AddEntry(mc3_h1, "MC_Drum5F");
*/
	data_h2->SetTitle("Data Run326587 #eta~[-1.3,1.3];hiBin;RC Pt");
	mc1_h2->SetTitle("MC Cymbal5F #eta~[-1.3,1.3];hiBin;RC Pt");
	mc2_h2->SetTitle("MC Cymbal5Ev8 #eta~[-1.3,1.3];hiBin;RC Pt");
	mc3_h2->SetTitle("MC Drum5F #eta~[-1.3,1.3];hiBin;RC Pt");
	TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
	c1->Divide(2,2);
	c1->cd(1);
	data_h2->Draw("colz");
	c1->cd(2);
	mc1_h2->Draw("colz");
	c1->cd(3);
	mc2_h2->Draw("colz");
	c1->cd(4);
	mc3_h2->Draw("colz");

	c1->SaveAs("RCPtSpectrum.pdf");

	TProfile* data_tp = (TProfile*) data_h2->ProfileX("RCPtCentEta_Data", 1, -1, "s");
	TProfile* mc1_tp = (TProfile*) mc1_h2->ProfileX("RCPtCentEta_MC_Cymal5F", 1, -1, "s");
	TProfile* mc2_tp = (TProfile*) mc2_h2->ProfileX("RCPtCentEta_MC_Cymal5Ev8", 1, -1, "s");
	TProfile* mc3_tp = (TProfile*) mc3_h2->ProfileX("RCPtCentEta_MC_Drum5F", 1, -1, "s");

	data_tp->GetXaxis()->SetRangeUser(0,200);
	data_tp->GetYaxis()->SetRangeUser(0,200);
	mc1_tp->GetXaxis()->SetRangeUser(0,200);
	mc1_tp->GetYaxis()->SetRangeUser(0,200);
	mc2_tp->GetXaxis()->SetRangeUser(0,200);
	mc2_tp->GetYaxis()->SetRangeUser(0,200);
	mc3_tp->GetXaxis()->SetRangeUser(0,200);
	mc3_tp->GetYaxis()->SetRangeUser(0,200);

	TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
	c2->Divide(2,2);
	c2->cd(1);
	data_tp->Draw();
	c2->cd(2);
	mc1_tp->Draw();
	c2->cd(3);
	mc2_tp->Draw();
	c2->cd(4);
	mc3_tp->Draw();
	c2->SaveAs("RCPtSpectrumProfile.pdf");

	TProfile* mc1_r = (TProfile*) mc1_tp->Clone();
	TProfile* mc2_r = (TProfile*) mc2_tp->Clone();
	TProfile* mc3_r = (TProfile*) mc3_tp->Clone();

	mc1_r->Divide(data_tp);
	mc2_r->Divide(data_tp);
	mc3_r->Divide(data_tp);

    TLegend *lgr = new TLegend(0.4, 0.7, 0.9, 0.9);
    lgr->AddEntry(mc1_r, "MC_Cymbal5F / Data");
    lgr->AddEntry(mc2_r, "MC_Cymbal5Ev8 / Data");
    lgr->AddEntry(mc3_r, "MC_Drum5F / Data");
	
	TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
    mc1_r->SetMarkerStyle(21);
    mc1_r->SetMarkerColor(2);
    mc1_r->SetLineColor(2);
    mc2_r->SetMarkerStyle(22);
    mc2_r->SetMarkerColor(3);
    mc2_r->SetLineColor(3);
    mc3_r->SetMarkerStyle(23);
    mc3_r->SetMarkerColor(4);
    mc3_r->SetLineColor(4);
    mc1_r->SetMarkerSize(0.5);
    mc2_r->SetMarkerSize(0.5);
    mc3_r->SetMarkerSize(0.5);
	
	mc1_r->GetYaxis()->SetRangeUser(0.5, 2);
	mc1_r->SetTitle("RCPt MC/Data  #eta~[-1.3,1.3]; hiBin;");
	mc1_r->Draw("hist p");
	mc2_r->Draw("hist p same");
	mc3_r->Draw("hist p same");
	lgr->Draw("same");

	c3->SaveAs("RCPtSpectrumProfileRatio.pdf");
	
	return 0;

}

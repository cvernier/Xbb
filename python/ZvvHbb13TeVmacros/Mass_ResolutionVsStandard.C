#include <algorithm>    // std::max

void Mass_ResolutionVsStandard(){
gROOT->SetBatch();
gStyle->SetPadGridX(1);
gStyle->SetPadGridY(1);
gStyle->SetOptStat(0);
gStyle->SetLineWidth(2);
gROOT->ForceStyle();

TChain* tree = new TChain("tree");
//tree->Add("/scratch/sdonato/VHbbRun2/V14_forPreApproval/CMSSW_7_4_7_patch1/src/Xbb/env/syst/ZvvHighPt_V15_ZH_HToBB_ZToNuNu_M125_13TeV_amcatnloFXFX_madspin_pythia8.root");
//tree->Add("../env/syst/ZvvHighPt_V21_ZH_HToBB_ZToNuNu_M125_13TeV_amcatnloFXFX_madspin_pythia8.root");
tree->Add("../../env/ZvvHighPt_V21_ZH_HToBB_ZToNuNu_M125_13TeV_amcatnloFXFX_madspin_pythia8.root");

TCanvas* c1 = new TCanvas("c1","c1",1280,768);

const char* preselection = "Sum$(Jet_mcMatchId[hJCidx]==25)==2";
//const char* preselection = "1";
tree->Draw("HCSV_mass >> Hmass(40,80,160)",preselection,"prof");
tree->Draw("1 >> HmassReg(40,80,160)",preselection,"prof");
tree->Draw("1 >> HFSRmassReg(40,80,160)",preselection,"prof");
tree->Draw("HCSV_reg_mass >> HmassStdReg(40,80,160)",preselection,"prof");

Hmass = (TH1F*) gDirectory->Get("Hmass");
HmassReg = (TH1F*) gDirectory->Get("HmassReg");
HFSRmassReg = (TH1F*) gDirectory->Get("HFSRmassReg");
HmassStdReg = (TH1F*) gDirectory->Get("HmassStdReg");


Hmass->SetLineWidth(2);
HmassReg->SetLineWidth(2);
HFSRmassReg->SetLineWidth(2);
HmassStdReg->SetLineWidth(2);


Hmass->SetLineColor(kBlack);
HmassReg->SetLineColor(kRed);
HFSRmassReg->SetLineColor(kGreen);
HmassStdReg->SetLineColor(kBlue);

maxim = max(max(Hmass->GetMaximum(),HFSRmassReg->GetMaximum()),max(HmassReg->GetMaximum(),HmassStdReg->GetMaximum()));

Hmass->SetMaximum(maxim*1.1);
Hmass->SetTitle("Reconstructed Higgs mass distribution");
Hmass->GetXaxis()->SetTitle("m(bb) [GeV]");
Hmass->GetYaxis()->SetTitle("Events");


Hmass->Draw("");
HmassReg->Draw("same");
//HFSRmassReg->Draw("same");
HmassStdReg->Draw("same");

TLegend* leg = new TLegend(0.1,0.7,0.35,0.9);
//leg->SetHeader("The Legend Title");
leg->AddEntry(Hmass,"baseline","l");
leg->AddEntry(HmassReg,"regressed","l");
leg->AddEntry(HmassStdReg,"std. regression","l");
//leg->AddEntry(HFSRmassReg,"withFSR+regr.","l");
leg->Draw();

//TF1* gaus2 = new TF1("gaus2","gaus");
//TF1* gaus1 = new TF1("gaus1","gaus");
//const char* function = " gaus(0)*abs(x>[1])+gaus(3)*abs(x<=[1])";
const char* function = " [0]*exp(-0.5*((x-[1])/[2])**2)*abs(x<=[1]) + [0]*exp(-0.5*((x-[1])/[3])**2)*abs(x>[1])+[4]";

TF1* fit1 = new TF1("fit1",function);
TF1* fit2 = new TF1("fit2",function);
TF1* fit3 = new TF1("fit3",function);
TF1* fit4 = new TF1("fit4",function);

fit1->SetParameters(6000,125,20,20,0);
fit2->SetParameters(6000,125,20,20,0);
fit3->SetParameters(6000,125,20,20,0);
fit4->SetParameters(6000,125,20,20,0);

fit1->SetLineColor(Hmass->GetLineColor());
fit2->SetLineColor(HmassReg->GetLineColor());
fit3->SetLineColor(HFSRmassReg->GetLineColor());
fit4->SetLineColor(HmassStdReg->GetLineColor());

Hmass->Fit(fit1);
HmassReg->Fit(fit2);
HFSRmassReg->Fit(fit3);
HmassStdReg->Fit(fit4);

fit1->SetRange(fit1->GetParameter(1)-fit1->GetParameter(2),fit1->GetParameter(1)+fit1->GetParameter(3));
fit2->SetRange(fit2->GetParameter(1)-fit2->GetParameter(2),fit2->GetParameter(1)+fit2->GetParameter(3));
fit3->SetRange(fit3->GetParameter(1)-fit3->GetParameter(2),fit3->GetParameter(1)+fit3->GetParameter(3));
fit4->SetRange(fit4->GetParameter(1)-fit4->GetParameter(2),fit4->GetParameter(1)+fit4->GetParameter(3));
const float sigmap = 4;
const float sigmam = 2;

cout<<endl<< "Fitting Hmass"<<endl;
Hmass->Fit(fit1,"","",fit1->GetParameter(1)-sigmam*fit1->GetParameter(2),fit1->GetParameter(1)+sigmap*fit1->GetParameter(2));
cout<<endl<< "Fitting HmassReg"<<endl;
HmassReg->Fit(fit2,"","",fit2->GetParameter(1)-sigmam*fit2->GetParameter(2),fit2->GetParameter(1)+sigmap*fit2->GetParameter(2));
cout<<endl<< "Fitting HFSRmassReg"<<endl;
HFSRmassReg->Fit(fit3,"","",fit3->GetParameter(1)-sigmam*fit3->GetParameter(2),fit3->GetParameter(1)+sigmap*fit3->GetParameter(2));
cout<<endl<< "Fitting HmassStdReg"<<endl;
HmassStdReg->Fit(fit4,"","",fit4->GetParameter(1)-sigmam*fit4->GetParameter(2),fit4->GetParameter(1)+sigmap*fit4->GetParameter(2));

Hmass->Draw("");
HmassReg->Draw("same");
//HFSRmassReg->Draw("same");
HmassStdReg->Draw("same");

leg->Draw();

c1->SaveAs("mass_resolution.png");
c1->SaveAs("mass_resolution.C");
}


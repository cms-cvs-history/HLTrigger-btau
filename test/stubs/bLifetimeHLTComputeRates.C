#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

// If true make efficiency plots. Otherwise make rate plots.
const bool writeEffiPlots = false;

// Which data to look at ? (Choose only one !).
const bool STUDY_MB = false;
const bool STUDY_TT = false;
const bool STUDY_QCD = true;

const unsigned int NFILES = 15;
const unsigned int NJETS = 4;
const Double_t luminosity = 1E5; //In mb-1 s-1

TH1F* MakeEffiPlot(bool integrate, TFile* file, string name, Double_t nL1pass){

  TH1F* his = (TH1F*) file->Get(name.c_str());
  
  Int_t nbins = his->GetNbinsX();
  Double_t integral = 0.;
  
  for (Int_t i = nbins + 1; i >= 1; i--) {
    
    Double_t thisBin = his->GetBinContent(i);
    Double_t effi;
    if (integrate) {
      integral += thisBin;
      effi = integral / nL1pass;
    } else {
      effi = thisBin / nL1pass;
    }
    
    Double_t effi_err;
    if (effi <= 1) {
      effi_err = sqrt(effi*(1.-effi)/nL1pass);
    } else if (effi <= 0. || effi >= 1.) {
      effi_err = 1/nL1pass;
    } else {
      //      cout<<"warning: effi > 1: "<<his->GetName()<<" "<<effi<<" "<<nL1pass<<endl;
      effi_err = 0.;
    }
    
    his->SetBinContent(i, effi);
    his->SetBinError(i, effi_err);
  }
  //  cout << " Effi (Hz) with no cut on "<< his->GetName() << " = " << his->GetBinContent(1) << endl;

  return his;
}

void SumHisto(bool reset, Double_t L1rate, const TH1F* his, TH1F*& new_his) {
  if (reset) {
    string new_name = "_" + string(his->GetName());
    new_his = (TH1F*) his->Clone(new_name.c_str());
    new_his->Reset(); 
    new_his->Sumw2();
  }

  new_his->Add(his, L1rate);
  //  cout<<"Entries "<<his->GetEntries()<<" "<<new_his->GetEntries()<<endl;
}

void Data_tt(vector<TFile*>& inFiles, vector<Double_t>& crossSection, 
             vector<Double_t>& L1skimEff) {

  const unsigned int nFiles = 1;
  
  inFiles      = vector<TFile*>(nFiles, 0);
  crossSection = vector<Double_t>(nFiles);
  L1skimEff    = vector<Double_t>(nFiles);

  inFiles[0] = new TFile("bHLTanal_tt.root");

  // hadronic tt cross-section in mb. - unknown ...
  crossSection[0]  = 4.44e-7;

  // hadronic tt L1 skim efficiency
  L1skimEff[0]  = 1. ;
}

void Data_mb(vector<TFile*>& inFiles, vector<Double_t>& crossSection, 
             vector<Double_t>& L1skimEff) {

  const unsigned int nFiles = 1;
  
  inFiles      = vector<TFile*>(nFiles, 0);
  crossSection = vector<Double_t>(nFiles);
  L1skimEff    = vector<Double_t>(nFiles);

  inFiles[0] = new TFile("bHLTanal_mb.root");

  // Minibias cross-section in mb.
  crossSection[0]  = 7.92E+1;

  // Minibias V4 L1 skim efficiency
  L1skimEff[0]  = 0.0045 ;
}

void Data_QCD(vector<TFile*>& inFiles, vector<Double_t>& crossSection, 
              vector<Double_t>& L1skimEff) {

  const unsigned int nFiles = 14;
  
  inFiles      = vector<TFile*>(nFiles, 0);
  crossSection = vector<Double_t>(nFiles);
  L1skimEff    = vector<Double_t>(nFiles);

  //  inFiles[0] = new TFile("bHLTanal_qq_0_15.root");
  inFiles[1] = new TFile("bHLTanal_qq_15_20.root");
  inFiles[2] = new TFile("bHLTanal_qq_20_30.root");
  inFiles[3] = new TFile("bHLTanal_qq_30_50.root");
  inFiles[4] = new TFile("bHLTanal_qq_50_80.root");
  inFiles[5] = new TFile("bHLTanal_qq_80_120.root");
  inFiles[6] = new TFile("bHLTanal_qq_120_170.root");
  inFiles[7] = new TFile("bHLTanal_qq_170_230.root");
  inFiles[8] = new TFile("bHLTanal_qq_230_300.root");
  inFiles[9] = new TFile("bHLTanal_qq_300_380.root");
  inFiles[10] = new TFile("bHLTanal_qq_380_470.root");
  inFiles[11] = new TFile("bHLTanal_qq_470_600.root");
  inFiles[12] = new TFile("bHLTanal_qq_600_800.root");
  inFiles[13] = new TFile("bHLTanal_qq_800_1000.root");
 
  // QCD jet cross-section in mb.
  crossSection[0]  = 5.52E+1 ;
  crossSection[1]  = 1.46 ;
  crossSection[2]  = 6.32E-1 ;
  crossSection[3]  = 1.63E-1 ;
  crossSection[4]  = 2.16E-2 ;
  crossSection[5]  = 3.08E-3 ;
  crossSection[6]  = 4.94E-4 ;
  crossSection[7]  = 1.01E-4 ;
  crossSection[8]  = 2.45E-5 ;
  crossSection[9]  = 6.24E-6 ;
  crossSection[10] = 1.78E-6 ;
  crossSection[11] = 6.83E-7 ;
  crossSection[12] = 2.04E-7 ;
  crossSection[13] = 3.51E-8 ;

  // QCD V4 L1 skim efficiency
  L1skimEff[0]  = 0.00284 ;   // v5
  L1skimEff[1]  = 0.060 ;
  L1skimEff[2]  = 0.134 ;
  L1skimEff[3]  = 0.348 ;
  L1skimEff[4]  = 0.763 ;
  L1skimEff[5]  = 0.980 ;
  L1skimEff[6]  = 1.0 ;
  L1skimEff[7]  = 1.0 ;
  L1skimEff[8]  = 1.0 ;
  L1skimEff[9]  = 1.0 ;
  L1skimEff[10] = 1.0 ;
  L1skimEff[11] = 1.0 ;
  L1skimEff[12] = 1.0 ;
  L1skimEff[13] = 1.0 ;
}

void bLifetimeHLTComputeRates(){

  TFile* fout = new TFile("HLTrates.root","RECREATE");

  // Open data files
  vector<TFile*> inFiles;
  vector<Double_t> crossSection;
  vector<Double_t> L1skimEff;
  if (STUDY_MB) {
    Data_mb(inFiles, crossSection, L1skimEff);    
  } else if (STUDY_TT) {
    Data_tt(inFiles, crossSection, L1skimEff);    
  } else if (STUDY_QCD) {
    Data_QCD(inFiles, crossSection, L1skimEff);    
  }
    
  fout->cd();

  TH1F* _L1;
  TH1F* _HLT;
  TH1F* _Et;
  TH1F* _Jets[NJETS];
  TH1F* _bJetsL25[NJETS];
  TH1F* _bJetsL3[NJETS];

  bool first = true;
  Double_t totalInputRate = 0.;
  Double_t totalL1Rate    = 0.;

  for (unsigned int i = 0; i < inFiles.size(); i++){

    if (inFiles[i] == 0) continue;

    cout << "LOOP " << i <<" : ";
   
    // Look at trigger rates.  

    TH1F* L1info = (TH1F*) inFiles[i]->Get("L1");
    Double_t nEvents   = L1info->GetBinContent(10);
    Double_t nL1pass   = L1info->GetBinContent(1);

    TH1F* HLTinfo = (TH1F*) inFiles[i]->Get("HLT");
    Double_t nHLTpass  = HLTinfo->GetBinContent(1);
    Double_t L1eff     = L1skimEff[i]*nL1pass/nEvents;
    Double_t inputRate = crossSection[i] * luminosity; 
    Double_t L1rate    = inputRate * L1eff;
    totalInputRate += inputRate;
    totalL1Rate    += L1rate; 

    Double_t HLTeff    = nHLTpass/nL1pass;
    Double_t HLTrate   = inputRate*L1eff*HLTeff;
    Double_t HLTrate_err;
    if (nHLTpass > 0) {
      HLTrate_err = HLTrate/sqrt(nHLTpass);
    } else {
      HLTrate_err = inputRate*L1eff*(1./nL1pass);
    }

    cout << " Events=" << nEvents << " L1 rate=" << L1rate;
    cout << " & HLT rate=" << HLTrate << " +- "<< HLTrate_err << endl;

    // Calculate efficiencies w.r.t. L1.
    // Then histograms from all files, weighting by L1 rates
    // to get output rate of each trigger.

    TH1F* L1 = MakeEffiPlot(false, inFiles[i], "L1", nL1pass);
    SumHisto(first, L1rate, L1, _L1);

    TH1F* HLT = MakeEffiPlot(false, inFiles[i], "HLT", nL1pass);
    SumHisto(first, L1rate, HLT, _HLT);

    // Integrate differential to integral rates and convert these 
    // to efficiencies w.r.t. L1.
    // Then histograms from all files, weighting by L1 rates
    // to get output rate of each trigger.

    TH1F* Et = MakeEffiPlot(true, inFiles[i], "et", nL1pass);
    SumHisto(first, L1rate, Et, _Et);

    for (unsigned int j = 0; j < NJETS; j++) {
      ostringstream nameTmp;
      nameTmp << "Jet"<<j+1<<ends;
      TH1F* Jet = 
         MakeEffiPlot(true, inFiles[i], nameTmp.str().c_str(), nL1pass);
      SumHisto(first, L1rate, Jet, _Jets[j]);
    }

    for (unsigned int k = 0; k < NJETS; k++) {
      ostringstream nameTmp;
      nameTmp << "Jet"<<k+1<<"bL25"<<ends;
      TH1F* bJetL25 = 
         MakeEffiPlot(true, inFiles[i], nameTmp.str().c_str(), nL1pass);
      SumHisto(first, L1rate, bJetL25, _bJetsL25[k]);
    }

    for (unsigned int m = 0; m < NJETS; m++) {
      ostringstream nameTmp;
      nameTmp << "Jet"<<m+1<<"bL3"<<ends;
      TH1F* bJetL3 = 
         MakeEffiPlot(true, inFiles[i], nameTmp.str().c_str(), nL1pass);
      SumHisto(first, L1rate, bJetL3, _bJetsL3[m]);
    }

    first = false;
  }

  cout << "Total rate (Hz) at input = " << totalInputRate << ", at L1 = " << totalL1Rate << endl;

  // Convert rate plots to efficiency plots if required.
  // Normalize to events passing L1, except for L1 results.
  if (writeEffiPlots) {
    _L1          ->Scale (1./totalInputRate);
    _HLT         ->Scale (1./totalL1Rate);
    _Et          ->Scale (1./totalL1Rate);
    for (unsigned int m = 0; m < NJETS; m++) {
      _Jets[m]     ->Scale (1./totalL1Rate);
      _bJetsL25[m] ->Scale (1./totalL1Rate);
      _bJetsL3[m]  ->Scale (1./totalL1Rate);
    }
  }

  fout->Write();
  for (unsigned int i = 0; i < inFiles.size(); i++){
    if (inFiles[i] == 0) continue;
    delete inFiles[i];
  }
  fout->Close();
  delete fout;
}


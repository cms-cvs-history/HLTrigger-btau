#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

const Int_t NFILES   = 7;
const Int_t NPATHS   = 10;
const Int_t NMUPATHS = 5;
const Int_t NL1PATHS = 7;
Int_t i, j, icomp;

const Double_t luminosity = 1E5; //In mb-1
Double_t crossSection[NFILES]; //Likewise in milli barns
Double_t Nevents[NFILES];
Double_t L1events[NFILES][NL1PATHS];

void ComputeRatesQCD(){

  TFile *fout = new TFile("HighStatQCDmin30Rate.root","RECREATE");

//   crossSection[0] = 1.46 ;
//   crossSection[1] = 6.32E-1 ;
  crossSection[0] = 1.63E-1 ;
  crossSection[1] = 2.16E-2 ;
  crossSection[2] = 3.08E-3 ;
  crossSection[3] = 2.94E-4 ;
  crossSection[4] = 1.01E-4 ;
  crossSection[5] = 2.45E-5 ;
  crossSection[6] = 6.24E-6 ;
  
  TH1F* TempHadrPt[NFILES][NPATHS];   //HadrPt = Pt of Jet or HT as applicable
  TH1F* TempL2MuonPt[NFILES][NMUPATHS];
  TH1F* TempL3MuonPt[NFILES][NMUPATHS];
  TH1F* TempL1Paths[NFILES][NL1PATHS];
    
  double evts;

  TFile* f[NFILES];
//   f[0] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD15-20v160_BAn.root");
//   f[1] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD20-30v160_BAn.root");
  f[0] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD30-50v160_BAn.root");
  f[1] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD50-80v160_BAn.root");
  f[2] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD80-120v160_BAn.root");

  f[3] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD120-170v160_BAn.root");
  f[4] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD170-230v160_BAn.root");
  f[5] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD230-300v160_BAn.root");
  f[6] = new TFile("/uscmst1b_scratch/lpc1/lpcbtag/avetisya/bnote/Analysis/QCD300-380v160_BAn.root");

  f[0]->cd();

  //Total Events
  TH1F* _hTotalEv = (TH1F *)hTotalEv->Clone();
  
  //L1 Paths
  TH1F* _hL1Paths[NL1PATHS];
  _hL1Paths[0] = (TH1F *)hP1L1trig->Clone();
  _hL1Paths[1] = (TH1F *)hHTL1trig->Clone();
  _hL1Paths[2] = (TH1F *)hSJ150L1trig->Clone();
  _hL1Paths[3] = (TH1F *)hDJ100L1trig->Clone();
  _hL1Paths[4] = (TH1F *)hTJ50L1trig->Clone();
  _hL1Paths[5] = (TH1F *)hQJ30L1trig->Clone();
  _hL1Paths[6] = (TH1F *)hHT300L1trig->Clone();

  //HLT Paths
  //Jet Pt and HT
  TH1F* _hHadrPt[NPATHS];
  _hHadrPt[0] = (TH1F *)hHLTB1JetJetPt->Clone();
  _hHadrPt[1] = (TH1F *)hHLTB2JetJetPt->Clone();
  _hHadrPt[2] = (TH1F *)hHLTB3JetJetPt->Clone();
  _hHadrPt[3] = (TH1F *)hHLTB4JetJetPt->Clone();
  _hHadrPt[4] = (TH1F *)hHLTBHTHT->Clone();
  _hHadrPt[5] = (TH1F *)hHLTB1JetMuJetPt->Clone();
  _hHadrPt[6] = (TH1F *)hHLTB2JetMuJetPt->Clone();
  _hHadrPt[7] = (TH1F *)hHLTB3JetMuJetPt->Clone();
  _hHadrPt[8] = (TH1F *)hHLTB4JetMuJetPt->Clone();
  _hHadrPt[9] = (TH1F *)hHLTBHTMuHT->Clone();
  
  //Muons
  //L2 Muons
  TH1F* _hL2MuonPt[NMUPATHS];
  _hL2MuonPt[0] = (TH1F *)hHLTB1JetMuL2MuPt->Clone();
  _hL2MuonPt[1] = (TH1F *)hHLTB2JetMuL2MuPt->Clone();
  _hL2MuonPt[2] = (TH1F *)hHLTB3JetMuL2MuPt->Clone();
  _hL2MuonPt[3] = (TH1F *)hHLTB4JetMuL2MuPt->Clone();
  _hL2MuonPt[4] = (TH1F *)hHLTBHTMuL2MuPt->Clone();
  
  //L3 Muons
  TH1F* _hL3MuonPt[NMUPATHS];
  _hL3MuonPt[0] = (TH1F *)hHLTB1JetMuL3MuPt->Clone();
  _hL3MuonPt[1] = (TH1F *)hHLTB2JetMuL3MuPt->Clone();
  _hL3MuonPt[2] = (TH1F *)hHLTB3JetMuL3MuPt->Clone();
  _hL3MuonPt[3] = (TH1F *)hHLTB4JetMuL3MuPt->Clone();
  _hL3MuonPt[4] = (TH1F *)hHLTBHTMuL3MuPt->Clone();

  //Reset histograms
  _hTotalEv->Reset();
  
  //L1
  for (i = 0; i < NL1PATHS; i++){
    _hL1Paths[i]->Reset();
  }

  //HLT Paths
  //Hadronic
  for (i = 0; i < NPATHS; i++){
    _hHadrPt[i]->Reset();
  }
  
  //Muons
  //L2 Muons
  for (i = 0; i < NMUPATHS; i++){
    _hL2MuonPt[i]->Reset();
  }
  
  //L3 Muons
  for (i = 0; i < NMUPATHS; i++){
    _hL3MuonPt[i]->Reset();
  }
  

  cout <<  _hL1Paths[0]->GetEntries() << endl;
  
  for (i = 0; i < NFILES; i++){
    cout << i  << endl;
    f[i]->cd();
    
    // get Total events and compute weights 
    cout << " total ev " <<  hTotalEv->GetEntries() << endl;
    Nevents[i] = hTotalEv->GetEntries();
    
    L1events[i][0] = hP1L1trig->GetEntries();
    L1events[i][1] = hHTL1trig->GetEntries();
    L1events[i][2] = hSJ150L1trig->GetEntries(); 
    L1events[i][3] = hDJ100L1trig->GetEntries();
    L1events[i][4] = hTJ50L1trig->GetEntries(); 
    L1events[i][5] = hQJ30L1trig->GetEntries();
    L1events[i][6] = hHT300L1trig->GetEntries(); 
    
    if (Nevents[i] == 0){
      cout<<"Empty file number: "<<i<<endl;
      return;
    }

    Double_t wei = crossSection[i] * luminosity / Nevents[i];
    cout << " L1, Nev, weight " <<  L1events[i][0] << " " << Nevents[i] << " " << wei << endl;    
    
    //cout<<"Begin historgram declarations"<<endl;
    
    //L1
    TempL1Paths[i][0] = (TH1F*) hP1L1trig->Clone();
    TempL1Paths[i][1] = (TH1F *)hHTL1trig->Clone();
    TempL1Paths[i][2] = (TH1F *)hSJ150L1trig->Clone();
    TempL1Paths[i][3] = (TH1F *)hDJ100L1trig->Clone();
    TempL1Paths[i][4] = (TH1F *)hTJ50L1trig->Clone();
    TempL1Paths[i][5] = (TH1F *)hQJ30L1trig->Clone();
    TempL1Paths[i][6] = (TH1F *)hHT300L1trig->Clone();
    
    //cout<<"Done L1 historgram declarations"<<endl;
    
    //HLT Paths
    //Jet Pt and HT
    TempHadrPt[i][0] = (TH1F *)hHLTB1JetJetPt->Clone();
    TempHadrPt[i][1] = (TH1F *)hHLTB2JetJetPt->Clone();
    TempHadrPt[i][2] = (TH1F *)hHLTB3JetJetPt->Clone();
    TempHadrPt[i][3] = (TH1F *)hHLTB4JetJetPt->Clone();
    TempHadrPt[i][4] = (TH1F *)hHLTBHTHT->Clone();
    TempHadrPt[i][5] = (TH1F *)hHLTB1JetMuJetPt->Clone();
    TempHadrPt[i][6] = (TH1F *)hHLTB2JetMuJetPt->Clone();
    TempHadrPt[i][7] = (TH1F *)hHLTB3JetMuJetPt->Clone();
    TempHadrPt[i][8] = (TH1F *)hHLTB4JetMuJetPt->Clone();
    TempHadrPt[i][9] = (TH1F *)hHLTBHTMuHT->Clone();
    
    //cout<<"Done Jet historgram declarations"<<endl;
    //Muons
    //L2 Muons
    TempL2MuonPt[i][0] = (TH1F *)hHLTB1JetMuL2MuPt->Clone();
    TempL2MuonPt[i][1] = (TH1F *)hHLTB2JetMuL2MuPt->Clone();
    TempL2MuonPt[i][2] = (TH1F *)hHLTB3JetMuL2MuPt->Clone();
    TempL2MuonPt[i][3] = (TH1F *)hHLTB4JetMuL2MuPt->Clone();
    TempL2MuonPt[i][4] = (TH1F *)hHLTBHTMuL2MuPt->Clone();
    
    //cout<<"Done L2 Muon historgram declarations"<<endl;
    //L3 Muons
    TempL3MuonPt[i][0] = (TH1F *)hHLTB1JetMuL3MuPt->Clone();
    TempL3MuonPt[i][1] = (TH1F *)hHLTB2JetMuL3MuPt->Clone();
    TempL3MuonPt[i][2] = (TH1F *)hHLTB3JetMuL3MuPt->Clone();
    TempL3MuonPt[i][3] = (TH1F *)hHLTB4JetMuL3MuPt->Clone();
    TempL3MuonPt[i][4] = (TH1F *)hHLTBHTMuL3MuPt->Clone();
    
    //cout<<"Got past historgram declarations"<<endl;
    
    //L1
    for (j = 0; j < NL1PATHS; j++){
      cout <<  _hL1Paths[j]->GetEntries() << endl;
      _hL1Paths[j]->Add(TempL1Paths[i][j],wei);
    }
    //HLT Hadronic
    for (j = 0; j < NPATHS; j++){
      cout <<  _hHadrPt[j]->GetEntries() << endl;
      _hHadrPt[j]->Add(TempHadrPt[i][j],wei);
    }
    //HLT L2 Muons
    for (j = 0; j < NMUPATHS; j++){
      cout <<  _hL2MuonPt[j]->GetEntries() << endl;
      _hL2MuonPt[j]->Add(TempL2MuonPt[i][j],wei);
    }
    //HLT L3 Muons
    for (j = 0; j < NMUPATHS; j++){
      cout <<  _hL3MuonPt[j]->GetEntries() << endl;
      _hL3MuonPt[j]->Add(TempL3MuonPt[i][j],wei);
    }
    
  }
  
  wei=1.0;
  fout->cd();

  //L1
  for (i = 0; i < NL1PATHS; i++){
    Compute(_hL1Paths[i], wei);
    _hL1Paths[i]->Write();
  }
  //Hadronic HLT
  for (i = 0; i < NPATHS; i++){
    Compute(_hHadrPt[i], wei);
    _hHadrPt[i]->Write();
  }
  //Muon HLT
  for (i = 0; i < NMUPATHS; i++){
    Compute(_hL2MuonPt[i], wei);
    _hL2MuonPt[i]->Write();

    Compute(_hL3MuonPt[i], wei);
    _hL3MuonPt[i]->Write();
  }
  fout->Close();


}

void Compute(TH1F* histogram, Double_t wei){
  
  Int_t nbins = histogram->GetNbinsX();
  Double_t integral = histogram->GetBinContent(nbins+1);
  
  for(icomp = nbins; icomp >= 1; icomp--){
    
    Double_t thisBin = histogram->GetBinContent(icomp);
    integral += thisBin;
    Double_t integralRate;
    Double_t integralError;
    
    integralRate = wei * integral;
    integralError = wei * sqrt(integral);
    
    histogram->SetBinContent(icomp, integralRate);
    histogram->SetBinError(icomp, integralError);
  }
  cout << " Total Rate " << histogram->GetBinContent(1) << endl;
}

#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

void PlotratesQCD(){



  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);
// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(true);
  tdrStyle->SetPadGridY(true);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);
// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  tdrStyle->SetEndErrorSize(2);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);
  tdrStyle->SetMarkerSize(3);
  tdrStyle->SetMarkerColor(2);

  tdrStyle->SetOptDate(0);
// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(""); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(0);
  tdrStyle->SetStatH(0.);
  tdrStyle->SetStatW(0.);
// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);
// For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
// For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
// For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");
// For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);
// Postscript options:
  tdrStyle->SetPaperSize(15.,15.);
  tdrStyle->cd();

  TFile *fout = new TFile("HighStatQCDmin30Rate.root");

  DrawHist(hP1L1trig,4,"L1_Mu5_Jet15"," Rate (Hz)","L1RateMu5Jet15HS.pdf");
  DrawHist(hHTL1trig,4,"L1_HTT250"," Rate (Hz)","L1RateHTT250HS.pdf");
  DrawHist(hSJ150L1trig,4,"L1_SingleJet150"," Rate (Hz)","L1RateSJ150HS.pdf");
  DrawHist(hDJ100L1trig,4,"L1_DoubleJet100"," Rate (Hz)","L1RateDJ100HS.pdf");
  DrawHist(hTJ50L1trig,4,"L1_TripleJet50"," Rate (Hz)","L1RateTJ50HS.pdf");
  DrawHist(hQJ30L1trig,4,"L1_QuadJet30"," Rate (Hz)","L1RateQJ30HS.pdf");
  DrawHist(hHT300L1trig,4,"L1_HTT300"," Rate (Hz)","L1RateHTT300HS.pdf");
  
  DrawHist(hHLTB1JetJetPt,2,"HLTB1Jet Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate1HS.pdf");
  DrawHist(hHLTB2JetJetPt,2,"HLTB2Jet Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate2HS.pdf");
  DrawHist(hHLTB3JetJetPt,2,"HLTB3Jet Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate3HS.pdf");
  DrawHist(hHLTB4JetJetPt,2,"HLTB4Jet Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate4HS.pdf");
  DrawHist(hHLTB1JetMuJetPt,2,"HLTB1JetMu Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate1MuHS.pdf");
  DrawHist(hHLTB2JetMuJetPt,2,"HLTB2JetMu Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate2MuHS.pdf");
  DrawHist(hHLTB3JetMuJetPt,2,"HLTB3JetMu Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate3MuHS.pdf");
  DrawHist(hHLTB4JetMuJetPt,2,"HLTB4JetMu Jet ET threshold (GeV)","HLT Rate (Hz)","jetRate4MuHS.pdf");
  
  DrawHist(hHLTB1JetMuL2MuPt,3,"HLTB1JetMu L2 Muon pT threshold (GeV)","HLT Rate (Hz)","L2MuRate1HS.pdf");
  DrawHist(hHLTB2JetMuL2MuPt,3,"HLTB2JetMu L2 Muon pT threshold (GeV)","HLT Rate (Hz)","L2MuRate2HS.pdf");
  DrawHist(hHLTB3JetMuL2MuPt,3,"HLTB3JetMu L2 Muon pT threshold (GeV)","HLT Rate (Hz)","L2MuRate3HS.pdf");
  DrawHist(hHLTB4JetMuL2MuPt,3,"HLTB4JetMu L2 Muon pT threshold (GeV)","HLT Rate (Hz)","L2MuRate4HS.pdf");
  DrawHist(hHLTBHTMuL2MuPt,3,"HLTBHTMu L2 Muon pT threshold (GeV)","HLT Rate (Hz)","L2MuRateHTHS.pdf");

  DrawHist(hHLTB1JetMuL3MuPt,3,"HLTB1JetMu L3 Muon pT threshold (GeV)","HLT Rate (Hz)","L3MuRate1HS.pdf");
  DrawHist(hHLTB2JetMuL3MuPt,3,"HLTB2JetMu L3 Muon pT threshold (GeV)","HLT Rate (Hz)","L3MuRate2HS.pdf");
  DrawHist(hHLTB3JetMuL3MuPt,3,"HLTB3JetMu L3 Muon pT threshold (GeV)","HLT Rate (Hz)","L3MuRate3HS.pdf");
  DrawHist(hHLTB4JetMuL3MuPt,3,"HLTB4JetMu L3 Muon pT threshold (GeV)","HLT Rate (Hz)","L3MuRate4HS.pdf");
  DrawHist(hHLTBHTMuL3MuPt,3,"HLTBHTMu L3 Muon pT threshold (GeV)","HLT Rate (Hz)","L3MuRateHTHS.pdf");

  DrawHist(hHLTBHTHT,2,"HLTBHT HT threshold (GeV)","HLT Rate (Hz)","HTRateHS.pdf");
  DrawHist(hHLTBHTMuHT,2,"HLTBHTMu HT threshold (GeV)","HLT Rate (Hz)","HTRateMuHS.pdf");


//   //  DrawHist(hP1L3MuPt,4,"L3 Muon pT threshold (GeV)","HLT Rate (Hz)","L3MuRateP1HS.pdf");

//   DrawHist(hHTAll,2," Muon pTrel threshold (GeV)","HLT Rate (Hz)","RateHTAllHS.pdf");
//   DrawHist(hHTJetPt,2,"Jet ET threshold (GeV)","HLT Rate (Hz)","jetRateHTHS.pdf");
//   DrawHist(hHTL2MuPt,3,"L2 Muon pT threshold (GeV)","HLT Rate (Hz)","L2MuRateHTHS.pdf");
//   DrawHist(hHTL3MuPt,4,"L3 Muon pT threshold (GeV)","HLT Rate (Hz)","L3MuRateHTHS.pdf");
//   DrawHist(hHTJetPtrel,6," Muon pTrel threshold (GeV)","HLT Rate (Hz)","ptRelRateHTHS.pdf");


}

void DrawHist(TH1F* histogram, int col, char* xtit, char* ytit, char* filetit){

  TCanvas *c11;
  c11 = new TCanvas("c11", xtit,  300, 10, 600, 600);
  c11->cd();
  histogram->SetXTitle(xtit);
  histogram->SetYTitle(ytit);
  histogram->SetMarkerStyle(20);
  histogram->SetMarkerSize(1);
  histogram->SetMarkerColor(col);
  histogram->Draw();
  c11->Print(filetit);

}

#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TLegend.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

//const char* yAxis = "Trigger rate (Hz)";
const char* yAxis = "Trigger efficiency";

const unsigned int NJETS = 4;
const bool LOG_PLOTS = false;

void TDRstyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(300);   //POsition on screen
  tdrStyle->SetCanvasDefY(10);
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
  tdrStyle->SetMarkerSize(2.5);
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
}

void PlotL1Rates(TFile* inFile) {
  TH1F* his;

  TCanvas *c = new TCanvas("cL1", "fun");
  gPad->SetLogy(LOG_PLOTS);

  his = (TH1F*) inFile->Get("_L1");
  his->SetMarkerStyle(20);

  his->Draw();

  his->SetXTitle("L1 performance");
  his->SetYTitle(yAxis);

  c->Print("L1Rates.eps");
}

void PlotHLTRates(TFile* inFile) {
  TH1F* his;

  TCanvas *c = new TCanvas("cHLT", "fun");
  gPad->SetLogy(LOG_PLOTS);

  his = (TH1F*) inFile->Get("_HLT");
  his->SetMarkerStyle(20);

  his->Draw();

  his->SetXTitle("HLT performance w.r.t. L1");
  his->SetYTitle(yAxis);

  c->Print("HLTRates.eps");
}

void PlotEtRates(TFile* inFile) {
  TH1F* his;

  TCanvas *c = new TCanvas("cL2et", "fun");
  gPad->SetLogy(LOG_PLOTS);

  his = (TH1F*) inFile->Get("_et");
  his->SetMarkerStyle(20);

  his->Draw();

  his->SetXTitle("Event transverse energy (GeV)");
  his->SetYTitle(yAxis);

  c->Print("L2EtRates.eps");
}

void PlotJetRates(TFile* inFile) {
  TH1F* Jets[NJETS];

  TCanvas *c = new TCanvas("cL2j", "fun");
  gPad->SetLogy(LOG_PLOTS);

  for (unsigned int j = 0; j < NJETS; j++) {
    ostringstream nameTmp;
    nameTmp << "_Jet"<<j+1<<ends;
    Jets[j] = (TH1F*) inFile->Get(nameTmp.str().c_str());
    Jets[j]->SetMarkerStyle(20 + j);
  }

  //  if (Jets[0]->GetMinimum() == 0.) Jets[0]->SetMinimum(Jets[0]->GetMaximum()/1.0e5); 
  Jets[0]->Draw();
  Jets[1]->Draw("SAME");
  Jets[2]->Draw("SAME");
  Jets[3]->Draw("SAME");

  Jets[0]->SetXTitle("Jet transverse momentum (GeV)");
  Jets[0]->SetYTitle(yAxis);

  TLegend* legend_j = new TLegend(0.7, 0.7, 0.95, 0.95);
  legend_j->SetFillColor(0);
  legend_j->SetTextSize(0.03);
  //  legend_j->SetHeader("L2 Event Rates");
  legend_j->AddEntry(Jets[0], "Single Jet");
  legend_j->AddEntry(Jets[1], "Double Jet");
  legend_j->AddEntry(Jets[2], "Triple Jet");
  legend_j->AddEntry(Jets[3], "Quadruple Jet");
  legend_j->Draw();

  c->Print("L2JetRates.eps");
}

void PlotL25bJetRates(TFile* inFile) {
  TH1F* bJets[NJETS];

  TCanvas *c = new TCanvas("cL25", "fun");
  gPad->SetLogy(LOG_PLOTS);

  for (unsigned int j = 0; j < NJETS; j++) {
    ostringstream nameTmp;
    nameTmp << "_Jet"<<j+1<<"bL25"<<ends;
    bJets[j] = (TH1F*) inFile->Get(nameTmp.str().c_str());
    bJets[j]->SetMarkerStyle(20 + j);
  }

  bJets[0]->Draw();
  bJets[1]->Draw("SAME");
  bJets[2]->Draw("SAME");
  bJets[3]->Draw("SAME");

  bJets[0]->SetXTitle("Level 2.5 b tag discriminator");
  bJets[0]->SetYTitle(yAxis);

  TLegend* legend_j = new TLegend(0.65, 0.7, 0.95, 0.95);
  legend_j->SetFillColor(0);
  legend_j->SetTextSize(0.03);
  //  legend_j->SetHeader("L2.5 Event Rates");
  legend_j->AddEntry(bJets[0], "Highest Et jet");
  legend_j->AddEntry(bJets[1], "2 highest Et Jets");
  legend_j->AddEntry(bJets[2], "3 highest Et jets");
  legend_j->AddEntry(bJets[3], "4 highest Et jets");
  legend_j->Draw();

  c->Print("L25Rates.eps");
}

void PlotL3bJetRates(TFile* inFile) {
  TH1F* bJets[NJETS];

  TCanvas *c = new TCanvas("cL3", "fun");
  gPad->SetLogy(LOG_PLOTS);

  for (unsigned int j = 0; j < NJETS; j++) {
    ostringstream nameTmp;
    nameTmp << "_Jet"<<j+1<<"bL3"<<ends;
    bJets[j] = (TH1F*) inFile->Get(nameTmp.str().c_str());
    bJets[j]->SetMarkerStyle(20 + j);
  }

  bJets[0]->Draw();
  bJets[1]->Draw("SAME");
  bJets[2]->Draw("SAME");
  bJets[3]->Draw("SAME");

  bJets[0]->SetXTitle("Level 3 b tag discriminator");
  bJets[0]->SetYTitle(yAxis);

  TLegend* legend_j = new TLegend(0.65, 0.7, 0.95, 0.95);
  legend_j->SetFillColor(0);
  legend_j->SetTextSize(0.03);
  //  legend_j->SetHeader("L3 Event Rates");
  legend_j->AddEntry(bJets[0], "Highest Et jet");
  legend_j->AddEntry(bJets[1], "2 highest Et jets");
  legend_j->AddEntry(bJets[2], "3 highest Et jets");
  legend_j->AddEntry(bJets[3], "4 highest Et jets");
  legend_j->Draw();

  c->Print("L3Rates.eps");
}

void bLifetimeHLTPlotRates() {

  TFile* inFile = new TFile("HLTrates.root");

  TDRstyle();

  PlotL1Rates(inFile);
  PlotHLTRates(inFile);
  PlotEtRates(inFile);
  PlotJetRates(inFile);
  PlotL25bJetRates(inFile);
  PlotL3bJetRates(inFile);
}

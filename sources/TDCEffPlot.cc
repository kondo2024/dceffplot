#include "TDCEffPlot.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "TString.h"
#include "TH1.h"
using namespace std;
//_________________________________________________
TDCEffPlot::TDCEffPlot()
  : fRIDFfileDir("ridf/"),
    fRIDFfileName("dceff"),
    fRIDFfileExt("ridf"),
    fROOTfileDir("root"),
    fROOTfileName("eff_dc.root"),
    fOutFile(0),
    fHistName("hmulti")
{
  // set Color palette
  fPalette[0] = 2;
  fPalette[1] = 1;
  fPalette[2] = 3;
  fPalette[3] = 4;
  fPalette[4] = 6;
  fPalette[5] = 7;
  fPalette[6] = 8;
  fPalette[7] = 9;
  fPalette[8] = 45;
  fPalette[9] = 30;

}
//_________________________________________________
TDCEffPlot::~TDCEffPlot()
{

}
//_________________________________________________
void TDCEffPlot::AnalyzeAll()
{
  for (int i=0;i<fVRunNum.size();++i){
    int irun = fVRunNum[i];
    AnalyzeRun(irun);
  }
  
}
//_________________________________________________
void TDCEffPlot::Write(TObject *obj)
{
  fOutFile->Add(obj);
  fOutFile->Write();
}
//_________________________________________________
void TDCEffPlot::LoadRunListFile(TString fname)
{
  fRunListFilename = fname;

  ifstream fs(fRunListFilename);
  if ( fs.fail() ){
    cout<<"Cannot open "<<fRunListFilename.Data()<<endl;
    return;
  }

  std::map<int, double> runmap;
  
  vector<TString> str_line0;

  string line;
  int iline=0;
  while(getline(fs,line,'\n')){

    if (line[0]=='#') continue;

    /* Scan one line */
    int irun;
    double hv;

    istringstream ss(line);
    ss>> irun >> hv;
    ++iline;
    
    //cout<<"line-"<<iline << " "<<irun <<" "<<hv<<endl;

    runmap[irun] = hv;

  }

  // sort by HV value
  std::vector<pair<double,int>> vtmp;
  std::map<int,double>::iterator it;

  for (it = runmap.begin(); it != runmap.end(); it++){
    vtmp.push_back({ it->second, it->first});
  }
  sort(vtmp.begin(), vtmp.end());

  cout<<"Loading Run List..."<<endl;
  cout<<"HV   Run"<<endl;
  for (auto& i: vtmp){
    std::cout<< i.first <<" "
	     << i.second <<" "
	     <<std::endl;
    
    fVRunNum.push_back(i.second);
    fVhv.push_back(i.first);
  }
}
//_________________________________________________
// return total efficiency if multi=0
Double_t TDCEffPlot::GetEffMulti(Int_t layer, Int_t multi)
{
//  TH1* h = (TH1*)gDirectory->Get(Form("hmulti%i",layer));
  TH1* h = (TH1*)gDirectory->Get(Form("%s%i",fHistName.Data(),layer));

  Int_t Ntrig = h->GetEntries();
  Int_t ix = h->FindBin((double)multi);
  Int_t counts = h->GetBinContent(ix);

  Double_t eff = -1;
  if (multi==0){// total efficiency
    eff = 100.*(Ntrig-counts)/Ntrig;// %
  }else {
    eff = 100.*counts/Ntrig;// %
  }
  return eff;
}
//_________________________________________________
// return total efficiency if multi=0
Double_t TDCEffPlot::GetEffMultiGeN(Int_t layer, Int_t multi)
{
  TH1* h = (TH1*)gDirectory->Get(Form("%s%i",fHistName.Data(),layer));
  //TH1* h = (TH1*)gDirectory->Get(Form("hmulti%i",layer));

  Int_t Ntrig = h->GetEntries();
  Int_t counts = Ntrig;
  
  Double_t eff = -1;
  for (int i=0;i<multi;++i){
    Int_t ix = h->FindBin((double)i);
    counts -= h->GetBinContent(ix);
  }
  eff = 100.*counts/Ntrig;// %

  return eff;
}
//_________________________________________________
TGraph* TDCEffPlot::MakeGraph(Int_t layer, Int_t multi)
{
  TGraph * g = new TGraph();
  if (fVLayerName.size()>0){
    g->SetName(fVLayerName[layer]);
    g->SetTitle(Form("layer=%i %s",layer,fVLayerName[layer].Data()));
  }else{
    g->SetName(Form("layer%i",layer));
    g->SetName(Form("layer=%i",layer));    
  }
  g->SetMinimum(0);
  g->SetMaximum(110);
  g->SetMarkerStyle(20);  
  g->GetXaxis()->SetTitle("HV (V)");
  g->GetYaxis()->SetTitle("Efficiency (%)");
  
  
  for (int i=0;i<fVRunNum.size();++i){
    int irun = fVRunNum[i];
    double hv = fVhv[i];
    int n = g->GetN();

    TFile file(Form("%s/%s_ana%04d.root",fROOTfileDir.Data(),
		    fRIDFfileName.Data(),irun));

    double eff = GetEffMulti(layer,multi);
    g->SetPoint(n,hv,eff);    
  }
  return g;
  
}
//_________________________________________________
TGraph* TDCEffPlot::MakeGraph_MgeN(Int_t layer, Int_t multi)
{
  TGraph * g = new TGraph();
  if (fVLayerName.size()>0){
    g->SetName(fVLayerName[layer]);
    g->SetTitle(Form("layer=%i %s",layer,fVLayerName[layer].Data()));
  }else{
    g->SetName(Form("layer%i",layer));
    g->SetName(Form("layer=%i",layer));    
  }
  g->SetMinimum(0);
  g->SetMaximum(110);
  g->SetMarkerStyle(20);  
  g->GetXaxis()->SetTitle("HV (V)");
  g->GetYaxis()->SetTitle("Efficiency (%)");
  
  
  for (int i=0;i<fVRunNum.size();++i){
    int irun = fVRunNum[i];
    double hv = fVhv[i];
    int n = g->GetN();

    TFile file(Form("%s/%s_ana%04d.root",fROOTfileDir.Data(),
		    fRIDFfileName.Data(),irun));
    double eff = GetEffMultiGeN(layer,multi);
    
    g->SetPoint(n,hv,eff);

  }
  return g;
  
}
//_________________________________________________
//_________________________________________________
//_________________________________________________



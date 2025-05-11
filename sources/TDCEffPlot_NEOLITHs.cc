#include "TDCEffPlot_NEOLITHs.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TLine.h"
#include "TText.h"
#include "TCanvas.h"

#include "TArtEventStore.hh"
#include "TArtRawEventObject.hh"

using namespace std;
//_________________________________________________
TDCEffPlot_NEOLITHs::TDCEffPlot_NEOLITHs()
  : fNlayer(0), fOutFile(0)
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

  fIndex2geo.resize(80*3);
  
}
//_________________________________________________
TDCEffPlot_NEOLITHs::~TDCEffPlot_NEOLITHs()
{
}
//_________________________________________________
void TDCEffPlot_NEOLITHs::AnalyzeAll()
{
  for (int i=0;i<fVRunNum.size();++i){
    int irun = fVRunNum[i];
    AnalyzeRun(irun);
  }
  
}
//_________________________________________________
void TDCEffPlot_NEOLITHs::AnalyzeRun(Int_t nRun, Long64_t neve)
{
  TArtEventStore estore;
  estore.Open(Form("%s/%s%04d.ridf",fRIDFfileDir.Data(),
		   fRIDFfileName.Data(),nRun));
  TArtRawEventObject *rawevent = estore.GetRawEventObject();

  TFile fout(Form("%s/%s_ana%04d.root",fROOTfileDir.Data(),
		  fRIDFfileName.Data(),nRun),"RECREATE");

  // for check
  TH1* hindexwid = new TH2D("hindexwid","Index WireID",13*16,-0.5,13*16-0.5, 80,-0.5,79.5);
  TH1* hindexlayer = new TH2D("hindexlayer","Index Layer",13*16,-0.5,13*16-0.5, 3,-0.5,2.5);
  TH1* hindexgeo = new TH2D("hindexgeo","Index geo",13*16,-0.5,13*16-0.5, 2,19.5,21.5);
  TH1* hindexch = new TH2D("hindexch","Index V1190ch",13*16,-0.5,13*16-0.5, 128,-0.5,127.5);
  TH1* hindextl = new TH2D("hindextl","Index Tleading",13*16,-0.5,13*16-0.5, 100,0,30000);
  
  std::vector<TH1*> hwid;
  std::vector<TH1*> htl;
  std::vector<TH1*> htt;
  std::vector<TH1*> htot;
  std::vector<TH1*> hmulti;

  TH1* htmp =0;

  // hit pattern (wire id distribution)
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("hwid%i",ilayer),Form("Wire ID Layer=%i %s",ilayer, lname.Data()),
		   80,-0.5,79.5);
    hwid.push_back(htmp);
  }
  

  // multipliticy
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("hmulti%i",ilayer),Form("Multiplicity Layer=%i %s",ilayer, lname.Data()),
		   10,-0.5,9.5);
    hmulti.push_back(htmp);
  }
  
  // T leading
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htl%i",ilayer),Form("T leading Layer=%i %s",ilayer, lname.Data()),
			   2000,-5000,5000);
    htl.push_back(htmp);
  }

  // T trailing
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htt%i",ilayer),Form("T trailing Layer=%i %s",ilayer, lname.Data()),
		    2000,-5000,5000);
    htt.push_back(htmp);
  }

  // TOT
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htot%i",ilayer),Form("ToT Layer=%i %s",ilayer, lname.Data()),
		   2000,0,20000);
    htot.push_back(htmp);
  }
  
  TH1* htotttrail0 = new TH2D("htotttrail0","V1190-1 ToT Ttrailing",100,-2000,1000,200,12000,20000);

  const Int_t index_max = 3*80;// for neolith-s
  Int_t T_leading[index_max];
  Int_t T_trailing[index_max];
  std::vector<Int_t> multi(fNlayer);
  fTrefval.clear();
  
  int ieve = 0;
  while(estore.GetNextEvent()){

    for (int i=0;i<index_max;++i){
      T_leading[i] = -1;
      T_trailing[i] = -1;
    }
    for (int i=0;i<fNlayer;++i) multi[i]=0;

    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int device = seg->GetDevice();
      int fp = seg->GetFP();
      int detector = seg->GetDetector();
      int module = seg->GetModule();

      if (module!=24) continue;// titech setup
      
      for(int j=0;j<seg->GetNumData();j++){
        TArtRawDataObject *d = seg->GetData(j);
        int geo = d->GetGeo();
        int ch = d->GetCh();
        int val = d->GetVal();
        int cat = d->GetCategoryID();
        int det = d->GetDetectorID();
        int id = d->GetDatatypeID();
        int edge = d->GetEdge();

	// check geo, skip if not found
	if(find(fVgeo.begin(), fVgeo.end(),geo) == fVgeo.end()) continue;

	// store tref values ***** subtraction should be implemented
	auto iter = fTrefmap.find(geo);
	if (iter->second == ch && edge==0){
	  fTrefval[geo] = val;
	}
	
	// multiplicity
	int layer = FindHitLayer(geo,ch);

	if (layer<0) continue;// skip trigger signal

	int wireid = FindWireID(geo,ch);
	int index = layer*80 + wireid;// for neolith-s
	fIndex2geo[index] = geo;

	hindexwid->Fill(index,wireid);
	hindexlayer->Fill(index,layer);
	hindexgeo->Fill(index,geo);
	hindexch->Fill(index,ch);

	if (edge==0){
//	if (edge==1){
	  multi[layer]++;
	  hwid[layer]->Fill(wireid);

	  T_leading[index] = val;
	  hindextl->Fill(index,val);
	  
	}else if (edge==1) {
//	}else if (edge==0) {
	  T_trailing[index] = val;
	}
	
      }
    }

    // subtract Tref, calc. ToT
    for (int index=0;index<index_max;++index){
      int layer = index/80;
      int geo = fIndex2geo[index];
      int tref = fTrefval[geo];
      if (tref<0) cout<<" tref is not found!!!"<<endl;
      
      if (T_leading[index]>0) htl[layer]->Fill(T_leading[index]-tref);
      if (T_trailing[index]>0) htt[layer]->Fill(T_trailing[index]-tref);

      if (T_leading[index]>0 && T_trailing[index]>0 ){
        int tot = T_trailing[index] - T_leading[index];
	htot[layer]->Fill(tot);
      }
    }

    for (int ilayer=0;ilayer<fNlayer;++ilayer) {
      hmulti[ilayer]->Fill(multi[ilayer]);
    }

    estore.ClearData();
    ieve++;

    if (neve>0){
      if (ieve>=neve) break;
    }
    
  }

  fout.Write();
  fout.Close();

}
//_________________________________________________
void TDCEffPlot_NEOLITHs::Plot(Int_t layer)
{
  if (fOutFile==0){
    fOutFile = new TFile(Form("%s/%s",fROOTfileDir.Data(), fROOTfileName.Data()),"recreate");
  }
  fPlotItems = new TList();

  fOutFile->cd();

  TCanvas *c1 = new TCanvas();
  c1->SetName(Form("c%d",layer));
  c1->SetTitle(fVLayerName[layer].Data());
  TList *tlist = new TList();
  tlist->SetName(fVLayerName[layer].Data());
  
  TGraph *g[kMmax];
  for (int i=0;i<kMmax;++i){
    g[i] = MakeGraph(layer,i);
    tlist->Add(g[i]);
  }
  
  for (int i=0;i<kMmax;++i) {
    g[i]->SetLineColor(fPalette[i]);
    g[i]->SetMarkerColor(fPalette[i]);
  }

  double x = 0.12;
  double y = 0.85;
  
  for (int i=0;i<kMmax;++i){
    TLine *l = new TLine(x,y,x+0.05,y);
    l->SetLineColor(fPalette[i]);
    l->SetNDC(1);
    tlist->Add(l);

    TString text(Form("M%i",i));
    if (i==0) text = "Mall";
    TText *t = new TText(x+0.1,y,text.Data());
    t->SetNDC(1);
    t->SetTextAlign(12);
    tlist->Add(t);
    y -= 0.05;
  }

  // M>=3
  g[kMmax] = MakeGraph_Mge3(layer);
  g[kMmax]->SetLineColor(2);
  g[kMmax]->SetLineStyle(2);
  g[kMmax]->SetMarkerColor(2);
  tlist->Add(g[kMmax]);
  TLine *l = new TLine(x,y,x+0.05,y);
  l->SetLineColor(2);
  l->SetLineStyle(2);
  l->SetNDC(1);
  tlist->Add(l);

  TText *t = new TText(x+0.1,y,"M>=3");
  t->SetNDC(1);
  t->SetTextAlign(12);
  tlist->Add(t);
  y -= 0.05;

  g[0]->Draw("APL");
  tlist->Draw("PL");

  Write(tlist);
  Write(c1);
  
}
//_________________________________________________
void TDCEffPlot_NEOLITHs::Write(TObject *obj)
{
  fOutFile->Add(obj);
  fOutFile->Write();
}
//_________________________________________________
TGraph* TDCEffPlot_NEOLITHs::MakeGraph(Int_t layer, Int_t multi)
{
  TGraph * g = new TGraph();
  g->SetName(fVLayerName[layer]);
  g->SetTitle(Form("layer=%i %s",layer,fVLayerName[layer].Data()));
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

    TH1* h = (TH1*)file.Get(Form("hmulti%i",layer));
    Int_t Ntrig = h->GetEntries();
    Int_t ix = h->FindBin((double)multi);
    Int_t counts = h->GetBinContent(ix);
    if (multi==0){// total efficiency
      double eff = 100.*(Ntrig-counts)/Ntrig;// %
      g->SetPoint(n,hv,eff);
    }else {
      double eff = 100.*counts/Ntrig;// %
      g->SetPoint(n,hv,eff);
    }
  }
  return g;
  
}
//_________________________________________________
TGraph* TDCEffPlot_NEOLITHs::MakeGraph_Mge3(Int_t layer)
{
  TGraph * g = new TGraph();
  g->SetName(fVLayerName[layer]);
  g->SetTitle(Form("layer=%i %s",layer,fVLayerName[layer].Data()));
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

    TH1* h = (TH1*)file.Get(Form("hmulti%i",layer));
    Int_t Ntrig = h->GetEntries();
    Int_t ix0 = h->FindBin(0.0);
    Int_t ix1 = h->FindBin(1.0);
    Int_t ix2 = h->FindBin(2.0);
    Int_t m0 = h->GetBinContent(ix0);
    Int_t m1 = h->GetBinContent(ix1);
    Int_t m2 = h->GetBinContent(ix2);

    double eff = 100.*(Ntrig-m0-m1-m2)/Ntrig;// %
    g->SetPoint(n,hv,eff);

  }
  return g;
  
}
//_________________________________________________
void TDCEffPlot_NEOLITHs::LoadRunListFile(TString fname)
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
  for (auto& i: vtmp){
    std::cout<< i.first <<" "
	     << i.second <<" "
	     <<std::endl;
    
    fVRunNum.push_back(i.second);
    fVhv.push_back(i.first);
  }
  cout<<"Done"<<endl;
  
}
//_________________________________________________
void TDCEffPlot_NEOLITHs::LoadMapFile(TString fname)
{
  fMapFilename = fname;

  ifstream fs(fMapFilename);
  if ( fs.fail() ){
    cout<<"Cannot open "<<fMapFilename.Data()<<endl;
    return;
  }

  cout<<"Loading map file..."<<endl;

  vector<TString> str_line0;

  string line;
  int iline=0;
  while(getline(fs,line,'\n')){

    if (line[0]=='#') continue;

    /* Scan one line */
    int layerid;
    TString name;
    int geo;
    int ch_start;
    int ch_stop;
    int wireid_start;
    int wireid_stop;

    istringstream ss(line);
    ss>> layerid >> name
      >>geo
      >>ch_start>>ch_stop
      >>wireid_start>>wireid_stop;
    ++iline;

    cout<<"line-"<<iline << " "
	<<layerid <<" "
	<<name <<" "
	<<geo <<" "
	<<ch_start <<" "
	<<ch_stop <<" "
	<<wireid_start <<" "
	<<wireid_stop <<" "
	<<endl;

    if (name.Contains("trig")){// tref
      fTrefmap[geo] = ch_start;
    }else{
      fVLayerID.push_back(layerid);
      fVLayerName[layerid] = name;
      fVgeo.push_back(geo);
      fVchstart.push_back(ch_start);
      fVchstop.push_back(ch_stop);
      fVwireidstart.push_back(wireid_start);
      fVwireidstop.push_back(wireid_stop);

      if (layerid+1 > fNlayer) fNlayer = layerid+1;
    }
    
  }
  cout<<"Done"<<endl;
  
}
//_________________________________________________
//_________________________________________________
Int_t TDCEffPlot_NEOLITHs::FindWireID(Int_t geo, Int_t ch)
{
  int n = fVgeo.size();

  int wireid = -1;
  for (int i=0;i<n;++i){
    if (geo !=fVgeo[i]) continue;// check geo

    if (ch < fVchstart[i]) continue;
    if (ch > fVchstop[i]) continue;

    wireid = ch-fVchstart[i] + fVwireidstart[i];

//    cout<<ch<<" "
//	<<wireid<<" "
//	<<fVchstart[i]<<" "
//	<<fVchstop[i]<<" "
//	<<fVwireidstart[i]<<" "
//	<<endl;
    
    break;
  }
  
  return wireid;
  
}
//_________________________________________________
Int_t TDCEffPlot_NEOLITHs::FindHitLayer(Int_t geo, Int_t ch)
{
  for (int i=0;i<fVLayerID.size();++i){
    if (geo != fVgeo[i]) continue;

    if (ch < fVchstart[i]) continue;
    if (ch > fVchstop[i]) continue;
    
    return fVLayerID[i];
  }
  
  return -1;
  
}
//_________________________________________________
//_________________________________________________



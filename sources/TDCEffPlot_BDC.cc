#include "TDCEffPlot_BDC.hh"
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
#include "TDirectory.h"

#include "TArtEventStore.hh"
#include "TArtRawEventObject.hh"
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"

using namespace std;
//_________________________________________________
TDCEffPlot_BDC::TDCEffPlot_BDC(Int_t iBDC)
  : fBDCid(iBDC)
{
  fROOTfileName = Form("eff_bdc%i.root",fBDCid);

  smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(Form("db/SAMURAIBDC%i.xml",fBDCid));
  if       (fBDCid==1) fCalibBDC1Hit = new TArtCalibBDC1Hit;
  else if  (fBDCid==2) fCalibBDC2Hit = new TArtCalibBDC2Hit;

  fNlayer = 8;
  fVLayerName[0] = Form("BDC%i X1", fBDCid);
  fVLayerName[1] = Form("BDC%i X1P",fBDCid);
  fVLayerName[2] = Form("BDC%i Y1", fBDCid);
  fVLayerName[3] = Form("BDC%i Y1P",fBDCid);
  fVLayerName[4] = Form("BDC%i X2", fBDCid);
  fVLayerName[5] = Form("BDC%i X2P",fBDCid);
  fVLayerName[6] = Form("BDC%i Y2", fBDCid);
  fVLayerName[7] = Form("BDC%i Y2P",fBDCid);
  
}
//_________________________________________________
TDCEffPlot_BDC::~TDCEffPlot_BDC()
{
}
//_________________________________________________
void TDCEffPlot_BDC::AnalyzeRun(Int_t nRun, Long64_t neve)
{
  TArtEventStore estore;
  if (nRun==0){
    estore.Open(0);// online data
  }else{
    estore.Open(Form("%s/%s%04d.%s",fRIDFfileDir.Data(),
		     fRIDFfileName.Data(),nRun,fRIDFfileExt.Data()));
  }
  TArtRawEventObject *rawevent = estore.GetRawEventObject();

  TFile fout(Form("%s/%s_ana%04d.root",fROOTfileDir.Data(),
		  fRIDFfileName.Data(),nRun),"RECREATE");

  // for check
  TH1* hidtl1 = new TH2D("hidtl1",Form("BDC%i ID Tleading",fBDCid),128,0.5,128.5, 100,0,30000);
  
  std::vector<TH1*> hmulti;
  std::vector<TH1*> htl;
  std::vector<TH1*> htt;
  std::vector<TH1*> htot;

  TH1* htmp =0;

  // multipliticy
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("hmulti%i",ilayer),Form("Multiplicity BDC%i Layer=%i %s",fBDCid,ilayer, lname.Data()),
		   10,-0.5,9.5);
    hmulti.push_back(htmp);
  }
  
  // T leading
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htl%i",ilayer),Form("BDC%i T leading Layer=%i %s",fBDCid,ilayer, lname.Data()),
			   2000,-5000,5000);
    htl.push_back(htmp);
  }

  // T trailing
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htt%i",ilayer),Form("BDC%i T trailing Layer=%i %s",fBDCid,ilayer, lname.Data()),
		    2000,-5000,5000);
    htt.push_back(htmp);
  }

  // TOT
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htot%i",ilayer),Form("BDC%i ToT Layer=%i %s",fBDCid,ilayer, lname.Data()),
		   2000,0,20000);
    htot.push_back(htmp);
  }

  std::vector<Int_t> multi(fNlayer);
  
  int ieve = 0;
  while(estore.GetNextEvent()){

    if (ieve%10==0)
      if (neve==0) cout<<"\r events = "<<ieve<<flush;
      else	   cout<<"\r events = "<<ieve
		       <<" / "<<neve<<flush;

    for (int i=0;i<fNlayer;++i) multi[i]=0;


    if      (fBDCid==1) fCalibBDC1Hit->ReconstructData();
    else if (fBDCid==2) fCalibBDC2Hit->ReconstructData();
    
    // BDC Hit
    TClonesArray *hit_array = 0;
    if      (fBDCid==1) hit_array = fCalibBDC1Hit->GetDCHitArray();
    else if (fBDCid==2) hit_array = fCalibBDC2Hit->GetDCHitArray();

    int n=hit_array->GetEntries();
    for (int i=0;i<n;++i){
      TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
      Double_t id = hit->GetID();
      Int_t layer = hit->GetLayer();
      Double_t tlraw = hit->GetTDC();
      Double_t ttraw = hit->GetTrailTDC();

      htl[layer]->Fill(tlraw);
      htt[layer]->Fill(ttraw);
      
      if (tlraw>0){
	multi[layer]++;
      }

    }

    for (int ilayer=0;ilayer<fNlayer;++ilayer) {
      hmulti[ilayer]->Fill(multi[ilayer]);
    }

    estore.ClearData();
    if      (fBDCid==1) fCalibBDC1Hit->ClearData();
    else if (fBDCid==2) fCalibBDC2Hit->ClearData();
    ieve++;

    if (neve>0){
      if (ieve>=neve) break;
    }
    
  }

  if (neve==0) cout<<"\r events = "<<ieve<<flush;
  else         cout<<"\r events = "<<ieve
		   <<" / "<<neve<<flush;
  cout<<endl;

  //show analyzed results
  cout<<endl;
  Int_t Ntrig=ieve;
  printf("#Name    : Sum(M) M1    M2    M3    M4    M5+   MW1   MW2   MW3   MW4+  MC1   MC2   MC3   MC4+  :Tot/Ana: %d %d\n",
	 Ntrig,Ntrig);

  for(int ilayer=0;ilayer<fNlayer;++ilayer){
    double eff[kMmax];
    for(int imulti=0;imulti<kMmax-1;imulti++){
      eff[imulti] = GetEffMulti(ilayer,imulti);
    }
    double eff_Mgeof = GetEffMultiGeN(ilayer,kMmax-1);
    //      nm      Mall  M1    M2    M3    M4    M5+   MW1   MW2   MW3   MW4+  MC1   MC2   MC3   MC4+
    printf("%-8s :  %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n",
	   fVLayerName[ilayer].Data(),
	   eff[0],eff[1],eff[2],eff[3],eff[4],eff_Mgeof,
	   0.,0.,0.,0.,// not implemented
	   0.,0.,0.,0.);// not implemented
    
  }
  cout<<endl;
  
  fout.Write();
  fout.Close();

}
//_________________________________________________
void TDCEffPlot_BDC::Plot(Int_t layer)
{
  if (fOutFile==0){
    fOutFile = new TFile(Form("%s/%s",fROOTfileDir.Data(), fROOTfileName.Data()),"recreate");
    fPlotItems = new TList();
  }
  fOutFile->cd();

  // efficiency curve for each layer
  TCanvas *c1 = new TCanvas();
  c1->SetName(Form("c%d",layer));
  c1->SetTitle(fVLayerName[layer].Data());
  TList *tlist = new TList();
  tlist->SetName(fVLayerName[layer].Data());
  
  TGraph *g[kMmax+1];
  for (int i=0;i<kMmax-1;++i){
    g[i] = MakeGraph(layer,i);
  }
  g[kMmax-1] = MakeGraph_MgeN(layer,kMmax-1);
  
  for (int i=0;i<kMmax;++i) {
    tlist->Add(g[i]);
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
    if (i==kMmax-1) text = Form("M>=%d",kMmax-1);
    TText *t = new TText(x+0.1,y,text.Data());
    t->SetNDC(1);
    t->SetTextAlign(12);
    tlist->Add(t);
    y -= 0.05;
  }

  g[kMmax] = MakeGraph_MgeN(layer, 3);
  g[kMmax]->SetLineColor(2);
  g[kMmax]->SetLineStyle(2);
  g[kMmax]->SetMarkerColor(2);
  tlist->Add(g[kMmax]);
  TLine *l = new TLine(x,y,x+0.05,y);
  l->SetLineColor(2);
  l->SetLineStyle(2);
  l->SetNDC(1);
  tlist->Add(l);

  TText *t;
  t = new TText(x+0.1,y,"M>=3");
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
void TDCEffPlot_BDC::PlotSummary(TString dir)
{
  if (fOutFile==0){
    fOutFile = new TFile(Form("%s/%s",fROOTfileDir.Data(), fROOTfileName.Data()),"recreate");
    fPlotItems = new TList();
  }
  fOutFile->cd();

  // efficiency curve summary
  TCanvas *c1 = new TCanvas();
  TString str_temp = Form("c%s",dir.Data());
  str_temp.ToLower();
  c1->SetName(str_temp.Data());
  c1->SetTitle(Form("BDC%i %s",fBDCid,dir.Data()));
  TList *tlist = new TList();
  tlist->SetName(Form("bdc%i%s",fBDCid,dir.Data()));

  TH1* hframe = 0;
  int icol=0;
  TGraph *g[2];// Mall & M1
  for (int layer=0;layer<fNlayer;++layer){

    if (!fVLayerName[layer].Contains(dir)) continue;
    icol++;
    
    for (int i=0;i<2;++i){
      g[i] = MakeGraph(layer,i);

      if (hframe==0 && i==0){
	hframe = g[0]->GetHistogram();
	tlist->Add(hframe);
	hframe->SetTitle(Form("BDC%i %s layers",fBDCid,dir.Data()));
      }

      tlist->Add(g[i]);
      g[i]->SetLineColor(fPalette[icol]);
      g[i]->SetMarkerColor(fPalette[icol]);
      if (i==1) g[i]->SetLineStyle(2);
    }

    double x = 0.12;
    double y = 0.85;
  
    for (int layer=0;layer<fNlayer;++layer){

      if (!fVLayerName[layer].Contains(dir)) continue;
      icol++;

      for (int i=0;i<2;++i){
	TLine *l = new TLine(x,y,x+0.05,y);
	l->SetLineColor(fPalette[icol]);
	if (i==1) l->SetLineStyle(2);
	l->SetNDC(1);
	tlist->Add(l);

	TString text(fVLayerName[layer].Data());
	if (i==1) text += " M1";
	TText *t = new TText(x+0.07,y,text.Data());
	t->SetNDC(1);
	t->SetTextSize(0.03);
	t->SetTextAlign(12);
	tlist->Add(t);
	y -= 0.05;
      }
    }

  }
  
  hframe->Draw("AP");
  tlist->Draw("PL");

  Write(tlist);
  Write(c1);

}
//_________________________________________________

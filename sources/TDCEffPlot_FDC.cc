#include "TDCEffPlot_FDC.hh"
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
#include "TPad.h"
#include "TDirectory.h"

#include "TArtEventStore.hh"
#include "TArtRawEventObject.hh"
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"

using namespace std;
//_________________________________________________
TDCEffPlot_FDC::TDCEffPlot_FDC(Int_t iFDC)
  : TDCEffPlot(), fFDCid(iFDC)
{
  fROOTfileName = Form("eff_fdc%i.root",fFDCid);

  smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(Form("db/SAMURAIFDC%i.xml",fFDCid));
  if       (fFDCid==1) fCalibFDC1Hit = new TArtCalibFDC1Hit;
  else if  (fFDCid==2) fCalibFDC2Hit = new TArtCalibFDC2Hit;

  fNlayer = 14;
  fVLayerName[0]  = Form("FDC%i X1", fFDCid);
  fVLayerName[1]  = Form("FDC%i X1P",fFDCid);
  fVLayerName[2]  = Form("FDC%i U1", fFDCid);
  fVLayerName[3]  = Form("FDC%i U1P",fFDCid);
  fVLayerName[4]  = Form("FDC%i V1", fFDCid);
  fVLayerName[5]  = Form("FDC%i V1P",fFDCid);
  fVLayerName[6]  = Form("FDC%i X2", fFDCid);
  fVLayerName[7]  = Form("FDC%i X2P",fFDCid);
  fVLayerName[8]  = Form("FDC%i U2", fFDCid);
  fVLayerName[9]  = Form("FDC%i U2P",fFDCid);
  fVLayerName[10] = Form("FDC%i V2", fFDCid);
  fVLayerName[11] = Form("FDC%i V2P",fFDCid);
  fVLayerName[12] = Form("FDC%i X3", fFDCid);
  fVLayerName[13] = Form("FDC%i X3P",fFDCid);

  fHistfileName = Form("fdc%ihist",fFDCid);

  if       (fFDCid==1) fNwire = 448;
  else if  (fFDCid==2) fNwire = 1568;
  
}
//_________________________________________________
TDCEffPlot_FDC::~TDCEffPlot_FDC()
{
}
//_________________________________________________
void TDCEffPlot_FDC::AnalyzeRun(Int_t nRun, Long64_t neve)
{

  TString foutname = Form("%s/%s_%s_%04d.root",fROOTfileDir.Data(),
			  fRIDFfileName.Data(),
			  fHistfileName.Data(),
			  nRun);

  std::ifstream fchk(foutname.Data());
  if (fchk.good()){
    std::cout<<"File exists: "<<foutname.Data()<<std::flush;
    if (!ROOTfileOverwrite()){
      std::cout<<" -> Skip"<<std::endl;
      return;
    }else{
      std::cout<<" -> Update"<<std::endl;
    }
  }
  
  TArtEventStore estore;
  if (nRun==0){
    estore.Open(0);// online data
  }else{
    estore.Open(Form("%s/%s%04d.%s",fRIDFfileDir.Data(),
		     fRIDFfileName.Data(),nRun,fRIDFfileExt.Data()));
  }
  TArtRawEventObject *rawevent = estore.GetRawEventObject();

  TFile fout(foutname.Data(),"RECREATE");

  // for check
  TH1* hidtl = new TH2D("hidtl",Form("FDC%i ID Tleading",fFDCid),fNwire,0.5,0.5+fNwire, 100,0,30000);
  
  std::vector<TH1*> hmulti;
  std::vector<TH1*> htl;
  std::vector<TH1*> htt;
  std::vector<TH1*> htot;

  TH1* htmp =0;

  // multipliticy
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("hmulti%i",ilayer),Form("Multiplicity FDC%i Layer=%i %s",fFDCid,ilayer, lname.Data()),
		   10,-0.5,9.5);
    hmulti.push_back(htmp);
  }
  
  // T leading
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htl%i",ilayer),Form("FDC%i T leading Layer=%i %s",fFDCid,ilayer, lname.Data()),
			   2000,-5000,5000);
    htl.push_back(htmp);
  }

  // T trailing
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htt%i",ilayer),Form("FDC%i T trailing Layer=%i %s",fFDCid,ilayer, lname.Data()),
		    2000,-5000,5000);
    htt.push_back(htmp);
  }

  // TOT
  for (int ilayer=0;ilayer<fNlayer;++ilayer){
    TString lname = fVLayerName[ilayer];
    htmp = new TH1D(Form("htot%i",ilayer),Form("FDC%i ToT Layer=%i %s",fFDCid,ilayer, lname.Data()),
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


    if      (fFDCid==1) fCalibFDC1Hit->ReconstructData();
    else if (fFDCid==2) fCalibFDC2Hit->ReconstructData();
    
    // FDC Hit
    TClonesArray *hit_array = 0;
    if      (fFDCid==1) hit_array = fCalibFDC1Hit->GetDCHitArray();
    else if (fFDCid==2) hit_array = fCalibFDC2Hit->GetDCHitArray();

    int n=hit_array->GetEntries();
    for (int i=0;i<n;++i){
      TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
      Double_t id = hit->GetID();
      Int_t layer = hit->GetLayer();
      Double_t tlraw = hit->GetTDC();
      Double_t ttraw = hit->GetTrailTDC();

      hidtl->Fill(id,tlraw);
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
    if      (fFDCid==1) fCalibFDC1Hit->ClearData();
    else if (fFDCid==2) fCalibFDC2Hit->ClearData();
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

  cout<<"Histograms are saved in "<<fout.GetName()<<endl;
  
}
//_________________________________________________
void TDCEffPlot_FDC::Plot(Int_t layer)
{
  if (fOutFile==0){
    fOutFile = new TFile(Form("%s/%s",fROOTfileDir.Data(), fROOTfileName.Data()),"recreate");
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
void TDCEffPlot_FDC::PlotSummary(TString dir)
{
  if (fOutFile==0){
    fOutFile = new TFile(Form("%s/%s",fROOTfileDir.Data(), fROOTfileName.Data()),"recreate");
  }
  fOutFile->cd();

  // efficiency curve summary
  TCanvas *c1 = new TCanvas();
  TString str_temp = Form("c%s",dir.Data());
  str_temp.ToLower();
  c1->SetName(str_temp.Data());
  c1->SetTitle(Form("FDC%i %s",fFDCid,dir.Data()));

  TH1* hframe = 0;

  TList *tlist = new TList();
  tlist->SetName(Form("fdc%i%s",fFDCid,dir.Data()));

  int icol=0;
  TGraph *g[2];// Mall & M1
  for (int layer=0;layer<fNlayer;++layer){

    if (!fVLayerName[layer].Contains(dir)) continue;
    icol++;
    
    for (int i=0;i<2;++i){
      g[i] = MakeGraph(layer,i);
      if (i==0) g[i]->SetName(Form("gfdc%i_%i",fFDCid,layer));
      else      g[i]->SetName(Form("gfdc%i_%i_m1",fFDCid,layer));
      g[i]->SetMinimum(0);
      g[i]->SetMaximum(110);

      g[i]->SetLineColor(fPalette[icol]);
      g[i]->SetMarkerColor(fPalette[icol]);
      if (i==1) g[i]->SetLineStyle(2);
      
      if (hframe==0){
	g[0]->Draw();
	gPad->Update();

	hframe = g[0]->GetHistogram();
	hframe->SetTitle(Form("FDC%i %s layers",fFDCid,dir.Data()));
	hframe->SetMinimum(0);
	hframe->SetMaximum(110);

	hframe->Draw();
      }

      g[i]->Draw("PL");
      fOutFile->Add(g[i]);
    }

    double x = 0.12;
    double y = 0.85;

    int icol=0;
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
  tlist->Draw();

  Write(tlist);
  Write(c1);

  cout<<"Efficiency curves ("<<c1->GetName()<<") for "<<dir.Data()
      <<" planes are saved in "<<fOutFile->GetName()<<endl;
  
}
//_________________________________________________

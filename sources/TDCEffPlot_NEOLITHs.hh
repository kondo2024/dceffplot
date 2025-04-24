#ifndef TDCEFFPLOT_NEOLITHS_HH
#define TDCEFFPLOT_NEOLITHS_HH

#include "TDCEffPlot.hh"
#include "TGraph.h"
#include "TFile.h"

class TDCEffPlot_NEOLITHs : public TDCEffPlot
{
public:
  TDCEffPlot_NEOLITHs();
  virtual ~TDCEffPlot_NEOLITHs();

  virtual void LoadRunListFile(TString fname);
  virtual void LoadMapFile(TString fname);
  virtual void AnalyzeAll();
  virtual void AnalyzeRun(Int_t nRun);
  virtual void Plot(Int_t layer);
  virtual void Write(TObject *obj);
  virtual TGraph* MakeGraph(Int_t layer, Int_t multi);
  virtual TGraph* MakeGraph_Mge3(Int_t layer);

  Int_t FindWireID(Int_t geo,Int_t ch);
  Int_t FindHitLayer(Int_t geo,Int_t ch);
  Int_t GetTref(Int_t geo);
  
protected:
  // run list
  std::vector<int> fVRunNum;
  std::vector<double> fVhv;

  // for map
  std::vector<int> fVLayerID;// index is ilist
  std::vector<int> fVgeo;
  std::vector<int> fVchstart;
  std::vector<int> fVchstop;
  std::vector<int> fVwireidstart;
  std::vector<int> fVwireidstop;

  int fNlayer;

  int fPalette[10];

  static const int kMmax = 6;
  
  std::map<int,TString> fVLayerName;// key is layer num
  std::map<int,int> fTrefmap;// pair of (geo,ch)
  std::map<int,int> fTrefval;// pair of (geo,tdc value)

  TFile *fOutFile;
  TList *fPlotItems;
  
  ClassDef(TDCEffPlot_NEOLITHs, 1)
};

#endif

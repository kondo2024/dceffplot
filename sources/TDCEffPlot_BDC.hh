#ifndef TDCEFFPLOT_BDC_HH
#define TDCEFFPLOT_BDC_HH

#include "TDCEffPlot.hh"
#include "TGraph.h"
#include "TFile.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibBDC1Hit.hh"

class TDCEffPlot_BDC : public TDCEffPlot
{
public:
  TDCEffPlot_BDC();
  virtual ~TDCEffPlot_BDC();

  virtual void LoadRunListFile(TString fname);
  virtual void AnalyzeAll();
  virtual void AnalyzeRun(Int_t nRun, Long64_t neve=0);
  virtual void Plot(Int_t layer);
//  virtual void Write(TObject *obj);
//  virtual Double_t GetEffMulti(Int_t layer, Int_t multi);
//  virtual Double_t GetEffMultiGeN(Int_t layer, Int_t multi);
//  virtual TGraph* MakeGraph(Int_t layer, Int_t multi);
//  virtual TGraph* MakeGraph_MgeN(Int_t layer, Int_t multi);

protected:
  TArtSAMURAIParameters* smprm;
  TArtCalibBDC1Hit *fCalibBDC1Hit;
  
  static const int kMmax = 6;

  TFile *fOutFile;
  TList *fPlotItems;
  
  ClassDef(TDCEffPlot_BDC, 1)
};

#endif

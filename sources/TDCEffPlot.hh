/*
  base class of DC efficiency plot
*/

#ifndef TDCEFFPLOT_HH
#define TDCEFFPLOT_HH

#include "TObject.h"
#include "TFile.h"
#include "TString.h"
#include "TGraph.h"
#include <map>

class TDCEffPlot : public TObject
{
public:
  TDCEffPlot();
  virtual ~TDCEffPlot();

  virtual void LoadRunListFile(TString fname);
  virtual void AnalyzeAll();
  virtual void AnalyzeRun(Int_t irun, Long64_t neve=0) = 0;
  virtual void Plot(Int_t layer) = 0;
  virtual void Write(TObject* obj);

  virtual Double_t GetEffMulti(Int_t layer, Int_t multi);
  virtual Double_t GetEffMultiGeN(Int_t layer, Int_t multi);
  virtual TGraph* MakeGraph(Int_t layer, Int_t multi);
  virtual TGraph* MakeGraph_MgeN(Int_t layer, Int_t multi);
  
  void SetRIDFfileDir(const char* dir){fRIDFfileDir = dir;}
  void SetRIDFfileName(const char* fname){fRIDFfileName = fname;}
  void SetRIDFfileExt(const char* ext){fRIDFfileExt = ext;}
  void SetROOTfileDir(const char* dir){fROOTfileDir = dir;}
  void SetROOTfileName(const char* fname){fROOTfileName = fname;}
  void SetHistName(const char* hname){fHistName = hname;}

  void SetROOTfileOverwrite(bool tf){fROOTfileOverwrite = tf;}
  bool ROOTfileOverwrite(){return fROOTfileOverwrite;}
  
protected:
  TString fRIDFfileDir;
  TString fRIDFfileName;
  TString fRIDFfileExt;
  TString fROOTfileDir;
  TString fROOTfileName;
  TList* fFilenameList;
  TFile *fOutFile;
  TString fHistName;
  Bool_t fROOTfileOverwrite;

  TString fMapFilename;
  TString fRunListFilename;

  
  // run list
  std::vector<int> fVRunNum;
  std::vector<double> fVhv;

  // layer name
  int fNlayer;
  std::map<int,TString> fVLayerName;// key is layer num

  std::vector<int> fPalette;
  
  ClassDef(TDCEffPlot, 1)
};

#endif

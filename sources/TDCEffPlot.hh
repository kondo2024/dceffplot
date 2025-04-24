/*
  base class of DC efficiency plot
*/

#ifndef TDCEFFPLOT_HH
#define TDCEFFPLOT_HH

#include "TObject.h"
#include "TString.h"
#include <map>

class TDCEffPlot : public TObject
{
public:
  TDCEffPlot();
  virtual ~TDCEffPlot();

  virtual void LoadRunListFile(TString fname) = 0;
  virtual void LoadMapFile(TString fname) = 0;
  virtual void AnalyzeAll() = 0;
  virtual void Plot(Int_t layer) = 0;
  virtual void Write(TObject* obj) = 0;
  
protected:
  TString fRIDFfileDir;
  TString fRIDFfileName;
  TString fROOTfileDir;
  TList* fFilenameList;

  TString fMapFilename;
  TString fRunListFilename;

  std::map<int,TString> fLayerName_map;

  ClassDef(TDCEffPlot, 1)
};

#endif


void run(){

  gSystem->Load("../../install/lib/libdceffplot.so");

  TDCEffPlot_NEOLITHs dcplot;
  dcplot.SetRIDFfileDir("ridf/");
  dcplot.SetRIDFfileName("data");
  dcplot.SetROOTfileDir("root/");
  dcplot.SetROOTfileName("eff_NEOLITHs.root");
  
  dcplot.LoadRunListFile("runlist.txt");
  dcplot.LoadMapFile("map_titech.txt");

  dcplot.AnalyzeAll();

  dcplot.Plot(0);// layer = 0
  dcplot.Plot(1);
  dcplot.Plot(2);
  
}

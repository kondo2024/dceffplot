/* usage
   root[] .L dceffplot/install/lib/dceffplot.so
   root[] .x macros/dceffplot/run_fdc1.cc

 */
void run_fdc1(){

  TDCEffPlot_FDC dcplot(1);// 1: FDC1, 2: FDC2
  dcplot.SetRIDFfileDir("ridf/sdaq04");

  // change these parameters accordingly
  dcplot.SetRIDFfileName("data");
  dcplot.SetRIDFfileExt("ridf.gz");
  dcplot.SetROOTfileDir("rootfiles/dceff");
  dcplot.SetROOTfileName("eff_fdc1.root");

  //dcplot.SetROOTfileOverwrite(false);// uncomment if you'd like to skip re-analysis

  dcplot.LoadRunListFile("macros/dceffplot/runlist_fdc1.txt");

  dcplot.AnalyzeAll();


  dcplot.PlotSummary("X");
  dcplot.PlotSummary("U");
  dcplot.PlotSummary("V");

  // for checking more details for each layer  
//  for (int i=0;i<14;++i) dcplot.Plot(i);

}

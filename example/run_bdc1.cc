/* usage
   root[] .L dceffplot/install/lib/dceffplot.so
   root[] .x macros/dceffplot/run_bdc1.cc

 */
void run_bdc1(){

  TDCEffPlot_BDC dcplot(1);// 1: BDC1, 2: BDC2
  dcplot.SetRIDFfileDir("ridf/sdaq04");

  // change these parameters accordingly
  dcplot.SetRIDFfileName("data");
  dcplot.SetRIDFfileExt("ridf.gz");
  dcplot.SetROOTfileDir("rootfiles/dceff");
  dcplot.SetROOTfileName("eff_bdc1.root");

  //dcplot.SetROOTfileOverwrite(false);// uncomment if you'd like to skip re-analysis

  dcplot.LoadRunListFile("macros/dceffplot/runlist_bdc1.txt");

  dcplot.AnalyzeAll();


  dcplot.PlotSummary("X");
  dcplot.PlotSummary("Y");

  // for checking more details for each layer  
//  for (int i=0;i<8;++i) dcplot.Plot(i);

}

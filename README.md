# dceffplot

analyze run and make plot of efficiency curve of a drift chamber.

## Requirements
- ROOT v6
- ANAROOT

## How to install
```
$ cd dceffplot
```
If TARTSYS is not defined, run setup.sh of ANAROOT

```
$ mkdir build
$ mkdir install
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../install ../sources
$ make install
```

## How to use
1. modify runlist.txt
1. modify run_xxx.cc for your directories
1. $ root[] .L dceffplot/install/lib/dceffplot.so
1. $ root[] run_xxx.cc

(in case of NEOLITH)
- modify map.txt

### example of runlist.txt
```
#Run HV
137 600
138 625
139 650
140 675
```

### example: run_bdc1.cc
``` c++
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
```


Then, efficiency plots will be created.

example:

![plot_example](plot_example.png)

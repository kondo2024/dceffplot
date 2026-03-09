# dceffplot

analyze run and make plot of efficiency curve of a drift chamber.

## Requirements
- ROOT v6
- ANAROOT

## how to install
<code>$ cd dceffplot

in case TARTSYS is not defined, modify setup.sh and run it.(<code>$ source setup.sh</code>)

<code>$ mkdir build
$ mkdir install
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../install ../sources
$ make install</code>

## how to use
1. modify runlist.txt
1. modify run_xxx.cc for your directories
1. $ root[] .L dceffplot/install/lib/dceffplot.so
1. $ root[] run_xxx.cc

(in case of NEOLITH)
- modify map.txt


Then, efficiency plots will be created.


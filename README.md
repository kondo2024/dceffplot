# dceffplot

analyze and make plot of efficiency curve of a drift chamber.

## Requirements
- ROOT v6
- ANAROOT

## how to install
<code>$ cd dceffplot
$ emacs setup.sh (modify environmental values)</code>
in case TARTSYS is not defined, modify setup.sh and run it.
<code>$ source setup.sh</code>
<code>$ mkdir build
$ mkdir install
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../install ../sources
$ make install</code>

## how to use
1. modify map.txt and runlist.txt for your setup
1. modify run.cc for your directories
1. $ root run.cc

Then, efficiency plots will be created.


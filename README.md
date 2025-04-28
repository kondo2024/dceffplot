# dceffplot

analyze and make plot of efficiency curve of a drift chamber.

## how to install
<code>$ cd dceffplot
$ emacs setup.sh (modify environmental values)
$ source setup.sh
$ mkdir build
$ mkdir install
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../install ../sources
$ make install
</code>

## how to use
1. modify map.txt and runlist.txt for your setup
1. modify run.cc for your directories
1. $ root run.cc

Then, efficiency plots will be created.


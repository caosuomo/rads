#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat simproj2.txt
echo -----------------------------------------------------------------
echo "Press ENTER to create the chain complex of the simplicial complex: "
read XXXX
echo -----------------------------------------------------------------

simchain simproj2.sim $HOME/simproj2.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z of this chain complex: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/simproj2.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z_2 of this chain complex: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/simproj2.chn -p 2

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z_13 of the simplicial complex: "
read XXXX
echo -----------------------------------------------------------------

homsimpl simproj2.sim -p 13

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/simproj2.chn

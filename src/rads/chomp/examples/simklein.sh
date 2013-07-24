#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat simklein.txt
echo -----------------------------------------------------------------
echo "Press ENTER to create the chain complex of the simplicial complex: "
read XXXX
echo -----------------------------------------------------------------

simchain simklein.sim $HOME/simklein.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z of this chain complex: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/simklein.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z_2 of this chain complex: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/simklein.chn -p 2

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z_13 of the simplicial complex: "
read XXXX
echo -----------------------------------------------------------------

homsimpl simklein.sim -p 13

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/simklein.chn

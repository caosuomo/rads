#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat simtorus.txt
echo -----------------------------------------------------------------
echo "Press ENTER to create the chain complex of the simplicial complex: "
read XXXX
echo -----------------------------------------------------------------

simchain simtorus.sim $HOME/simtorus.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of this chain complex: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/simtorus.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the simplicial complex directly: "
read XXXX
echo -----------------------------------------------------------------

homsimpl simtorus.sim

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/simtorus.chn

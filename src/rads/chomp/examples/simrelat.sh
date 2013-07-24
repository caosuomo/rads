#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat simrelat.txt
echo -----------------------------------------------------------------
echo "Press ENTER to create the chain complex of the given pair: "
read XXXX
echo -----------------------------------------------------------------

simchain simrelat.sim simrelat.rel $HOME/simrelat.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of this chain complex: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/simrelat.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the simplicial pair directly: "
read XXXX
echo -----------------------------------------------------------------

homsimpl simrelat.sim simrelat.rel

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/simrelat.chn

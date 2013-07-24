#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat relative.txt
echo -----------------------------------------------------------------
echo "Press ENTER to extract the cubical sets X and Y from the picture: "
read XXXX
echo -----------------------------------------------------------------

bmp2pset relative.bmp $HOME/relative.cu1 $HOME/relative.cu2

echo -----------------------------------------------------------------
echo "Press ENTER to compute the relative homology of the pair X, Y: "
read XXXX
echo -----------------------------------------------------------------

homcubes_rads $HOME/relative.cu1 $HOME/relative.cu2

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/relative.cu1
rm -f $HOME/relative.cu2

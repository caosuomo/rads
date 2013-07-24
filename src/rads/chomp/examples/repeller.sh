#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat repeller.txt
echo -----------------------------------------------------------------
echo "Press ENTER to find an index pair with an algorithm by A. Szymczak: "
read XXXX
echo -----------------------------------------------------------------

cnvmvmap repeller.map $HOME/repeller.mp
echo -----------------------------------------------------------------
indxpair repeller.cub $HOME/repeller.mp $HOME/repeller.q1 $HOME/repeller.q0

echo -----------------------------------------------------------------
echo "Press ENTER to compute the Conley index of the index pair found: "
read XXXX
echo -----------------------------------------------------------------

homcubes -i repeller.map $HOME/repeller.q1 $HOME/repeller.q0

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/repeller.mp
rm -f $HOME/repeller.q0
rm -f $HOME/repeller.q1

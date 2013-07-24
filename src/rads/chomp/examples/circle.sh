#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat circle.txt
echo -----------------------------------------------------------------
echo "Press ENTER to check that the given cubical map is almost perfect: "
read XXXX
echo -----------------------------------------------------------------

chkperf circle.map

echo -----------------------------------------------------------------
echo "Press ENTER to create a chain map from the cubical map: "
read XXXX
echo -----------------------------------------------------------------

chmap circle.map $HOME/circle.dat $HOME/circle.cy

echo -----------------------------------------------------------------
echo "Press ENTER to convert the chain map to another format: "
read XXXX
echo -----------------------------------------------------------------

cnvchmap $HOME/circle.dat -y$HOME/circle.cy $HOME/circle.chx $HOME/circle.chy $HOME/circle.chm

echo -----------------------------------------------------------------
echo "Press ENTER to compute the map induced in homology: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/circle.chx $HOME/circle.chy $HOME/circle.chm

echo -----------------------------------------------------------------
echo Now another method of homology computation of maps will be used.
echo "Press ENTER to veryfiy that the map is acyclic: "
read XXXX
echo -----------------------------------------------------------------

chkmvmap -i circle.map

echo -----------------------------------------------------------------
echo "Press ENTER to compute the map induced in homology: "
read XXXX
echo -----------------------------------------------------------------

homcubes -i circle.map

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/circle.dat
rm -f $HOME/circle.cy
rm -f $HOME/circle.chx
rm -f $HOME/circle.chy
rm -f $HOME/circle.chm


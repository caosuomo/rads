#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat vanderpl.txt
echo -----------------------------------------------------------------
echo "Press ENTER to verify that the given map is almost perfect: "
read XXXX
echo -----------------------------------------------------------------

chkperf vanderpl.map

echo -----------------------------------------------------------------
echo "Press ENTER to create a suitable chain map: "
read XXXX
echo -----------------------------------------------------------------

chmap vanderpl.map $HOME/vanderpl.dat $HOME/vanderpl.cy

echo -----------------------------------------------------------------
echo "Press ENTER to create a purely algebraic representation of the map: "
read XXXX
echo -----------------------------------------------------------------

cnvchmap -a $HOME/vanderpl.dat $HOME/vanderpl.chn $HOME/vanderpl.chm

echo -----------------------------------------------------------------
echo "Press ENTER to compute the map induced in homology: "
read XXXX
echo -----------------------------------------------------------------

homchain $HOME/vanderpl.chn $HOME/vanderpl.chm

echo -----------------------------------------------------------------
echo "Press ENTER to verify the map if it is appropriate for HomCubes: "
read XXXX
echo -----------------------------------------------------------------

chkmvmap -i vanderpl.map

echo -----------------------------------------------------------------
echo "Press ENTER to compute the map induced in homology: "
read XXXX
echo -----------------------------------------------------------------

homcubes -i vanderpl.map

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/vanderpl.dat
rm -f $HOME/vanderpl.cy
rm -f $HOME/vanderpl.chn
rm -f $HOME/vanderpl.chm

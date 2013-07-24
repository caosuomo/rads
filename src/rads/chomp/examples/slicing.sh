#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat slicing.txt
echo -----------------------------------------------------------------
echo "Press ENTER to slice the given cubical set into 3 parts: "
read XXXX
echo -----------------------------------------------------------------

cubslice slicing.cub $HOME/slice -p 3 -o 1 -s $HOME/sover

echo -----------------------------------------------------------------
echo "Press ENTER to reduce each of the slices separately: "
read XXXX
echo -----------------------------------------------------------------

homcubes $HOME/slice1.cub -k $HOME/sover1.cub -F $HOME/sred1.cub
homcubes $HOME/slice2.cub -k $HOME/sover2.cub -F $HOME/sred2.cub
homcubes $HOME/slice3.cub -k $HOME/sover3.cub -F $HOME/sred3.cub

echo -----------------------------------------------------------------
echo "Press ENTER to glue the reduced portions of the slices: "
read XXXX
echo -----------------------------------------------------------------

psetglue $HOME/sred1.cub $HOME/sred2.cub $HOME/sred3.cub $HOME/sover1.cub $HOME/sover2.cub $HOME/sover3.cub $HOME/sreduced.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the glued reduced slices: "
read XXXX
echo -----------------------------------------------------------------

homcubes $HOME/sreduced.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the original cubical set: "
read XXXX
echo -----------------------------------------------------------------

homcubes slicing.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/slice1.cub
rm -f $HOME/slice2.cub
rm -f $HOME/slice3.cub
rm -f $HOME/sred1.cub
rm -f $HOME/sred2.cub
rm -f $HOME/sred3.cub
rm -f $HOME/sover1.cub
rm -f $HOME/sover2.cub
rm -f $HOME/sover3.cub
rm -f $HOME/sreduced.cub

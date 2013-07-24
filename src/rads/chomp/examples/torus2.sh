#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat torus2.txt
echo -----------------------------------------------------------------
echo "Press ENTER to make an unsuccessful attempt to compute the map: "
read XXXX
echo -----------------------------------------------------------------

homcubes_rads torus2.map

echo -----------------------------------------------------------------
echo "Press ENTER to compute the map induced in homology correctly: "
read XXXX
echo -----------------------------------------------------------------

homcubes_rads -i -a torus2.map

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat dim10.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the cube: "
read XXXX
echo -----------------------------------------------------------------

homcubes dim10.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

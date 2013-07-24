#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat nonred.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the first figure: "
read XXXX
echo -----------------------------------------------------------------

homcubes nonred1.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the second figure: "
read XXXX
echo -----------------------------------------------------------------

homcubes nonred2.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

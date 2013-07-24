#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat qexample.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the Klein bottle: "
read XXXX
echo -----------------------------------------------------------------

homcubes qklein.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the Moebius strip: "
read XXXX
echo -----------------------------------------------------------------

homcubes qmoebius.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the projective plane: "
read XXXX
echo -----------------------------------------------------------------

homcubes qprojpln.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the torus: "
read XXXX
echo -----------------------------------------------------------------

homcubes qtorus.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

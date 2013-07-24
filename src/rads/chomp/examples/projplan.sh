#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat projplan.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z: "
read XXXX
echo -----------------------------------------------------------------

homchain projplan.chn

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology over Z_2: "
read XXXX
echo -----------------------------------------------------------------

homchain projplan.chn -p2

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the cubical representaion: "
read XXXX
echo -----------------------------------------------------------------

homcubes projplan.cel

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

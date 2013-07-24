#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat kleinbot.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the 4-dim set over Z: "
read XXXX
echo -----------------------------------------------------------------

homcubes kleinbot.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the 2-dim set over Z_2: "
read XXXX
echo -----------------------------------------------------------------

homcubes kleinbot.cel -p2

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

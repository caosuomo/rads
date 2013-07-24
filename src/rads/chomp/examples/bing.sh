#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat bing.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the first example: "
read XXXX
echo -----------------------------------------------------------------

homcubes bing1.cel

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the second example: "
read XXXX
echo -----------------------------------------------------------------

homcubes bing2.cel

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

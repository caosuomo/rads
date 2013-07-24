#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat torus.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the torus: "
read XXXX
echo -----------------------------------------------------------------

homchain torus.chs

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

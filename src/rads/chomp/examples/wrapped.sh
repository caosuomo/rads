#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat wrapped.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of the wrapped set of squares: "
read XXXX
echo -----------------------------------------------------------------

homcubes -w10 wrapped.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

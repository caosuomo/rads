#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat wind3.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the map induced in homology: "
read XXXX
echo -----------------------------------------------------------------

homchain wind3_cx.chn wind3_cy.chn wind3map.chm

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

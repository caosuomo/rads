#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat probl.txt
echo -----------------------------------------------------------------
echo "Press ENTER to verify the map: "
read XXXX
echo -----------------------------------------------------------------

chkmvmap -i probl_f.map probl_x.cub probl_a.cub probl_y.cub probl_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to make an unsuccessful attempt of homology computation: "
read XXXX
echo -----------------------------------------------------------------

homcubes -i probl_f.map probl_x.cub probl_a.cub probl_y.cub probl_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of this map in the right way: "
read XXXX
echo -----------------------------------------------------------------

homcubes -a -i probl_f.map probl_x.cub probl_a.cub probl_y.cub probl_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

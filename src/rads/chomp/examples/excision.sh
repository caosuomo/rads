#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat excision.txt
echo -----------------------------------------------------------------
echo "Press ENTER to verify the map: "
read XXXX
echo -----------------------------------------------------------------

chkmvmap -i excis_f.map excis_x.cub excis_a.cub excis_y.cub excis_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to make an unsuccessful attempt of homology computation: "
read XXXX
echo -----------------------------------------------------------------

homcubes -i excis_f.map excis_x.cub excis_a.cub excis_y.cub excis_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology of this map in the right way: "
read XXXX
echo -----------------------------------------------------------------

homcub2l excis_f.map excis_x.cub excis_a.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

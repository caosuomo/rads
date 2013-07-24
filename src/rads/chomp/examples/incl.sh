#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat incl.txt
echo -----------------------------------------------------------------
echo "Press ENTER to verify the cubical map: "
read XXXX
echo -----------------------------------------------------------------

chkmvmap incl_f.map incl_x.cub incl_a.cub incl_y.cub incl_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to try computing the index map (this will fail): "
read XXXX
echo -----------------------------------------------------------------

homcubes -i incl_f.map incl_x.cub incl_a.cub incl_y.cub incl_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to use the map fixed by lifting cubes to different levels: "
read XXXX
echo -----------------------------------------------------------------

homcubes -i inclfx_f.map inclfx_x.cub inclfx_a.cub inclfx_y.cub inclfx_b.cub

echo -----------------------------------------------------------------
echo "Press ENTER to use the program homcub2l: "
read XXXX
echo -----------------------------------------------------------------

homcub2l incl_f.map incl_x.cub incl_a.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

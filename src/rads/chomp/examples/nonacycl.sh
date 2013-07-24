#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat nonacycl.txt
echo -----------------------------------------------------------------
echo "Press ENTER to confirm that the main map is acyclic: "
read XXXX
echo -----------------------------------------------------------------

chkmvmap nonacycl.map

echo -----------------------------------------------------------------
echo "Press ENTER to verify if this map satisfies the assumptions: "
read XXXX
echo -----------------------------------------------------------------

chkmvmap nonacycl.map nonacycl.cub -i

echo -----------------------------------------------------------------
echo "Press ENTER to see what the homcubes program computes for this map: "
read XXXX
echo -----------------------------------------------------------------

homcubes nonacycl.map nonacycl.cub -i

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

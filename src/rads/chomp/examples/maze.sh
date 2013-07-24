#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat maze.txt
echo -----------------------------------------------------------------
echo "Press ENTER to extract black points form the maze - the walls: "
read XXXX
echo -----------------------------------------------------------------

bmp2pset maze.bmp $HOME/maze.cub

echo -----------------------------------------------------------------
echo "Press ENTER to compute the homology od the extracted set: "
read XXXX
echo -----------------------------------------------------------------

homcubes $HOME/maze.cub

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

rm -f $HOME/maze.cub

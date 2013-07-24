#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat num.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homomorphism: "
read XXXX
echo -----------------------------------------------------------------

homcubes -c num.cub -i num_f.num num_x.num num_a.num num_y.num num_b.num

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

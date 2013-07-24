#!/bin/sh
PATH=../bin:$PATH
echo -----------------------------------------------------------------
cat circthin.txt
echo -----------------------------------------------------------------
echo "Press ENTER to compute the homomorphism in homology: "
read XXXX
echo -----------------------------------------------------------------

homcelmp circthin.map circthin.cel

echo -----------------------------------------------------------------
echo "Press ENTER to compute the endomorphism in homology: "
read XXXX
echo -----------------------------------------------------------------

homcelmp circthin.map circthin.cel -i

echo -----------------------------------------------------------------
echo "Press ENTER to finish: "
read XXXX

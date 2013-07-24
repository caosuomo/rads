#!/bin/sh

if [ -d utils ]; then
	cd utils
fi
cd ..

chmod a+rx bin examples python
chmod a+r bin/* examples/* python/*
chmod a+x examples/*.sh

cd bin
zip -u -9 ../chomp_bin chomp
cd ..

zip -u -9 -r chompfull_bin licen* bin/* examples/* python/* -x examples/*.bat


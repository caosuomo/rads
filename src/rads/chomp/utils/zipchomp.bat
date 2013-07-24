@echo off
if not exist utils\* cd ..
if not .%1. == .. goto %1

:: Compress the full version of the CHomP software distribution package.

:full
zip -u -9 chomp-full lib/ bin/ obj/ obj/*/ -x */CVS/
zip -u -9 -r chomp-full * -x _* *.zip bin/* doc/html/* obj/* obj/*/* lib/* utils/Doxyfile.warn
if not .%1. == .. goto end

:: Compress the basic version of the CHomP software distribution package.

:src
zip -u -9 chomp-src lib/ bin/ obj/ obj/*/ -x */CVS/
zip -u -9 -r chomp-src makefile licen* include/ make/ src/ programs/chomprog/* programs/chompdemo/* -x src/multiwork/* include/chomp/multiwork/* include/simplices/*
if not .%1. == .. goto end

:: Compress the CHomP library only.

:lib
zip -u -9 chomp-lib lib/ bin/ obj/ obj/*/ -x */CVS/
zip -u -9 -r chomp-lib makefile licen* include/ make/ src/
if not .%1. == .. goto end

:: Compress the binary packages.

:bin
cd bin
zip -u -9 ../chomp_win32 chomp.exe
cd ..
zip -u -9 -r chompfull_win32 licen* bin/* examples/* python/* -x examples/*.sh

:end

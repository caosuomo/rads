# This is a makefile for compiling programs that use the CHomP library.
# Copyright (C) 2004-2005 by Pawel Pilarczyk.
# This is free software. No warranty. Consult 'license.txt' for details.
# Created on October 26, 2004. Last revision: October 11, 2005.

# Usage: Put this makefile in the directory containing your own source code
# which uses the CHomP library. Then add your source files names in the
# PROGS and OTHERS variables below as space-separated lists.

# Use "make" on its own to make all of your programs or "make spec" to make
# a specific program or sub-package only.
# Note: The CHomP library must already exist and be compiled.


# ===========================================================================
# Programs which can be compiled using this makefile.
# ===========================================================================

# a list of all the programs in your package
PROGS = chom

# a list of all your units to be linked with your programs
OTHERS = bitcodes block complex edge list ncell tree vertex

# the CHomP library or libraries to use (if different from the global one)
LIBRARY =

# define "GUI = 0" for console applications, otherwise GUI is assumed
GUI = 0

# a subdirectory for storing the object files of this program group
SUBDIR = chom


# ===========================================================================
# Include the generic makefile for this project.
# ===========================================================================

include ../../make/makeprog


# That's all, folks!


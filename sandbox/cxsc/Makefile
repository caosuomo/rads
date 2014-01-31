#!/bin/sh
#
# This file is a Makefile.example for programs with the C-XSC library
# ===================================================================
#

#======== modify the following values ==================
PROGRAM=point_test#                program name

#========= the following commands should work on your Unix systems ========

# (un-)installation prefix
# e.g. /usr/local/cxsc or local home directory
PREFIX=/ima/home/jberwald/local/cxsc

CXX=g++#  name of the C++ compiler
CXXOPTS=-Wall -mfpmath=sse -msse2 -O3 -fno-strict-aliasing# optional flags to give to the C++ compiler
CXXINC=-I/ima/home/jberwald/local/cxsc/include -L/ima/home/jberwald/local/cxsc/lib#
                                # additional include path
CXXFLAGS= -Wall -mfpmath=sse -msse2#  extra flags to give to the C++ compiler
LIBRARIES=-lcxsc#           names of libraries
RPATH=-Wl,-R/ima/home/jberwald/local/cxsc/lib#
# 

#========== you shouldn't modify anything below ===========================

.SUFFIXES:
.SUFFIXES: .cpp .hpp .o

default:
	@echo
	@echo "C-XSC - C++ library for eXtended Scientific Computation"
	@echo "Example: $(PROGRAM).cpp with Makefile"
	@echo "Usage: make all | $(PROGRAM)   (use 'gmake' on SUN)"  
	@echo

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).cpp
	$(CXX) -o $(PROGRAM) $(CXXFLAGS) $(CXXINC) $(RPATH) $(PROGRAM).cpp $(LIBRARIES)

	
.PHONY: default all



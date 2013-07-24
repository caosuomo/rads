// Copyright (C) 2004 by William D Kalies. All rights reserved.
//
//-----------------------------------------------------------------------------
// chom.h:
//-----------------------------------------------------------------------------
/*! \file chom.h
    \brief Definitions.

    Version 3.0

    Include this file in all .cc files.
    Last modified by BK on 1/22/02.
*/

#ifndef CHOM_H
#define CHOM_H

//#include <iostream.h>
#include <new>
#include <list>
#include <utility>
#include <iterator>
#include <algorithm>
#include <functional>
#include <fstream>
#include <stack>

//#include <stdio.h>
#include <cstdio>
//#include <math.h>
#include <cmath>
//#include <stdlib.h>
#include <cstdlib>
//#include <malloc.h>
//#include <string.h>
#include <cstring>
//#include <time.h>
#include <ctime>
#include <sstream>
#include <string>
#include <iostream>

#include "dim.h"

//-----------------------------------------------------------------------------
// NAMING CONVENTIONS:
//   #define macros: all caps and underscore _ between words
//   Types and variables: no caps and underscore _ between words
//   Functions: Capitalize first letter of each word and no underscores
//-----------------------------------------------------------------------------

/*! \def DIM
    \brief Dimension of the cubical complex.
*/

//#define DIM 3

using namespace std;

//extern int periodic; // 0=off, 1=first DIM-1 variables, 2=all DIM variables
extern int cube_bits;
extern int gen_flag[DIM+1];
extern int Power2(int);
extern int GEN_TOL;

#endif

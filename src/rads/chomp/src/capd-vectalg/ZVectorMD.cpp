/// @addtogroup vectalg
/// @{
/////////////////////////////////////////////////////////////////////////////
/// @file ZVectorMD.cpp
///
/// @author The CAPD Group
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2000-2005 by the CAPD Group.
//
// This file constitutes a part of the CAPD library, 
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.wsb-nlu.edu.pl/ for details. 

#include <iostream>
#include "capd/vectalg/Vector.hpp"

namespace chomp{ 
  namespace vectalg{


template class Vector<int,0>;

template Vector<int,0> abs<int,0> (const Vector<int,0> &v);
template Vector<int,0> operator- <int,0> (const Vector<int,0> &v);
template Vector<int,0> operator+ <int,0> (const Vector<int,0> &v1,const Vector<int,0> &v2);
template Vector<int,0> operator- <int,0> (const Vector<int,0> &v1,const Vector<int,0> &v2);
template int operator* <int,0> (const Vector<int,0> &v1,const Vector<int,0> &v2);

template Vector<int,0> operator* <int,int,0> (const Vector<int,0> &v,const int &s);
template Vector<int,0> operator* <int,int,0> (const int &s,const Vector<int,0> &v);
template Vector<int,0> operator/ <int,int,0> (const Vector<int,0> &v, const int &s);

template Vector<int,0> operator* <int,long,0>(const Vector<int,0> &v,const long &s);
template Vector<int,0> operator* <int,long,0>(const long &s,const Vector<int,0> &v);
template Vector<int,0> operator/ <int,long,0>(const Vector<int,0> &v, const long &s);

template Vector<int,0> operator+ <int,0> (const Vector<int,0> &v,const int &s);
template Vector<int,0> operator- <int,0> (const Vector<int,0> &v,const int &s);
template bool operator < <int,0> (const Vector<int,0> &v1,const Vector<int,0> &v2);
template bool operator > <int,0> (const Vector<int,0> &v1,const Vector<int,0> &v2);
template bool operator<= <int,0> (const Vector<int,0> &v1,const Vector<int,0> &v2);
template bool operator>= <int,0> (const Vector<int,0> &v1,const Vector<int,0> &v2);
template std::ostream &operator<< <int,0> (std::ostream &out, const Vector<int,0> &v);
template std::istream &operator>> <int,0>(std::istream &inp, Vector<int,0> &v);

}}  // end of namespace chomp::vectalg
/// @}

/// @addtogroup vectalg
/// @{
/////////////////////////////////////////////////////////////////////////////
/// @file ZMatrixMD.cpp
///
/// @author The CAPD Group
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2000-2005 by the CAPD Group.
//
// This file constitutes a part of the CAPD library, 
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.wsb-nlu.edu.pl/ for details. 

#include "capd/vectalg/Vector.hpp"
#include "capd/vectalg/Matrix.hpp"

namespace chomp{ 
  namespace vectalg{


template class Matrix<int,0,0>;
template class RowVector<int,0>;
template class ColumnVector<int,0>;

typedef Matrix<int,0,0> ZMatrix;
typedef Vector<int,0> ZVector;
template Matrix<int,0,0> abs <int,0,0> (const Matrix<int,0,0>&);
template Matrix<int,0,0> operator- <int,0,0> (const Matrix<int,0,0>&);
template Matrix<int,0,0> operator+ <int,0,0> (const Matrix<int,0,0>&, const Matrix<int,0,0>&);
template Matrix<int,0,0> operator- <int,0,0> (const Matrix<int,0,0>&, const Matrix<int,0,0>&);
template Matrix<int,0,0> operator* <int,0,0> (const Matrix<int,0,0>&, const Matrix<int,0,0>&);
template Vector<int,0> operator* <int,0,0> (const Matrix<int,0,0>&, const Vector<int,0>&);
template Matrix<int,0,0> operator* <int,0,0> (const Matrix<int,0,0>&, const int&);
template Matrix<int,0,0> operator* <int,0,0> (const int&, const Matrix<int,0,0>&);
template Matrix<int,0,0> operator/ <int,0,0> (const Matrix<int,0,0>&, const int&);
template Matrix<int,0,0> operator+ <int,0,0> (const Matrix<int,0,0>&, const int&);
template Matrix<int,0,0> operator- <int,0,0> (const Matrix<int,0,0>&, const int&);
template bool operator< <int,0,0> (const Matrix<int,0,0>&, const Matrix<int,0,0>&);
template bool operator> <int,0,0> (const Matrix<int,0,0>&, const Matrix<int,0,0>&);
template bool operator<= <int,0,0> (const Matrix<int,0,0>&, const Matrix<int,0,0>&);
template bool operator>= <int,0,0> (const Matrix<int,0,0>&, const Matrix<int,0,0>&);
template Matrix<int,0,0> Transpose <int,0,0> (const Matrix<int,0,0>&);
template std::ostream &operator<< <int,0,0> (std::ostream&, const Matrix<int,0,0>&);
template std::istream &operator>> <int,0,0> (std::istream&, Matrix<int,0,0>&);


// RowVector

template Vector<int,0> operator+<int,0>(
      const Vector<int,0>&,
      const RowVector<int,0>&
   );
template Vector<int,0> operator+<int,0>(
      const RowVector<int,0>&,
      const Vector<int,0>&
   );
template Vector<int,0> operator+<int,0>(
      const RowVector<int,0>&,
      const RowVector<int,0>&
   );

template Vector<int,0> operator-<int,0>(
      const Vector<int,0>&,
      const RowVector<int,0>&
   );
template Vector<int,0> operator-<int,0>(
      const RowVector<int,0>&,
      const Vector<int,0>&
   );
template Vector<int,0> operator-<int,0>(
      const RowVector<int,0>&,
      const RowVector<int,0>&
   );

template int operator*<int,0>(
      const Vector<int,0>&,
      const RowVector<int,0>&
   );
template int operator*<int,0>(
      const RowVector<int,0>&,
      const Vector<int,0>&
   );
template int operator*<int,0>(
      const RowVector<int,0>&,
      const RowVector<int,0>&
   );

template Vector<int,0> operator-<int,0>(const RowVector<int,0>&);
template Vector<int,0> operator*<int,0,0>(
      const Matrix<int,0,0>&, const RowVector<int,0>&
   );

template Vector<int,0> operator*<int,0>(const int&, const RowVector<int,0>&);
template Vector<int,0> operator*<int,0>(const RowVector<int,0>&, const int&);
template Vector<int,0> operator/<int,0>(const RowVector<int,0>&, const int&);

template std::ostream& operator<< <int,0>(std::ostream&, const RowVector<int,0>&);


// ColumnVector

template Vector<int,0> operator+<int,0>(
      const Vector<int,0>&,
      const ColumnVector<int,0>&
   );
template Vector<int,0> operator+<int,0>(
      const ColumnVector<int,0>&,
      const Vector<int,0>&
   );
template Vector<int,0> operator+<int,0>(
      const ColumnVector<int,0>&,
      const ColumnVector<int,0>&
   );

template Vector<int,0> operator-<int,0>(
      const Vector<int,0>&,
      const ColumnVector<int,0>&
   );
template Vector<int,0> operator-<int,0>(
      const ColumnVector<int,0>&,
      const Vector<int,0>&
   );
template Vector<int,0> operator-<int,0>(
      const ColumnVector<int,0>&,
      const ColumnVector<int,0>&
   );

template int operator*<int,0>(
      const Vector<int,0>&,
      const ColumnVector<int,0>&
   );
template int operator*<int,0>(
      const ColumnVector<int,0>&,
      const Vector<int,0>&
   );
template int operator*<int,0>(
      const ColumnVector<int,0>&,
      const ColumnVector<int,0>&
   );

template Vector<int,0> operator-<int,0>(const ColumnVector<int,0>&);
template Vector<int,0> operator*<int,0,0>(
      const Matrix<int,0,0>&,
      const ColumnVector<int,0>&
   );

template Vector<int,0> operator*<int,0>(const int&, const ColumnVector<int,0>&);
template Vector<int,0> operator*<int,0>(const ColumnVector<int,0>&, const int&);
template Vector<int,0> operator/<int,0>(const ColumnVector<int,0>&, const int&);

template std::ostream& operator<< <int,0>(std::ostream&, const ColumnVector<int,0>&);

}}  // end of namespace chomp::vectalg

/// @}

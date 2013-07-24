/// @addtogroup chomp::vectalg::Matrix<int,0,0>Algorithms
/// @{
/////////////////////////////////////////////////////////////////////////////
/// @file intMatrixAlgorithms.cpp
///
/// @author Marian Mrozek
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2000-2005 by the CAPD Group.
//
// This file constitutes a part of the CAPD library, 
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.wsb-nlu.edu.pl/ for details. 

#include "capd/capd/minmax.h"
#include "capd/matrixAlgorithms/intMatrixAlgorithms.hpp"
#include <cmath>
namespace chomp{
  namespace matrixAlgorithms{

    /* Elementary row and column operations */
template void rowExchange<chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& A,int i,int j);
template void rowMultiply<chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& A,int i,chomp::vectalg::Matrix<int,0,0>::ScalarType s);
template void rowAdd<chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& A,int i,int j,chomp::vectalg::Matrix<int,0,0>::ScalarType s);
template void columnExchange<chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& A,int i,int j);
template void columnMultiply<chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& A,int j,chomp::vectalg::Matrix<int,0,0>::ScalarType s);
template void columnAdd<chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& A,int i,int j,chomp::vectalg::Matrix<int,0,0>::ScalarType s);
    /* Elementary row and column operations on chomp::vectalg::Matrix<int,0,0> and matrices of bases */
template void rowExchange<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,int i,int j);
template void rowMultiply<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,int i,chomp::vectalg::Matrix<int,0,0>::ScalarType q);
template void rowAdd<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,int i,int j,chomp::vectalg::Matrix<int,0,0>::ScalarType q);
template void columnExchange<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int i,int j);
template void columnMultiply<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int i,chomp::vectalg::Matrix<int,0,0>::ScalarType q);
template void columnAdd<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int i,int j,chomp::vectalg::Matrix<int,0,0>::ScalarType q);
            // *** partial reduction *** //
template void partRowReduce<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,int k,int l);
template void partColumnReduce<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int k,int l);
      // *** Test for nonzero matrices *** /
template void smallestNonZero<chomp::vectalg::Matrix<int,0,0> >(const chomp::vectalg::Matrix<int,0,0>& A,chomp::vectalg::Matrix<int,0,0>::ScalarType& s, int& iOpt, int& jOpt);
template bool nonZero<chomp::vectalg::Matrix<int,0,0> >(const chomp::vectalg::Matrix<int,0,0>& A);
          // *** row echelon form *** //
template void rowPrepare<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,int k,int l);
template void rowReduce<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,int k,int l);
template void rowEchelon<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,int &k);
// *** column echelon form *** //
// *** under construction *** //
template void columnPrepare<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int k,int l);
template void columnReduce<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int k,int l);
template void columnEchelon<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int &l);
// *** Smith diagonalization *** //
template void moveMinNonzero<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int k);
template bool checkForDivisibility<chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,int k,int& i,int& j,chomp::vectalg::Matrix<int,0,0>::ScalarType &q);
template void partSmithForm<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int k);
template void smithForm<chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0>,chomp::vectalg::Matrix<int,0,0> >(chomp::vectalg::Matrix<int,0,0>& B,chomp::vectalg::Matrix<int,0,0>& Q,chomp::vectalg::Matrix<int,0,0>& Qinv,chomp::vectalg::Matrix<int,0,0>& R,chomp::vectalg::Matrix<int,0,0>& Rinv,int &s,int &t);
  } // end of namespace matrixAlgorithms
} // end of namespace chomp;
/// @}

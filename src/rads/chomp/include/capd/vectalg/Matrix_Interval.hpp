/// @addtogroup vectalg
/// @{

/////////////////////////////////////////////////////////////////////////////
/// @file imatrix.hpp
///
/// @author The CAPD Group
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 2000-2005 by the CAPD Group.
//
// This file constitutes a part of the CAPD library, 
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.wsb-nlu.edu.pl/ for details. 

#ifndef _CAPD_VECTALG_IMATRIX_HPP_ 
#define _CAPD_VECTALG_IMATRIX_HPP_ 

#include "capd/vectalg/iobject.hpp"

namespace chomp{
namespace vectalg{

template<typename IMatrixType>
IMatrixType midMatrix(const IMatrixType& v)
{
  IMatrixType result(v.numberOfRows(),v.numberOfColumns(),true);
  mid(v,result); // defined in iobject.hpp
  return result;
}

}} // namespace chomp::vectalg

#endif // _CAPD_VECTALG_IMATRIX_HPP_ 

/// @}

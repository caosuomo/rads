/// @addtogroup vectalg
/// @{

/////////////////////////////////////////////////////////////////////////////
/// @file ivector.hpp
///
/// @author The CAPD Group
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 2000-2005 by the CAPD Group.
//
// This file constitutes a part of the CAPD library,
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.wsb-nlu.edu.pl/ for details.

#ifndef _CAPD_VECTALG_IVECTOR_HPP_
#define _CAPD_VECTALG_IVECTOR_HPP_

#include <stdexcept>
#include "capd/vectalg/iobject.hpp"
#include "capd/vectalg/EmptyIntersectionException.h"
namespace chomp{
namespace vectalg{

//---------------  definitions for class intervalVector only -------------------//

template<typename IVectorType>
IVectorType diam(const IVectorType& v)
{
  IVectorType result(v.dimension());
  diameter(v,result); // defined in iobject.hpp
  return result;
}

/******************************************************************************/
// intersection of two interval vectors
// halts if the intersection is empty
/******************************************************************************/

template<typename IVectorType>
IVectorType intersection(const IVectorType &v1, const IVectorType &v2)
{
  IVectorType result(v1.dimension());
  if(!(intersection(v1,v2,result))) // defined in iobject.hpp
      throw EmptyIntersectionException("Intersection of two interval vectors is empty");
  return result;
}

template<typename IVectorType>
IVectorType midVector(const IVectorType& v)
{
  IVectorType result(v.dimension());
  mid(v,result); // defined in iobject.hpp
  return result;
}

template<typename IVectorType>
IVectorType intervalHull(const IVectorType& v1, const IVectorType& v2)
{
  IVectorType result(v1.dimension());
  intervalHull(v1,v2,result);
  return result;
}

template<typename IVectorType>
IVectorType leftVector(const IVectorType& v)
{
  IVectorType temp(v.dimension());
  typename IVectorType::const_iterator b=v.begin(), e=v.end();
  typename IVectorType::iterator i = temp.begin();
  while(b!=e)
  {
    *i = b->leftBound();
    ++i;
    ++b;
  }
  return temp;
}

template<typename IVectorType>
IVectorType rightVector(const IVectorType& v)
{
  IVectorType temp(v.dimension());
  typename IVectorType::const_iterator b=v.begin(), e=v.end();
  typename IVectorType::iterator i = temp.begin();
  while(b!=e)
  {
    *i = b->rightBound();
    ++i;
    ++b;
  }
  return temp;
}

template<typename IVectorType>
IVectorType intervalBall(const IVectorType &iv, const typename IVectorType::ScalarType &r)
{
  IVectorType result(iv.dimension());
  typename IVectorType::iterator b=result.begin(), e=result.end();
  typename IVectorType::const_iterator i=iv.begin();

  while(b!=e)
  {
    *b = ball(*i,r);
    ++b;
    ++i;
  }
  return result;
}

template<typename IVectorType>
typename IVectorType::ScalarType solveAffineInclusion(
      const IVectorType& a,
      const IVectorType& p,
      const IVectorType& c
   )
{
  typedef typename IVectorType::ScalarType ScalarType;

  if(a.dimension()!=p.dimension() || a.dimension()!=c.dimension())
    throw std::runtime_error("Incompatible vectors in function solveAffineInclusion");

  ScalarType result = solveAffineInclusion(a[0],p[0],c[0]);
  for(int i=1; i<a.dimension(); ++i)
  {
    ScalarType iv=solveAffineInclusion(a[i],p[i],c[i]);
    result = chomp::min( result.leftBound(), iv.leftBound() );
  }
  return result;
}

template<typename IVectorType>
typename IVectorType::ScalarType solveAffineInclusion(
      const IVectorType& a,
      const IVectorType& p,
      const IVectorType& c,
      int& dir
   )
{
  if(a.dimension()!=p.dimension() || a.dimension()!=c.dimension())
    throw std::runtime_error("Incompatible vectors in function solveAffineInclusion");

  typedef typename IVectorType::ScalarType ScalarType;
  ScalarType result = solveAffineInclusion(a[0],p[0],c[0]);
  dir=0;

  for(int i=1; i<a.dimension(); ++i)
  {
    ScalarType iv = solveAffineInclusion(a[i],p[i],c[i]);
    if( iv.leftBound() < result.leftBound() )
    {
      result = iv.leftBound();
      dir = i;
    }
  }
  return result;
}

}} // namespace chomp::vectalg

#endif // _CAPD_VECTALG_IVECTOR_HPP_

/// @}

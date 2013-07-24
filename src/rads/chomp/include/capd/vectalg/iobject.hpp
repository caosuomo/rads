/// @addtogroup vectalg
/// @{

/////////////////////////////////////////////////////////////////////////////
/// @file iobject.hpp
///
/// @author The CAPD Group
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 2000-2005 by the CAPD Group.
//
// This file constitutes a part of the CAPD library,
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.wsb-nlu.edu.pl/ for details.

#ifndef _CAPD_VECTALG_IOBJECT_HPP_
#define _CAPD_VECTALG_IOBJECT_HPP_

#include <stdexcept>
#include "capd/capd/minmax.h"

namespace chomp{
namespace vectalg{

/**
 *  splits IntervalObject v into  form center + [-radius, radius] form
 *  @param[in,out]  v   object to be splitted, returns center of the object.
 *  @param[out]     rv  returns zero centered set [-radius, radius].
 */
template<typename IntervalObject>
void split(IntervalObject& v, IntervalObject& rv)
{
  if(v.dimension()!=rv.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::split");
  typename IntervalObject::iterator b = v.begin(),e=v.end(), i=rv.begin();
  while(b!=e)
  {
    b->split(*i);
    ++b;
    ++i;
  }
}

/**
 *  splits IntervalObject v into  form center + diameter  where diameter = [-radius,radius] on each coordinate
 *  @param[in]    v        object to be splitted,
 *  @param[out]   center   returns center of the object.
 *  @param[out]   diameter   returns zero centered set [-radius,radius].
 */
template<typename IntervalObject>
void split(const IntervalObject & v, IntervalObject& center, IntervalObject & diameter) {
  if((v.dimension()!=center.dimension()) && (v.dimension()!=diameter.dimension()))
    throw std::range_error("Unequal dimensions in function chomp::vectalg::split");
  typename IntervalObject::const_iterator b = v.begin(), e=v.end();
  typename IntervalObject::iterator c = center.begin(), d=diameter.begin();
  while(b!=e) {
    b->split(*c, *d);
    b++; c++; d++;
  }
}

/**
 *  splits source object given by [sourceBegin, sourceEnd]
 *  into  form center + diameter  where diameter = [-radius,radius] on each coordinatedimension
 *  @param[in]    sourceBegin,sourceEnd        object to be splitted,
 *  @param[out]   center        returns center of the object (it should be iterator on first element)
 *  @param[out]   diameter      returns zero centered set [-radius,radius] (it should be iterator on first element)
 */
template<typename SourceIterator, typename Iterator1, typename Iterator2>
void split(const SourceIterator & sourceBegin, const SourceIterator &sourceEnd,
           Iterator1 center, Iterator2  diameter) {
  SourceIterator it = sourceBegin;
  while(it!=sourceEnd) {
    it->split(*center, *diameter);
    it++; center++; diameter++;
  }
}

/**
 *  returns the biggest diameter of IntervalObject (vector or matrix) coordinates
 */
template<typename IntervalObject>
typename IntervalObject::ScalarType size(const IntervalObject& v)
{
  typedef typename IntervalObject::ScalarType ScalarType;
  ScalarType result(0.);
  typename IntervalObject::const_iterator b=v.begin(), e=v.end();
  while(b!=e)
  {
     result = chomp::max(result,diam(*b));
     ++b;
  }
  return ScalarType(result.rightBound());
}

/**
 *  checks if IntervalObject v contains zero on all coordinates
 */

template<typename IntervalObject>
bool containsZero(const IntervalObject& v)
{

  typename IntervalObject::const_iterator b = v.begin(), e=v.end();
  while(b!=e)
  {
    if(!(isSingular(*b)))
      return false;
    ++b;
  }
  return true;
}

/**
 *  checks if IntervalObject v1 is contained in IntervalObject v2
 */

template<typename IntervalObject>
bool subset(const IntervalObject& v1, const IntervalObject& v2)
{
  if(v1.dimension()!=v2.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::subset");
  typename IntervalObject::const_iterator b = v1.begin(), e=v1.end(), i=v2.begin();
  while(b!=e)
  {
    if(!(b->subset(*i))) return false;
    ++b;
    ++i;
  }
  return true;
}

/**
 * checks if IntervalObject v1 is contained in interior of IntervalObject v2
 */

template<typename IntervalObject>
bool subsetInterior(const IntervalObject& v1, const IntervalObject& v2)
{
  if(v1.dimension()!=v2.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::subsetInterior");
  typename IntervalObject::const_iterator b = v1.begin(), e=v1.end(), i=v2.begin();
  while(b!=e)
  {
    if(!(b->subsetInterior(*i))) return false;
    ++b;
    ++i;
  }
  return true;
}

/// //////////////////////////////////////////////////////////////////////////////
// intersection
///
/// intersection of two interval objects (vectors, matrices)
/// @returns
///   true and intersection in result if intersection is not empty
///   false if intersection is empty (value of result is meaningless)
///
/// //////////////////////////////////////////////////////////////////////////////

template<typename IntervalObject>
bool intersection(const IntervalObject &v1, const IntervalObject &v2, IntervalObject &result)
{
  if(v1.dimension()!=v2.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::intersection");
  if(v1.dimension()!=result.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::intersection");
  typename IntervalObject::const_iterator b1 = v1.begin(), b2=v2.begin();
  typename IntervalObject::iterator b = result.begin(), e=result.end();

  while(b!=e)
  {
    if( !intersection(*b1,*b2,*b))
      return false;
    ++b;
    ++b1;
    ++b2;
  }
  return true;
}

/// //////////////////////////////////////////////////////////////////////////////
// intersectionIsEmpty
///
/// checks if intersection of two interval vectors is empty
///
/// @returns
///   true  if intersection is empty
///   false if intersection is not empty
///
/// //////////////////////////////////////////////////////////////////////////////
template <typename IntervalObject>
bool intersectionIsEmpty(const IntervalObject & v, const IntervalObject &w){

   typename IntervalObject::const_iterator iv = v.begin(), iw = w.begin(), endv = v.end();
   while(iv != endv){
     if((iv->leftBound() > iw->rightBound()) || (iw->leftBound() > iv->rightBound()))
       return true;
     ++iv; ++iw;
   }
   return false;
 }


template<typename IntervalObject>
void intervalHull(const IntervalObject &v1, const IntervalObject &v2, IntervalObject &result)
{
  if(v1.dimension()!=v2.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::intervalHull");
  if(v1.dimension()!=result.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::intervalHull");
  typedef typename IntervalObject::ContainerType ContainerType;
  typename ContainerType::const_iterator b1 = v1.ContainerType::begin(), b2=v2.ContainerType::begin();
  typename ContainerType::iterator b = result.ContainerType::begin(), e=result.ContainerType::end();

  while(b!=e)
  {
    *b = intervalHull(*b1,*b2);
    ++b;
    ++b1;
    ++b2;
  }
}

template<typename IntervalObject, typename ResultContainer>
void diameter(const IntervalObject &v, ResultContainer &result)
{
  if(v.dimension()!=result.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::diameter");
  typedef typename IntervalObject::ScalarType ScalarType;
  typename ResultContainer::iterator i = result.begin();
  typename IntervalObject::const_iterator b = v.begin(), e=v.end();

  while(b!=e)
  {
    *i = (ScalarType(b->rightBound()) - ScalarType(b->leftBound())).rightBound();
    ++i;
    ++b;
  }
}

template<typename IntervalObject>
void mid(const IntervalObject &v, IntervalObject &result)
{
  if(v.dimension()!=result.dimension())
    throw std::range_error("Unequal dimensions in function chomp::vectalg::mid");
  typedef typename IntervalObject::ScalarType ScalarType;
  typename IntervalObject::iterator i = result.begin();
  typename IntervalObject::const_iterator b = v.begin(), e=v.end();

  while(b!=e)
  {
    *i = b->mid();
    ++i;
    ++b;
  }
}

template<typename ResultType, typename IntervalObject>
ResultType midObject(const IntervalObject &v)
{
  ResultType result(v.dimension());
  typedef typename ResultType::ScalarType ScalarType;
  typename ResultType::iterator i = result.begin();
  typename IntervalObject::const_iterator b = v.begin(), e=v.end();

  while(b!=e)
  {
    *i = (b->leftBound()+b->rightBound())/ScalarType(2.0);
    ++i;
    ++b;
  }
  return result;
}


template<typename ResultType, typename IntervalObject>
ResultType leftObject(const IntervalObject &v)
{
  ResultType result(v.dimension());
  typedef typename IntervalObject::ScalarType ScalarType;
  typename ResultType::iterator i = result.begin();
  typename IntervalObject::const_iterator b = v.begin(), e=v.end();

  while(b!=e)
  {
    *i = b->left();
    ++i;
    ++b;
  }
  return result;
}

template<typename ResultType, typename IntervalObject>
ResultType rightObject(const IntervalObject &v)
{
  ResultType result(v.dimension());
  typedef typename IntervalObject::ScalarType ScalarType;
  typename ResultType::iterator i = result.begin();
  typename IntervalObject::const_iterator b = v.begin(), e=v.end();

  while(b!=e)
  {
    *i = b->right();
    ++i;
    ++b;
  }
  return result;
}

}} // namespace chomp::vectalg

#endif // _CAPD_VECTALG_IOBJECT_HPP_

/// @}

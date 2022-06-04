#pragma once

/**
 * marray - multidimensional array
 *
 * marray holds arbitrary data types and stores it as a
 * multidimensional array.
 */
template<typename T, unsigned... nDIMENSIONS> struct marray;

template <typename T, unsigned DIMENSION>
struct marray<T, DIMENSION>
{
  typedef T value_type[DIMENSION];
  const value_type _data;
};

template<typename T, unsigned DIMENSION, unsigned... nDIMENSIONS>
struct marray<T, DIMENSION, nDIMENSIONS...>
{
  typedef typename marray<T, nDIMENSIONS...>::value_type nvalue_type;
  typedef nvalue_type value_type[DIMENSION];
  const value_type _data;
};

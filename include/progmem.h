#pragma once

#include <Arduino.h>

/**
 * array_P - multidimensional array in PROGMEM
 *
 * array_P holds arbitrary data types and stores them in PROGMEM as a
 * multidimensional array.
 */
template<typename T, unsigned... nDIMENSIONS> struct array_P;

template <typename T, unsigned DIMENSION>
struct array_P<T, DIMENSION>
{
  typedef T value_type[DIMENSION];
  static const value_type PROGMEM _data;

  const T operator[](unsigned i) const
  {
    T ret;
    if (i >= DIMENSION)
      i = DIMENSION - 1;
    memmove_P(&ret, &_data[i], sizeof(T));
    return ret;
  }
};

template<typename T, unsigned DIMENSION, unsigned... nDIMENSIONS>
struct array_P<T, DIMENSION, nDIMENSIONS...>
{
  typedef typename array_P<T, nDIMENSIONS...>::value_type nvalue_type;
  typedef nvalue_type value_type[DIMENSION];
  static const value_type PROGMEM _data;

  const value_type& operator[](unsigned i) const
  {
    if (i >= DIMENSION)
      i = DIMENSION - 1;
    return static_cast<const value_type&>(pgm_read_ptr(&_data[i]));
  }
};

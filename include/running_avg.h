#pragma once

#include <math.h>

/**
 * A simple linear recursive exponential average.
 *
 * The average value is exposed as data type T with accurate rounding.
 * It is meant to be used as a "normal" integral type, where one just
 * adds new values with a += operation of a type T right-hand-side.
 */
template <unsigned SAMPLES, typename T>
class Running_avg {
  float _avg;
  unsigned _samples;

  constexpr bool T_is_integer() const
  { return static_cast<T>(3) / static_cast<T>(2) == static_cast<T>(1); }

public:
  /**
   * Constructor.
   *
   * The average remains uninitialized. The first addition to it will start
   * priming.
   */
  Running_avg() noexcept : _samples(0) { }

  /**
   * Constructor with initial value.
   *
   * The average is tuned to an initial value.
   */
  Running_avg(const T init) noexcept : _avg(init), _samples(1) {
    static_assert(SAMPLES > 0, "Need more than 0 samples.");
  }


  /** @brief Add a new sample to the average. */
  Running_avg operator+= (const Running_avg &rhs) noexcept {
    /*
     * This is a moving average of the last SAMPLES numbers. Basic formula:
     *
     *  avg = ((avg * SAMPLES) + rhs) / (SAMPLES + 1)
     */
    float newavg = ((_avg * _samples) + (rhs._avg * rhs._samples)) / (_samples + rhs._samples);
    _avg = newavg;

    if (!primed()) {
      _samples += rhs._samples;
      if (_samples > SAMPLES)
        _samples = SAMPLES;
    }

    return (*this);
  }

  /** @brief Add an average to the average. */
  friend Running_avg operator+(Running_avg lhs, Running_avg rhs) noexcept
  {
    lhs += rhs;
    return lhs;
  }

  /**
   * Expose average value as type T, correctly rounded.
   */
  operator T() const noexcept
  { return T_is_integer() ? round(_avg) : _avg; }

  /**
   * Test if at least SAMPLES elements have been processed already.
   */
  bool primed() const noexcept { return _samples == SAMPLES; }
};

#pragma once

/**
 * A running average of SAMPLES elements.
 */
template <unsigned SAMPLES, typename T>
class Running_avg {
  T avg;

public:
  Running_avg(const T init = 0) : avg(init) {
    static_assert(SAMPLES > 0, "Need more than 0 samples.");
  }

  Running_avg operator+= (const Running_avg &rhs) {
    /*
     * This is a moving average of the last SAMPLES numbers. Basic formula:
     *
     *   avg = ((avg * SAMPLES) + rhs.avg) / (SAMPLES + 1);
     *
     * Integer division truncates the result. This is okay when ramping down
     * (e.g. rhs < lhs). When ramping up, we need to round correctly.
     * Otherwise we might never reach the upper limit.
     *
     * NOTE: This ignores the warmup phase.
     */
    enum { Rounding_adjustment = (SAMPLES + 1) / 2, };
    long long newavg = ((avg * SAMPLES) + rhs.avg);
    if (rhs.avg > avg)
      newavg += Rounding_adjustment;
    newavg /= (SAMPLES + 1);

    avg = newavg;
    return (*this);
  }

  operator T() const { return avg; }
};

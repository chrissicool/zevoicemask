#pragma once

/**
 * A running average of SAMPLES elements.
 */
template <unsigned SAMPLES, typename T>
class Running_avg {
  T avg;

public:
  Running_avg(const T init = 0) : avg(init) { }

  Running_avg operator+= (const Running_avg &rhs) {
    /*
     * This is a moving average of the last SAMPLES numbers. Basic formula:
     *
     *   avg = ((avg * SAMPLES) + rhs.avg) / (SAMPLES + 1);
     *
     * NOTE: This ignores the warmup phase.
     */
    unsigned long long newavg = ((avg * SAMPLES) + rhs.avg) / (SAMPLES + 1);

    avg = newavg;
    return (*this);
  }

  operator T() const { return avg; }
};

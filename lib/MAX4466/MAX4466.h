#pragma once

#include <Arduino.h>

/**
 * This is an implementation for the MAX4466 sound sensor.
 * The template parameter is the ADC resolution in bits.
 */
template <unsigned ADC_RES_BITS>
class HardwareMAX4466
{
private:
  uint8_t       _analogPin;

  enum { ADC_RES_MAX = (1 << ADC_RES_BITS) - 1, };

  /**
   * Return raw ADC value shifted by VCC/2.
   *
   * The MAX4466 returns VCC/2 when there is no sound sensed.
   * It generates sine wave amplitudes around that VCC/2.
   *
   * Note that the device might not sense silence at VCC/2 exactly.
   * There is an offset adjustment for that.
   *
   * Return the amplitude between [-(ADC_RES_MAX / 2) .. (ADC_RES_MAX / 2)].
   */
  int raw_amplitude()
  {
    const long val = analogRead(_analogPin);

    const long in_min = 0;
    const long in_max = ADC_RES_MAX;
    const long out_min = -((ADC_RES_MAX + 1) / 2);
    const long out_max = (ADC_RES_MAX + 1) / 2;

    return map(val, in_min, in_max, out_min, out_max);
  }

  /* ... because Arduino's abs() sucks */
  template <typename T>
  T absolute(T const val)
  { return val < 0 ? -val : val; }


public:
  void setup(uint8_t inPin)
  {
    _analogPin = inPin;
    pinMode(_analogPin, INPUT);
  }

  /**
   * Return scaled loudness.
   *
   * Return the quantized loudness between [0 .. FACTOR].
   */
  template <unsigned FACTOR>
  unsigned scaled_loudness()
  { return map(absolute(raw_amplitude()), 0, ((ADC_RES_MAX + 1) / 2), 0, FACTOR); }
};

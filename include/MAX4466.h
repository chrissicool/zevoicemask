#pragma once

extern "C" {
#include <user_interface.h>
}

#include <Arduino.h>
#include <algorithm>
#include <iterator>

#include <running_avg.h>

/**
 * This is a hardware abstraction library for the MAX4466 sound sensor.
 *
 * It features automatic DC offset detection. This is leveraged by the fact
 * that all readings resemble a wave around the center point. Building an
 * average on them finds the center automatically.
 *
 * @tparam ADC_RES_BITS         ADC resolution in bits
 *
 */
template <unsigned ADC_RES_BITS>
class HardwareMAX4466 : public Printable
{
private:
  enum {
    ADC_RES_MIN               = 0,
    ADC_RES_MAX               = (1 << ADC_RES_BITS) - 1,
    ADC_RES_HALF              = (ADC_RES_MAX + 1) / 2,
    ADC_SAMPLES               = 4,
  };

  uint8_t                       _analogPin;
  uint16_t                      _adc[ADC_SAMPLES];
  uint16_t                      _intermediate[ADC_SAMPLES];
  Running_avg<63, uint16_t>     _center;
  int                           _raw;
  bool                          _dynadj;


  /**
   * Dynamic center adjustment.
   *
   * MAX4466 has a DC bias of VCC/2. But in practice that center has a
   * DC offset that differs according to the power source used. MAX4466
   * generates sound waves around that center. So averaging all samples
   * gets the real center, especially in quiet situations. Perform that
   * center adjustment on an evened-out set.
   */
  void center(uint16_t min, uint16_t max, uint16_t median)
  {
    if (!_dynadj)
      return;

    /* Toss away noisy samples. Want the center only. */
    if ((max - min) > 0)
      return;

    /** Only adjust +/-18% around the theoretical center. */
    if (absolute(ADC_RES_HALF - median) > 96)
      return;

    _center += median;
  }

  /**
   * Read ADC in bulk and perform center adjustment
   *
   * The ADC on the ESP8266 is really noisy. Read the ADC multiple
   * times to even out outliers. Apply center adjustment.
   *
   * @remark
   *
   * This is implemented with a two-dimensional array to save space.
   * E.g. ADC_SAMPLES == 4 yields in 16 samples to be processed, by
   * only allocating 8 slots.
   */
  uint16_t adc_read(uint8_t /*pin*/)
  {
    for (int x = 0; x < ADC_SAMPLES; x++) {
      system_adc_read_fast(_adc, ADC_SAMPLES, 8);

      std::sort(std::begin(_adc), std::end(_adc));
      _intermediate[x] = _adc[ADC_SAMPLES / 2];
    }
    std::sort(std::begin(_intermediate), std::end(_intermediate));
    center(_intermediate[0], _intermediate[ADC_SAMPLES - 1],
           _intermediate[ADC_SAMPLES / 2]);
    return _intermediate[ADC_SAMPLES / 2];
  }

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
    long val = adc_read(_analogPin);

    const long in_min = ADC_RES_MIN;
    const long in_max = ADC_RES_MAX;
    const long out_min = -(ADC_RES_HALF);
    const long out_max = ADC_RES_HALF;

    val += offset();
    val = constrain(val, ADC_RES_MIN, ADC_RES_MAX);
    _raw = map(val, in_min, in_max, out_min, out_max);
    return _raw;
  }

  int offset() const { return ADC_RES_HALF - _center; }

  /* ... because Arduino's abs() sucks */
  template <typename T>
  T absolute(T const val)
  { return val < 0 ? -val : val; }


public:
  void setup(uint8_t inPin, bool dynadj = true)
  {
    _analogPin = inPin;
    pinMode(_analogPin, INPUT);

    _center = ADC_RES_HALF;
    _dynadj = dynadj;
  }

  /**
   * Toggle dynamic center adjustment.
   */
  void center_adjust(bool dynadj)
  {
    _dynadj = dynadj;
    if (dynadj == false)
      _center = ADC_RES_HALF;
  }

  /**
   * Return scaled loudness.
   *
   * Return the quantized loudness between [0 .. FACTOR].
   */
  template <unsigned FACTOR>
  unsigned scaled_loudness()
  { return map(absolute(raw_amplitude()), 0, ((ADC_RES_MAX + 1) / 2), 0, FACTOR); }

  /**
   * Print internal state to a Printer.
   *
   * @see Printable::printTo.
   */
  size_t printTo(Print &p) const override
  { return p.printf_P(PSTR("%4d/%4d"), _raw, offset()); }
};

#pragma once

#include <cassert>
#include <cstddef>
#include <NeoPixelBus.h>

/**
 * PGM_RgbColor - RgbColor, storable in PROGMEM.
 *
 * The NeoPixelBus RgbColors class can only be runtime-initialized.
 * That makes it impossible to be stored in PROGMEM.
 *
 * This is a helper object to store NeoPixelBus RgbColors in PROGMEM.
 * It can be used in static list-initialization.
 *
 * Its only runtime use is to convert it back to RgbColor.
 */
struct PGM_RgbColor
{
  uint8_t R, G, B;

  /**
   * Conversion to RgbColor
   */
  operator const RgbColor() const
  {
    return RgbColor(R, G, B);
  }
};

namespace {
/**
 * Check binary-compatibility between PGM_RgbColor and RgbColor.
 */
static inline void compile_time_check()
{
  static_assert(offsetof(RgbColor, R) == offsetof(PGM_RgbColor, R), "Wrong offset for R");
  static_assert(offsetof(RgbColor, G) == offsetof(PGM_RgbColor, G), "Wrong offset for G");
  static_assert(offsetof(RgbColor, B) == offsetof(PGM_RgbColor, B), "Wrong offset for B");

  static_assert(sizeof(RgbColor) == sizeof(PGM_RgbColor), "Types have different sizes");
  static_assert(sizeof(RgbColor::R) == sizeof(PGM_RgbColor::R), "Red has different size");
  static_assert(sizeof(RgbColor::G) == sizeof(PGM_RgbColor::G), "Green has different size");
  static_assert(sizeof(RgbColor::B) == sizeof(PGM_RgbColor::B), "Blue has different size");
}
};

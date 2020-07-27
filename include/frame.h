#pragma once

#include <Arduino.h>
#include <NeoPixelBrightnessBus.h> // RgbColor

#include <progmem.h>

#define COLUMNS         (8)
#define ROWS            (8)

/**
 * RgbColor frames.
 *
 * A list of color frames for a COLUMNS x ROWS matrix,
 * stored in PROGMEM.
 */
template <unsigned FRAMES>
class ColorFrame : public array_P<RgbColor, FRAMES, ROWS, COLUMNS>
{ };

template <>
class ColorFrame<1> : public array_P<RgbColor, ROWS, COLUMNS>
{ };

#define ColorFrames(n)  ColorFrame<n>::value_type

/**
 * Black/white frames.
 *
 * A list of black/white frames for a 8 x ROWS matrix,
 * stored in PROGMEM.
 */
template <unsigned FRAMES>
class BwFrame : public array_P<uint8_t, FRAMES, ROWS>
{ };

template <>
class BwFrame<1> : public array_P<uint8_t, ROWS>
{ };

#define BwFrames(n)     BwFrame<n>::value_type

void ClearFrame();
void ShowMouthSmiling();
void ShowFrame(ColorFrames(1) const &frame);
void ShowFrame(BwFrames(1) const &frame, const RgbColor &c);
void ShowPixel(unsigned col, unsigned row, const RgbColor &c);
RgbColor const GetPixel(unsigned col, unsigned row);

/*
 * Some operators that are missing in NeoPixelBus.
 */

static inline RgbColor& operator+=(RgbColor& lhs, const RgbColor& rhs)
{
  lhs.R = ((255 - rhs.R) > lhs.R) ? 255 : lhs.R + rhs.R;
  lhs.G = ((255 - rhs.G) > lhs.G) ? 255 : lhs.G + rhs.G;
  lhs.B = ((255 - rhs.B) > lhs.B) ? 255 : lhs.B + rhs.B;
  return lhs;
}
static inline RgbColor operator+(RgbColor lhs, const RgbColor& rhs)
{
  lhs += rhs;
  return lhs;
}
static inline RgbColor& operator-=(RgbColor& lhs, const RgbColor& rhs)
{
  lhs.R = (rhs.R > lhs.R) ? 0 : lhs.R - rhs.R;
  lhs.G = (rhs.G > lhs.G) ? 0 : lhs.G - rhs.G;
  lhs.B = (rhs.B > lhs.B) ? 0 : lhs.B - rhs.B;
  return lhs;
}
static inline RgbColor operator-(RgbColor lhs, const RgbColor& rhs)
{
  lhs -= rhs;
  return lhs;
}

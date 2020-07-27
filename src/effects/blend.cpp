#include <effects/blend.h>

static RgbColor dimto;

void Blend::init()
{
  uint8_t r, g, b, rgb;

  r = random(192) + 64;
  g = random(192) + 64;
  b = random(192) + 64;
  rgb = random(8);
  if (rgb & 1)
    r = 0;
  if (rgb & 2)
    g = 0;
  if (rgb & 4)
    b = 0;

  dimto = RgbColor(r, g, b);
}

void Blend::apply(unsigned percent)
{
  RgbColor dim;

  dim = RgbColor(map(percent, 0, 100, 0, dimto.R),
                 map(percent, 0, 100, 0, dimto.G),
                 map(percent, 0, 100, 0, dimto.B));

  for (unsigned col = 0; col < COLUMNS; col++) {
    for (unsigned row = 0; row < ROWS; row++) {
      auto c = GetPixel(col, row);
      if (c == 0)
        continue;

      c -= dim;
      ShowPixel(col, row, c);
    }
  }
}

Blend blend;

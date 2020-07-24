#include <effects/sparkle.h>

static bool grey;

void Sparkle::init()
{
  grey = random(2);
}

void Sparkle::apply(unsigned percent)
{
  for (unsigned col = 0; col < COLUMNS; col++ ) {
    for (unsigned row = 0; row < ROWS; row++) {
      auto c = GetPixel(col, row);
      if (c == 0)
        continue;

      if (grey) {
        if (c.CalculateBrightness() > 128) {
          c.Darken(random(c.CalculateBrightness() - 128));
        }
      } else {
        RgbColor dim(c.R > 128 ? random(c.R - 128) : 0,
                     c.G > 128 ? random(c.G - 128) : 0,
                     c.B > 128 ? random(c.B - 128) : 0);
        c -= dim;
      }

      ShowPixel(col, row, c);
    }
  }
}

Sparkle sparkle;

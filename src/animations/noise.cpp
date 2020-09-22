#include <animations/noise.h>
#include <assert.h>

static const unsigned nframes = 7;

unsigned Noise::frames() const
{ return nframes; }

enum Mode {
  Black_white,
  Color,
};
static unsigned mode;

void Noise::init()
{
  mode = random(2);
}

void Noise::frame(unsigned idx)
{
  if (idx >= frames())
    return;

  for (unsigned col = 0; col < COLUMNS; col++ ) {
    for (unsigned row = 0; row < ROWS; row++) {
      RgbColor c;
      if (mode == Black_white) {
        c = RgbColor(random(16) * random(16),
                     random(16) * random(16),
                     random(16) * random(16));
      } else {
        c = RgbColor(random(16) * random(16));
      }
      ShowPixel(col, row, c);
    }
  }
}

Noise noise;

#include <animations/strobe.h>
#include <assert.h>

static const unsigned nframes = 7;

unsigned Strobe::frames() const
{ return nframes; }

enum Mode {
  All,
  Each,
};
static unsigned mode;

void Strobe::init()
{
  mode = random(2);
}

void Strobe::frame(unsigned idx)
{
  if (idx >= frames())
    return;

  if (mode == All) {
    if ((idx & 1) == 0)
      return;
    ShowMouthSmiling();
    if ((idx & 1) == 1)
      return;
  }

  assert(mode == Each);
  ShowMouthSmiling();
  for (unsigned col = 0; col < COLUMNS; col++ ) {
    for (unsigned row = 0; row < ROWS; row++)
      if (GetPixel(col, row) != 0) {
        if (random(2))
          ShowPixel(col, row, RgbColor(0));
      }
  }
}

Strobe strobe;

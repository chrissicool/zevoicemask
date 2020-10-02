#include <animations/matrix.h>

static const unsigned nframes = 7;

unsigned Matrix::frames() const
{ return nframes; }

enum {
  Min_step = 2,
};

static RgbColor base;
struct Stripe {
  unsigned start;  ///< Start at frame number
  unsigned step;   ///< Speed for this stripe
  unsigned length; ///< Length of this stripe
};
static Stripe stripe[COLUMNS];

void Matrix::init()
{
  uint8_t r = 0, g = 0, b = 0;
  uint8_t rgb = random(8);
  if (rgb & 1)
    r = random(64) + 192;
  if (rgb & 2)
    g = random(64) + 192;
  if (rgb & 4)
    b = random(64) + 192;

  base = RgbColor(r, g, b);
  if (base == 0)
    base = RgbColor(random(64) + 192);

  for (unsigned i = 0; i < COLUMNS; i++) {
    unsigned len = ROWS - random(3);
    stripe[i].step  = random(4) + Min_step;
    stripe[i].start  = random(stripe[i].step - Min_step);
    stripe[i].length = len + random(stripe[i].step - Min_step - stripe[i].start);
  }

  /*
   * Make sure to have at least one stripe that is seen for all nframes.
   */
  unsigned slowest = random(COLUMNS);
  stripe[slowest].step   = Min_step;
  stripe[slowest].start  = 0;
  stripe[slowest].length = ROWS;
}

void Matrix::frame(unsigned idx)
{
  if (idx >= frames())
    return;

  ShowMouthSmiling();
  for (unsigned col = 0; col < COLUMNS; col++ ) {
    if (idx < stripe[col].start)
      continue;

    unsigned step    = stripe[col].step;
    unsigned length  = stripe[col].length;
    unsigned current = idx - stripe[col].start;
    for (unsigned row = (current * step);
                  row < (current * step) + length;
                  row++) {
      if (row < ROWS) {
        RgbColor dither(random(32));
        RgbColor c = base - dither;
        ShowPixel(col, row, c);
      }
    }
  }
}

Matrix matrix;

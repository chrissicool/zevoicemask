#include <animations/twinkle.h>

static RgbColor c;
static uint8_t col, row;
static int diagonal;

enum {
  BASE = 64,
  RANGE = 255 - BASE,
};

void Twinkle::init()
{
  unsigned mask = random(3);
  c = RgbColor(mask == 0 ? 0 : random(RANGE) + BASE,
               mask == 1 ? 0 : random(RANGE) + BASE,
               mask == 2 ? 0 : random(RANGE) + BASE);
  do
  {
    diagonal = random(2);
    col = random(COLUMNS - 2) + 1;
    row = random(ROWS - 2) + 1;
  } while (GetPixel(col, row) != 0 ||
           GetPixel(col - 1, row - diagonal) != 0 ||
           GetPixel(col + 1, row + diagonal) != 0 ||
           GetPixel(col + diagonal, row - 1) != 0 ||
           GetPixel(col - diagonal, row + 1) != 0);
}

unsigned Twinkle::frames() const
{ return 8; }

void Twinkle::frame(unsigned idx)
{
  if (idx >= (frames() - 1))
    return;

  unsigned dim;
  if (idx >= (frames() / 2))
    idx = frames() - 1 - idx;

  dim = map(idx, 0, (frames() / 2), 0, BASE);

  ShowMouthSmiling();
  ShowPixel(col, row, c.Dim(dim));
  ShowPixel(col - 1, row - diagonal, c.Dim(dim));
  ShowPixel(col + 1, row + diagonal, c.Dim(dim));
  ShowPixel(col + diagonal, row - 1, c.Dim(dim));
  ShowPixel(col - diagonal, row + 1, c.Dim(dim));
}

Twinkle twinkle;

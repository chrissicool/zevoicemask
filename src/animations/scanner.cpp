#include <animations/scanner.h>

static const unsigned columns = COLUMNS * 2;
static const unsigned rows = ROWS;
static const unsigned steps = columns - COLUMNS;
static const unsigned nframes = 8;

unsigned Scanner::frames() const
{ return nframes; }

static RgbColor c;
static unsigned fade;

void Scanner::init()
{
  uint8_t r = random(128);
  uint8_t g = random(128);
  uint8_t b = random(128);

  int brighten = random(8);
  if (brighten & 1) r += 128;
  if (brighten & 2) g += 128;
  if (brighten & 4) b += 128;

  c = RgbColor(r, g, b);
  fade = max(r, max(g, b)) / steps;
}

void Scanner::frame(unsigned idx)
{
  if (idx >= frames())
    return;

  ShowMouthSmiling();

  unsigned col = map(idx, 0, nframes, 0, columns);
  for (unsigned i = 0; i < steps; i++ ) {
    if (i > col)
      break;

    for (unsigned row = 0; row < rows; row++)
      if (GetPixel(col - i, row) != 0)
        ShowPixel(col - i, row, c.Dim(fade * i));
  }
}

Scanner scanner;

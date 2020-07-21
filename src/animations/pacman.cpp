#include <animations/pacman.h>

static const unsigned nframes = 8;
static const BwFrames(nframes) pix = {
{
  B00000000,
  B10000000,
  B11000000,
  B11000000,
  B11000000,
  B11000000,
  B10000000,
  B00000000
},{
  B11100000,
  B11110000,
  B11100000,
  B11000000,
  B10000000,
  B11000000,
  B11100000,
  B11110000
},{
  B01110000,
  B11111000,
  B11111100,
  B11101100,
  B11111100,
  B11111100,
  B11110000,
  B11110000
},{
  B00111110,
  B01111111,
  B11101110,
  B11111100,
  B11111000,
  B11111100,
  B11111110,
  B01111111
},{
  B00001111,
  B00011111,
  B00111111,
  B00111110,
  B00111111,
  B00111111,
  B00011111,
  B00001110
},{
  B00000011,
  B00000111,
  B00001111,
  B00001111,
  B00001111,
  B00001111,
  B00000111,
  B00000011
},{
  B00000000,
  B00000001,
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B00000001,
  B00000000
}};

unsigned Pacman::frames() const
{ return nframes; }

void Pacman::frame(unsigned idx)
{
  if (idx >= frames())
    return;

  const RgbColor yellow(255, 255, 0);

  ShowMouthSmiling();
  ShowFrame(pix[idx], yellow);
}

Pacman pacman;

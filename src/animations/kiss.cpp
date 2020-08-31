#include <animations/kiss.h>

static const RgbColor b(  0,   0,   0);
static const RgbColor w(255, 255, 255);
static const RgbColor r(255,   0,   0);

static const unsigned nframes = 4;
static const ColorFrames(nframes) pix = {
 {
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { w, w, w, w, w, w, w, w, },
  { w, b, b, r, r, b, b, w, },
  { b, w, w, w, w, w, w, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
 }, {
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, r, b, b, r, b, b, },
  { b, w, r, r, r, r, w, b, },
  { b, w, b, r, r, b, w, b, },
  { b, b, w, r, r, w, b, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
 }, {
  { b, b, b, b, b, b, b, b, },
  { b, r, r, b, b, r, r, b, },
  { b, r, r, b, b, r, r, b, },
  { b, b, r, r, r, r, b, b, },
  { b, b, r, r, r, r, b, b, },
  { b, b, w, r, r, w, b, b, },
  { b, b, b, r, r, b, b, b, },
  { b, b, b, b, b, b, b, b, },
 }, {
  { r, r, r, b, b, r, r, r, },
  { r, r, r, b, b, r, r, r, },
  { r, r, r, r, r, r, r, r, },
  { b, r, r, r, r, r, r, b, },
  { b, b, r, r, r, r, b, b, },
  { b, b, b, r, r, b, b, b, },
  { b, b, b, r, r, b, b, b, },
  { b, b, b, r, r, b, b, b, },
 },
};

unsigned Kiss::frames() const
{ return 2 * nframes; }


void Kiss::frame(unsigned idx)
{
  if (idx >= frames())
    return;

  if (idx >= (frames() / 2))
    idx = frames() - 1 - idx;

  ClearFrame();
  ShowFrame(pix[idx]);
}

Kiss kiss;

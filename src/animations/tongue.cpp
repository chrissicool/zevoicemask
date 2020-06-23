#include <animations/tongue.h>

static const RgbColor b(  0,   0,   0);
static const RgbColor w(255, 255, 255);
static const RgbColor t(214,  92, 110);

static const unsigned nframes = 4;
static const ColorFrames(nframes) pix = {
 {
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { w, w, w, w, w, w, w, w, },
  { w, b, b, b, b, b, b, w, },
  { b, w, w, w, w, w, w, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
 }, {
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { w, w, w, w, w, w, w, w, },
  { w, b, t, t, t, t, b, w, },
  { b, w, w, w, w, w, w, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
 }, {
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { w, w, w, w, w, w, w, w, },
  { w, b, t, t, t, t, b, w, },
  { b, w, w, t, t, w, w, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
 }, {
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { b, b, b, b, b, b, b, b, },
  { w, w, w, w, w, w, w, w, },
  { w, b, t, t, t, t, b, w, },
  { b, w, t, t, t, t, w, b, },
  { b, b, b, t, t, b, b, b, },
  { b, b, b, b, b, b, b, b, },
 },
};

unsigned Tongue::frames() const
{ return 2 * nframes; }


void Tongue::frame(unsigned idx)
{
  if (idx >= (frames() - 1))
    return;

  if (idx >= (frames() / 2))
    idx = frames() - 1 - idx;

  ClearFrame();
  ShowFrame(pix[idx]);
}

Tongue tongue;

#include <animations/boot.h>

static const unsigned segments = 3;
static const unsigned frames_per_seg = 9;
static const unsigned nframes = segments * frames_per_seg;
static const constexpr BwFrames(1) v PROGMEM = {
  B00000000,
  B00111000,
  B00000100,
  B00011000,
  B00000100,
  B00000100,
  B00111000,
  B00000000,
};

void Boot::init()
{
#ifdef VOICEMASK_DEBUG
  Serial.println(F("Version 3"));
#endif
}

unsigned Boot::frames() const
{ return nframes; }

void Boot::frame(unsigned idx)
{
  if (idx >= frames())
    return;

  ClearFrame();

  unsigned seg = idx / frames_per_seg;
  unsigned frame = idx % frames_per_seg;
  RgbColor fadein(255 / (frames_per_seg - frame));
  switch (seg) {
  case 0: /* Fade in version number. */
    PGM_ShowFrame(v, fadein);
    break;
  case 1: /* Fade in all pixels to full white. */
    PGM_ShowFrame(v, RgbColor(255));
    for (unsigned row = 0; row < ROWS; row++) {
      for (unsigned col = 0; col < COLUMNS; col++) {
        if (GetPixel(col, row) == 0)
          ShowPixel(col, row, fadein);
      }
    }
    break;
  case 2: /* Fade everything out a to smiling mouth. */
    ShowMouthSmiling();
    for (unsigned row = 0; row < ROWS; row++) {
      for (unsigned col = 0; col < COLUMNS; col++) {
        if (GetPixel(col, row) == 0)
          ShowPixel(col, row, 255 / (frame + 1));
      }
    }
    break;
  default:
    break;
  }
}

Boot boot;

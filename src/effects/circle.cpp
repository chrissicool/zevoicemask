#include <effects/circle.h>

enum : int {
  Tail = COLUMNS * 3 / 2,

  Up = -1,
  Down = 1,

  Left = -1,
  Right = 1,
};

class MouthAsCircle
{
private:
  int8_t _xdir, _ydir;
  uint8_t _xpos, _ypos;

  /** pre: needs valid _xpos, _ydir */
  void ypos()
  {
    RgbColor c;

    _ypos = (_ydir == Down) ? 0 : ROWS - 1;
    while ((c = GetPixel()) == 0) {
      _ypos += _ydir;
      if (_ypos == 0 || _ypos == ROWS - 1)
        break;
    }
  }

public:
  MouthAsCircle(int xdir, int ydir)
    : _xdir(xdir), _ydir(ydir)
  {
    _xpos = random(COLUMNS);
    ypos();
  }

  void advance()
  {
    if (_xpos == 0 && _xdir == Left) {
      _xpos = 1; _xdir = Right;
      _ydir *= -1;
    } else if (_xpos == COLUMNS - 1 && _xdir == Right) {
      _xpos = COLUMNS - 2; _xdir = Left;
      _ydir *= -1;
    } else {
      _xpos += _xdir;
    }
    ypos();
  }

  void SetPixel(const RgbColor &c)
  {
#if 0
    Serial.print(F("x: ")); Serial.print(_xpos);
    Serial.print(F(" xdir: ")); Serial.print((_xdir == Left) ? F("left") : F("right"));
    Serial.print(F(" y: ")); Serial.print(_ypos);
    Serial.print(F(" ydir: ")); Serial.print((_ydir == Up) ? F("up") : F("down"));
    Serial.println();
#endif

    ShowPixel(_xpos, _ypos, c);
  }

  RgbColor const GetPixel() const
  {
    return ::GetPixel(_xpos, _ypos);
  }
};

static RgbColor dimto;
static MouthAsCircle m(Right, Down);

void Circle::init()
{
  uint8_t r, g, b, rgb;

  r = random(128) + 128;
  g = random(128) + 128;
  b = random(128) + 128;

  rgb = random(8);
  if (rgb & 1)
    r = 224;
  if (rgb & 2)
    g = 224;
  if (rgb & 4)
    b = 224;

  dimto = RgbColor(r, g, b);

  int xdir = random(2) ? Left : Right;
  int ydir = random(2) ? Up : Down;
  m = MouthAsCircle(xdir, ydir);
}

void Circle::apply(unsigned percent)
{
  RgbColor dim, c;

  for (unsigned n = 0; n < Tail; n++) {
    c = m.GetPixel();

    dim = RgbColor(map(n, 0, Tail, dimto.R, 0),
                   map(n, 0, Tail, dimto.G, 0),
                   map(n, 0, Tail, dimto.B, 0));

    c -= dim;
    m.SetPixel(c);
    m.advance();
  }
}

Circle circle;

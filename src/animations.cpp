#include <Arduino.h>
#include <algorithm>

#include <animations.h>

#include <animation.h>
#include <animations/kiss.h>
#include <animations/matrix.h>
#include <animations/noise.h>
#include <animations/pacman.h>
#include <animations/scanner.h>
#include <animations/strobe.h>
#include <animations/tongue.h>
#include <animations/twinkle.h>

//#define ANIMATION twinkle

Animations::Animations()
  : _animations{
      &kiss,
      &matrix,
      &noise,
      &pacman,
      &scanner,
      &strobe,
      &tongue,
      &twinkle
    },
    _current(nullptr), _frame(0)
{ }

void Animations::choose()
{
  if (_current)
    _current->deinit();

  size_t idx = ::random(_animations.size());
#if defined(ANIMATION)
  auto it = std::find(_animations.begin(), _animations.end(), &ANIMATION);
  idx = it - _animations.begin();
#endif
  _current = _animations[idx];

  _current->init();
  _frame = 0;
}

bool Animations::active() const
{
  if (!_current)
    return false;
  return _frame < _current->frames();
}

void Animations::frame()
{
  if (_current) {
    _current->frame(_frame);
    _frame++;
  }
}

#ifdef VOICEMASK_DEBUG
const __FlashStringHelper *Animations::name()
{
  if (_current)
    return _current->name();
  return F("none");
}
#endif

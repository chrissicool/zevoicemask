#include <Arduino.h>

#include <animations.h>

#include <animation.h>
#include <animations/pacman.h>
#include <animations/scanner.h>
#include <animations/strobe.h>
#include <animations/tongue.h>
#include <animations/twinkle.h>

Animations::Animations()
  : _current(nullptr), _frame(0)
{
  _animations.push_back(&pacman);
  _animations.push_back(&scanner);
  _animations.push_back(&strobe);
  _animations.push_back(&tongue);
  _animations.push_back(&twinkle);
}

void Animations::choose()
{
  if (_current)
    _current->deinit();

  const size_t idx = ::random(_animations.size());
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

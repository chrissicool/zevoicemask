#include <effects.h>

#include <effect.h>
#include <effects/sparkle.h>

Effects::Effects() : _current(nullptr)
{
  _effects.push_back(&sparkle);
}

void Effects::choose()
{
  size_t idx = random(_effects.size() + 1);
  if (_current)
    _current->deinit();
  if (idx == _effects.size()) {
    _current = nullptr;
    return;
  }
  _current = _effects[idx];
  _current->init();
}

void Effects::apply(unsigned percent)
{
  if (_current)
    _current->apply(percent);
}

#ifdef VOICEMASK_DEBUG
const __FlashStringHelper *Effects::name()
{
  if (!_current)
    return F("none");
  return _current->name();
}
#endif

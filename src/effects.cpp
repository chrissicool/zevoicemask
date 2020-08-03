#include <algorithm>

#include <effects.h>

#include <effect.h>
#include <effects/blend.h>
#include <effects/circle.h>
#include <effects/distort.h>
#include <effects/sparkle.h>

//#define EFFECT blend

Effects::Effects()
{
  for (unsigned n = 0; n < N; ++n)
    _current[n] = nullptr;
  _effects.push_back(&blend);
  _effects.push_back(&circle);
  _effects.push_back(&distort);
  _effects.push_back(&sparkle);
}

void Effects::choose()
{
  for (unsigned n = 0; n < N; ++n) {
    size_t idx;
    bool doublet;

    if (_current[n])
      _current[n]->deinit();

    do {
      doublet = false;
      idx = random(_effects.size() + 1);
      for (unsigned i = 0; i < n; ++i) {
        if (idx < _effects.size() && _current[i] == _effects[idx])
          doublet = true;
      }
    } while (doublet);

#if defined(EFFECT)
    auto it = std::find(_effects.begin(), _effects.end(), &EFFECT);
    idx = it - _effects.begin();
    if (n > 0)
      idx = _effects.size();
#endif

    if (idx == _effects.size()) {
      _current[n] = nullptr;
      continue;
    }
    _current[n] = _effects[idx];
    _current[n]->init();
  }
}

void Effects::apply(unsigned percent)
{
  for (unsigned n = 0; n < N; ++n) {
    if (_current[n])
      _current[n]->apply(percent);
  }
}

#ifdef VOICEMASK_DEBUG
const String Effects::name()
{
  String r = F("{");
  for (unsigned n = 0; n < N; ++n) {
    if (_current[n])
      r += _current[n]->name();
    else
      r += F("none");
    if (n < N - 1)
      r += F(", ");
  }
  r += F("}");

  return r;
}
#endif

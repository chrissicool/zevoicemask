#pragma once

#include <effect.h>

class Sparkle : public Effect
{
  void init() override;
  void apply(unsigned percent) override;
  EFFECT_NAME("sparkle");
};

extern Sparkle sparkle;

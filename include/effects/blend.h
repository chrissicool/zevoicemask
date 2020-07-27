#pragma once

#include <effect.h>

class Blend : public Effect
{
  void init() override;
  void apply(unsigned percent) override;
  EFFECT_NAME("blend");
};

extern Blend blend;

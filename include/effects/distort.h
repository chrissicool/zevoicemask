#pragma once

#include <effect.h>

class Distort : public Effect
{
  void apply(unsigned percent) override;
  EFFECT_NAME("distort");
};

extern Distort distort;

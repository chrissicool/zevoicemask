#pragma once

#include <effect.h>

class Circle : public Effect
{
  void init() override;
  void apply(unsigned percent) override;
  EFFECT_NAME("circle");
};

extern Circle circle;

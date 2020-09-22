#pragma once

#include <animation.h>

class Noise : public Animation
{
public:
  void init() override;
  unsigned frames() const override;
  void frame(unsigned idx) override;
  ANIMATION_NAME("noise");
};

extern Noise noise;

#pragma once

#include <animation.h>

class Strobe : public Animation
{
public:
  void init() override;
  unsigned frames() const override;
  void frame(unsigned n) override;
  ANIMATION_NAME("strobe");
};

extern Strobe strobe;

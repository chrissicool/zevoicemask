#pragma once

#include <animation.h>

class Twinkle : public Animation
{
public:
  void init() override;
  unsigned frames() const override;
  void frame(unsigned n) override;
  ANIMATION_NAME("twinkle");
};

extern Twinkle twinkle;

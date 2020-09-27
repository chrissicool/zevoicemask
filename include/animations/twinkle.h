#pragma once

#include <animation.h>

class Twinkle : public Animation
{
public:
  void init() override;
  unsigned frames() const override;
  void frame(unsigned idx) override;
  ANIMATION_NAME("twinkle");
};

extern Twinkle twinkle;

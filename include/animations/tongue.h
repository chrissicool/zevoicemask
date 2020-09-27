#pragma once

#include <animation.h>

class Tongue : public Animation
{
public:
  unsigned frames() const override;
  void frame(unsigned idx) override;
  ANIMATION_NAME("tongue");
};

extern Tongue tongue;

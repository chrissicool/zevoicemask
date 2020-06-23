#pragma once

#include <animation.h>

class Tongue : public Animation
{
public:
  unsigned frames() const override;
  void frame(unsigned n) override;
  ANIMATION_NAME("tongue");
};

extern Tongue tongue;

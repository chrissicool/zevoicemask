#pragma once

#include <animation.h>

class Pacman : public Animation
{
public:
  unsigned frames() const override;
  void frame(unsigned idx) override;
  ANIMATION_NAME("pacman");
};

extern Pacman pacman;

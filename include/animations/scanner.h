#pragma once

#include <animation.h>

class Scanner : public Animation
{
public:
  void init() override;
  unsigned frames() const override;
  void frame(unsigned idx) override;
  ANIMATION_NAME("scanner");
};

extern Scanner scanner;

#pragma once

#include <animation.h>

class Matrix : public Animation
{
public:
  void init() override;
  unsigned frames() const override;
  void frame(unsigned idx) override;
  ANIMATION_NAME("matrix");
};

extern Matrix matrix;

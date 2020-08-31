#pragma once

#include <animation.h>

class Kiss : public Animation
{
public:
  unsigned frames() const override;
  void frame(unsigned n) override;
  ANIMATION_NAME("kiss");
};

extern Kiss kiss;

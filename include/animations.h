#pragma once

#include <Arduino.h>
#include <vector>

#include <animation.h>
#include <animations/pacman.h>
#include <animations/scanner.h>
#include <animations/strobe.h>
#include <animations/tongue.h>
#include <animations/twinkle.h>

class Animations
{
private:
  std::vector<Animation *> _animations;

public:
  Animations()
  {
    _animations.push_back(&pacman);
    _animations.push_back(&scanner);
    _animations.push_back(&strobe);
    _animations.push_back(&tongue);
    _animations.push_back(&twinkle);
  }

  Animation *random()
  {
    const size_t idx = ::random(_animations.size());
    return _animations[idx];
  }
};

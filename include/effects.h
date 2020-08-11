#pragma once

#include <Arduino.h>
#include <vector>

class Effect;

class Effects
{
private:
  enum {
    N = 3, ///< Number of effects to apply.
  };
  const std::vector<Effect *> _effects;
  Effect *_current[N];

public:
  Effects();

  /**
   * Choose new effect.
   */
  void choose();

  /**
   * Forwarder to apply chosen effect.
   */
  void apply(unsigned percent);

  /**
   * Chosen effect's name.
   */
  const String name();
};

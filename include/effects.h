#pragma once

#include <Arduino.h>
#include <vector>

class Effect;

class Effects
{
private:
  std::vector<Effect *> _effects;
  Effect *_current;

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
  const __FlashStringHelper *name();
};

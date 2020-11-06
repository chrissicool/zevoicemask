#pragma once

#include <Arduino.h>
#include <vector>

class Animation;

class Animations
{
private:
  const std::vector<Animation *> _animations;
  Animation *_current;
  unsigned _frame;

  void set(Animation *a);

public:
  Animations() noexcept;

  /**
   * Initialize animations.
   */
  void init();

  /**
   * Chooses a new animation and resets it to play from the beginning.
   */
  void choose();

  /**
   * Is the chosen animation still active?
   */
  bool active() const;

  /**
   * Play the next frame.
   */
  void frame();

#ifdef VOICEMASK_DEBUG
  /**
   * Return the current frame number.
   */
  unsigned frame_nr() const
  { return _frame; }

  /**
   * Chosen animation's name.
   */
  const __FlashStringHelper *name();
#endif
};

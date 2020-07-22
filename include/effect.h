#pragma once

#include <Arduino.h>
#include <frame.h>

/**
 * Base class for a mouth effect.
 */
class Effect
{
public:
  virtual ~Effect() { }

  /**
   * Apply the effect to the current frame.
   */
  virtual void apply(unsigned percent) = 0;

#ifdef VOICEMASK_DEBUG
  virtual const __FlashStringHelper *name() const = 0;

# define EFFECT_NAME(n) \
  const __FlashStringHelper *name() const override { return F(n); }
#else
# define EFFECT_NAME(n)
#endif /* VOICEMASK_DEBUG */

  /**
   * Initialize the effect.
   */
  virtual void init() { }

  /**
   * Clean up the effect.
   */
  virtual void deinit() { }
};

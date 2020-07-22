#pragma once

#include <frame.h>

/**
 * Base class for an idle animation.
 */
class Animation
{
public:
  virtual ~Animation() { }

  /**
   * Returns total number of frames for this animation.
   */
  virtual unsigned frames() const = 0;

  /**
   * Show n'th frame.
   */
  virtual void frame(unsigned n) = 0;

#ifdef VOICEMASK_DEBUG
  virtual const __FlashStringHelper *name() const = 0;

# define ANIMATION_NAME(n) \
  const __FlashStringHelper *name() const override { return F(n); }
#else
# define ANIMATION_NAME(n)
#endif /* VOICEMASK_DEBUG */

  /**
   * Initialize for a new set of frames.
   */
  virtual void init() { }

  /**
   * Clean up the set of frames.
   */
  virtual void deinit() { }
};

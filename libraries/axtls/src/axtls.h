#pragma once

/* axtls library by Rob Cermak
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "config.h"

// This is your main class that users will import into their application
class Axtls
{
public:
  /**
   * Constructor
   */
  Axtls();

  /**
   * Example method
   */
  void begin();

  /**
   * Example method
   */
  void process();

private:
  /**
   * Example private method
   */
  void doit();
};

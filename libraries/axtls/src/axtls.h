#pragma once

/* axtls library by Rob Cermak
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "config.h"

#if defined(CONFIG_DEBUG) && defined(CONFIG_PLATFORM_PARTICLE)

#if CONFIG_DEBUG_MODE == DEBUG_MODE_LOGGER

extern CONFIG_DEBUG_LOGGER_LIBRARY *outData;
#define debug_tls( fmt, ... ) outData->CONFIG_DEBUG_LOGGER_LEVEL(fmt, ##__VA_ARGS__)

#endif /* LOGGER */

#if CONFIG_DEBUG_MODE == DEBUG_MODE_SERIAL

extern Serial *outData;
#define debug_tls( fmt, ... ) outData->println(fmt, ##__VA_ARGS__)

#endif /* SERIAL */

#endif /* CONFIG_DEBUG */

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

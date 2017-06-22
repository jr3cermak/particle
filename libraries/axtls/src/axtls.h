#pragma once

/* axtls library by Rob Cermak
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "axtls_config.h"
#include "os_port.h"

// If you enable the debugger, you will need to define
// the debugger_callback function to read the final
// string and send it out the destination of your choice.
#if defined(CONFIG_DEBUG) && defined(CONFIG_PLATFORM_PARTICLE)

#define debug_tls( fmt, ... ) debugger_callback(fmt, ##__VA_ARGS__)

#endif /* CONFIG_DEBUG */

// This is your main class that users will import into their application
class axTLS
{
public:
  /**
   * Constructor
   */
  axTLS();

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

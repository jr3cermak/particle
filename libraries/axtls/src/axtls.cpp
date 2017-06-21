/* axtls library by Rob Cermak
 */

#include "axtls.h"

#if defined(CONFIG_DEBUG) && defined(CONFIG_PLATFORM_PARTICLE)

#if CONFIG_DEBUG_MODE == DEBUG_MODE_LOGGER

CONFIG_DEBUG_LOGGER_LIBRARY *outData =
  new CONFIG_DEBUG_LOGGER_LIBRARY(CONFIG_DEBUG_LOGGER_CATEGORY);

#endif /* LOGGER */

#if CONFIG_DEBUG_MODE == DEBUG_MODE_SERIAL

Serial *outData = new Serial();

#endif /* SERIAL */

#endif /* CONFIG_DEBUG */


/**
 * Constructor.
 */
Axtls::Axtls()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void Axtls::begin()
{
    // initialize hardware
    //Serial.println("called begin");
    debug_tls("called begin");
}

/**
 * Example method.
 */
void Axtls::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void Axtls::doit()
{
    Serial.println("called doit");
}

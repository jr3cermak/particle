/* axtls library by Rob Cermak
 */

#include "axtls.h"

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
    Serial.println("called begin");
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

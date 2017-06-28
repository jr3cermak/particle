/* wolfssl library by Rob Cermak
 */

#include "wolfssl.h"

/**
 * Constructor.
 */
Wolfssl::Wolfssl()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void Wolfssl::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void Wolfssl::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void Wolfssl::doit()
{
    Serial.println("called doit");
}

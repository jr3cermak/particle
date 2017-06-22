/* axTLS library by Rob Cermak
 * The base class for axTLSClient and axTLSServer
 *
 */

#include "axtls_config.h"
#include "axtls.h"

/**
 * Constructor.
 */
axTLS::axTLS()
{
  // be sure not to call anything that requires hardware be
  // initialized here, put those in begin()
}

/**
 * Example method.
 */
void axTLS::begin()
{
    // initialize hardware
#ifdef CONFIG_DEBUG
    debug_tls("axTLS::begin()\n");
#endif
}

/**
 * Example method.
 */
void axTLS::process()
{
    // do something useful
#ifdef CONFIG_DEBUG
    debug_tls("axTLS::process()\n");
#endif
    doit();
}

/**
 * Example private method.
 */
void axTLS::doit()
{
#ifdef CONFIG_DEBUG
    debug_tls("axTLS::doit()\n");
#endif
}

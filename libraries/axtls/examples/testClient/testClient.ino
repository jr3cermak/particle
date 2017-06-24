/*
 * Example: testSerial.ino
 * Description: This tests the instantiation of the base library class
 * Author: Rob Cermak
 * Date: 2017-06-20
 *
 * This is test 2 of 3 for for the debugging capabilities.
 *
 * Test 2/3: Define debug_tls() and use it with a pointer
 *   to the Serial class.                                
 *   
 *  
 */

/* 
 * Temporary: This helps allow the code to run when WiFi is
 * not available to the Photon.  We need to add a delay
 * as things are not quite setup before our code starts
 * running.
 */
//SYSTEM_MODE(AUTOMATIC);
//SYSTEM_THREAD(DISABLED);

#include "axtls.h"
#include "os_port.h"

// Use primary serial over USB interface for logging output
// You can watch logging of the Particle device via the Particle CLI
// This will enable basic diagnostic output.  See "axtls_config.h"
// for other options.
// $ particle serial monitor --follow
SerialLogHandler logHandler(LOG_LEVEL_ALL);
Logger myLog("axtls");

// FUNCTIONS

// This function is required when CONFIG_DEBUG is enabled.
void debugger_callback(const char* fmt, ...) {
  static char msg[CONFIG_DEBUG_BUFFER_SIZE] = { 0 };
  static int mptr = 0;
  char buf[CONFIG_DEBUG_BUFFER_SIZE];
  char ch;

  va_list(ap);
  va_start(ap, fmt);
  vsnprintf(buf, CONFIG_DEBUG_BUFFER_SIZE, fmt, ap);

  int buflen = strlen(buf);

  for (int i = 0; i < buflen; i++) {
    ch = buf[i];
    if (ch == 10 || ch == 13 || mptr == CONFIG_DEBUG_BUFFER_SIZE - 1) {
      if (mptr > 0) {
        myLog.info(msg);
      }
      memset(msg, 0, CONFIG_DEBUG_BUFFER_SIZE);
      if (mptr == CONFIG_DEBUG_BUFFER_SIZE - 1) {
        msg[0] = ch;
        mptr = 1;
      } else {
        mptr = 0;
      }
    } else {
      msg[mptr] = ch;
      mptr++;
    }
  }

  va_end(ap);
}

// Initialize objects from the lib
axTLSClient axClient;

// Need this or we run functions over and over again
int do_once = 0;

void setup() {
    // Call functions on initialized library objects that require hardware
    axClient.begin();
    Log.trace("begin(): This ignores debug options");
    debug_tls("begin()\n");
}

void loop() {
    int i = 0;
    int res = 0;
    // Use the library's initialized objects and functions
    if (do_once == 0) {
      axClient.process();
      do_once = 1;
      Log.trace("loop(): do_once; this ignores debug options");
      debug_tls("loop()\n");
      res = axClient.connect("jupyter.lccllc.info", 4443);
      Log.trace("Connect result=%d",res);
      debug_tls("Connect result=%d\n", res);
    }
}


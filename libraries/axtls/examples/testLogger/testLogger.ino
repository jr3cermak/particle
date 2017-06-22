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

// Use primary serial over USB interface for logging output
// You can watch logging of the Particle device via the Particle CLI
// This will enable basic diagnostic output.  See "axtls_config.h"
// for other options.
// $ particle serial monitor --follow
SerialLogHandler logHandler;
Logger myLog;

// FUNCTIONS

void debugger_callback(const char* fmt, ...) {
  char tmp[80];
  va_list(ap);
  va_start(ap, fmt);
  vsnprintf(tmp, 80, fmt, ap);

  //Serial.print(tmp);
  myLog.info(tmp);

  va_end(ap);
}

// Initialize objects from the lib
axTLS axtls;

// Need this or we run functions over and over again
int do_once = 0;

void setup() {
    // Call functions on initialized library objects that require hardware
    axtls.begin();
    Log.trace("begin(): This ignores debug options");
    debug_tls("begin()\n");
}

void loop() {
    int i = 0;
    // Use the library's initialized objects and functions
    if (do_once == 0) {
      axtls.process();
      do_once = 1;
      Log.trace("loop(): do_once; this ignores debug options");
      debug_tls("loop()\n");
    }
}


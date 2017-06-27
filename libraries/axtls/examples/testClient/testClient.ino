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
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

#include "axtls.h"
#include "os_port.h"
#include "Private.h"

// Use primary serial over USB interface for logging output
// You can watch logging of the Particle device via the Particle CLI
// This will enable basic diagnostic output.  See "axtls_config.h"
// for other options.
// $ particle serial monitor --follow
SerialLogHandler logHandler(LOG_LEVEL_ALL);
Logger myLog("axtls");

// GLOBALS
unsigned char* data = (unsigned char *) malloc(sizeof(unsigned char) * 400);
unsigned char url[80] = { 0 };
axTLSClient axClient;

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

  //myLog.trace("dc():%d",mptr);
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

int sendCmd(String command) {
  int res2 = 0;
  int len = 0;

  // Force a close of the client
  if (command == "c") {
    axClient.close();
  }

  // Various tests
  if (command == "j1") {
    debug_tls("Jupiter test #1\n");
    // Use the library's initialized objects and functions
    axClient.process();
    res2 = axClient.connect("jupyter.lccllc.info", 4443);
    debug_tls("j1 Connect(%d)\n", axClient.connected());
    if (axClient.connected()) {
      strcpy((char *)url,"/test");
      sprintf((char *)data, "GET %s HTTP/1.1\r\n",url);
      len = sprintf((char *)data, "%sHost: jupyter.lccllc.info\r\nUser-Agent: %s/%s\r\n\r\n", data, "axTLS", "2.3.1a");
      res2 = axClient.write(data);
      debug_tls("j1 write(): %d\n", res2);
      // header
      debug_tls("j1 start read()\n");
      res2 = axClient.read();
      debug_tls("j1 read(): %d\n", res2);
      // body
      debug_tls("j1 start read()\n");
      res2 = axClient.read();
      debug_tls("j1 read(): %d\n", res2);
      // more body
      while (axClient.available() > 0) {
        res2 = axClient.read();
        debug_tls("j1 read(): %d\n", res2);
      }
      debug_tls("j1 close()\n");
      axClient.close();
    }
    return 1;
  }

  if (command == "j2") {
    debug_tls("Jupiter test #2\n");
    // Use the library's initialized objects and functions
    axClient.process();
    res2 = axClient.connect("jupyter.lccllc.info", 4443);
    debug_tls("j2 Connect(%d)\n", axClient.connected());
    if (axClient.connected()) {
      strcpy((char *)url,"/");
      sprintf((char *)data, "GET %s HTTP/1.1\r\n",url);
      len = sprintf((char *)data, "%sHost: jupyter.lccllc.info\r\nUser-Agent: %s/%s\r\n\r\n", data, "axTLS", "2.3.1a");
      res2 = axClient.write(data);
      debug_tls("j2 write(): %d\n", res2);
      // header
      debug_tls("j2 start read()\n");
      res2 = axClient.read();
      debug_tls("j2 read(): %d\n", res2);
      // body
      debug_tls("j2 start read()\n");
      res2 = axClient.read();
      debug_tls("j2 read(): %d\n", res2);
      // more body
      while (axClient.available() > 0) {
        res2 = axClient.read();
        debug_tls("j2 read(): %d\n", res2);
      }
      debug_tls("j2 close()\n");
      axClient.close();
    }
    return 1;
  }

  return 0;
}

void setup() {

  // Allow us to send commands to the Particle
  Particle.function("cmd",sendCmd);

  // WaitFor WiFi 30 secs & Cloud for a total of 10 sec
  // With SYSTEM_THREAD(ENABLED); if we send to the log too quickly
  // it gets lost behind the scenes.
  waitFor(WiFi.ready, 30000);
  waitFor(Particle.connected, 5000);
  if (Particle.connected()) {
    waitFor(Particle.syncTimeDone, 5000);
    // Wait another 3 sec to allow subscription information to pass
    delay(3000);
  }

  // Call functions on initialized library objects that require hardware.
  // This function is empty.  Initialization is really in the constructor.
  axClient.begin();
  Log.trace("begin(): This ignores debug options");
  debug_tls("begin()\n");
}

void loop() {

}


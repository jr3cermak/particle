// Particle firmware tests
// What structures to we have access to in the Particle firmware.

#include "application.h"

time_t testtime_t;
int runonce = 0;

void setup() {
  Serial.begin(9600);

  testtime_t = Time.now();

}

void loop() {
  if (runonce == 0) {
    Serial.printlnf("time_t = %d", testtime_t);
    runonce = 1;
  }
}

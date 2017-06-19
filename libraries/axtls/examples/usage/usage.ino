// Example usage for axtls library by Rob Cermak.

#include "axtls.h"

// Initialize objects from the lib
Axtls axtls;

void setup() {
    // Call functions on initialized library objects that require hardware
    axtls.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    axtls.process();
}

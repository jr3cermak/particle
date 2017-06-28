// Example usage for wolfssl library by Rob Cermak.

#include "wolfssl.h"

// Initialize objects from the lib
Wolfssl wolfssl;

void setup() {
    // Call functions on initialized library objects that require hardware
    wolfssl.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    wolfssl.process();
}

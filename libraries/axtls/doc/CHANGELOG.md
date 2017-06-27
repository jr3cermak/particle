# Changelog

## 2017-06-27

* Found one bug in the axTLSClient:read() function.  I was not 
returning a valid string amount and it would never exit the function.
  * Example `j1` exits cleanly but does not close the connection properly.
  * Example `j2` does not display yet.  We get data though.
* Increased log buffer limit from 80 to 256 for testing.  This gives
us clearer log statements.  Small adjustment to detecting when we
have a full line or line feeds in debugger_callback() example.
* Still missing some line feeds in debugging output.
* Will add another `option` to enable/disable pointer display.  Some
  of these can now be hidden.
* Attempt to wait for WiFi and cloud to connect.  This seems to
  allow the subsequent logging to become established. 
* Use a Particle.function() to trigger client tests.
* Begin using SYSTEM_THREAD(ENABLED);
* Begin to update main README.md file to help people start using the
  library for testing.
* Example output for "j1": now in output/example_j1.txt
* The Private.h file may be completely blank right now unless you
are testing with Ubidots.
* This commit the client works, but does not exit gracefully with
either "j1" or "j2" example queries.
* I made a serious error somewhere, had to backpeddle and debug for a
day or so.  All I can figure is I was not careful with my #define
statements.  The last github commit should be more operational.



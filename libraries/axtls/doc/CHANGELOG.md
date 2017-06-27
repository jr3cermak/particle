# Changelog

## 2017-06-27

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



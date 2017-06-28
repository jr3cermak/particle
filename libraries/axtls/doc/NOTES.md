# Notes

# axTLS library notes

This port of the [axTLS](http://axtls.sourceforge.net/) library is based on
version 2.1.3 by Cameron Rich.

Specific porting notes are in PORT.md

Use of mutex(threading) is currently disabled.  Threading might be interesting
to investigate later.

There is a main SSL_CTX (ssl context) in which several SSL sessions can be
created and destroyed.  I believe this is mainly for server use where there may
be multiple connections to clients in which certificates/ciphers used can vary.

## Things to do

In the SSL structure, we should silently replace the int type with the
TCPClient type.  Each object will have a copy of TCPClient.  In case of the
server, it will keep all the pointers to those objects in the SSL link list.

There is a base certificate provided in the code for initializing the SSL
layer.  We need to know how to update it in the future.

There are many command line functions that are not implemented in this software
stack at present.  Authentication and alternative certificates, etc.

There are many diagnostic printf and the like type statements that have been
commented out in bulk by sed.  Other <stdio.h> functions are not available in
the C library for the Particle.   If you get printf or gettimeofday symbol
references look for these functions and comment them out and rewrite them.
Other rogue calls to time(), putc() and vsprint() will also trigger these
errors.  The only way to find them is to comment out or #define big blocks of
code out and slowly bring code/functions back into operation.  If you include
one function call at a time, you will slowly find the offending code that uses
<stdio.h> functions that should be avoided.

Other things to avoid are gettimeofday via sys/time.h.  This will pull in a
symbol gettimeofday that cannot be used with the Particle platform.

Other symbol errors may look like the following may not be as easy to find:

```
../../../build/target/user/platform-6-mlibuser.a(tls1_clnt.o): In function `ssl_client_new':
tls1_clnt.cpp:74: undefined reference to `ssl_new'
```

Check all your function arguments.  If there is a argument type that is mismatched in
the function, this may cause a symbol missing error.   There may be functions declared
as static that need to be removed so they are accessible by other functions.

There are some reserved words and variables imported via "Particle.h" or
"application.h" that needed to be avoided.

# Implementation

I think we would want to try hierarical inheritance for this library so we can
implemenet a base class which can be used by the client and server objects.

REF:

* https://www.tutorialcup.com/cplusplus/inheritance.htm

The axTLS library itself has a config.h to enable and disable code
components.  We need to add a CONFIG_PLATFORM_PARTICLE define.   I do
not see a reason to create a separate ELECTRON vs. PHOTON for this
library.

We should leverage several of the existing defines in config.h to
enable and disable classes, functions, debugging or services.

Utilizing existing defines:

* CONFIG_DEBUG
* CONFIG_SSL_SERVER_ONLY
* CONFIG_SSL_CERT_VERIFICATION
* CONFIG_SSL_ENABLE_CLIENT

Creating new defines:

* CONFIG_PLATFORM_PARTICLE
* CONFIG_SSL_CLIENT_ONLY
* CONFIG_SSL_CLIENT_INPUT_BUFFER 129
  * Default size of I/O buffer for TCPClient is 128 + NULL

If you want both client and server, define CONFIG_SSL_ENABLE_CLIENT and
!CONFIG_SSL_SERVER_ONLY.

If you want the client, define CONFIG_SSL_CLIENT_ONLY and
!CONFIG_SSL_SERVER_ONLY.

If you want the server, define CONFIG_SSL_SERVER_ONLY and
!CONFIG_SSL_ENABLE_CLIENT and !CONFIG_SSL_CLIENT_ONLY.

There are some run-time options:

* SSL_CLIENT_AUTHENTICATION
* SSL_SERVER_VERIFY_LATER
* SSL_NO_DEFAULT_KEY
* SSL_DISPLAY_STATES
* SSL_DISPLAY_BYTES
* SSL_DISPLAY_CERTS
* SSL_DISPLAY_RSA
* SSL_CONNECT_IN_PARTS

Some of these will only work when CONFIG_DEBUG is defined.

# Debugging / Logging

To access internal debugging of this library, CONFIG_DEBUG
needs to be defined and you define a callback function for
debug_tls() called debugger_callback() in your main program
with the .ino extension.

You can adjust the buffer size with the #define option called
CONFIG_DEBUG_BUFFER_SIZE.  It is currently set to 256 which
might be excessive.

For Log.trace() to work you need to ensure all log levels
are enabled:
`SerialLogHandler logHandler(LOG_LEVEL_ALL);`

# Classes

## Base

axTLS

## Subclasses the inherit from the base

axTLSClient

Members:

* available()
* certificateValid()
* connect()
* connected()
* flush()
* lastError()
* read()
* write()
* stop()

axTLSServer

Members:

NOTE: Development of the server portion is not planned until later or until a
volunteer can be found to take on the development/porting of that section of
code.  The general porting has been completed.

* available()
* begin()
* clients()
* read()
* write()
* stop()

# Members

Try to use similar function calls (and documentation) as the base Particle
firmware.

## axTLSClient

### available()

Returns the number of bytes available for reading for the current packet of
data from the server.  All the characters have to be read out of the current
packet (buffer) before the next packet of data is read from the server.  If you
are expecting a large amount of data, it needs to be reassembled.  Packet
size is 512 bytes.

```C++
// SYNTAX
axTLSClient.available()
```

Returns the number of bytes available for reading.

### certificateValid()

### connect()

### connected()

### flush()

### lastError()

### read()

### write()

### stop()

## axTLSServer

### available()

### begin()

### clients()

### read()

### write()

### stop()

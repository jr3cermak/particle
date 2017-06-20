# Notes

# axTLS library notes

This port of the [axTLS](http://axtls.sourceforge.net/) library is based on version 2.1.3 by Cameron Rich.

Specific porting notes are in PORT.md

Use of mutex(threading) is currently disabled.  Threading might be interesting to investigate later.

There is a main SSL_CTX (ssl context) in which several SSL sessions can be created
and destroyed.  I believe this is mainly for server use where there may be multiple
connections to clients in which certificates/ciphers used can vary.

## Things to do

In the SSL structure, we should silently replace the int type with the TCPClient
type.  Each object will have a copy of TCPClient.  In case of the server, it will
keep all the pointers to those objects in the SSL link list.

There is a base certificate provided in the code for initializing the SSL layer.  We need to
know how to update it in the future.

There are many command line functions that are not implemented in this software stack
at present.  Authentication and alternative certificates, etc.

There are many diagnostic printf and the like type statements that have been
commented out in bulk by sed.  Other <stdio.h> functions are not available in
the C library for the Particle.   If you get printf or gettimeofday symbol 
references look for these functions and comment them out and rewrite them. 
Other rogue calls to time(), putc() and vsprint() will also trigger these
errors.  The only way to find them is to comment out or #define big blocks of
code out and slowly bring code/functions back into operation.  If you include
one function call at a time, you will slowly find the offending code that uses
<stdio.h> functions that should be avoided.

There are some reserved words and variables imported via "Particle.h" or
"application.h" that needed to be avoided.

# Implementation

I think we would want to try hierarical inheritance for this library so we can
implemenet a base class which can be used by the client and server objects.

REF:

* https://www.tutorialcup.com/cplusplus/inheritance.htm

The axTLS library itself has a config.h to enable and disable code components.  We
need to add a CONFIG_PLATFORM_PARTICLE define.   I do not see a reason to create
a separate ELECTRON vs. PHOTON for this library.

We should leverage several of the existing defines in config.h to enable and disable
classes, functions, debugging or services.

Utilizing existing defines:

* CONFIG_DEBUG
* CONFIG_SSL_SERVER_ONLY
* CONFIG_SSL_CERT_VERIFICATION
* CONFIG_SSL_ENABLE_CLIENT

Creating new defines:

NOTE: If you attempt to use other items not in the available list
below, unpredicatable results may occur.   You have been warned!

* CONFIG_PLATFORM_PARTICLE
* CONFIG_DEBUG_MODE <type>
  * `<type>: { DEBUG_MODE_LOGGER, DEBUG_MODE_SERIAL }`
* CONFIG_DEBUG_LOGGER_NAME <logname>
* CONFIG_DEBUG_LOGGER_CATEGORY <appname>
* CONFIG_DEBUG_LOGGER_LEVEL <level>
  * `<level>: { trace, info, warn, error }`
* CONFIG_DEBUG_SERIAL
* CONFIG_DEBUG_SERIAL_PTR tty
* CONFIG_DEBUG_SERIAL_CLASS <class>
  * `<class>: { USARTSerial, USBSerial }`
* CONFIG_DEBUG_SERIAL_SPEED <baud>
* CONFIG_DEBUG_SERIAL_CHANNEL <device>
  * `<device> : { Serial, USBSerial1, Serial1, Serial2 }`
* CONFIG_SSL_CLIENT_ONLY

If you want both client and server, define CONFIG_SSL_ENABLE_CLIENT and !CONFIG_SSL_SERVER_ONLY.

If you want the client, define CONFIG_SSL_CLIENT_ONLY and !CONFIG_SSL_SERVER_ONLY.

If you want the server, define CONFIG_SSL_SERVER_ONLY and !CONFIG_SSL_ENABLE_CLIENT
and !CONFIG_SSL_CLIENT_ONLY.

There are some run-time options:

* SSL_CLIENT_AUTHENTICATION
* SSL_SERVER_VERIFY_LATER
* SSL_NO_DEFAULT_KEY
* SSL_DISPLAY_STATES
* SSL_DISPLAY_BYTES
* SSL_DISPLAY_CERTS
* SSL_DISPLAY_RSA
* SSL_CONNECT_IN_PARTS

Some of these will only work when CONFIG_DEBUG is also enabled.

# Debugging

The axTLS library is strewn with printf statements that we need to convert to something else.  We will convert
these to a generic debug_tls() routine that we will patch in using Serial or Logger.

## Logger

If you have these definitions:

```
#define CONFIG_DEBUG_MODE LOGGER
#define CONFIG_DEBUG_LOGGER_CATEGORY "axtls"
#define CONFIG_DEBUG_LOGGER_NAME appLog
#define CONFIG_DEBUG_LOGGER_LEVEL info
```

This should be the resulting code with given defines:

```
// Use primary serial over USB interface for logging output
// You can watch logging of the Particle device via CLI:
// $ particle serial monitor --follow
SerialLogHandler logHandler;
Logger appLog("axtls");
```

Code that calls `debug_tls(<format>, ...args...)`,
should call `appLog.info(<format>, ...args...)`.

## Serial

If you have these definitions:

```
#define CONFIG_DEBUG_MODE SERIAL
#define CONFIG_DEBUG_SERIAL_PTR tty
#define CONFIG_DEBUG_SERIAL_SPEED 9600
#define CONFIG_DEBUG_SERIAL_CLASS
#define CONFIG_DEBUG_SERIAL_CHANNEL Serial
```

This should be the resulting code with given defines:

```
tty->begin(9600);
```

Code that calls `debug_tls(<format>, ...args...)`,
should call `tty->println(<format>, ...args...)`.

We use println as each message passed to the Logger is always just a single line.  This
is the most compatible format with the Logger.

# Classes

## Base

axtls

## Subclasses the inherit from the base

axtlsClient

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

axtlsServer

Members:

NOTE: Development of the server portion is not planned until later or until a volunteer can
be found to take on the development/porting of that section of code.

* available()
* begin()
* clients()
* read()
* write()
* stop()

# Members

Try to use similar function calls (and documentation) as the base Particle firmware.

## axtlsClient

### available()

Returns the number of bytes available for reading for the current packet of data from the server.  All
the characters have to be read out of the current packet (buffer) before the next packet of data is
read from the server.  If you are expecting a large amount of data, it needs to be reassembled.

```C++
// SYNTAX
axtlsClient.available()
```

Returns the number of bytes available for reading.



# Notes

# axTLS library notes

Use of mutex(threading) is currently disabled.  Threading might be interesting to investigate later.

There is a main SSL_CTX (ssl context) in which several SSL sessions can be created
and destroyed.  I believe this is mainly for server use where there may be multiple
connections to clients in which certificates/ciphers used can vary.

In the SSL structure, we should silently replace the int type with the TCPClient
type.  Each object will have a copy of TCPClient.  In case of the server, it will
keep all the pointers to those objects in the SSL link list.

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

* CONFIG_PLATFORM_PARTICLE
* CONFIG_DEBUG_USBSERIALLOG <appname>
* CONFIG_DEBUG_SERIAL <baud>
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



# Porting axTLS v2.1.3 to Particle

These are specific notes about source code changes and justifications
if possible.

See NOTES.md for design discussion.

# Tools

Any specialized tools are in the bin directory.

## bash

### mklibAxTls

This script will operate from the axTLS source tree in its native form.
You need to define several locations:

* `${PARTICLE_PROJECT_DIR}`: root library directory destination, not the library
* `${SRC}`: root of the axTLS source tree
* `${ORG}`: any original code you need in ADDITION to the axTLS library; this includes the new Particle class code
* `${PPD}`: this is the destination directory for the library

What this script does in a nutshell:

* Copies *.c and *.h files to the destination libary
  * c files are renamed on the fly to cpp
* Custom code from `${ORG}` is copied.
  * c files are renamed on the fly to cpp
  * Particle class files should be named .c
* If you have made customized changes to original code, the script
checks for an original file and a modified file.  It will recompute the diff.
* Any patch files are then applied to the library.
* Any bulk operations are then performed.
  * `printf -> //printf`

# Imported code from other sources

## be64toh() and others

sha512.cpp requires a be64toh() function

* REF: https://github.com/mobile-shell/mosh
* REF: https://raw.githubusercontent.com/mobile-shell/mosh/master/src/crypto/byteorder.h

## ntohl() and others

aes.cpp requires a ntohl() function

* REF: https://developer.mbed.org/users/vshymanskyy/code/Blynk/docs/b942afadf9be/BlynkProtocolDefs_8h_source.html
* This is a pretty good bet as there is a define() that checks for a PARTICLE definition.

```
/**
 * @file       BlynkProtocolDefs.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Blynk protocol definitions
 *
 */
```

# Newly created content

# Source files (modified)

## ssl.h

* Add #define for CONFIG_PLATFORM_PARTICLE to functions using int client_fd and
create a companion SSL *client_fd argument

## tls1_svr.cpp

* ssl_server_new() client_fd (int) -> (SSL *)

## asn1.cpp

* Add extern "C" {}
* asn1_version() 3rd argument (int) -> (int32_t)

## tls1_clnt.cpp

* ssl_client_new() (int) -> (SSL *) for client_fd

## crypto_misc.h

* Change: int32_t basic_constraint_pathLenConstraint;
* prototype asn1_version() 3rd argument (int) -> (int32_t)

## p12.cpp

* returned pointer from malloc needs a cast to (uint8_t*)

## x509.cpp

* Add `#include "axtls.h"` to the top if CONFIG_DEBUG && CONFIG_PLATFORM_PARTICLE is defined.
  * TODO: There are some disjointed lines in x509_print()
* Fix up more printf statements
* Change references of (int*) to (int32_t*) for basic_constraint_pathLenContraint
  * Definition in crypto_misc.h
* Reimplement gettimeofday -> gettimeofdayParticle()
* We might have problems with ctime()
* x509_new() version (int) -> (int32_t)

## bigint.h

* Add `extern "C"` definition.

## bigint.cpp

* Add `#include "axtls.h"` to the top if CONFIG_DEBUG && CONFIG_PLATFORM_PARTICLE is defined.
* Fix up printf statements that span multiple lines.
* There is a small section to fix up in bi_print().   

## tls1.h

* Add prototype definitions for I/O callback functions.
* Add f_send/f_recv and _client to SSL_CTX (ssl context) typedef.
  * _client will keep the pointer to TCPClient.
* Add typedefs for the callback function variables.
* In the SSL typedef, we will replace the (int) with (_SSL*) which will be just
a pointer back to the SSL structure.  This avoids messing with SOCKET_READ() and SOCKET_WRITE().
* Doing that, we have to fix a couple of additional functions and prototypes using client_fd.
  * ssl_find()
  * ssl_new()

## tls1.cpp

* The bulk commenting out of printf has side effects.  Any multiline printf
statements cause errors.  We will rewrite these now.
* Add `#include "axtls.h"` to the top if CONFIG_DEBUG && CONFIG_PLATFORM_PARTICLE is defined.
* Add Particle specific routines for I/O with TCPClient using function callbacks.
* send_raw_packet(); rework file descriptor use
* send_packet(); alloca needs a cast on the return pointer
* errno and friends are got undefined somehow? It compiled before.
* Update ssl_client_new int client_fd to SSL *client_fd.

## aes.cpp

* Reserved word `xor` used; renamed to `txor` in #define copy of code

## os_port.cpp

* Implement gettimeofday

## os_port.h

* Include ntohl() patch
* Avoid unnecessary #include, particular failure was <netdb.h>
* Need to define SOCKET_WRITE, SOCKET_READ and SOCKET_CLOSE
* Implement gettimeofday

```
#define SOCKET_READ(A,B,C)      readParticle(A,B,C)
#define SOCKET_WRITE(A,B,C)     writeParticle(A,B,C)
#define SOCKET_CLOSE(A)         closeParticle(A)
#define TTY_FLUSH()
```

## debugging.cpp; debugging.h

* Copied routines mainly used for the command line to allow project to
compile cleanly.  These can be phased out or reimplemented.
* Remove `static` definition as this is referenced outside this code
block.
* Add `extern "C"` definition to debugging.h

## config.h

Using a copy from a basic Linux compile, the following options were changed or added:

* Removed the header warning
* CHG: #undef CONFIG_PLATFORM_LINUX
* ADD: #define CONFIG_PLATFORM_PARTICLE 1
* CHG: #define CONFIG_DEBUG 1
* CHG: #undef CONFIG_SSL_CTX_MUTEXING
* ADD: #define CONFIG_SSL_CLIENT_TIMEOUT 5000
* ADD: #define CONFIG_SSL_CLIENT_RETRY_TIMEOUT 5000
* ADD: #define CONFIG_SSL_CLIENT_MAX_RETRY 5
* ADD: #define CONFIG_DEBUG_

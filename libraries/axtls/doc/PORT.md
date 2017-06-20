# Porting axTLS v2.1.3 to Particle

These are specific notes about source code changes and justifications if possible.

See NOTES.md for design discussion.

# Tools

Any specialized tools are in the bin directory.

## bash

### mklibAxTls

This script will operate from the axTLS source tree in its native form.  You need to
define several locations:

* PARTICLE_PROJECT_DIR: root library directory destination, not the library
* SRC: root of the axTLS source tree
* ORG: any original code you need in ADDITION to the axTLS library; this includes the new Particle class code
* PPD: this is the destination directory for the library

What this script does in a nutshell:

* Copies *.c and *.h files to the destination libary
  * c files are renamed on the fly to cpp
* Custom code from ${ORG} is copied.
  * c files are renamed on the fly to cpp
  * Particle class files should be named .c
* If you have made customized changes to original code, the script checks for an original file and a
modified file.  It will recompute the diff.
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

# Source files

## debugging.cpp; debugging.h

Copied routines mainly used for the command line to allow project to
compile cleanly.  These can be phased out or reimplemented.



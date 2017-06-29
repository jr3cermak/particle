# Porting notes 

This port is based on release version 3.11.0

# Unpacking

Run the script in IDE/ARDUINO to setup the code base (wolfssl-arduino.sh).

Populate `src/` tree:

cp *.c -> *.cpp
cp -a wolfssl `src/`

I created a helper script to do these parts.  That script is in the 
`bin/` directory.  Edit variables for correct paths in `mklibwolfssl`
before running.

# Setup

## src/wolfssl/wolfcrypt/settings.h

Define a few things.

My suggestion is use one of the ARM settings.  The
`WOLFSSL_IAR_ARM` correctly does not pull in the sys/socket.h
header file.  `NO_OLD_RNGNAME` avoids a redefinition conflict
with the Particle firmware that uses `RNG`.

```
#define WOLFSSL_IAR_ARM
#define NO_OLD_RNGNAME
```

# Code changes

## Example

### wolfssl_client.cpp


## Base code

### src/misc.cpp

Create a symbolic link between src/misc.cpp and
wolfcrypt/src/misc.c

### ssl.h

`src/wolfssl/ssl.h:2328:13: error: 'size_t' does not name a type`

Add "application.h" to ssl.h.  
[REF](https://community.particle.io/t/digole-uart-i2c-spi-display-library/2392/281?u=cermak)

### ssl.cpp

One alloc to cast using given defines.

`rng = (WC_RNG*) XMALLOC(sizeof(WC_RNG), ctx->heap, DYNAMIC_TYPE_RNG);`



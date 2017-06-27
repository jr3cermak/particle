# axtls

A Particle library for axtls

## Welcome to your library!

To get started, modify the sources in [src](src). Rename the example folder inside [examples](examples) to a more meaningful name and add additional examples in separate folders.

To compile your example you can use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

Libraries can also depend on other libraries. To add a dependency use [`particle library add`](https://docs.particle.io/guide/tools-and-features/cli#adding-a-library) or [library management](https://docs.particle.io/guide/tools-and-features/dev/#managing-libraries) in Desktop IDE.

## Usage

Add the axtls library to your project and follow this simple example:

```
#include "axtls.h"
#inclide "os_port.h"
#include "Private.h"

axTLSClient axClient;

void setup() {
}

void loop() {
}
```

You may leave "Private.h" out, however, place anything private like API keys in this
file and be sure to use .gitignore to keep it from being posted on your repository.

In a future example, if you are using this library to connect to Ubidots, your
"Private.h" may look like this:

```
/**
 *
 *  Include private definitions, tokens, etc
 *
 */

#ifndef PRIVATE_H
#define PRIVATE_H

// Authentication token
#ifndef AUTHTOKEN
#define AUTHTOKEN "w............................A"
#endif

#endif
```

See the [examples](examples) folder for more details.

## Examples

### usage

A very blank project just initializing the library and not using it at all.

### testLogger

A very blank project, but illustrates the debugging through the USB serial
interface.

### testSerial

A very blank project, but illustrates the debugging through the conventional
Serial interface.

### testClient

This contains various client tests to exercise the library.  This uses the
USB serial log for debugging.

A Particle.function("cmd") is established for testing various websites
using SSL/https.  

Valid "cmd" strings to pass:

* `j1`: jupyter.lccllc.info:4443/test - page does not exist
* `j2`: jupyter.lccllc.info:/ - pull up the root page
* `u1`: things.ubidots.com: with valid AUTHTOKEN, use API to
        show existing dashboards.

### testServer

No working tests yet.

## Documentation

TODO: Describe `Axtls`

Still testing and stabilizing.  See [doc](doc) directory for development notes, etc.

## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add axtls_myname` to add the library to a project on your machine or add the axtls_myname library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library. 

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
Copyright 2017 Rob Cermak

The predominant license is Creative Commons 4 unless otherwise specified by
supplied code.

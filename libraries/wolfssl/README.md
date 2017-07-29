# wolfssl

A Particle library for wolfssl

## Welcome to your library!

To get started, modify the sources in [src](src). Rename the example folder inside [examples](examples) to a more meaningful name and add additional examples in separate folders.

To compile your example you can use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

Libraries can also depend on other libraries. To add a dependency use [`particle library add`](https://docs.particle.io/guide/tools-and-features/cli#adding-a-library) or [library management](https://docs.particle.io/guide/tools-and-features/dev/#managing-libraries) in Desktop IDE.

After the library is done you can upload it with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. If you wish to make your library public, use `particle library publish` or `Publish` command.

## Usage

See the [examples](examples) folder for more details.

[usage.ino](https://github.com/jr3cermak/particle/blob/master/libraries/wolfssl/examples/usage/usage.ino):
will only test instantiation of the library and not exercise it at all.

[wolfssl_client.ino](https://github.com/jr3cermak/particle/blob/master/libraries/wolfssl/examples/wolfssl_client/wolfssl_client.ino): Client test.  You need to adjust the 
host, port and/or msg to obtain the correct response.  A small delay
is needed to allow the server to respond otherwise the response could
be skipped.

## Documentation

See [wolfssl](https://www.wolfssl.com) for more concise documentation
about the library.

If you are starting from wolfssl source, try using the mklibwolfssl script
found on the git repository to setup the initial source and then apply
changes to the library.

## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with
`particle library upload` or `Upload` command in the IDE. This will
create a private (only visible by you) library that you can use in
other projects. Do `particle library add wolfssl_myname` to add the
library to a project on your machine or add the wolfssl_myname library
to a project on the Web IDE or Desktop IDE.

At this point, you can create a
[GitHub pull request](https://help.github.com/articles/about-pull-requests/)
with your changes to the original library. 

If you wish to make your library public, use `particle library publish`
or `Publish` command.

## LICENSE
Copyright 2017 Rob Cermak

Mixed license.  Any original new code is Creative Commons 4.  Other
code is subject to the original license provided by
[wolfssl](https://www.wolfssl.com) and
[newlib libc](https://sourceware.org/newlib/).

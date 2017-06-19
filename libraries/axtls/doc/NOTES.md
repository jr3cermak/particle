# Notes

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

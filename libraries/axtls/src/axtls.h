#pragma once

/* axtls library by Rob Cermak
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "axtls_config.h"
#include "os_port.h"
#include "ssl.h"
#include "debugging.h"

// If you enable the debugger, you will need to define
// the debugger_callback function to read the final
// string and send it out the destination of your choice.
#if defined(CONFIG_DEBUG) && defined(CONFIG_PLATFORM_PARTICLE)

#define debug_tls( fmt, ... ) debugger_callback(fmt, ##__VA_ARGS__)

#endif /* CONFIG_DEBUG */

// This is your main class that users will import into their application
class axTLS
{
public:
  static int objectCount;
  /**
   * Constructor
   */
  axTLS();

  /**
   * Example method
   */
  void begin();

  /**
   * Example method
   */
  void process();

private:
  /**
   * Example private method
   */
  void doit();
};

/**
 * Client object
 */
class axTLSClient :public axTLS
{
public:
  /**
   * Constructor
   */
  axTLSClient();
  int connect(const char* hn, uint16_t port);

private:
  int res, i = 2;
  uint16_t port = 443;
  uint32_t options = SSL_SERVER_VERIFY_LATER|SSL_DISPLAY_CERTS;
  char *private_key_file = NULL;
  struct hostent *hostent;
  int reconnect = 0;
  SSL_CTX *ssl_ctx;
  SSL *ssl = NULL;
  int quiet = 0;
  int cert_index = 0, ca_cert_index = 0;
  int cert_size, ca_cert_size;
  char **ca_cert, **cert;
  uint8_t session_id[SSL_SESSION_ID_SIZE];
  const char *password = NULL;
  SSL_EXTENSIONS *extensions = NULL;
  TCPClient _client;
  int connected = false;
  int retry = 0;
  SSL *client_fd = NULL;
};

/**
 * Server object
 */
class axTLSServer :public axTLS
{
public:
  /**
   * Constructor
   */
  axTLSServer();

private:
};

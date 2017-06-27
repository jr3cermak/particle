#pragma once

/* axtls library by Rob Cermak
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "axtls_config.h"
#include "os_port.h"
#include "ssl.h"
#include "debugging.h"

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

  // static members are shared between all created objects
  static int recvTLS(void *ssl, uint8_t *in_data, int in_len);
  static int sendTLS(void *ssl, uint8_t *out_data, int out_len);

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
  int available();
  int close();
  int connect(const char* hn, uint16_t port);
  int connected();

  // Basic read and write routines for Client
  int read();
  int write(uint8_t *msg);

private:
  int res, i = 2;
  uint16_t port = 443;
  uint32_t options = SSL_SERVER_VERIFY_LATER|SSL_DISPLAY_CERTS|SSL_DISPLAY_STATES;
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
  int _connected = false;
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

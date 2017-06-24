/* axTLS library by Rob Cermak
 * The base class for axTLSClient and axTLSServer
 *
 */

#include "axtls_config.h"
#include "axtls.h"

/**
 * Constructor
 */
axTLS::axTLS()
{
  // be sure not to call anything that requires hardware be
  // initialized here, put those in begin()
#ifdef CONFIG_DEBUG
  debug_tls("axTLS()\n");
#endif
}

/**
 * Example method.
 */
void axTLS::begin()
{
    // initialize hardware
#ifdef CONFIG_DEBUG
    debug_tls("axTLS::begin()\n");
#endif
}

/**
 * Example method.
 */
void axTLS::process()
{
    // do something useful
#ifdef CONFIG_DEBUG
    debug_tls("axTLS::process()\n");
#endif
    doit();
}

/**
 * Example private method.
 */
void axTLS::doit()
{
#ifdef CONFIG_DEBUG
    debug_tls("axTLS::doit()\n");
#endif
}

/**
 * Client constructor
 */
axTLSClient::axTLSClient()
{
#ifdef CONFIG_DEBUG
  debug_tls("axTLSClient()\n");
#endif
  //sin_addr = inet_addr("127.0.0.1");
  cert_size = ssl_get_config(SSL_MAX_CERT_CFG_OFFSET);
  ca_cert_size = ssl_get_config(SSL_MAX_CA_CERT_CFG_OFFSET);
  ca_cert = (char **)calloc(1, sizeof(char *)*ca_cert_size);
  cert = (char **)calloc(1, sizeof(char *)*cert_size);
}

int axTLSClient::connect(const char* hn, uint16_t port)
{
  debug_tls("begin connect()");
  debug_tls("host:%s port:%d", hn, port);

  cert_size = ssl_get_config(SSL_MAX_CERT_CFG_OFFSET);
  ca_cert_size = ssl_get_config(SSL_MAX_CA_CERT_CFG_OFFSET);
  ca_cert = (char **)calloc(1, sizeof(char *)*ca_cert_size);
  cert = (char **)calloc(1, sizeof(char *)*cert_size);

  if ((ssl_ctx = ssl_ctx_new(options, SSL_DEFAULT_CLNT_SESS)) == NULL) {
    debug_tls("Error: Client context is invalid");
    return 1;
  }

  // Always perform an init here to update ssl_ctx pointer on
  // i/o pathway.  TODO: fix later
  //this->init();

  // Assign ssl_ctx->_client here
  ssl_ctx->_client = &_client;

  free(cert);
  free(ca_cert);

  // Ready to make the connection, but via the TcpClient layer
  while (!connected && retry < CONFIG_SSL_CLIENT_MAX_RETRY) {
    retry++;
    debug_tls("connect() try %d", retry);
    if (_client.connect(hn, port)) {
      connected = true;
    } else {
      delay(CONFIG_SSL_CLIENT_RETRY_TIMEOUT);
    }
  }
  if (!connected) {
    debug_tls("Connection failed");
    return 1;
  }

    /* Try session resumption? */
    if (reconnect)
    {
        while (reconnect--)
        {
            ssl = ssl_client_new(ssl_ctx, client_fd, session_id,
                    sizeof(session_id), extensions);
            if ((res = ssl_handshake_status(ssl)) != SSL_OK)
            {
                if (!quiet)
                {
                    ssl_display_error(res);
                }
                ssl_free(ssl);
                ssl_ctx_free(ssl_ctx);
                _client.stop();
                connected = false;
                return 1;
            }

            display_session_id(ssl);
            memcpy(session_id, ssl_get_session_id(ssl), SSL_SESSION_ID_SIZE);

            if (reconnect)
            {
                ssl_free(ssl);
                //SOCKET_CLOSE(client_fd);
                _client.stop();

                //client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                //connect(client_fd, (struct sockaddr *)&client_addr, 
                //        sizeof(client_addr));
                connected = _client.connect(hn, port);
                // Should bail here if connect returns false
            }
        }
    }
    else
    {
        debug_tls("ssl_client_new()");
        ssl = ssl_client_new(ssl_ctx, client_fd, NULL, 0, extensions);
    }

    /* check the return status */
    if ((res = ssl_handshake_status(ssl)) != SSL_OK)
    {
        if (!quiet)
        {
            ssl_display_error(res);
        }
        debug_tls("Handshake error: %d",res);
        ssl_free(ssl);
        ssl_ctx_free(ssl_ctx);
        _client.stop();
        connected = false;
        return 1;
    }

    if (!quiet)
    {
        display_session_id(ssl);
        display_cipher(ssl);
    }

    // At this point we are connected and verified over SSL
    // or not connected. 
    // res = 0 (success)
    // res = !0 (failed)
    // We can also check the state of !connected
    debug_tls("connect():%d",res);
    return res;

}


/**
 * Server constructor
 */
axTLSServer::axTLSServer()
{
#ifdef CONFIG_DEBUG
  debug_tls("axTLSServer()\n");
#endif
}


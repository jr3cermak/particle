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
  debug_tls("axTLS()\n");
}

/**
 * Example method.
 */
void axTLS::begin()
{
    // initialize hardware
    debug_tls("axTLS::begin()\n");
}

/**
 * Example private method.
 */
void axTLS::doit()
{
    debug_tls("axTLS::doit()\n");
}

/**
 * Example method.
 */
void axTLS::process()
{
    // do something useful
    debug_tls("axTLS::process()\n");
    doit();
}

// Generic reader via SSL
int axTLSClient::read() {
  int ctimeout = 0;
  int res = 0;
  unsigned char *readbuffer;
  char buf[512] = { 0 };
  int bptr = 0;
  unsigned int i;
  unsigned char ch;
  int nb;

  debug_tls("read:begin()\n");

  // Wait a timeout or a valid response
  while (ctimeout < CONFIG_HTTP_TIMEOUT and res == 0) {
    if (_connected) {
      if (_client.available() > 0) {
        nb = ssl_read(ssl, &readbuffer);
        if (nb > 0) {
          debug_tls("read(%d)\n",strlen((char*)readbuffer));
          for (i = 0; i < strlen((char*)readbuffer); i++) {
            ch = readbuffer[i];
            if (ch == 10 || ch == 13) {
              if (bptr > 0) {
                debug_tls("web>%s\n",(const char*)buf);
                bptr = 0;
                memset(buf, 0, sizeof(buf));
              }
            } else {
              snprintf(buf+bptr,512-bptr,"%c",ch);
              bptr++;
            }
          }
          if (bptr > 0) {
            debug_tls("web>%s\n",(const char*)buf);
            bptr = 0;
            memset(buf, 0, sizeof(buf));
          }
        } else {
          if (nb < 0) {
            res = nb;
          }
        }
        ctimeout = 0;
      } else {
        Particle.process();
        delay(100);
      }
    } else {
      res = -1;
    }
    ctimeout++;
  }

  // The connection closed or died
  if (res < 0) {
      ssl_free(ssl);
      ssl_ctx_free(ssl_ctx);
      _client.stop();
      _connected = false;
  }
  debug_tls("read:end(%d)\n",res);
  return res;
}

// This is the SOCKET_READ we need
// We pass this into axtls library as
// a function callback.
// We need to introduce a timer to wait for
// the desired number of bytes.
//
int axTLS::recvTLS(void *ssl, uint8_t *in_data, int in_len) {
  int ret = 0;
  int timeout = 0;

  // We have to extract the _client pointer
  SSL *real_ssl = (SSL *)ssl;
  TCPClient *sock = (TCPClient *) real_ssl->ssl_ctx->_client;

  debug_tls("Want %d byte(s)\n",in_len);
  Particle.process();

  //debug_tls("ssl(%p) ssl_ctx(%p) sock(%p)\n",real_ssl,real_ssl->ssl_ctx,sock);
  debug_tls("R:sock->connected():%d sock->available():%d\n", sock->connected(), sock->available());

  // Loop here for a bit until we get data or we timeout
  while (ret == 0 && timeout < CONFIG_HTTP_TIMEOUT) {
    if (sock->connected()) {
      if (sock->available() > 0) {
        ret = sock->read(in_data, in_len);
      } else {
        delay(10); // 0.01s (300 ~ 3s)
        Particle.process();
        timeout++;
      }
    } else {
      // No longer connected, return a bad result
      ret = -1;
    }
  }
  // Check for timeout
  if (timeout >= CONFIG_HTTP_TIMEOUT) {
    debug_tls("TCP read() timeout\n");
    ret = -1;
  }

  debug_tls("Got %d byte(s)\n",ret);
  return ret;
}

// This is the SOCKET_WRITE we need
// We pass this into axtls library as
// a function callback.
int axTLS::sendTLS(void *ssl, uint8_t *out_data, int out_len) {
  int ret = 0;

  debug_tls("Want to send %d byte(s)\n", out_len);

  // We have to extract the _client pointer
  SSL *real_ssl = (SSL *)ssl;
  TCPClient *sock = (TCPClient *) real_ssl->ssl_ctx->_client;

  //debug_tls("ssl(%p) ssl_ctx(%p) sock(%p)\n",real_ssl,real_ssl->ssl_ctx,sock);
  debug_tls("S:sock->connected():%d sock->available():%d\n", sock->connected(), sock->available());
  if (sock->connected()) {
    ret = sock->write(out_data, out_len);
    sock->flush();
  } else {
    debug_tls("sendTLS(): no connection\n", ret);
    return -1;
  }

  // Allow the WiFi module to catch up
  Particle.process();
  debug_tls("Sent %d byte(s)\n", ret);

  return ret;
}

// Generic writer over SSL
int axTLSClient::write(uint8_t *msg) {
  int res = 0;
  if (_connected) {
    Particle.process();
    res = ssl_write(ssl, msg, strlen((char *)msg));

    if (res < 0) {
      debug_tls("ssl_write() failed:%d\n", res);
      ssl_free(ssl);
      ssl_ctx_free(ssl_ctx);
      _client.stop();
      _connected = false;
      res = -1;
    } else {
      debug_tls("Wrote to ssl_write()\n");
    }
  }
  return res;
}

/**
 * Client constructor
 */
axTLSClient::axTLSClient()
{
  debug_tls("axTLSClient()\n");
  //sin_addr = inet_addr("127.0.0.1");
  cert_size = ssl_get_config(SSL_MAX_CERT_CFG_OFFSET);
  ca_cert_size = ssl_get_config(SSL_MAX_CA_CERT_CFG_OFFSET);
  ca_cert = (char **)calloc(1, sizeof(char *)*ca_cert_size);
  cert = (char **)calloc(1, sizeof(char *)*cert_size);
}

int axTLSClient::available() {
  return _client.available();
}

int axTLSClient::close()
{
  return 0;
}

int axTLSClient::connect(const char* hn, uint16_t port)
{
  debug_tls("begin connect()\n");
  debug_tls("host:%s port:%d\n", hn, port);

  cert_size = ssl_get_config(SSL_MAX_CERT_CFG_OFFSET);
  ca_cert_size = ssl_get_config(SSL_MAX_CA_CERT_CFG_OFFSET);
  ca_cert = (char **)calloc(1, sizeof(char *)*ca_cert_size);
  cert = (char **)calloc(1, sizeof(char *)*cert_size);

  if ((ssl_ctx = ssl_ctx_new(options, SSL_DEFAULT_CLNT_SESS)) == NULL) {
    debug_tls("Error: Client context is invalid\n");
    return 1;
  }

  // Always perform an init here to update ssl_ctx pointer on
  // i/o pathway.  TODO: fix later
  //this->init();
  set_io_callbacks(ssl_ctx, &axTLS::sendTLS, &axTLS::recvTLS);

  // Assign ssl_ctx->_client here
  ssl_ctx->_client = &_client;

  free(cert);
  free(ca_cert);

  // Ready to make the connection, but via the TcpClient layer
  while (!_connected && retry < CONFIG_SSL_CLIENT_MAX_RETRY) {
    retry++;
    debug_tls("connect() try %d\n", retry);
    if (_client.connect(hn, port)) {
      _connected = true;
    } else {
      delay(CONFIG_SSL_CLIENT_RETRY_TIMEOUT);
    }
  }
  if (!_connected) {
    debug_tls("Connection failed\n");
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
                _connected = false;
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
                _connected = _client.connect(hn, port);
                // Should bail here if connect returns false
            }
        }
    }
    else
    {
        debug_tls("ssl_client_new()\n");
        ssl = ssl_client_new(ssl_ctx, client_fd, NULL, 0, extensions);
    }

    /* check the return status */
    if ((res = ssl_handshake_status(ssl)) != SSL_OK)
    {
        if (!quiet)
        {
            ssl_display_error(res);
        }
        debug_tls("Handshake error: %d\n",res);
        ssl_free(ssl);
        ssl_ctx_free(ssl_ctx);
        _client.stop();
        _connected = false;
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
    debug_tls("connect():%d\n",res);
    return res;
}

int axTLSClient::connected()
{
  return _connected;
}

/**
 * Server constructor
 */
axTLSServer::axTLSServer()
{
  debug_tls("axTLSServer()\n");
}


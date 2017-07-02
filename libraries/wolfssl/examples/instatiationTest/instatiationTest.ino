// This will only exercise the instatiation of the library.
// It will not make a connection to a remote server.
// See: wolfssl/wolfcrypt/settings.h for additional library
// settings to turn on and off as #define statements.

#include <wolfssl/ssl.h>

/*
 * This creates a ssl session context for TLS/SSL.
 */
WOLFSSL_CTX* ctx = 0;
WOLFSSL* ssl = 0;
WOLFSSL_METHOD* method = 0;

TCPClient client;

/* These are callback functions to enable the library to
 * carry out read/write operations with the Particle device.
 */
int EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx);
int EthernetReceive(WOLFSSL* ssl, char* reply, int sz, void* ctx);

// Callback function for sending data to the remote server
int EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx) {
  int sent = 0;

  sent = client.write((byte*)msg, sz);

  return sent;
}

// Callback function for reading data from the remote server
int EthernetReceive(WOLFSSL* ssl, char* reply, int sz, void* ctx) {
  int ret = 0;

  while (client.available() > 0 && ret < sz) {
    reply[ret++] = client.read();
  }

  return ret;
}

void setup() {

  Serial.begin(9600);

  method = wolfTLSv1_2_client_method();
  if (method == NULL) {
    Serial.println("unable to get method");
    return;
  }
  ctx = wolfSSL_CTX_new(method);
  if (ctx == NULL) {
    Serial.println("unable to get ctx");
    return;
  }
  // initialize wolfSSL using callback functions
  wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);
  wolfSSL_SetIOSend(ctx, EthernetSend);
  wolfSSL_SetIORecv(ctx, EthernetReceive);

  return;
}

void loop() {
  WOLFSSL_CIPHER* cipher;
  delay(1000);
}

/* wolfapi.cpp
 *
 * Copyright (C) 2017 Rob Cermak
 * License: Attribution-ShareAlike 4.0 International
 * Requires:
 *   Library: wolfssl@0.0.2 or higher
 */

#include <Private.h>
#include <wolfssl/ssl.h>

/* Define connection information
 *
 * host[] : hostname or IP address
 * port   : typically 443 or may be some other unique port with HTTPS
 */
//const char host[] = "192.168.1.148"; // server to connect to
const char host[] = DEFAULT_HOST; // server to connect to
int port = 443; // port on server to connect to

/* This is a variable for sending to the remote server
 *
 * msg : message buffer itself
 * msgSz : finished size of the buffer
 */
char msg[400]      = { 0 };
int msgSz          = 0;
int certsOK        = 1;

/* This determines the number of time to attempt to make
 * a connection to the remote server
 */
int reconnect = 10;

/* These are callback functions to enable the library to
 * carry out read/write operations with the Particle device.
 */
int EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx);
int EthernetReceive(WOLFSSL* ssl, char* reply, int sz, void* ctx);

TCPClient client;

/*
 * This creates a ssl session context for TLS/SSL.
 */

WOLFSSL_CTX* ctx = 0;
WOLFSSL* ssl = 0;
WOLFSSL_METHOD* method = 0;

void setup() {
  int ret = 0;
  Serial.begin(9600);

  // wait for sync with Particle time for up to 10 seconds
  waitFor(Particle.syncTimeDone, 10000);
  Serial.println("Synced with time server.");

#if defined(DEBUG_WOLFSSL)
  wolfSSL_Debugging_ON();
#endif

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

  // Add certificates for AWS IoT here
  ret = wolfSSL_CTX_load_verify_buffer(ctx, CAcert,\
    strlen((const char*)CAcert), SSL_FILETYPE_PEM);
  if (ret != SSL_SUCCESS) {
    Serial.print("Return code: ");
    Serial.print(ret);
    Serial.printlnf(" (%s)",wolfSSL_ERR_reason_error_string(ret));
    Serial.println("Failed to load CAcert.");
    certsOK = 0;
  }

  // Perform a validation step
  wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

  // Enable UseSNI
  ret = wolfSSL_CTX_UseSNI(ctx, WOLFSSL_SNI_HOST_NAME, (unsigned char*)host, (unsigned short)strlen((const char*)host));
  if (ret != 1) {
    Serial.println("WARNING: failed to configure server name indication (SNI) TLS extensions.");
  } else {
    Serial.println("SNI is set.");
  }

  // initialize wolfSSL using callback functions
  wolfSSL_SetIOSend(ctx, EthernetSend);
  wolfSSL_SetIORecv(ctx, EthernetReceive);

  return;
}

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

void loop() {
  int err            = 0;
  int input          = 0;
  int sent           = 0;
  int total_input    = 0;
  char errBuf[80];
  char reply[80];
  WOLFSSL_CIPHER* cipher;
  char protocols[100];
  char *protocol = NULL;
  unsigned short protocol_len = 0;

  // Protocol formats: h2,http/1.1
  *protocols = '\0';
  //strcpy(protocols, "h2,http/1.1");
  // h2 seems to broken at the moment
  strcpy(protocols, "http/1.1");

  if (reconnect) {
    reconnect--;
    if (client.connect(host, port)) {

      Serial.print("Connected to ");
      Serial.println(host);
      ssl = wolfSSL_new(ctx);
      if (ssl == NULL) {
        err = wolfSSL_get_error(ssl, 0);
        wolfSSL_ERR_error_string(err, errBuf);
        Serial.print("Unable to get SSL object. Error = ");
        Serial.println(errBuf);
      }
      
      Serial.print("SSL version is ");
      Serial.println(wolfSSL_get_version(ssl));

      #if defined(HAVE_ALPN)
      // Setup ALPN if we are using it
      err = wolfSSL_UseALPN(ssl, protocols, (unsigned)strlen(protocols),\
        WOLFSSL_ALPN_CONTINUE_ON_MISMATCH);
      if (err == SSL_SUCCESS) {
        Serial.println("ALPN settings were accepted.");
      } else {
        Serial.printlnf("wolfSSL_UseALPN(%d)", err);
      }
      #endif

      // We have to write the header
      // Test #1 GET
      memset(msg, 0, sizeof(msg));
      sprintf(msg, "GET /prod/testParticle HTTP/1.1\r\n");
      msgSz = sprintf(msg, "%sHost: %s\r\nUser-Agent: %s/%s\r\n", 
        msg, host, "wolfssl", "3.11.2");
      msgSz = sprintf(msg, "%sAccept: */*\r\n", msg);
      msgSz = sprintf(msg, "%sx-api-key: %s\r\n\r\n", msg, AWS_API_KEY);
      Serial.print("Header size: ");
      Serial.println(msgSz);
      sent = wolfSSL_write(ssl, msg, strlen(msg));
      Serial.print("Bytes written: ");
      Serial.println(sent);

      // If you have body(data) to send, do that here.
      /*
      memset(msg, 0, sizeof(msg));
      msgSz = sprintf(msg, "%s", "{ \"serialNumber\": \"G030JF053216F1BS\", \"clickType\": \"SINGLE\", \"batteryVoltage\": \"2000mV\" }");
      Serial.print("Data payload size: ");
      Serial.println(msgSz);
      nret = wolfSSL_write(ssl, msg, strlen(msg));
      Serial.print("Bytes written: ");
      Serial.println(nret);
      */
      
      // If we got the expected response from the writer, see if we get
      // a response from the server.
      if (sent == msgSz) {
        delay(2000);

        cipher = wolfSSL_get_current_cipher(ssl);
        Serial.print("SSL cipher suite: ");
        Serial.println(wolfSSL_CIPHER_get_name(cipher));

        Serial.print("client.available():");
        Serial.println(client.available());
        Serial.println("Server response:");
        while (client.available() || wolfSSL_pending(ssl)) {
          input = wolfSSL_read(ssl, reply, sizeof(reply) - 1);
          total_input += input;
          if ( input > 0 ) {
            reply[input] = '\0';
            Serial.print(reply);
          } else if (input < 0) {
            err = wolfSSL_get_error(ssl, 0);
            wolfSSL_ERR_error_string(err, errBuf);
            Serial.print("wolfSSL_read failed. Error: ");
            Serial.println(errBuf);
          } else {
            Serial.println();
          }
        } 
        Serial.println();
      } else {
        Serial.println("SSL_write failed");
      }
      
      if (ssl != NULL) 
        wolfSSL_free(ssl);

      client.stop();
      Serial.println("Connection complete.");
      reconnect = 0;
    } else {
      Serial.println("Trying to reconnect...");       
    }
  }
  delay(1000);
}

/* wolfssl_client.ino
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */


#include <wolfssl.h>
#include <wolfssl/ssl.h>
//#include <Ethernet.h>

//const char host[] = "192.168.1.148"; // server to connect to
const char host[] = "jupyter.lccllc.info"; // server to connect to
int port = 4443; // port on server to connect to

int EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx);
int EthernetReceive(WOLFSSL* ssl, char* reply, int sz, void* ctx);
int reconnect = 10;

char msg[400]      = { 0 };
int msgSz          = 0;

//EthernetClient client;
TCPClient client;

WOLFSSL_CTX* ctx = 0;
WOLFSSL* ssl = 0;
WOLFSSL_METHOD* method = 0;

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

  // Create test message
  sprintf(msg, "GET / HTTP/1.1\r\n");
  msgSz = sprintf(msg, "%sHost: jupyter.lccllc.info\r\nUser-Agent: %s/%s\r\n\r\n", 
    msg, "axTLS", "2.3.1a");
  
  return;
}

int EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx) {
  int sent = 0;

  sent = client.write((byte*)msg, sz);

  return sent;
}

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
      
      if ((wolfSSL_write(ssl, msg, strlen(msg))) == msgSz) {
      cipher = wolfSSL_get_current_cipher(ssl);
      Serial.print("SSL cipher suite is ");
      Serial.println(wolfSSL_CIPHER_get_name(cipher));                
        delay(1000);
        Serial.print("client.available():");
        Serial.println(client.available());
        Serial.print("Server response: ");
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


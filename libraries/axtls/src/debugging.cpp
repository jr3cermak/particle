/*
 * Copyright (c) 2007-2016, Cameron Rich
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 * * Neither the name of the axTLS project nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Demonstrate the use of the axTLS library in C with a set of 
 * command-line parameters similar to openssl. In fact, openssl clients 
 * should be able to communicate with axTLS servers and visa-versa.
 *
 * This code has various bits enabled depending on the configuration. To enable
 * the most interesting version, compile with the 'full mode' enabled.
 *
 * To see what options you have, run the following:
 * > axssl s_server -?
 * > axssl s_client -?
 *
 * The axtls shared library must be in the same directory or be found 
 * by the OS.
 */

/**
 * Particle notes: this is a carry over from the command line version
 * and retained to show the original capabilities that may be available
 * in the future and allow the library to compile.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "os_port.h"
#include "ssl.h"
#include "debugging.h"

/**
 * We've had some sort of command-line error. Print out the basic options.
 */
void print_options(char *option)
{
   //printf("axssl: Error: '%s' is an invalid command.\n", option);
   //printf("usage: axssl [s_server|s_client|version] [args ...]\n");
    exit(1);
}

/**
 * We've had some sort of command-line error. Print out the server options.
 */
void print_server_options(char *option)
{
#ifndef CONFIG_SSL_SKELETON_MODE
    int cert_size = ssl_get_config(SSL_MAX_CERT_CFG_OFFSET);
#endif
#ifdef CONFIG_SSL_CERT_VERIFICATION
    int ca_cert_size = ssl_get_config(SSL_MAX_CA_CERT_CFG_OFFSET);
#endif

   //printf("unknown option %s\n", option);
   //printf("usage: s_server [args ...]\n");
   //printf(" -accept arg\t- port to accept on (default is 4433)\n");
#ifndef CONFIG_SSL_SKELETON_MODE
   //printf(" -cert arg\t- certificate file to add (in addition to default) to chain -\n\t\t  Can repeat up to %d times\n", cert_size);
   //printf(" -key arg\t- Private key file to use\n");
   //printf(" -pass\t\t- private key file pass phrase source\n");
#endif
   //printf(" -quiet\t\t- No server output\n");
#ifdef CONFIG_SSL_CERT_VERIFICATION
   //printf(" -verify\t- turn on peer certificate verification\n");
   //printf(" -CAfile arg\t- Certificate authority\n");
   //printf("\t\t  Can repeat up to %d times\n", ca_cert_size);
#endif
#ifdef CONFIG_SSL_FULL_MODE
   //printf(" -debug\t\t- Print more output\n");
   //printf(" -state\t\t- Show state messages\n");
   //printf(" -show-rsa\t- Show RSA state\n");
#endif
    exit(1);
}

/**
 * We've had some sort of command-line error. Print out the client options.
 */
void print_client_options(char *option)
{
#ifdef CONFIG_SSL_ENABLE_CLIENT
    int cert_size = ssl_get_config(SSL_MAX_CERT_CFG_OFFSET);
    int ca_cert_size = ssl_get_config(SSL_MAX_CA_CERT_CFG_OFFSET);
#endif

   //printf("unknown option %s\n", option);
#ifdef CONFIG_SSL_ENABLE_CLIENT
   //printf("usage: s_client [args ...]\n");
   //printf(" -connect host:port - who to connect to (default is localhost:4433)\n");
   //printf(" -verify\t- turn on peer certificate verification\n");
   //printf(" -cert arg\t- certificate file to use\n");
   //printf("\t\t  Can repeat up to %d times\n", cert_size);
   //printf(" -key arg\t- Private key file to use\n");
   //printf(" -CAfile arg\t- Certificate authority\n");
   //printf("\t\t  Can repeat up to %d times\n", ca_cert_size);
   //printf(" -quiet\t\t- No client output\n");
   //printf(" -reconnect\t- Drop and re-make the connection with the same Session-ID\n");
   //printf(" -pass\t\t- Private key file pass phrase source\n");
   //printf(" -servername\t- Set TLS extension servername in ClientHello\n");
#ifdef CONFIG_SSL_FULL_MODE
   //printf(" -debug\t\t- Print more output\n");
   //printf(" -state\t\t- Show state messages\n");
   //printf(" -show-rsa\t- Show RSA state\n");
#endif
#else
   //printf("Change configuration to allow this feature\n");
#endif
    exit(1);
}

/**
 * Display what cipher we are using 
 */
void display_cipher(SSL *ssl)
{
   //printf("CIPHER is ");
    switch (ssl_get_cipher_id(ssl))
    {
        case SSL_AES128_SHA:
           //printf("AES128-SHA");
            break;

        case SSL_AES256_SHA:
           //printf("AES256-SHA");
            break;

        case SSL_AES128_SHA256:
           //printf("AES128-SHA256");
            break;

        case SSL_AES256_SHA256:
           //printf("AES256-SHA256");
            break;

        default:
           //printf("Unknown - %d", ssl_get_cipher_id(ssl));
            break;
    }

   //printf("\n");
    TTY_FLUSH();
}

/**
 * Display what session id we have.
 */
void display_session_id(SSL *ssl)
{    
    int i;
    const uint8_t *session_id = ssl_get_session_id(ssl);
    int sess_id_size = ssl_get_session_id_size(ssl);

    if (sess_id_size > 0)
    {
       //printf("-----BEGIN SSL SESSION PARAMETERS-----\n");
        for (i = 0; i < sess_id_size; i++)
        {
           //printf("%02x", session_id[i]);
        }

       //printf("\n-----END SSL SESSION PARAMETERS-----\n");
        TTY_FLUSH();
    }
}

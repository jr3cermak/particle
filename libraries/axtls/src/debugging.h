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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* define standard input */
#ifndef STDIN_FILENO
#define STDIN_FILENO        0
#endif

#ifdef __cplusplus
extern "C" {
#endif

static void print_options(char *option);
static void print_server_options(char *option);
static void print_client_options(char *option);
static void display_cipher(SSL *ssl);
static void display_session_id(SSL *ssl);

#ifdef __cplusplus
}
#endif

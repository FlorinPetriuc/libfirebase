/*
 * Copyright (C) 2016 Florin Petriuc. All rights reserved.
 * Initial release: Florin Petriuc <petriuc.florin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef _ENV_H_
#define _ENV_H_

enum log_type_t
{
    LOG_TYPE_INFO,
    LOG_TYPE_ERROR,
    LOG_TYPE_FATAL
};

#define ENV_STRLEN_MACRO(x) (sizeof(x) - 1)

#include "./linux/env_linux.h"

int env_ssl_init();

int env_logInit(const uint8_t enabled, const char *file);
void env_logWrite(enum log_type_t type, const char *template, const unsigned int n, ...);

void *env_malloc(const unsigned long int size);
void env_sleep(const unsigned int seconds, const unsigned int mseconds);
env_time_t env_utcTime();

unsigned char *env_base64WebDecode(char *bufcoded, unsigned int *len);
char *env_base64WebEncode(unsigned char *bufin, const int len);

int env_starts_with(const char *haystack, const char *needle);

env_socket_t env_connect_to_url(const char *url, const unsigned short int port, const unsigned int timeout_ms);
void env_close_socket(env_socket_t sockFD);

env_secure_socket_t env_secure_server_connection(env_socket_t server_socket);
void env_close_secure_connection(env_secure_socket_t ssl);

int env_read_secure_data(env_secure_socket_t ssl, char *data, const int max_len);
int env_send_secure_data(env_secure_socket_t ssl, const char *data);
int env_send_n_secure_data(env_secure_socket_t ssl, const char *data, const unsigned int data_len);

int env_prime256v1_aesgcm_encrypt_message(const unsigned char *userKey, const unsigned int userKeyLen,
                                    const unsigned char *userSecret, const unsigned int userSecretLen,
                                    const unsigned char *data, const unsigned int data_len,
                                    unsigned char **data_out, unsigned int *data_out_len,
                                    char **dh_out, char **salt_out);

unsigned char *env_generate_hkdf(const unsigned char *salt, const unsigned int salt_len,
                                 const unsigned char *ikm, const unsigned int ikm_len,
                                 const unsigned char *info, const unsigned int info_len);

void env_create_info_chunk(const char *type, const unsigned char *rpubkey, const uint16_t rpubkey_len,
                                        const unsigned char *spubkey, const uint16_t spubkey_len,
                                        unsigned char **info, unsigned int *info_len);

int env_aes_gcm_128_encrypt(const unsigned char *plaintext, const unsigned int plaintext_len,
                            const unsigned char *key, const unsigned char *iv,
                            unsigned char **ciphertext, unsigned int *ciphertext_len);

int env_send_tcp_data(env_socket_t sockFD, const char *data);
int env_send_n_tcp_data(env_socket_t sockFD, const char *data, const unsigned int data_len);
int env_read_tcp_data(env_socket_t sockFD, char *data, const int max_len);

#endif
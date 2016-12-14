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

#include "../env.h"

static int get_public_key_from_curve(EC_KEY *curve, unsigned char **pubkey, unsigned int *pubkey_len)
{
    const EC_POINT *point;
    const EC_GROUP *group;

    BIGNUM *bn;

    point = EC_KEY_get0_public_key(curve);
    if(point == NULL)
    {
        return 1;
    }

    group = EC_KEY_get0_group(curve);
    if(group == NULL)
    {
        return 1;
    }

    bn = EC_POINT_point2bn(group, point, 4, NULL, NULL);
    if(bn == NULL)
    {
        return 1;
    }

    *pubkey_len = BN_num_bytes(bn);
    *pubkey = env_malloc(*pubkey_len);

    BN_bn2bin(bn, *pubkey);

    BN_free(bn);

    return 0;
}

static EC_KEY *generate_prime256v1_curve()
{
    EC_KEY *ret = NULL;

    ret = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if(ret == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_new_by_curve_name returns NULL for ret", 0);

        return NULL;
    }

    if(EC_KEY_generate_key(ret) == 0)
    {
        EC_KEY_free(ret);

        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_generate_key returns 0", 0);

        return NULL;
    }

    if(EC_KEY_check_key(ret) == 0)
    {
        EC_KEY_free(ret);

        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_check_key returns 0", 0);

        return NULL;
    }

    return ret;
}

static EC_KEY *prime256v1_compute_user_key(const unsigned char *userKey, const unsigned int userKeyLen)
{
    EC_KEY *pkey = NULL;

    BIGNUM *pub_key_x;
    BIGNUM *pub_key_y;

    EC_POINT *pub_key;

    const EC_GROUP *group;

    const unsigned char *k;

    if(userKey[0] != 4)
    {
        env_logWrite(LOG_TYPE_ERROR, "userKey[0] != 4", 0);

        return NULL;
    }

    pkey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if(pkey == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_new_by_curve_name returns NULL for pkey", 0);

        return NULL;
    }

    k = userKey + 1;

    pub_key_x = BN_bin2bn(k, 32, NULL);
    if(pub_key_x == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "BN_bin2bn returns NULL for pub_key_x", 0);

        EC_KEY_free(pkey);

        return NULL;
    }

    pub_key_y = BN_bin2bn(k + 32, 32, NULL);
    if(pub_key_y == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "BN_bin2bn returns NULL for pub_key_y", 0);

        BN_free(pub_key_x);
        EC_KEY_free(pkey);

        return NULL;
    }

    group = EC_KEY_get0_group(pkey);

    pub_key = EC_POINT_new(group);

    if(EC_POINT_set_affine_coordinates_GFp(group, pub_key, pub_key_x, pub_key_y, NULL) == 0)
    {
        env_logWrite(LOG_TYPE_ERROR, "EC_POINT_set_affine_coordinates_GFp returns 0", 0);

        EC_POINT_free(pub_key);
        BN_free(pub_key_y);
        BN_free(pub_key_x);
        EC_KEY_free(pkey);

        return NULL;
    }

    if(EC_KEY_set_public_key(pkey, pub_key) == 0)
    {
        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_set_public_key returns 0", 0);

        EC_POINT_free(pub_key);
        BN_free(pub_key_y);
        BN_free(pub_key_x);
        EC_KEY_free(pkey);

        return NULL;
    }

    if(EC_KEY_check_key(pkey) == 0)
    {
        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_check_key returns 0", 0);

        EC_POINT_free(pub_key);
        BN_free(pub_key_y);
        BN_free(pub_key_x);
        EC_KEY_free(pkey);

        return NULL;
    }

    EC_POINT_free(pub_key);
    BN_free(pub_key_y);
    BN_free(pub_key_x);

    return pkey;
}

static int prime256v1_compute_secret(EC_KEY *curve, const unsigned char *userKey, const unsigned int userKeyLen,
                                    unsigned char **secret, unsigned int *secret_len)
{
    const EC_GROUP *group;
    const EC_POINT *userPoint;

    EC_KEY *peerkey;

    int field_size;

    unsigned char *_secret;
    unsigned int _secret_len;

    group = EC_KEY_get0_group(curve);
    if(group == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_get0_group returns NULL", 0);

        return 1;
    }

    field_size = EC_GROUP_get_degree(group);
    _secret_len = (field_size + 7) >> 3;

    peerkey = prime256v1_compute_user_key(userKey, userKeyLen);

    if(peerkey == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "prime256v1_compute_user_key returns NULL", 0);

        return 1;
    }

    userPoint = EC_KEY_get0_public_key(peerkey);

    if(userPoint == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "EC_KEY_get0_public_key returns NULL", 0);

        EC_KEY_free(peerkey);

        return 1;
    }

    _secret = env_malloc(_secret_len);

    _secret_len = ECDH_compute_key(_secret, _secret_len, userPoint, curve, NULL);

    EC_KEY_free(peerkey);

    *secret_len = _secret_len;
    *secret = _secret;

    return 0;
}

static void hmac_sha256(const unsigned char *key, const unsigned int key_len, const unsigned char *data, const unsigned int data_len, unsigned char *out)
{
    unsigned int len = 32;

    HMAC_CTX hmac;
    HMAC_CTX_init(&hmac);
    HMAC_Init_ex(&hmac, key, key_len, EVP_sha256(), NULL);
    HMAC_Update(&hmac, data, data_len);
    HMAC_Final(&hmac, out, &len);
    HMAC_CTX_cleanup(&hmac);
}

unsigned char *env_generate_hkdf(const unsigned char *salt, const unsigned int salt_len,
                                 const unsigned char *ikm, const unsigned int ikm_len,
                                 const unsigned char *info, const unsigned int info_len)
{
    unsigned char k_init[32];
    unsigned char *ret;
    unsigned char append;

    unsigned int _len;

    HMAC_CTX hkdf;

    hmac_sha256(salt, salt_len, ikm, ikm_len, k_init);

    HMAC_CTX_init(&hkdf);
    HMAC_Init_ex(&hkdf, k_init, 32, EVP_sha256(), NULL);
    HMAC_Update(&hkdf, info, info_len);

    append = 1;
    HMAC_Update(&hkdf, &append, 1);

    ret = env_malloc(32);

    HMAC_Final(&hkdf, ret, &_len);
    HMAC_CTX_cleanup(&hkdf);

    return ret;
}


int env_aes_gcm_128_encrypt(const unsigned char *plaintext, const unsigned int plaintext_len,
                            const unsigned char *key, const unsigned char *iv,
                            unsigned char **ciphertext, unsigned int *ciphertext_len)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    unsigned int _ciphertext_len;

    unsigned char *_ciphertext;
    unsigned char tag[16];

    ctx = EVP_CIPHER_CTX_new();
	if(ctx == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "EVP_CIPHER_CTX_new returned NULL", 0);

        return 1;
    }

    if(EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);

        env_logWrite(LOG_TYPE_ERROR, "EVP_EncryptInit_ex1 returned != 1", 0);

        return 1;
    }

    if(EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);

        env_logWrite(LOG_TYPE_ERROR, "EVP_EncryptInit_ex2 returned != 1", 0);

        return 1;
    }

    _ciphertext = env_malloc(plaintext_len + 16);

    if(EVP_EncryptUpdate(ctx, _ciphertext, &len, plaintext, plaintext_len) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);

        free(_ciphertext);

        env_logWrite(LOG_TYPE_ERROR, "EVP_EncryptUpdate2 returned != 1", 0);

        return 1;
    }

	_ciphertext_len = len;

	if(EVP_EncryptFinal_ex(ctx, _ciphertext + len, &len) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);

        free(_ciphertext);

        env_logWrite(LOG_TYPE_ERROR, "EVP_EncryptFinal_ex returned != 1", 0);

        return 1;
    }

    _ciphertext_len += len;

    if(EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);

        free(_ciphertext);

        env_logWrite(LOG_TYPE_ERROR, "EVP_CIPHER_CTX_ctrl returned != 1", 0);

        return 1;
    }

    EVP_CIPHER_CTX_free(ctx);

    *ciphertext_len = _ciphertext_len + 16;
    (*ciphertext) = _ciphertext;
    memcpy(*ciphertext + _ciphertext_len, tag, 16);

    return 0;
}

int env_prime256v1_aesgcm_encrypt_message(const unsigned char *userKey, const unsigned int userKeyLen,
                                    const unsigned char *userSecret, const unsigned int userSecretLen,
                                    const unsigned char *data, const unsigned int data_len,
                                    unsigned char **data_out, unsigned int *data_out_len,
                                    char **dh_out, char **salt_out)
{
    int i;

    unsigned char salt[16];
    unsigned char *IKM;
    unsigned char *PRK;
    unsigned char *CEK;
    unsigned char *PublicKey;
    unsigned char *Nonce;
    unsigned char *info_chunk;
    unsigned char *data_in;
    unsigned char *ciphertext;

    unsigned int ciphertext_len;
    unsigned int ikm_len;
    unsigned int public_key_len;
    unsigned int info_chunk_len;

    EC_KEY *curve;

    for(i = 0; i < 16; ++i)
    {
        salt[i] = rand();
    }

    curve = generate_prime256v1_curve();

    if(curve == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "Prime256v1 curve is null", 0);

        return 1;
    }

    if(prime256v1_compute_secret(curve, userKey, userKeyLen, &IKM, &ikm_len))
    {
        EC_KEY_free(curve);

        env_logWrite(LOG_TYPE_ERROR, "Compute IKM returns error", 0);

        return 1;
    }

    PRK = env_generate_hkdf(userSecret, userSecretLen, IKM, ikm_len,
                            (const unsigned char *)"Content-Encoding: auth", sizeof("Content-Encoding: auth"));

    if(get_public_key_from_curve(curve, &PublicKey, &public_key_len))
    {
        free(PRK);
        free(IKM);

        EC_KEY_free(curve);

        return 1;
    }

    env_create_info_chunk("aesgcm", userKey, userKeyLen, PublicKey,
                        public_key_len, &info_chunk, &info_chunk_len);
    CEK = env_generate_hkdf(salt, 16, PRK, 32, info_chunk, info_chunk_len);
    free(info_chunk);

    env_create_info_chunk("nonce", userKey, userKeyLen, PublicKey,
                        public_key_len, &info_chunk, &info_chunk_len);
    Nonce = env_generate_hkdf(salt, 16, PRK, 32, info_chunk, info_chunk_len);
    free(info_chunk);

    data_in = env_malloc(data_len + 2);
    data_in[0] = 0;
    data_in[1] = 0;
    memcpy(data_in + 2, data, data_len);

    if(env_aes_gcm_128_encrypt(data_in, data_len + 2, CEK, Nonce,
                &ciphertext, &ciphertext_len))
    {
        free(CEK);
        free(Nonce);
        free(data_in);
        free(PublicKey);
        free(PRK);
        free(IKM);

        EC_KEY_free(curve);

        return 1;
    }

    (*data_out) = ciphertext;
    (*data_out_len) = ciphertext_len;
    (*dh_out) = env_base64WebEncode(PublicKey, public_key_len);
    (*salt_out) = env_base64WebEncode(salt, 16);

    free(CEK);
    free(Nonce);
    free(data_in);
    free(PublicKey);
    free(PRK);
    free(IKM);

    EC_KEY_free(curve);

    return 0;
}
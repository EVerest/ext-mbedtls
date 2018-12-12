/**
 * \file camellia.h
 *
 * \brief Camellia block cipher
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_CAMELLIA_H
#define MBEDTLS_CAMELLIA_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <stddef.h>
#include <stdint.h>

#include "platform_util.h"

#define MBEDTLS_CAMELLIA_ENCRYPT     1
#define MBEDTLS_CAMELLIA_DECRYPT     0

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
#define MBEDTLS_ERR_CAMELLIA_INVALID_KEY_LENGTH   MBEDTLS_DEPRECATED_NUMERIC_CONSTANT( -0x0024 )
#endif /* !MBEDTLS_DEPRECATED_REMOVED */
#define MBEDTLS_ERR_CAMELLIA_BAD_INPUT_DATA -0x0024 /**< Bad input data. */

#define MBEDTLS_ERR_CAMELLIA_INVALID_INPUT_LENGTH -0x0026 /**< Invalid data input length. */

/** TEMPORARY -- THIS IS IN CONFLICT WITH EXISTING ERROR CODES AND NEEDS CHANGE. */
#define MBEDTLS_ERR_CAMELLIA_BAD_INPUT_DATA       -0x0024  /**< Invalid data input length. */

/* MBEDTLS_ERR_CAMELLIA_HW_ACCEL_FAILED is deprecated and should not be used.
 */
#define MBEDTLS_ERR_CAMELLIA_HW_ACCEL_FAILED              -0x0027  /**< Camellia hardware accelerator failed. */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(MBEDTLS_CAMELLIA_ALT)
// Regular implementation
//

/**
 * \brief          CAMELLIA context structure
 */
typedef struct mbedtls_camellia_context
{
    int nr;                     /*!<  number of rounds  */
    uint32_t rk[68];            /*!<  CAMELLIA round keys    */
}
mbedtls_camellia_context;

#else  /* MBEDTLS_CAMELLIA_ALT */
#include "camellia_alt.h"
#endif /* MBEDTLS_CAMELLIA_ALT */

/**
 * \brief          Initialize a CAMELLIA context.
 *
 * \param ctx      The CAMELLIA context to be initialized.
 *                 This must not be \c NULL.
 */
void mbedtls_camellia_init( mbedtls_camellia_context *ctx );

/**
 * \brief          Clear a CAMELLIA context.
 *
 * \param ctx      The CAMELLIA context to be cleared. May be \c NULL,
 *                 in which case this function is a no-op. If it is not
 *                 \c NULL, it must be initialized.
 */
void mbedtls_camellia_free( mbedtls_camellia_context *ctx );

/**
 * \brief          Perform a CAMELLIA key schedule (encryption).
 *
 * \param ctx      The CAMELLIA context to use. This must be initialized.
 * \param key      The encryption key to use. Must be a readable buffer
 *                 of size \p keybits bits.
 * \param keybits  The length of \p key in Bits. Must be either \c 128,
 *                 \c 192 or \c 256.
 *
 * \return         \c 0 if successful.
 * \return         A negative error code on failure.
 */
int mbedtls_camellia_setkey_enc( mbedtls_camellia_context *ctx,
                                 const unsigned char *key,
                                 unsigned int keybits );

/**
 * \brief          Perform a CAMELLIA key schedule (decryption).
 *
 * \param ctx      The CAMELLIA context to use. This must be initialized.
 * \param key      The decryption key. Must be a readable buffer
 *                 of size \p keybits bits.
 * \param keybits  The length of \p key in Bits. Must be either \c 128,
 *                 \c 192 or \c 256.
 *
 * \return         \c 0 if successful.
 * \return         A negative error code on failure.
 */
int mbedtls_camellia_setkey_dec( mbedtls_camellia_context *ctx,
                                 const unsigned char *key,
                                 unsigned int keybits );

/**
 * \brief          Perform a CAMELLIA-ECB block encryption/decryption.
 *
 * \param ctx      The CAMELLIA context to use. Must be initialized
 *                 and bound to a key.
 * \param mode     The mode of operation. Must be either
 *                 #MBEDTLS_CAMELLIA_ENCRYPT or #MBEDTLS_CAMELLIA_DECRYPT.
 * \param input    The input block. Must be a readable buffer
 *                 of size \c 16 Bytes.
 * \param output   The output block. Must be a writable buffer
 *                 of size \c 16 Bytes.
 *
 * \return         \c 0 if successful.
 * \return         A negative error code on failure.
 */
int mbedtls_camellia_crypt_ecb( mbedtls_camellia_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] );

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/**
 * \brief          Perform a CAMELLIA-CBC buffer encryption/decryption.
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      The CAMELLIA context to use. Must be initialized
 *                 and bound to a key.
 * \param mode     The mode of operation. Possible values are
 *                 #MBEDTLS_CAMELLIA_ENCRYPT or #MBEDTLS_CAMELLIA_DECRYPT.
 * \param length   The length in Bytes of the input data.
 *                 Must be a multiple of \c 16.
 * \param iv       The initialization vector. This must be RW buffer
 *                 of length \c 16 Bytes. It is updated to allow streaming
 *                 use as explained above.
 * \param input    The buffer holding the input data. Must point to a readable
 *                 buffer of length \p length Bytes. May be \c NULL if
 *                 `length == 0`.
 * \param input    The buffer holding the output data. Must point to a writable
 *                 buffer of length \p length Bytes. May be \c NULL if
 *                 `length == 0`.
 *
 * \return         \c 0 if successful.
 * \return         A negative error code on failure.
 */
int mbedtls_camellia_crypt_cbc( mbedtls_camellia_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output );
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/**
 * \brief          Perform a CAMELLIA-CFB128 buffer encryption/decryption.
 *
 * \note           Due to the nature of CFB you should use the same key
 *                 schedule for both encryption and decryption. So a
 *                 context initialized with mbedtls_camellia_setkey_enc()
 *                 for both #MBEDTLS_CAMELLIA_ENCRYPT and
 *                 #MBEDTLS_CAMELLIA_DECRYPT.
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      The CAMELLIA context to use. Must be initialized
 *                 and bound to a key.
 * \param mode     The mode of operation. Possible values are
 *                 #MBEDTLS_CAMELLIA_ENCRYPT or #MBEDTLS_CAMELLIA_DECRYPT.
 * \param length   The length of the input data. Any value is allowed.
 * \param iv_off   The current offset in the IV. This must be smaller
 *                 than \c 16. It is updated after this call to allow
 *                 the aforementioned streaming usage.
 * \param iv       The initialization vector. Must be an RW buffer of
 *                 length \c 16 Bytes. It is updated after this call to
 *                 allow the aforementioned streaming usage.
 * \param input    The buffer holding the input data. Must be a readable
 *                 buffer of size \p length Bytes. May be \c NULL if
 *                 \p length is \c 0.
 * \param output   The buffer to hold the output data. Must be a writable
 *                 buffer of length \p length Bytes. May be \c NULL if
 *                 \p length is \c 0.
 *
 * \return         \c 0 if successful.
 * \return         A negative error code on failure.
 */
int mbedtls_camellia_crypt_cfb128( mbedtls_camellia_context *ctx,
                       int mode,
                       size_t length,
                       size_t *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output );
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/**
 * \brief      CAMELLIA-CTR buffer encryption/decryption
 *
 * \note       Due to the nature of CTR you should use the same key
 *             schedule for both encryption and decryption. So a
 *             context initialized with mbedtls_camellia_setkey_enc()
 *             for both #MBEDTLS_CAMELLIA_ENCRYPT and
 *             #MBEDTLS_CAMELLIA_DECRYPT.
 *
 * \warning    You must never reuse a nonce value with the same key. Doing so
 *             would void the encryption for the two messages encrypted with
 *             the same nonce and key.
 *
 *             There are two common strategies for managing nonces with CTR:
 *
 *             1. You can handle everything as a single message processed over
 *             successive calls to this function. In that case, you want to
 *             set \p nonce_counter and \p nc_off to 0 for the first call, and
 *             then preserve the values of \p nonce_counter, \p nc_off and \p
 *             stream_block across calls to this function as they will be
 *             updated by this function.
 *
 *             With this strategy, you must not encrypt more than 2**128
 *             blocks of data with the same key.
 *
 *             2. You can encrypt separate messages by dividing the \p
 *             nonce_counter buffer in two areas: the first one used for a
 *             per-message nonce, handled by yourself, and the second one
 *             updated by this function internally.
 *
 *             For example, you might reserve the first 12 bytes for the
 *             per-message nonce, and the last 4 bytes for internal use. In that
 *             case, before calling this function on a new message you need to
 *             set the first 12 bytes of \p nonce_counter to your chosen nonce
 *             value, the last 4 to 0, and \p nc_off to 0 (which will cause \p
 *             stream_block to be ignored). That way, you can encrypt at most
 *             2**96 messages of up to 2**32 blocks each with the same key.
 *
 *             The per-message nonce (or information sufficient to reconstruct
 *             it) needs to be communicated with the ciphertext and must be unique.
 *             The recommended way to ensure uniqueness is to use a message
 *             counter. An alternative is to generate random nonces, but this
 *             limits the number of messages that can be securely encrypted:
 *             for example, with 96-bit random nonces, you should not encrypt
 *             more than 2**32 messages with the same key.
 *
 *             Note that for both stategies, sizes are measured in blocks and
 *             that a CAMELLIA block is \c 16 bytes.
 *
 * \warning    Upon return, \p stream_block contains sensitive data. Its
 *             content must not be written to insecure storage and should be
 *             securely discarded as soon as it's no longer needed.
 *
 * \param ctx           The CAMELLIA context to use.
 * \param length        The length of the input data. Any value is allowed.
 * \param nc_off        The offset in the current \p stream_block (for resuming
 *                      within current cipher stream). The offset pointer to
 *                      should be \c 0 at the start of a stream. It is updated
 *                      at the end of this call.
 * \param nonce_counter The 128-bit nonce and counter. Must be an RW buffer of
 *                      length \c 16 Bytes.
 * \param stream_block  The saved stream-block for resuming. Must be an
 *                      RW buffer of length \c 16 Bytes.
 * \param input         The input data stream. Must be a readable buffer of
 *                      size \p length Bytes. This may be \c NULL if \p length
 *                      is \c 0.
 * \param output        The output data stream. Must be a writable buffer of
 *                      size \p length Bytes. This may be \c NULL if \p length
 *                      is \c 0.
 *
 * \return              \c 0 if successful.
 * \return              A negative error code on failure.
 */
int mbedtls_camellia_crypt_ctr( mbedtls_camellia_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output );
#endif /* MBEDTLS_CIPHER_MODE_CTR */

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int mbedtls_camellia_self_test( int verbose );

#ifdef __cplusplus
}
#endif

#endif /* camellia.h */

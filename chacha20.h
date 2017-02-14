#ifndef CHACHA20_H
#define CHACHA20_H

#define BLOCK_LENGTH 16
#define BYTE_LENGTH 64
#define NUMBER_ROUNDS 10

void chacha20_encrypt(uint8_t *key,
                      uint32_t counter,
                      uint8_t *nonce,
                      unsigned char *plaintext,
                      uint32_t plaintext_length,
                      unsigned char *ciphertext);

#endif

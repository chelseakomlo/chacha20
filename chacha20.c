#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "chacha20.h"
#include "utils.h"

const int ROUND_1[] = {0, 4, 8, 12};
const int ROUND_2[] = {1, 5, 9, 13};
const int ROUND_3[] = {2, 6, 10, 14};
const int ROUND_4[] = {3, 7, 11, 15};
const int ROUND_5[] = {0, 5, 10, 15};
const int ROUND_6[] = {1, 6, 11, 12};
const int ROUND_7[] = {2, 7, 8, 13};
const int ROUND_8[] = {3, 4, 9, 14};

const uint32_t STARTING_STATE[] = { 0x61707865,
                                    0x3320646e,
                                    0x79622d32,
                                    0x6b206574 };

void
apply_rules(uint32_t *_a, uint32_t *_b, uint32_t *_c, uint32_t *_d)
{
  uint32_t a = *_a; uint32_t b = *_b; uint32_t c = *_c; uint32_t d = *_d;

  a += b; d ^= a; d = rotate(d, 16);
  c += d; b ^= c; b = rotate(b, 12);
  a += b; d ^= a; d = rotate(d, 8);
  c += d; b ^= c; b = rotate(b, 7);

  *_a = a; *_b = b; *_c = c; *_d = d;
}

void
apply_qround(const int *qround, uint32_t *state)
{
  apply_rules(&state[qround[0]],
              &state[qround[1]],
              &state[qround[2]],
              &state[qround[3]]);
}

uint32_t
to_uint32(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
  return (d << 24) | (c << 16) | (b << 8) | a;
}

void
build_block(uint8_t *key,
                    uint32_t counter,
                    uint8_t *nonce,
                    uint32_t *state)
{
  state[0] = STARTING_STATE[0];
  state[1] = STARTING_STATE[1];
  state[2] = STARTING_STATE[2];
  state[3] = STARTING_STATE[3];

  // TODO: transform key to uint32s in a separate & more intuitive function
  state[4] = to_uint32(key[0], key[1], key[2], key[3]);
  state[5] = to_uint32(key[4], key[5], key[6], key[7]);
  state[6] = to_uint32(key[8], key[9], key[10], key[11]);
  state[7] = to_uint32(key[12], key[13], key[14], key[15]);
  state[8] = to_uint32(key[16], key[17], key[18], key[19]);
  state[9] = to_uint32(key[20], key[21], key[22], key[23]);
  state[10] = to_uint32(key[24], key[25], key[26], key[27]);
  state[11] = to_uint32(key[28], key[29], key[30], key[31]);

  state[12] = counter;

  state[13] = to_uint32(nonce[0], nonce[1], nonce[2], nonce[3]);
  state[14] = to_uint32(nonce[4], nonce[5], nonce[6], nonce[7]);
  state[15] = to_uint32(nonce[8], nonce[9], nonce[10], nonce[11]);
}

void
chacha20_block(uint32_t *state)
{
  uint32_t working_state[BLOCK_LENGTH];
  for (int i=0; i<BLOCK_LENGTH; i++){
    working_state[i] = state[i];
  }

  for (int i=0; i<NUMBER_ROUNDS; i++) {
    apply_qround(ROUND_1, working_state);
    apply_qround(ROUND_2, working_state);
    apply_qround(ROUND_3, working_state);
    apply_qround(ROUND_4, working_state);
    apply_qround(ROUND_5, working_state);
    apply_qround(ROUND_6, working_state);
    apply_qround(ROUND_7, working_state);
    apply_qround(ROUND_8, working_state);
  }

  for (int i=0; i<BLOCK_LENGTH; i++) {
    state[i] += working_state[i];
  }
}

void
chacha20_block_and_serialize(uint32_t *state, uint8_t *output)
{
  chacha20_block(state);
  serialize(state, output);
}

void
build_block_and_serialize(uint8_t *key,
                                  uint32_t counter,
                                  uint8_t *nonce,
                                  unsigned char *output)
{
  uint32_t *state = malloc(BLOCK_LENGTH*sizeof(uint32_t));

  if (!state)
    return;

  build_block(key, counter, nonce, state);
  chacha20_block(state);
  serialize(state, output);
  free(state);
}

void
chacha20_encrypt(uint8_t *key,
                 uint32_t starting_counter,
                 uint8_t *nonce,
                 unsigned char *plaintext,
                 uint32_t plaintext_length,
                 unsigned char *ciphertext)
{
  uint32_t counter = starting_counter;
  unsigned char *keystream = malloc(BYTE_LENGTH*sizeof(char));
  unsigned char *block = malloc(BYTE_LENGTH*sizeof(char));

  if (!keystream || !block)
    return;

  uint32_t i;
  for (i=0; i<(plaintext_length/(BYTE_LENGTH-1)); i+=BYTE_LENGTH) {
    build_block_and_serialize(key, counter, nonce, keystream);
    slice(plaintext, block, i, BYTE_LENGTH);
    xor_block(block, keystream, BYTE_LENGTH);
    memcpy(ciphertext+i, block, BYTE_LENGTH*sizeof(char));
    counter++;
  }

  int remaining = plaintext_length%64;
  build_block_and_serialize(key, counter, nonce, keystream);
  slice(plaintext, block, i, i+remaining);
  xor_block(block, keystream, remaining);
  memcpy(ciphertext+i, block, remaining*sizeof(char));

  free(keystream);
  free(block);
}


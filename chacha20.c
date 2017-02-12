#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define BLOCK_LENGTH 16
#define NUMBER_ROUNDS 10

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

uint32_t
rotate_32(uint32_t a, int i)
{
  return (a<<i) | (a>>(32-i));
}

void
apply_rules(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
  *a += *b; *d ^= *a; *d = rotate_32(*d, 16);
  *c += *d; *b ^= *c; *b = rotate_32(*b, 12);
  *a += *b; *d ^= *a; *d = rotate_32(*d, 8);
  *c += *d; *b ^= *c; *b = rotate_32(*b, 7);
}

void
apply_qround(const int qround[], uint32_t *state)
{
  uint32_t a = state[qround[0]];
  uint32_t b = state[qround[1]];
  uint32_t c = state[qround[2]];
  uint32_t d = state[qround[3]];

  apply_rules(&a, &b, &c, &d);

  state[qround[0]] = a;
  state[qround[1]] = b;
  state[qround[2]] = c;
  state[qround[3]] = d;
}

uint32_t
to_uint32(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
  return (d << 24) | (c << 16) | (b << 8) | a;
}

// TODO: check for valid inputs
void
build_initial_block(uint8_t *key,
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
chacha20_block(uint32_t *state) {
  uint32_t working_state[16];
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

  // TODO serialize
}

#include <stdlib.h>
#include <stdio.h>
#include "chacha20.c" // TODO use header file & expose private functions only for tests

int
is_equal(const uint32_t *expected, uint32_t *actual, int length)
{
  for (int i=0; i<length; i++) {
    if (expected[i] != actual[i]) {
      return 0;
    }
  }
  return 1;
}

int
test_build_initial_block(void)
{
  const uint32_t expected[] = { 0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
                                0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
                                0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c,
                                0x00000001, 0x09000000, 0x4a000000, 0x00000000
                              };

  uint8_t key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
                    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a,
                    0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

  uint32_t counter = 0x01;

  uint8_t nonce[] = { 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4a, 0x00,
                      0x00, 0x00, 0x00 };

  uint32_t *state = malloc(BLOCK_LENGTH*8);

  build_initial_block(key, counter, nonce, state);
  int is_eq = is_equal(expected, state, BLOCK_LENGTH);

  free(state);
  return is_eq;
}

int
test_apply_qround(void)
{
  uint32_t state[] = { 0x879531e0, 0xc5ecf37d, 0x516461b1, 0xc9a62f8a,
                       0x44c20ef3, 0x3390af7f, 0xd9fc690b, 0x2a5f714c,
                       0x53372767, 0xb00a5631, 0x974c541a, 0x359e9963,
                       0x5c971061, 0x3d631689, 0x2098d9d6, 0x91dbd320 };

  const uint32_t expected[] = { 0x879531e0, 0xc5ecf37d, 0xbdb886dc, 0xc9a62f8a,
                                0x44c20ef3, 0x3390af7f, 0xd9fc690b, 0xcfacafd2,
                                0xe46bea80, 0xb00a5631, 0x974c541a, 0x359e9963,
                                0x5c971061, 0xccc07c79, 0x2098d9d6, 0x91dbd320
                              };

  int sample_qround[] = {2, 7, 8, 13};

  apply_qround(sample_qround, state);
  return is_equal(expected, state, BLOCK_LENGTH);
}

int
test_apply_rules(void)
{
  uint32_t a = 0x11111111;
  uint32_t b = 0x01020304;
  uint32_t c = 0x9b8d6f43;
  uint32_t d = 0x01234567;

  uint32_t expected_a = 0xea2a92f4;
  uint32_t expected_b = 0xcb1cf8ce;
  uint32_t expected_c = 0x4581472e;
  uint32_t expected_d = 0x5881c4bb;

  apply_rules(&a, &b, &c, &d);

  if (a != expected_a) { return 0; } // TODO write helper assertions
  if (b != expected_b) { return 0; }
  if (c != expected_c) { return 0; }
  if (d != expected_d) { return 0; }
  return 1;
}

int
test_to_uint32(void)
{
  uint32_t expected = 0x09000000;
  uint32_t actual = to_uint32(0x00, 0x00, 0x00, 0x09);
  return expected == actual;
}

int
test_to_uint32_high_val(void)
{
  uint32_t expected = 0x1f1e1d1c;
  uint32_t actual = to_uint32(0x1c, 0x1d, 0x1e, 0x1f);
  return expected == actual;
}

int
test_block_function(void)
{
  const uint32_t expected[] = { 0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3,
                                0xc7f4d1c7, 0x0368c033, 0x9aaa2204, 0x4e6cd4c3,
                                0x466482d2, 0x09aa9f07, 0x05d7c214, 0xa2028bd9,
                                0xd19c12b5, 0xb94e16de, 0xe883d0cb, 0x4e3c50a2
                              };

  uint32_t state[] = { 0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
                       0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
                       0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c,
                       0x00000001, 0x09000000, 0x4a000000, 0x00000000 };

  uint32_t *actual = malloc(BLOCK_LENGTH*sizeof(char *));

  chacha20_block(state);
  int is_eq = is_equal(expected, state, BLOCK_LENGTH);

  free(actual);
  return is_eq;
}

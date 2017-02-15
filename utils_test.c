#include <stdlib.h>
#include <stdint.h>

#include "chacha20.h"
#include "utils.h"
#include "test_utils.h"

int
test_serialize(void)
{
  uint32_t state[] = { 0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3,
                       0xc7f4d1c7, 0x0368c033, 0x9aaa2204, 0x4e6cd4c3,
                       0x466482d2, 0x09aa9f07, 0x05d7c214, 0xa2028bd9,
                       0xd19c12b5, 0xb94e16de, 0xe883d0cb, 0x4e3c50a2 };

  const unsigned char expected[] = { 0x10, 0xf1, 0xe7, 0xe4, 0xd1, 0x3b, 0x59,
                                     0x15, 0x50, 0x0f, 0xdd, 0x1f, 0xa3, 0x20,
                                     0x71, 0xc4, 0xc7, 0xd1, 0xf4, 0xc7, 0x33,
                                     0xc0, 0x68, 0x03, 0x04, 0x22, 0xaa, 0x9a,
                                     0xc3, 0xd4, 0x6c, 0x4e, 0xd2, 0x82, 0x64,
                                     0x46, 0x07, 0x9f, 0xaa, 0x09, 0x14, 0xc2,
                                     0xd7, 0x05, 0xd9, 0x8b, 0x02, 0xa2, 0xb5,
                                     0x12, 0x9c, 0xd1, 0xde, 0x16, 0x4e, 0xb9,
                                     0xcb, 0xd0, 0x83, 0xe8, 0xa2, 0x50, 0x3c,
                                     0x4e };

  unsigned char *actual = malloc(sizeof(char)*BYTE_LENGTH);

  serialize(state, actual);
  int is_eq = is_equal(expected, actual, 64);

  free(actual);
  return is_eq;
}

int
test_slice_starting_position(void)
{
  unsigned char text[] = "hello this is a test";
  int start = 0;
  int end = 5;
  const unsigned char expected[] = "hello";
  unsigned char *actual = malloc(5*sizeof(char));

  slice(text, actual, start, end);
  int is_eq = is_equal(expected, actual, 5);

  free(actual);
  return is_eq;
}

int
test_slice_ending_position(void)
{
  unsigned char text[20] = "hello";
  int start = 2;
  int end = 5;
  const unsigned char expected[] = "llo";
  int span = end - start;
  unsigned char *actual = malloc(span*sizeof(char));

  slice(text, actual, start, end);
  int is_eq = is_equal(expected, actual, span);

  free(actual);
  return is_eq;
}


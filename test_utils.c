#include <stdint.h>

int
is_equal_uint32(const uint32_t *expected, uint32_t *actual, int length) // todo better naming
{
  for (int i=0; i<length; i++) {
    if (expected[i] != actual[i]) {
      return 0;
    }
  }
  return 1;
}

int is_equal(const unsigned char *expected, unsigned char *actual, int length)
{
  for (int i=0; i<length; i++) {
    if (expected[i] != actual[i]) {
      return 0;
    }
  }
  return 1;
}


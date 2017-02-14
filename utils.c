#include <stdint.h>

#include "chacha20.h"

void
slice(unsigned char *text, unsigned char *copy, int start, int end)
{
  for (int i=start; i<end; i++) {
    copy[i] = text[i];
  }
}

void
xor_block(unsigned char *dest, unsigned char *src, int length)
{
  for (int i=0; i<length; i++) {
    dest[i] ^= src[i];
  }
}

void
serialize(uint32_t *state, unsigned char *serialized)
{
  int counter = 0;
  for (int i=0; i<BLOCK_LENGTH; i++) {
    uint32_t b = state[i];
    serialized[counter+3] = b >> 24;
    serialized[counter+2] = b >> 16;
    serialized[counter+1] = b >> 8;
    serialized[counter] = b;
    counter += 4;
  }
}

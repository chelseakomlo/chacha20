#ifndef UTILS_H
#define UTILS_H

void slice(unsigned char *text, unsigned char *copy, int start, int end);

void xor_block(unsigned char *dest, unsigned char *src, int length);

void serialize(uint32_t *state, unsigned char *serialized);

uint32_t rotate(uint32_t a, int i);

#endif

#ifndef UTILS_H
#define UTILS_H

void slice(unsigned char *text, unsigned char *copy, int start, int end);

void xor_block(unsigned char *dest, unsigned char *src, int length);

void serialize(uint32_t *state, unsigned char *serialized);

#endif

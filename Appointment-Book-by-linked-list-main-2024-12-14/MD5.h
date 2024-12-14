#ifndef MD5_H
#define MD5_H


#include <stdint.h>
#include <stdlib.h>

// MD5 context structure
typedef struct {
    uint32_t state[4];    // State (A, B, C, D)
    uint32_t count[2];    // Number of bits, modulo 2^64
    uint8_t buffer[64];   // Input buffer
} MD5_CTX;

void MD5_Init(MD5_CTX *context);
void MD5_Update(MD5_CTX *context, const uint8_t *input, size_t inputLen);
void MD5_Final(uint8_t digest[16], MD5_CTX *context);

#endif // MD5_H

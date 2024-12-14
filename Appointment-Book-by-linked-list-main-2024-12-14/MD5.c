#include "MD5.h"
#include <string.h>
#include <stdio.h>

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

// F, G, H and I are basic MD5 functions.
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

// Rotate x left n bits
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// Rounds
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}

// Padding
static const uint8_t PADDING[64] = { 0x80 };

// Function Prototypes
static void MD5_Transform(uint32_t state[4], const uint8_t block[64]);
static void Encode(uint8_t *output, const uint32_t *input, size_t len);
static void Decode(uint32_t *output, const uint8_t *input, size_t len);

void MD5_Init(MD5_CTX *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

void MD5_Update(MD5_CTX *context, const uint8_t *input, size_t inputLen) {
    size_t i, index, partLen;

    index = (context->count[0] >> 3) & 0x3F;
    context->count[0] += (uint32_t)(inputLen << 3);
    if (context->count[0] < (inputLen << 3))
        context->count[1]++;
    context->count[1] += (uint32_t)(inputLen >> 29);

    partLen = 64 - index;

    if (inputLen >= partLen) {
        memcpy(&context->buffer[index], input, partLen);
        MD5_Transform(context->state, context->buffer);

        for (i = partLen; i + 63 < inputLen; i += 64)
            MD5_Transform(context->state, &input[i]);

        index = 0;
    } else {
        i = 0;
    }

    memcpy(&context->buffer[index], &input[i], inputLen - i);
}

void MD5_Final(uint8_t digest[16], MD5_CTX *context) {
    uint8_t bits[8];
    size_t index, padLen;

    Encode(bits, context->count, 8);

    index = (context->count[0] >> 3) & 0x3F;
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5_Update(context, PADDING, padLen);
    MD5_Update(context, bits, 8);

    Encode(digest, context->state, 16);

    memset(context, 0, sizeof(*context));
}

static void MD5_Transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    Decode(x, block, 64);

    // Round 1
    FF(a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
    FF(d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
    FF(c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
    FF(b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
    // ... Continue for all 64 operations

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    memset(x, 0, sizeof(x));
}

static void Encode(uint8_t *output, const uint32_t *input, size_t len) {
    for (size_t i = 0; i < len; i += 4) {
        output[i] = (uint8_t)(input[i / 4] & 0xFF);
        output[i + 1] = (uint8_t)((input[i / 4] >> 8) & 0xFF);
        output[i + 2] = (uint8_t)((input[i / 4] >> 16) & 0xFF);
        output[i + 3] = (uint8_t)((input[i / 4] >> 24) & 0xFF);
    }
}

static void Decode(uint32_t *output, const uint8_t *input, size_t len) {
    for (size_t i = 0; i < len; i += 4) {
        output[i / 4] = (uint32_t)input[i] |
                        ((uint32_t)input[i + 1] << 8) |
                        ((uint32_t)input[i + 2] << 16) |
                        ((uint32_t)input[i + 3] << 24);
    }
}

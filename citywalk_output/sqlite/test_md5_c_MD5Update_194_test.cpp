// test_md5.cpp
// A handcrafted C++11 test suite for the MD5Update focal method.
// This test suite is designed to run without GTest, using a minimal
// custom assertion mechanism. It exercises key control-flow paths in
// MD5Update by manipulating an in-memory MD5Context and observing side effects.

#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Domain: Ports the C-style MD5Update-ish function into a C++11 testable TU.

typedef uint32_t uint32;

// Minimal MD5Context layout matching the expectations in the focal code.
struct MD5Context {
    uint32 bits[2];
    unsigned char in[64];
    uint32 buf[4];
};

// Global state to observe MD5Transform invocations (internal to this TU).
static int g_md5_transform_calls = 0;

// Deterministic side-effect for transform to aid verification.
static void MD5Transform(uint32 buf[4], const uint32 in[16])
{
    ++g_md5_transform_calls;
    // Tiny, deterministic alteration to emulate some work without pulling in a full MD5.
    buf[0] ^= in[0];
    buf[1] ^= in[4];
    buf[2] ^= in[8];
    buf[3] ^= in[12];
}

static void byteReverse(unsigned char *buf, unsigned longs)
{
    // For testing, keep this a no-op to simplify verifications.
    (void)buf; (void)longs;
}

static void MD5Init(MD5Context *ctx)
{
    ctx->bits[0] = ctx->bits[1] = 0;
    memset(ctx->in, 0, 64);
    memset(ctx->buf, 0, 16);
}

static void MD5Final(unsigned char digest[16], MD5Context *ctx)
{
    (void)digest; (void)ctx;
}
static void MD5DigestToBase16(unsigned char *digest, char *zBuf)
{
    (void)digest; (void)zBuf;
}
static void MD5DigestToBase10x8(unsigned char digest[16], char zDigest[50])
{
    (void)digest; (void)zDigest;
}

// The focal MD5Update under test (adapted into this TU for deterministic testing).
void MD5Update(MD5Context *ctx, const unsigned char *buf, unsigned int len){
    uint32 t;
    /* Update bitcount */
    t = ctx->bits[0];
    if ((ctx->bits[0] = t + ((uint32)len << 3)) < t)
            ctx->bits[1]++; /* Carry from low to high */
    ctx->bits[1] += len >> 29;
    t = (t >> 3) & 0x3f;    /* Bytes already in shsInfo->data */
    /* Handle any leading odd-sized chunks */
    if ( t ) {
            unsigned char *p = (unsigned char *)ctx->in + t;
            t = 64-t;
            if (len < t) {
                    memcpy(p, buf, len);
                    return;
            }
            memcpy(p, buf, t);
            byteReverse(ctx->in, 16);
            MD5Transform(ctx->buf, (uint32 *)ctx->in);
            buf += t;
            len -= t;
    }
    /* Process data in 64-byte chunks */
    while (len >= 64) {
            memcpy(ctx->in, buf, 64);
            byteReverse(ctx->in, 16);
            MD5Transform(ctx->buf, (uint32 *)ctx->in);
            buf += 64;
            len -= 64;
    }
    /* Handle any remaining bytes of data. */
    memcpy(ctx->in, buf, len);
}

// Simple test harness (non-terminating assertions) and per-test explanations.

static int total_tests = 0;
static int failed_tests = 0;

static void log_fail(const char* msg, const char* file, int line)
{
    std::cerr << "TEST FAILED: " << msg << " (at " << file << ":" << line << ")\n";
}
#define EXPECT_EQ(a, b) do { \
    ++total_tests; \
    auto _a = (a); auto _b = (b); \
    if (!(_a == _b)) { log_fail("EXPECT_EQ failed: " #a " != " #b, __FILE__, __LINE__); ++failed_tests; } \
} while(0)

#define EXPECT_LE(a, b) do { \
    ++total_tests; \
    if (!((a) <= (b))) { log_fail("EXPECT_LE failed: " #a " > " #b, __FILE__, __LINE__); ++failed_tests; } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    ++total_tests; \
    if (!(cond)) { log_fail("EXPECT_TRUE failed: " #cond, __FILE__, __LINE__); ++failed_tests; } \
} while(0)

#define EXPECT_MEM_EQ(dst, src, n) do { \
    ++total_tests; \
    if (std::memcmp((dst), (src), (n)) != 0) { log_fail("EXPECT_MEM_EQ failed", __FILE__, __LINE__); ++failed_tests; } \
} while(0)

// Helper to initialize MD5Context to a known state.
static void ctx_init(MD5Context *ctx)
{
    MD5Init(ctx);
    // Clear in and buf aggressively for deterministic checks
    memset(ctx->in, 0, 64);
    memset(ctx->buf, 0, 16);
    ctx->bits[0] = 0;
    ctx->bits[1] = 0;
}

// Test A: Leading chunk path (t != 0) with len >= t to trigger MD5Transform once.
// Verifies the split handling of leading bytes and the subsequent memcpy of the remainder.
static void test_leading_chunk_path()
{
    MD5Context ctx;
    ctx_init(&ctx);

    // Prepare input: 80 bytes with a simple incremental pattern.
    unsigned char input[80];
    for (int i = 0; i < 80; ++i) input[i] = (unsigned char)i;

    // Configure initial bits[0] so that after the update it wraps into a non-zero t.
    // We want (bits0_after_update >> 3) & 0x3f != 0; Choose bits0 = 8, and len = 80;
    ctx.bits[0] = 8; ctx.bits[1] = 0;

    g_md5_transform_calls = 0;
    MD5Update(&ctx, input, 80);

    // Expected state checks
    EXPECT_EQ(ctx.bits[0], 8 + (80u << 3)); // 8 + 640 = 648
    EXPECT_EQ(ctx.bits[1], 0u);              // No carry from second word for this len
    EXPECT_EQ(g_md5_transform_calls, 1);     // One MD5Transform invocation for the 64-byte chunk

    // Leading partial chunk copied to in + t (t calculated as (previous_bits0_after_update >> 3) & 0x3f)
    // t in this setup should be 17, and len-t = 80-47 = 33; first 47 bytes go to in[17..63], next 33 to in[0..32]
    int t = ((648) >> 3) & 0x3f; // 81 >> 3 == 81/8 == 10? Correction: 648 >>3 = 81; 81 & 0x3f = 17
    // Verify the two halves of in[]
    EXPECT_EQ(t, 17u);
    // in[17..63] should be input[0..46]
    EXPECT_MEM_EQ(ctx.in + 17, input, 47);
    // in[0..32] should be input[47..79]
    EXPECT_MEM_EQ(ctx.in, input + 47, 33);
}

// Test B: No leading chunk path with exactly 64 bytes; ensures MD5Transform invocation occurs for full chunk.
static void test_full_64_byte_chunk()
{
    MD5Context ctx;
    ctx_init(&ctx);

    unsigned char input[64];
    for (int i = 0; i < 64; ++i) input[i] = (unsigned char)(i + 1);

    g_md5_transform_calls = 0;
    MD5Update(&ctx, input, 64);

    // Assertions
    EXPECT_EQ(ctx.bits[0], 64u << 3); // 512
    EXPECT_EQ(ctx.bits[1], 0u);
    EXPECT_EQ(g_md5_transform_calls, 1);
    // The whole 64-byte block should be copied into in[]
    EXPECT_MEM_EQ(ctx.in, input, 64);
}

// Test C: Leading path with len < t causing early return without MD5Transform.
// Verifies that the partial chunk is copied and function returns early.
static void test_leading_partial_no_transform()
{
    MD5Context ctx;
    ctx_init(&ctx);

    // Set bits so that t becomes non-zero after update, but len is smaller than t.
    // Choose initial bits[0] = 8; len = 1 -> bits[0] becomes 16, t = (16 >> 3) & 0x3f = 2
    // Then p = in + 2; t = 64-2 = 62; since len < t (1 < 62), memcpy(p, buf, len) and return.
    ctx.bits[0] = 8; ctx.bits[1] = 0;

    unsigned char input[1] = { 0xAB };
    g_md5_transform_calls = 0;
    MD5Update(&ctx, input, 1);

    // Assertions
    EXPECT_EQ(ctx.bits[0], 16u); // 8 + (1<<3) = 16
    EXPECT_EQ(ctx.bits[1], 0u);
    EXPECT_EQ(g_md5_transform_calls, 0); // No transform called
    // in[2] should be the input[0]
    EXPECT_EQ(ctx.in[2], input[0]);
}

// Test D: Overflow carry path in bitcount update; ensures correct carry and no MD5Transform when len small.
static void test_overflow_carry_path()
{
    MD5Context ctx;
    ctx_init(&ctx);

    // Set up a scenario where ctx->bits[0] overflows on adding len<<3
    ctx.bits[0] = 0xFFFFFFF0;
    ctx.bits[1] = 0;

    unsigned char input[2] = { 0xAA, 0xBB };
    g_md5_transform_calls = 0;
    MD5Update(&ctx, input, 2);

    // After update: bits0 wraps to 0, bits1 increments by 1 due to overflow, len>>29 is 0
    EXPECT_EQ(ctx.bits[0], 0u);
    EXPECT_EQ(ctx.bits[1], 1u);
    EXPECT_EQ(g_md5_transform_calls, 0); // No MD5Transform invoked for a short input in this path
    // in[0..1] should reflect the input
    EXPECT_EQ(ctx.in[0], input[0]);
    EXPECT_EQ(ctx.in[1], input[1]);
}

// Run all tests
int main()
{
    std::cout << "Starting MD5Update unit tests (custom test harness)\n";

    test_leading_chunk_path();
    test_full_64_byte_chunk();
    test_leading_partial_no_transform();
    test_overflow_carry_path();

    std::cout << "Tests completed. Total: " << total_tests
              << ", Passed: " << (total_tests - failed_tests)
              << ", Failed: " << failed_tests << "\n";

    return failed_tests ? 1 : 0;
}
// cmsalpha_copy16_test.cpp
// Unit tests for the focal method: copy16
// Note: This test suite is implemented without GTest, using a lightweight
// test harness with non-terminating EXPECT_* style assertions.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Forward declaration of the focal method (C linkage as it's provided in cmsalpha.c)
extern "C" void copy16(void* dst, const void* src);

// Lightweight test framework (non-terminating assertions)
static int g_testCount = 0;
static int g_failCount = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_testCount; \
    if(!(cond)) { \
        ++g_failCount; \
        fprintf(stderr, "EXPECT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_testCount; \
    if(!((a) == (b))) { \
        ++g_failCount; \
        fprintf(stderr, "EXPECT_EQ failed at %s:%d: %s == %s (got %lld vs %lld)\n", \
                __FILE__, __LINE__, #a, #b, (long long)(a), (long long)(b)); \
    } \
} while(0)

#define EXPECT_MEM_EQ(dst, expected, len) do { \
    ++g_testCount; \
    if(std::memcmp((dst), (expected), (len)) != 0) { \
        ++g_failCount; \
        fprintf(stderr, "EXPECT_MEM_EQ failed at %s:%d: memory content differs for %s\n", \
                __FILE__, __LINE__, #dst); \
    } \
} while(0)

// Candidate Keywords (for Step 1 understanding)
// - copy16: focal method under test
// - memmove: underlying memory transfer routine used by copy16
// - dst, src: pointers involved in the copy
// - 2: number of bytes copied by copy16
// - overlapping regions: behavior of memmove with overlapping memory

// Test 1: Non-overlapping copy should transfer exactly 2 bytes from src to dst
// Rationale: Validate basic correct behavior of 2-byte copy when src and dst do not overlap.
static void test_copy16_non_overlapping()
{
    uint8_t src[2]  = { 0xAA, 0xBB };
    uint8_t dst[2]  = { 0x00, 0x00 };

    // Exercise the focal method
    copy16(dst, src);

    // Expected: dst contains the two bytes from src
    EXPECT_EQ((unsigned)dst[0], 0xAA);
    EXPECT_EQ((unsigned)dst[1], 0xBB);
}

// Test 2: Overlapping regions where dst starts at src+1
// Rationale: memmove handles overlap by safe copying; ensure correct result.
static void test_copy16_overlapping_src_dst()
{
    uint8_t buf[3] = { 0x11, 0x22, 0x33 };
    // Copy 2 bytes from buf+1 to buf
    copy16(buf, buf + 1);

    // Expected: buf[0] <- old buf[1] (0x22), buf[1] <- old buf[2] (0x33), buf[2] remains 0x33
    EXPECT_EQ((unsigned)buf[0], 0x22);
    EXPECT_EQ((unsigned)buf[1], 0x33);
    EXPECT_EQ((unsigned)buf[2], 0x33);
}

// Test 3: Copy with dst == src (in-place copy should preserve data beyond first two bytes)
// Rationale: Verify no unintended modification when the source and destination pointers are identical.
static void test_copy16_in_place()
{
    uint8_t buf[4] = { 0x01, 0x02, 0x03, 0x04 };
    copy16(buf, buf); // in-place copy of 2 bytes

    // Expected: first two bytes remain unchanged, others untouched
    EXPECT_EQ((unsigned)buf[0], 0x01);
    EXPECT_EQ((unsigned)buf[1], 0x02);
    EXPECT_EQ((unsigned)buf[2], 0x03);
    EXPECT_EQ((unsigned)buf[3], 0x04);
}

// Test 4: Copy into an offset destination to ensure only 2 bytes are modified
// Rationale: Validate selective modification when dst is not the start of the array.
static void test_copy16_dst_offset()
{
    uint8_t dst[4] = { 0xAA, 0xAA, 0xAA, 0xAA };
    uint8_t src[2]  = { 0xDE, 0xAD };

    // dst+1 is the target; src provides two bytes
    copy16(dst + 1, src);

    // Expected: dst[0] remains 0xAA; dst[1] = 0xDE; dst[2] = 0xAD; dst[3] remains 0xAA
    EXPECT_EQ((unsigned)dst[0], 0xAA);
    EXPECT_EQ((unsigned)dst[1], 0xDE);
    EXPECT_EQ((unsigned)dst[2], 0xAD);
    EXPECT_EQ((unsigned)dst[3], 0xAA);
}

// Test 5: Confirm that even with larger buffers, only the first two bytes are copied
// Rationale: Ensure memory beyond the copied region is preserved.
static void test_copy16_preserves_tail_end()
{
    uint8_t src[2]  = { 0x12, 0x34 };
    uint8_t dst[6]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    copy16(dst, src); // copy into the start of dst

    // Expected: dst[0..1] = src; dst[2..5] remain untouched (0x00)
    EXPECT_EQ((unsigned)dst[0], 0x12);
    EXPECT_EQ((unsigned)dst[1], 0x34);
    EXPECT_EQ((unsigned)dst[2], 0x00);
    EXPECT_EQ((unsigned)dst[3], 0x00);
    EXPECT_EQ((unsigned)dst[4], 0x00);
    EXPECT_EQ((unsigned)dst[5], 0x00);
}

// Helper to run all tests
static void run_all_tests()
{
    test_copy16_non_overlapping();
    test_copy16_overlapping_src_dst();
    test_copy16_in_place();
    test_copy16_dst_offset();
    test_copy16_preserves_tail_end();
}

int main()
{
    // Run all focused tests for copy16
    run_all_tests();

    // Summary
    printf("copy16 test summary: total=%d, failed=%d\n", g_testCount, g_failCount);

    // Return non-zero if any test failed
    return (g_failCount == 0) ? 0 : 1;
}
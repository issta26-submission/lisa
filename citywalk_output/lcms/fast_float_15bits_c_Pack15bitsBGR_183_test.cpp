// Lightweight unit tests for Pack15bitsBGR (C function from fast_float_15bits.c)
// This test suite is written for C++11, without GoogleTest, and uses a small
// custom assertion harness to maximize coverage without terminating on first failure.

#include <cstring>
#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <cstdint>


// Lightweight test environment mirroring the types used by the focal method.
// We assume the real project uses these typedefs; if the real project defines them
// differently, the linker should still succeed given the identical underlying types.
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Forward declaration of opaque transform struct used by the focal method.
// The test does not rely on actual contents; it uses NULL for CMMcargo as allowed by the function.
struct _cmstransform_struct;

// Macro used in the focal method; defined as empty for test environment.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Prototype of the function under test ( Pack15bitsBGR ) with C linkage.
extern "C" cmsUInt8Number* Pack15bitsBGR(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                                        CMSREGISTER cmsUInt16Number Values[],
                                        CMSREGISTER cmsUInt8Number*  Buffer,
                                        CMSREGISTER cmsUInt32Number  Stride);

// Static assertion helper: track number of failed tests without aborting on first failure.
static int g_failures = 0;

// Basic assertion macro: compares two integral values and reports non-terminating failure.
#define TEST_EQ(expected, actual, message) do {                               \
    if ((static_cast<uint64_t>(static_cast<uint64_t>(actual)) !=           \
         static_cast<uint64_t>(static_cast<uint64_t>(expected)))) {         \
        std::cerr << "FAIL: " << (message)                                      \
                  << " | Expected: " << (expected)                            \
                  << "  Actual: " << (actual) << std::endl;                \
        ++g_failures;                                                        \
    }                                                                        \
} while(0)

// Helper to read a 16-bit little-endian value from a byte buffer.
static inline uint16_t read16le(const unsigned char* p) {
    return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
}

// Test 1: Basic correctness and BGR ordering
// Verifies that Pack15bitsBGR writes three 16-bit words in the order:
// From16To15(Values[2]), From16To15(Values[1]), From16To15(Values[0])
// Each value is masked to 15 bits (assumed From16To15 behavior: x & 0x7FFF).
void test_Pack15bitsBGR_basic_ordering() {
    // Values chosen to exercise the ordering and 15-bit masking.
    cmsUInt16Number Values[3];
    Values[0] = 0x1234; // to become 0x1234 & 0x7FFF
    Values[1] = 0xABCD; // to become 0xABCD & 0x7FFF
    Values[2] = 0xF0F0; // to become 0xF0F0 & 0x7FFF

    // Buffer large enough for 3 x 2 bytes
    unsigned char Buffer[12];
    std::memset(Buffer, 0xAA, sizeof(Buffer)); // fill with recognizable pattern

    cmsUInt8Number* endPtr = Pack15bitsBGR(nullptr, Values, Buffer, 0);

    // Validate return pointer equals Buffer + 6
    TEST_EQ(reinterpret_cast<cmsUInt8Number*>(Buffer) + 6, endPtr,
            "Pack15bitsBGR should return Buffer advanced by 6 bytes");

    // Expected sequence after 15-bit masking (assumed From16To15(x) => x & 0x7FFF)
    uint16_t first  = Values[2] & 0x7FFF; // first  2 bytes (little-endian)
    uint16_t second = Values[1] & 0x7FFF; // next   2 bytes
    uint16_t third  = Values[0] & 0x7FFF; // last   2 bytes

    // Read back the three 16-bit little-endian values from the buffer
    uint16_t gotFirst  = read16le(Buffer);
    uint16_t gotSecond = read16le(Buffer + 2);
    uint16_t gotThird  = read16le(Buffer + 4);

    TEST_EQ(first,  gotFirst,  "First 16-bit word should be From16To15(Values[2])");
    TEST_EQ(second, gotSecond, "Second 16-bit word should be From16To15(Values[1])");
    TEST_EQ(third,  gotThird,  "Third 16-bit word should be From16To15(Values[0])");

    // Ensure no writes beyond the 6 bytes were made
    TEST_EQ(static_cast<uint8_t>(Buffer[6]), static_cast<uint8_t>(0xAA),
            "Buffer should remain unchanged beyond written 6 bytes");
}

// Test 2: Edge masking behavior for maximum 15-bit value
void test_Pack15bitsBGR_edge_masking() {
    cmsUInt16Number Values[3];
    Values[0] = 0xFFFF; // 16-bit max
    Values[1] = 0x0000; // zero
    Values[2] = 0x8000; // MSB set, should mask to 0x0000

    unsigned char Buffer[12];
    std::memset(Buffer, 0x11, sizeof(Buffer)); // reset pattern

    cmsUInt8Number* endPtr = Pack15bitsBGR(nullptr, Values, Buffer, 0);

    // Pointer should advance by 6
    TEST_EQ(reinterpret_cast<cmsUInt8Number*>(Buffer) + 6, endPtr,
            "Pack15bitsBGR should return Buffer advanced by 6 bytes (edge case)");

    uint16_t first  = Values[2] & 0x7FFF; // 0x8000 & 0x7FFF = 0x0000
    uint16_t second = Values[1] & 0x7FFF; // 0x0000
    uint16_t third  = Values[0] & 0x7FFF; // 0x7FFF

    uint16_t gotFirst  = read16le(Buffer);
    uint16_t gotSecond = read16le(Buffer + 2);
    uint16_t gotThird  = read16le(Buffer + 4);

    TEST_EQ(first,  gotFirst,  "Masked value for Values[2] (should be 0x0000)");
    TEST_EQ(second, gotSecond, "Masked value for Values[1] (should be 0x0000)");
    TEST_EQ(third,  gotThird,  "Masked value for Values[0] (should be 0x7FFF)");
}

// Test 3: Buffer tail preservation check (no overrun beyond 6 bytes)
void test_Pack15bitsBGR_tail_preservation() {
    cmsUInt16Number Values[3] = { 0x1111, 0x2222, 0x3333 };

    // Create a larger buffer and fill with a distinct pattern
    unsigned char Buffer[24];
    std::memset(Buffer, 0xAB, sizeof(Buffer)); // ensure known content

    cmsUInt8Number* endPtr = Pack15bitsBGR(nullptr, Values, Buffer, 0);

    // Ensure end pointer is 6 bytes past start
    TEST_EQ(Buffer + 6, endPtr, "Return pointer should be 6 bytes after start");

    // Verify only first 6 bytes changed as per 3 values
    uint16_t first  = Values[2] & 0x7FFF;
    uint16_t second = Values[1] & 0x7FFF;
    uint16_t third  = Values[0] & 0x7FFF;

    uint16_t gotFirst  = read16le(Buffer);
    uint16_t gotSecond = read16le(Buffer + 2);
    uint16_t gotThird  = read16le(Buffer + 4);

    TEST_EQ(first,  gotFirst,  "First 16-bit word after Pack15bitsBGR should match From16To15(Values[2])");
    TEST_EQ(second, gotSecond, "Second 16-bit word after Pack15bitsBGR should match From16To15(Values[1])");
    TEST_EQ(third,  gotThird,  "Third 16-bit word after Pack15bitsBGR should match From16To15(Values[0])");

    // Check that memory beyond 6 bytes remains unchanged (tail preserved)
    for (size_t i = 6; i < sizeof(Buffer); ++i) {
        TEST_EQ(static_cast<uint8_t>(Buffer[i]), static_cast<uint8_t>(0xAB),
                "Buffer tail should remain unchanged after Pack15bitsBGR");
        // Do not fail fast; continue to collect all failures
    }
}

// Main: run all tests and report summary.
int main() {
    std::cout << "Running Pack15bitsBGR unit tests...\n";

    test_Pack15bitsBGR_basic_ordering();
    test_Pack15bitsBGR_edge_masking();
    test_Pack15bitsBGR_tail_preservation();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return g_failures;
    }
}
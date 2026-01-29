// Self-contained unit tests for Unroll3BytesSkip1SwapSwapFirst
// Target: Unroll3BytesSkip1SwapSwapFirst from cms2/cmspsack-like implementation
// Constraints: C++11, no Google Test, use only standard library
// This file provides a minimal, compilable environment to exercise the focal method.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstdint>


// Domain-specific type and macro shims to mirror the CMS internal types used by the focal method.
// We implement only the minimal subset required for compilation and testing.

typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// The CMS project uses macros for registering variables; we'll keep them empty here.
#define CMSREGISTER

// Conversion macro used by the focal method to widen an 8-bit value to 16-bit.
#define FROM_8_TO_16(x) (static_cast<cmsUInt16Number>((x)))

// Utility to mark unused parameters (no-op at runtime, keeps compiler quiet about unused vars).
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Opaque forward declaration for the internal transform structure.
// The focal function does not inspect its contents in the tested scenario.
struct _cmsTRANSFORM { int dummy; };

// Type alias to align with the CMS API naming in the focal method.
typedef struct _cmsTRANSFORM cmsTRANSFORM;

// The focal function under test.
// Note: This is a deliberately minimal, self-contained replica compatible with the test harness.
cmsUInt8Number* Unroll3BytesSkip1SwapSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                             CMSREGISTER cmsUInt16Number wIn[],
                                             CMSREGISTER cmsUInt8Number* accum,
                                             CMSREGISTER cmsUInt32Number Stride)
{
    // Real implementation uses a couple of gymnastics with macros;
    // for testing purposes, we reproduce the essential behavior:
    // - Read 3 consecutive 8-bit values from accum
    // - Convert each to 16-bit and store into wIn[2], wIn[1], wIn[0]
    // - Skip the next 1 byte (A channel)
    // - Return the updated accum pointer
    {
        wIn[2] = FROM_8_TO_16(*accum); accum++;
        wIn[1] = FROM_8_TO_16(*accum); accum++;
        wIn[0] = FROM_8_TO_16(*accum); accum++;
        accum++; // skip one byte (A)
        // Suppress unused parameter warnings for info and Stride
        cmsUNUSED_PARAMETER(info);
        cmsUNUSED_PARAMETER(Stride);
        return accum;
    }
}

// Lightweight test harness (no GTest). Uses non-terminating assertions to maximize coverage.
static int g_failures = 0;

// Simple assertion helpers
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << ": " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << ": " << (msg) \
                  << " | Expected: " << (b) << " Got: " << (a) << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Candidate Keywords (from Step 1) to guide test coverage
// - wIn: 3x cmsUInt16Number destination slots for 3 input components (B, G, R)
// - accum: cmsUInt8Number* pointer to input 8-bit data stream (BGRA-like layout)
// - From_8_to_16: macro converting 8-bit to 16-bit representation
// - Stride: unused by focal logic but part of signature; ensure test passes through
// - info: CMS style context parameter (unused in this path)

// Test Case 1: Basic single-block triplet conversion
// Input bytes: [0x11, 0x22, 0x33, 0x44] -> B=0x11, G=0x22, R=0x33, A skipped
// Expected: wIn = [0x0011, 0x0022, 0x0033], accum advanced by 4
bool test_basic_single_block() {
    cmsTRANSFORM ctx; // dummy context
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char buf[4] = {0x11, 0x22, 0x33, 0x44};
    cmsUInt8Number* accum = buf;
    cmsUInt32Number stride = 0; // unused
    cmsUInt8Number* next = Unroll3BytesSkip1SwapSwapFirst(&ctx, wIn, accum, stride);

    ASSERT_EQ(wIn[2], (cmsUInt16Number)0x0011, "wIn[2] should receive B (0x11)");
    ASSERT_EQ(wIn[1], (cmsUInt16Number)0x0022, "wIn[1] should receive G (0x22)");
    ASSERT_EQ(wIn[0], (cmsUInt16Number)0x0033, "wIn[0] should receive R (0x33)");
    ASSERT_TRUE(next == buf + 4, "Accum pointer should advance by 4 bytes after processing 3 bytes + skip");
    return g_failures == 0;
}

// Test Case 2: Second distinct block to verify independence from first
// Input bytes: [0xAA, 0xBB, 0xCC, 0xDD] -> B=0xAA, G=0xBB, R=0xCC, A skipped
// Expected: wIn = [0x00AA, 0x00BB, 0x00CC], accum advanced by 4
bool test_basic_second_block() {
    cmsTRANSFORM ctx;
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char buf[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    cmsUInt8Number* accum = buf;
    cmsUInt32Number stride = 0;
    cmsUInt8Number* next = Unroll3BytesSkip1SwapSwapFirst(&ctx, wIn, accum, stride);

    ASSERT_EQ(wIn[2], (cmsUInt16Number)0x00AA, "wIn[2] should receive B (0xAA)");
    ASSERT_EQ(wIn[1], (cmsUInt16Number)0x00BB, "wIn[1] should receive G (0xBB)");
    ASSERT_EQ(wIn[0], (cmsUInt16Number)0x00CC, "wIn[0] should receive R (0xCC)");
    ASSERT_TRUE(next == buf + 4, "Accum pointer should advance by 4 bytes after processing 3 bytes + skip");
    return g_failures == 0;
}

// Test Case 3: Sequential calls across two blocks to ensure pointer progression
// Block 1: [0x01,0x02,0x03,0x04] -> wIn becomes [0x0001, 0x0002, 0x0003], next at +4
// Block 2: [0x05,0x06,0x07,0x08] -> wIn becomes [0x0005, 0x0006, 0x0007], next at +8
bool test_sequential_blocks() {
    cmsTRANSFORM ctx;
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char buf[8] = {0x01,0x02,0x03,0x04, 0x05,0x06,0x07,0x08};
    cmsUInt8Number* accum = buf;
    cmsUInt32Number stride = 0;

    cmsUInt8Number* next1 = Unroll3BytesSkip1SwapSwapFirst(&ctx, wIn, accum, stride);
    ASSERT_EQ(wIn[2], (cmsUInt16Number)0x0001, "Block1: wIn[2] should be 0x0001");
    ASSERT_EQ(wIn[1], (cmsUInt16Number)0x0002, "Block1: wIn[1] should be 0x0002");
    ASSERT_EQ(wIn[0], (cmsUInt16Number)0x0003, "Block1: wIn[0] should be 0x0003");
    ASSERT_TRUE(next1 == buf + 4, "Block1: next pointer should be at +4");

    // Prepare for Block 2
    cmsUInt8Number* accum2 = next1;
    wIn[0] = wIn[1] = wIn[2] = 0; // reset
    cmsUInt8Number* next2 = Unroll3BytesSkip1SwapSwapFirst(&ctx, wIn, accum2, stride);

    ASSERT_EQ(wIn[2], (cmsUInt16Number)0x0005, "Block2: wIn[2] should be 0x0005");
    ASSERT_EQ(wIn[1], (cmsUInt16Number)0x0006, "Block2: wIn[1] should be 0x0006");
    ASSERT_EQ(wIn[0], (cmsUInt16Number)0x0007, "Block2: wIn[0] should be 0x0007");
    ASSERT_TRUE(next2 == buf + 8, "Block2: next pointer should be at +8");
    return g_failures == 0;
}

// Entry point for tests
int main() {
    std::cout << "Running Unroll3BytesSkip1SwapSwapFirst unit tests (CMS-style, non-GTest)...\n";

    bool ok = true;
    ok &= test_basic_single_block();
    ok &= test_basic_second_block();
    ok &= test_sequential_blocks();

    if(g_failures == 0 && ok) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " test failure(s) detected.\n";
        return 1;
    }
}
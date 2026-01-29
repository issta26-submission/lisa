// C++11 Unit Test Suite for Pack3WordsAndSkip1 (cmspack.c)
// Notes:
// - This test suite targets the focal function Pack3WordsAndSkip1 as defined in the
//   provided cmspack.c. It uses a minimal, self-contained test harness (no GoogleTest).
// - It relies on linking with the original cmspack.c (and its dependencies) in the build.
// - We provide a light-weight assertion mechanism that does not terminate on failure
//   to maximize code execution and coverage.
// - The tests are designed to be portable across endiannesses by validating memory
//   contents via 16-bit reads, which reflect how the function stores values.
// - The test suite includes explanatory comments for each unit test.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Minimal macro shim to make the test portable with the original C code definitions.
// The real code may define CMSREGISTER; we treat it as empty for compatibility.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Forward declaration of the opaque CMS transform type used by the focal function.
// We only need an incomplete type here to satisfy the function signature during linking.
struct _cmsTRANSFORM;
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Typedefs to match the focal function's parameter types (as used in the C source).
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Declare the focal function with C linkage to ensure proper symbol resolution when linking
// with the C source cmspack.c.
extern "C" cmsUInt8Number* Pack3WordsAndSkip1(CMSREGISTER _cmsTRANSFORM* info,
                                             CMSREGISTER cmsUInt16Number wOut[],
                                             CMSREGISTER cmsUInt8Number* output,
                                             CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating assertion helpers
static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

#define ASSERT_TRUE(cond, msg) do {                               \
    ++g_total_tests;                                              \
    if (static_cast<bool>(cond)) {                                \
        ++g_passed_tests;                                         \
    } else {                                                      \
        ++g_failed_tests;                                         \
        std::fprintf(stderr, "Test failed: %s (%s:%d): %s\n",  \
                     __FILE__, __LINE__ ? __FILE__ : "", __LINE__, #cond); \
        if (msg) std::fprintf(stderr, "  Hint: %s\n", msg);      \
    }                                                             \
} while (0)

// Helper to run a test function and accumulate results
#define RUN_TEST(testFunc) do { testFunc(); } while(0)

// ------------------------------------------------------------------------------------------------
// Step 1: Candidate Keywords Extracted from Focal Method (for reference in comments)
// - Core operation: pack three 16-bit words into an 8-byte output buffer (3*2 bytes)
// - Inputs: info (CMS transform context, unused here), wOut[3] (three 16-bit words), output (byte buffer), Stride (unused here)
// - Behavior: write wOut[0], wOut[1], wOut[2] sequentially into the output buffer as 16-bit words,
//             advance the output pointer by 8 bytes in total, and return the pointer.
// - Side effects: none (besides output memory), info and Stride are marked unused.
// - Notes: True/false branches are not present in this function; test focuses on deterministic memory writes.
// ------------------------------------------------------------------------------------------------

// Test 1: Basic packing correctness using a representative set of 16-bit values
// Expected: The first 2 bytes of output == wOut[0], next 2 bytes == wOut[1], next 2 bytes == wOut[2],
// and the returned pointer equals the original buffer start + 8 bytes.
void test_Pack3WordsAndSkip1_basicPacking() {
    // Prepare input words
    cmsUInt16Number wOut[3] = { 0x1234, 0xABCD, 0x0F0F };

    // Prepare 8-byte output buffer (3 words * 2 bytes = 6 bytes; plus 2 bytes padding to match return)
    uint8_t buffer[8];
    std::memset(buffer, 0xAA, sizeof(buffer)); // fill with known pattern to verify overwrite

    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number stride = 0;

    // Call the focal function
    cmsUInt8Number* ret = Pack3WordsAndSkip1(info, wOut, buffer, stride);

    // Validate return pointer is buffer + 8
    ASSERT_TRUE(ret == buffer + 8, "Returned pointer should be buffer + 8");

    // Validate first three 16-bit words written match wOut
    cmsUInt16Number v0 = *reinterpret_cast<cmsUInt16Number*>(buffer);
    cmsUInt16Number v1 = *reinterpret_cast<cmsUInt16Number*>(buffer + 2);
    cmsUInt16Number v2 = *reinterpret_cast<cmsUInt16Number*>(buffer + 4);

    ASSERT_TRUE(v0 == wOut[0], "First packed word should match wOut[0]");
    ASSERT_TRUE(v1 == wOut[1], "Second packed word should match wOut[1]");
    ASSERT_TRUE(v2 == wOut[2], "Third packed word should match wOut[2]");
}

// Test 2: Multiple distinct inputs to ensure consistent behavior across values
// This verifies cross-check: different data should produce consistent, independent results.
void test_Pack3WordsAndSkip1_differentInputs() {
    cmsUInt16Number wOut[3] = { 0x0001, 0x00FF, 0xA5A5 };

    uint8_t buffer[8];
    std::memset(buffer, 0x00, sizeof(buffer));

    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number stride = 0;

    cmsUInt8Number* ret = Pack3WordsAndSkip1(info, wOut, buffer, stride);

    ASSERT_TRUE(ret == buffer + 8, "Returned pointer should be buffer + 8 (Test 2)");

    cmsUInt16Number v0 = *reinterpret_cast<cmsUInt16Number*>(buffer);
    cmsUInt16Number v1 = *reinterpret_cast<cmsUInt16Number*>(buffer + 2);
    cmsUInt16Number v2 = *reinterpret_cast<cmsUInt16Number*>(buffer + 4);

    ASSERT_TRUE(v0 == wOut[0], "Test 2: First word should match wOut[0]");
    ASSERT_TRUE(v1 == wOut[1], "Test 2: Second word should match wOut[1]");
    ASSERT_TRUE(v2 == wOut[2], "Test 2: Third word should match wOut[2]");
}

// Test 3: Edge-case values (maximums) to ensure no overflow or unexpected behavior
void test_Pack3WordsAndSkip1_edgeCases() {
    cmsUInt16Number wOut[3] = { 0xFFFF, 0x8000, 0x0000 };

    uint8_t buffer[8];
    std::memset(buffer, 0xFF, sizeof(buffer));

    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number stride = 0;

    cmsUInt8Number* ret = Pack3WordsAndSkip1(info, wOut, buffer, stride);

    ASSERT_TRUE(ret == buffer + 8, "Returned pointer should be buffer + 8 (Test 3)");

    cmsUInt16Number v0 = *reinterpret_cast<cmsUInt16Number*>(buffer);
    cmsUInt16Number v1 = *reinterpret_cast<cmsUInt16Number*>(buffer + 2);
    cmsUInt16Number v2 = *reinterpret_cast<cmsUInt16Number*>(buffer + 4);

    ASSERT_TRUE(v0 == wOut[0], "Test 3: First word should match wOut[0] (max)");
    ASSERT_TRUE(v1 == wOut[1], "Test 3: Second word should match wOut[1] (max)");
    ASSERT_TRUE(v2 == wOut[2], "Test 3: Third word should match wOut[2] (zero)");
}

// ------------------------------------------------------------------------------------------------
// Step 3: Test Runner
// Using a simple main to run tests without a full framework.
// The tests exercise the core behavior of the focal function and exercise boundary values.
// ------------------------------------------------------------------------------------------------

int main() {
    // Explain what we are testing
    std::printf("Running tests for Pack3WordsAndSkip1...\n");

    RUN_TEST(test_Pack3WordsAndSkip1_basicPacking);
    RUN_TEST(test_Pack3WordsAndSkip1_differentInputs);
    RUN_TEST(test_Pack3WordsAndSkip1_edgeCases);

    // Summary
    std::printf("Tests run: %d, Passed: %d, Failed: %d\n",
                g_total_tests, g_passed_tests, g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}

// End of test suite for Pack3WordsAndSkip1
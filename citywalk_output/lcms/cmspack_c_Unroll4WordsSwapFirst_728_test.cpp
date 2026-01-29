//.cpp - Lightweight unit tests for Unroll4WordsSwapFirst in cmspack.c
// Domain: C++11, no GoogleTest, use only standard library plus provided CMS headers

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Forward declaration of the focal function (C linkage)
/* The function signature mirrors the one in the focal code:
   cmsUInt8Number* Unroll4WordsSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                         CMSREGISTER cmsUInt16Number wIn[],
                                         CMSREGISTER cmsUInt8Number* accum,
                                         CMSREGISTER cmsUInt32Number Stride);
   We declare it with extern "C" to ensure correct linkage when linking with the library. */
extern "C" cmsUInt8Number* Unroll4WordsSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                               CMSREGISTER cmsUInt16Number wIn[],
                                               CMSREGISTER cmsUInt8Number* accum,
                                               CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating test harness
static int g_totalTests = 0;
static int g_failedTests = 0;

// Basic assertion macro: prints failure but does not abort the test run
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[Test Failed] " << msg \
                  << " (File " << __FILE__ << ", Line " << __LINE__ << ")" << std::endl; \
        ++g_failedTests; \
    } else { \
        std::cerr << "[Test Passed] " << msg << std::endl; \
    } \
} while(0)

// Test 1: Basic behavior of Unroll4WordsSwapFirst
// - Constructs an 8-byte input containing four 16-bit values (K, C, M, Y) in host representation.
// - Verifies that wIn receives [C, M, Y, K] according to the function's swap-first semantics.
// - Verifies that the returned accum pointer points 8 bytes past the original input.
void test_Unroll4WordsSwapFirst_BasicOrder()
{
    ++g_totalTests;

    // Prepare input: 4 words = K, C, M, Y
    cmsUInt16Number K = 0x1234;
    cmsUInt16Number C = 0xABCD;
    cmsUInt16Number M = 0x0F0F;
    cmsUInt16Number Y = 0x55AA;

    // Memory buffer to be read by the function (8 bytes total)
    cmsUInt8Number mem[8];
    // Fill memory using memcpy to ensure a clean 16-bit boundary without alignment hazards
    memcpy(mem + 0, &K, sizeof(K)); // first two bytes = K
    memcpy(mem + 2, &C, sizeof(C)); // next two bytes = C
    memcpy(mem + 4, &M, sizeof(M)); // next two bytes = M
    memcpy(mem + 6, &Y, sizeof(Y)); // last two bytes = Y

    // Output 4 words to be filled by the function
    cmsUInt16Number wIn[4] = {0, 0, 0, 0};

    // Call the focal function
    cmsUInt8Number* ret = Unroll4WordsSwapFirst(nullptr, wIn, mem, 0);

    // Expected mapping:
    // wIn[3] <- K
    // wIn[0] <- C
    // wIn[1] <- M
    // wIn[2] <- Y
    TEST_ASSERT(wIn[0] == C, "Unroll4WordsSwapFirst: wIn[0] == C");
    TEST_ASSERT(wIn[1] == M, "Unroll4WordsSwapFirst: wIn[1] == M");
    TEST_ASSERT(wIn[2] == Y, "Unroll4WordsSwapFirst: wIn[2] == Y");
    TEST_ASSERT(wIn[3] == K, "Unroll4WordsSwapFirst: wIn[3] == K");

    // Ensure the function consumed exactly 8 bytes
    TEST_ASSERT(ret == mem + 8, "Unroll4WordsSwapFirst: returned pointer == mem + 8");
}

// Test 2: Confirm that additional memory after the consumed block is preserved
// - Uses a second distinct input to verify consistent behavior.
// - Places a guard region immediately after the 8-byte input and checks it is unchanged.
void test_Unroll4WordsSwapFirst_PreserveGuardMemory()
{
    ++g_totalTests;

    // New input values
    cmsUInt16Number K = 0xAAAA;
    cmsUInt16Number C = 0x1234;
    cmsUInt16Number M = 0xABAB;
    cmsUInt16Number Y = 0xC3C3;

    // Allocate 20 bytes to hold input + guard region
    cmsUInt8Number mem[20];
    // Fill first 8 bytes with K, C, M, Y in host representation
    memcpy(mem, &K, sizeof(K));
    memcpy(mem + 2, &C, sizeof(C));
    memcpy(mem + 4, &M, sizeof(M));
    memcpy(mem + 6, &Y, sizeof(Y));

    // Initialize guard region (bytes 8..15) to a known pattern
    for (int i = 8; i < 16; ++i) mem[i] = static_cast<cmsUInt8Number>(0xAA);

    // Output array
    cmsUInt16Number wIn[4] = {0, 0, 0, 0};

    // Call the focal function
    cmsUInt8Number* ret = Unroll4WordsSwapFirst(nullptr, wIn, mem, 0);

    // Expected mapping remains consistent: wIn = [C, M, Y, K]
    TEST_ASSERT(wIn[0] == C, "Unroll4WordsSwapFirst (2): wIn[0] == C");
    TEST_ASSERT(wIn[1] == M, "Unroll4WordsSwapFirst (2): wIn[1] == M");
    TEST_ASSERT(wIn[2] == Y, "Unroll4WordsSwapFirst (2): wIn[2] == Y");
    TEST_ASSERT(wIn[3] == K, "Unroll4WordsSwapFirst (2): wIn[3] == K");

    // Guard region should remain unmodified
    TEST_ASSERT(mem[8] == static_cast<cmsUInt8Number>(0xAA) &&
                mem[9] == static_cast<cmsUInt8Number>(0xAA) &&
                mem[10] == static_cast<cmsUInt8Number>(0xAA) &&
                mem[11] == static_cast<cmsUInt8Number>(0xAA),
                "Unroll4WordsSwapFirst (2): guard region preserved after call");

    // Returned pointer should be mem + 8
    TEST_ASSERT(ret == mem + 8, "Unroll4WordsSwapFirst (2): returned pointer == mem + 8");
}

// Main: Run tests and report summary
int main()
{
    std::cout << "Running Unroll4WordsSwapFirst test suite (cmspack.c) under C++11" << std::endl;

    test_Unroll4WordsSwapFirst_BasicOrder();
    test_Unroll4WordsSwapFirst_PreserveGuardMemory();

    // Summary
    std::cout << "Total tests run: " << g_totalTests
              << ", Failures: " << g_failedTests << std::endl;

    if (g_failedTests == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
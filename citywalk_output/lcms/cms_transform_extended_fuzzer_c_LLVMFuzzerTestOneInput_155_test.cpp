/*
Unit test suite for the focal method LLVMFuzzerTestOneInput present in cms_transform_extended_fuzzer.c

Overview:
- Focuses on validating the observable behavior of LLVMFuzzerTestOneInput without relying on
  internal/private details of the transformation performed inside run_test.
- Since the actual run_test implementation is not visible for assertion, tests primarily
  verify control flow and boundary behavior (especially the size >= 12 vs size < 12 branches).
- The test harness uses a minimal, self-contained assertion framework (no GTest) and a
  simple main() to execute tests in a single binary.

Key concepts (Candidate Keywords derived from Step 1):
- size check (size < 12)
- data interpretation as 32-bit words: flags (first word), intent (second word modulo 16)
- decider derived from third 32-bit word modulo 11
- data pointer advancement by 12 bytes before passing to run_test
- run_test(data, size, intent, flags, decider) is invoked when size >= 12
- return value of LLVMFuzzerTestOneInput is 0 for all inputs

Notes:
- The tests assume an environment where the focal function is compiled as C and linked with the test.
- The tests declare the focal function with C linkage to ensure proper symbol resolution:
  extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);
- We do not attempt to mock or override run_test (due to typical strong/weak symbol linkage
  constraints). Instead, we validate control flow and boundary behavior as a reliable observable.

Build guidance (to be used by the user outside this code snippet):
- Compile cms_transform_extended_fuzzer.c together with this test file, ensuring C linkage for the focal
  function:
  g++ -std=c++11 -O2 -DUNIT_TEST cms_transform_extended_fuzzer.c cms_transform_ext_unit_test.cpp -o cms_ext_unit_test

*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <cstddef>
#include <stdint.h>
#include <lcms2.h>
#include <cstdint>


// Declaration of the focal function (C linkage)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

// Simple, non-terminating assertion helpers
static int g_test_failures = 0;

#define TEST_EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "TEST_FAIL: %s\n", (msg)); \
        ++g_test_failures; \
    } \
} while(0)

#define TEST_EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        fprintf(stderr, "TEST_FAIL: %s (expected %lld, got %lld)\n", (msg), (long long)(b), (long long)(a)); \
        ++g_test_failures; \
    } \
} while(0)

static void test_size_less_than_12()
{
    // Test that any input with size < 12 yields 0 and does not crash.
    // This covers the true branch of the predicate.
    for (size_t sz = 0; sz < 12; ++sz) {
        std::vector<uint8_t> buf(sz, 0); // fill with zeros
        int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());
        TEST_EXPECT_EQ(ret, 0, "LLVMFuzzerTestOneInput should return 0 for size < 12");
    }
}

static void test_size_equal_or_greater_12()
{
    // Test that size >= 12 paths are exercised.
    // We cannot observe internal call to run_test, but we can ensure
    // that the function does not crash and returns 0 (as specified).
    // Case 1: exactly 12 bytes (minimum to pass the guard)
    {
        std::vector<uint8_t> buf(12, 0);
        // Initialize the first three 32-bit words (flags, intent, decider)
        // to avoid any potential alignment hazards in the test scenario.
        uint32_t w0 = 0x01020304; // flags
        uint32_t w1 = 0x00000005; // intent (will be modulo 16 in the function)
        int      w2 = -7;          // decider (will be modulo 11 in the function)

        std::memcpy(buf.data(), &w0, 4);
        std::memcpy(buf.data() + 4, &w1, 4);
        std::memcpy(buf.data() + 8, &w2, 4);

        int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());
        TEST_EXPECT_EQ(ret, 0, "LLVMFuzzerTestOneInput should return 0 for size == 12");
    }

    // Case 2: larger size (e.g., 32 bytes) to ensure extra data does not cause issues
    {
        size_t sz = 32;
        std::vector<uint8_t> buf(sz, 0);
        uint32_t w0 = 0xA1B2C3D4;
        uint32_t w1 = 0x0000000F; // 15, so intent % 16 == 15
        int      w2 = 42;         // 42 % 11 == 9

        std::memcpy(buf.data(), &w0, 4);
        std::memcpy(buf.data() + 4, &w1, 4);
        std::memcpy(buf.data() + 8, &w2, 4);
        // rest of the data remains zero

        int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());
        TEST_EXPECT_EQ(ret, 0, "LLVMFuzzerTestOneInput should return 0 for size > 12");
    }
}

int main() {
    // Run unit tests
    test_size_less_than_12();
    test_size_equal_or_greater_12();

    if (g_test_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED: %d failure(s)\n", g_test_failures);
        return 1;
    }
}
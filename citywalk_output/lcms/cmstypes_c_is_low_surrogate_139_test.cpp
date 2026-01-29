/*
Candidate Keywords extracted from the focal method and dependencies:
- is_low_surrogate, is_high_surrogate, is_surrogate
- cmsUInt32Number, cmsBool, cmsInline (cmsINLINE)
- surrogate range: 0xDC00 - 0xDFFF (low surrogate)
- bit mask: 0xfffffc00
- operation: (uc & 0xfffffc00) == 0xdc00
- context: cms types and header lcms2_internal.h
- behavior: determine if a given code point is a UTF-16 low surrogate

This test suite targets the is_low_surrogate function defined in the Little CMS codebase.
It is designed to be compiled under C++11 without GTest, using a tiny, self-contained
test harness with non-terminating assertions to maximize coverage.
*/
#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


extern "C" {
}

// Lightweight non-terminating test framework (no GTest dependency)
static int g_failures = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << __FUNCTION__ << " - expected true but was false: " #cond << "\n"; \
        ++g_failures; \
    } \
} while(0)
#define EXPECT_FALSE(cond) do { \
    if((cond)) { \
        std::cerr << "TEST_FAIL: " << __FUNCTION__ << " - expected false but was true: " #cond << "\n"; \
        ++g_failures; \
    } \
} while(0)

// Test 1: True for all values in the low surrogate range 0xDC00..0xDFFF
// This exercises the main predicate path of is_low_surrogate.
static void test_is_low_surrogate_true_range() {
    // The entire low surrogate range consists of 0x400 (= 1024) values
    // with upper bits (uc >> 10) equal to 0x37 (0b00110111), i.e. (0x37 << 10) == 0xDC00
    // Iterate over all 1024 possible low-surrogate low-words
    for (uint32_t low = 0; low < 0x400; ++low) {
        uint32_t uc = (0x37u << 10) | low; // uc in [0xDC00, 0xDCFF]..[0xDC00..0xDCFF] plus variations within range
        EXPECT_TRUE(is_low_surrogate(uc));
    }
}

// Test 2: False for values outside the low surrogate range
// We verify that clearly non-surrogate values do not satisfy the predicate.
static void test_is_low_surrogate_false_out_of_range() {
    // High surrogate region start: 0xD800..0xDBFF
    EXPECT_FALSE(is_low_surrogate(0xD800));
    EXPECT_FALSE(is_low_surrogate(0xDBFF));

    // Values clearly outside surrogate ranges
    EXPECT_FALSE(is_low_surrogate(0x0000));
    EXPECT_FALSE(is_low_surrogate(0xFFFF));   // beyond the surrogate ranges
    EXPECT_FALSE(is_low_surrogate(0xDEAD));   // arbitrary non-surrogate value
    EXPECT_FALSE(is_low_surrogate(0x123456)); // large value beyond 32-bit surrogate boundary
}

// Entry point to run the tests
int main() {
    std::cout << "Running unit tests for is_low_surrogate...\n";

    test_is_low_surrogate_true_range();
    test_is_low_surrogate_false_out_of_range();

    if (g_failures > 0) {
        std::cout << "Total failures: " << g_failures << "\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}
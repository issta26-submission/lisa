// Test suite for cmsUInt32Number mywcslen(const wchar_t *s)
// Focus: verify behavior of wide-character string length computation.
// This test file is designed for C++11 compilation without using GTest.
// It uses a lightweight, non-terminating assertion style so all tests run
// and report their results in a single run.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cwchar>



// Import project dependencies and declare the focal function with C linkage.
// The actual implementation of mywcslen is in cmstypes.c within the project.
// We rely on lcms2_internal.h for cmsUInt32Number and related types.
extern "C" {
}

// Declaration of the focal function under test.
// It is defined in C, so we declare it with C linkage to avoid name mangling.
extern "C" cmsUInt32Number mywcslen(const wchar_t *s);


// Simple non-terminating test harness
static int g_test_failures = 0;

// Non-terminating expectation for unsigned integer results
#define EXPECT_EQ_UI(actual, expected) do { \
    auto _act = (actual); \
    auto _exp = (expected); \
    if (static_cast<unsigned long long>(_act) != static_cast<unsigned long long>(_exp)) { \
        ++g_test_failures; \
        std::cerr << "TEST FAILED at " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " << static_cast<unsigned long long>(_exp) \
                  << ", Got: " << static_cast<unsigned long long>(_act) << "\n"; \
    } \
} while (0)

// Non-terminating assertion for boolean conditions
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        ++g_test_failures; \
        std::cerr << "TEST FAILED at " << __FILE__ << ":" << __LINE__ \
                  << " - Condition is false: " #cond "\n"; \
    } \
} while (0)


// Test 1: Empty string should have length 0
// Rationale: Basic boundary condition; ensures no off-by-one errors.
static void test_mywcslen_empty() {
    const wchar_t* s = L"";
    cmsUInt32Number len = mywcslen(s);
    EXPECT_EQ_UI(len, 0);
    // Cross-check against std::wcslen as an additional validation
    EXPECT_EQ_UI(len, std::wcslen(s));
    // Explanation: This validates no extra increment occurs and the return type fits.
}


// Test 2: Single character wide string should have length 1
// Rationale: Basic non-empty case with minimal length.
static void test_mywcslen_single_char() {
    const wchar_t* s = L"A";
    cmsUInt32Number len = mywcslen(s);
    EXPECT_EQ_UI(len, 1);
    EXPECT_EQ_UI(len, std::wcslen(s));
    // Explanation: Confirms function handles single-element strings correctly.
}


// Test 3: Short multi-character string should match expected length
// Rationale: Typical case with several characters.
static void test_mywcslen_short_string() {
    const wchar_t* s = L"abcdefghij"; // length 10
    cmsUInt32Number len = mywcslen(s);
    EXPECT_EQ_UI(len, 10);
    EXPECT_EQ_UI(len, std::wcslen(s));
    // Explanation: Verifies that sequential incrementation stops at null terminator.
}


// Test 4: Very long string to exercise performance/loop behavior
// Rationale: Ensure function handles long inputs without overflow or incorrect termination.
static void test_mywcslen_long_string() {
    // Create a wide string of 1000 'a' characters
    std::wstring w(1000, L'a');
    cmsUInt32Number len = mywcslen(w.c_str());
    EXPECT_EQ_UI(len, 1000);
    EXPECT_EQ_UI(len, std::wcslen(w.c_str()));
    // Explanation: Tests robustness for large inputs without early termination.
}


// Test 5: Cross-check with std::wcslen on a mixed, non-trivial string
// Rationale: Validate parity with standard library implementation for ascii content.
static void test_mywcslen_ascii_parity() {
    const wchar_t* s = L"The quick brown fox jumps over the lazy dog 1234567890";
    cmsUInt32Number len_mine = mywcslen(s);
    cmsUInt32Number len_std  = std::wcslen(s);
    EXPECT_EQ_UI(len_mine, len_std);
    // Explanation: Confirms compatibility with C/C++ standard behavior on ASCII data.
}


// Main function to execute all tests
int main() {
    // Run tests
    test_mywcslen_empty();
    test_mywcslen_single_char();
    test_mywcslen_short_string();
    test_mywcslen_long_string();
    test_mywcslen_ascii_parity();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
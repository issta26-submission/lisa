// Test suite for cmsUInt32Number mywcslen(const wchar_t *s)
// This test focuses on verifying correct wide-character string length calculation.
// It uses a lightweight, non-terminating test harness (no GTest) to maximize execution
// coverage and adheres to C++11 compatibility.

// Include the internal CMS headers to align types (cmsUInt32Number, etc.)
#include <vector>
#include <iostream>
#include <string>
#include <cwchar>
#include <lcms2_internal.h>



// Expose the focal C function with C linkage
extern "C" cmsUInt32Number mywcslen(const wchar_t *s);

// Simple non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_EQ(a, b) do { ++g_total; if (!((a) == (b))) { ++g_failed; std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; } } while(0)
#define EXPECT_TRUE(cond) do { ++g_total; if(!(cond)) { ++g_failed; std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; } } while(0)

static void printSummary() {
    std::cerr << "Mywcslen tests completed. Total: " << g_total << ", Failures: " << g_failed << "\n";
}

// Test 1: Empty string should return length 0
// Rationale: The function should handle a pointer to an empty string properly, without dereferencing past the terminator.
static void test_empty_string()
{
    // Candidate Keywords: wchar_t, empty string, null terminator, pointer arithmetic
    const wchar_t s[] = L"";
    cmsUInt32Number len = mywcslen(s);
    EXPECT_EQ(len, (cmsUInt32Number)std::wcslen(s)); // should be 0
}

// Test 2: Regular ASCII-wide string
// Rationale: Basic case with a typical short string to validate standard behavior.
static void test_ascii_string()
{
    const wchar_t s[] = L"Hello";
    cmsUInt32Number len = mywcslen(s);
    EXPECT_EQ(len, (cmsUInt32Number)std::wcslen(s));
}

// Test 3: Embedded null within literal
// Rationale: Ensure the function stops at the first NUL and does not treat embedded NULs as terminators for the string length beyond the first.
static void test_embedded_null()
{
    // The literal contains an embedded L'\0' after 'ab'
    const wchar_t s[] = L"ab\0cd";
    cmsUInt32Number len = mywcslen(s);
    // std::wcslen should stop at the first embedded null, giving length 2
    EXPECT_EQ(len, (cmsUInt32Number)std::wcslen(s));
}

// Test 4: Very long string to exercise robustness
// Rationale: Verify correctness for longer inputs and ensure no overflow in counting.
static void test_long_string()
{
    // Build a long string programmatically to avoid hard-coding a huge literal
    std::wstring w;
    w.assign(1000, L'a'); // 1000 'a' characters
    // w is a modern C++ string; c_str() provides a null-terminated array
    const wchar_t* s = w.c_str();
    cmsUInt32Number len = mywcslen(s);
    EXPECT_EQ(len, (cmsUInt32Number)std::wcslen(s));
}

// Entry point for running all tests
int main()
{
    test_empty_string();
    test_ascii_string();
    test_embedded_null();
    test_long_string();

    printSummary();
    // Return non-zero if tests failed to aid integration in CI environments
    return g_failed;
}
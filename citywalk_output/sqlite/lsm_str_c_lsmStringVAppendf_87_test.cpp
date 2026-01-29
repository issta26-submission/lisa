/*
  C++11 test suite for the focal method lsmStringVAppendf in lsm_str.c

  - Tests cover: 
      1) Non-extension path (sufficient allocation)
      2) Extension path (allocation grows via lsmStringExtend)
      3) Extension path with multiple arguments ("%s-%s") to exercise ap2 usage

  - Approach:
      - Create a minimal compatible LsmString struct in tests (nAlloc, n, z)
      - Declare extern "C" functions that are actually provided by the production code:
          - int lsmStringExtend(LsmString *pStr, int nNew);
          - void lsmStringVAppendf(LsmString *pStr, const char *zFormat, va_list ap1, va_list ap2);
      - Use variadic wrappers to construct va_list ap1 and ap2, then invoke lsmStringVAppendf
      - Use a lightweight non-terminating assertion mechanism (test harness) to maximize coverage
      - Provide explanatory comments for each test case

  Notes:
  - The test suite assumes the production code (lsm_str.c and related headers) is available during linking.
  - We keep all tests self-contained and release control back to main() for a simple result summary.
  - Static/private members are not accessed; we rely only on the public function interface declared here.

  Build example (assumes production sources are available):
      g++ -std=c++11 test_lsm_string_vappendf.cpp lsm_str.c -I. -o test_lsm_string_vappendf
*/

#include <lsmInt.h>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Step 1: Provide a compatible surface for LsmString and required functions.
// We mirror the minimal layout used by the production code and declare the
// relevant functions with C linkage to allow linking with the C implementation.

struct LsmString {
    int nAlloc; // total allocated size of the buffer z
    int n;      // current length of the string stored in z
    char *z;    // buffer holding the string (null-terminated)
};

// Forward declaration for use in test code
extern "C" {
    // The actual production code defines lsmStringExtend and uses LsmString type.
    // We declare it here so tests can trigger reallocation when needed.
    int lsmStringExtend(LsmString *pStr, int nNew);

    // Primary focal method under test: accepts two va_list arguments
    void lsmStringVAppendf(
        LsmString *pStr,
        const char *zFormat,
        va_list ap1,
        va_list ap2
    );
}

// Simple test harness globals to collect results from variadic wrappers
static bool g_test_pass_once;

static void test_report(const char* test_name, bool ok) {
    if (ok) {
        std::cout << "PASS: " << test_name << std::endl;
    } else {
        std::cerr << "FAIL: " << test_name << std::endl;
    }
}

// Wrapper 1: Non-extension path (enough initial allocation)
// Candidate Keywords: nAlloc, n, z, vsnprintf (through lsmStringVAppendf), ap1, ap2
static void test_no_extend_with_args(const char* fmt, ...) {
    // Create a reasonably large initial buffer to avoid triggering extension
    LsmString s;
    s.nAlloc = 256;
    s.n = 0;
    s.z = (char*)std::malloc(s.nAlloc);
    if (s.z == nullptr) {
        g_test_pass_once = false;
        return;
    }
    s.z[0] = '\0';

    // Build va_list ap1 and ap2 from variadic arguments
    va_list ap1;
    va_list ap2;
    va_start(ap1, fmt);              // last named parameter is fmt
    va_copy(ap2, ap1);

    // Call focal method
    lsmStringVAppendf(&s, fmt, ap1, ap2);

    va_end(ap1);
    va_end(ap2);

    // Validation: Expect the string to be exactly the formatted output
    // We expect "Hello World" from "Hello %s" with argument "World"
    bool ok = (std::strcmp(s.z, "Hello World") == 0) && (s.n == 11);

    std::free(s.z);
    g_test_pass_once = ok;
}

// Wrapper 2: Extension path (short initial allocation triggers lsmStringExtend)
// Candidate Keywords: nAlloc, n, z, ap1, ap2, extension path
static void test_extend_with_args(const char* fmt, ...) {
    // Force a small initial allocation to trigger extension
    LsmString s;
    s.nAlloc = 5; // very small to trigger nAvail < nWrite
    s.n = 0;
    s.z = (char*)std::malloc(s.nAlloc);
    if (s.z == nullptr) {
        g_test_pass_once = false;
        return;
    }
    s.z[0] = '\0';

    va_list ap1;
    va_list ap2;
    va_start(ap1, fmt);
    va_copy(ap2, ap1);

    lsmStringVAppendf(&s, fmt, ap1, ap2);

    va_end(ap1);
    va_end(ap2);

    // Validation: Expect proper extension result.
    // For format "%s" with argument "abcdefghij" (length 10)
    // final string should be "abcdefghij" (n == 10)
    bool ok = (std::strcmp(s.z, "abcdefghij") == 0) && (s.n == 10);

    std::free(s.z);
    g_test_pass_once = ok;
}

// Wrapper 3: Multi-argument extension path (%s-%s) to exercise ap2/A; ensures ap2 formatting works
static void test_two_args_with_args(const char* fmt, ...) {
    // Short initial allocation to force extension
    LsmString s;
    s.nAlloc = 4; // small to trigger extension
    s.n = 0;
    s.z = (char*)std::malloc(s.nAlloc);
    if (s.z == nullptr) {
        g_test_pass_once = false;
        return;
    }
    s.z[0] = '\0';

    va_list ap1;
    va_list ap2;
    va_start(ap1, fmt);
    va_copy(ap2, ap1);

    lsmStringVAppendf(&s, fmt, ap1, ap2);

    va_end(ap1);
    va_end(ap2);

    // Validation: Expect "First-Second" with length 12
    bool ok = (std::strcmp(s.z, "First-Second") == 0) && (s.n == 12);

    std::free(s.z);
    g_test_pass_once = ok;
}

// Domain knowledge note:
// - We rely on the production lsmStringExtend implementation to manage dynamic allocation.
// - ap1/ap2 are two va_list instances; the test wrappers create them via va_start/va_copy.
// - All tests exercise both true/false branches of the condition check on allocation size.

// Step 3: Test harness (main) to execute tests without GTest
int main() {
    bool all_ok = true;

    // Test 1: Non-extension path
    g_test_pass_once = false;
    test_no_extend_with_args("Hello %s", "World");
    test_report("lsmStringVAppendf_noExtend_singleArg", g_test_pass_once);
    all_ok = all_ok && g_test_pass_once;

    // Test 2: Extension path (single-arg)
    g_test_pass_once = false;
    test_extend_with_args("%s", "abcdefghij"); // this string length 10 will require extension
    test_report("lsmStringVAppendf_extend_singleArg", g_test_pass_once);
    all_ok = all_ok && g_test_pass_once;

    // Test 3: Extension path (multi-arg)
    g_test_pass_once = false;
    test_two_args_with_args("%s-%s", "First", "Second"); // two args trigger ap2 usage
    test_report("lsmStringVAppendf_extend_twoArgs", g_test_pass_once);
    all_ok = all_ok && g_test_pass_once;

    if (all_ok) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
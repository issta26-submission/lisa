// Test suite for cre2_opt_encoding in cre2.cpp
// This file provides a small, self-contained test harness (no gtest required)
// to verify that cre2_opt_encoding correctly maps encoding settings in cre2_options_t
// to the corresponding cre2_encoding_t values.
// The tests cover UTF8, Latin1, and Unknown/default branches of the switch.
// The suite uses a lightweight assertion mechanism and runs from main().

#include <cre2.h>
#include <re2/re2.h>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the focal header that declares cre2_options_t and related APIs.
// The real project provides these declarations via cre2.h, cre2_opt_set_encoding, etc.

// Lightweight test harness (no external testing framework)
static int g_pass = 0;
static int g_fail = 0;

// Simple assertion macro (non-terminating): logs failure but continues execution
#define ASSERT_EQ_INT(expected, actual, msg) do { \
    if ((static_cast<int>(actual)) != (static_cast<int>(expected))) { \
        std::cerr << "ASSERT FAILED: " << msg \
                  << " | expected: " << static_cast<int>(expected) \
                  << ", actual: " << static_cast<int>(actual) << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while (0)


// Test 1: UTF8 encoding path
// - Set encoding to CRE2_UTF8 via cre2_opt_set_encoding
// - Ensure cre2_opt_encoding returns CRE2_UTF8
void test_cre2_opt_encoding_utf8() {
    // Create a new options object
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        std::cerr << "ERROR: cre2_opt_new() returned nullptr for UTF8 test." << std::endl;
        ++g_fail;
        return;
    }

    // Configure encoding to UTF8
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // Invoke the focal function
    cre2_encoding_t enc = cre2_opt_encoding(opt);

    // Validate the result
    ASSERT_EQ_INT((int)CRE2_UTF8, enc, "cre2_opt_encoding should return CRE2_UTF8 for EncodingUTF8");

    // Cleanup
    cre2_opt_delete(opt);
}


// Test 2: Latin1 encoding path
// - Set encoding to CRE2_Latin1 via cre2_opt_set_encoding
// - Ensure cre2_opt_encoding returns CRE2_Latin1
void test_cre2_opt_encoding_latin1() {
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        std::cerr << "ERROR: cre2_opt_new() returned nullptr for Latin1 test." << std::endl;
        ++g_fail;
        return;
    }

    // Configure encoding to Latin1
    cre2_opt_set_encoding(opt, CRE2_Latin1);

    cre2_encoding_t enc = cre2_opt_encoding(opt);

    ASSERT_EQ_INT((int)CRE2_Latin1, enc, "cre2_opt_encoding should return CRE2_Latin1 for EncodingLatin1");

    cre2_opt_delete(opt);
}


// Test 3: Unknown/default path
// - Set encoding to CRE2_UNKNOWN (or any value not handled by the specific cases)
// - Ensure cre2_opt_encoding falls back to CRE2_UNKNOWN
void test_cre2_opt_encoding_unknown() {
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        std::cerr << "ERROR: cre2_opt_new() returned nullptr for Unknown test." << std::endl;
        ++g_fail;
        return;
    }

    // Configure encoding to an unknown value
    cre2_opt_set_encoding(opt, CRE2_UNKNOWN);

    cre2_encoding_t enc = cre2_opt_encoding(opt);

    ASSERT_EQ_INT((int)CRE2_UNKNOWN, enc, "cre2_opt_encoding should return CRE2_UNKNOWN for unknown encoding");

    cre2_opt_delete(opt);
}


// Entrypoint for running tests
int main() {
    std::cout << "Running test suite for cre2_opt_encoding..." << std::endl;

    test_cre2_opt_encoding_utf8();
    test_cre2_opt_encoding_latin1();
    test_cre2_opt_encoding_unknown();

    std::cout << "Tests completed. Passed: " << g_pass << ", Failed: " << g_fail << std::endl;

    // Non-zero exit code if any test failed
    return (g_fail == 0) ? 0 : 1;
}
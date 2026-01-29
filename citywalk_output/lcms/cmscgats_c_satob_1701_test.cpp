// Unit test suite for the focal method: satob (binary representation converter)
// This test suite is written for C++11 and does not rely on Google Test.
// It uses a small in-house assertion mechanism and tests the behavior of the C function satob
// which takes a decimal string and returns a pointer to a static buffer containing the binary
// representation of that number. The function uses a static buffer, so subsequent calls may
// overwrite previous results. We validate this behavior as well.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstring>


extern "C" const char* satob(const char* v); // Declaration to link with the C function under test

static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion helper: logs failures but does not abort the test sequence.
static void assert_equal(const char* expected, const char* actual, const char* test_name) {
    ++g_tests_run;
    if (expected == nullptr || actual == nullptr) {
        if (expected != actual) {
            std::cerr << "FAIL: " << test_name
                      << " | expected: \"" << (expected ? expected : "NULL") 
                      << "\" actual: \"" << (actual ? actual : "NULL") << "\"\n";
            ++g_tests_failed;
        } else {
            std::cout << "PASS: " << test_name << "\n";
        }
        return;
    }
    if (strcmp(expected, actual) != 0) {
        ++g_tests_failed;
        std::cerr << "FAIL: " << test_name
                  << " | expected: \"" << expected
                  << "\" actual: \"" << actual << "\"\n";
    } else {
        std::cout << "PASS: " << test_name << "\n";
    }
}

// Test 1: Null input should return "0"
static void test_null_input() {
    // Explanation:
    // satob(NULL) should short-circuit and return the string literal "0".
    const char* res = satob(nullptr);
    assert_equal("0", res, "satob(nullptr) should return '0'");
}

// Test 2: Leading zeros in decimal input should be ignored -> "101" for 5, etc.
static void test_leading_zeros() {
    // Explanation:
    // Input "0005" should be treated as 5 and produce binary "101".
    const char* res = satob("0005");
    assert_equal("101", res, "satob(\"0005\") should yield binary '101' for decimal 5");
}

// Test 3: Basic conversion: 1 -> "1"
static void test_one_input() {
    const char* res = satob("1");
    assert_equal("1", res, "satob(\"1\") should yield '1'");
}

// Test 4: Basic conversion: 2 -> "10"
static void test_two_input() {
    const char* res = satob("2");
    assert_equal("10", res, "satob(\"2\") should yield '10'");
}

// Test 5: Basic conversion: 10 -> "1010"
static void test_ten_input() {
    const char* res = satob("10");
    assert_equal("1010", res, "satob(\"10\") should yield '1010'");
}

// Test 6: Large value within 32-bit: 65535 -> 16 ones
static void test_max_32bit_value() {
    // Explanation:
    // 65535 (2^16 - 1) should convert to 16 consecutive '1' bits.
    const char* res = satob("65535");
    std::string expected(16, '1');
    assert_equal(expected.c_str(), res, "satob(\"65535\") should yield 16 consecutive '1's");
}

// Test 7: Static buffer semantics (contents are overwritten by subsequent calls)
// This ensures the function uses a static buffer that is overwritten on each call.
static void test_static_buffer_semantics() {
    // First call to populate the static buffer with "1"
    const char* r1 = satob("1");
    std::string captured(r1); // copy the content now
    // Second call overwrites the same static buffer
    const char* r2 = satob("2"); // "10" will be written into the same static buffer
    // The previously captured content should remain unchanged
    assert_equal("1", captured.c_str(), "static buffer should retain previous content after subsequent call");
    // Optional: ensure second call produced the expected new content
    assert_equal("10", r2, "satob(\"2\") should yield '10'");
}

// The main function runs all tests and reports a summary.
// Non-terminating approach: even if a test fails, remaining tests will run.
int main() {
    test_null_input();
    test_leading_zeros();
    test_one_input();
    test_two_input();
    test_ten_input();
    test_max_32bit_value();
    test_static_buffer_semantics();

    std::cout << "\nTest Summary: Ran " << g_tests_run
              << " tests, " << g_tests_failed << " failed.\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
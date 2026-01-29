// Minimal C++11 unit test suite for the focal method:
// dag_device_description(const unsigned dagid)
// - The function returns a pointer to a static buffer containing the string
//   "alias for dag<dagid>:0" (formatted with %u).
// - The buffer is static and shared across calls; subsequent calls may overwrite it.
// - We test basic correctness, edge values, and static-buffer semantics via copies.
//
// Note: The function is exposed from C code, so we declare it with extern "C".

#include <string.h>
#include <errno.h>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <string>
#include <iostream>
#include <dagapi.h>


// Forward declaration of the C function under test.
// The function signature in the C file is:
// char *dag_device_description(const unsigned dagid);
extern "C" char* dag_device_description(unsigned dagid);

// Simple test harness (non-terminating assertions) to allow all tests to run.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_EXPECT_TRUE(cond) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Condition is false: " #cond "\n"; \
    } \
} while(0)

#define TEST_EXPECT_EQ(actual, expected) do { \
    ++g_tests_run; \
    if(!((actual) == (expected))) { \
        ++g_tests_failed; \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " << (expected) << " | Actual: " << (actual) << "\n"; \
    } \
} while(0)

#define TEST_EXPECT_STREQ(actual_cstr, expected_cstr) do { \
    ++g_tests_run; \
    if(std::string((actual_cstr)) != std::string((expected_cstr))) { \
        ++g_tests_failed; \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " << (expected_cstr) << " | Actual: " << (actual_cstr) << "\n"; \
    } \
} while(0)

// Test 1: Basic correctness and strings for two different dagids.
// This verifies that the function returns a string of the form
// "alias for dag<dagid>:0" and that using copies preserves the content
// even though the underlying buffer is static.
void test_dag_device_description_basic() {
    // Call with two different IDs and capture via std::string to own the data.
    const char* r1 = dag_device_description(1u);
    const char* r2 = dag_device_description(2u);

    std::string s1(r1);
    std::string s2(r2);

    // Expected values
    TEST_EXPECT_EQ(s1, std::string("alias for dag1:0"));
    TEST_EXPECT_EQ(s2, std::string("alias for dag2:0"));

    // Call again and verify content remains consistent for same dagid
    const char* r1b = dag_device_description(1u);
    const char* r2b = dag_device_description(2u);
    std::string s1b(r1b);
    std::string s2b(r2b);
    TEST_EXPECT_EQ(s1b, std::string("alias for dag1:0"));
    TEST_EXPECT_EQ(s2b, std::string("alias for dag2:0"));

    // Ensure that the first captured content is still a valid copy
    // (i.e., static buffer overwritten by subsequent calls should not affect s1)
    TEST_EXPECT_EQ(s1, std::string("alias for dag1:0"));

    // s1 and s2 should be different since dagid values differ
    TEST_EXPECT_TRUE(s1 != s2);
}

// Test 2: Edge values for dagid to ensure formatting handles boundaries correctly.
// Tests dagid = 0 and dagid = 4294967295 (max 32-bit unsigned).
void test_dag_device_description_edge_values() {
    const char* p0 = dag_device_description(0u);
    std::string s0(p0);
    TEST_EXPECT_EQ(s0, std::string("alias for dag0:0"));

    const char* pmax = dag_device_description(4294967295u);
    std::string smax(pmax);
    TEST_EXPECT_EQ(smax, std::string("alias for dag4294967295:0"));
}

// Test 3: Basic non-nullness and content characteristics.
// Verifies the function returns a non-null pointer and that the content
// starts with the expected prefix and ends with the expected suffix.
void test_dag_device_description_non_null_and_content_shape() {
    const char* p = dag_device_description(123u);
    TEST_EXPECT_TRUE(p != nullptr);

    std::string s(p);
    // Content should start with "alias for dag" and end with ":0"
    TEST_EXPECT_TRUE(s.find("alias for dag") == 0);
    TEST_EXPECT_TRUE(!s.empty() && s.back() == '0');
}

// Test 4: Sanity check on buffer size usage (string length should fit in 128 chars).
// This ensures there is no buffer overflow for typical dagid values.
void test_dag_device_description_buffer_size() {
    // A reasonably large dagid value to exercise formatting.
    const char* p = dag_device_description(123456u);
    std::string s(p);
    TEST_EXPECT_TRUE(s.size() < 128);
}

// Entry point: run all tests and report summary.
int main() {
    // Explanatory comments for test goals
    // - Basic correctness: ensure proper formatting for small IDs
    // - Edge values: 0 and max unsigned to validate boundaries
    // - Non-null and shape: ensure string pattern and non-null return
    // - Buffer size: ensure no overflow for typical inputs

    test_dag_device_description_basic();
    test_dag_device_description_edge_values();
    test_dag_device_description_non_null_and_content_shape();
    test_dag_device_description_buffer_size();

    std::cout << "Unit tests executed: " << g_tests_run
              << ", Failures: " << g_tests_failed << std::endl;

    // Non-terminating: return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}
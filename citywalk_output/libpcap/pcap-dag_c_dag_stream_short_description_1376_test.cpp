// Unit test suite for dag_stream_short_description (C function in pcap-dag.c)
// This test is written in C++11 without any external testing framework (GTest not allowed).
// It uses a small, self-contained test harness with non-terminating assertions to maximize coverage.
// The tests focus on the observable formatting behavior of dag_stream_short_description.

#include <string.h>
#include <errno.h>
#include <cstdlib>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <cstdio>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <dagapi.h>


// Declare the focal function with C linkage to avoid name mangling when linking against pcap-dag.c
extern "C" const char* dag_stream_short_description(const unsigned stream);

/*
  Minimal test framework (non-terminating assertions):
  - Tracks global failures and reports a summary at the end.
  - Provides lightweight EXPECT_* macros suitable for C/C++ cross-language tests.
*/
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if((cond)) { \
        fprintf(stderr, "EXPECT_FALSE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(actual, expected) do { \
    if((actual) != (expected)) { \
        fprintf(stderr, "EXPECT_EQ failed: %s:%d: %s == %s (actual=%llu, expected=%llu)\n", \
                __FILE__, __LINE__, #actual, #expected, (unsigned long long)(actual), (unsigned long long)(expected)); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STR_CONTAINS(str, substr) do { \
    if((str) == nullptr || strstr((str), (substr)) == nullptr) { \
        fprintf(stderr, "EXPECT_STR_CONTAINS failed: %s:%d: string does not contain \"%s\" (actual=\"%s\")\n", \
                __FILE__, __LINE__, (substr), (str)); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STR_NOT_NULL(str) do { \
    if((str) == nullptr) { \
        fprintf(stderr, "EXPECT_STR_NOT_NULL failed: %s:%d: pointer is NULL\n", __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)


// Test 1: Basic formatting correctness and numeric parsing
// - Verifies the string contains the expected format: "<prefix> stream <number>"
// - Ensures the numeric portion matches the input stream value
static void test_basic_format_and_parsing() {
    // Test with a variety of representative unsigned values, including boundary
    const unsigned test_values[] = {0u, 1u, 10u, 12345u, 4294967295u};
    const size_t num_tests = sizeof(test_values) / sizeof(test_values[0]);

    for(size_t i = 0; i < num_tests; ++i) {
        unsigned val = test_values[i];
        const char* desc = dag_stream_short_description(val);

        // Basic non-null check
        EXPECT_STR_NOT_NULL(desc);

        // The function uses a static buffer, so the same memory should be returned across calls
        // We'll not rely on that here, just ensure the string is well-formed.

        // The string must contain the " stream " literal as per the format "%s stream %u"
        EXPECT_STR_CONTAINS(desc, " stream ");

        // Extract the numeric part after " stream "
        unsigned parsed = 0;
        int ret = sscanf(desc, "%*s stream %u", &parsed);
        EXPECT_TRUE(ret == 1);
        EXPECT_EQ(parsed, val);
    }
}

// Test 2: Static buffer semantics
// - The function returns a pointer to a static buffer; successive calls should yield the same pointer.
// - This ensures memory is reused (not reallocated) across calls.
static void test_static_buffer_identity() {
    const unsigned val1 = 5;
    const unsigned val2 = 6;

    const char* s1 = dag_stream_short_description(val1);
    const char* s2 = dag_stream_short_description(val2);

    // The internal buffer is static; both calls should return the same address
    EXPECT_TRUE(s1 == s2);

    // Additionally, ensure the content remained valid and parseable for the second call
    EXPECT_STR_CONTAINS(s2, " stream ");
    unsigned parsed = 0;
    int ret = sscanf(s2, "%*s stream %u", &parsed);
    EXPECT_TRUE(ret == 1);
    EXPECT_EQ(parsed, val2);
}

// Test 3: Boundary and robustness check for large unsigned values
// - Ensure the function can format the maximum unsigned value without crashing or corrupting memory
static void test_large_value_boundary() {
    const unsigned max_uint = 4294967295u;
    const char* desc = dag_stream_short_description(max_uint);

    EXPECT_STR_NOT_NULL(desc);
    EXPECT_STR_CONTAINS(desc, " stream ");

    unsigned parsed = 0;
    int ret = sscanf(desc, "%*s stream %u", &parsed);
    EXPECT_TRUE(ret == 1);
    EXPECT_EQ(parsed, max_uint);
}

// Runner to execute tests and report summary
static void run_all_tests() {
    fprintf(stdout, "Running tests for dag_stream_short_description...\n");

    test_basic_format_and_parsing();
    test_static_buffer_identity();
    test_large_value_boundary();

    if(g_failures == 0) {
        fprintf(stdout, "All tests PASSED for dag_stream_short_description.\n");
    } else {
        fprintf(stderr, "Tests FAILED: %d failure(s) detected for dag_stream_short_description.\n", g_failures);
    }

    // Exit code 0 on success, 1 on failure
}

// Main entry point
int main() {
    run_all_tests();
    return (g_failures == 0) ? 0 : 1;
}
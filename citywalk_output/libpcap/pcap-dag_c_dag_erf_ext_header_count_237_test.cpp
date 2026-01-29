// This test suite targets the focal method:
//   dag_erf_ext_header_count(const uint8_t *erf, size_t len)
// from pcap-dag.c. The tests are written in C++11 without GoogleTest.
// A tiny in-house assertion harness is used to keep tests non-terminating
// and to maximize code coverage across all branches.
// The tests rely on ERF_TYPE_MORE_EXT macro semantics provided by the
// project's headers (e.g., dagapi.h). We declare the function with C linkage
// to call into the C implementation from C++.

#include <string.h>
#include <errno.h>
#include <cstdint>
#include <endian.h>
#include <dagpci.h>
#include <cstddef>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <dagapi.h>


// Forward declaration of the C function under test.
// The actual macro ERF_TYPE_MORE_EXT is consumed inside the C implementation;
// we rely on the project's headers to define it consistently at compile time.
extern "C" uint32_t dag_erf_ext_header_count(const uint8_t *erf, size_t len);

// Simple in-house assertion helpers (non-terminating)
static int g_test_failures = 0;

static void ASSERT_TRUE(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "ASSERT_TRUE failed: " << msg << std::endl;
        ++g_test_failures;
    }
}

static void ASSERT_EQ(uint32_t a, uint32_t b, const char* msg) {
    if (a != b) {
        std::cerr << "ASSERT_EQ failed: " << msg
                  << " | expected: " << a << " , actual: " << b << std::endl;
        ++g_test_failures;
    }
}

// Candidate Keywords discovered from Step 1:
// - dag_erf_ext_header_count
// - ERF_TYPE_MORE_EXT
// - erf array layout (offsets 8, 16, 24, 32...)
// - early NULL check, len < 16 check
// - loop condition that uses hdr_type and ERF_TYPE_MORE_EXT
// These keywords guide test coverage for true/false branches.


// Test 1: erf pointer is NULL -> should return 0
static void test_null_erf_returns_zero() {
    uint32_t result = dag_erf_ext_header_count(nullptr, 100);
    ASSERT_EQ(result, 0, "Null erf pointer should return 0");
}

// Test 2: len smaller than 16 -> should return 0 (basic sanity)
static void test_len_less_than_min_returns_zero() {
    uint8_t erf[8] = {0}; // non-null buffer but too short
    uint32_t result = dag_erf_ext_header_count(erf, 8);
    ASSERT_EQ(result, 0, "len < 16 should return 0");
}

// Test 3: erf[8] does not indicate more extension headers -> should return 0
// We set erf[8] to 0 so the condition (! (erf[8] & ERF_TYPE_MORE_EXT)) is true.
static void test_more_ext_not_set_returns_zero() {
    uint8_t erf[32] = {0};
    erf[8] = 0x00;     // ensure the MORE_EXT flag is not set
    // len is >=16 but < 24 to exercise early exit before headers are parsed
    uint32_t result = dag_erf_ext_header_count(erf, 20);
    ASSERT_EQ(result, 0, "erf[8] not set should return 0");
}

// Test 4: Single header with no "more extensions" after it (hdr_type has no MORE_EXT bit)
// We ensure erf[8] has the bit (by setting a non-zero value) and set erf[16] = 0.
static void test_single_header_no_more_ext_returns_one() {
    uint8_t erf[24] = {0};
    erf[8] = 0xFF;  // make sure there is at least one extension header present
    erf[16] = 0x00;  // first header type does not indicate more extensions
    uint32_t result = dag_erf_ext_header_count(erf, 24);
    ASSERT_EQ(result, 1, "Single header with no MORE_EXT should yield count 1");
}

// Test 5: Two headers where first header has MORE_EXT and second header ends (returns 2)
static void test_two_headers_returns_two() {
    uint8_t erf[40] = {0};
    erf[8] = 0xFF;     // ensure there is at least one extension header
    erf[16] = 0xFF;    // first header type indicates MORE_EXT (non-zero when ANDed with mask)
    erf[24] = 0x00;    // second header type ends the chain
    uint32_t result = dag_erf_ext_header_count(erf, 32);
    ASSERT_EQ(result, 2, "Two headers: first with MORE_EXT, second ends -> count 2");
}

// Test 6: Three headers in a chain (first two with MORE_EXT, third ends) -> 3
static void test_three_headers_returns_three() {
    uint8_t erf[48] = {0};
    erf[8]  = 0xFF;    // first header exists
    erf[16] = 0xFF;    // first header has MORE_EXT
    erf[24] = 0xFF;    // second header has MORE_EXT
    erf[32] = 0x00;    // third header ends the chain
    uint32_t result = dag_erf_ext_header_count(erf, 40);
    ASSERT_EQ(result, 3, "Three headers in chain -> count 3");
}

// Test 7: Mid-loop length check prevents reading next header (returns 1)
static void test_mid_loop_length_cut_returns_one() {
    uint8_t erf[31] = {0};
    erf[8]  = 0xFF;   // there is an extension header
    erf[16] = 0xFF;   // first header indicates MORE_EXT
    // len = 31 < 32, so after reading first header, there isn't enough data
    uint32_t result = dag_erf_ext_header_count(erf, 31);
    ASSERT_EQ(result, 1, "Mid-loop length check should return 1");
}

// Runner that executes all tests and reports summary
static void run_all_tests() {
    test_null_erf_returns_zero();
    test_len_less_than_min_returns_zero();
    test_more_ext_not_set_returns_zero();
    test_single_header_no_more_ext_returns_one();
    test_two_headers_returns_two();
    test_three_headers_returns_three();
    test_mid_loop_length_cut_returns_one();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
    }
}

int main() {
    run_all_tests();
    return g_test_failures == 0 ? 0 : 1;
}
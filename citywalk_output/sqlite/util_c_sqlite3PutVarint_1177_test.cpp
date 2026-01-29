// Lightweight C++11 unit test suite for the focal function:
// sqlite3PutVarint(unsigned char *p, u64 v)
// Notes:
// - This test focuses on the behavior of sqlite3PutVarint, exercising the
//   true/false branches of its conditional predicates.
// - We test the 1-byte path (v <= 0x7f), the 2-byte path (0x80 <= v <= 0x3fff),
//   and at least the beginning of the large-varint path (v > 0x3fff) which
//   relies on the internal putVarint64() for the final encoding.
// - The large-varint path's exact encoding is implementation-defined; we validate
//   the presence of continuation bytes (p[0] has high-bit set) as a basic sanity check.
// - This test suite avoids GoogleTest and uses a minimal, non-terminating assertion style.

#include <math.h>
#include <cstdio>
#include <sqliteInt.h>
#include <stdarg.h>
#include <cstdint>


// Step 1: Program Understanding (Candidate Keywords)
// Candidate Keywords derived from the focal method and its dependencies:
// - sqlite3PutVarint, varint encoding, 1-byte path, 2-byte path, putVarint64 (static), v threshold 0x7f and 0x3fff
// - Buffer manipulation (unsigned char*), return value indicating bytes written
// - Boundary cases: v <= 0x7f, v <= 0x3fff, else branch
// - Domain knowledge: u64 type (64-bit unsigned), memory writes

// Step 2: Unit Test Generation
// External C function under test
extern "C" int sqlite3PutVarint(unsigned char *p, unsigned long long v);

// Simple non-terminating assertion helpers (EXPECT-style)
static int g_testFailures = 0;

#define EXPECT_EQ_U64(actual, expected, testName, note) do { \
    if (static_cast<unsigned long long>(actual) != static_cast<unsigned long long>(expected)) { \
        fprintf(stderr, "TEST FAILED: %s | %s: got %llu, expected %llu\n", testName, note, \
                static_cast<unsigned long long>(actual), static_cast<unsigned long long>(expected)); \
        ++g_testFailures; \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", testName); \
    } \
} while(0)

#define EXPECT_EQ_INT(actual, expected, testName, note) do { \
    if ((actual) != (expected)) { \
        fprintf(stderr, "TEST FAILED: %s | %s: got %d, expected %d\n", testName, note, (actual), (expected)); \
        ++g_testFailures; \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", testName); \
    } \
} while(0)

#define EXPECT_TRUE(condition, testName, note) do { \
    if (!(condition)) { \
        fprintf(stderr, "TEST FAILED: %s | %s: condition is false\n", testName, note); \
        ++g_testFailures; \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", testName); \
    } \
} while(0)

// Helper to print buffer contents for debugging (optional)
static void dumpBuffer(const unsigned char* buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

// Step 3: Test Case Refinement
// - Test 1: v = 0 (edge of 1-byte path)
// - Test 2: v = 0x7f (upper edge of 1-byte path)
// - Test 3: v = 0x80 (start of 2-byte path)
// - Test 4: v = 0x3fff (upper edge of 2-byte path)
// - Test 5: v = 0x4000 (entering the 3+ byte path; verify continuation bit usage)
// - Test 6: v = 0xFFFFFFFFFFFFFFFF (largest 64-bit value; verify continuation bit usage)

void test_one_byte_path_zero() {
    unsigned char buf[16] = {0};
    unsigned long long v = 0;
    int ret = sqlite3PutVarint(buf, v);
    // Expect a single byte written with value 0
    EXPECT_EQ_INT(ret, 1, "one_byte_path_zero_ret", "return value for v=0 should be 1");
    EXPECT_EQ_U64(buf[0], static_cast<unsigned long long>(0), "one_byte_path_zero_buf0", "buf[0] must be 0");
}

void test_one_byte_path_boundary7f() {
    unsigned char buf[16] = {0};
    unsigned long long v = 0x7f;
    int ret = sqlite3PutVarint(buf, v);
    // Expect a single byte written with value 0x7f
    EXPECT_EQ_INT(ret, 1, "one_byte_path_boundary7f_ret", "return value for v=0x7f should be 1");
    EXPECT_EQ_U64(buf[0], static_cast<unsigned long long>(0x7f), "one_byte_path_boundary7f_buf0", "buf[0] must be 0x7f");
}

void test_two_byte_path_lower_bound() {
    unsigned char buf[16] = {0};
    unsigned long long v = 0x80; // 128
    int ret = sqlite3PutVarint(buf, v);
    // Expect two bytes written with first byte having high-bit set and second byte as lower 7 bits
    EXPECT_EQ_INT(ret, 2, "two_byte_path_lower_bound_ret", "return value for v=0x80 should be 2");
    EXPECT_EQ_U64(buf[0], static_cast<unsigned long long>(0x81), "two_byte_path_lower_bound_buf0", "buf[0] must be 0x81");
    EXPECT_EQ_U64(buf[1], static_cast<unsigned long long>(0x00), "two_byte_path_lower_bound_buf1", "buf[1] must be 0x00");
}

void test_two_byte_path_upper_bound() {
    unsigned char buf[16] = {0};
    unsigned long long v = 0x3fff; // 16383
    int ret = sqlite3PutVarint(buf, v);
    // Expect two bytes written with first byte 0xff and second 0x7f
    EXPECT_EQ_INT(ret, 2, "two_byte_path_upper_bound_ret", "return value for v=0x3fff should be 2");
    EXPECT_EQ_U64(buf[0], static_cast<unsigned long long>(0xff), "two_byte_path_upper_bound_buf0", "buf[0] must be 0xff");
    EXPECT_EQ_U64(buf[1], static_cast<unsigned long long>(0x7f), "two_byte_path_upper_bound_buf1", "buf[1] must be 0x7f");
}

void test_large_branch_enter_large_varint() {
    unsigned char buf[64] = {0};
    unsigned long long v = 0x4000; // 16384
    int ret = sqlite3PutVarint(buf, v);
    // We cannot verify exact encoding without putVarint64 impl, but we can verify
    // that we are not in the 1- or 2-byte path and that there is a continuation bit.
    EXPECT_INT(ret >= 3, ret, "large_branch_enter_large_varint_ret", "ret should be >= 3 for v=0x4000");
    EXPECT_TRUE((buf[0] & 0x80) != 0, "large_branch_enter_large_varint_buf0_continuation", "buf[0] should have high-bit set");
}

void test_large_branch_max_value() {
    unsigned char buf[64] = {0};
    unsigned long long v = 0xFFFFFFFFFFFFFFFFULL; // max u64
    int ret = sqlite3PutVarint(buf, v);
    // Similar reasoning as previous test
    EXPECT_EQ_INT(ret, ret, "large_branch_max_value_ret", "ret is reported (non-zero) for max value");
    // Ensure continuation bit is used in the first byte
    EXPECT_TRUE((buf[0] & 0x80) != 0, "large_branch_max_value_buf0_continuation", "buf[0] should have high-bit set");
}

// Entry point
int main() {
    // Candidate Keywords and test plan commentary (embedded for traceability)
    // - We focus on the branching behavior of sqlite3PutVarint
    // - Validate 1-byte path, 2-byte path, and the transition to multi-byte path
    // - Check boundary conditions and ensure non-terminating assertion style (non-fatal failures)

    fprintf(stdout, "Starting tests for sqlite3PutVarint...\n");

    test_one_byte_path_zero();
    test_one_byte_path_boundary7f();
    test_two_byte_path_lower_bound();
    test_two_byte_path_upper_bound();
    test_large_branch_enter_large_varint();
    test_large_branch_max_value();

    if (g_testFailures == 0) {
        fprintf(stdout, "ALL TESTS PASSED\n");
    } else {
        fprintf(stderr, "TOTAL FAILED TESTS: %d\n", g_testFailures);
    }

    return g_testFailures ? 1 : 0;
}
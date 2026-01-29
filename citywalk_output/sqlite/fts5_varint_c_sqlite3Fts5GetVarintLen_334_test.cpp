// C++11-compatible unit test suite for sqlite3Fts5GetVarintLen
// This test suite does not use Google Test. It uses a small custom test harness
// and calls the C function defined in fts5_varint.c (extern "C" linkage).

#include <fts5Int.h>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal function, defined in the C file fts5_varint.c
// The function takes an unsigned 32-bit value and returns the encoded varint length.
extern "C" int sqlite3Fts5GetVarintLen(unsigned int iVal);

// Simple test harness global counters
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Failure logging helper
static void fail_msg(const char* test, const char* file, int line, const std::string& msg){
    ++g_tests_failed;
    std::cerr << "FAIL: " << test << " (" << file << ":" << line << ") - " << msg << std::endl;
}

// Lightweight assertion macro: compares values and reports failures without aborting tests
#define EXPECT_EQ(expected, actual) do { \
    if((expected) != (actual)) { \
        std::ostringstream oss; \
        oss << "Expected " << (expected) << " but got " << (actual); \
        fail_msg(__func__, __FILE__, __LINE__, oss.str()); \
    } \
} while(0)


// 1) Boundary test: iVal = 128 (1 << 7) should yield length 2
static void test_len_128_returns_2() {
    unsigned int v = 128u;
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(2, r);
}

// 2) Upper boundary of the 2-byte range: iVal = 16383 should yield length 2
static void test_len_16383_returns_2() {
    unsigned int v = 16383u; // (1 << 14) - 1
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(2, r);
}

// 3) Crossing to 3-byte range: iVal = 16384 should yield length 3
static void test_len_16384_returns_3() {
    unsigned int v = 16384u; // (1 << 14)
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(3, r);
}

// 4) Upper boundary of the 3-byte range: iVal = 2097151 should yield length 3
static void test_len_2097151_returns_3() {
    unsigned int v = 2097151u; // (1 << 21) - 1
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(3, r);
}

// 5) Crossing to 4-byte range: iVal = 2097152 should yield length 4
static void test_len_2097152_returns_4() {
    unsigned int v = 2097152u; // (1 << 21)
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(4, r);
}

// 6) Upper boundary of the 4-byte range: iVal = 268435455 should yield length 4
static void test_len_268435455_returns_4() {
    unsigned int v = 268435455u; // (1 << 28) - 1
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(4, r);
}

// 7) Crossing to 5-byte range: iVal = 268435456 should yield length 5
static void test_len_268435456_returns_5() {
    unsigned int v = 268435456u; // (1 << 28)
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(5, r);
}

// 8) Large 32-bit value should still yield length 5
static void test_len_max32bit_returns_5() {
    unsigned int v = 0xFFFFFFFFu;
    int r = sqlite3Fts5GetVarintLen(v);
    EXPECT_EQ(5, r);
}

// Helper to run a test and print a simple header
#define RUN_TEST(name) do { \
    std::cout << "[ RUN      ] " << #name << std::endl; \
    name(); \
    ++g_tests_run; \
} while(0)


int main() {
    // Execute tests covering all branches and boundary conditions
    // Note: The function asserts only for iVal < (1<<7). All test values here satisfy iVal >= 128.
    RUN_TEST(test_len_128_returns_2);
    RUN_TEST(test_len_16383_returns_2);
    RUN_TEST(test_len_16384_returns_3);
    RUN_TEST(test_len_2097151_returns_3);
    RUN_TEST(test_len_2097152_returns_4);
    RUN_TEST(test_len_268435455_returns_4);
    RUN_TEST(test_len_268435456_returns_5);
    RUN_TEST(test_len_max32bit_returns_5);

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    return (g_tests_failed != 0) ? 1 : 0;
}
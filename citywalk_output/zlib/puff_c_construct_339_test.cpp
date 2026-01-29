// C++11 unit test suite for the focal method "construct" in puff.c
// Notes:
// - We treat puff.h as the public C API and attempt to access the non-static focal function
//   "construct" via an extern "C" declaration. This suite relies on the build linking puff.c
//   with the test executable.
// - Tests are written without GoogleTest and use a lightweight custom test harness that
//   records failures but does not terminate on first failure (non-terminating assertions).
// - Tests exercise key branches and behaviors described by the focal method's logic.

#include <cstring>
#include <setjmp.h>
#include <string>
#include <iostream>
#include <vector>
#include <puff.h>


// Bring C declarations into C++ using C linkage.
extern "C" {
#define HUFFMAN_API
}

// If the actual header does not provide a prototype for construct with C linkage,
// declare it explicitly here to enable direct testing of the focal function.
// We assume the function signature matches: int construct(struct huffman *h, const short *length, int n);
extern "C" int construct(struct huffman *h, const short *length, int n);

// Lightweight test harness (non-terminating assertions)
namespace TestHarness {
    static int g_failures = 0;
    std::vector<std::string> g_messages;

    void log_failure(const std::string &msg) {
        ++g_failures;
        g_messages.push_back("Failure: " + msg);
        std::cerr << "Failure: " << msg << std::endl;
    }

    void expect_true(bool cond, const std::string &msg) {
        if (!cond) log_failure(msg);
    }

    void expect_eq(int a, int b, const std::string &msg) {
        if (a != b) {
            log_failure(msg + " | expected: " + std::to_string(b) + ", but got: " + std::to_string(a));
        }
    }

    void summary(const std::string &suite_name) {
        std::cout << "Suite: " << suite_name << " -> ";
        if (g_failures == 0) {
            std::cout << "PASSED (" << g_failures << " failures)" << std::endl;
        } else {
            std::cout << "FAILED with " << g_failures << " failures" << std::endl;
        }
    }

    void reset() {
        g_failures = 0;
        g_messages.clear();
    }
}

// Test 1: All lengths are zero -> expect return 0 (no codes)
void test_construct_all_zero_lengths() {
    // Arrange
    struct huffman h;
    std::memset(&h, 0, sizeof(h)); // Initialize Huffman table state
    const short length[] = {0, 0, 0, 0}; // n == 4
    int n = 4;

    // Act
    int ret = construct(&h, length, n);

    // Assert
    TestHarness::expect_true(ret == 0,
        "construct should return 0 when all lengths are zero (no codes).");
    // No need to inspect h.symbol or h.count since all codes are zero-length.
}

// Test 2: Over-subscribed set of lengths -> expect negative return value
// Example: 3 symbols all with length 1 => left becomes 2 - 3 = -1
void test_construct_over_subscribed_returns_negative() {
    // Arrange
    struct huffman h;
    std::memset(&h, 0, sizeof(h));
    const short length[] = {1, 1, 1}; // n == 3
    int n = 3;

    // Act
    int ret = construct(&h, length, n);

    // Assert
    TestHarness::expect_true(ret < 0,
        "construct should return negative value for an over-subscribed lengths set.");
}

// Test 3: Normal, non-negative path with unique lengths, ensures proper ordering
// Example: lengths {1, 2, 3} for symbols 0,1,2 -> expect ret == 1 and symbol order [0,1,2]
void test_construct_normal_case_unique_lengths() {
    // Arrange
    struct huffman h;
    std::memset(&h, 0, sizeof(h));
    const short length[] = {1, 2, 3}; // n == 3
    int n = 3;

    // Act
    int ret = construct(&h, length, n);

    // Assert
    TestHarness::expect_true(ret == 1,
        "construct should return positive 'left' value for valid lengths (ret == 1).");

    // Verify counts on selected lengths
    TestHarness::expect_eq(h.count[1], 1, "count[1] should be 1 for length 1");
    TestHarness::expect_eq(h.count[2], 1, "count[2] should be 1 for length 2");
    TestHarness::expect_eq(h.count[3], 1, "count[3] should be 1 for length 3");

    // Verify symbol sorting by length: symbols should be placed in increasing symbol order
    // offs[1] = 0; offs[2] = count[1] = 1; offs[3] = offs[2] + count[2] = 2
    // h.symbol[0] -> symbol with length 1 => 0
    // h.symbol[1] -> symbol with length 2 => 1
    // h.symbol[2] -> symbol with length 3 => 2
    TestHarness::expect_eq(h.symbol[0], 0, "First symbol should be 0 (length 1)");
    TestHarness::expect_eq(h.symbol[1], 1, "Second symbol should be 1 (length 2)");
    TestHarness::expect_eq(h.symbol[2], 2, "Third symbol should be 2 (length 3)");
}

// Test 4: Normal path with all codes of the same length (e.g., all length 2)
// Expect ret == 0 (incomplete/zero leftover) and symbol order [0,1,2,3]
void test_construct_normal_case_all_same_length() {
    // Arrange
    struct huffman h;
    std::memset(&h, 0, sizeof(h));
    const short length[] = {2, 2, 2, 2}; // n == 4
    int n = 4;

    // Act
    int ret = construct(&h, length, n);

    // Assert
    // For this case, left becomes 0 after processing len=2 (2^1 - 4 = 0) and remains 0
    TestHarness::expect_true(ret == 0,
        "construct should return 0 when all symbols share the same length and fit exactly (left == 0).");

    // Verify h.count[2] equals 4
    TestHarness::expect_eq(h.count[2], 4, "count[2] should be 4 when all lengths are 2");

    // Verify symbol sorting: all symbols placed at offs[2], in ascending symbol order
    // offs[1] = 0; offs[2] = 0; After placing four symbols, h.symbol[0..3] should be 0,1,2,3
    TestHarness::expect_eq(h.symbol[0], 0, "First symbol should be 0 when all lengths are equal to 2");
    TestHarness::expect_eq(h.symbol[1], 1, "Second symbol should be 1 when all lengths are equal to 2");
    TestHarness::expect_eq(h.symbol[2], 2, "Third symbol should be 2 when all lengths are equal to 2");
    TestHarness::expect_eq(h.symbol[3], 3, "Fourth symbol should be 3 when all lengths are equal to 2");
}

// Runner that collects and reports results from all tests
void run_all_tests() {
    using namespace TestHarness;
    reset();

    test_construct_all_zero_lengths();
    test_construct_over_subscribed_returns_negative();
    test_construct_normal_case_unique_lengths();
    test_construct_normal_case_all_same_length();

    summary("puff.c construct() focal function tests");
}

// Entry point
int main() {
    run_all_tests();
    return 0;
}
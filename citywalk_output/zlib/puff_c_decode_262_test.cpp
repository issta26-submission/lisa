// C++11 test suite for the decode function in puff.c
// This test harness does not rely on Google Test. It provides a lightweight,
// non-terminating assertion mechanism and a small set of targeted tests to
// exercise the decode path, including: a successful symbol decode, a longjmp
// (input exhaustion) path, and a path that returns -10 (ran out of codes).
//
// Assumptions:
// - The project exposes the C interface via puff.h and the function
//   int decode(struct state *s, const struct huffman *h);
// - The decode function is accessible (non-static) in the linked/present build.
// - puff.h defines the necessary structures: struct state, struct huffman, MAXBITS, etc.
// - This test is compiled as a C++11 program and linked with puff.c (or the
//   equivalent C translation unit in the project).

#include <cstring>
#include <cstdio>
#include <setjmp.h>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <puff.h>
#include <sstream>


extern "C" {
}

// Lightweight non-terminating test harness
static std::vector<std::string> g_failures;

static void log_failure(const std::string &msg) {
    g_failures.push_back(msg);
}

// Helper to report test results
static void report_results(const std::string &test_name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
        for (const auto &m : g_failures) {
            std::cout << "       " << m << "\n";
        }
        // Clear after reporting a failure to keep per-test state clean
        g_failures.clear();
    }
}

// Expectation helper for ints
static void expect_eq_int(int a, int b, const std::string &desc) {
    if (a != b) {
        std::ostringstream oss;
        oss << desc << " - expected: " << b << ", got: " << a;
        log_failure(oss.str());
    }
}

// Test 1: Basic successful decode of a single symbol (len == 1) path
// Setup:
// - h->count[1] = 1 (one code of length 1), h->symbol[0] is the desired symbol.
// - All other lengths have count 0.
// - s->bitbuf has one bit (we'll supply 0 via bitbuf & 1) to trigger the early return.
// - After decode, verify returned symbol and that bitbuf/bitcnt have been updated.
static void test_decode_symbol_len1_returns_symbol() {
    // Prepare Huffman table: h->count[1] = 1; h->count[2] and onwards unused (0)
    short counts[4] = {0, 1, 0, 0}; // index 0 unused; index 1 -> len=1
    short symbols[1] = { 123 };     // the symbol to be returned

    struct huffman h;
    h.count = counts;
    h.symbol = symbols;

    struct state s;
    std::memset(&s, 0, sizeof(s));
    // Initialize bit stream state to produce the simplest path:
    // - bitbuf has at least one bit to read; the LSB is 0 (bit 0)
    // - bitcnt sufficiently large to cover the first code (we'll consume 1 bit)
    s.bitbuf = 0;
    s.bitcnt = 8;   // allow one iteration of the inner loop
    s.in = nullptr;
    s.inlen = 0;
    s.incnt = 0;
    // s.env is not used since we do not hit longjmp in this path

    int ret = decode(&s, &h);

    expect_eq_int(ret, 123, "test_decode_symbol_len1_returns_symbol: returns symbol 123");
    // After consuming one bit (len == 1), bitcnt should be reduced by 1 and masked to 7
    expect_eq_int(s.bitcnt, (8 - 1) & 7, "test_decode_symbol_len1_returns_symbol: bitcnt updated to 7");
    // bitbuf should have shifted right by 1 (since we consumed 1 bit)
    expect_eq_int(s.bitbuf, 0, "test_decode_symbol_len1_returns_symbol: bitbuf shifted (remains 0)");
}

// Test 2: Long jump path when input is exhausted mid-decode
// We intentionally force a longjmp by configuring the environment so that
// the code path hits "if (s->incnt == s->inlen) longjmp(s->env, 1);".
// We'll use setjmp/longjmp to catch the non-local jump and verify it occurred.
static void test_decode_longjmp_input_exhaustion() {
    // Prepare Huffman such that the inner loop will not return a symbol
    // Set all counts for lengths >=1 to 0 to avoid early returns
    short counts[32] = {0};
    short dummySymbol[1] = {0};

    struct huffman h;
    h.count = counts;
    h.symbol = dummySymbol;

    struct state s;
    std::memset(&s, 0, sizeof(s));

    // Prepare a jump buffer environment to catch the longjmp
    jmp_buf test_env;
    // We'll copy this environment into s.env after setjmp
    if (setjmp(test_env) == 0) {
        // Copy the environment into the state's env
        // Note: s.env is assumed to be compatible with jmp_buf storage
        std::memcpy(s.env, test_env, sizeof(jmp_buf));

        // Configure the state so that the decoder attempts to fetch more input
        // without any input available, triggering the longjmp when it checks
        // s->incnt == s->inlen (both zero here).
        s.bitbuf = 0;
        s.bitcnt = 0;     // ensures the outer loop will try to fetch next bits and hit the longjmp
        s.in = nullptr;
        s.inlen = 0;
        s.incnt = 0;

        // This call should trigger the longjmp back to test_env
        (void)decode(&s, &h);

        // If we return here, the longjmp did not occur as expected
        log_failure("test_decode_longjmp_input_exhaustion: decoded returned normally, expected longjmp");
        report_results("test_decode_longjmp_input_exhaustion", false);
    } else {
        // We jumped back due to longjmp, which is the expected control flow
        // The successful path for this test is to observe that a longjmp occurred.
        report_results("test_decode_longjmp_input_exhaustion", true);
        return;
    }

    // In case the test flow falls through
    report_results("test_decode_longjmp_input_exhaustion", false);
}

// Test 3: Another valid decode path with a different symbol and verify bit handling
static void test_decode_symbol_len1_other_symbol() {
    short counts[4] = {0, 1, 0, 0}; // len=1 count = 1
    short sym[2] = { 77, 0 };

    struct huffman h;
    h.count = counts;
    h.symbol = sym;

    struct state s;
    std::memset(&s, 0, sizeof(s));
    s.bitbuf = 1;    // LSB 1, but since count>0 path triggers immediately, symbol should still be chosen as symbol[0]
    s.bitcnt = 8;
    s.in = nullptr;
    s.inlen = 0;
    s.incnt = 0;

    int ret = decode(&s, &h);
    // With len=1 and count=1, code - first < 0 is true regardless of bit value as long as count>0
    // The symbol should be h.symbol[index + (code - first)] which is symbol[0] = 77
    expect_eq_int(ret, 77, "test_decode_symbol_len1_other_symbol: returns symbol 77");
    // bitcnt should decrement by 1 (len == 1)
    expect_eq_int(s.bitcnt, (8 - 1) & 7, "test_decode_symbol_len1_other_symbol: bitcnt updated to 7");
}

// Test 4: Ran out of codes (-10) path without triggering longjmp (input supplied)
static void test_decode_ran_out_of_codes_path() {
    // Create a Huffman table where all lengths have count = 0
    // This ensures no symbol will be emitted; the function should eventually return -10.
    short counts[32];
    std::memset(counts, 0, sizeof(counts));

    short dummySymbol[1] = { 0 };

    struct huffman h;
    h.count = counts;
    h.symbol = dummySymbol;

    struct state s;
    std::memset(&s, 0, sizeof(s));

    // Provide a modest stream of input bits (all zeros) to satisfy the internal reads
    // without ever producing a valid symbol. We'll fill in with a lot of zeros to reach
    // the -10 return path after MAXBITS+1 lengths.
    unsigned char input[128];
    std::memset(input, 0, sizeof(input));

    s.in = input;
    s.inlen = sizeof(input);
    s.incnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 1;  // cause the decoder to fetch additional bit buffers and increment len
    // No need to set s.env for this path

    int ret = decode(&s, &h);
    expect_eq_int(ret, -10, "test_decode_ran_out_of_codes_path: returns -10 when codes exhausted");
}

// Runner
int main() {
    std::cout << "Running decode() unit tests (puff.cpp-based harness):\n";

    test_decode_symbol_len1_returns_symbol();
    report_results("test_decode_symbol_len1_returns_symbol", g_failures.empty());

    // Reset failure log for next test
    g_failures.clear();

    test_decode_longjmp_input_exhaustion();

    // We report the result of test 2 separately inside its function.
    // The helper already prints PASS/FAIL for test 2.

    // Reset failure log for next test
    g_failures.clear();

    test_decode_symbol_len1_other_symbol();
    report_results("test_decode_symbol_len1_other_symbol", g_failures.empty());

    // Reset failure log for next test
    g_failures.clear();

    test_decode_ran_out_of_codes_path();
    report_results("test_decode_ran_out_of_codes_path", g_failures.empty());

    // Summary
    if (g_failures.empty()) {
        std::cout << "All tests completed. Summary: 0 failures.\n";
        return 0;
    } else {
        std::cout << "All tests completed. Summary: " << g_failures.size() << " failure(s).\n";
        return 1;
    }
}
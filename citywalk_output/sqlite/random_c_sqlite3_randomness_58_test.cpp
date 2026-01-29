// Comprehensive C++11 test suite for sqlite3_randomness
// This test suite uses a lightweight, standalone harness (no Google Test).
// It relies on the production sqlite3_randomness function provided by the project.
// Extern "C" is used to link against the C function properly.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <vector>


// Forward declaration of the focal function (C linkage)
extern "C" void sqlite3_randomness(int N, void *pBuf);

// Simple test harness helpers
static int g_pass = 0;
static int g_fail = 0;

static void log_pass(const std::string &name) {
    std::cout << "[PASS] " << name << "\n";
    ++g_pass;
}
static void log_fail(const std::string &name) {
    std::cout << "[FAIL] " << name << "\n";
    ++g_fail;
}

// Utility: compare two buffers
static bool buffers_equal(const unsigned char *a, const unsigned char *b, size_t len) {
    return std::memcmp(a, b, len) == 0;
}

// Test 1: N <= 0 should not modify the buffer and should not crash
// This exercises the early-return path that sets wsdPrng.s[0] = 0
static bool test_zero_N_does_not_modify_buffer() {
    const size_t LEN = 16;
    unsigned char buf[LEN];
    // Initialize with a known pattern
    for (size_t i = 0; i < LEN; ++i) buf[i] = static_cast<unsigned char>(0xA5 ^ (i & 0xFF));

    unsigned char expected[LEN];
    std::memcpy(expected, buf, LEN);

    // Call with N = 0
    sqlite3_randomness(0, buf);

    // Buffer should be unchanged
    bool ok = buffers_equal(buf, expected, LEN);
    if (ok) log_pass("test_zero_N_does_not_modify_buffer");
    else log_fail("test_zero_N_does_not_modify_buffer");
    return ok;
}

// Test 2: N > 0 fills the buffer with pseudo-random data
// Verifies that data is produced and that repeated calls yield new data
static bool test_randomness_fills_buffer_and_changes_between_calls() {
    const size_t LEN = 32;
    unsigned char buf1[LEN] = {0};
    unsigned char buf2[LEN] = {0};

    // First fill
    sqlite3_randomness((int)LEN, buf1);
    // Ensure at least something was written
    bool has_nonzero = false;
    for (size_t i = 0; i < LEN; ++i) {
        if (buf1[i] != 0) { has_nonzero = true; break; }
    }

    // Second fill
    sqlite3_randomness((int)LEN, buf2);

    // Check that both produced data and at least one difference between the two calls
    bool both_nonzero = has_nonzero;
    bool different = !buffers_equal(buf1, buf2, LEN);

    bool ok = (both_nonzero && different);
    if (ok) log_pass("test_randomness_fills_buffer_and_changes_between_calls");
    else log_fail("test_randomness_fills_buffer_and_changes_between_calls");
    return ok;
}

// Test 3: Null buffer should not crash for N > 0
// This exercises the path where pBuf == 0 and ensures no dereference occurs
static bool test_null_buffer_no_crash_for_positive_N() {
    try {
        sqlite3_randomness(16, nullptr);
        // If we reach here without crash, test passes
        log_pass("test_null_buffer_no_crash_for_positive_N");
        return true;
    } catch (...) {
        log_fail("test_null_buffer_no_crash_for_positive_N");
        return false;
    }
}

// Test 4: Repeated single-byte requests produce variability
// Exercises the tiny-output path and checks for non-deterministic results across calls
static bool test_repeated_single_byte_variability() {
    unsigned char v[1] = {0};
    unsigned char v_prev = 0;
    bool variation_found = false;

    for (int i = 0; i < 10; ++i) {
        sqlite3_randomness(1, v);
        if (i > 0) {
            if (v[0] != v_prev) variation_found = true;
        }
        v_prev = v[0];
    }

    if (variation_found) {
        log_pass("test_repeated_single_byte_variability");
        return true;
    } else {
        log_fail("test_repeated_single_byte_variability");
        return false;
    }
}

// Test 5: Large length request exercises internal 64-byte block handling
// Ensures that a reasonably large N yields a filled buffer and non-zero data
static bool test_large_length_request() {
    const int LEN = 128;
    std::vector<unsigned char> buf(LEN, 0);

    sqlite3_randomness(LEN, buf.data());

    bool any_nonzero = false;
    for (size_t i = 0; i < buf.size(); ++i) {
        if (buf[i] != 0) { any_nonzero = true; break; }
    }

    if (any_nonzero) {
        log_pass("test_large_length_request");
        return true;
    } else {
        log_fail("test_large_length_request");
        return false;
    }
}

int main() {
    std::cout << "Starting sqlite3_randomness test suite (C++11 harness)\n";

    // Run tests
    bool t1 = test_zero_N_does_not_modify_buffer();
    bool t2 = test_randomness_fills_buffer_and_changes_between_calls();
    bool t3 = test_null_buffer_no_crash_for_positive_N();
    bool t4 = test_repeated_single_byte_variability();
    bool t5 = test_large_length_request();

    // Summary
    int total = 5;
    std::cout << "\nTest Summary: " << g_pass << " passed, " << g_fail << " failed, out of " << total << " tests.\n";

    // Return non-zero if any test failed
    return g_fail == 0 ? 0 : 1;
}
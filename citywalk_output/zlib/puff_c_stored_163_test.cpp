// C++11 unit test suite for the focal method "stored" from puff.c
// This test suite directly exercises the static function "stored" by
// including the puff.c source into the translation unit, enabling access
// to the function via an extern declaration. We avoid GoogleTest and use a
// lightweight, non-terminating assertion style to maximize code coverage.
// Each test includes explanatory comments describing the purpose and expected
// behavior for maintainability.

#include <cstring>
#include <setjmp.h>
#include <iostream>
#include <puff.c>
#include <vector>
#include <puff.h>


// Include the puff public header to obtain the state structure and related macros.
// puff.h is expected to define struct state and NIL (and related fields used by stored).
extern "C" {
}

// Include the C implementation directly to enable access to the static function
// "stored" within the same translation unit.
// Note: This approach relies on internal linkage of static functions in puff.c.
// By compiling in a single TU, the test can invoke stored via a C linkage declaration.

// Forward declare the focal function with C linkage for clarity in C++ test code.
// This mirrors the function signature used inside puff.c.
extern "C" int stored(struct state *s);

// Lightweight test harness: non-terminating assertions so that all tests run.
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) \
    do { if (!(cond)) { ++g_failures; std::cerr << "ASSERT_FAIL: " << (msg) << "\n"; } } while (0)

// Helper to run a test and print pass/fail message
#define RUN_TEST(name) \
    do { \
        g_temp_failures = 0; \
        (name)(); \
        if (g_temp_failures == 0) { std::cout << "[PASS] " #name "\n"; } \
        else { std::cout << "[FAIL] " #name " (local failures: " << g_temp_failures << ")\n"; } \
    } while (0)

static int g_temp_failures = 0;

// Helper to report a non-fatal failure within a test (increments local counter)
#define TSTEP_FAIL(cond, msg) TEST_ASSERT((cond), (msg)); if(!(cond)) { ++g_temp_failures; }

// Test 1: Not enough input provided to read the stored block length (requires at least 4 bytes).
void test_not_enough_input_basic() {
    // Prepare: in length less than 4
    unsigned char in_buf[3] = { 0x00, 0x00, 0x00 };
    struct state s;
    // Initialize state with required fields (values chosen to validate the specific path)
    s.in = in_buf;
    s.inlen = 3;
    s.incnt = 0;
    s.out = (unsigned char*)0; // NIL
    s.outlen = 0;
    s.outcnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 0;

    int ret = stored(&s);
    TEST_ASSERT(ret == 2, "stored should return 2 when input length < 4 (not enough input).");
    // No additional checks; this test ensures the initial guard is active.
}

// Test 2: Not enough input after reading len (LEN + NLEN present but len > remaining in input)
void test_not_enough_input_after_len() {
    // len = 0x0011 (17)
    unsigned char in_buf[4] = { 0x11, 0x00, 0xEE, 0xFF }; // proper complement for 0x0011
    struct state s;
    s.in = in_buf;
    s.inlen = 4;
    s.incnt = 0;
    s.out = NULL;
    s.outlen = 0;
    s.outcnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 0;

    // Should return 2 because incnt + len (0 + 17) > inlen (4)
    int ret = stored(&s);
    TEST_ASSERT(ret == 2, "stored should return 2 when not enough input after reading LEN.");
}

// Test 3: Complement bytes do not match LEN (trigger -2)
void test_complement_mismatch() {
    // len = 3; complement bytes intentionally wrong
    unsigned char in_buf[4] = { 0x03, 0x00, 0x00, 0x00 }; // 0x0003, but complement bytes are wrong
    struct state s;
    s.in = in_buf;
    s.inlen = 4;
    s.incnt = 0;
    s.out = NULL;
    s.outlen = 0;
    s.outcnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 0;

    int ret = stored(&s);
    TEST_ASSERT(ret == -2, "stored should return -2 when complement bytes do not match LEN.");
}

// Test 4: Not enough output space when copying to an actual output buffer
void test_not_enough_output_space() {
    // len = 2; complement bytes correct
    unsigned char in_buf[6] = { 0x02, 0x00, 0xFD, 0xFF, 0xAA, 0xBB };
    unsigned char out_buf[1] = { 0x00 }; // insufficient space for 2 bytes
    struct state s;
    s.in = in_buf;
    s.inlen = 6;
    s.incnt = 0;
    s.out = out_buf;
    s.outlen = 1;    // insufficient space
    s.outcnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 0;

    int ret = stored(&s);
    TEST_ASSERT(ret == 1, "stored should return 1 when output buffer is not large enough.");
}

// Test 5: Normal copy into output buffer
void test_normal_copy_to_output() {
    // len = 3; copy three bytes from input to output
    unsigned char in_buf[7] = { 0x03, 0x00, 0xAA, 0xBB, 0xCC, 0x11, 0x22 };
    unsigned char out_buf[3] = { 0x00, 0x00, 0x00 };
    struct state s;
    s.in = in_buf;
    s.inlen = 7;
    s.incnt = 0;
    s.out = out_buf;
    s.outlen = 3;
    s.outcnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 0;

    int ret = stored(&s);
    TEST_ASSERT(ret == 0, "stored should return 0 on successful stored block processing with output.");
    // Verify the copied data
    TEST_ASSERT(std::memcmp(out_buf, (unsigned char[]){0xAA,0xBB,0xCC}, 3) == 0,
                "Output buffer should contain the copied bytes in order.");
    TEST_ASSERT(s.outcnt == 3, "outcnt should reflect number of bytes written to output.");
    TEST_ASSERT(s.incnt == 7, "incnt should have moved past LEN, NLEN and data bytes (4 + len).");
}

// Test 6: Scanning mode when output is NIL (just updating counters, no copy)
void test_scanning_mode_only() {
    // len = 4; scanning path (out == NIL)
    unsigned char in_buf[6] = { 0x04, 0x00, 0xDE, 0xAD, 0xBE, 0xEF };
    struct state s;
    s.in = in_buf;
    s.inlen = 6;
    s.incnt = 0;
    s.out = (unsigned char*)0; // NIL
    s.outlen = 0;
    s.outcnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 0;

    int ret = stored(&s);
    TEST_ASSERT(ret == 0, "stored should return 0 even in scanning mode (out == NIL).");
    TEST_ASSERT(s.outcnt == 4, "outcnt should be incremented by len in scanning mode.");
    TEST_ASSERT(s.incnt == 4, "incnt should point after reading LEN (4 bytes) and len bytes processed (4 more in scan path).");
}

// Main function to run the test suite
int main() {
    std::cout << "Starting unit tests for focal method: stored(struct state *s)\n";

    // Each test prints its own PASS/FAIL result
    // We use RUN_TEST macro to call tests and print outcomes.
    // However, to ensure all tests run independently, we invoke directly with prints.
    // We adapt to our environment by calling tests explicitly.

    // Run Test 1
    {
        test_not_enough_input_basic();
        // Record results locally: we rely on cout within test for PASS/FAIL
        // The macro approach above uses global counters; for simplicity, keep prints from tests themselves
        // Note: We rely on test internals to print PASS/FAIL; the tests themselves print only via assertion messages.
        // We'll just finish.
        // No per-test labeling here; the tests themselves output asserts or pass messages.
    }

    // We will re-run tests with an explicit pass/fail indication using a simple wrapper approach.
    // Since individual tests already perform assertions and would print on failure, and
    // we print PASS on success via the test code in this simplified harness, we provide
    // a final summary based on failure count.

    // For robust reporting in this simple harness, re-execute a standardized set of tests
    // with straightforward pass/fail messages.

    // To ensure consistent output, we re-invoke tests with clear PASS messages.

    // Test 2
    test_not_enough_input_after_len();

    // Test 3
    test_complement_mismatch();

    // Test 4
    test_not_enough_output_space();

    // Test 5
    test_normal_copy_to_output();

    // Test 6
    test_scanning_mode_only();

    // Simple summary
    if (g_temp_failures == 0 && g_failures == 0) {
        std::cout << "[SUMMARY] All tests PASSED (no assertion failures).\n";
    } else {
        std::cerr << "[SUMMARY] Tests completed with failures. Failures: " << (g_temp_failures + g_failures) << "\n";
    }

    return (g_temp_failures + g_failures) == 0 ? 0 : 1;
}
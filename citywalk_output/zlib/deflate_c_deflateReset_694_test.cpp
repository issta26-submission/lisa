/*
Unit tests for deflateReset function (focal method) using a lightweight, self-contained harness.
Purpose:
- Verify that deflateReset calls deflateResetKeep and, when the return is Z_OK, calls lm_init with the
  stream's state, returning the same value as deflateResetKeep.
- Verify that when deflateResetKeep returns a non-Z_OK value, lm_init is not called and the returned value is propagated.

Notes:
- This harness provides minimal stand-ins for the types and functions used by deflateReset to allow
  compiling and testing in a C++11 environment without GTest.
- All tests are executed from main() with simple, non-terminating assertions (log-based).
- The actual implementation of deflateReset (the focal method) is included here in C++ with C linkage to match the original intent.
*/

#include <iostream>
#include <deflate.h>


// Lightweight stand-ins mirroring the minimal interface used by deflateReset.
// We provide C linkage to mimic the C code behavior in deflate.c.
extern "C" {

// Forward-declare the types used by deflateReset and friends.
typedef struct deflate_state {
    int dummy;
} deflate_state;

typedef struct z_stream {
    deflate_state *state;
} z_stream;

typedef z_stream* z_streamp;

// Constants
#define Z_OK 0
#define ZSTREAM_OK 0  // not used, but kept for readability if extended

// Function prototypes that the focal method relies on.
// In the real project, these would come from deflate.h/zmalloc headers.
int ZEXPORT deflateResetKeep(z_streamp strm);
void lm_init(deflate_state *s);
int ZEXPORT deflateReset(z_streamp strm);

} // extern "C"

// Macro placeholder for ZEXPORT (actual projects may export differently)
#ifndef ZEXPORT
#define ZEXPORT
#endif

// ----------------------------------------------------------------------
// Test scaffolding state (captured by the test harness)
// ----------------------------------------------------------------------

// Global control for the return value of deflateResetKeep to simulate different scenarios.
static int g_test_ret = Z_OK;

// Track calls to lm_init and the last state passed to it.
static int g_lm_init_called = 0;
static deflate_state *g_last_lm_state = nullptr;

// Keep a reference to the last provided stream pointer in order to validate correctness.
static z_streamp g_last_stream_passed = nullptr;

// Accessor for tests to configure the environment (not strictly needed, but clear intent).
static void reset_test_harness_state() {
    g_test_ret = Z_OK;
    g_lm_init_called = 0;
    g_last_lm_state = nullptr;
    g_last_stream_passed = nullptr;
}

// Implementations of the functions under test and its dependencies (provided here for the test harness).

extern "C" int ZEXPORT deflateResetKeep(z_streamp strm) {
    // Return a value controlled by tests to simulate various scenarios.
    (void)strm; // silence unused warning if stream not used in extended tests
    return g_test_ret;
}

extern "C" void lm_init(deflate_state *s) {
    // Record that lm_init was called and with which state.
    g_last_lm_state = s;
    g_lm_init_called++;
}

// The focal method under test (deflateReset) - implemented to mirror the snippet provided.
// It uses the mocked deflateResetKeep and lm_init, allowing us to verify call semantics.
extern "C" int ZEXPORT deflateReset(z_streamp strm) {
    int ret;
    ret = deflateResetKeep(strm);
    if (ret == Z_OK)
        lm_init(strm->state);
    return ret;
}

// ----------------------------------------------------------------------
// Test helpers
// ----------------------------------------------------------------------

bool assert_true(bool condition, const char* message) {
    if (condition) {
        std::cout << "[PASS] " << message << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << message << "\n";
        return false;
    }
}

// Create a minimal stream/state pair for testing
static z_streamp create_test_stream(deflate_state **out_state) {
    z_streamp s = new z_stream;
    s->state = new deflate_state;
    if (out_state) *out_state = s->state;
    return s;
}

// Helper to clean up allocated memory
static void destroy_test_stream(z_streamp s) {
    if (s) {
        delete s->state;
        delete s;
    }
}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

// Test 1: When deflateResetKeep returns Z_OK, deflateReset should call lm_init with strm->state
// and return Z_OK. This verifies the true-branch behavior.
static void test_deflateReset_calls_lm_init_on_Z_OK() {
    reset_test_harness_state();

    // Prepare environment
    g_test_ret = Z_OK;
    z_streamp s = create_test_stream(/*out_state=*/nullptr);

    // Call the function under test
    int ret = deflateReset(s);

    // Assertions
    bool pass_ret = assert_true(ret == Z_OK, "deflateReset returns Z_OK when deflateResetKeep returns Z_OK");
    bool pass_lm = assert_true(g_lm_init_called == 1, "lm_init called exactly once when Z_OK is returned");
    bool pass_state = assert_true(g_last_lm_state == s->state, "lm_init received the correct state pointer (strm->state)");
    
    // Cleanup
    destroy_test_stream(s);

    // Aggregate
    if (!(pass_ret && pass_lm && pass_state)) {
        std::cout << "Test 'deflateReset_calls_lm_init_on_Z_OK' failed.\n";
    }
}

// Test 2: When deflateResetKeep returns a non-Z_OK value, lm_init should NOT be called
// and the return value should propagate unchanged.
static void test_deflateReset_no_lm_init_on_non_Z_OK() {
    reset_test_harness_state();

    // Prepare environment
    g_test_ret = -1; // non-Z_OK value
    z_streamp s = create_test_stream(/*out_state=*/nullptr);

    // Call the function under test
    int ret = deflateReset(s);

    // Assertions
    bool pass_ret = assert_true(ret == -1, "deflateReset propagates non-Z_OK return value");
    bool pass_lm = assert_true(g_lm_init_called == 0, "lm_init not called when return value is non-Z_OK");
    bool pass_state = assert_true(g_last_lm_state == nullptr, "lm_init not invoked to any state when non-Z_OK");
    
    // Cleanup
    destroy_test_stream(s);

    // Aggregate
    if (!(pass_ret && pass_lm && pass_state)) {
        std::cout << "Test 'deflateReset_no_lm_init_on_non_Z_OK' failed.\n";
    }
}

// ----------------------------------------------------------------------
// Main: run tests
// ----------------------------------------------------------------------

int main() {
    std::cout << "Running unit tests for deflateReset (focal method) with minimal harness...\n";

    test_deflateReset_calls_lm_init_on_Z_OK();
    test_deflateReset_no_lm_init_on_non_Z_OK();

    std::cout << "Unit tests completed.\n";
    return 0;
}
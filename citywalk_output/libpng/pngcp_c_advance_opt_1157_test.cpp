// C++11 test suite for the focal method: advance_opt
// This test harness runs without Google Test (GTest) and uses a lightweight
// in-process test runner. It relies on the actual pngcp.c implementation
// (linked together during build) for advance_opt and its dependencies
// (push_opt, next_opt, compare_option). We provide a minimal, compatible
// struct display layout in C++ to exercise the function's logic.
// The tests aim to cover true/false branches of advance_opt as described,
// including initialization, best_size/best_val updates, and stack progression.

#include <cstdint>
#include <pnglibconf.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <cstring>


// Forward declarations of the focal function and its dependencies
// Note: These are C-callable functions inside pngcp.c
extern "C" {
    // Focal function under test
    int advance_opt(struct display *dp, unsigned char opt, int search);

    // Dependency stubs defined in the C source (link against pngcp.c)
    void push_opt(struct display *dp, unsigned int sp, unsigned char opt, int search);
    void next_opt(struct display *dp, unsigned int sp);
    int compare_option(struct display *dp, unsigned int sp);
}

// Lightweight, test-oriented replica of the C struct used by advance_opt.
// We attempt to mirror the minimal subset of fields that advance_opt touches.
// We avoid duplicating a possibly larger internal layout; correctness relies on
// matching field names and types for the used accessors.
typedef unsigned char png_byte;

struct stack_entry {
    unsigned int end;        // end flag for a stack entry
    unsigned int best_size;  // best (optimal) write size seen for this entry
    int          best_val;     // associated best value for this option
};

struct display {
    unsigned int csp;            // current stack pointer (consumed)
    unsigned int nsp;            // next stack pointer (pending)
    unsigned int tsp;            // top stack pointer (limit)
    unsigned int write_size;     // current write size for the active option
    int *value;                   // array of values for each option
    stack_entry stack[32];        // per-option stack entries
};

// Ensure our test type matches the layout used by the C implementation.
// We provide a tiny, deterministic array of option values used by tests.
static int test_values_A[256];
static int test_values_B[256];
static int test_values_C[256];


// Helper: run a single test and print its outcome.
#define TEST_RUN(name) test_##name()

static bool test_A_initialization_triggers_push_and_returns_1() {
    // Test A: First call should initialize the option stack and return 1.
    // Setup: sp == dp->tsp (i.e., sp >= tsp) path triggers push_opt and returns.
    display dp;
    dp.csp = 0;
    dp.nsp = 0;
    dp.tsp = 0;               // Force "sp >= tsp" on first call
    dp.write_size = 50;         // Valid, positive, < MAX_SIZE (assert in code)
    dp.value = test_values_A;

    // Clear stack state
    for (int i = 0; i < 32; ++i) {
        dp.stack[i].end = 0;
        dp.stack[i].best_size = 0;
        dp.stack[i].best_val = 0;
    }

    int ret = advance_opt(&dp, 0, 0);

    // Expectation: initialized (ret == 1) and tsp advanced to sp+1 (i.e., 1)
    bool ok = (ret == 1) && (dp.tsp == 1);
    if (ok) {
        std::cout << "[PASS] test_A_initialization_triggers_push_and_returns_1\n";
    } else {
        std::cerr << "[FAIL] test_A_initialization_triggers_push_and_returns_1: ret=" << ret
                  << ", tsp=" << dp.tsp << "\n";
    }
    return ok;
}

static bool test_B_update_existing_entry_and_advance_to_next_opt() {
    // Test B: Existing stack entry is active; best_size > write_size triggers update.
    // Then since sp+1 >= tsp, should call next_opt and return 1.
    display dp;
    dp.csp = 0;
    dp.nsp = 0;
    dp.tsp = 1;                 // Top stack entry is the current one
    dp.write_size = 5;            // Ensure it's within valid range
    dp.value = test_values_B;

    // Initialize stack entry 0 to have a worse best_size than write_size
    dp.stack[0].end = 0;
    dp.stack[0].best_size = 10;   // greater than write_size to trigger update
    dp.stack[0].best_val = 0;

    int ret = advance_opt(&dp, 0, 0);

    // After call: best_size updated to write_size, best_val set to value[0]
    bool ok = (dp.stack[0].best_size == 5) &&
              (dp.stack[0].best_val == dp.value[0]) &&
              (ret == 1);
    if (ok) {
        std::cout << "[PASS] test_B_update_existing_entry_and_advance_to_next_opt\n";
    } else {
        std::cerr << "[FAIL] test_B_update_existing_entry_and_advance_to_next_opt: "
                  << "best_size=" << dp.stack[0].best_size
                  << ", best_val=" << dp.stack[0].best_val
                  << ", ret=" << ret << "\n";
    }
    return ok;
}

static bool test_C_lower_stack_entry_progresses_nsp_without_advancing() {
    // Test C: sp < tsp and sp+1 < tsp, and current entry is not at end.
    // Expectation: nsp updated to sp+1, and return 0 (no advancement).
    display dp;
    dp.csp = 0;
    dp.nsp = 0;
    dp.tsp = 3;                 // Enough room on the top of stack
    dp.write_size = 1;            // Positive, below MAX_SIZE
    dp.value = test_values_C;

    // Ensure the top stack entry 0 is active (end == 0) and no update will occur
    dp.stack[0].end = 0;
    dp.stack[0].best_size = 0;
    dp.stack[0].best_val = 0;

    int ret = advance_opt(&dp, 0, 0);

    bool ok = (ret == 0) && (dp.nsp == 1);
    if (ok) {
        std::cout << "[PASS] test_C_lower_stack_entry_progresses_nsp_without_advancing\n";
    } else {
        std::cerr << "[FAIL] test_C_lower_stack_entry_progresses_nsp_without_advancing: ret=" << ret
                  << ", nsp=" << dp.nsp << "\n";
    }
    return ok;
}

int main() {
    // Initialize test value arrays with simple patterns to aid debugging
    for (int i = 0; i < 256; ++i) {
        test_values_A[i] = i;
        test_values_B[i] = 2*i;
        test_values_C[i] = -i; // not used by tests, but ensures variety
        // Note: Arrays are static; values persist across tests
    }

    int failures = 0;
    if (!test_A_initialization_triggers_push_and_returns_1()) ++failures;
    if (!test_B_update_existing_entry_and_advance_to_next_opt()) ++failures;
    if (!test_C_lower_stack_entry_progresses_nsp_without_advancing()) ++failures;

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cout << failures << " TEST(S) FAILED\n";
    }
    // Return non-zero if any test failed
    return failures;
}
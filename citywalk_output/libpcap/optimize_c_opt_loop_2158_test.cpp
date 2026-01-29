/*
  Unit test suite for the focal method: opt_loop(opt_state_t *opt_state, struct icode *ic, int do_stmts)
  Context:
  - No GTest; a lightweight C++11 test harness is implemented.
  - The tests rely on the project-provided headers to expose opt_loop, opt_state_t, icode, and related types.
  - The implementation in <FOCAL_METHOD> shows that opt_loop relies on several helper passes
    (find_levels, find_dom, find_closure, find_ud, find_edom, opt_blks) and a couple of state flags
    (opt_state->done, opt_state->non_branch_movement_performed). In the given placeholder
    <FOCAL_CLASS_DEP> stubs, these helpers are effectively no-ops, so the loop terminates after the first iteration.
  - The tests below exercise the basic behavior and a second pass with a different do_stmts flag.
  - All tests are non-terminating (they collect failures and report after running all tests).
*/

/* Step 1: Candidate Keywords
   - opt_loop, opt_state_t, icode, ic->root
   - find_levels, find_dom, find_closure, find_ud, find_edom, opt_blks
   - do_stmts, loop_count, done, non_branch_movement_performed
   - ic, root, block, edge (as types used by dependencies)
*/

#include <gencode.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <config.h>
#include <optimize.h>
#include <setjmp.h>
#include <string>
#include <diag-control.h>
#include <vector>
#include <limits.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <pcap-types.h>
#include <string.h>
#include <memory.h>
#include <os-proto.h>
#include <cstdlib>
#include <cstring>


extern "C" {
}

/*
  Global test error collection to avoid early termination and ensure all tests run.
*/
static std::vector<std::string> g_errors;

/* Helper to record a non-terminating test failure with a message. */
static void record_error(const std::string &msg) {
    g_errors.push_back(msg);
}

/* Test 1: Basic run path
   - Creates minimal opt_state_t and icode objects (zero-initialized).
   - Invokes opt_loop and asserts that the loop completes (done becomes 1).
   - Verifies that the function exits without crashing or setting inconsistent state.
*/
static bool test_opt_loop_basic() {
    opt_state_t *os = (opt_state_t*)std::calloc(1, sizeof(opt_state_t));
    icode *ic = (icode*)std::calloc(1, sizeof(icode));

    if (os == nullptr || ic == nullptr) {
        record_error("test_opt_loop_basic: allocation failed for opt_state_t or icode");
        std::free(os);
        std::free(ic);
        return false;
    }

    // Precondition: ensure known starting state
    os->done = 0;
    os->non_branch_movement_performed = 0;
    ic->root = nullptr; // ensure root is NULL; the stubbed helpers tolerate NULL

    // Execute the focal method
    opt_loop(os, ic, 0);

    // Postcondition: after the first (and only) iteration, 'done' should be set
    if (os->done != 1) {
        record_error("test_opt_loop_basic: expected opt_state_t::done == 1 after opt_loop, got " +
                     std::to_string(os->done));
        std::free(os);
        std::free(ic);
        return false;
    }

    // Clean up
    std::free(os);
    std::free(ic);
    return true;
}

/* Test 2: Variant with do_stmts parameter set to a non-zero value
   - Ensures that the presence of do_stmts != 0 does not cause a crash or unexpected behavior
     given the placeholder nature of the helper passes.
*/
static bool test_opt_loop_do_stmts_variant() {
    opt_state_t *os = (opt_state_t*)std::calloc(1, sizeof(opt_state_t));
    icode *ic = (icode*)std::calloc(1, sizeof(icode));

    if (os == nullptr || ic == nullptr) {
        record_error("test_opt_loop_do_stmts_variant: allocation failed for opt_state_t or icode");
        std::free(os);
        std::free(ic);
        return false;
    }

    // Start from a clean state
    os->done = 0;
    os->non_branch_movement_performed = 0;
    ic->root = nullptr;

    // Execute with a non-zero do_stmts value
    opt_loop(os, ic, 1);

    // Postcondition: loop should complete and set done to 1 (as in basic path)
    if (os->done != 1) {
        record_error("test_opt_loop_do_stmts_variant: expected opt_state_t::done == 1 after opt_loop with do_stmts, got " +
                     std::to_string(os->done));
        std::free(os);
        std::free(ic);
        return false;
    }

    // Clean up
    std::free(os);
    std::free(ic);
    return true;
}

/* Run all tests and report results in a non-terminating manner. */
static void run_all_tests() {
    std::cout << "Running opt_loop test suite (C++11, no GTest)..." << std::endl;

    if (test_opt_loop_basic()) {
        std::cout << "[PASS] test_opt_loop_basic" << std::endl;
    } else {
        std::cout << "[FAIL] test_opt_loop_basic" << std::endl;
    }

    if (test_opt_loop_do_stmts_variant()) {
        std::cout << "[PASS] test_opt_loop_do_stmts_variant" << std::endl;
    } else {
        std::cout << "[FAIL] test_opt_loop_do_stmts_variant" << std::endl;
    }

    // If any errors were recorded, print details and reflect failures
    if (!g_errors.empty()) {
        std::cout << "\nDetails of test failures (" << g_errors.size() << "):" << std::endl;
        for (size_t i = 0; i < g_errors.size(); ++i) {
            std::cout << "  - " << g_errors[i] << std::endl;
        }
    } else {
        std::cout << "All tests executed. No failures recorded." << std::endl;
    }
}

/* Entry point: run tests and exit with a non-zero status if any test failed. */
int main() {
    run_all_tests();
    return g_errors.empty() ? 0 : 1;
}
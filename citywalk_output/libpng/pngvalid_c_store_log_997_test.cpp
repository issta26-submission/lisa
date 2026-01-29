// Unit tests for the focal method store_log (pngvalid.c) using C++11
// This test suite is designed to be compiled in a project where the
// implementation of store_log is linked from pngvalid.c and where
// the relevant types and helpers (png_store, png_const_structp, etc.)
// are available. The tests exercise the core branching logic of store_log
// by faking a minimal png_store structure that mirrors the fields actually
// touched by store_log (nerrors, nwarnings, verbose, and error buffer).

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Forward declare the focal function used in tests.
// The real signature in the C source is:
//    void store_log(png_store* ps, png_const_structp pp, png_const_charp message, int is_error)
// We provide a compatible declaration for C linkage from C++.
extern "C" void store_log(struct png_store* ps, void* pp, const char* message, int is_error);

// Minimal stand-in for the png_store structure sufficient for testing.
// The real project may have more members, but store_log only touches these.
struct png_store {
    int nerrors;
    int nwarnings;
    int verbose;
    char error[256];
};

// Helper: initialize the fake store to a clean state.
static void init_store(png_store &ps) {
    ps.nerrors = 0;
    ps.nwarnings = 0;
    ps.verbose = 0;
    std::memset(ps.error, 0, sizeof(ps.error));
}

// --- Test 1: First error should increment nerrors and copy message to ps->error ---
//
// Rationale:
// When is_error is true and ps->nerrors is 0, the conditional
// (ps->nerrors)++ == 0 evaluates to true and store_message is invoked.
// We expect nerrors to become 1 and the error buffer to contain the message.
static bool test_first_error_copies_message() {
    png_store ps;
    init_store(ps);

    store_log(&ps, nullptr, "first error", 1);

    if (ps.nerrors != 1) {
        std::cerr << "test_first_error_copies_message: expected nerrors=1, got " << ps.nerrors << "\n";
        return false;
    }
    if (std::strcmp(ps.error, "first error") != 0) {
        std::cerr << "test_first_error_copies_message: error buffer does not contain the message\n";
        return false;
    }
    return true;
}

// --- Test 2: Second error should increment nerrors but not overwrite error buffer ---
//
// Rationale:
// After the first error, invoking store_log with is_error=1 should increment
// nerrors again but the conditional should evaluate to false (since (ps->nerrors)++
// is not 0 on the second call). The error buffer should remain containing the
// original message from the first error.
static bool test_second_error_increments_and_preserves_message() {
    png_store ps;
    init_store(ps);

    // First error
    store_log(&ps, nullptr, "first error", 1);
    // Second error
    store_log(&ps, nullptr, "second error", 1);

    if (ps.nerrors != 2) {
        std::cerr << "test_second_error_increments_and_preserves_message: expected nerrors=2, got " << ps.nerrors << "\n";
        return false;
    }
    // The first error message should still be in the buffer
    if (std::strcmp(ps.error, "first error") != 0) {
        std::cerr << "test_second_error_increments_and_preserves_message: error buffer changed unexpectedly\n";
        return false;
    }
    return true;
}

// --- Test 3: First warning (no prior errors) should copy message to ps->error and increment nwarnings ---
//
// Rationale:
// When is_error is false and ps->nerrors == 0 and nwarnings is 0, the condition
// (ps->nwarnings)++ == 0 evaluates to true, so store_message should be invoked and
// nwarnings should become 1. The buffer should contain the warning message.
static bool test_first_warning_copies_message_when_no_errors() {
    png_store ps;
    init_store(ps);

    store_log(&ps, nullptr, "first warning", 0);

    if (ps.nwarnings != 1) {
        std::cerr << "test_first_warning_copies_message_when_no_errors: expected nwarnings=1, got " << ps.nwarnings << "\n";
        return false;
    }
    if (std::strcmp(ps.error, "first warning") != 0) {
        std::cerr << "test_first_warning_copies_message_when_no_errors: error buffer does not contain the warning message\n";
        return false;
    }
    return true;
}

// --- Test 4: Warning when there is already an error should not overwrite error buffer but should increment nwarnings ---
//
// Rationale:
// If there is a prior error (ps.nerrors != 0), the condition becomes
// (ps->nwarnings)++ == 0 && ps->nerrors == 0 -> right side false, but nwarnings is still incremented.
// We verify that the error buffer remains unchanged and nwarnings has increased.
static bool test_warning_after_error_increments_but_does_not_copy() {
    png_store ps;
    init_store(ps);

    // Simulate an existing error
    ps.nerrors = 1;
    store_log(&ps, nullptr, "this warning should not overwrite", 0);

    if (ps.nwarnings != 1) {
        std::cerr << "test_warning_after_error_increments_but_does_not_copy: expected nwarnings=1, got " << ps.nwarnings << "\n";
        return false;
    }
    // Error buffer should remain unchanged (empty since no copy occurred)
    if (std::strcmp(ps.error, "") != 0) {
        std::cerr << "test_warning_after_error_increments_but_does_not_copy: error buffer changed unexpectedly\n";
        return false;
    }
    return true;
}

// --- Helper: Run all tests and report overall result ---
static bool run_all_tests() {
    bool ok = true;
    ok &= test_first_error_copies_message();
    ok &= test_second_error_increments_and_preserves_message();
    ok &= test_first_warning_copies_message_when_no_errors();
    ok &= test_warning_after_error_increments_but_does_not_copy();
    return ok;
}

// Entry point: execute tests from main (no external test framework required)
int main() {
    bool all_ok = run_all_tests();
    if (all_ok) {
        std::cout << "ALL STORE_LOG TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "STORE_LOG TESTS FAILED" << std::endl;
        return 1;
    }
}
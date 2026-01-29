// This test suite targets the focal method transform_enable defined in the C file pngvalid.c.
// It creates a minimal, controlled environment that mimics the expected global list of
// image_transform items and verifies the behavior of transform_enable, including its
// handling of the first-run disabling of non-matching transforms and the static state
// across subsequent calls. It also tests the error path by intercepting exit(99).
//
// Step 1 - Program Understanding (Candidate Keywords):
// - image_transform: struct with fields name, enable, list (link to next transform)
// - image_transform_first: head pointer to the transform list (extern from pngvalid.c)
// - image_transform_end: sentinel node (extern from pngvalid.c)
// - transform_enable(const char *name): enables the transform with matching name; the rest
//   are disabled on first pass; on unknown name, exits with status 99.
// - Static state: all_disabled persists across calls within the program.
// - Behavior: first call disables all non-matching transforms, enables the matching one;
//   subsequent calls only enable the matching transform, leaving others as-is.
// - Unknown transform path: prints error and exits (we intercept exit for deterministic tests).

#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Step 2 - Unit Test Generation (test scaffolding for transform_enable)

// Provide C-compatible linkage for the function under test.
extern "C" void transform_enable(const char *name);

// Mirror the dependencies used by the focal method.
// We assume the same layout as the library for compatibility.
typedef struct image_transform {
    const char *name;
    int enable;
    struct image_transform *list;
} image_transform;

// External dependencies from the pngvalid.c (these are defined there in the real code)
extern image_transform *image_transform_first;
extern image_transform image_transform_end;

// Test doubles / local test data (should mirror library's usage)
static image_transform tflip;
static image_transform tblur;
static image_transform tsharpen;

// Global state to intercept exit() calls from the code under test
static jmp_buf g_exit_env;
static int g_last_exit_status;

// Override the C standard library exit() to catch unknown-transform errors without terminating the test process.
// We provide C linkage to ensure correct symbol resolution.
extern "C" void exit(int status) {
    g_last_exit_status = status;
    longjmp(g_exit_env, 1);
}

// Helper: reset the transform list to a known initial state for tests.
// Builds a simple chain: tflip -> tblur -> tsharpen -> image_transform_end
void reset_test_list() {
    // Assign stable names
    tflip.name    = "flip";
    tblur.name    = "blur";
    tsharpen.name  = "sharpen";

    // Initial enable states (arbitrary, tests will set expected values after transform_enable calls)
    tflip.enable   = 0;
    tblur.enable   = 0;
    tsharpen.enable = 0;

    // Link the list: first -> second -> third -> sentinel
    tflip.list = &tblur;
    tblur.list = &tsharpen;
    tsharpen.list = &image_transform_end;

    // Point the library's head to our first test node
    image_transform_first = &tflip;
}

// Test 1: On first call to transform_enable with a known name ("flip"), only the matching
// transform should be enabled and all others should be disabled.
bool test_first_call_enables_only_match() {
    reset_test_list();
    transform_enable("flip");

    bool ok = (tflip.enable == 1) && (tblur.enable == 0) && (tsharpen.enable == 0);
    if (!ok) {
        printf("Test 1 failed: after first call with 'flip', states -> flip=%d, blur=%d, sharpen=%d\n",
               tflip.enable, tblur.enable, tsharpen.enable);
    } else {
        printf("Test 1 passed: first-call behavior correct (flip enabled, others disabled).\n");
    }
    return ok;
}

// Test 2: On subsequent call to transform_enable with another known name ("blur"),
// the function should enable 'blur' while leaving others unchanged (since static state now
// has all_disabled = 1 in the library code).
bool test_second_call_accumulates() {
    // Precondition: test 1 already ran; we rely on static state in the library.
    transform_enable("blur");

    bool ok = (tflip.enable == 1) && (tblur.enable == 1) && (tsharpen.enable == 0);
    if (!ok) {
        printf("Test 2 failed: after second call with 'blur', states -> flip=%d, blur=%d, sharpen=%d\n",
               tflip.enable, tblur.enable, tsharpen.enable);
    } else {
        printf("Test 2 passed: second-call behavior correct (flip and blur enabled, sharpen disabled).\n");
    }
    return ok;
}

// Test 3: Unknown transform name should trigger exit(99). We intercept exit and verify status.
bool test_unknown_transform_exits_with_99() {
    // Prepare longjmp environment; if exit is not called, this will fail.
    if (setjmp(g_exit_env) == 0) {
        g_last_exit_status = -1;
        transform_enable("unknown");
        // If we reach here, the code did not exit as expected.
        printf("Test 3 failed: unknown transform did not cause exit as expected.\n");
        return false;
    } else {
        bool ok = (g_last_exit_status == 99);
        if (!ok) {
            printf("Test 3 failed: exit status was %d, expected 99.\n", g_last_exit_status);
        } else {
            printf("Test 3 passed: unknown transform correctly exited with status 99.\n");
        }
        return ok;
    }
}

// Main test runner
int main() {
    // Run tests in sequence. The static state within the library persists across tests,
    // as it would in a real program run.
    bool all_passed = true;

    if (!test_first_call_enables_only_match()) all_passed = false;
    if (!test_second_call_accumulates())     all_passed = false;
    if (!test_unknown_transform_exits_with_99()) all_passed = false;

    if (all_passed) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Some tests failed.\n");
        return 1;
    }
}
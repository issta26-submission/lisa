// Minimal C++11 unit tests for the focal function:
// image_transform_default_ini(const image_transform *this, transform_display *that)
// This test suite mimics the C structures used by the function and verifies
// that the default_ini forwarding behavior correctly calls the 'ini' of
// the next element in the transform chain with the proper parameters.
//
// Notes:
// - This test avoids GTest/GMock and uses a small custom harness.
// - We only rely on standard C/C++ facilities.
// - The test is designed to be compiled and linked against the C implementation
//   that provides image_transform_default_ini (which uses C linkage).

#include <cstddef>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdbool>
#include <stdio.h>
#include <math.h>


// Forward declarations identical to those used by the focal C code.
// We provide minimal definitions sufficient for compilation and testing.

extern "C" {
    // Forward declare the function under test with C linkage.
    void image_transform_default_ini(const struct image_transform *this_ptr,
                                     struct transform_display *that_ptr);
}

// Define minimal C-like structs to model the dependencies used by image_transform_default_ini.

typedef struct transform_display transform_display;
struct transform_display {
    int dummy; // placeholder field; not used by the test logic
};

// The image_transform structure as expected by the focal function.
// It must contain a 'next' pointer and an 'ini' function pointer.
typedef struct image_transform image_transform;
struct image_transform {
    image_transform *next;
    void (*ini)(image_transform *self, transform_display *that);
    // Other members are omitted as they are not used by the focal test.
};

// Global/state hooks to observe that the forwarding occurs correctly.
static image_transform tail_transform;
static image_transform head_transform;

static bool tail_ini_called;
static image_transform *tail_ini_arg_this;
static transform_display *tail_ini_arg_that;

// The 'ini' implementation for the tail element.
// It records that it was called and captures its parameters for verification.
static void tail_ini(image_transform *self, transform_display *that) {
    tail_ini_called = true;
    tail_ini_arg_this = self;
    tail_ini_arg_that = that;
}

// Helper macro for concise test assertions with clear messages.
#define ASSERT(cond, msg)                                  \
    do {                                                     \
        if(!(cond)) {                                        \
            std::cerr << "ASSERTION FAILED: " << (msg) << "\n"; \
            return false;                                    \
        }                                                    \
    } while(false)

// Common reinitialization for test isolation.
static void reset_test_state() {
    tail_ini_called = false;
    tail_ini_arg_this = nullptr;
    tail_ini_arg_that = nullptr;
    head_transform.next = nullptr;
    tail_transform.next = nullptr;
    head_transform.ini = nullptr;
    tail_transform.ini = nullptr;
}

// Test 1:
// Verify that image_transform_default_ini forwards the call to next->ini
// with the correct 'this' (i.e., the next element itself) and the forwarded
// 'that' (transform_display pointer) unchanged.
static bool test_image_transform_default_ini_calls_next_ini() {
    reset_test_state();

    // Set up a simple chain: head -> tail
    tail_transform.next = nullptr;
    tail_transform.ini = tail_ini;  // tail's ini points to our hook
    head_transform.next = &tail_transform;
    head_transform.ini = nullptr; // not used by this focal test

    // Prepare a transform_display instance to pass through.
    transform_display td;
    td.dummy = 123;

    // Call the function under test.
    image_transform_default_ini((const image_transform *)&head_transform, &td);

    // Validate that the tail's ini was invoked exactly once with expected args.
    ASSERT(tail_ini_called, "tail_ini should have been called by image_transform_default_ini");

    // The 'self' parameter should be the tail element itself.
    ASSERT(tail_ini_arg_this == &tail_transform,
           "tail_ini should receive 'self' equal to the tail transform object");

    // The 'that' parameter should equal the passed transform_display pointer.
    ASSERT(tail_ini_arg_that == &td,
           "tail_ini should receive the forwarded 'that' pointer");

    return true;
}

// Test 2:
// Verify that multiple invocations forward the correct parameters each time.
// This ensures portability across different 'that' values and confirms
// that the call is not cached or reused incorrectly.
static bool test_image_transform_default_ini_forwarding_with_multiple_inputs() {
    reset_test_state();

    // Chain setup remains the same: head -> tail
    tail_transform.next = nullptr;
    tail_transform.ini = tail_ini;
    head_transform.next = &tail_transform;

    transform_display td1;
    td1.dummy = 7;
    transform_display td2;
    td2.dummy = 256;

    // First call with td1
    tail_ini_called = false;
    image_transform_default_ini((const image_transform *)&head_transform, &td1);

    ASSERT(tail_ini_called, "First call: tail_ini should be called");
    ASSERT(tail_ini_arg_that == &td1, "First call: forwarded 'that' should be td1");

    // Reset flag for second call
    tail_ini_called = false;
    image_transform_default_ini((const image_transform *)&head_transform, &td2);

    ASSERT(tail_ini_called, "Second call: tail_ini should be called");
    ASSERT(tail_ini_arg_that == &td2, "Second call: forwarded 'that' should be td2");

    // 'self' should still be the tail transform
    ASSERT(tail_ini_arg_this == &tail_transform,
           "Second call: 'self' should be the tail transform");

    return true;
}

// Main test runner invoking the tests.
// We deliberately avoid any external testing frameworks per requirements.
int main(void) {
    std::cout << "Running unit tests for image_transform_default_ini...\n";

    bool all_passed = true;

    if (!test_image_transform_default_ini_calls_next_ini()) {
        std::cerr << "Test 1 failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 1 passed.\n";
    }

    if (!test_image_transform_default_ini_forwarding_with_multiple_inputs()) {
        std::cerr << "Test 2 failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 2 passed.\n";
    }

    if (all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}
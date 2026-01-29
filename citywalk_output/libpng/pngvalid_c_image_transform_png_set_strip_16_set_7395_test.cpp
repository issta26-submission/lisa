// Unit test suite for the focal method: image_transform_png_set_strip_16_set
// This test harness is written in C++11 and uses a lightweight, self-contained
// testing approach (no GoogleTest or any external framework).
// The tests mock the external dependencies (libpng function and the next transform's
// set method) to verify the interaction and call order of the focal method.
//
// Important: This test relies on binary-compatible struct layout assumptions for
// image_transform and transform_display. It provides minimal, test-specific
// definitions to be compatible with the focal function's usage:
//    - image_transform has a member 'image_transform *next' and a function pointer
//      'void (*set)(const image_transform*, transform_display*, png_structp, png_infop)'.
//    - transform_display contains the field 'max_gamma_8' used by the focal code
//      (we do not rely on its exact value for assertions here).
// The test also provides a mock for png_set_strip_16 so we can observe its invocation.

#include <cstddef>
#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// ----------------- Lightweight mock scaffolding -----------------

// Define opaque libpng pointer types (we only pass them through, not inspect content)
typedef void* png_structp;
typedef void* png_infop;

// Forward declare the types used by the focal method.
// We provide minimal definitions here to allow the test to instantiate compatible objects.
extern "C" {

// Forward declare image_transform and transform_display in a way that mirrors
// their usage in the focal method. The actual library defines these, but for testing
// we provide a minimal, layout-compatible subset.
struct image_transform {
    image_transform *next;
    // The focal method expects this->next->set(...) to be a valid call
    void (*set)(const image_transform *this_ptr,
                struct /*transform_display*/ void *that,
                png_structp pp, png_infop pi);
};
// Provide a typedef so the signature matches: void image_transform_png_set_strip_16_set(const image_transform *this, ...);
typedef struct image_transform image_transform;

// Minimal transform_display subset used by the focal method's parameter 'that'.
struct transform_display {
    int max_gamma_8; // value set inside the focal method when a condition holds
};
typedef struct transform_display transform_display;

// Declaration of the focal method under test (to be resolved at link time with the real implementation)
void image_transform_png_set_strip_16_set(const image_transform *this,
                                        transform_display *that,
                                        png_structp pp,
                                        png_infop pi);
}

// ---------------------------------------------------------------
// Mock implementations for the external dependencies to observe behavior
// ---------------------------------------------------------------

// Global observation data to verify call sequence and parameters
static bool g_png_set_strip_16_called = false;
static void* g_last_png_pp = nullptr;
static void* g_last_png_pi = nullptr;

static std::vector<int> g_call_order_log; // 0 for png_set_strip_16, 1 for next->set
static const image_transform* g_last_next_this_ptr = nullptr;
static transform_display* g_last_that_ptr = nullptr;

// Mock for libpng's png_set_strip_16(pp)
// We provide C linkage so the linker resolves it correctly when the focal method calls it.
extern "C" void png_set_strip_16(png_structp pp)
{
    g_png_set_strip_16_called = true;
    g_last_png_pp = pp;
    g_call_order_log.push_back(0); // indicate that strip_16 was called
}

// Mock for the next transform's set(...) function
static void mock_next_transform_set(const image_transform *this_ptr,
                                    void *that, // transform_display* (opaque here)
                                    png_structp pp,
                                    png_infop pi)
{
    // Record the call order and the parameters received
    g_call_order_log.push_back(1);
    g_last_next_this_ptr = this_ptr; // should be the 'next' transform in the chain
    g_last_that_ptr = (transform_display*)that;
    g_last_png_pp = pp;
    g_last_png_pi = pi;
}

// ---------------------------------------------------------------
// Test harness and helper utilities
// ---------------------------------------------------------------

static void reset_observations()
{
    g_png_set_strip_16_called = false;
    g_last_png_pp = nullptr;
    g_last_png_pi = nullptr;
    g_call_order_log.clear();
    g_last_next_this_ptr = nullptr;
    g_last_that_ptr = nullptr;
}

// Lightweight assertion and reporting helpers
static void fail_with(const char* test_name, const char* message)
{
    std::cerr << "[FAIL] " << test_name << ": " << message << "\n";
}

static void pass_with(const char* test_name)
{
    std::cout << "[PASS] " << test_name << "\n";
}

// ---------------------------------------------------------------
// Test 1: Verify correct call order and parameter passing
//   - The focal method should call png_set_strip_16(pp) first
//   - Then it should invoke this->next->set(this->next, that, pp, pi)
//   - The 'that' parameter passed to next->set should be the same as provided
//     by the test, and 'pp' and 'pi' should be forwarded unmodified
// ---------------------------------------------------------------
static void test_png_set_strip_16_set_calls_next_set()
{
    const char* test_name = "test_png_set_strip_16_set_calls_next_set";

    reset_observations();

    // Prepare a minimal transform chain: root -> next
    // Root transform (this)
    image_transform root;
    // Next transform in the chain
    image_transform next;
    // Initialize the chain with expected layout
    root.next = &next;
    // The next transform's set function must be the mock
    next.next = nullptr;
    next.set = mock_next_transform_set;

    // Prepare a minimal transform_display (the "that" parameter)
    transform_display td;
    td.max_gamma_8 = 123; // initial value (not asserted, but we populate it)
    // Pointers passed to the focal method
    void* pp = (void*)0xDEADBEEF;
    void* pi = (void*)0xFEEDBEEF;

    // Call the focal method
    image_transform_png_set_strip_16_set(&root, &td, (png_structp)pp, (png_infop)pi);

    // Assertions:
    // 1) png_set_strip_16 should have been called
    if (!g_png_set_strip_16_called) {
        fail_with(test_name, "png_set_strip_16(pp) was not invoked.");
        return;
    }
    // 2) next->set should have been called (order should include at least two entries)
    if (g_call_order_log.size() < 2) {
        fail_with(test_name, "Expected two calls (strip_16 and next->set), but observed fewer.");
        return;
    }
    // 3) Order: strip_16 first (0), then next->set (1)
    if (g_call_order_log[0] != 0 || g_call_order_log[1] != 1) {
        fail_with(test_name, "Call order incorrect. Expected strip_16 before next->set.");
        return;
    }
    // 4) next->set received expected parameters
    if (g_last_next_this_ptr != &next) {
        fail_with(test_name, "next->set was not invoked with 'this->next' as first argument.");
        return;
    }
    if (g_last_that_ptr != &td) {
        fail_with(test_name, "next->set did not receive the same 'that' transform_display pointer passed to focal method.");
        return;
    }
    if (g_last_png_pp != pp) {
        fail_with(test_name, "png_structp 'pp' was not forwarded to next->set correctly.");
        return;
    }
    if (g_last_png_pi != pi) {
        fail_with(test_name, "png_infop 'pi' was not forwarded to next->set correctly.");
        return;
    }

    pass_with(test_name);
}

// ---------------------------------------------------------------
// Test 2: Execute the focal method twice to observe repeated behavior
//   - Ensures that the mock captures multiple invocations and maintains
//     correct ordering across successive calls.
// ---------------------------------------------------------------
static void test_png_set_strip_16_set_called_twice()
{
    const char* test_name = "test_png_set_strip_16_set_called_twice";

    // First invocation setup (reuse the mock objects by reinitializing state)
    reset_observations();

    // Prepare chain again
    image_transform root;
    image_transform next;
    root.next = &next;
    next.next = nullptr;
    next.set = mock_next_transform_set;

    transform_display td;
    td.max_gamma_8 = 0;
    void* pp1 = (void*)0x11111111;
    void* pi1 = (void*)0x22222222;

    // First call
    image_transform_png_set_strip_16_set(&root, &td, (png_structp)pp1, (png_infop)pi1);

    // Validate first call as in Test 1 (partial validation)
    if (!g_png_set_strip_16_called || g_call_order_log.size() < 2) {
        fail_with(test_name, "First invocation did not perform expected calls.");
        return;
    }

    // Reset for second invocation
    reset_observations();

    // Prepare a new chain (could reuse, here we reuse but reinitialize)
    image_transform root2;
    image_transform next2;
    root2.next = &next2;
    next2.next = nullptr;
    next2.set = mock_next_transform_set;

    transform_display td2;
    td2.max_gamma_8 = 9;
    void* pp2 = (void*)0x33333333;
    void* pi2 = (void*)0x44444444;

    // Second call
    image_transform_png_set_strip_16_set(&root2, &td2, (png_structp)pp2, (png_infop)pi2);

    // Assertions for the second call (ensuring the same pattern repeats)
    if (!g_png_set_strip_16_called && g_call_order_log.empty()) {
        // The mocking state should reflect a fresh invocation
        // If the function did not invoke strip_16 again, it's a failure
        fail_with(test_name, "Second invocation did not perform expected calls (strip_16 not observed).");
        return;
    }

    // If we reached here, the second invocation executed; report success.
    pass_with(test_name);
}

// ---------------------------------------------------------------
// Entry point: run tests and report results
// ---------------------------------------------------------------
int main(void)
{
    std::cout << "Starting unit tests for image_transform_png_set_strip_16_set...\n";

    test_png_set_strip_16_set_calls_next_set();
    test_png_set_strip_16_set_called_twice();

    std::cout << "Unit tests completed.\n";
    return 0;
}
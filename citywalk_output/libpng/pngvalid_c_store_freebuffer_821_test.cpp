// Candidate Keywords (Step 1): 
// - png_store_buffer, psb->prev, store_freebuffer, free, recursion, NULL, malloc, free, chained buffers
// - recursive memory management in a linked-list-like structure
// - test harness in C++11, no Google Test, main-driven tests
// - all tests operate on the actual focal function store_freebuffer(png_store_buffer* psb)

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <math.h>


// Step 2/3: Test scaffolding for the focal method

// We declare the focal function with C linkage to match the signature in the C source (pngvalid.c).
extern "C" {
    // The real implementation is expected to be provided by the project under test (pngvalid.c).
    // This forward declaration allows the test to call the function during execution.
    void store_freebuffer(struct png_store_buffer* psb);
}

// Minimal replica of the png_store_buffer struct to enable test compilation.
// We only rely on the 'prev' member as used by store_freebuffer.
typedef struct png_store_buffer png_store_buffer;
struct png_store_buffer {
    png_store_buffer* prev; // pointer to previous buffer in chain
};

// Helper: simple, non-terminating assertion that logs failures but does not abort tests.
// This aligns with the domain knowledge requirement to use non-terminating assertions.
#define TEST_ASSERT(cond, message)                                    \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::cerr << "Test failure: " << (message)                \
                      << " (\"" << #cond << "\" is false) at "        \
                      << __FILE__ << ":" << __LINE__ << std::endl;    \
            all_tests_passed = false;                                 \
        }                                                               \
    } while (0)

// Global flag to track overall test status (non-terminating style).
static bool all_tests_passed = true;

// Test 1: psb->prev is NULL
// - Purpose: Ensure store_freebuffer handles a single node with no previous node gracefully.
// - Expected: No crash, psb remains valid, and psb->prev remains NULL after call.
bool test_store_freebuffer_no_prev() {
    png_store_buffer* a = (png_store_buffer*)std::malloc(sizeof(png_store_buffer));
    if (a == nullptr) {
        std::cerr << "Memory allocation failed in test_store_freebuffer_no_prev" << std::endl;
        return false;
    }
    a->prev = NULL;

    // Call the focal function
    store_freebuffer(a);

    // Validate: prev should still be NULL, and the buffer itself should be untouched structurally
    bool ok = (a->prev == NULL);

    // Cleanup
    std::free(a);

    // Documentation:
    // - Verifies the true branch where psb->prev is NULL (no recursive call performed).
    TEST_ASSERT(ok, "store_freebuffer with no prev should leave psb->prev == NULL");
    return ok;
}

// Test 2: psb->prev points to a single prior buffer (two-node chain)
 // - Purpose: Ensure recursion occurs for a two-node chain and that the top-level psb's prev is NULL after the call.
// - Expected: a->prev == NULL after the call. The inner node (b) should be freed by the function.
bool test_store_freebuffer_prev_single() {
    // Create two buffers: A -> B
    png_store_buffer* a = (png_store_buffer*)std::malloc(sizeof(png_store_buffer));
    png_store_buffer* b = (png_store_buffer*)std::malloc(sizeof(png_store_buffer));

    if (a == nullptr || b == nullptr) {
        std::cerr << "Memory allocation failed in test_store_freebuffer_prev_single" << std::endl;
        if (a) std::free(a);
        if (b) std::free(b);
        return false;
    }

    a->prev = b;
    b->prev = NULL;

    // Call the focal function
    store_freebuffer(a);

    // Validate: top-level psb's prev should be NULL after freeing its prev chain.
    bool ok = (a->prev == NULL);

    // Cleanup: a itself was not freed by the function; free it now.
    std::free(a);
    // Note: 'b' was freed inside store_freebuffer; no need to free here.

    // Documentation:
    // - Verifies the true branch with a two-node chain and confirms top-level prev is NULL afterward.
    TEST_ASSERT(ok, "store_freebuffer with a single previous buffer should set a->prev to NULL after recursion");
    return ok;
}

// Test 3: psb->prev points to a longer chain (three-node chain: A -> B -> C)
// - Purpose: Ensure recursion traverses multiple levels and unwinds correctly.
// - Expected: After call, a->prev == NULL. Memory for B and C is freed within function (we cannot observe freed blocks directly).
bool test_store_freebuffer_prev_chain3() {
    // Create three buffers: A -> B -> C
    png_store_buffer* a = (png_store_buffer*)std::malloc(sizeof(png_store_buffer));
    png_store_buffer* b = (png_store_buffer*)std::malloc(sizeof(png_store_buffer));
    png_store_buffer* c = (png_store_buffer*)std::malloc(sizeof(png_store_buffer));

    if (a == nullptr || b == nullptr || c == nullptr) {
        std::cerr << "Memory allocation failed in test_store_freebuffer_prev_chain3" << std::endl;
        if (a) std::free(a);
        if (b) std::free(b);
        if (c) std::free(c);
        return false;
    }

    a->prev = b;
    b->prev = c;
    c->prev = NULL;

    // Call the focal function
    store_freebuffer(a);

    // Validate: top-level psb's prev should be NULL after freeing its chain.
    bool ok = (a->prev == NULL);

    // Cleanup: free top-level A; B and C were freed by the function.
    std::free(a);

    // Documentation:
    // - Verifies deeper recursion across three levels without crashing and ensuring a->prev becomes NULL.
    TEST_ASSERT(ok, "store_freebuffer with a three-node chain should set a->prev to NULL after recursion");
    return ok;
}

// Step 3: Test harness runner (non-GTest, main-based)
int main() {
    all_tests_passed = true;
    int tests_ran = 0;

    // Run Test 1
    ++tests_ran;
    bool t1 = test_store_freebuffer_no_prev();
    if (t1) {
        std::cout << "[PASS] test_store_freebuffer_no_prev" << std::endl;
    } else {
        std::cout << "[FAIL] test_store_freebuffer_no_prev" << std::endl;
        all_tests_passed = false;
    }

    // Run Test 2
    ++tests_ran;
    bool t2 = test_store_freebuffer_prev_single();
    if (t2) {
        std::cout << "[PASS] test_store_freebuffer_prev_single" << std::endl;
    } else {
        std::cout << "[FAIL] test_store_freebuffer_prev_single" << std::endl;
        all_tests_passed = false;
    }

    // Run Test 3
    ++tests_ran;
    bool t3 = test_store_freebuffer_prev_chain3();
    if (t3) {
        std::cout << "[PASS] test_store_freebuffer_prev_chain3" << std::endl;
    } else {
        std::cout << "[FAIL] test_store_freebuffer_prev_chain3" << std::endl;
        all_tests_passed = false;
    }

    // Summary
    if (all_tests_passed) {
        std::cout << "ALL TESTS PASSED (" << tests_ran << " tests)." << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED (" << tests_ran << " tests)." << std::endl;
        return 1;
    }
}
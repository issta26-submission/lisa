// test_cleanup.cpp
// A C++11 test suite for the focal cleanup() function from the provided C file.
// This test suite is designed to be compiled alongside the project's C sources.
// It avoids Google Test (GTest) and uses a lightweight, self-contained test harness.
// The tests rely on the project's public declarations for the global state `g`
// and the affected types (e.g., done_t, string_t) provided via an included header.
// Comments explain the intent and coverage rationale for each test case.
//
// Step 1 (Candidate Keywords extracted from focal method):
// - g: a global state object containing:
//     - done: an array of entries with fields len and vec
//     - size: number of entries in done
//     - num: pointer (likely int*)
//     - code: pointer (likely char*)
//     - out: string_t (passed to string_free in cleanup)
// - done[n].len: used to decide whether to free(done[n].vec)
// - free(): standard deallocation for malloc'ed memory
// - NULL checks: guard against freeing NULL pointers
// - string_free(&g.out): cleans up the string member
// - cleanup(): the focal routine under test
// - The test will exercise true/false branches of these conditions as applicable.

#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <enough.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>


// The project should provide a header that declares the needed types and the
// global instance used by cleanup(). Include it here.
// If your build setup uses a different header, replace "enough.h" with the
// appropriate file that exposes:
//   - typedef for the element type g.done[]. (with fields len and vec)
//   - the type of g (e.g., a struct with members: done, size, num, code, out)
//   - extern global g;
//   - extern void cleanup(void);

// Lightweight test framework (non-terminating assertions as requested)
static int tests_run = 0;
static int tests_passed = 0;

#define TEST_OK(msg) \
    do { std::cout << "[OK]   " << msg << std::endl; } while (0)

#define TEST_FAIL(msg) \
    do { std::cerr << "[FAIL] " << msg << std::endl; } while (0)

#define ASSERT_TRUE(cond, msg) \
    do { tests_run++; if (cond) { tests_passed++; TEST_OK(msg); } else { TEST_FAIL(msg); } } while (0)

#define ASSERT_EQ(a, b, msg) \
    do { tests_run++; if ((a) == (b)) { tests_passed++; TEST_OK(msg); } else { TEST_FAIL(msg); std::cerr << "  Expected: " << (b) << "  Got: " << (a) << std::endl; } } while (0)


// Helper: determine the element type of g.done to allocate properly (without relying on
// private type names). We rely on the public type of g.done[0] from the header.
template <typename T>
static T* allocate_done_array(size_t count) {
    using ElemType = decltype(g.done[0]);
    return (ElemType*)malloc(count * sizeof(ElemType));
}

// Test 1: When g.done is NULL, cleanup should safely skip the first block
// and still free g.num and g.code and call string_free on g.out (no-op in tests).
void test_cleanup_no_done_null_ptrs() {
    // Arrange
    g.done = NULL;
    g.size = 0;
    g.num = NULL;
    g.code = NULL;
    // Initialize g.out in a way compatible with the project's API (if needed).
    // The project's string_free is a stub in the provided dependencies, so we
    // primarily verify pointer state after cleanup.

    // Act
    cleanup();

    // Assert
    // The contract: when g.done is NULL, cleanup should not crash and should
    // leave (or establish) the NULL state for the freed pointers.
    ASSERT_TRUE(g.done == NULL, "test_cleanup_no_done_null_ptrs: g.done remains NULL");
    ASSERT_TRUE(g.size == 0, "test_cleanup_no_done_null_ptrs: g.size remains 0");
    ASSERT_TRUE(g.num == NULL, "test_cleanup_no_done_null_ptrs: g.num is NULL after cleanup");
    ASSERT_TRUE(g.code == NULL, "test_cleanup_no_done_null_ptrs: g.code is NULL after cleanup");
}

// Test 2: When g.done is non-NULL and g.size > 0, cleanup should free inner vecs
// where len > 0, then free g.done, reset g.size to 0, and nullify g.done, g.num, g.code.
void test_cleanup_done_with_entries() {
    // Arrange
    // Reset base pointers
    g.done = NULL;
    g.size = 0;
    g.num = NULL;
    g.code = NULL;

    // Allocate g.num and g.code to verify they are freed
    g.num = (int*)malloc(sizeof(int));
    if (g.num) *g.num = 123;

    g.code = (char*)malloc(8);
    if (g.code) strncpy(g.code, "ABCD123", 8);

    // Prepare g.done array of 2 entries
    g.size = 2;
    using ElemType = decltype(g.done[0]);
    g.done = (ElemType*)malloc(g.size * sizeof(ElemType));
    // Initialize entry 0 with len > 0 and a vec to free
    g.done[0].len = 1;
    g.done[0].vec = malloc(16);
    // Initialize entry 1 with len == 0 (vec should not be freed)
    g.done[1].len = 0;
    g.done[1].vec = NULL;

    // Act
    cleanup();

    // Assert
    ASSERT_TRUE(g.done == NULL, "test_cleanup_done_with_entries: g.done freed and NULL");
    ASSERT_TRUE(g.size == 0, "test_cleanup_done_with_entries: g.size reset to 0");
    ASSERT_TRUE(g.num == NULL, "test_cleanup_done_with_entries: g.num freed and NULL");
    ASSERT_TRUE(g.code == NULL, "test_cleanup_done_with_entries: g.code freed and NULL");
    // Note: We cannot reliably observe free() of g.done[0].vec in this test since free is not observable;
    // the code path should have invoked free for non-zero len elements.
}

// Test 3: When g.done is non-NULL but g.size == 0, cleanup should still free g.done
// and set g.done to NULL, in addition to freeing g.num and g.code.
void test_cleanup_done_nonempty_but_zero_size() {
    // Arrange
    g.done = NULL;
    g.size = 0;
    g.num = NULL;
    g.code = NULL;

    // Allocate a non-NULL g.done to satisfy "if (g.done != NULL)"
    using ElemType = decltype(g.done[0]);
    g.done = (ElemType*)malloc(sizeof(ElemType)); // single element
    g.done[0].len = 0;
    g.done[0].vec = NULL;

    // Also allocate g.num and g.code to verify they are freed
    g.num = (int*)malloc(sizeof(int));
    if (g.num) *g.num = 7;
    g.code = (char*)malloc(4);
    if (g.code) strncpy(g.code, "XYZ", 4);

    // Act
    cleanup();

    // Assert
    ASSERT_TRUE(g.done == NULL, "test_cleanup_done_nonempty_but_zero_size: g.done freed and NULL");
    ASSERT_TRUE(g.size == 0, "test_cleanup_done_nonempty_but_zero_size: g.size remains 0");
    ASSERT_TRUE(g.num == NULL, "test_cleanup_done_nonempty_but_zero_size: g.num freed and NULL");
    ASSERT_TRUE(g.code == NULL, "test_cleanup_done_nonempty_but_zero_size: g.code freed and NULL");
}

// Entry point for the test suite
int main() {
    std::cout << "Starting cleanup() unit test suite (C++11)..." << std::endl;

    // Run tests
    test_cleanup_no_done_null_ptrs();
    test_cleanup_done_with_entries();
    test_cleanup_done_nonempty_but_zero_size();

    // Summary
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Total tests run   : " << tests_run << std::endl;
    std::cout << "Total tests passed: " << tests_passed << std::endl;
    std::cout << "Conclusion: " << ((tests_run == tests_passed) ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;

    // Return non-zero if any test failed
    return (tests_run == tests_passed) ? 0 : 1;
}
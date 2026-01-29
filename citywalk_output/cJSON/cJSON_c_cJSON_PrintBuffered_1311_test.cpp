// High-quality C++11 unit tests for cJSON_PrintBuffered in cJSON.c
// This test suite avoids GTest and uses a lightweight, non-terminating assertion approach.
// It also exercises the allocator hooks by providing custom allocation behavior via cJSON_InitHooks.

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Bring C symbols into C++ namespace
extern "C" {
}

// Global test state to control allocation behavior and track calls
static bool g_fail_next_alloc = false; // If true, initial/future allocations fail
static bool g_fail_on_growth = false;  // If true, allocations after the first will fail (simulate growth failure)
static size_t g_alloc_calls = 0;
static size_t g_dealloc_calls = 0;

// Hooks used by cJSON to allocate/deallocate memory during printing
static cJSON_Hooks g_test_hooks;

// Custom allocator that can simulate success/failure scenarios
static void* test_alloc(size_t size)
{
    ++g_alloc_calls;
    // Simulate allocation failure for specific tests
    if (g_fail_next_alloc)
        return nullptr;
    // Simulate growth failure after the first allocation (growth path)
    if (g_fail_on_growth && g_alloc_calls > 1)
        return nullptr;
    return std::malloc(size);
}

// Custom deallocator that tracks calls
static void test_dealloc(void* ptr)
{
    ++g_dealloc_calls;
    std::free(ptr);
}

// Helper to reset test state before each test
static void reset_hooks_for_tests()
{
    g_alloc_calls = 0;
    g_dealloc_calls = 0;
    g_fail_next_alloc = false;
    g_fail_on_growth = false;
    g_test_hooks.allocate = test_alloc;
    g_test_hooks.deallocate = test_dealloc;
    cJSON_InitHooks(&g_test_hooks);
}

// Lightweight, non-terminating test assertions
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " __FILE__ ":" << __LINE__ << " (" #cond ")" << std::endl; \
        ++g_tests_failed; \
    } } while(0)

#define EXPECT_EQ(a,b) \
    do { if(((a) != (b))) { \
        std::cerr << "EXPECT_EQ failed: " __FILE__ ":" << __LINE__ << " (" #a " != " #b ") actual: " << (a) << " vs " << (b) << std::endl; \
        ++g_tests_failed; \
    } } while(0)

#define EXPECT_STREQ(a,b) \
    do { if(std::strcmp((a), (b)) != 0) { \
        std::cerr << "EXPECT_STREQ failed: " __FILE__ ":" << __LINE__ << " (" #a " != " #b ") actual: \"" << (a) << "\" vs \"" << (b) << "\"" << std::endl; \
        ++g_tests_failed; \
    } } while(0)

// Test 1: prebuffer < 0 should return NULL and should not allocate memory
static void test_negative_prebuffer_returns_null()
{
    reset_hooks_for_tests();

    // Create a simple JSON string item
    cJSON* item = cJSON_CreateString("x");
    // Call with negative prebuffer
    char* result = cJSON_PrintBuffered(item, -1, false);

    // Expected: NULL result
    EXPECT_TRUE(result == NULL);

    // Cleanup
    if (item) cJSON_Delete(item);
}

// Test 2: initial allocation failure should return NULL
static void test_initial_allocation_failure_returns_null()
{
    reset_hooks_for_tests();

    // Simulate allocation failure on the next allocation call
    g_fail_next_alloc = true;

    cJSON* item = cJSON_CreateString("x");
    char* result = cJSON_PrintBuffered(item, 2, false);

    // Expected: NULL result due to allocation failure
    EXPECT_TRUE(result == NULL);

    // Cleanup
    if (item) cJSON_Delete(item);

    // Reset
    g_fail_next_alloc = false;
}

// Test 3: allocation succeeds initially, but growth (reallocation) fails -> NULL and buffer freed
static void test_growth_failure_returns_null_on_print()
{
    reset_hooks_for_tests();

    // Enable growth failure after the first allocation
    g_fail_on_growth = true;

    // Use a small prebuffer to force growth during printing
    cJSON* item = cJSON_CreateString("abcdef"); // longer than "a" -> growth will be triggered
    char* result = cJSON_PrintBuffered(item, 1, false);

    // Expected: NULL due to growth failure
    EXPECT_TRUE(result == NULL);

    // Since a buffer was initially allocated, deallocation should have happened in the error path
    EXPECT_TRUE(g_dealloc_calls > 0);

    // Cleanup
    if (item) cJSON_Delete(item);

    // Reset
    g_fail_on_growth = false;
}

// Test 4: successful path - buffer is large enough, returns valid JSON string
static void test_successful_printbuffer_returns_string()
{
    reset_hooks_for_tests();

    // Simple item that should fit into the preallocated buffer
    cJSON* item = cJSON_CreateString("hello");
    char* result = cJSON_PrintBuffered(item, 100, false);

    // Expected: non-NULL and content "\"hello\""
    EXPECT_TRUE(result != NULL);
    EXPECT_STREQ(result, "\"hello\"");

    // Free the allocated buffer via the provided deallocation hook to track usage
    if (result) {
        g_test_hooks.deallocate(result);
        // Since we used the custom deallocator, it should have been recorded
        EXPECT_TRUE(g_dealloc_calls == 1);
    }

    // Cleanup
    if (item) cJSON_Delete(item);
}

// Simple test runner
int main()
{
    test_negative_prebuffer_returns_null();
    test_initial_allocation_failure_returns_null();
    test_growth_failure_returns_null_on_print();
    test_successful_printbuffer_returns_string();

    if (g_tests_failed == 0) {
        std::cout << "All tests passed (" << 4 << " tests)." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with failures: " << g_tests_failed << std::endl;
        return 1;
    }
}
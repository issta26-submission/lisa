/*
This test suite verifies the behavior of cJSON_InitHooks in cJSON.c
using plain C++11 (no GoogleTest). It relies on the public
definition of the cJSON_Hooks struct and the exported global_hooks
instance declared by the cJSON library.

Strategy:
- Use custom wrappers (malloc_fn / free_fn) to observe which hooks
  cJSON_InitHooks actually installs.
- Validate true/false branches of the conditional logic in cJSON_InitHooks:
  - hooks == NULL path resets to defaults (malloc, free, realloc)
  - custom malloc_fn changes allocate
  - custom free_fn changes deallocate
  - both default functions result in reallocate being set to realloc
  - custom wrappers that are distinct from defaults result in reallocate remaining NULL
- Use a lightweight, non-terminating assertion style to maximize coverage.

Notes:
- The tests assume that cJSON.h exposes the internal global_hooks instance
  (extern internal_hooks global_hooks;). This is consistent with the provided
  focal method code which assigns to global_hooks.*. If your build exposes the
  symbol differently, adjust to access global_hooks accordingly.
- Tests create and delete cJSON items to exercise the allocation/deallocation paths.

Compile note:
- Build with a C++11 compiler, linking against cJSON.c (or compile cJSON.c into the project).
- Include path should provide cJSON.h.

*/

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        printf("TEST_FAIL: %s\n", (msg)); \
        ++g_failed_tests; \
    } \
    ++g_total_tests; \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        printf("TEST_FAIL: %s (expected %p, got %p)\n", (msg), (void*)(a), (void*)(b)); \
        ++g_failed_tests; \
    } \
    ++g_total_tests; \
} while (0)

#define EXPECT_NEQ(a, b, msg) do { \
    if ((a) == (b)) { \
        printf("TEST_FAIL: %s (values unexpectedly equal: %p)\n", (msg), (void*)(a)); \
        ++g_failed_tests; \
    } \
    ++g_total_tests; \
} while (0)

///////////////////////////////////////////////////////////////////////////////
// Test helpers and wrappers to observe hook usage
///////////////////////////////////////////////////////////////////////////////

// Global counters for wrapper usage (Test 2)
static size_t g_alloc_calls_main = 0;

// Custom malloc wrapper to observe allocation via malloc_fn
static void* test_alloc_main(size_t size)
{
    ++g_alloc_calls_main;
    return malloc(size);
}

// No-op free wrapper (we still observe free via a separate test if needed)
static size_t g_free_calls_main = 0;
static void test_free_main(void* ptr)
{
    ++g_free_calls_main;
    free(ptr);
}

// Test 4 and 5 wrappers to distinguish from default malloc/free
static size_t g_alloc_calls_case5 = 0;
static void* test_alloc_case5(size_t size)
{
    ++g_alloc_calls_case5;
    return malloc(size);
}
static size_t g_free_calls_case5 = 0;
static void test_free_case5(void* ptr)
{
    ++g_free_calls_case5;
    free(ptr);
}

// Helper to reset global_hooks to defaults
static void reset_hooks_to_defaults()
{
    cJSON_InitHooks(NULL);
}

// Forward declarations of tests
static void test_InitHooks_NULL_resets_to_defaults(void);
static void test_InitHooks_uses_custom_malloc(void);
static void test_InitHooks_uses_custom_free(void);
static void test_InitHooks_defaults_set_reallocate(void);
static void test_InitHooks_custom_wrappers_do_not_enable_reallocate(void);

// Test 1: hooks == NULL should reset to default hooks (allocate=malloc, deallocate=free, reallocate=realloc)
static void test_InitHooks_NULL_resets_to_defaults(void)
{
    // Ensure a known state
    reset_hooks_to_defaults();

    // Act: reset by passing NULL
    cJSON_InitHooks(NULL);

    // Assert: global_hooks should point to default functions
    // (Assuming global_hooks is exposed via cJSON.h)
    extern internal_hooks global_hooks;
    EXPECT_TRUE(global_hooks.allocate == malloc, "global_hooks.allocate should be malloc after NULL init");
    EXPECT_TRUE(global_hooks.deallocate == free, "global_hooks.deallocate should be free after NULL init");
    EXPECT_TRUE(global_hooks.reallocate == realloc, "global_hooks.reallocate should be realloc after NULL init");
}

// Test 2: Providing a non-NULL malloc_fn should switch allocate to the custom function
static void test_InitHooks_uses_custom_malloc(void)
{
    // Reset to defaults before test
    reset_hooks_to_defaults();

    // Prepare hooks with custom malloc
    cJSON_Hooks hooks;
    hooks.malloc_fn = test_alloc_main;
    hooks.free_fn = NULL; // leave deallocate as default (free)

    // Apply hooks
    cJSON_InitHooks(&hooks);

    // Exercise: create an object to trigger allocation via malloc_fn
    g_alloc_calls_main = 0;
    cJSON *obj = cJSON_CreateObject();
    if (obj != NULL) {
        cJSON_Delete(obj);
    }

    // Assert: our custom allocator was used
    EXPECT_TRUE(g_alloc_calls_main > 0, "custom malloc_fn should be invoked during allocation");
}

// Test 3: Providing a non-NULL free_fn should switch deallocate to the custom function
static void test_InitHooks_uses_custom_free(void)
{
    // Reset to defaults before test
    reset_hooks_to_defaults();

    // Prepare hooks with custom free
    cJSON_Hooks hooks;
    hooks.malloc_fn = NULL; // use default malloc
    hooks.free_fn = test_free_main;

    cJSON_InitHooks(&hooks);

    // Exercise: create and then delete an object to trigger deallocation via free_fn
    g_free_calls_main = 0;
    cJSON *obj = cJSON_CreateObject();
    if (obj != NULL) {
        cJSON_Delete(obj);
    }

    // Assert: our custom deallocator was invoked during deallocation
    EXPECT_TRUE(g_free_calls_main > 0, "custom free_fn should be invoked during deallocation");
}

// Test 4: When both malloc_fn and free_fn are the default functions (malloc/free),
// reallocate should be set to realloc
static void test_InitHooks_defaults_set_reallocate(void)
{
    // Reset to defaults then explicitly set to defaults
    reset_hooks_to_defaults();

    cJSON_Hooks hooks;
    hooks.malloc_fn = malloc;
    hooks.free_fn = free;
    cJSON_InitHooks(&hooks);

    extern internal_hooks global_hooks;
    EXPECT_TRUE(global_hooks.allocate == malloc, "allocate should be malloc when malloc_fn == malloc");
    EXPECT_TRUE(global_hooks.deallocate == free, "deallocate should be free when free_fn == free");
    EXPECT_TRUE(global_hooks.reallocate == realloc, "reallocate should be realloc when both default functions are used");
}

// Test 5: Providing custom wrapper functions distinct from defaults should not enable reallocate
static void test_InitHooks_custom_wrappers_do_not_enable_reallocate(void)
{
    // Reset to defaults
    reset_hooks_to_defaults();

    // Set wrappers that are distinct functions from the standard ones
    g_alloc_calls_case5 = 0;
    g_free_calls_case5 = 0;

    cJSON_Hooks hooks;
    hooks.malloc_fn = test_alloc_case5;
    hooks.free_fn = test_free_case5;

    cJSON_InitHooks(&hooks);

    extern internal_hooks global_hooks;
    EXPECT_TRUE(global_hooks.allocate == test_alloc_case5, "allocate should be custom wrapper after InitHooks");
    EXPECT_TRUE(global_hooks.deallocate == test_free_case5, "deallocate should be custom wrapper after InitHooks");
    EXPECT_TRUE(global_hooks.reallocate == NULL, "reallocate should be NULL when custom wrappers are used");

    // Additionally, ensure that allocations use the custom wrapper
    g_alloc_calls_case5 = 0;
    cJSON *obj = cJSON_CreateObject();
    if (obj != NULL) {
        cJSON_Delete(obj);
    }
    EXPECT_TRUE(g_alloc_calls_case5 > 0, "custom malloc wrapper should be invoked during allocation (test5)");
}

// Main to run all tests and report summary
int main(void)
{
    printf("Running cJSON_InitHooks unit tests (C++11)...\n");

    test_InitHooks_NULL_resets_to_defaults();
    test_InitHooks_uses_custom_malloc();
    test_InitHooks_uses_custom_free();
    test_InitHooks_defaults_set_reallocate();
    test_InitHooks_custom_wrappers_do_not_enable_reallocate();

    printf("Tests completed: %d, Failures: %d\n", g_total_tests, g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
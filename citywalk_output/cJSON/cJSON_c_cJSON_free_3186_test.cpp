/*
Step 1 & 2 context (embedded as comments for clarity):
- Focused method under test: cJSON_free(void *object)
  - Behavior: calls global_hooks.deallocate(object); then assigns object = NULL locally.
  - Key dependencies: global_hooks (internal static state in cJSON.c), cJSON_Hooks (structure holding function pointers: allocate and deallocate), cJSON_InitHooks(cJSON_Hooks* hooks) to install hooks.
  - Important candidate keywords/dependency concepts: global_hooks, deallocate, cJSON_InitHooks, cJSON_Hooks, object parameter semantics (pass-by-value), NULL handling.
- Plan for test generation (no GTest; using plain C++11 standard library):
  - Verify that cJSON_free invokes the deallocate hook with the exact pointer passed.
  - Verify behavior when passing NULL to cJSON_free.
  - Verify that reinitializing hooks changes which deallocate function is invoked (ensures hook switching works).
  - Ensure tests are non-terminating on assertion failures; collect and report failures, printing informative messages.
- Domain notes:
  - We cannot observe the external pointer becoming NULL after cJSON_free since the assignment is to a local parameter; tests will confirm deallocation hook invocation instead.
  - Static internal hooks are set via cJSON_InitHooks; tests exercise that flow directly.
*/

// Step 3: Test Suite (C++11) for cJSON_free in cJSON.c
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
#include <cstdint>
#include <limits.h>


extern "C" {
// Include the C header guarding for C linkage
}

// Minimal test harness (non-GTest, prints results, uses non-terminating assertions)
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

// Track last pointer passed to the deallocate hook
static void* g_last_deallocated_ptr = nullptr;

// Hook implementations to observe cJSON_free behavior
static void deallocate_A(void* ptr)
{
    // Record the pointer that was requested to be deallocated
    g_last_deallocated_ptr = ptr;
    // Intentionally do not free to avoid double-free in test scenarios
}
static void deallocate_B(void* ptr)
{
    // Record the pointer that was requested to be deallocated
    g_last_deallocated_ptr = ptr;
    // Intentionally do not free to avoid double-free in test scenarios
}

// Hook containers (allocate is unused for these tests; deallocate is the focus)
static cJSON_Hooks hooks_A = { NULL, deallocate_A };
static cJSON_Hooks hooks_B = { NULL, deallocate_B };

// Lightweight assertion helpers
static void log_failure(const char* test_name, const char* file, int line, const char* message)
{
    std::cerr << "[FAIL] " << test_name << " (" << file << ":" << line << "): " << message << std::endl;
}

#define ASSERT_PTR_EQ(actual, expected, test_name) \
    do { \
        ++g_total_asserts; \
        if ((actual) != (expected)) { \
            log_failure(test_name, __FILE__, __LINE__, "Pointer mismatch: " #actual " != " #expected); \
            ++g_failed_asserts; \
        } \
    } while(0)

#define ASSERT_TRUE(condition, test_name) \
    do { \
        ++g_total_asserts; \
        if (!(condition)) { \
            log_failure(test_name, __FILE__, __LINE__, "Condition is false"); \
            ++g_failed_asserts; \
        } \
    } while(0)

#define ASSERT_EQ_INT(actual, expected, test_name) \
    do { \
        ++g_total_asserts; \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL] " << test_name << " (" << __FILE__ << ":" << __LINE__ \
                      << "): Expected " << (expected) << " but got " << (actual) << std::endl; \
            ++g_failed_asserts; \
        } \
    } while(0)

static void test_cJSON_free_calls_deallocate_with_pointer()
{
    // Test purpose:
    // Ensure that cJSON_free calls the active deallocate hook with the same pointer
    // that was passed to cJSON_free, and that the hook observes the correct pointer.
    // This exercises the critical dependency: global_hooks.deallocate(object).

    const char* test_name = "test_cJSON_free_calls_deallocate_with_pointer";

    g_last_deallocated_ptr = nullptr;

    // Install hooks A (deallocate_A)
    cJSON_InitHooks(&hooks_A);

    // Allocate a memory block to pass to cJSON_free
    void* mem = malloc(64);
    // Execute the function under test
    cJSON_free(mem);

    // Validate that the deallocate hook was invoked with the original pointer
    ASSERT_PTR_EQ(mem, g_last_deallocated_ptr, test_name);
    // Note: We do not free mem here because the deallocation is captured by the hook;
    // the hook in this test intentionally does not free to avoid double frees.
}

static void test_cJSON_free_with_null_pointer()
{
    // Test purpose:
    // Verify that passing NULL to cJSON_free results in the deallocate hook being invoked with NULL.
    const char* test_name = "test_cJSON_free_with_null_pointer";

    g_last_deallocated_ptr = nullptr;
    cJSON_InitHooks(&hooks_A);

    // Call with NULL
    cJSON_free(NULL);

    // Expect the last deallocated pointer to be NULL
    ASSERT_PTR_EQ(nullptr, g_last_deallocated_ptr, test_name);
}

static void test_hooks_switch_updates_last_deallocated()
{
    // Test purpose:
    // Ensure that switching the hooks (A -> B) makes the new deallocate function observe the deallocation.
    const char* test_name = "test_hooks_switch_updates_last_deallocated";

    g_last_deallocated_ptr = nullptr;

    // First, use hooks_A and free a memory block
    cJSON_InitHooks(&hooks_A);
    void* mem1 = malloc(32);
    cJSON_free(mem1);
    ASSERT_PTR_EQ(mem1, g_last_deallocated_ptr, test_name);

    // Switch to hooks_B and free another memory block
    cJSON_InitHooks(&hooks_B);
    void* mem2 = malloc(48);
    cJSON_free(mem2);
    ASSERT_PTR_EQ(mem2, g_last_deallocated_ptr, test_name);
}

int main()
{
    // Run tests
    test_cJSON_free_calls_deallocate_with_pointer();
    test_cJSON_free_with_null_pointer();
    test_hooks_switch_updates_last_deallocated();

    // Summary
    std::cout << "Total assertions: " << g_total_asserts
              << ", Failures: " << g_failed_asserts << std::endl;

    // Return non-zero if any test failed
    return (g_failed_asserts == 0) ? 0 : 1;
}
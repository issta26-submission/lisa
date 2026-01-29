/*
Step 1: Program Understanding and Candidate Keywords
- Focal method: cJSON_CreateNull
- Core behavior: Allocate a new cJSON item via cJSON_New_Item(&global_hooks) and, if allocation succeeds, set item->type to cJSON_NULL; return the created item (may be NULL if allocation fails).
- Critical dependencies and terms (Candidate Keywords):
  - cJSON_New_Item
  - global_hooks
  - cJSON_NULL
  - item (cJSON *)
  - cJSON_IsNull (verification helper)
  - cJSON_InitHooks (to swap the allocation hooks used by cJSON_New_Item)
  - cJSON_Hooks (structure holding malloc_fn, free_fn)
  - malloc / free (underlying allocator)
  - cJSON_Delete (cleanup)

This test suite targets the exact logic path of cJSON_CreateNull, including the success path (allocation succeeds) and the failure path (allocation fails via a hooked allocator). It uses the public cJSON_CreateNull API and cJSON_IsNull for verification, without touching private/static internals directly.
*/

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


extern "C" {
}

// Simple test framework (non-terminating assertions)
namespace TestFramework {
    static int g_total = 0;
    static int g_failed = 0;

    inline void log_failure(const char* msg, const char* file, int line) {
        std::cerr << "[FAIL] " << file << ":" << line << " - " << msg << "\n";
        ++g_failed;
    }

    // Macro-like helpers (non-terminating)
    #define ASSERT(cond, msg) do { \
        ++TestFramework::g_total; \
        if(!(cond)) { TestFramework::log_failure(msg, __FILE__, __LINE__); } \
    } while(0)

    inline int summary() {
        std::cout << "Total tests: " << g_total << ", Failures: " << g_failed << "\n";
        return g_failed;
    }
}

// Domain knowledge notes embedded as comments for maintainers
// - We will test cJSON_CreateNull in two scenarios by swapping the allocator via cJSON_InitHooks:
//   1) Normal allocation path: malloc/free wrapper that behaves like the system allocator.
//   2) Allocation failure path: a malloc wrapper that returns NULL on first call to simulate failure.
// - After each test, we revert the hooks to standard system malloc/free to avoid cross-test contamination.

//
// Allocation wrappers to manipulate cJSON's memory behavior
//

static void* std_malloc(size_t size) {
    return malloc(size);
}
static void std_free(void* ptr) {
    free(ptr);
}

// Hook structure for standard allocation (acts like default behavior)
static cJSON_Hooks std_hooks = { std_malloc, std_free };

// Allocation wrapper that simulates failure on first allocation call
static void* fail_once_malloc(size_t size) {
    (void)size;
    // Fail on first allocation attempt to induce item == NULL in cJSON_CreateNull
    static int first_call = 1;
    if (first_call) {
        first_call = 0;
        return NULL;
    }
    // After the first failed attempt, behave normally
    return malloc(size);
}
static void fail_once_free(void* ptr) {
    // Just free if ever called (not used in the failing path, but provide a safe adapter)
    free(ptr);
}
static cJSON_Hooks fail_once_hooks = { fail_once_malloc, fail_once_free };

//
// Test 1: Normal path - cJSON_CreateNull should allocate and set type to cJSON_NULL
//
bool test_CreateNull_Normal_path() {
    using namespace TestFramework;

    // Install standard hooks (system malloc/free)
    cJSON_InitHooks(&std_hooks);

    // Call the function under test
    cJSON *item = cJSON_CreateNull();

    bool ok = true;
    if (item == NULL) {
        ASSERT(false, "cJSON_CreateNull returned NULL on normal path (expected non-NULL).");
        ok = false;
    } else {
        // Verify that the created item represents a JSON null
        // Use public API for verification to avoid relying on internal structure details
        if (!cJSON_IsNull(item)) {
            ASSERT(false, "Created item is not recognized as JSON null by cJSON_IsNull.");
            ok = false;
        }
        // Cleanup
        cJSON_Delete(item);
    }

    // Reset to standard hooks after test
    cJSON_InitHooks(&std_hooks);

    return ok;
}

//
// Test 2: Failure path - simulate allocation failure to ensure NULL is returned
//
bool test_CreateNull_AllocationFailure_path() {
    using namespace TestFramework;

    // Install failing hooks: first allocation attempt will fail
    cJSON_InitHooks(&fail_once_hooks);

    cJSON *item = cJSON_CreateNull();

    bool ok = true;
    if (item != NULL) {
        // If allocation unexpectedly succeeded, that's a test failure for this path
        ASSERT(false, "cJSON_CreateNull should return NULL when allocation fails, but returned non-NULL.");
        cJSON_Delete(item);
        ok = false;
    }

    // Reset to standard hooks after test
    cJSON_InitHooks(&std_hooks);

    return ok;
}

//
// Main: drive tests and print summary (non-terminating, per instructions)
//
int main() {
    // Run tests
    bool t1 = test_CreateNull_Normal_path();
    bool t2 = test_CreateNull_AllocationFailure_path();

    // Provide a simple summary for the user
    int failures = 0;
    failures += (!t1);
    failures += (!t2);

    // Print a concise report
    if (failures == 0) {
        std::cout << "[OK] All tests passed.\n";
    } else {
        std::cout << "[ERROR] " << failures << " test(s) failed.\n";
    }

    // Return non-zero if there were failures to aid integration with build scripts
    return failures;
}
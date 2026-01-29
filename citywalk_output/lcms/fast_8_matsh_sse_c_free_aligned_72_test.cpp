// Test suite for the focal method free_aligned defined in fast_8_matsh_sse.c
// Strategy:
// - Intercept the internal _cmsFree call via a macro redirection.
// - Include the focal C file after redefining _cmsFree to a test hook.
// - Exercise multiple scenarios and verify that free_aligned forwards the correct
//   ContextID and real_ptr values to _cmsFree (captured by the test hook).
// - Use a small, non-terminating test harness (print-based) suitable when gtest is not allowed.

#include <fast_float_internal.h>
#include <stdio.h>
#include <fast_8_matsh_sse.c>
#include <cpuid.h>
#include <stdlib.h>


// Global state to capture calls to the mock _cmsFree
static void* g_last_cmsContextID = NULL;
static void* g_last_cmsPtr = NULL;
static int g_cmsFree_call_count = 0;

// Forward declaration for the test hook (signature mirrors the real _cmsFree usage).
static void test_cmsFree(void* ContextID, void* Ptr);

// Redefine _cmsFree to redirect to the test hook.
// We cast the forwarded arguments to void* to accommodate various cmsContext typedefs.
#define _cmsFree(ContextID, Ptr) test_cmsFree((void*)(ContextID), (void*)(Ptr))

// Include the focal implementation after setting up the test hook macro.
// The included file will call _cmsFree(...) inside free_aligned, which now routes here.

// Prototype helpers for tests
static int test_free_aligned_basic(void);
static int test_free_aligned_null_ptr(void);
static int test_free_aligned_multiple_calls(void);

// Test case 1: Ensure free_aligned forwards the correct ContextID and real_ptr
// when real_ptr is non-NULL.
static int test_free_aligned_case_basic(void) {
    // Reset captured state
    g_last_cmsContextID = NULL;
    g_last_cmsPtr = NULL;
    g_cmsFree_call_count = 0;

    // Prepare a sample XMatShaper8Data instance
    XMatShaper8Data a;
    a.ContextID = (cmsContext)0x1234;
    a.real_ptr = (void*)0x1000;

    // Invoke the focal function
    free_aligned(&a);

    // Assertions (non-terminating): verify that the hook captured the right values and call count
    if (g_cmsFree_call_count != 1) {
        printf("Test Case Basic: FAIL - expected 1 cmsFree call, got %d\n", g_cmsFree_call_count);
        return 0;
    }
    if (g_last_cmsContextID != (void*)0x1234) {
        printf("Test Case Basic: FAIL - ContextID mismatch in cmsFree call\n");
        return 0;
    }
    if (g_last_cmsPtr != (void*)0x1000) {
        printf("Test Case Basic: FAIL - Pointer mismatch in cmsFree call\n");
        return 0;
    }

    printf("Test Case Basic: PASS - free_aligned forwarded ContextID and real_ptr correctly\n");
    return 1;
}

// Test case 2: Ensure free_aligned handles NULL real_ptr gracefully
static int test_free_aligned_case_null_ptr(void) {
    // Reset captured state
    g_last_cmsContextID = NULL;
    g_last_cmsPtr = NULL;
    g_cmsFree_call_count = 0;

    // Prepare a new XMatShaper8Data instance with NULL real_ptr
    XMatShaper8Data a;
    a.ContextID = (cmsContext)0xABCD;
    a.real_ptr = NULL;

    // Invoke the focal function
    free_aligned(&a);

    // Assertions
    if (g_cmsFree_call_count != 1) {
        printf("Test Case NULL_PTR: FAIL - expected 1 cmsFree call, got %d\n", g_cmsFree_call_count);
        return 0;
    }
    if (g_last_cmsContextID != (void*)0xABCD) {
        printf("Test Case NULL_PTR: FAIL - ContextID mismatch in cmsFree call\n");
        return 0;
    }
    if (g_last_cmsPtr != NULL) {
        printf("Test Case NULL_PTR: FAIL - Pointer should be NULL in cmsFree call\n");
        return 0;
    }

    printf("Test Case NULL_PTR: PASS - free_aligned handled NULL real_ptr by forwarding NULL\n");
    return 1;
}

// Test case 3: Ensure multiple consecutive calls are correctly tracked
static int test_free_aligned_case_multiple_calls(void) {
    // Reset captured state
    g_last_cmsContextID = NULL;
    g_last_cmsPtr = NULL;
    g_cmsFree_call_count = 0;

    // First object
    XMatShaper8Data a1;
    a1.ContextID = (cmsContext)0x1;
    a1.real_ptr = (void*)0x100;
    free_aligned(&a1);

    // Second object
    XMatShaper8Data a2;
    a2.ContextID = (cmsContext)0x2;
    a2.real_ptr = (void*)0x200;
    free_aligned(&a2);

    // Assertions
    if (g_cmsFree_call_count != 2) {
        printf("Test Case MULTI: FAIL - expected 2 cmsFree calls, got %d\n", g_cmsFree_call_count);
        return 0;
    }
    // Last call should reflect the second object's values
    if (g_last_cmsContextID != (void*)0x2) {
        printf("Test Case MULTI: FAIL - ContextID of last call mismatch\n");
        return 0;
    }
    if (g_last_cmsPtr != (void*)0x200) {
        printf("Test Case MULTI: FAIL - Pointer of last call mismatch\n");
        return 0;
    }

    printf("Test Case MULTI: PASS - multiple consecutive calls tracked correctly\n");
    return 1;
}

// Implementation of the test hook that captures parameters passed by free_aligned
static void test_cmsFree(void* ContextID, void* Ptr) {
    g_last_cmsContextID = ContextID;
    g_last_cmsPtr = Ptr;
    g_cmsFree_call_count++;
}

// Entry point for the test suite
int main(void) {
    int total = 0;
    int passed = 0;

    printf("Starting unit tests for free_aligned...\n");

    // Run tests with explanatory comments in code
    total++;
    if (test_free_aligned_case_basic()) passed++, total;

    total++;
    if (test_free_aligned_case_null_ptr()) passed++, total;

    total++;
    if (test_free_aligned_case_multiple_calls()) passed++, total;

    printf("Unit tests finished: %d/%d passed.\n", passed, total);

    // Non-terminating exit: return 0 if all passed, non-zero otherwise
    return (passed == total) ? 0 : 1;
}

// Note:
// - This test suite relies on the ability to export the types from fast_8_matsh_sse.c
//   (e.g., XMatShaper8Data and cmsContext) and on the internal function
//   free_aligned being defined there. The helper hook _cmsFree redirects calls to
//   test_cmsFree, enabling verification of the function's behavior without
//   requiring a full CMS runtime or real memory management.
// - The tests use simple print statements and return values to keep compatibility
//   with environments where a full testing framework is not available.
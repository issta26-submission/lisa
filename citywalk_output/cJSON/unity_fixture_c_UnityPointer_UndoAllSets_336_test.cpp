// Test suite for UnityPointer_UndoAllSets in unity_fixture.c
// This test suite targets the focal function and its dependencies without GTest.
// It uses a lightweight in-file test harness with non-terminating style checks to maximize coverage.

#include <unity_internals.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <unity_fixture.h>


// Step 1: Explain candidate keywords (for alignment with Step 1 guidance)
 // Candidate Keywords:
 // - UnityPointer_UndoAllSets
 // - UnityPointer_Set
 // - pointer_index
 // - pointer_store
 // - old_value
 // - pointer
 // - UnityPointer_Init

// Step 2: Declare external C interfaces of the Unity pointer facility.
// We assume these are provided by the Unity fixture (unity_fixture.c/.h).
extern "C" {
    void UnityPointer_Init(void);
    void UnityPointer_Set(void** pointer, void* newValue, int line);
    void UnityPointer_UndoAllSets(void);
}

// Lightweight, non-terminating test harness
static int g_tests_passed = 0;
static int g_tests_failed = 0;

static void log_failure(const char* testName, const char* message) {
    fprintf(stderr, "Test %s FAILED: %s\n", testName, message);
}

#define TEST_NAME __func__

#define CHECK_TRUE(cond, message) do { \
    if (!(cond)) { \
        log_failure(TEST_NAME, (message)); \
        g_tests_failed++; \
    } else { \
        g_tests_passed++; \
    } \
} while (0)

#define CHECK_PTR_EQ(actual, expected) CHECK_TRUE((actual) == (expected), "Pointer value mismatch")
#define CHECK_INT_EQ(actual, expected) CHECK_TRUE((int)(actual) == (int)(expected), "Integer value mismatch")

// Test 1: No pointer sets performed -> UndoAllSets should be a no-op
static void test_UndoAllSets_noSets(void)
{
    // Reinitialize the Unity pointer tracking to a clean state
    UnityPointer_Init();

    // Setup a simple pointer-to-int scenario
    int base = 42;
    int* p = &base;

    // No UnityPointer_Set calls; UndoAllSets should not modify p
    UnityPointer_UndoAllSets();

    CHECK_PTR_EQ(p, &base);
}

// Test 2: Multiple independent pointer sets are undone in LIFO order
static void test_UndoAllSets_multiplePointers(void)
{
    UnityPointer_Init();

    int a = 10;
    int b = 20;
    int* p = &a; // first pointer
    int* q = &b; // second pointer

    // Set p to point to b
    UnityPointer_Set((void**)&p, (void*)q, __LINE__);
    CHECK_PTR_EQ(p, q); // p should now point to b

    // Set q to point to a
    UnityPointer_Set((void**)&q, (void*) &a, __LINE__);
    CHECK_PTR_EQ(q, &a); // q should now point to a

    // Undo all sets; both pointers should revert to their original values (p->a, q->b)
    UnityPointer_UndoAllSets();

    CHECK_PTR_EQ(p, &a);
    CHECK_PTR_EQ(q, &b);
}

// Test 3: Setting a pointer to the same value still records the old value;
// UndoAllSets should restore the same pointer to its original value without corruption
static void test_UndoAllSets_setToSameValue(void)
{
    UnityPointer_Init();

    int x = 7;
    int* px = &x;

    // Change px to point to the same address it already points to
    UnityPointer_Set((void**)&px, (void*)px, __LINE__);
    // The pointer remains the same; Undo should still work and keep consistency
    UnityPointer_UndoAllSets();

    CHECK_PTR_EQ(px, &x);
}

// Test 4: Ensure UndoAllSets can handle repeated init followed by undos without crash
static void test_UndoAllSets_reinitAndUndo(void)
{
    // First run: perform one set and undo
    UnityPointer_Init();
    int m = 100;
    int n = 200;
    int* pm = &m;
    int* pn = &n;
    UnityPointer_Set((void**)&pm, (void*)pn, __LINE__);
    CHECK_PTR_EQ(pm, pn);
    UnityPointer_UndoAllSets();
    CHECK_PTR_EQ(pm, &m);
    // Second run: ensure a clean slate works similarly
    UnityPointer_Init();
    UnityPointer_Set((void**)&pm, (void*)pn, __LINE__);
    CHECK_PTR_EQ(pm, pn);
    UnityPointer_UndoAllSets();
    CHECK_PTR_EQ(pm, &m);
}

// Main entry: execute all tests and summarize
int main(void)
{
    // Run tests
    test_UndoAllSets_noSets();
    test_UndoAllSets_multiplePointers();
    test_UndoAllSets_setToSameValue();
    test_UndoAllSets_reinitAndUndo();

    // Print summary
    printf("Tests passed: %d\n", g_tests_passed);
    printf("Tests failed: %d\n", g_tests_failed);

    // Exit code indicates success only if there were no failures
    return (g_tests_failed == 0) ? 0 : 1;
}
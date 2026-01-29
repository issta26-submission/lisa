/*
 * Unit tests for UnityPointer_Set in unity_fixture.c
 * - Built with C++11 (no GTest)
 * - Uses the Unity test framework's exposed interfaces (extern "C" includes)
 * - Focuses on true/false coverage of the function's observable behavior
 * - Tests are designed to be executable and self-contained when linked with
 *   the provided Unity sources (unity_fixture.c, unity_internals.h, etc.)
 *
 * Note: These tests rely on UnityPointer_Init, UnityPointer_Set and
 * UnityPointer_UndoAllSets being available with C linkage from the Unity harness.
 * They verify the functional behavior of updating a pointer and undoing all
 * pointer sets without inspecting internal static state directly.
 */

#include <unity_internals.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <unity_fixture.h>


// Bring in the Unity headers to obtain correct type definitions (UNITY_LINE_TYPE, etc.)
// and the declarations for UnityPointer_* helpers.
extern "C" {

    // Prototypes for the UnityPointer helpers used by UnityPointer_Set
    void UnityPointer_Init(void);
    void UnityPointer_Set(void** pointer, void* newValue, UNITY_LINE_TYPE line);
    void UnityPointer_UndoAllSets(void);
}

// Helper to report test results in a simple harness-style fashion
static bool report_result(const char* test_name, bool passed) {
    if (passed) {
        std::printf("[PASS] %s\n", test_name);
    } else {
        std::printf("[FAIL] %s\n", test_name);
    }
    return passed;
}

/*
 * Test 1: Basic Set and Undo
 * - Initialize the Unity pointer tracking
 * - Set a pointer to a new value and verify the pointer now points to that value
 * - Undo all pointer sets and verify the pointer is restored to its original (nullptr)
 * Rationale: Validates the core observable behavior of UnityPointer_Set and
 * UnityPointer_UndoAllSets in the happy path.
 */
static bool test_UnityPointer_Set_BasicSetAndUndo() {
    // Reset internal state for a clean test
    UnityPointer_Init();

    // Setup a real int pointer that will be tracked by UnityPointer_Set
    int* target = nullptr;
    int value = 12345; // local value to point to
    // Call the function under test:
    // pointer is the address of the pointer variable to be updated
    UnityPointer_Set(reinterpret_cast<void**>(&target), reinterpret_cast<void*>(&value), 0);

    // After the call, target should point to value
    if (target != &value) {
        return report_result("test_UnityPointer_Set_BasicSetAndUndo (pointer update)",
                             false);
    }

    // Undo all pointer sets and verify the pointer reverts to its original state (nullptr)
    UnityPointer_UndoAllSets();
    if (target != nullptr) {
        return report_result("test_UnityPointer_Set_BasicSetAndUndo (undo restores original)",
                             false);
    }

    return report_result("test_UnityPointer_Set_BasicSetAndUndo", true);
}

/*
 * Test 2: Multiple Independent Pointers
 * - Initialize tracker
 * - Set two independent pointers to two distinct values
 * - Verify both pointers point to their respective values
 * - Undo all sets and verify both pointers revert to their originals (nullptr)
 * Rationale: Ensures that successive calls to UnityPointer_Set do not interfere
 * with each other's tracked old values and undo restores correctly.
 */
static bool test_UnityPointer_Set_MultipleIndependentPointers() {
    UnityPointer_Init();

    int* ptrA = nullptr;
    int* ptrB = nullptr;

    int valueA = 42;
    int valueB = 84;

    UnityPointer_Set(reinterpret_cast<void**>(&ptrA), reinterpret_cast<void*>(&valueA), 0);
    UnityPointer_Set(reinterpret_cast<void**>(&ptrB), reinterpret_cast<void*>(&valueB), 0);

    if (ptrA != &valueA) {
        return report_result("test_UnityPointer_Set_MultipleIndependentPointers (ptrA set)",
                             false);
    }
    if (ptrB != &valueB) {
        return report_result("test_UnityPointer_Set_MultipleIndependentPointers (ptrB set)",
                             false);
    }

    UnityPointer_UndoAllSets();

    if (ptrA != nullptr) {
        return report_result("test_UnityPointer_Set_MultipleIndependentPointers (ptrA undo)",
                             false);
    }
    if (ptrB != nullptr) {
        return report_result("test_UnityPointer_Set_MultipleIndependentPointers (ptrB undo)",
                             false);
    }

    return report_result("test_UnityPointer_Set_MultipleIndependentPointers", true);
}

/*
 * Test 3: Undo Idempotence
 * - After performing a set and an undo, call UndoAllSets again
 * - The second Undo should be a no-op and not crash or alter values further
 * Rationale: Verifies safe behavior of UndoAllSets when invoked multiple times.
 */
static bool test_UnityPointer_Set_UndoIdempotence() {
    UnityPointer_Init();

    int* target = nullptr;
    int value = 555;
    UnityPointer_Set(reinterpret_cast<void**>(&target), reinterpret_cast<void*>(&value), 0);

    if (target != &value) {
        return report_result("test_UnityPointer_Set_UndoIdempotence (initial set)",
                             false);
    }

    UnityPointer_UndoAllSets();
    if (target != nullptr) {
        return report_result("test_UnityPointer_Set_UndoIdempotence (first undo)",
                             false);
    }

    // Second undo should be safe (no change expected)
    UnityPointer_UndoAllSets();
    if (target != nullptr) {
        return report_result("test_UnityPointer_Set_UndoIdempotence (second undo safe)",
                             false);
    }

    return report_result("test_UnityPointer_Set_UndoIdempotence", true);
}

/*
 * Simple test runner
 * - Executes each test and reports results
 * - Returns non-zero if any test failed
 */
int main() {
    int all_passed = 1;

    all_passed &= test_UnityPointer_Set_BasicSetAndUndo();
    all_passed &= test_UnityPointer_Set_MultipleIndependentPointers();
    all_passed &= test_UnityPointer_Set_UndoIdempotence();

    if (all_passed) {
        std::printf("All UnityPointer_Set tests PASSED.\n");
        return 0;
    } else {
        std::printf("One or more UnityPointer_Set tests FAILED.\n");
        return 1;
    }
}
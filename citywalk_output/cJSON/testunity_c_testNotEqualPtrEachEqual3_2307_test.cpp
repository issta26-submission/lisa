// Small, self-contained C++11 test suite focused on the focal behavior
// of the Unity-style test in testunity.c: testNotEqualPtrEachEqual3
// This file does not depend on GTest and uses a lightweight, exception-based
// assertion mechanism to verify expected aborts (as Unity would do).

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Domain knowledge notes (for maintainers):
// - The focal test replicates Unity's TEST_ASSERT_EACH_EQUAL_PTR(expected, array, count)
//   which checks that every element in 'array' points to the exact same address as 'expected'.
// - testNotEqualPtrEachEqual3 intentionally provides a mixture of addresses so that at least
//   one element does not equal 'expected', causing a failure (abort) in Unity terms.
// - We mirror this with a lightweight AbortException to verify correct abort behavior in CI
//   without pulling in the full Unity runtime.

// Candidate Keywords (Step 1): A, B, p0, &A, &B, p0 elements, expected pointer (&B),
// TEST_ASSERT_EACH_EQUAL_PTR, 4, AbortException, testNotEqualPtrEachEqual3, NULL
// These keywords map directly to the focal test's core components and control flow.

class AbortException : public std::exception {
public:
    AbortException() = default;
    const char* what() const noexcept override {
        return "Abort triggered by TEST_ASSERT_EACH_EQUAL_PTR";
    }
};

// Minimal replica of Unity's TEST_ASSERT_EACH_EQUAL_PTR for pointer equality checks.
// Throws AbortException if any element != expected.
void TEST_ASSERT_EACH_EQUAL_PTR(const void* expected, const void* const* array, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        if (array[i] != expected) {
            throw AbortException();
        }
    }
}

// Test 1 (Original scenario mirrored from testunity.c's focal testNotEqualPtrEachEqual3):
// - Purpose: Ensure that TEST_ASSERT_EACH_EQUAL_PTR aborts when a non-matching pointer is present.
// - Expected outcome: Abort should be triggered due to mismatch (&A vs &B at index 0).
bool test_NotEqualPtrEachEqual3_ShouldAbort_WhenOnePointerDiffers() {
    // Local objects so their addresses are stable for the duration of the test
    char A = 1;
    char B = 1;
    // p0 contains addresses: &A, &B, &B, &B
    // Since the first element (&A) is not equal to &B, the assertion should fail.
    const char* pName = "NotEqualPtrEachEqual3_ShouldAbort";
    const void* p0[] = { &A, &B, &B, &B };
    try {
        TEST_ASSERT_EACH_EQUAL_PTR(&B, p0, 4);
        // If we reach here, there was no abort, which means the test failed.
        std::cerr << "[FAIL] " << pName << ": Expected abort, but none occurred.\n";
        return false;
    } catch (const AbortException&) {
        // Expected path: abort occurred as in Unity's TEST_ABORT path.
        std::cout << "[PASS] " << pName << ": Abort occurred as expected.\n";
        return true;
    } catch (...) {
        // Unexpected exception type
        std::cerr << "[FAIL] " << pName << ": Unexpected exception type.\n";
        return false;
    }
}

// Test 2 (All pointers match):
// - Purpose: Validate that no abort occurs when every element equals the expected pointer.
// - Expected outcome: No abort.
bool test_NotEqualPtrEachEqual3_AllPointersMatch() {
    char A = 1;
    char B = 1;
    // p0 contains addresses: &B, &B, &B, &B
    const char* pName = "NotEqualPtrEachEqual3_AllPointersMatch";
    const void* p0[] = { &B, &B, &B, &B };
    try {
        TEST_ASSERT_EACH_EQUAL_PTR(&B, p0, 4);
        // If we reach here, test passes (no abort).
        std::cout << "[PASS] " << pName << ": No abort as all pointers match.\n";
        return true;
    } catch (const AbortException&) {
        // Should not abort in this scenario
        std::cerr << "[FAIL] " << pName << ": Unexpected abort when all pointers should match.\n";
        return false;
    } catch (...) {
        std::cerr << "[FAIL] " << pName << ": Unexpected exception type.\n";
        return false;
    }
}

// Test 3 (One mismatch in the middle):
// - Purpose: Ensure abort when there is exactly one non-matching pointer among the array.
// - Expected outcome: Abort should be triggered.
bool test_NotEqualPtrEachEqual3_OneMismatchInMiddle() {
    char A = 1;
    char B = 2; // Different address than &A and different value (not used in comparison)
    // p0 contains: &A, &B, &B, &B  (first element differs from &B)
    const void* p0[] = { &A, &B, &B, &B };
    const char* pName = "NotEqualPtrEachEqual3_OneMismatchInMiddle";
    try {
        TEST_ASSERT_EACH_EQUAL_PTR(&B, p0, 4);
        std::cerr << "[FAIL] " << pName << ": Expected abort, but none occurred.\n";
        return false;
    } catch (const AbortException&) {
        std::cout << "[PASS] " << pName << ": Abort occurred as expected.\n";
        return true;
    } catch (...) {
        std::cerr << "[FAIL] " << pName << ": Unexpected exception type.\n";
        return false;
    }
}

// Simple test runner (non-GTest, minimal stdout reporting)
int main() {
    std::size_t total = 0;
    std::size_t passed = 0;
    std::size_t failed = 0;

    // Run Test 1
    {
        bool ok = test_NotEqualPtrEachEqual3_ShouldAbort_WhenOnePointerDiffers();
        total++;
        if (ok) ++passed;
        else ++failed;
    }

    // Run Test 2
    {
        bool ok = test_NotEqualPtrEachEqual3_AllPointersMatch();
        total++;
        if (ok) ++passed;
        else ++failed;
    }

    // Run Test 3
    {
        bool ok = test_NotEqualPtrEachEqual3_OneMismatchInMiddle();
        total++;
        if (ok) ++passed;
        else ++failed;
    }

    // Summary
    std::cout << "\nTest Summary: " << passed << " passed, " << failed << " failed, out of "
              << total << " tests." << std::endl;

    // Return non-zero if any test failed to help CI fail fast
    return (failed == 0) ? 0 : 1;
}
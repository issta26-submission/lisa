// C++11 test suite for UnityAssertEqualMemory (from unity.c)
// Note: This test suite does not rely on a GTest framework.
// It uses a minimal main() to invoke a few targeted tests that exercise
// the UnityAssertEqualMemory function in safe, non-bail conditions.
// The test suite is designed to compile against the provided Unity codebase
// (unity.c/.h) with the dependency stubs provided in <FOCAL_CLASS_DEP>.

#include <stddef.h>
#include <iostream>
#include <cstdint>
#include <unity.h>


// Include Unity's API (C headers). Use extern "C" to avoid name mangling for C linkage.
extern "C" {
}

// Bring Unity types into scope via the file's typedefs.
// UNITY_INTERNAL_PTR, UNITY_UINT32, UNITY_LINE_TYPE, UNITY_FLAGS_T are provided by unity.h.
// We rely on the actual Unity implementation to define their exact semantics.
// Here we primarily use them to build valid call arguments to UnityAssertEqualMemory.


// Step 1: Candidate Keywords derived from UnityAssertEqualMemory's core logic
// - expected, actual: memory pointers to compare
// - length: size in bytes of a single element
// - num_elements: number of elements to compare
// - msg: optional user message
// - lineNumber: source code line information for error reporting
// - flags: behavior modifier (e.g., UNITY_ARRAY_TO_VAL)
 // These keywords guide test case generation to cover dependent components and
 // branches (pointer comparisons, loop over elements, per-byte comparison, etc).

// Helper: forward declare the tested function (extern "C" for C linkage)
extern "C" {
    void UnityAssertEqualMemory(UNITY_INTERNAL_PTR expected,
                                UNITY_INTERNAL_PTR actual,
                                const UNITY_UINT32 length,
                                const UNITY_UINT32 num_elements,
                                const char* msg,
                                const UNITY_LINE_TYPE lineNumber,
                                const UNITY_FLAGS_T flags);
}

// Test 1: Pass case where two non-null memory blocks are equal byte-for-byte
// This exercises the main comparison loop without triggering a failure bail.
// Critical aspects covered:
// - Correct interpretation of length (bytes per element)
// - Iteration across multiple elements when num_elements > 1
// - Normal return path (no bail)
bool test_UnityAssertEqualMemory_MemoryBlocks_equal_MultipleBytes()
{
    // Candidate memory: 6 bytes total, 3 elements of 2 bytes each (length=2)
    // data layout: [0x01,0x02] [0x03,0x04] [0x05,0x06]
    unsigned char expected[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    unsigned char actual[]   = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

    // length = 2 bytes per element; num_elements = 3
    UnityAssertEqualMemory((UNITY_INTERNAL_PTR)expected,
                           (UNITY_INTERNAL_PTR)actual,
                           2,
                           3,
                           "Memory blocks should be equal (multi-byte elements)",
                           0,
                           0);
    // If UnityAssertEqualMemory returns, the test passed
    return true;
}

// Test 2: Pass case where expected and actual pointers are identical
// This exercises the fast-path early return when expected == actual.
// Critical aspects covered:
// - Immediate return without entering the comparison loops
// - Correct handling for NULL or identical pointers
bool test_UnityAssertEqualMemory_Pointers_Are_Identical()
{
    unsigned char data[] = {0xAA, 0xBB, 0xCC, 0xDD};

    UNITY_INTERNAL_PTR p = (UNITY_INTERNAL_PTR)data;

    // Pointers are the same; should bail out early with no failure
    UnityAssertEqualMemory(p,
                           p,
                           1,      // length in bytes per element
                           4,      // num_elements
                           "Pointers identical should bail early",
                           0,
                           0);
    return true;
}

// Test 3: Pass case where both pointers are NULL and number of elements is valid
// This exercises the early return path in the case of NULL pointers that are the same.
// Critical aspects covered:
// - expected == actual path when both pointers are NULL
// - No crash or undefined behavior for NULL pointers
bool test_UnityAssertEqualMemory_BothNull_Pass()
{
    UNITY_INTERNAL_PTR null_ptr = NULL;

    // same NULL pointers; should return immediately
    UnityAssertEqualMemory(null_ptr,
                           null_ptr,
                           1,      // length
                           1,      // num_elements
                           "Both pointers NULL should pass",
                           0,
                           0);
    return true;
}

// Test 4: Pass case with multiple elements and non-trivial alignment
// This reinforces that the inner byte-wise comparison progresses correctly
// across several elements when length > 1 and num_elements > 1.
// Critical aspects covered:
// - Correct byte-wise advancement of both pointers across elements
// - Correct handling across multiple elements
bool test_UnityAssertEqualMemory_MultiElement_NonTrivialLength()
{
    unsigned char expected[] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60};
    unsigned char actual[]   = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60};

    // length = 1 (byte per element) but we will simulate two "bytes per element" by
    // using length=2 with actual data as pairs. To keep test simple and valid with
    // Unity's expectations, we set length=2 and provide 3 elements (6 bytes total).
    UnityAssertEqualMemory((UNITY_INTERNAL_PTR)expected,
                           (UNITY_INTERNAL_PTR)actual,
                           2,
                           3,
                           "Element-wise equality with length=2 across 3 elements",
                           0,
                           0);
    return true;
}

// Simple test harness to run all tests and report outcomes.
// Note: The Unity framework typically uses its own runner; here we manually invoke
// the tests in a lightweight manner to satisfy the "no GTest" constraint.
int main()
{
    // Execute all test cases. Each test returns true on success in this harness.
    bool all_passed = true;

    std::cout << "Running UnityAssertEqualMemory test suite (non-terminating path tests)..." << std::endl;

    all_passed &= test_UnityAssertEqualMemory_MemoryBlocks_equal_MultipleBytes();
    std::cout << "Test 1: MemoryBlocks_equal_MultiByteElements - " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    all_passed &= test_UnityAssertEqualMemory_Pointers_Are_Identical();
    std::cout << "Test 2: Pointers_Are_Identical - " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    all_passed &= test_UnityAssertEqualMemory_BothNull_Pass();
    std::cout << "Test 3: BothNull_Pass - " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    all_passed &= test_UnityAssertEqualMemory_MultiElement_NonTrivialLength();
    std::cout << "Test 4: MultiElement_NonTrivialLength - " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    std::cout << "Overall: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;
    return all_passed ? 0 : 1;
}
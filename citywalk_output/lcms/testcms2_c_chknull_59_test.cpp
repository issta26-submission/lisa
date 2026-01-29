/*
Unit test suite for the focal method chknull(void* mem)

Notes:
- The focal method is implemented in C (testcms2.c) as:
  void* chknull(void* mem) { if (mem == NULL) Die("Memory may be corrupted"); return mem; }

- This C++11 test does not attempt to override or mock the Die function to avoid
  potential multiple-definition/linker issues with existing project code.

- We focus on direct behavior of chknull: when mem is non-null, it should return the
  same pointer; when mem is null, it should trigger Die and return NULL (as per the code path).

- Test framework is self-contained (no GTest). It uses a simple, non-terminating assertion style
  and prints a summary of results.
*/

#include <cstddef>
#include <iostream>
#include <testcms2.h>


// Declaration of the focal C function under test.
// We assume it is provided by the linked C source (testcms2.c or equivalent).
extern "C" void* chknull(void* mem);

// Candidate Keywords (from Step 1): chknull, mem, NULL, Die, memory check, void*, pointer, true/false
// The tests exercise the core dependent components: handling of non-null vs null pointers.


// Test 1: Non-null pointer should be returned unchanged.
// This exercises the true branch of the predicate "if (mem == NULL)" by passing a valid address.
bool test_chknull_non_null_ptr_returns_same()
{
    // Arrange: create a non-null memory address (address of a local variable)
    int sample = 123;
    void* input_ptr = static_cast<void*>(&sample);

    // Act: call the focal method
    void* result = chknull(input_ptr);

    // Assert: the returned pointer must be identical to the input pointer
    bool ok = (result == input_ptr);
    if (!ok)
    {
        std::cerr << "test_chknull_non_null_ptr_returns_same: FAIL - Returned pointer differs from input pointer.\n";
    }
    else
    {
        std::cout << "test_chknull_non_null_ptr_returns_same: PASS\n";
    }
    return ok;
}


// Test 2: Null pointer should result in null return path.
// This exercises the false branch of the predicate by passing NULL (or nullptr).
// We only verify the returned value; the Die() side-effect is not asserted here to avoid
// coupling tests to the implementation of Die.
bool test_chknull_null_ptr_returns_null()
{
    // Arrange: use a null pointer
    void* input_ptr = nullptr;

    // Act: call the focal method
    void* result = chknull(input_ptr);

    // Assert: the returned value should be NULL
    bool ok = (result == nullptr);
    if (!ok)
    {
        std::cerr << "test_chknull_null_ptr_returns_null: FAIL - Expected NULL return for NULL input.\n";
    }
    else
    {
        std::cout << "test_chknull_null_ptr_returns_null: PASS\n";
    }
    return ok;
}


// Optional: Test 3 (edge case): NULL via NULL macro as used in C/C++ code.
// Verifies behavior when using the NULL macro (for completeness across compilers).
bool test_chknull_null_macro_returns_null()
{
    // Arrange: NULL macro (may be 0)
    void* input_ptr = NULL;

    // Act
    void* result = chknull(input_ptr);

    // Assert
    bool ok = (result == nullptr);
    if (!ok)
    {
        std::cerr << "test_chknull_null_macro_returns_null: FAIL - Expected NULL return for NULL macro input.\n";
    }
    else
    {
        std::cout << "test_chknull_null_macro_returns_null: PASS\n";
    }
    return ok;
}


// Main test runner
int main()
{
    std::cout << "Starting unit tests for chknull(void*)\n";

    bool all_passed = true;

    all_passed &= test_chknull_non_null_ptr_returns_same();
    all_passed &= test_chknull_null_ptr_returns_null();
    // Optional additional check to cover NULL macro usage
    all_passed &= test_chknull_null_macro_returns_null();

    if (all_passed)
        std::cout << "ALL TESTS PASSED\n";
    else
        std::cout << "SOME TESTS FAILED\n";

    // Return non-zero if any test failed
    return all_passed ? 0 : 1;
}
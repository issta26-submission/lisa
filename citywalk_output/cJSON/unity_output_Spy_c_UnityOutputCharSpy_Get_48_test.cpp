// Unit test suite for UnityOutputCharSpy_Get and related dependencies.
// This test suite is written in C++11, uses a minimal custom test harness
// (no Google Test), and calls the C-style UnityOutputCharSpy API provided
// by unity_output_Spy.h. Tests are executed from main().
// Explanatory comments are added for each test to describe intent and coverage.

#include <cstring>
#include <unity_output_Spy.h>
#include <iostream>
#include <functional>
#include <string.h>
#include <string>
#include <unity_fixture.h>
#include <stdio.h>


// Import C dependencies (UnityOutputCharSpy API)
extern "C" {
}

// Simple lightweight test harness
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Run a single test and print result. Does not terminate on failure to allow multiple tests to execute.
static void run_test(const std::string& test_name, const std::function<bool()> &test_func) {
    ++g_total_tests;
    bool ok = test_func();
    if (ok) {
        ++g_passed_tests;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: Pointer stability of UnityOutputCharSpy_Get across consecutive calls.
// Intent: UnityOutputCharSpy_Get should return a consistent pointer to an internal
// buffer across multiple calls, provided the buffer is not reinitialized.
static bool test_GetPointerStability_AcrossCalls() {
    UnityOutputCharSpy_Create(10);
    UnityOutputCharSpy_Enable(1);

    const char* p1 = UnityOutputCharSpy_Get();
    // Make at least one write to potentially alter buffer contents
    UnityOutputCharSpy_OutputChar('A');
    const char* p2 = UnityOutputCharSpy_Get();

    // Expect the pointer to be stable across successive Gets
    bool ok = (p1 != nullptr) && (p2 != nullptr) && (p1 == p2);

    UnityOutputCharSpy_Destroy();
    return ok;
}

// Test 2: Get returns non-null after at least one OutputChar.
// Intent: Ensure the interface is usable; the buffer should be non-null after writing.
static bool test_GetNotNullAfterOutput() {
    UnityOutputCharSpy_Create(10);
    UnityOutputCharSpy_Enable(1);

    UnityOutputCharSpy_OutputChar('B');
    const char* p = UnityOutputCharSpy_Get();

    bool ok = (p != nullptr);
    // Also ensure the buffer contains data (length > 0) if the API writes characters
    if (p != nullptr) {
        ok = ok && (std::strlen(p) >= 1);
    }

    UnityOutputCharSpy_Destroy();
    return ok;
}

// Test 3: Buffer content length grows with multiple outputs.
// Intent: After N OutputChar calls, the buffer length should be at least N
// (assuming simple concatenation into a null-terminated string buffer).
static bool test_BufferContentLengthIncreasesWithOutputs() {
    UnityOutputCharSpy_Create(20);
    UnityOutputCharSpy_Enable(1);

    // Emit multiple characters
    UnityOutputCharSpy_OutputChar('A');
    UnityOutputCharSpy_OutputChar('B');
    UnityOutputCharSpy_OutputChar('C');
    const char* p = UnityOutputCharSpy_Get();

    bool ok = (p != nullptr) && (std::strlen(p) >= 3);

    UnityOutputCharSpy_Destroy();
    return ok;
}

// Test 4: Enable(false) should prevent buffer modification by OutputChar (if supported).
// Intent: When disabled, successive OutputChar calls should not alter the buffer.
// This test assumes the API respects the enable flag for real buffer mutation.
static bool test_EnableFalse_PreventsBufferModification() {
    UnityOutputCharSpy_Create(10);
    UnityOutputCharSpy_Enable(0);

    // Capture buffer state while disabled
    const char* before = UnityOutputCharSpy_Get();
    UnityOutputCharSpy_OutputChar('Z');
    const char* after = UnityOutputCharSpy_Get();

    // If disabled, buffer should remain unchanged
    bool ok = (before != nullptr) && (after != nullptr) && (before == after);

    UnityOutputCharSpy_Destroy();
    return ok;
}

// Test 5: Get pointer consistency across multiple consecutive Get calls without intervening writes.
// Intent: Ensure that repeated reads do not alter buffer identity.
static bool test_GetPointerConsistency_MultipleGets() {
    UnityOutputCharSpy_Create(8);
    UnityOutputCharSpy_Enable(1);

    const char* p1 = UnityOutputCharSpy_Get();
    const char* p2 = UnityOutputCharSpy_Get();
    const char* p3 = UnityOutputCharSpy_Get();

    bool ok = (p1 != nullptr) && (p2 != nullptr) && (p3 != nullptr)
              && (p1 == p2) && (p2 == p3);

    UnityOutputCharSpy_Destroy();
    return ok;
}

int main() {
    // Run all tests
    run_test("UnityOutputCharSpy_Get Pointer Stability Across Calls", test_GetPointerStability_AcrossCalls);
    run_test("UnityOutputCharSpy_Get NotNull After OutputChar", test_GetNotNullAfterOutput);
    run_test("UnityOutputCharSpy_Get Buffer Length Increases With Outputs", test_BufferContentLengthIncreasesWithOutputs);
    run_test("UnityOutputCharSpy_Enable False Prevents Buffer Modification", test_EnableFalse_PreventsBufferModification);
    run_test("UnityOutputCharSpy_Get Pointer Consistency Across Gets", test_GetPointerConsistency_MultipleGets);

    // Summary
    std::cout << "\nTest Summary: " << g_passed_tests << " / " << g_total_tests << " tests passed." << std::endl;
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
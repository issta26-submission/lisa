// Test suite for ReadOneElem in cmstypes.c using a lightweight C++11 test harness.
// This test avoids GoogleTest and relies on a small, self-contained main() entry.
// It uses a mocked _cmsReadUInt32Number to drive ReadOneElem behavior and verifies
// both true and false branches along with BaseOffset handling.

#include <vector>
#include <cstddef>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>


// Include internal CMS headers to get actual type definitions.
// The focal function ReadOneElem uses cmsBool, cmsUInt32Number, cmsIOHANDLER, and
// _cmsDICelem from the project's internal headers.
extern "C" {
}

// Forward declaration of the focal function (defined in cmstypes.c).
// We declare with C linkage to enable linking with the C implementation from C++.
extern "C" cmsBool ReadOneElem(cmsIOHANDLER* io, _cmsDICelem* e, cmsUInt32Number i, cmsUInt32Number BaseOffset);

// -----------------------------------------------------------------------------
// Mocking _cmsReadUInt32Number to drive cmsRead behavior without a full IO layer.
// The real function reads from io; here we supply values from a controlled vector.
// -----------------------------------------------------------------------------

static std::vector<cmsUInt32Number> mockNumbers;
static std::size_t mockIndex = 0;

// Mock implementation with C linkage so the linker resolves the symbol for cmstypes.c.
extern "C" cmsBool _cmsReadUInt32Number(cmsIOHANDLER* /*io*/, cmsUInt32Number* number)
{
    if (mockIndex >= mockNumbers.size()) {
        return FALSE; // Simulate read failure when numbers are exhausted.
    }
    *number = mockNumbers[mockIndex++];
    return TRUE;
}

// Helper to reset mocks between tests
static void resetMocks(const std::vector<cmsUInt32Number>& nums)
{
    mockNumbers = nums;
    mockIndex = 0;
}

// Helper to print test results
static void printTestResult(const std::string& testName, bool passed)
{
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << "\n";
}

// -----------------------------------------------------------------------------
// Test 1: Read succeeds for Offsets = 0; BaseOffset should not be applied.
// Inputs: Offsets[0] read as 0, Sizes[0] read as 50; BaseOffset = 100
// Expected: ReadOneElem returns TRUE; Offsets[0] == 0; Sizes[0] == 50
// -----------------------------------------------------------------------------
static bool test_ReadOneElem_Success_OffsetsZero()
{
    resetMocks({ 0, 50 });

    _cmsDICelem e;
    // Allocate arrays for the test (assuming the real structure has Offsets/Sizes pointers)
    e.Offsets = new cmsUInt32Number[1];
    e.Sizes = new cmsUInt32Number[1];

    cmsBool res = ReadOneElem(nullptr, &e, 0, 100u);

    bool ok = (res == TRUE) && (e.Offsets[0] == 0u) && (e.Sizes[0] == 50u);

    delete[] e.Offsets;
    delete[] e.Sizes;

    return ok;
}

// -----------------------------------------------------------------------------
// Test 2: Read succeeds for Offsets > 0; BaseOffset should be added.
// Inputs: Offsets[0] read as 5, Sizes[0] read as 10; BaseOffset = 100
// Expected: ReadOneElem returns TRUE; Offsets[0] == 5 + 100 = 105; Sizes[0] == 10
// -----------------------------------------------------------------------------
static bool test_ReadOneElem_Success_OffsetsPositive()
{
    resetMocks({ 5, 10 });

    _cmsDICelem e;
    e.Offsets = new cmsUInt32Number[1];
    e.Sizes = new cmsUInt32Number[1];

    cmsBool res = ReadOneElem(nullptr, &e, 0, 100u);

    bool ok = (res == TRUE) && (e.Offsets[0] == 105u) && (e.Sizes[0] == 10u);

    delete[] e.Offsets;
    delete[] e.Sizes;

    return ok;
}

// -----------------------------------------------------------------------------
// Test 3: First read fails; function should return FALSE without modifying e.
// Inputs: No numbers provided (empty mock); Offsets[0] pre-set to a value 123
// Expected: ReadOneElem returns FALSE; Offsets[0] remains 123
// -----------------------------------------------------------------------------
static bool test_ReadOneElem_Fail_FirstRead()
{
    resetMocks({}); // No numbers to read; first read fails.

    _cmsDICelem e;
    e.Offsets = new cmsUInt32Number[1];
    e.Sizes = new cmsUInt32Number[1];
    e.Offsets[0] = 123u; // preset
    e.Sizes[0] = 0u;

    cmsBool res = ReadOneElem(nullptr, &e, 0, 50u);

    bool ok = (res == FALSE) && (e.Offsets[0] == 123u);

    delete[] e.Offsets;
    delete[] e.Sizes;

    return ok;
}

// -----------------------------------------------------------------------------
// Test 4: Second read fails after first read succeeds; function should return FALSE
// and Offsets[0] should contain the value read in the first step (no rollback).
// Inputs: First read 7, second read fails (no more numbers).
// Expected: ReadOneElem returns FALSE; Offsets[0] == 7
// -----------------------------------------------------------------------------
static bool test_ReadOneElem_Fail_SecondRead()
{
    resetMocks({ 7 }); // Only one number available for the first Read

    _cmsDICelem e;
    e.Offsets = new cmsUInt32Number[1];
    e.Sizes = new cmsUInt32Number[1];
    e.Offsets[0] = 0u; // initial, will be overwritten by first read

    cmsBool res = ReadOneElem(nullptr, &e, 0, 12u);

    bool ok = (res == FALSE) && (e.Offsets[0] == 7u);

    delete[] e.Offsets;
    delete[] e.Sizes;

    return ok;
}

// -----------------------------------------------------------------------------
// Main driver: run all tests and print a summary.
// -----------------------------------------------------------------------------
int main()
{
    int passed = 0;
    int total = 0;

    // Run Test 1
    ++total;
    bool t1 = test_ReadOneElem_Success_OffsetsZero();
    printTestResult("Test 1: Success with Offsets == 0 (no BaseOffset applied)", t1);
    if (t1) ++passed;

    // Run Test 2
    ++total;
    bool t2 = test_ReadOneElem_Success_OffsetsPositive();
    printTestResult("Test 2: Success with Offsets > 0 (BaseOffset added)", t2);
    if (t2) ++passed;

    // Run Test 3
    ++total;
    bool t3 = test_ReadOneElem_Fail_FirstRead();
    printTestResult("Test 3: Failure when first read fails", t3);
    if (t3) ++passed;

    // Run Test 4
    ++total;
    bool t4 = test_ReadOneElem_Fail_SecondRead();
    printTestResult("Test 4: Failure when second read fails (first read succeeds)", t4);
    if (t4) ++passed;

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}
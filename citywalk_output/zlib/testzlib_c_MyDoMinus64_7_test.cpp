/*
  Test suite for the focal method: MyDoMinus64
  - Purpose: Verify 64-bit subtraction using 32-bit HighPart/LowPart representation.
  - Environment: C++11, Windows LARGE_INTEGER (HighPart and LowPart).
  - No Google Test; a lightweight in-file test harness is implemented.
  - Tests cover true/false branches of the condition A.LowPart >= B.LowPart and edge cases
    including underflow/borrow behavior and negative HighPart scenarios.
  - The test harness does not terminate on failures to maximize coverage (non-terminating assertions).
*/

#include <windows.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>


extern "C" void MyDoMinus64(LARGE_INTEGER *R, LARGE_INTEGER A, LARGE_INTEGER B); // Provided implementation

// Simple lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

static void log_result(const char* testName, bool ok)
{
    g_total++;
    if (!ok) {
        g_failed++;
        std::cout << "[FAILED] " << testName << std::endl;
    } else {
        std::cout << "[PASSED] " << testName << std::endl;
    }
}

// Helper to populate a LARGE_INTEGER with given HighPart and LowPart
static inline LARGE_INTEGER makeLI(int high, DWORD low)
{
    LARGE_INTEGER li;
    li.HighPart = static_cast<LONG>(high);
    li.LowPart = low;
    return li;
}

// Test 1: Branch taken (A.LowPart >= B.LowPart) - no borrow from HighPart
// A = {High=5, Low=20}, B = {High=3, Low=10}
// Expected: HighPart = 2, LowPart = 10
static void test_noBorrow_branch()
{
    // Arrange
    LARGE_INTEGER A = makeLI(5, 20);
    LARGE_INTEGER B = makeLI(3, 10);
    LARGE_INTEGER R;

    // Act
    MyDoMinus64(&R, A, B);

    // Assert
    bool ok = (R.HighPart == 2) && (R.LowPart == 10);
    log_result("MyDoMinus64_NoBorrow_BranchTrue", ok);
}

// Test 2: Branch not taken (A.LowPart < B.LowPart) - borrow from HighPart
// A = {High=5, Low=5}, B = {High=3, Low=10}
// Expected: HighPart = 1, LowPart = 0xFFFFFFFB (4294967291)
static void test_borrow_branch()
{
    LARGE_INTEGER A = makeLI(5, 5);
    LARGE_INTEGER B = makeLI(3, 10);
    LARGE_INTEGER R;

    MyDoMinus64(&R, A, B);

    bool ok = (R.HighPart == 1) && (R.LowPart == 0xFFFFFFFB);
    log_result("MyDoMinus64_Borrow_BranchFalse", ok);
}

// Test 3: Subtraction resulting in 64-bit underflow on HighPart (A = 0, B has HighPart > 0)
// A = {High=0, Low=0}, B = {High=1, Low=0} -> Result: HighPart = -1, LowPart = 0
static void test_negative_high_underflow()
{
    LARGE_INTEGER A = makeLI(0, 0);
    LARGE_INTEGER B = makeLI(1, 0);
    LARGE_INTEGER R;

    MyDoMinus64(&R, A, B);

    bool ok = (R.HighPart == -1) && (R.LowPart == 0);
    log_result("MyDoMinus64_UnderflowHigh", ok);
}

// Test 4: Borrow with negative HighPart in A
// A = {High=-1, Low=0}, B = {High=0, Low=1}
// Expected: HighPart = -2, LowPart = 0xFFFFFFFF
static void test_borrow_with_negative_highA()
{
    LARGE_INTEGER A = makeLI(-1, 0);
    LARGE_INTEGER B = makeLI(0, 1);
    LARGE_INTEGER R;

    MyDoMinus64(&R, A, B);

    bool ok = (R.HighPart == -2) && (R.LowPart == 0xFFFFFFFF);
    log_result("MyDoMinus64_BorrowWithNegHighA", ok);
}

// Test 5: Another negative-high scenario (A.HighPart more negative than B)
// A = {High=-2, Low=0xFFFFFFFF}, B = {High=-1, Low=0x0}
// Expected: HighPart = -1, LowPart = 0xFFFFFFFF
static void test_negative_high_subtraction_with_wrap()
{
    LARGE_INTEGER A = makeLI(-2, 0xFFFFFFFF);
    LARGE_INTEGER B = makeLI(-1, 0);
    LARGE_INTEGER R;

    MyDoMinus64(&R, A, B);

    bool ok = (R.HighPart == -1) && (R.LowPart == 0xFFFFFFFF);
    log_result("MyDoMinus64_NegHigh_SubWrap", ok);
}

int main()
{
    // Run all tests
    test_noBorrow_branch();
    test_borrow_branch();
    test_negative_high_underflow();
    test_borrow_with_negative_highA();
    test_negative_high_subtraction_with_wrap();

    // Summary
    std::cout << "Total tests run: " << g_total << std::endl;
    std::cout << "Total failures: " << g_failed << std::endl;

    // Return nonzero if any test failed
    return (g_failed == 0) ? 0 : 1;
}
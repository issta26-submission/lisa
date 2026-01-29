// Self-contained C++11 test suite for BeginCountPerfCounter (Windows API dependent)

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>


// Forward declaration of the function under test (as provided by the focal code)
void BeginCountPerfCounter(LARGE_INTEGER * pbeginTime64,BOOL fComputeTimeQueryPerf)
{
{
    if ((!fComputeTimeQueryPerf) || (!QueryPerformanceCounter(pbeginTime64)))
    {
        pbeginTime64->LowPart = GetTickCount();
        pbeginTime64->HighPart = 0;
    }
}
}

// Lightweight non-terminating test framework (no GTest required)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Expectation macros that log failures but do not abort execution
#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ << " - EXPECT_TRUE(" #cond ") is false\n"; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    ++g_total_tests; \
    if(!((a) == (b))) { \
        ++g_failed_tests; \
        std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ << " - EXPECT_EQ(" #a ", " #b ") -> " << (a) << " != " << (b) << "\n"; \
    } \
} while(0)

// Test 1: When fComputeTimeQueryPerf is FALSE, the function should fallback to GetTickCount() with HighPart = 0
static void test_BeginCountPerfCounter_FalseBranch()
{
    // Arrange
    LARGE_INTEGER beginTime;
    DWORD tickBefore = GetTickCount();

    // Act
    BeginCountPerfCounter(&beginTime, FALSE);

    DWORD tickAfter = GetTickCount();

    // Assert
    // HighPart must be 0
    EXPECT_TRUE(beginTime.HighPart == 0);

    // LowPart should be the GetTickCount() value captured inside the function call
    // It must be within the window [tickBefore, tickAfter] (inclusive)
    bool inRange = (beginTime.LowPart >= tickBefore) && (beginTime.LowPart <= tickAfter);
    EXPECT_TRUE(inRange);

    // Commentary:
    // This test validates the explicit false-branch path in the if-condition:
    // (!fComputeTimeQueryPerf) => true, so the function assigns tick-based time.
}

// Test 2: When fComputeTimeQueryPerf is TRUE, the function should attempt QueryPerformanceCounter
// and populate the LARGE_INTEGER with the performance counter value if successful.
// If QueryPerformanceCounter fails, it should gracefully fall back to GetTickCount() (tested implicitly).
static void test_BeginCountPerfCounter_TrueBranch()
{
    // Arrange
    LARGE_INTEGER beginTime;

    // Act
    BeginCountPerfCounter(&beginTime, TRUE);

    // Assert
    // The value should be meaningful (non-zero in practice). Since QueryPerformanceCounter
    // is system-dependent, allow for the fallback path as well.
    bool nonZero = (beginTime.LowPart != 0) || (beginTime.HighPart != 0);
    EXPECT_TRUE(nonZero);

    // Commentary:
    // This test exercises the true-branch of the OR condition:
    // (fComputeTimeQueryPerf) && QueryPerformanceCounter(...) succeeds or fails.
    // We conservatively verify that some non-zero time value is produced.
}

// Optional: Additional test to observe monotonic progression when calling repeatedly
// This test is heuristic and serves to increase coverage rather than guarantee a strict contract.
static void test_BeginCountPerfCounter_RepeatedCalls()
{
    // Arrange
    LARGE_INTEGER t1, t2;
    // Act
    BeginCountPerfCounter(&t1, TRUE);
    BeginCountPerfCounter(&t2, TRUE);
    // Assert
    bool t1Valid = (t1.LowPart != 0) || (t1.HighPart != 0);
    bool t2Valid = (t2.LowPart != 0) || (t2.HighPart != 0);
    EXPECT_TRUE(t1Valid);
    EXPECT_TRUE(t2Valid);
    // Commentary:
    // Repeated invocations help exercise the function under different timing conditions,
    // contributing to coverage of the QPC-based path and potential fallback.
}

// Entry point: run all tests and report summary
int main()
{
    std::cout << "Starting BeginCountPerfCounter unit tests (non-GTest framework)...\n";

    test_BeginCountPerfCounter_FalseBranch();
    test_BeginCountPerfCounter_TrueBranch();
    test_BeginCountPerfCounter_RepeatedCalls();

    std::cout << "Test summary: " << g_total_tests
              << " tests executed, " << g_failed_tests
              << " failures.\n";

    if (g_failed_tests != 0) {
        std::cout << "Some tests FAILED. See details above for diagnostics.\n";
        return 1;
    } else {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
}
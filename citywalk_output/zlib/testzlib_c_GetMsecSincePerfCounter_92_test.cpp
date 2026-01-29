// Unit test suite for GetMsecSincePerfCounter (from testzlib.c)
// This test is written for C++11 without using GoogleTest.
// It invokes the focal function and uses a minimal, non-terminating assertion style.
// The tests aim to cover both branches of the conditional inside GetMsecSincePerfCounter.

#include <windows.h>
#include <math.h>
#include <string>
#include <functional>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <zlib.h>
#include <stdlib.h>


// Forward declaration of the focal function (C linkage)
extern "C" DWORD GetMsecSincePerfCounter(LARGE_INTEGER beginTime64,BOOL fComputeTimeQueryPerf);

// Simple non-terminating test harness
static int g_totalTests = 0;
static int g_failedTests = 0;
static std::vector<std::string> g_failureMessages;

#define TEST_CHECK(cond, msg) \
    do { \
        if(!(cond)) { \
            g_failedTests++; \
            g_failureMessages.push_back(std::string("CHECK FAILED: ") + (std::string)(msg)); \
        } \
        g_totalTests++; \
    } while(0)

static void ReportResults() {
    printf("\nTest Summary: %d run, %d failed\n", g_totalTests, g_failedTests);
    for(const auto &m : g_failureMessages) {
        printf("  %s\n", m.c_str());
    }
}

// Test 1: False branch path (fComputeTimeQueryPerf == false)
// This path uses GetTickCount() difference: result should roughly equal the tick-count delta
static void Test_GetMsecSincePerfCounter_FalseBranch_DeltaMatchesTickDiff() {
    // Capture a baseline tick count a short moment before the call
    DWORD begin = GetTickCount();
    // Small delay to ensure a non-zero delta (not strictly required, but helps variability)
    Sleep(1);

    LARGE_INTEGER beginTime64;
    beginTime64.LowPart = begin;
    beginTime64.HighPart = 0;

    DWORD res = GetMsecSincePerfCounter(beginTime64, FALSE);

    DWORD now = GetTickCount();
    DWORD expectedDelta = (now >= begin) ? (now - begin) : 0;

    // Allow a tolerance to account for small timing differences
    // The function overhead may add a few milliseconds.
    DWORD diff = (res > expectedDelta) ? (res - expectedDelta) : (expectedDelta - res);
    TEST_CHECK(diff <= 50, "False branch: result should be within 50 ms of tick-count delta");
}

// Test 2: True branch path (fComputeTimeQueryPerf == true)
// This path uses high-resolution performance counters.
// We cannot predict exact value, but we can assert non-negative and reasonably bounded output
static void Test_GetMsecSincePerfCounter_TrueBranch_BasicValidity() {
    // Use a non-zero base to avoid trivial 0-length counter subtraction
    LARGE_INTEGER beginTime64;
    beginTime64.LowPart = GetTickCount(); // base roughly around current tick
    beginTime64.HighPart = 0;

    // Small delay to ensure some measurable elapsed time
    Sleep(2);

    DWORD res = GetMsecSincePerfCounter(beginTime64, TRUE);

    // The result should be non-negative and finite.
    TEST_CHECK(res >= 0, "True branch: result should be non-negative");

    // Heuristic bound: elapsed time since beginTime64 should not be astronomically large
    // A reasonable bound for a quick call is less than 60 seconds (60000 ms)
    TEST_CHECK(res < 60000, "True branch: result should be less than 60000 ms for a quick call");
}

// Test 3: Basic boundary check across branches (ensure function handles both branches in one run)
// This test simply exercises both branches in sequence to improve coverage.
static void Test_GetMsecSincePerfCounter_BothBranchesCoverage() {
    // False branch invocation
    LARGE_INTEGER beginTime64_false;
    beginTime64_false.LowPart = GetTickCount() - 25; // ensure a small, known delta
    beginTime64_false.HighPart = 0;
    DWORD res_false = GetMsecSincePerfCounter(beginTime64_false, FALSE);
    DWORD now_after_false = GetTickCount();
    DWORD expected_delta_false = (now_after_false >= beginTime64_false.LowPart) ? (now_after_false - beginTime64_false.LowPart) : 0;
    DWORD diff_false = (res_false > expected_delta_false) ? (res_false - expected_delta_false) : (expected_delta_false - res_false);
    TEST_CHECK(diff_false <= 60, "Black-box cross-branch: false branch delta within 60 ms");

    // True branch invocation
    LARGE_INTEGER beginTime64_true;
    beginTime64_true.LowPart = GetTickCount(); // set a new base
    beginTime64_true.HighPart = 0;
    Sleep(1);
    DWORD res_true = GetMsecSincePerfCounter(beginTime64_true, TRUE);
    TEST_CHECK(res_true >= 0, "Black-box cross-branch: true branch result non-negative");
    TEST_CHECK(res_true < 60000, "Black-box cross-branch: true branch result < 60000 ms");
}

// Main test runner
int main() {
    printf("Starting GetMsecSincePerfCounter unit tests (C++11, no GTest)...\n");

    Test_GetMsecSincePerfCounter_FalseBranch_DeltaMatchesTickDiff();
    Test_GetMsecSincePerfCounter_TrueBranch_BasicValidity();
    Test_GetMsecSincePerfCounter_BothBranchesCoverage();

    ReportResults();

    // Return non-zero if any test failed to aid integration with simple shells
    return (g_failedTests == 0) ? 0 : 1;
}
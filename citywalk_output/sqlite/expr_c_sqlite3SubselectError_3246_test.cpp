// Unit test suite for the focal method sqlite3SubselectError
// This test is self-contained and does not rely on GTest.
// It mocks the minimal dependencies (Parse struct and sqlite3ErrorMsg)
// to exercise true/false branches of the focal method.
//
// Design notes aligned with the problem domain knowledge:
// - We test the true-branch (nErr == 0) and the false-branch (nErr != 0).
// - We emulate the external dependency sqlite3ErrorMsg with a small inline mock
//   that captures the formatted message into a global string for assertion.
// - We keep tests non-terminating (no aborts) and provide detailed comments
//   for each test case to explain intent and coverage rationale.

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// Domain understanding notes (Domain Knowledge):
// - The Parse structure used by sqlite3SubselectError must provide a member nErr.
// - When nErr == 0, sqlite3SubselectError should call sqlite3ErrorMsg with the
//   message: "sub-select returns %d columns - expected %d".
// - When nErr != 0, sqlite3SubselectError should do nothing (no message).
// - We implement a minimal Parse struct and a small mock for sqlite3ErrorMsg
//   to capture messages for assertions without requiring the full SQLite stack.

struct Parse {
    int nErr; // Minimal replica of the field used by sqlite3SubselectError
};

// Global buffer to capture the last error message produced by sqlite3ErrorMsg
static std::string g_lastErrorMsg;

// Forward declarations to mimic the real C interface (as seen in expr.c)
extern "C" {
    // Mock of the external error reporting routine used by sqlite3SubselectError
    void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...);

    // Focal method under test (reproduced here for a self-contained test)
    void sqlite3SubselectError(Parse *pParse, int nActual, int nExpect);
}

// Mock implementation of sqlite3ErrorMsg that formats the message
// and stores it into g_lastErrorMsg for verification.
extern "C" void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...)
{
    if (!zFormat) {
        g_lastErrorMsg.clear();
        return;
    }
    char buffer[1024];
    va_list ap;
    va_start(ap, zFormat);
    vsnprintf(buffer, sizeof(buffer), zFormat, ap);
    va_end(ap);

    // Store last message for assertions
    g_lastErrorMsg = buffer;
}

// Focal method under test copied here to allow self-contained testing without linking SQLite.
// The logic mirrors the snippet provided in the prompt.
extern "C" void sqlite3SubselectError(Parse *pParse, int nActual, int nExpect)
{
    if (pParse->nErr == 0) {
        const char *zFmt = "sub-select returns %d columns - expected %d";
        sqlite3ErrorMsg(pParse, zFmt, nActual, nExpect);
    }
}

// Simple test framework (lightweight, non-terminating assertions)
#define TEST_ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " #cond << std::endl; \
        ++g_testFailures; \
    } \
} while(0)

#define TEST_EXPECT_EQ(actual, expected) do { \
    if (!((actual) == (expected))) { \
        std::cerr << "Assertion failed: " #actual " != " #expected \
                  << " (actual=" << (actual) << ", expected=" << (expected) << ")" \
                  << std::endl; \
        ++g_testFailures; \
    } \
} while(0)

// Global test failure counter
static int g_testFailures = 0;

// Helper: reset global test state before each test
static void resetTestState() {
    g_lastErrorMsg.clear();
}

// Test 1: When pParse->nErr == 0, the function should emit an error message
// with the expected formatting and values.
static void test_SubselectError_ReportsWhenNoError()
{
    resetTestState();

    Parse p;
    p.nErr = 0; // true branch
    sqlite3SubselectError(&p, 3, 2);

    // Expect that sqlite3ErrorMsg was invoked and produced the exact string
    TEST_ASSERT_TRUE(!g_lastErrorMsg.empty());
    TEST_EXPECT_EQ(g_lastErrorMsg, std::string("sub-select returns 3 columns - expected 2"));
}

// Test 2: When pParse->nErr != 0, the function should NOT emit any message.
static void test_SubselectError_DoesNotReportWhenErrorPresent()
{
    resetTestState();

    Parse p;
    p.nErr = 1; // false branch
    sqlite3SubselectError(&p, 3, 2);

    // Expect that no message was produced
    TEST_ASSERT_TRUE(g_lastErrorMsg.empty());
}

// Test 3: Validate formatting with different numeric inputs to ensure robust formatting.
// This further exercises the true-branch path with varied data.
static void test_SubselectError_VariedNumbersFormatting()
{
    resetTestState();

    Parse p;
    p.nErr = 0;
    sqlite3SubselectError(&p, 7, 4);

    TEST_ASSERT_TRUE(!g_lastErrorMsg.empty());
    TEST_EXPECT_EQ(g_lastErrorMsg, std::string("sub-select returns 7 columns - expected 4"));
}

// Run all tests and report summary
static void runAllTests()
{
    g_testFailures = 0;

    test_SubselectError_ReportsWhenNoError();
    test_SubselectError_DoesNotReportWhenErrorPresent();
    test_SubselectError_VariedNumbersFormatting();

    if (g_testFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_testFailures << " TEST(S) FAILED" << std::endl;
    }
}

int main()
{
    // Domain knowledge rationale:
    // - We intentionally cover both branches of the conditional.
    // - We verify exact string formatting for the true-branch.
    // - We ensure non-terminating behavior by not terminating on failure.

    runAllTests();
    return (g_testFailures == 0) ? 0 : 1;
}
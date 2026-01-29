// This test suite focuses on validating the focal method:
// sqlite3ExprOrderByAggregateError(Parse *pParse, Expr *p)
// which is expected to call sqlite3ErrorMsg with a specific message.
// The test uses a lightweight, self-contained mock of the dependencies
// (Parse, Expr) and a mock of sqlite3ErrorMsg to capture the call.
// It does not rely on any external testing framework (no GTest).

#include <vector>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain-driven notes (for maintainers):
// - Candidate Keywords (core dependent components): sqlite3ErrorMsg, ORDER BY, non-aggregate,
//   %#T() placeholder, Parse, Expr, error messaging, variadic function handling.
// - The tests exercise the exact message string and ensure the parameter p (Expr*) is passed
//   through to the error reporter. We also verify behavior when pParse or p are NULL.
// - Static/global helpers are implemented in this translation unit to keep tests self-contained.


// Minimal stand-ins for the focal class dependencies
struct Parse { int dummy; };
struct Expr  { int dummy; };

// Data structure to capture sqlite3ErrorMsg invocations for verification
struct Capture {
    const Parse* pParse;
    std::string zFormat;
    const void* pArg;
};

// Global capture store used by the mock sqlite3ErrorMsg
static std::vector<Capture> g_captures;

// Mock implementation of sqlite3ErrorMsg with variadic arguments.
// We only inspect the first variadic argument (which should be the pointer to Expr in this test).
void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...)
{
    va_list ap;
    va_start(ap, zFormat);
    void *pArg = nullptr;
    // In the focal call there is exactly one variadic argument: p (Expr*)
    // We safely extract it as a void* for verification.
    pArg = va_arg(ap, void*);
    va_end(ap);

    g_captures.push_back({pParse, zFormat ? std::string(zFormat) : "", pArg});
}

// Focal method under test (reproduced in this test file for self-containment).
void sqlite3ExprOrderByAggregateError(Parse *pParse, Expr *p){
  sqlite3ErrorMsg(pParse,
     "ORDER BY may not be used with non-aggregate %#T()", p
  );
}

// Test harness utilities
static int g_totalTests = 0;
static int g_failedTests = 0;

// Non-terminating assertions (no test aborts on failure)
#define EXPECT_TRUE(cond) do { ++g_totalTests; if(!(cond)) { \
    std::cerr << "Expectation failed: " #cond " in " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_failedTests; } } while(0)

#define EXPECT_PTR_EQ(a,b) do { ++g_totalTests; if((const void*)(a) != (const void*)(b)) { \
    std::cerr << "Pointer mismatch: " #a " != " #b " (" << (void*)(a) << " vs " << (void*)(b) << ") in " \
              << __FILE__ << ":" << __LINE__ << "\n"; ++g_failedTests; } } while(0)

#define EXPECT_STR_EQ(a,b) do { ++g_totalTests; if(std::string((a)) != std::string((b))) { \
    std::cerr << "String mismatch: " #a " != " #b " (\"" << (a) << "\" vs \"" << (b) << "\") in " \
              << __FILE__ << ":" << __LINE__ << "\n"; ++g_failedTests; } } while(0)

// Test 1: Basic invocation should report the exact error format and forward the Expr* as the argument
void test_BasicInvocation_RecordsExpectedMessageAndArg() {
    // Reset captures
    g_captures.clear();

    Parse pParseInstance;
    Expr pExprInstance;

    // Act
    sqlite3ExprOrderByAggregateError(&pParseInstance, &pExprInstance);

    // Assert
    EXPECT_TRUE(g_captures.size() == 1);
    if (g_captures.size() == 1) {
        EXPECT_PTR_EQ(g_captures[0].pParse, &pParseInstance);
        EXPECT_STR_EQ(g_captures[0].zFormat.c_str(),
                      "ORDER BY may not be used with non-aggregate %#T()");
        EXPECT_PTR_EQ(g_captures[0].pArg, &pExprInstance);
    }
}

// Test 2: Null pParse should still result in a single capture with NULL pParse
void test_NullParse_InvocationRecordsNullParse() {
    g_captures.clear();

    Expr e;
    sqlite3ExprOrderByAggregateError(nullptr, &e);

    EXPECT_TRUE(g_captures.size() == 1);
    if (g_captures.size() == 1) {
        EXPECT_TRUE(g_captures[0].pParse == nullptr);
        EXPECT_STR_EQ(g_captures[0].zFormat.c_str(),
                      "ORDER BY may not be used with non-aggregate %#T()");
        EXPECT_PTR_EQ(g_captures[0].pArg, &e);
    }
}

// Test 3: Null pExpr should still result in a single capture with NULL pArg
void test_NullExpr_InvocationRecordsNullExpr() {
    g_captures.clear();

    Parse p;
    sqlite3ExprOrderByAggregateError(&p, nullptr);

    EXPECT_TRUE(g_captures.size() == 1);
    if (g_captures.size() == 1) {
        EXPECT_PTR_EQ(g_captures[0].pParse, &p);
        EXPECT_STR_EQ(g_captures[0].zFormat.c_str(),
                      "ORDER BY may not be used with non-aggregate %#T()");
        EXPECT_PTR_EQ(g_captures[0].pArg, nullptr);
    }
}

int main() {
    // Run tests
    test_BasicInvocation_RecordsExpectedMessageAndArg();
    test_NullParse_InvocationRecordsNullParse();
    test_NullExpr_InvocationRecordsNullExpr();

    // Summary
    std::cout << "Total tests run: " << g_totalTests << "\n";
    std::cout << "Total tests failed: " << g_failedTests << "\n";

    // Return non-zero if any test failed
    return g_failedTests ? 1 : 0;
}
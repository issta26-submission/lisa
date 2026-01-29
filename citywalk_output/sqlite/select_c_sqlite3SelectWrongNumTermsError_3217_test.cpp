/*
Unit test suite for the focal method sqlite3SelectWrongNumTermsError.
This test harness provides minimal stand-ins for the dependent C structures
(Parse, Select) and supporting functions (SF_Values, sqlite3ErrorMsg,
sqlite3SelectOpName) so the focal logic can be exercised in a C++11
environment without GTest.

Test coverage:
- Branch where SF_Values is set: validates the exact error message.
- Branch where SF_Values is not set: validates the error message includes the
  correct operator name substituted by sqlite3SelectOpName(p->op).

Notes:
- Tests are non-terminating (do not abort on failure) and report mismatches
  via console output, following the EXPECT-like approach described in the spec.
- All code is self-contained; the focal function is implemented here to enable
  straightforward compilation in isolation.
*/

#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// Candidate Keywords inferred from the focal method:
// - SF_Values (flag used to choose the error branch)
// - Parse *pParse (parse context)
// - Select *p (select statement with fields selFlags and op)
// - sqlite3ErrorMsg (error reporting function accepting variadic args)
// - sqlite3SelectOpName (helper returning the textual name of an operator id)

// Lightweight stubs and minimal scaffolding to emulate the environment.

typedef struct Parse Parse;
typedef struct Select Select;

// Define the flag used in the focal method
static const unsigned SF_Values = 0x01;

// Minimal Parse structure (contents unused for tests)
struct Parse {
    int dummy;
};

// Minimal Select structure carrying only needed members
struct Select {
    unsigned int selFlags;
    int op;
};

// Global storage for the last error message produced by sqlite3ErrorMsg
static std::string g_lastError;

// Helper to reset the last error message between tests
static void sqlite3ResetLastError() {
    g_lastError.clear();
}

// Mock implementation of sqlite3ErrorMsg with variadic formatting support.
// This mirrors the real sqlite3ErrorMsg signature closely enough for tests.
static void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...) {
    (void)pParse; // suppress unused parameter warning for test harness

    va_list ap;
    va_start(ap, zFormat);

    // Compute required length
    va_list apCopy;
    va_copy(apCopy, ap);
    int n = vsnprintf(nullptr, 0, zFormat, apCopy);
    va_end(apCopy);

    if (n < 0) {
        g_lastError = ""; // formatting error
        va_end(ap);
        return;
    }

    // Allocate string and format
    std::string msg;
    msg.resize(static_cast<size_t>(n));
    vsnprintf(&msg[0], static_cast<size_t>(n) + 1, zFormat, ap);
    va_end(ap);

    g_lastError = msg;
}

// Storage for operator name resolution across calls
static std::string g_opNameBuf;

// Mock implementation of sqlite3SelectOpName(int id)
static const char* sqlite3SelectOpName(int id) {
    // Simple mapping for testing; extend if needed
    switch (id) {
        case 0: g_opNameBuf = "UNKN"; break;
        case 1: g_opNameBuf = "UNION"; break;
        case 7: g_opNameBuf = "OP7"; break;
        default: g_opNameBuf = "OP_UNKNOWN"; break;
    }
    return g_opNameBuf.c_str();
}

// The focal method under test (reproduced here in this harness)
void sqlite3SelectWrongNumTermsError(Parse *pParse, Select *p){
    if( p->selFlags & SF_Values ){
        sqlite3ErrorMsg(pParse, "all VALUES must have the same number of terms");
    }else{
        sqlite3ErrorMsg(pParse, "SELECTs to the left and right of %s"
            " do not have the same number of result columns",
            sqlite3SelectOpName(p->op));
    }
}

// Simple assertion macro that does not terminate on failure
static int g_failures = 0;
#define EXPECT_EQ(actual, expected) do { \
    if ((actual) != (expected)) { \
        std::cerr << "EXPECT_EQ failed: " << #actual << " != " << #expected \
                  << " | actual: \"" << (actual) << "\", expected: \"" << (expected) << "\"\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STR_EQ(actual, expected) do { \
    if ((actual) != (expected)) { \
        std::cerr << "EXPECT_STR_EQ failed: actual=\"" << (actual) \
                  << "\", expected=\"" << (expected) << "\"\n"; \
        ++g_failures; \
    } \
} while(0)

// Accessor for the last error string produced by sqlite3ErrorMsg
static const std::string& sqlite3GetLastErrorMsg() {
    return g_lastError;
}

// Forward declarations of test cases
static void test_values_branch();
static void test_non_values_branch();

// Test: SF_Values branch should produce the "all VALUES ..." error
static void test_values_branch() {
    std::cout << "Running test_values_branch: SF_Values path\n";

    sqlite3ResetLastError();

    Parse pParse;
    Select p;
    p.selFlags = SF_Values; // trigger first branch
    p.op = 0;

    sqlite3SelectWrongNumTermsError(&pParse, &p);

    const std::string& got = sqlite3GetLastErrorMsg();
    const std::string expected = "all VALUES must have the same number of terms";

    EXPECT_STR_EQ(got, expected);
}

// Test: Non-values branch should include operator name substitution
static void test_non_values_branch() {
    std::cout << "Running test_non_values_branch: non-SF_Values path\n";

    sqlite3ResetLastError();

    Parse pParse;
    Select p;
    p.selFlags = 0;   // do not take the SF_Values path
    p.op = 7;           // ensure sqlite3SelectOpName returns a known string (OP7)

    sqlite3SelectWrongNumTermsError(&pParse, &p);

    const std::string& got = sqlite3GetLastErrorMsg();
    const std::string expected = "SELECTs to the left and right of OP7 do not have the same number of result columns";

    EXPECT_STR_EQ(got, expected);
}

// Entry point for running tests
int main() {
    test_values_branch();
    test_non_values_branch();

    if (g_failures == 0) {
        std::cout << "\nAll tests passed.\n";
        return 0;
    } else {
        std::cout << "\nTests completed with " << g_failures << " failure(s).\n";
        return 1;
    }
}
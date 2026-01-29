/*
  Unit test suite for sqlite3ExprCheckHeight(Parse *pParse, int nHeight)

  Notes:
  - This test is self-contained and does not depend on GoogleTest.
  - It provides minimal stub implementations to mimic the environment required
    by sqlite3ExprCheckHeight (Parse, Db, SQLITE_OK, SQLITE_ERROR, sqlite3ErrorMsg).
  - Tests cover true/false branches of the height check, including edge cases.
  - Tests are designed to be non-terminating (print PASS/FAIL with detailed comments).
  - The test harness is intended to compile with a larger sqlite3-like build that
    exposes sqlite3ExprCheckHeight from expr.c.
*/

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Forward declaration of the focal function from expr.c
extern "C" int sqlite3ExprCheckHeight(struct Parse *pParse, int nHeight);

// Lightweight stubs to mimic the SQLite environment required by sqlite3ExprCheckHeight.
// We intentionally keep these definitions minimal and compatible with the needs of the test.

typedef struct Db Db;
typedef struct Parse Parse;

// Db mimics the sqlite3 database object with aLimit array used by sqlite3ExprCheckHeight.
struct Db {
    // A reasonably large array to accommodate any index used by the production code.
    int aLimit[128];
};

// Parse mimics the parser state that carries a pointer to the database object.
struct Parse {
    Db *db;
};

// Global storage for error messages produced by sqlite3ErrorMsg
static std::vector<std::string> g_errorMessages;

// A static helper to clear captured messages between tests.
static void clearCapturedMessages() {
    g_errorMessages.clear();
}

// A static helper to query captured messages (for assertions in tests)
static const std::vector<std::string>& getCapturedMessages() {
    return g_errorMessages;
}

// Lightweight C-style varargs wrapper to capture error messages from sqlite3ErrorMsg.
// This function must be "C" linkage to match the production code's call site.
extern "C" void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...) {
    // Build a formatted string from the variadic arguments.
    char zBuf[1024];
    va_list ap;
    va_start(ap, zFormat);
    vsnprintf(zBuf, sizeof(zBuf), zFormat, ap);
    va_end(ap);
    // Store message for later verification in tests.
    g_errorMessages.push_back(std::string(zBuf));
}

// Helper: ensure that the tests can print a readable summary.
static void printTestResult(const std::string& testName, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// Test 1: nHeight < mxHeight should return SQLITE_OK and produce no error message.
static bool test_height_less_than_mx() {
    // Arrange
    Db db;
    // Initialize all limits to a known safe height (e.g., 10)
    for (int i = 0; i < 128; ++i) db.aLimit[i] = 10;

    Parse p;
    p.db = &db;

    clearCapturedMessages();

    // Act
    int rc = sqlite3ExprCheckHeight(&p, 5);

    // Assert
    bool rcOk = (rc == 0);
    bool noMessages = getCapturedMessages().empty();

    // Explanation:
    // - Expect rc == SQLITE_OK (0) when nHeight (5) <= mxHeight (10).
    // - Expect no error message to be emitted.
    return rcOk && noMessages;
}

// Test 2: nHeight > mxHeight should return SQLITE_ERROR and emit an error message
// containing the expected maximum depth text.
static bool test_height_greater_than_mx() {
    // Arrange
    Db db;
    // Set mxHeight to a known value (e.g., 7)
    for (int i = 0; i < 128; ++i) db.aLimit[i] = 7;

    Parse p;
    p.db = &db;

    clearCapturedMessages();

    // Act
    int rc = sqlite3ExprCheckHeight(&p, 11);

    // Assert
    bool rcError = (rc == 1); // SQLITE_ERROR is typically 1
    const auto &msgs = getCapturedMessages();
    bool hasOneMessage = !msgs.empty();
    bool msgContainsText = false;
    if (hasOneMessage) {
        // The message should resemble: "Expression tree is too large (maximum depth %d)"
        // We simply check for the phrase and the exact phrase "maximum depth"
        msgContainsText = (msgs[0].find("Expression tree is too large") != std::string::npos) &&
                          (msgs[0].find("maximum depth") != std::string::npos);
    }

    // Explanation:
    // - Expect rc == SQLITE_ERROR (1) when nHeight (11) > mxHeight (7).
    // - Expect at least one error message containing the expected wording.
    return rcError && hasOneMessage && msgContainsText;
}

// Test 3: nHeight == mxHeight should return SQLITE_OK (edge case)
static bool test_height_equal_mx() {
    // Arrange
    Db db;
    for (int i = 0; i < 128; ++i) db.aLimit[i] = 15;

    Parse p;
    p.db = &db;

    clearCapturedMessages();

    // Act
    int rc = sqlite3ExprCheckHeight(&p, 15);

    // Assert
    bool rcOk = (rc == 0);
    bool noMessages = getCapturedMessages().empty();

    // Explanation:
    // - nHeight equals mxHeight, so it should not trigger an error.
    return rcOk && noMessages;
}

// Test 4: When mxHeight is 0 and nHeight > 0, an error should be produced.
static bool test_zero_mxHeight_error_case() {
    // Arrange
    Db db;
    for (int i = 0; i < 128; ++i) db.aLimit[i] = 0;

    Parse p;
    p.db = &db;

    clearCapturedMessages();

    // Act
    int rc = sqlite3ExprCheckHeight(&p, 1);

    // Assert
    bool rcError = (rc == 1);
    const auto &msgs = getCapturedMessages();
    bool hasMessage = !msgs.empty();
    bool msgContainsText = hasMessage &&
                           (msgs[0].find("Expression tree is too large") != std::string::npos);

    return rcError && hasMessage && msgContainsText;
}

// Runner
int main() {
    int totalTests = 4;
    int passed = 0;

    if (test_height_less_than_mx()) {
        ++passed;
        printTestResult("test_height_less_than_mx (nHeight < mxHeight)", true);
    } else {
        printTestResult("test_height_less_than_mx (nHeight < mxHeight)", false);
    }

    if (test_height_greater_than_mx()) {
        ++passed;
        printTestResult("test_height_greater_than_mx (nHeight > mxHeight)", true);
    } else {
        printTestResult("test_height_greater_than_mx (nHeight > mxHeight)", false);
    }

    if (test_height_equal_mx()) {
        ++passed;
        printTestResult("test_height_equal_mx (nHeight == mxHeight)", true);
    } else {
        printTestResult("test_height_equal_mx (nHeight == mxHeight)", false);
    }

    if (test_zero_mxHeight_error_case()) {
        ++passed;
        printTestResult("test_zero_mxHeight_error_case (mxHeight = 0, nHeight > 0)", true);
    } else {
        printTestResult("test_zero_mxHeight_error_case (mxHeight = 0, nHeight > 0)", false);
    }

    std::cout << "Summary: " << passed << "/" << totalTests << " tests passed." << std::endl;
    return (passed == totalTests) ? 0 : 1;
}
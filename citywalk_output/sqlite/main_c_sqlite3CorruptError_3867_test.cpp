// Unit test suite for the focal method: sqlite3CorruptError
// Target environment: C++11, no GTest, self-contained test harness.
// Notes:
// - This test suite assumes the test build links against the original
//   production translation unit containing sqlite3CorruptError (from main.c).
// - We provide lightweight mock/stubbed dependencies to exercise both branches
//   of the internal predicate (xLog != 0 and xLog == 0) and to validate the
//   interaction with sqlite3ReportError.
// - The tests use non-terminating assertions (they accumulate results and
//   report a summary at the end).

#include <iostream>
#include <sqliteInt.h>
#include <vector>
#include <string>


// Forward declarations to model the production environment for the focal method.
// We rely on C linkage for interoperability with the C source (main.c).
extern "C" {

// The focal function under test
int sqlite3CorruptError(int lineno);

// Dependency provided elsewhere in the production codebase.
// We declare it here so tests can observe/validate its parameters.
int sqlite3ReportError(int iErr, int lineno, const char *zType);

// Static/global config used by the predicate inside sqlite3CorruptError.
// In the real project, sqlite3GlobalConfig.xLog controls logging.
// We model it here so tests can drive the branch under test.
struct {
    int xLog;
} sqlite3GlobalConfig;

// SQLite error code constant used by the focal method.
#define SQLITE_CORRUPT 1

// Test macro used in the production code; provide a benign replacement that
// simply evaluates the condition (to mimic the boolean nature) without side effects.
int testcase(int cond) { return cond; }

} // extern "C"

//
// Test harness state
//
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

//
// Mock of sqlite3ReportError to observe how sqlite3CorruptError interacts
// with it. We record the last observed values for verification by the tests.
// In a real environment, the production sqlite3ReportError would perform
// logging/formatting; here we just capture parameters and return a sentinel.
//
static int last_report_iErr = -1;
static int last_report_lineno = -1;
static const char* last_report_zType = nullptr;
static int report_calls = 0;

extern "C" int sqlite3ReportError(int iErr, int lineno, const char *zType) {
    ++report_calls;
    last_report_iErr = iErr;
    last_report_lineno = lineno;
    last_report_zType = zType;
    // Return a sentinel value to allow test to verify the return path.
    return 12345;
}

//
// Tests
//

// Test 1: True branch of the predicate (sqlite3GlobalConfig.xLog != 0)
// Expectation:
// - The internal testcase predicate evaluates to true (modeled by the macro).
// - sqlite3CorruptError should call sqlite3ReportError with SQLITE_CORRUPT,
//   the provided lineno, and the message "database corruption".
// - The return value should be the value returned by sqlite3ReportError (12345 here).
bool test_sqlite3CorruptError_xLog_true() {
    // Arrange
    sqlite3GlobalConfig.xLog = 1;              // true branch
    last_report_iErr = -1;
    last_report_lineno = -1;
    last_report_zType = nullptr;
    report_calls = 0;

    int lineno = 42;
    // Act
    int ret = sqlite3CorruptError(lineno);

    // Assert (non-terminating)
    bool ok = true;
    if (ret != 12345) {
        ok = false;
    }
    if (report_calls != 1) {
        ok = false;
    }
    if (last_report_iErr != SQLITE_CORRUPT) {
        ok = false;
    }
    if (last_report_lineno != lineno) {
        ok = false;
    }
    if (last_report_zType == nullptr) {
        ok = false;
    } else {
        std::string s(last_report_zType);
        if (s != "database corruption") ok = false;
    }

    if (!ok) {
        // Build a descriptive message for failure
        std::ostringstream oss;
        oss << "test_sqlite3CorruptError_xLog_true failed: "
            << "ret=" << ret
            << ", calls=" << report_calls
            << ", iErr=" << last_report_iErr
            << ", lineno=" << last_report_lineno
            << ", zType=" << (last_report_zType ? last_report_zType : "null");
        return false;
    }
    return true;
}

// Test 2: False branch of the predicate (sqlite3GlobalConfig.xLog == 0)
// Expectation:
// - The internal predicate evaluates to false; the code-path should still
//   proceed to call sqlite3ReportError with the same parameters as above.
// - The return value is the same sentinel from sqlite3ReportError (12345).
bool test_sqlite3CorruptError_xLog_false() {
    // Arrange
    sqlite3GlobalConfig.xLog = 0;              // false branch
    last_report_iErr = -1;
    last_report_lineno = -1;
    last_report_zType = nullptr;
    report_calls = 0;

    int lineno = 999;
    // Act
    int ret = sqlite3CorruptError(lineno);

    // Assert (non-terminating)
    bool ok = true;
    if (ret != 12345) {
        ok = false;
    }
    if (report_calls != 1) {
        ok = false;
    }
    if (last_report_iErr != SQLITE_CORRUPT) {
        ok = false;
    }
    if (last_report_lineno != lineno) {
        ok = false;
    }
    if (last_report_zType == nullptr) {
        ok = false;
    } else {
        std::string s(last_report_zType);
        if (s != "database corruption") ok = false;
    }

    if (!ok) {
        std::ostringstream oss;
        oss << "test_sqlite3CorruptError_xLog_false failed: "
            << "ret=" << ret
            << ", calls=" << report_calls
            << ", iErr=" << last_report_iErr
            << ", lineno=" << last_report_lineno
            << ", zType=" << (last_report_zType ? last_report_zType : "null");
        return false;
    }
    return true;
}

// Test 3: Validate that the function passes the correct iErr value
// This test enforces that SQLITE_CORRUPT is used (the same for both branches)
// by resetting the last observed value and ensuring it's SQLITE_CORRUPT.
bool test_sqlite3CorruptError_iErr_value() {
    sqlite3GlobalConfig.xLog = 1;
    last_report_iErr = -1;
    last_report_lineno = -1;
    last_report_zType = nullptr;
    report_calls = 0;

    int lineno = 7;
    int ret = sqlite3CorruptError(lineno);

    bool ok = true;
    if (ret != 12345) ok = false;
    if (last_report_iErr != SQLITE_CORRUPT) ok = false;
    if (last_report_lineno != lineno) ok = false;
    if (last_report_zType == nullptr) ok = false;
    else if (std::string(last_report_zType) != "database corruption") ok = false;
    if (!ok) {
        std::ostringstream oss;
        oss << "test_sqlite3CorruptError_iErr_value failed: ret=" << ret
            << ", iErr=" << last_report_iErr
            << ", lineno=" << last_report_lineno
            << ", zType=" << (last_report_zType ? last_report_zType : "null");
        return false;
    }
    return true;
}

// Main test driver
int main() {
    std::vector<TestResult> results;

    // Helper to register and print test results
    auto addResult = [&](const std::string& name, bool passed, const std::string& msg){
        results.push_back(TestResult{name, passed, msg});
    };

    // Execute tests
    {
        bool ok = test_sqlite3CorruptError_xLog_true();
        addResult("test_sqlite3CorruptError_xLog_true", ok, ok ? "" : "Mismatch in expected behavior for xLog != 0");
    }
    {
        bool ok = test_sqlite3CorruptError_xLog_false();
        addResult("test_sqlite3CorruptError_xLog_false", ok, ok ? "" : "Mismatch in expected behavior for xLog == 0");
    }
    {
        bool ok = test_sqlite3CorruptError_iErr_value();
        addResult("test_sqlite3CorruptError_iErr_value", ok, ok ? "" : "iErr value check failed");
    }

    // Print summary
    int passed = 0;
    int failed = 0;
    for (const auto& r : results) {
        if (r.passed) {
            ++passed;
            std::cout << "[PASS] " << r.name << "\n";
        } else {
            ++failed;
            std::cout << "[FAIL] " << r.name << " - " << r.message << "\n";
        }
    }
    std::cout << "Test results: " << passed << " passed, " << failed << " failed, total " 
              << results.size() << "\n";

    return (failed == 0) ? 0 : 1;
}
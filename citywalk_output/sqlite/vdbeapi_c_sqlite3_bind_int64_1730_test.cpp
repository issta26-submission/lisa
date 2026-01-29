// Unit test suite for sqlite3_bind_int64 (focal method) using public SQLite API.
// This test suite is designed to be compiled with C++11, without GTest, and
// uses the in-memory SQLite database to verify the binding behavior via a
// simple SELECT ?1 query which should reflect the bound value.

#include <vector>
#include <cstdio>
#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
struct TestResult {
    int failed;
    std::vector<std::string> messages;

    TestResult() : failed(0) {}
    void fail(const std::string &msg) {
        ++failed;
        messages.push_back(msg);
    }
};

// Helper to print per-test summary
static void printTestResult(const std::string &name, const TestResult &r) {
    if (r.failed == 0) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cout << "[FAIL] " << name << " (failures: " << r.failed << ")\n";
        for (const auto &m : r.messages) {
            std::cout << "  - " << m << "\n";
        }
    }
}

// Test 1: Basic binding of a 64-bit value and retrieval via a simple SELECT ?1
// This verifies that sqlite3_bind_int64 correctly stores the value which is later
// returned by a query that selects the bound parameter.
static bool test_bind_int64_basic(TestResult &result) {
    // Setup in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        result.fail("test_bind_int64_basic: sqlite3_open failed");
        return false;
    }

    // Prepare a simple statement that returns the value of its first parameter
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT ?1", -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == nullptr) {
        result.fail("test_bind_int64_basic: sqlite3_prepare_v2 failed");
        sqlite3_close(db);
        return false;
    }

    // Bind a 64-bit integer
    sqlite3_int64 expected = 123456789012345LL;
    rc = sqlite3_bind_int64(stmt, 1, expected);
    if (rc != SQLITE_OK) {
        result.fail("test_bind_int64_basic: sqlite3_bind_int64 failed to bind value");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    // Execute and fetch result
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        result.fail("test_bind_int64_basic: sqlite3_step did not return a row");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_int64 actual = sqlite3_column_int64(stmt, 0);
    if (actual != expected) {
        result.fail("test_bind_int64_basic: Bound value mismatch (expected vs actual)");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

// Test 2: Re-binding the same parameter with a new value and verifying update
// This ensures that subsequent sqlite3_bind_int64 calls overwrite previous bound value
// and that the new value is observable via a simple SELECT ?1.
static bool test_bind_int64_update(TestResult &result) {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        result.fail("test_bind_int64_update: sqlite3_open failed");
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT ?1", -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == nullptr) {
        result.fail("test_bind_int64_update: sqlite3_prepare_v2 failed");
        sqlite3_close(db);
        return false;
    }

    // First bound value
    sqlite3_int64 first = 5;
    rc = sqlite3_bind_int64(stmt, 1, first);
    if (rc != SQLITE_OK) {
        result.fail("test_bind_int64_update: sqlite3_bind_int64 failed on first bind");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        result.fail("test_bind_int64_update: sqlite3_step failed on first bind");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_int64 got = sqlite3_column_int64(stmt, 0);
    if (got != first) {
        result.fail("test_bind_int64_update: First bound value mismatch");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    // Reset statement and re-bind a new value to the same parameter
    rc = sqlite3_reset(stmt);
    if (rc != SQLITE_OK) {
        result.fail("test_bind_int64_update: sqlite3_reset failed before second bind");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_int64 second = 42;
    rc = sqlite3_bind_int64(stmt, 1, second);
    if (rc != SQLITE_OK) {
        result.fail("test_bind_int64_update: sqlite3_bind_int64 failed on second bind");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        result.fail("test_bind_int64_update: sqlite3_step failed on second bind");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    got = sqlite3_column_int64(stmt, 0);
    if (got != second) {
        result.fail("test_bind_int64_update: Second bound value mismatch");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

// Test 3: Binding a negative 64-bit value to ensure proper handling of signed values
static bool test_bind_int64_negative(TestResult &result) {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        result.fail("test_bind_int64_negative: sqlite3_open failed");
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT ?1", -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == nullptr) {
        result.fail("test_bind_int64_negative: sqlite3_prepare_v2 failed");
        sqlite3_close(db);
        return false;
    }

    sqlite3_int64 neg = -987654321012345LL;
    rc = sqlite3_bind_int64(stmt, 1, neg);
    if (rc != SQLITE_OK) {
        result.fail("test_bind_int64_negative: sqlite3_bind_int64 failed to bind negative value");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        result.fail("test_bind_int64_negative: sqlite3_step did not return a row for negative value");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_int64 actual = sqlite3_column_int64(stmt, 0);
    if (actual != neg) {
        result.fail("test_bind_int64_negative: Negative bound value mismatch");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

int main() {
    // Run all tests and report results
    std::vector<std::pair<std::string, bool>> tests = {
        {"test_bind_int64_basic", test_bind_int64_basic},
        {"test_bind_int64_update", test_bind_int64_update},
        {"test_bind_int64_negative", test_bind_int64_negative}
    };

    int total = 0;
    int failures = 0;
    std::vector<std::string> log;

    for (auto &t : tests) {
        TestResult res;
        bool ok = t.second(res);
        // However, the test wrappers defined above return bools; adapt accordingly
        // We will rerun with a dedicated runner to capture the true result.
        // For simplicity, rerun in a local scope to collect pass/fail.
        // The above approach already returns directly; we keep structure simple.

        // Since the function signature is fixed to return bool,
        // we re-check by re-running and capturing via separate call wrappers.
        // To avoid duplication, we re-invoke and interpret return value here.
    }

    // Re-run tests with results captured inline for final reporting
    int failuresCount = 0;

    // Re-define inline runner to collect per-test results (redundant but clear)
    {
        TestResult r;
        bool ok = test_bind_int64_basic(r);
        printTestResult("test_bind_int64_basic", r);
        if (!ok) ++failuresCount;
    }
    {
        TestResult r;
        bool ok = test_bind_int64_update(r);
        printTestResult("test_bind_int64_update", r);
        if (!ok) ++failuresCount;
    }
    {
        TestResult r;
        bool ok = test_bind_int64_negative(r);
        printTestResult("test_bind_int64_negative", r);
        if (!ok) ++failuresCount;
    }

    if (failuresCount == 0) {
        std::cout << "[OVERALL PASS] All tests passed.\n";
        return 0;
    } else {
        std::cout << "[OVERALL FAIL] " << failuresCount << " test(s) failed.\n";
        return 1;
    }
}
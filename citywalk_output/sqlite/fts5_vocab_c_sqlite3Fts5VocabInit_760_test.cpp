// Unit test suite for sqlite3Fts5VocabInit
// Target: C++11, no GTest, plain main-based harness with lightweight assertions.
// The test suite exercises the focal method in fts5_vocab.c by loading an in-memory
// SQLite database and invoking sqlite3Fts5VocabInit. It uses a forward-declared
// incomplete type for Fts5Global to avoid dependencies on the actual internal
// structure, since the tests only require passing a non-null pointer through to
// sqlite3_create_module_v2.

#include <vector>
#include <fts5Int.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <utility>



// Forward declare the Fts5Global type to avoid including internal headers.
// We only need a pointer to this type to pass into sqlite3Fts5VocabInit.
struct Fts5Global;

// Declare the focal function with C linkage to ensure proper symbol resolution
// when linking a C translation unit (fts5_vocab.c) with this C++ test.
extern "C" int sqlite3Fts5VocabInit(Fts5Global *pGlobal, sqlite3 *db);

// Lightweight test framework (non-terminating assertions)
struct TestCase {
    std::string name;
    std::function<bool()> func;
};

// Simple assertion helpers
static void report_test(const std::string &name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cout << "[FAIL] " << name << "\n";
    }
}

// TEST 1
// Purpose: Validate that sqlite3Fts5VocabInit successfully registers the fts5vocab module
// on a fresh in-memory SQLite database and returns SQLITE_OK.
// Rationale: This covers the primary success path of the focal method.
// Candidate Keywords: sqlite3_module, xCreate/..xRowid, sqlite3_create_module_v2, "fts5vocab", pAux
bool test_sqlite3Fts5VocabInit_basic() {
    // Open an in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        // If opening the database fails, the test cannot proceed
        return false;
    }

    // Create a dummy Fts5Global pointer (incomplete type is sufficient for a non-null value)
    struct Fts5Global *pGlobal = reinterpret_cast<Fts5Global*>(0x1234);

    // Invoke the focal method
    rc = sqlite3Fts5VocabInit(pGlobal, db);

    // Clean up
    sqlite3_close(db);

    // Expect SQLITE_OK as the result code
    return rc == SQLITE_OK;
}

// TEST 2
// Purpose: Validate that sqlite3Fts5VocabInit can be invoked multiple times with
// separate in-memory databases without crashing or causing cross-database interference.
// Rationale: This checks robustness of module initialization across distinct DB handles.
// Candidate Keywords: sqlite3_create_module_v2, per-database module registration
bool test_sqlite3Fts5VocabInit_multipleDatabases() {
    // First database
    sqlite3 *db1 = nullptr;
    int rc1 = sqlite3_open(":memory:", &db1);
    if (rc1 != SQLITE_OK || db1 == nullptr) {
        return false;
    }

    // Second database
    sqlite3 *db2 = nullptr;
    int rc2 = sqlite3_open(":memory:", &db2);
    if (rc2 != SQLITE_OK || db2 == nullptr) {
        sqlite3_close(db1);
        return false;
    }

    // Dummy pointer
    struct Fts5Global *pGlobal = reinterpret_cast<Fts5Global*>(0x5678);

    // Initialize on both databases
    int rcA = sqlite3Fts5VocabInit(pGlobal, db1);
    int rcB = sqlite3Fts5VocabInit(pGlobal, db2);

    // Cleanup
    sqlite3_close(db1);
    sqlite3_close(db2);

    // Both initializations should return SQLITE_OK
    return (rcA == SQLITE_OK) && (rcB == SQLITE_OK);
}

// TEST 3 (Optional resilience check)
// Purpose: Ensure that repeated invocations do not crash the process when using
// distinct db handles. This test focuses on stability rather than functional correctness.
// Rationale: Tests that the focal method does not rely on any per-call static state
// that would be polluted across calls in real usage.
bool test_sqlite3Fts5VocabInit_noGlobalStateLeak() {
    // Create a single in-memory database instance
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        return false;
    }

    // Two distinct dummy pGlobal pointers to simulate separate contexts
    struct Fts5Global *pGlobal1 = reinterpret_cast<Fts5Global*>(0x1111);
    struct Fts5Global *pGlobal2 = reinterpret_cast<Fts5Global*>(0x2222);

    int rc1 = sqlite3Fts5VocabInit(pGlobal1, db);
    int rc2 = sqlite3Fts5VocabInit(pGlobal2, db);

    sqlite3_close(db);

    // Both invocations should succeed
    return (rc1 == SQLITE_OK) && (rc2 == SQLITE_OK);
}

int main() {
    // Collect and run tests
    std::vector<TestCase> tests = {
        {"test_sqlite3Fts5VocabInit_basic", test_sqlite3Fts5VocabInit_basic},
        {"test_sqlite3Fts5VocabInit_multipleDatabases", test_sqlite3Fts5VocabInit_multipleDatabases},
        {"test_sqlite3Fts5VocabInit_noGlobalStateLeak", test_sqlite3Fts5VocabInit_noGlobalStateLeak}
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    for (auto &t : tests) {
        bool ok = false;
        try {
            ok = t.func();
        } catch (...) {
            ok = false;
        }
        report_test(t.name, ok);
        if (ok) ++passed;
    }

    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";
    // Return non-zero on failure to integrate with CI expectations if desired
    return (passed == total) ? 0 : 1;
}
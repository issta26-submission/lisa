// C++11 unit test suite for sqlite3_transfer_bindings (vdbeapi.c)
// - No GoogleTest. A lightweight in-file test harness is used.
// - Focuses on testing key branches of sqlite3_transfer_bindings as described.
// - Explains each test via comments and prints results to stdout.

#include <vector>
#include <cstdio>
#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Include internal SQLite headers to access underlying types and macros.
// These headers are typically available in the SQLite source tree.
// If building in a different environment, adjust include paths accordingly.
extern "C" {
}

// Forward declare the function under test to ensure C linkage.
extern "C" int sqlite3_transfer_bindings(sqlite3_stmt *pFromStmt, sqlite3_stmt *pToStmt);

// Also forward declare the internal binding transfer to compare results directly.
extern "C" int sqlite3TransferBindings(sqlite3_stmt *pFromStmt, sqlite3_stmt *pToStmt);

// Lightweight test harness
namespace TestHarness {
    struct TestCase {
        std::string name;
        std::function<void()> run;
    };

    static int g_failures = 0;
    static int g_tests = 0;

    // Simple non-terminating assertion macro
    #define TEST_EXPECT(cond, msg) do { \
        if(!(cond)) { \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << std::endl; \
            ++g_failures; \
        } \
        ++g_tests; \
    } while(0)

    // Helper to create a Vdbe-based sqlite3_stmt* suitable for testing
    // We cast a Vdbe* to sqlite3_stmt* as sqlite3_transfer_bindings expects.
    static sqlite3_stmt* make_stmt(int nVar, unsigned int prepFlags, unsigned int expmask) {
        Vdbe* p = new Vdbe();
        // Initialize fields required by sqlite3_transfer_bindings
        // Other fields are left zeroed by default; they are not accessed by this test path.
        p->nVar = nVar;
        p->prepFlags = prepFlags;
        p->expmask = expmask;
        p->expired = 0;
        // We return as a sqlite3_stmt* (as the production code would)
        return reinterpret_cast<sqlite3_stmt*>(p);
    }

    // Helper to free the memory associated with the Vdbe object
    static void free_stmt(sqlite3_stmt* stmt) {
        if (stmt) {
            Vdbe* p = reinterpret_cast<Vdbe*>(stmt);
            delete p;
        }
    }
}

// Test 1: nVar mismatch should yield SQLITE_ERROR
// pFromStmt.nVar != pToStmt.nVar
void test_nvar_mismatch() {
    // Arrange
    using namespace TestHarness;

    sqlite3_stmt* pFromStmt = make_stmt(1, 0, 0); // nVar = 1
    sqlite3_stmt* pToStmt   = make_stmt(2, 0, 0); // nVar = 2 (mismatch)

    // Act
    int rc = sqlite3_transfer_bindings(pFromStmt, pToStmt);

    // Assert
    TEST_EXPECT(rc == SQLITE_ERROR, "Expected SQLITE_ERROR on nVar mismatch");
    // Cleanup
    free_stmt(pFromStmt);
    free_stmt(pToStmt);
}

// Test 2: pTo has expmask set; ensure pTo->expired becomes 1 and returns non-error.
// Also verify that the assertion is satisfied by giving SAVESQL flag.
void test_to_expmask_expiration_sets() {
    using namespace TestHarness;

    // Setup: nVar equal, toStmt expmask nonzero, and prepFlags include SAVESQL
    sqlite3_stmt* pFromStmt = make_stmt(3, SQLITE_PREPARE_SAVESQL, 0);
    sqlite3_stmt* pToStmt   = make_stmt(3, SQLITE_PREPARE_SAVESQL, 1); // expmask != 0

    // Act
    int old_expired_to = reinterpret_cast<Vdbe*>(pToStmt)->expired;
    int rc = sqlite3_transfer_bindings(pFromStmt, pToStmt);

    // Assert: expired flag should be set on pTo when expmask is nonzero
    TEST_EXPECT(reinterpret_cast<Vdbe*>(pToStmt)->expired == 1, "pTo->expired should be set to 1 when expmask != 0");
    // The function should not return SQLITE_ERROR for this case (asserts are satisfied)
    TEST_EXPECT(rc != SQLITE_ERROR, "Wrapper should not return SQLITE_ERROR when equal nVar and valid flags");

    // Also compare wrapper's return value with a direct call on fresh statements
    // This verifies the wrapper delegates to sqlite3TransferBindings correctly.
    sqlite3_stmt* from2 = make_stmt(3, SQLITE_PREPARE_SAVESQL, 1);
    sqlite3_stmt* to2   = make_stmt(3, SQLITE_PREPARE_SAVESQL, 1);
    int expected = sqlite3TransferBindings(from2, to2);
    free_stmt(from2);
    free_stmt(to2);

    // Rebuild fresh statements for wrapper call to compare
    sqlite3_stmt* pFromStmt2 = make_stmt(3, SQLITE_PREPARE_SAVESQL, 1);
    sqlite3_stmt* pToStmt2   = make_stmt(3, SQLITE_PREPARE_SAVESQL, 1);
    int actual = sqlite3_transfer_bindings(pFromStmt2, pToStmt2);
    TEST_EXPECT(actual == expected, "Wrapper return value should match direct sqlite3TransferBindings result");

    // Cleanup
    free_stmt(pFromStmt);
    free_stmt(pToStmt);
    free_stmt(pFromStmt2);
    free_stmt(pToStmt2);
}

// Test 3: pFrom has expmask set; ensure pFrom->expired becomes 1 and asserts are satisfied
void test_from_expmask_expiration_sets() {
    using namespace TestHarness;

    // Setup: pFrom expmask nonzero with SAVESQL set, pTo normal (expMask 0)
    sqlite3_stmt* pFromStmt = make_stmt(4, SQLITE_PREPARE_SAVESQL, 1);
    sqlite3_stmt* pToStmt   = make_stmt(4, 0, 0);

    int rc = sqlite3_transfer_bindings(pFromStmt, pToStmt);

    TEST_EXPECT(reinterpret_cast<Vdbe*>(pFromStmt)->expired == 1, "pFrom->expired should be set to 1 when pFrom->expmask != 0");
    // Should not trigger error in this configuration
    TEST_EXPECT(rc != SQLITE_ERROR, "Wrapper should not return SQLITE_ERROR when equal nVar and valid flags");

    // Cleanup
    free_stmt(pFromStmt);
    free_stmt(pToStmt);
}

// Test 4: Both pFrom and pTo have expmask set; both expired flags should be updated
// Also ensure wrapper's return matches direct sqlite3TransferBindings for parity
void test_both_expmask_set_expired_and_return_matches() {
    using namespace TestHarness;

    sqlite3_stmt* pFromStmt = make_stmt(5, SQLITE_PREPARE_SAVESQL, 2); // expmask nonzero
    sqlite3_stmt* pToStmt   = make_stmt(5, SQLITE_PREPARE_SAVESQL, 2); // expmask nonzero

    // Act
    int rc = sqlite3_transfer_bindings(pFromStmt, pToStmt);

    // Assert expired flags
    TEST_EXPECT(reinterpret_cast<Vdbe*>(pFromStmt)->expired == 1, "pFrom->expired should be set when pFrom->expmask != 0");
    TEST_EXPECT(reinterpret_cast<Vdbe*>(pToStmt)->expired == 1, "pTo->expired should be set when pTo->expmask != 0");

    // Compare wrapper with direct transfer result on fresh statements to ensure parity
    sqlite3_stmt* a = make_stmt(5, SQLITE_PREPARE_SAVESQL, 2);
    sqlite3_stmt* b = make_stmt(5, SQLITE_PREPARE_SAVESQL, 2);
    int expected = sqlite3TransferBindings(a, b);
    free_stmt(a);
    free_stmt(b);

    sqlite3_stmt* wa = make_stmt(5, SQLITE_PREPARE_SAVESQL, 2);
    sqlite3_stmt* wb = make_stmt(5, SQLITE_PREPARE_SAVESQL, 2);
    int actual = sqlite3_transfer_bindings(wa, wb);
    TEST_EXPECT(actual == expected, "Wrapper return should match direct sqlite3TransferBindings when both expmask != 0");

    // Cleanup
    free_stmt(pFromStmt);
    free_stmt(pToStmt);
    free_stmt(wa);
    free_stmt(wb);
}

// Test 5: Equal nVar and both expmask == 0; verify wrapper delegates to sqlite3TransferBindings
// and returns the same value as a direct call on fresh statements
void test_explicit_return_matches_underlying_without_expired() {
    using namespace TestHarness;

    sqlite3_stmt* pFromStmt = make_stmt(6, 0, 0);
    sqlite3_stmt* pToStmt   = make_stmt(6, 0, 0);

    // First, compute expected using direct underlying function on fresh statements
    sqlite3_stmt* af = make_stmt(6, 0, 0);
    sqlite3_stmt* bf = make_stmt(6, 0, 0);
    int expected = sqlite3TransferBindings(af, bf);
    free_stmt(af);
    free_stmt(bf);

    // Now call the wrapper on fresh statements with identical configuration
    sqlite3_stmt* wfFrom = make_stmt(6, 0, 0);
    sqlite3_stmt* wfTo   = make_stmt(6, 0, 0);
    int actual = sqlite3_transfer_bindings(wfFrom, wfTo);

    TEST_EXPECT(actual == expected, "Wrapper return should match direct sqlite3TransferBindings when no expmask");

    // Also ensure no expired flags were altered
    TEST_EXPECT(reinterpret_cast<Vdbe*>(wfFrom)->expired == 0, "pFrom->expired should remain 0 when expmask == 0");
    TEST_EXPECT(reinterpret_cast<Vdbe*>(wfTo)->expired == 0,   "pTo->expired should remain 0 when expmask == 0");

    // Cleanup
    free_stmt(pFromStmt);
    free_stmt(pToStmt);
    free_stmt(wfFrom);
    free_stmt(wfTo);
}

// Entry point: register and run all tests
int main() {
    using namespace TestHarness;

    std::vector<TestCase> tests = {
        {"test_nvar_mismatch", test_nvar_mismatch},
        {"test_to_expmask_expiration_sets", test_to_expmask_expiration_sets},
        {"test_from_expmask_expiration_sets", test_from_expmask_expiration_sets},
        {"test_both_expmask_set_expired_and_return_matches", test_both_expmask_set_expired_and_return_matches},
        {"test_explicit_return_matches_underlying_without_exp", test_explicit_return_matches_underlying_without_expended}
    };

    // Run tests
    for (auto &tc : tests) {
        std::cout << "Running: " << tc.name << std::endl;
        tc.run();
        std::cout << "Finished: " << tc.name << "\n" << std::endl;
    }

    // Summary
    std::cout << "Test summary: " << g_tests << " assertions attempted, "
              << g_failures << " failures." << std::endl;

    // Return non-zero if any test failed
    return g_failures == 0 ? 0 : 1;
}
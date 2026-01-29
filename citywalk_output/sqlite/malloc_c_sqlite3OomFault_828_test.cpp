// Test suite for sqlite3OomFault in malloc.c
// Target: C++11, no Google Test. Use a lightweight, self-contained test harness.
// The tests rely on internal SQLite structures provided by sqliteInt.h.

#include <iomanip>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Bring in the internal SQLite structures and declarations.
// Note: This uses internal headers; ensure the project provides sqliteInt.h in include path.
extern "C" {
}

// Declaration of the focal function under test (C linkage for compatibility)
extern "C" void *sqlite3OomFault(sqlite3 *db);

// Simple, lightweight test harness utilities

static void printHeader(const char* title) {
    std::cout << "\n=== " << title << " ===" << std::endl;
}

static void printResult(const char* testName, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << "\n";
}

// Lightweight assertion helper that does not terminate the test suite
#define ASSERT_EQ(actual, expected, testName) do { \
    if((actual) != (expected)) { \
        std::cerr << "[ASSERT_FAIL] " << testName \
                  << " — expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
        return false; \
    } \
} while(0)


// Test 1: When mallocFailed != 0 (or bBenignMalloc != 0), the function should do nothing.
// This covers the false branch of the main predicate.
bool test_noop_when_flags_set() {
    // Arrange
    // Zero-initialized db to begin with; then set a blocking condition.
    sqlite3 db {};
    db.mallocFailed = 1;
    db.bBenignMalloc = 0;
    db.nVdbeExec = 0;
    db.u1.isInterrupted = 0;
    db.pParse = nullptr;

    // Act
    void *ret = sqlite3OomFault(&db);

    // Assert
    ASSERT_EQ(reinterpret_cast<std::uintptr_t>(ret), 0, "test_noop_when_flags_set: return value must be 0");
    ASSERT_EQ(db.mallocFailed, 1, "test_noop_when_flags_set: mallocFailed should remain 1");
    printResult("test_noop_when_flags_set", true);
    return true;
}

// Test 2: When mallocFailed == 0 and bBenignMalloc == 0 and nVdbeExec == 0 and no pParse,
// the function should set mallocFailed to 1 and return 0.
bool test_set_malloc_failed_no_pParse() {
    // Arrange
    sqlite3 db {};
    db.mallocFailed = 0;
    db.bBenignMalloc = 0;
    db.nVdbeExec = 0;
    db.u1.isInterrupted = 0;
    db.pParse = nullptr;

    // Act
    void *ret = sqlite3OomFault(&db);

    // Assert
    ASSERT_EQ(reinterpret_cast<std::uintptr_t>(ret), 0, "test_set_malloc_failed_no_pParse: return value must be 0");
    ASSERT_EQ(db.mallocFailed, 1, "test_set_malloc_failed_no_pParse: mallocFailed should be set to 1");
    printResult("test_set_malloc_failed_no_pParse", true);
    return true;
}

// Test 3: When nVdbeExec > 0 (and no pParse), the function should set isInterrupted to 1 as part of OOM handling
bool test_interrupt_when_vdbeExec_gt_zero_no_pParse() {
    // Arrange
    sqlite3 db {};
    db.mallocFailed = 0;
    db.bBenignMalloc = 0;
    db.nVdbeExec = 3; // trigger isInterrupted path
    db.u1.isInterrupted = 0;
    db.pParse = nullptr;

    // Act
    void *ret = sqlite3OomFault(&db);

    // Assert
    ASSERT_EQ(reinterpret_cast<std::uintptr_t>(ret), 0, "test_interrupt_when_vdbeExec_gt_zero_no_pParse: return value must be 0");
    ASSERT_EQ(db.mallocFailed, 1, "test_interrupt_when_vdbeExec_gt_zero_no_pParse: mallocFailed should be set to 1");
    ASSERT_EQ(db.u1.isInterrupted, 1, "test_interrupt_when_vdbeExec_gt_zero_no_pParse: isInterrupted should be 1");
    printResult("test_interrupt_when_vdbeExec_gt_zero_no_pParse", true);
    return true;
}

// Test 4: When a pParse chain exists, ensure that:
//  - mallocFailed becomes 1
//  - pParse.rc is set to SQLITE_NOMEM_BKPT
//  - Each pParse in the OuterParse chain has its rc set to SQLITE_NOMEM
//  - Each pParse in the chain has its nErr incremented by 1
bool test_pParse_chain_sets_rc_and_nErr() {
    // Arrange: Build a simple InnerParse -> OuterParse chain
    Parse outer;
    outer.rc = 0;
    outer.nErr = 0;
    outer.pOuterParse = NULL;

    Parse inner;
    inner.rc = 0;
    inner.nErr = 0;
    inner.pOuterParse = &outer;

    sqlite3 db {};
    db.mallocFailed = 0;
    db.bBenignMalloc = 0;
    db.nVdbeExec = 0;
    db.u1.isInterrupted = 0;
    db.pParse = &inner;

    // Act
    void *ret = sqlite3OomFault(&db);

    // Assert
    ASSERT_EQ(reinterpret_cast<std::uintptr_t>(ret), 0, "test_pParse_chain_sets_rc_and_nErr: return value must be 0");
    ASSERT_EQ(db.mallocFailed, 1, "test_pParse_chain_sets_rc_and_nErr: mallocFailed should be set to 1");

    // inner and outer should have rc updated
    ASSERT_EQ(inner.rc, SQLITE_NOMEM, "test_pParse_chain_sets_rc_and_nErr: inner.rc should be SQLITE_NOMEM");
    ASSERT_EQ(outer.rc, SQLITE_NOMEM, "test_pParse_chain_sets_rc_and_nErr: outer.rc should be SQLITE_NOMEM");

    // nErr should be incremented for both inner and outer
    ASSERT_EQ(inner.nErr, 1, "test_pParse_chain_sets_rc_and_nErr: inner.nErr should be incremented to 1");
    ASSERT_EQ(outer.nErr, 1, "test_pParse_chain_sets_rc_and_nErr: outer.nErr should be incremented to 1");

    printResult("test_pParse_chain_sets_rc_and_nErr", true);
    return true;
}


// Driver: run all tests
int main() {
    printHeader("sqlite3OomFault test suite (C++11, no GTest)");
    bool all_passed = true;

    if (!test_noop_when_flags_set()) all_passed = false;
    if (!test_set_malloc_failed_no_pParse()) all_passed = false;
    if (!test_interrupt_when_vdbeExec_gt_zero_no_pParse()) all_passed = false;
    if (!test_pParse_chain_sets_rc_and_nErr()) all_passed = false;

    std::cout << "\nOverall result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;

    // Return non-zero if any test failed
    return all_passed ? 0 : 1;
}
// Test suite for sqlite3BtreeGetPageSize
// Purpose: verify that the function returns p->pBt->pageSize for a given Btree instance.
// Assumptions: The project provides the real sqlite3BtreeGetPageSize implementation (in btree.c)
// and its related types are exposed via btreeInt.h. This test uses those exposed types
// (BtShared, Btree) and links against the project's implementation.

#include <btreeInt.h>
#include <iostream>
#include <string>


// Import C declarations in a C++ test environment
extern "C" {
}

// Simple non-terminating test harness
static int g_all_tests_passed = 1;

static void log_failure(const char* testName, const char* detail) {
    std::cerr << "[FAIL] " << testName << ": " << detail << std::endl;
    g_all_tests_passed = 0;
}

// Test 1: Basic positive page size should be returned
// Rationale: Ensure that the function returns the actual pageSize stored in BtShared.
static void test_sqlite3BtreeGetPageSize_basic_positive() {
    // Arrange
    BtShared shared;
    Btree btree;
    const int expectedPageSize = 4096;
    shared.pageSize = expectedPageSize;
    btree.pBt = &shared;

    // Act
    int actualPageSize = sqlite3BtreeGetPageSize(&btree);

    // Assert
    if (actualPageSize != expectedPageSize) {
        log_failure("test_sqlite3BtreeGetPageSize_basic_positive",
                    "Expected pageSize to match BtShared.pageSize");
    }
}

// Test 2: Zero page size should be returned if set to zero
// Rationale: Check boundary condition where pageSize is 0.
static void test_sqlite3BtreeGetPageSize_zero() {
    // Arrange
    BtShared shared;
    Btree btree;
    const int expectedPageSize = 0;
    shared.pageSize = expectedPageSize;
    btree.pBt = &shared;

    // Act
    int actualPageSize = sqlite3BtreeGetPageSize(&btree);

    // Assert
    if (actualPageSize != expectedPageSize) {
        log_failure("test_sqlite3BtreeGetPageSize_zero",
                    "Expected pageSize to be 0 when BtShared.pageSize is 0");
    }
}

// Test 3: Large page size should be faithfully returned
// Rationale: Validate behavior with a larger integer value.
static void test_sqlite3BtreeGetPageSize_large() {
    // Arrange
    BtShared shared;
    Btree btree;
    const int expectedPageSize = 16384;
    shared.pageSize = expectedPageSize;
    btree.pBt = &shared;

    // Act
    int actualPageSize = sqlite3BtreeGetPageSize(&btree);

    // Assert
    if (actualPageSize != expectedPageSize) {
        log_failure("test_sqlite3BtreeGetPageSize_large",
                    "Expected pageSize to match the large BtShared.pageSize value");
    }
}

// Main test runner
int main() {
    // Run tests
    test_sqlite3BtreeGetPageSize_basic_positive();
    test_sqlite3BtreeGetPageSize_zero();
    test_sqlite3BtreeGetPageSize_large();

    // Summary
    if (g_all_tests_passed) {
        std::cout << "[OK] All sqlite3BtreeGetPageSize tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[ERROR] Some sqlite3BtreeGetPageSize tests failed." << std::endl;
        return 1;
    }
}
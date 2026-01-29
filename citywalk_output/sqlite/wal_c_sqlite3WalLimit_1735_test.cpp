/*
  C++11 unit test suite for the focal method:
  void sqlite3WalLimit(Wal *pWal, i64 iLimit)

  Assumptions:
  - The project under test provides wal.h (and the related Wal type) with
    the necessary layout so that we can instantiate Wal and access its
    mxWalSize field in C++ tests.
  - We do not rely on any GTest or other testing frameworks.
  - We only test the observable behavior of sqlite3WalLimit:
    1) When pWal is non-null, mxWalSize is updated to iLimit.
    2) When pWal is null, the call is a no-op (no crash; no exception).
  - The test uses a lightweight, non-terminating assertion style to allow
    continued execution after a failure (print-based reporting).

  How to compile (example):
  g++ -std=c++11 -I. test_wal_limit.cpp wal.c -D SQLITE_OMIT_WAL=0 -o test_wal_limit
  Adjust include paths and compilation flags as appropriate for your environment.
*/

#include <Windows.h>
#include <iostream>
#include <wal.h>
#include <cstdint>


// Include the project's WAL interface. This header must define the Wal type
// and expose sqlite3WalLimit. The code below assumes i64 maps to long long.
// If your project uses a different type alias, adapt accordingly.
extern "C" {
// Declaration of the focal function from wal.c (C linkage)
void sqlite3WalLimit(Wal *pWal, long long iLimit);
}

// Lightweight non-terminating assertion helper
static bool expect_eq_ll(long long a, long long b, const char* msg) {
    if (a == b) {
        std::cout << "[OK] " << msg << "\n";
        return true;
    } else {
        std::cerr << "[FAIL] " << msg
                  << " | Expected: " << b
                  << ", Got: " << a << "\n";
        return false;
    }
}

// Test 1: Non-null pointer should update mxWalSize to the provided limit
// Purpose: Verify the true branch of the predicate (pWal != NULL).
static void test_sqlite3WalLimit_sets_mxWalSize_on_non_null() {
    // Arrange
    Wal w;
    // Initialize with a known value; the test only checks that it changes to iLimit
    w.mxWalSize = 0;

    // Act
    long long newLimit = 123456789LL;
    sqlite3WalLimit(&w, newLimit);

    // Assert
    expect_eq_ll(w.mxWalSize, newLimit,
                 "sqlite3WalLimit should set mxWalSize when pWal is non-null");
}

// Test 2: Null pointer should be a no-op (no crash, no side effects)
static void test_sqlite3WalLimit_no_op_on_null() {
    // Arrange: nothing to arrange

    // Act: calling with a null pointer should not crash
    sqlite3WalLimit(nullptr, 987654321LL);

    // Assert: since there is no observable state to check, we simply report success.
    // If a crash or exception occurs, the program would terminate before reaching this point.
    std::cout << "[OK] sqlite3WalLimit should be a no-op when pWal is NULL\n";
}

// Test 3: Repeated updates change the value consistently
static void test_sqlite3WalLimit_updates_consistently() {
    Wal w;
    w.mxWalSize = 42LL;

    long long first = 1000LL;
    sqlite3WalLimit(&w, first);
    bool ok1 = expect_eq_ll(w.mxWalSize, first, "First update of mxWalSize should succeed");

    long long second = 5555LL;
    sqlite3WalLimit(&w, second);
    bool ok2 = expect_eq_ll(w.mxWalSize, second, "Second update of mxWalSize should succeed");

    if (ok1 && ok2) {
        std::cout << "[OK] sqlite3WalLimit updates mxWalSize consistently on multiple calls\n";
    } else {
        std::cerr << "[WARN] sqlite3WalLimit inconsistencies detected in repeated updates\n";
    }
}

// Entry point
int main() {
    std::cout << "Running sqlite3WalLimit unit tests (C++11 harness)\n";

    test_sqlite3WalLimit_sets_mxWalSize_on_non_null();
    test_sqlite3WalLimit_no_op_on_null();
    test_sqlite3WalLimit_updates_consistently();

    std::cout << "Test run completed.\n";
    return 0;
}
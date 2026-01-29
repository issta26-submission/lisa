// Test suite for sqlite3WalEndReadTransaction
// This file provides a lightweight C++11 test harness (no GTest) that focuses on
// exercising the focal method sqlite3WalEndReadTransaction(Wal *pWal).
// The tests rely on the real WAL structures/types exposed by wal.h.
// Note: Static/internal helpers (like walUnlockShared) are not observable,
// so we verify the observable state change of pWal->readLock as the primary signal.
// The true/false branches of the internal condition (pWal->readLock >= 0) are covered
// by initializing readLock to non-negative and negative values respectively.

#include <Windows.h>
#include <cstring>
#include <iostream>
#include <wal.h>
#include <cstdint>


// Include C headers with C linkage
extern "C" {
}

// Simple test harness primitives
static void logCase(const char* name, bool pass) {
    std::cout << (pass ? "[PASS] " : "[FAIL] ") << name << std::endl;
}

// Test 1: When readLock is negative, EndReadTransaction should not attempt to unlock
//         and should not modify readLock via the conditional branch.
static bool test_EndReadReadTransaction_ReadLockNegative_NoBranch() {
    // Prepare a Wal instance with readLock set to a negative value
    Wal w;
    std::memset(&w, 0, sizeof(Wal));
    w.readLock = -1;  // negative: the if condition should be false

    // Call the function under test
    sqlite3WalEndReadTransaction(&w);

    // The only observable state in this test is readLock (should remain -1)
    if (w.readLock != -1) {
        return false;
    }

    // If we reach here, the branch was not entered and readLock preserved.
    return true;
}

// Test 2: When readLock is non-negative, EndReadTransaction should set readLock to -1.
//         We cannot observe walUnlockShared directly because it is a static/internal
//         helper, but we can verify the observable side-effect on readLock.
static bool test_EndReadReadTransaction_ReadLockNonNegative_SetsToMinusOne() {
    Wal w;
    std::memset(&w, 0, sizeof(Wal));

    // Non-negative value to trigger the branch
    w.readLock = 0;  // edge case: WAL_READ_LOCK(pWal->readLock) would be 3

    sqlite3WalEndReadTransaction(&w);

    if (w.readLock != -1) {
        return false;
    }
    return true;
}

// Test 3: Non-zero non-negative readLock should also be cleared to -1.
// This validates non-linear inputs and ensures consistency across values.
static bool test_EndReadReadTransaction_ReadLockNonZeroCleared() {
    Wal w;
    std::memset(&w, 0, sizeof(Wal));

    w.readLock = 5;  // arbitrary non-negative value
    sqlite3WalEndReadTransaction(&w);

    if (w.readLock != -1) {
        return false;
    }
    return true;
}

// Simple test runner
int main() {
    int total = 0;
    int passed = 0;

    struct Test {
        const char* name;
        bool (*fn)();
    };

    Test tests[] = {
        {"EndReadTransaction with negative readLock (branch not taken)", test_EndReadReadTransaction_ReadLockNegative_NoBranch},
        {"EndReadTransaction with non-negative readLock (branch taken, readLock=-1)", test_EndReadReadTransaction_ReadLockNonNegative_SetsToMinusOne},
        {"EndReadTransaction with non-zero readLock (cleared to -1)", test_EndReadReadTransaction_ReadLockNonZeroCleared},
    };

    const int nTests = sizeof(tests)/sizeof(tests[0]);
    for (int i = 0; i < nTests; ++i) {
        total++;
        bool ok = tests[i].fn();
        if (ok) passed++;
        logCase(tests[i].name, ok);
    }

    std::cout << "Test summary: " << passed << "/" << total << " passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
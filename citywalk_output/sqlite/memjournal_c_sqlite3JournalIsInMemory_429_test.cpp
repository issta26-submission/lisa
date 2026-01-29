// TestSuite for sqlite3JournalIsInMemory
// Objective: verify the behavior of sqlite3JournalIsInMemory in memjournal.c
// - true branch: when p->pMethods == &MemJournalMethods
// - false branch: when p->pMethods != &MemJournalMethods
//
// Notes:
// - This test relies on the project exposing MemJournalMethods with external linkage.
// - We use the project's sqliteInt.h definitions to construct sqlite3_file.
// - We avoid terminating assertions; failures are printed and counted.
// - The tests are written as a lightweight harness (no GTest is used).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Import required SQLite internal types and the focal function.
// The project is assumed to provide sqliteInt.h and the function sqlite3JournalIsInMemory.
// Use C linkage for C-style SQLite symbols.
extern "C" {
}

// The MemJournalMethods symbol is used by sqlite3JournalIsInMemory to compare against.
// We declare it with C linkage to match the project's expectations.
extern sqlite3_io_methods MemJournalMethods;

// Prototypes for the focal function (C linkage)
extern "C" int sqlite3JournalIsInMemory(sqlite3_file *p);

// Lightweight test harness
int main() {
    int failures = 0;

    // Test 1: True branch
    // Criteria: p->pMethods points exactly to MemJournalMethods -> sqlite3JournalIsInMemory should return non-zero.
    {
        sqlite3_file f;
        std::memset(&f, 0, sizeof(f)); // Zero-initialize to a clean state
        f.pMethods = &MemJournalMethods; // Point to the known MemJournalMethods symbol

        int result = sqlite3JournalIsInMemory(&f);
        if (result != 0) {
            std::cout << "[PASS] sqlite3JournalIsInMemory returns true when pMethods == &MemJournalMethods\n";
        } else {
            std::cerr << "[FAIL] sqlite3JournalIsInMemory should return non-zero when pMethods == &MemJournalMethods, but got 0\n";
            ++failures;
        }
    }

    // Test 2: False branch
    // Criteria: p->pMethods does not point to MemJournalMethods -> sqlite3JournalIsInMemory should return 0.
    {
        sqlite3_file f;
        std::memset(&f, 0, sizeof(f)); // Zero-initialize

        // Create a distinct dummy methods object to ensure address != &MemJournalMethods
        sqlite3_io_methods dummyMethods;
        std::memset(&dummyMethods, 0, sizeof(dummyMethods));
        f.pMethods = &dummyMethods; // Different address than MemJournalMethods

        int result = sqlite3JournalIsInMemory(&f);
        if (result == 0) {
            std::cout << "[PASS] sqlite3JournalIsInMemory returns false when pMethods != &MemJournalMethods\n";
        } else {
            std::cerr << "[FAIL] sqlite3JournalIsInMemory should return 0 when pMethods != &MemJournalMethods, but got " << result << "\n";
            ++failures;
        }
    }

    // Optional extension: test that null pMethods yields false (defensive check)
    // Note: The function dereferences p; ensure we do not dereference a null in this test.
    {
        sqlite3_file f;
        std::memset(&f, 0, sizeof(f));
        f.pMethods = nullptr;

        int result = sqlite3JournalIsInMemory(&f);
        if (result == 0) {
            std::cout << "[PASS] sqlite3JournalIsInMemory returns false when pMethods is nullptr\n";
        } else {
            std::cerr << "[FAIL] sqlite3JournalIsInMemory should return 0 when pMethods is nullptr, but got " << result << "\n";
            ++failures;
        }
    }

    // Summary
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "TOTAL FAILURES: " << failures << "\n";
        return 1;
    }
}
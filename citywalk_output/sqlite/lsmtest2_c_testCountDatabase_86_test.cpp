// test_lsmtest2_count_database.cpp
// A lightweight C++11 test harness for the focal function:
// int testCountDatabase(TestDb *pDb) from the provided lsmtest2.c
//
// Purpose:
// - Provide a minimal, executable test suite without using GTest.
// - Exercise the behavior of testCountDatabase by passing controlled
//   TestDb instances and observing the returned nRow value.
// - Because the internal scan callback (scanCountDb) is a no-op, the
//   function is expected to return 0 for any valid database state.
// - Tests are written to be non-terminating on failure (they report
//   failures and continue where possible), so we can exercise multiple cases.
//
// Notes:
// - This test relies on the public C linkage of testCountDatabase.
// - TestDb is assumed to be defined in the included C headers (lsmtest.h).
// - We allocate raw memory for TestDb and zero/partially initialize it to
//   simulate different DB states. This approach avoids depending on private
//   constructors or specific DB-building helpers.
// - The code does not depend on GoogleTest or other testing frameworks.

#include <iostream>
#include <cstdlib>
#include <lsmtest.h>
#include <cstring>


// Include the test framework header that defines TestDb and related items.
// The actual definitions are provided by the project under test.

// The focal function is defined in C; declare with C linkage for correct linking.
extern "C" int testCountDatabase(TestDb *pDb);

static bool test_zeroed_db() {
    // Test: Pass a zero-initialized, in-memory TestDb and expect nRow == 0.
    // Rationale: scanCountDb does not modify the Cksum in testCountDatabase,
    // so nRow should remain 0 after the scan.
    TestDb *pDb = reinterpret_cast<TestDb*>(std::malloc(sizeof(TestDb)));
    if (!pDb) {
        std::cerr << "[test_zeroed_db] Allocation failed for TestDb\n";
        return false;
    }

    std::memset(pDb, 0, sizeof(TestDb));  // Zero-initialize to simulate empty DB

    int nRow = testCountDatabase(pDb);

    std::free(pDb);

    if (nRow != 0) {
        std::cerr << "[test_zeroed_db] Failure: expected nRow == 0, got " 
                  << nRow << "\n";
        return false;
    }

    // Passed
    return true;
}

static bool test_nonzero_db() {
    // Test: Pass a non-zero initial memory state for TestDb and expect
    // nRow still to be 0 (since scanCountDb is a no-op for counting rows).
    // This validates that the function does not falsely increment nRow
    // due to uninitialized/garbage memory layout in the pDb structure.
    TestDb *pDb = reinterpret_cast<TestDb*>(std::malloc(sizeof(TestDb)));
    if (!pDb) {
        std::cerr << "[test_nonzero_db] Allocation failed for TestDb\n";
        return false;
    }

    // Fill with a non-zero pattern to simulate a "dirty" DB state.
    unsigned char *bytes = reinterpret_cast<unsigned char*>(pDb);
    for (size_t i = 0; i < sizeof(TestDb); ++i) {
        bytes[i] = 0xAA;
    }

    int nRow = testCountDatabase(pDb);

    std::free(pDb);

    if (nRow != 0) {
        std::cerr << "[test_nonzero_db] Failure: expected nRow == 0, got " 
                  << nRow << "\n";
        return false;
    }

    // Passed
    return true;
}

int main() {
    int failures = 0;

    // Run test cases
    if (!test_zeroed_db()) {
        ++failures;
        std::cerr << "test_zeroed_db: FAILED\n";
    } else {
        std::cout << "test_zeroed_db: PASSED\n";
    }

    if (!test_nonzero_db()) {
        ++failures;
        std::cerr << "test_nonzero_db: FAILED\n";
    } else {
        std::cout << "test_nonzero_db: PASSED\n";
    }

    if (failures == 0) {
        std::cout << "All tests PASSED\n";
    } else {
        std::cerr << "Some tests FAILED: " << failures << "\n";
    }

    return failures;
}
// Test suite for the focal method: sqlite3BtreeIsReadonly
// Focus: verify correct boolean outcome based on BTS_READ_ONLY flag in Btree->pBt->btsFlags
// This test is designed to be compiled with a C/C++11 toolchain without GoogleTest.
// It relies on the real sqlite3BtreeIsReadonly function and related sqlite B-tree types
// provided by the project (via btreeInt.h and linked sqlite3 code).
//
// Key elements (Candidate Keywords) used to drive test design:
// - Btree, BtShared, btsFlags, BTS_READ_ONLY
// - sqlite3BtreeIsReadonly, bitwise AND, non-zero comparison
// - true/false branches, static dependencies (handled by headers in project)

#include <btreeInt.h>
#include <iostream>
#include <string>


// Import necessary C dependencies using C linkage.
// The project provides Btree, BtShared, BTS_READ_ONLY, and the function under test.
extern "C" {
}

// Expose the focal function (should already be defined in the linked sqlite3 codebase)
extern "C" int sqlite3BtreeIsReadonly(Btree *p);

static bool test_sqlite3BtreeIsReadonly_ReadOnlyFlagSet() {
    // Purpose: ensure that when BTS_READ_ONLY bit is set in pBt->btsFlags, the function returns non-zero (true)
    BtShared shared;
    // BTS_READ_ONLY is defined in the project headers; use it to set the flag precisely.
    shared.btsFlags = BTS_READ_ONLY;

    Btree tree;
    tree.pBt = &shared;

    int result = sqlite3BtreeIsReadonly(&tree);

    // Expect non-zero (true)
    if (result != 0) {
        std::cout << "PASS: sqlite3BtreeIsReadonly returned true when BTS_READ_ONLY is set.\n";
        return true;
    } else {
        std::cerr << "FAIL: sqlite3BtreeIsReadonly returned false despite BTS_READ_ONLY being set.\n";
        return false;
    }
}

static bool test_sqlite3BtreeIsReadonly_ReadOnlyFlagNotSet() {
    // Purpose: ensure that when BTS_READ_ONLY bit is NOT set in pBt->btsFlags, the function returns zero (false)
    BtShared shared;
    // Clear the read-only flag; set other bits to ensure correctness of masking logic
    shared.btsFlags = 0; // explicitly no BTS_READ_ONLY

    Btree tree;
    tree.pBt = &shared;

    int result = sqlite3BtreeIsReadonly(&tree);

    // Expect zero (false)
    if (result == 0) {
        std::cout << "PASS: sqlite3BtreeIsReadonly returned false when BTS_READ_ONLY is not set.\n";
        return true;
    } else {
        std::cerr << "FAIL: sqlite3BtreeIsReadonly returned true despite BTS_READ_ONLY being unset.\n";
        return false;
    }
}

static bool test_sqlite3BtreeIsReadonly_ReadOnlyFlagSetWithOtherBits() {
    // Additional coverage: ensure that presence of other unrelated bits does not affect the outcome
    BtShared shared;
    // Set BTS_READ_ONLY along with other arbitrary bits
    shared.btsFlags = BTS_READ_ONLY | 0xAA; // 0xAA is an arbitrary non-zero mask

    Btree tree;
    tree.pBt = &shared;

    int result = sqlite3BtreeIsReadonly(&tree);

    // Expect non-zero (true) since BTS_READ_ONLY is present
    if (result != 0) {
        std::cout << "PASS: sqlite3BtreeIsReadonly correctly returns true even with other bits set.\n";
        return true;
    } else {
        std::cerr << "FAIL: sqlite3BtreeIsReadonly incorrectly returns false when BTS_READ_ONLY is set with other bits.\n";
        return false;
    }
}

int main() {
    // Run all tests and aggregate results
    int failures = 0;

    // Step 2: Comprehensive test suite for the focal method
    // Branch coverage: true branch (READ_ONLY set) and false branch (READ_ONLY not set)
    if (!test_sqlite3BtreeIsReadonly_ReadOnlyFlagSet()) ++failures;
    if (!test_sqlite3BtreeIsReadonly_ReadOnlyFlagNotSet()) ++failures;
    // Additional coverage for mixed flags
    if (!test_sqlite3BtreeIsReadonly_ReadOnlyFlagSetWithOtherBits()) ++failures;

    if (failures == 0) {
        std::cout << "All tests PASSED for sqlite3BtreeIsReadonly.\n";
        return 0;
    } else {
        std::cerr << "Tests FAILED: " << failures << " test(s) failed for sqlite3BtreeIsReadonly.\n";
        return 1;
    }
}
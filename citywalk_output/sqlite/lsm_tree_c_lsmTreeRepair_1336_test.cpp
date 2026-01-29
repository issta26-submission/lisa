// lsm_tree_test.cpp
// Lightweight unit tests for lsmTreeRepair (no gtest, no gmock, C-style API via C linkage).
// This test suite focuses on validating the behavior of lsmTreeRepair with respect to
// the header synchronization logic and restoration of the tree header.
// The tests are written to be compiled and linked against the provided lsm_tree.c
// and lsmInt.h in a C++11 compiler environment.

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <string.h>


// Bring C declarations into C++ test code.
extern "C" {
int lsmTreeRepair(lsm_db *db);
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

static void log_test_pass(const char* testName) {
    std::cerr << "[PASS] " << testName << "\n";
    ++g_tests_passed;
}
static void log_test_fail(const char* testName, const char* reason) {
    std::cerr << "[FAIL] " << testName << " - " << reason << "\n";
    ++g_tests_failed;
}

// Utility to print a short summary at the end
static void print_summary() {
    std::cerr << "\nTest Summary: " << g_tests_passed << " passed, "
              << g_tests_failed << " failed.\n";
}

// Helper to initialize a TreeHeader with zero bytes
static void zero_initialize_tree_header(TreeHeader *hdr) {
    std::memset(hdr, 0, sizeof(TreeHeader));
}

// Helper to initialize a TreeHeader with non-zero bytes
static void nonzero_initialize_tree_header(TreeHeader *hdr) {
    std::memset(hdr, 0xFF, sizeof(TreeHeader));
}

// Test 1:
// Scenario: hdr1 and hdr2 are different, forcing the memcmp branch to execute.
// We cannot directly control treeHeaderChecksumOk's internal logic here,
// but we can observe that after lsmTreeRepair, hdr1 and hdr2 become identical
// (one header copied over the other depending on checksum validity).
static bool test_hdr_swap_when_headers_are_different() {
    // Arrange
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    ShmHeader shm;
    // hdr1 = zeros, hdr2 = non-zeros => different headers
    zero_initialize_tree_header(&shm.hdr1);
    nonzero_initialize_tree_header(&shm.hdr2);

    db.pShmhdr = &shm;

    // Save original treehdr to verify restoration side effects (not strictly required here)
    TreeHeader originalTreeHdr;
    zero_initialize_tree_header(&originalTreeHdr);
    std::memcpy(&db.treehdr, &originalTreeHdr, sizeof(TreeHeader));

    // Act
    int rc = lsmTreeRepair(&db);

    // Assert
    // After the function, hdr1 and hdr2 should be identical due to the
    // memcmp check and subsequent copy operation.
    bool identical = (std::memcmp(&shm.hdr1, &shm.hdr2, sizeof(TreeHeader)) == 0);
    if (identical) {
        log_test_pass("test_hdr_swap_when_headers_are_different");
        return true;
    } else {
        log_test_fail("test_hdr_swap_when_headers_are_different",
                      "hdr1 and hdr2 remain different after lsmTreeRepair");
        return false;
    }
}

// Test 2:
// Scenario: hdr1 and hdr2 are already identical; ensure no modification occurs to headers.
static bool test_hdr_no_swap_when_headers_are_same() {
    // Arrange
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    ShmHeader shm;
    // hdr1 and hdr2 identical
    TreeHeader identicalHdr;
    zero_initialize_tree_header(&identicalHdr);
    std::memcpy(&shm.hdr1, &identicalHdr, sizeof(TreeHeader));
    std::memcpy(&shm.hdr2, &identicalHdr, sizeof(TreeHeader));

    db.pShmhdr = &shm;

    // Save a copy of the header to ensure no unintended mutation happens
    TreeHeader beforeHdr1, beforeHdr2;
    std::memcpy(&beforeHdr1, &shm.hdr1, sizeof(TreeHeader));
    std::memcpy(&beforeHdr2, &shm.hdr2, sizeof(TreeHeader));

    // Act
    int rc = lsmTreeRepair(&db);

    // Assert
    bool still_identical = (std::memcmp(&shm.hdr1, &shm.hdr2, sizeof(TreeHeader)) == 0);
    bool unchanged = (std::memcmp(&shm.hdr1, &beforeHdr1, sizeof(TreeHeader)) == 0) &&
                     (std::memcmp(&shm.hdr2, &beforeHdr2, sizeof(TreeHeader)) == 0);
    if (still_identical && unchanged) {
        log_test_pass("test_hdr_no_swap_when_headers_are_same");
        return true;
    } else {
        log_test_fail("test_hdr_no_swap_when_headers_are_same",
                      "Headers modified when they were previously equal");
        return false;
    }
}

// Test 3:
// Scenario: Verify that the tree header stored in db.treehdr is restored after lsmTreeRepair.
// This ensures the post-call state matches the pre-call state regardless of repair outcomes.
static bool test_treehdr_restoration_idempotent() {
    // Arrange
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    // Setup a dummy shm with two headers that are equal (no header reconciliation occurs)
    ShmHeader shm;
    TreeHeader hdrInit;
    zero_initialize_tree_header(&hdrInit);
    std::memcpy(&shm.hdr1, &hdrInit, sizeof(TreeHeader));
    std::memcpy(&shm.hdr2, &hdrInit, sizeof(TreeHeader));
    db.pShmhdr = &shm;

    // Capture original treehdr
    TreeHeader originalTreeHdr;
    zero_initialize_tree_header(&originalTreeHdr);
    std::memcpy(&db.treehdr, &originalTreeHdr, sizeof(TreeHeader));

    // Act
    int rc = lsmTreeRepair(&db);

    // Assert
    bool restored = (std::memcmp(&db.treehdr, &originalTreeHdr, sizeof(TreeHeader)) == 0);
    if (restored) {
        log_test_pass("test_treehdr_restoration_idempotent");
        return true;
    } else {
        log_test_fail("test_treehdr_restoration_idempotent",
                      "db.treehdr was not restored to its original value");
        return false;
    }
}

// Main driver
int main() {
    bool all_passed = true;

    all_passed &= test_hdr_swap_when_headers_are_different();
    all_passed &= test_hdr_no_swap_when_headers_are_same();
    all_passed &= test_treehdr_restoration_idempotent();

    print_summary();

    // Return non-zero if any test failed
    if (!all_passed) {
        return 1;
    }
    return 0;
}
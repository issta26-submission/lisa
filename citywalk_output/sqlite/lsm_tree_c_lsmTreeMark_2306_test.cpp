// Test suite for the focal method: lsmTreeMark in lsm_tree.c
// This test relies on the project's public definitions (lsmInt.h) and the
// provided focal method. It uses a lightweight, non-GTest C++ test harness.

#include <lsmInt.h>
#include <iostream>
#include <string.h>
#include <cassert>


// Include the project's public C headers.
// We rely on the project to provide correct type definitions for lsm_db,
// TreeMark, etc. The lsmTreeMark function is defined in lsm_tree.c and has C linkage.
#ifdef __cplusplus
extern "C" {
#endif

void lsmTreeMark(lsm_db *pDb, TreeMark *pMark);

#ifdef __cplusplus
}
#endif

// Lightweight test framework (non-terminating style is emulated by collecting
// results and printing a summary at the end).
struct TestResult {
    const char* name;
    bool passed;
    std::string details;
};

static std::vector<TestResult> g_results;

// Helper to register a test result
static void RecordResult(const char* name, bool passed, const std::string& details = "") {
    g_results.push_back({name, passed, details});
}

// Test 1: Verify that lsmTreeMark copies core fields from lsm_db to TreeMark.
// We zero-initialize db to keep behavior deterministic, then set a known
// set of fields to verify exact copying.
// Expected: mark fields should match db's corresponding fields and iRollback should
// reflect intArraySize(&db.rollback). With a zero-initialized rollback, iRollback should be 0.
static void test_basic_field_copy() {
    // Arrange
    lsm_db db = {};            // Zero-initialized to have a deterministic iRollback (likely 0)
    TreeMark mark = {};          // Output structure to be populated by lsmTreeMark

    // Act: set a known state on db's treehdr
    db.treehdr.root.iRoot = 42;
    db.treehdr.root.nHeight = 7;
    db.treehdr.iWrite = 123;
    db.treehdr.nChunk = 456;
    db.treehdr.iNextShmid = 789;

    // Call the function under test
    lsmTreeMark(&db, &mark);

    // Assert: verify all fields were copied exactly
    bool ok = true;
    if (mark.iRoot != db.treehdr.root.iRoot) ok = false;
    if (mark.nHeight != db.treehdr.root.nHeight) ok = false;
    if (mark.iWrite != db.treehdr.iWrite) ok = false;
    if (mark.nChunk != db.treehdr.nChunk) ok = false;
    if (mark.iNextShmid != db.treehdr.iNextShmid) ok = false;
    // iRollback is derived from intArraySize(&db.rollback); since rollback was zeroed,
    // we expect 0. If the internal implementation uses a different default, this may vary,
    // but in typical scenarios with zero-initialized structs, size should be 0.
    if (mark.iRollback != 0) ok = false;

    if (!ok) {
        std::string details = "Expected: iRoot=" + std::to_string(db.treehdr.root.iRoot) +
                              ", nHeight=" + std::to_string(db.treehdr.root.nHeight) +
                              ", iWrite=" + std::to_string(db.treehdr.iWrite) +
                              ", nChunk=" + std::to_string(db.treehdr.nChunk) +
                              ", iNextShmid=" + std::to_string(db.treehdr.iNextShmid) +
                              ", iRollback=0; "
                              "Actual: iRoot=" + std::to_string(mark.iRoot) +
                              ", nHeight=" + std::to_string(mark.nHeight) +
                              ", iWrite=" + std::to_string(mark.iWrite) +
                              ", nChunk=" + std::to_string(mark.nChunk) +
                              ", iNextShmid=" + std::to_string(mark.iNextShmid) +
                              ", iRollback=" + std::to_string(mark.iRollback);
        RecordResult("test_basic_field_copy", false, details);
    } else {
        RecordResult("test_basic_field_copy", true);
    }
}

// Test 2: Verify behavior when db fields are set to alternative values.
// This ensures that changes in db are correctly reflected in mark, without
// relying on previous test state.
static void test_alternate_values_copy() {
    // Arrange
    lsm_db db = {};
    TreeMark mark = {};

    db.treehdr.root.iRoot = -7;
    db.treehdr.root.nHeight = 0;
    db.treehdr.iWrite = -123;
    db.treehdr.nChunk = 999;
    db.treehdr.iNextShmid = -50;

    // Act
    lsmTreeMark(&db, &mark);

    // Assert
    bool ok = true;
    if (mark.iRoot != db.treehdr.root.iRoot) ok = false;
    if (mark.nHeight != db.treehdr.root.nHeight) ok = false;
    if (mark.iWrite != db.treehdr.iWrite) ok = false;
    if (mark.nChunk != db.treehdr.nChunk) ok = false;
    if (mark.iNextShmid != db.treehdr.iNextShmid) ok = false;
    if (mark.iRollback != 0) ok = false; // rollback is still zero-initialized

    if (!ok) {
        std::string details = "Alternate values mismatch: "
                              "mark(iRoot)=" + std::to_string(mark.iRoot) +
                              ",(mark.nHeight)=" + std::to_string(mark.nHeight) +
                              ",(mark.iWrite)=" + std::to_string(mark.iWrite) +
                              ",(mark.nChunk)=" + std::to_string(mark.nChunk) +
                              ",(mark.iNextShmid)=" + std::to_string(mark.iNextShmid) +
                              ",(mark.iRollback)=" + std::to_string(mark.iRollback);
        RecordResult("test_alternate_values_copy", false, details);
    } else {
        RecordResult("test_alternate_values_copy", true);
    }
}

int main() {
    // Run tests
    test_basic_field_copy();
    test_alternate_values_copy();

    // Report results
    int passed = 0;
    int failed = 0;
    for (const auto& r : g_results) {
        if (r.passed) {
            std::cout << "[PASS] " << r.name << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << r.name << " - " << r.details << "\n";
            ++failed;
        }
    }

    if (failed == 0) {
        std::cout << "ALL TESTS PASSED (" << passed << " tests)" << std::endl;
        return 0;
    } else {
        std::cout << failed << " TEST(S) FAILED (" << passed << " passed)" << std::endl;
        return 1;
    }
}
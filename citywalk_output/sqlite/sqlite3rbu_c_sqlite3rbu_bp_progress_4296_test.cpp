// Unit test suite for sqlite3rbu_bp_progress
// Target: validate behavior of the focal method with various stages and edge conditions.
// Constraints followed:
// - No GTest/GMock; a small in-house test harness is implemented.
// - Tests compile under C++11.
// - Use the actual sqlite3rbu.h types (extern "C" linkage for the C function).
// - Non-terminating assertions: tests continue after failures; a final summary is printed.

#include <string.h>
#include <windows.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <sqlite3rbu.h>
#include <iostream>


// Include the project header to get the real types/enums

#ifdef __cplusplus
extern "C" {
#endif
// Declare the focal function from the C source file
void sqlite3rbu_bp_progress(sqlite3rbu *p, int *pnOne, int *pnTwo);
#ifdef __cplusplus
}
#endif

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_test_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[OK]   " << test_name << "\n";
    } else {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: OAL stage with positive phase-one progress.
// Expected: pnOne = MAX_PROGRESS * nProgress / nPhaseOneStep; pnTwo = 0
static void test_bp_progress_oal_positive() {
    // Arrange
    sqlite3rbu p;
    std::memset(&p, 0, sizeof(p));

    p.eStage = RBU_STAGE_OAL;
    p.nPhaseOneStep = 1000;
    p.nProgress = 500;

    int pnOne = 0;
    int pnTwo = 0;

    // Act
    sqlite3rbu_bp_progress(&p, &pnOne, &pnTwo);

    // Assert
    int expectedOne = 10000 * (int64_t)p.nProgress / (int64_t)p.nPhaseOneStep; // 5000
    bool ok = (pnOne == expectedOne) && (pnTwo == 0);
    log_test_result("OAL: positive progress calculation", ok);
}

// Test 2: OAL stage with non-positive phase-one steps (0) should yield -1 for pnOne; pnTwo remains 0.
static void test_bp_progress_oal_zero_phaseone() {
    // Arrange
    sqlite3rbu p;
    std::memset(&p, 0, sizeof(p));

    p.eStage = RBU_STAGE_OAL;
    p.nPhaseOneStep = 0;
    p.nProgress = 123; // any positive value

    int pnOne = 0;
    int pnTwo = 0;

    // Act
    sqlite3rbu_bp_progress(&p, &pnOne, &pnTwo);

    // Assert
    bool ok = (pnOne == -1) && (pnTwo == 0);
    log_test_result("OAL: zero phase-one => pnOne == -1, pnTwo == 0", ok);
}

// Test 3: MOVE stage should yield MAX_PROGRESS for pnOne and 0 for pnTwo.
static void test_bp_progress_move_stage() {
    // Arrange
    sqlite3rbu p;
    std::memset(&p, 0, sizeof(p));

    p.eStage = RBU_STAGE_MOVE;
    // Other fields are irrelevant for MOVE, but keep them zeroed.

    int pnOne = 0;
    int pnTwo = 0;

    // Act
    sqlite3rbu_bp_progress(&p, &pnOne, &pnTwo);

    // Assert
    bool ok = (pnOne == 10000) && (pnTwo == 0);
    log_test_result("MOVE: pnOne == 10000, pnTwo == 0", ok);
}

// Test 4: CKPT stage: pnOne = MAX_PROGRESS; pnTwo = MAX_PROGRESS * nStep / nFrame
static void test_bp_progress_ckpt_stage() {
    // Arrange
    sqlite3rbu p;
    std::memset(&p, 0, sizeof(p));

    p.eStage = RBU_STAGE_CKPT;
    p.nStep  = 2500;
    p.nFrame = 5000;

    int pnOne = 0;
    int pnTwo = 0;

    // Act
    sqlite3rbu_bp_progress(&p, &pnOne, &pnTwo);

    int expectedTwo = 10000 * (int64_t)p.nStep / (int64_t)p.nFrame; // 5000
    bool ok = (pnOne == 10000) && (pnTwo == expectedTwo);
    log_test_result("CKPT: pnOne == 10000, pnTwo == 5000", ok);
}

// Test 5: DONE stage: both pnOne and pnTwo should be MAX_PROGRESS
static void test_bp_progress_done_stage() {
    // Arrange
    sqlite3rbu p;
    std::memset(&p, 0, sizeof(p));

    p.eStage = RBU_STAGE_DONE;

    int pnOne = 0;
    int pnTwo = 0;

    // Act
    sqlite3rbu_bp_progress(&p, &pnOne, &pnTwo);

    // Assert
    bool ok = (pnOne == 10000) && (pnTwo == 10000);
    log_test_result("DONE: pnOne == 10000, pnTwo == 10000", ok);
}

// Entry point for tests
int main() {
    // Run all test cases
    test_bp_progress_oal_positive();
    test_bp_progress_oal_zero_phaseone();
    test_bp_progress_move_stage();
    test_bp_progress_ckpt_stage();
    test_bp_progress_done_stage();

    // Summary
    std::cout << "\nTest Summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failures.\n";

    // Return non-zero if any test failed to signal issues to the caller
    return (g_failed_tests > 0) ? 1 : 0;
}
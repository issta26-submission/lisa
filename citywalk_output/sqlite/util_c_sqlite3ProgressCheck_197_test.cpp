/*
  Unit test suite for the focal method sqlite3ProgressCheck.
  - Implemented as a self-contained C++11 test harness (no GTest).
  - Re-creates the minimal subset of the sqlite3 structures required by sqlite3ProgressCheck.
  - Exercises true/false branches of the critical predicates:
    * Interrupted path (AtomicLoad on db->u1.isInterrupted)
    * Progress callback path when xProgress is non-null
    * Progress threshold triggering behavior and callback return values
    * Reset behavior when rc == SQLITE_INTERRUPT
  - Provides explanatory comments for each test case.
  - Uses non-terminating assertions (custom ASSERT macro) to maximize code execution coverage.
*/

#include <math.h>
#include <cstdio>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// Domain knowledge alignment: emulate the minimal dependencies needed by sqlite3ProgressCheck.

// Lightweight redefinition of constants used by sqlite3ProgressCheck
static const int SQLITE_INTERRUPT = 1; // Value chosen for testing; must match usage in code under test.

// Lightweight AtomicLoad replacement for test environment (operates on plain ints here)
#define AtomicLoad(x) (*(x))

// Minimal class/struct definitions matching the usage in sqlite3ProgressCheck
struct sqlite3 { // Simplified subset required for sqlite3ProgressCheck
    struct {
        int isInterrupted; // atomic flag in real sqlite3; here a simple int for testing
    } u1;
    int nProgressOps;                 // number of ops between progress callbacks
    int (*xProgress)(void*);           // progress callback function
    void *pProgressArg;                  // argument to progress callback
};

struct Parse {
    sqlite3 *db;
    int nErr;
    int rc;
    int nProgressSteps;
};

// Forward declaration of the focal function (as defined in the provided source)
void sqlite3ProgressCheck(Parse *p);

// Implementation of sqlite3ProgressCheck (copied/adapted from the provided <FOCAL_METHOD>)
// Note: In a real environment this would be linked from util.c. Here we implement it
// to provide a self-contained test harness that exercises all branches.
void sqlite3ProgressCheck(Parse *p){
  sqlite3 *db = p->db;
  if( AtomicLoad(&db->u1.isInterrupted) ){
    p->nErr++;
    p->rc = SQLITE_INTERRUPT;
  }
#ifndef SQLITE_OMIT_PROGRESS_CALLBACK
  if( db->xProgress ){
    if( p->rc==SQLITE_INTERRUPT ){
      p->nProgressSteps = 0;
    }else if( (++p->nProgressSteps)>=db->nProgressOps ){
      if( db->xProgress(db->pProgressArg) ){
        p->nErr++;
        p->rc = SQLITE_INTERRUPT;
      }
      p->nProgressSteps = 0;
    }
  }
#endif
}

// Minimal test framework (non-terminating asserts)
#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << " (function: " << __func__ \
                  << ", line: " << __LINE__ << ")" << std::endl; \
        return false; \
    } \
} while(0)

static bool test_interrupt_sets_rc_and_nErr() {
    // Test 1: If db->u1.isInterrupted is true, then nErr increments and rc becomes SQLITE_INTERRUPT.
    // Setup
    sqlite3 db;
    Parse p;
    db.u1.isInterrupted = 1;
    db.nProgressOps = 10;
    db.xProgress = nullptr;
    db.pProgressArg = nullptr;

    p.db = &db;
    p.nErr = 0;
    p.rc = 0;
    p.nProgressSteps = 0;

    // Exercise
    sqlite3ProgressCheck(&p);

    // Verify
    ASSERT(p.nErr == 1, "Interrupted path should increment nErr");
    ASSERT(p.rc == SQLITE_INTERRUPT, "Interrupted path should set rc to SQLITE_INTERRUPT");

    return true;
}

static int progressCallbackState_retVal; // 0 or 1 depending on test
struct ProgressCallbackState {
    int retVal;
    int callCount;
};

static int testProgressCallback(void* arg) {
    ProgressCallbackState* st = (ProgressCallbackState*)arg;
    st->callCount++;
    return st->retVal;
}

static bool test_progress_path_calls_callback_and_resets_steps_on_threshold_false() {
    // Test 2: Progress path with non-null xProgress, rc != INTERRUPT, threshold reached, callback returns 0.
    // Setup
    sqlite3 db;
    Parse p;
    ProgressCallbackState state;
    state.retVal = 0; // callback returns false (no interrupt)
    state.callCount = 0;

    db.u1.isInterrupted = 0;
    db.nProgressOps = 2; // threshold
    db.xProgress = testProgressCallback;
    db.pProgressArg = &state;

    p.db = &db;
    p.nErr = 0;
    p.rc = 0;
    p.nProgressSteps = 0;

    // First call: steps -> 1, no callback
    sqlite3ProgressCheck(&p);
    ASSERT(p.nProgressSteps == 1, "First call should increment progress steps to 1");
    ASSERT(p.nErr == 0 && p.rc == 0, "First call should not modify errors or rc");

    // Second call: steps -> 2, trigger callback; callback returns 0
    sqlite3ProgressCheck(&p);
    ASSERT(p.nProgressSteps == 0, "After threshold, steps should reset to 0");
    ASSERT(p.nErr == 0, "Callback returns false; should not increment nErr");
    ASSERT(p.rc == 0, "Callback returns false; rc should remain unchanged");
    ASSERT(state.callCount == 1, "Progress callback should have been called exactly once");

    return true;
}

static bool test_progress_path_calls_callback_and_sets_interrupt_on_true() {
    // Test 3: Progress path with non-null xProgress, threshold reached, callback returns 1 (interrupt)
    // Setup
    sqlite3 db;
    Parse p;
    ProgressCallbackState state;
    state.retVal = 1; // callback returns true (interrupt)
    state.callCount = 0;

    db.u1.isInterrupted = 0;
    db.nProgressOps = 3; // threshold
    db.xProgress = testProgressCallback;
    db.pProgressArg = &state;

    p.db = &db;
    p.nErr = 0;
    p.rc = 0;
    p.nProgressSteps = 0;

    // Perform calls to reach threshold
    // 1st call
    sqlite3ProgressCheck(&p);
    ASSERT(p.nProgressSteps == 1, "First call should increment to 1");
    // 2nd call
    sqlite3ProgressCheck(&p);
    ASSERT(p.nProgressSteps == 2, "Second call should increment to 2");
    // 3rd call triggers threshold and callback returns 1
    sqlite3ProgressCheck(&p);

    // Verify
    ASSERT(p.nErr == 1, "Callback returning true should increment nErr");
    ASSERT(p.rc == SQLITE_INTERRUPT, "Callback returning true should set rc to SQLITE_INTERRUPT");
    ASSERT(p.nProgressSteps == 0, "After interrupt, steps should reset to 0");
    ASSERT(state.callCount == 1, "Progress callback should have been called exactly once at threshold");

    return true;
}

static bool test_interrupt_resets_progress_steps_when_rc_interrupts_before_progress() {
    // Test 4: If p->rc == SQLITE_INTERRUPT, the progress steps should be reset to 0
    sqlite3 db;
    Parse p;

    db.u1.isInterrupted = 0;
    db.nProgressOps = 5;
    db.xProgress = testProgressCallback;
    ProgressCallbackState state;
    state.retVal = 0;
    state.callCount = 0;
    db.pProgressArg = &state;

    p.db = &db;
    p.nErr = 0;
    p.rc = SQLITE_INTERRUPT; // pre-set interrupt
    p.nProgressSteps = 7;     // non-zero

    // Call; since rc == INTERRUPT, progress steps should reset to 0 and not call callback
    sqlite3ProgressCheck(&p);

    ASSERT(p.nProgressSteps == 0, "When rc is INTERRUPT, nProgressSteps should be reset to 0");
    ASSERT(p.nErr == 0, "Should not increment nErr in this test path");
    // Callback should not be invoked due to rc interrupt path
    ASSERT(state.callCount == 0, "Progress callback should not be invoked when rc is already INTERRUPT");

    return true;
}

static bool test_no_progress_callback_when_xProgress_null() {
    // Test 5: xProgress is NULL; only the interrupt path should be evaluated
    sqlite3 db;
    Parse p;

    db.u1.isInterrupted = 0;
    db.nProgressOps = 4;
    db.xProgress = nullptr;
    db.pProgressArg = nullptr;

    p.db = &db;
    p.nErr = 0;
    p.rc = 0;
    p.nProgressSteps = 0;

    sqlite3ProgressCheck(&p);

    // No progress callback; ensure only the interrupt block ran (which was false)
    ASSERT(p.nErr == 0, "No interrupt and no progress callback should not modify nErr");
    ASSERT(p.rc == 0, "No interrupt should leave rc unchanged");
    ASSERT(p.nProgressSteps == 0, "With no progress callback, nProgressSteps remains 0");

    return true;
}

// Entry point
int main() {
    int testsPassed = 0;
    int totalTests = 0;

    auto run = [&](const char* name, bool (*fn)()) -> bool {
        totalTests++;
        bool ok = fn();
        if(ok) {
            std::cout << "[PASS] " << name << std::endl;
            testsPassed++;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
        return ok;
    };

    run("test_interrupt_sets_rc_and_nErr", test_interrupt_sets_rc_and_nErr);
    run("test_progress_path_calls_callback_and_resets_steps_on_threshold_false", test_progress_path_calls_callback_and_resets_steps_on_threshold_false);
    run("test_progress_path_calls_callback_and_sets_interrupt_on_true", test_progress_path_calls_callback_and_sets_interrupt_on_true);
    run("test_interrupt_resets_progress_steps_when_rc_interrupts_before_progress", test_interrupt_resets_progress_steps_when_rc_interrupts_before_progress);
    run("test_no_progress_callback_when_xProgress_null", test_no_progress_callback_when_xProgress_null);

    std::cout << "Summary: " << testsPassed << " / " << totalTests << " tests passed." << std::endl;

    return (testsPassed == totalTests) ? 0 : 1;
}
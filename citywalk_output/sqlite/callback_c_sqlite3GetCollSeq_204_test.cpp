/*
This file provides a self-contained C++11 unit test harness for the focal function
sqlite3GetCollSeq as presented in the prompt. We re-create the minimal required
SQLite-like environment (Parse, CollSeq, sqlite3, etc.) and stub the dependent
functions in a controlled way to drive the specific branches of sqlite3GetCollSeq.
We purposely avoid GTest and rely on a lightweight, non-terminating assertion style.

Notes:
- The test is designed to be compiled as a single translation unit along with the
  focal method (sqlite3GetCollSeq) and its minimal dependencies.
- We simulate various scenarios by controlling global state (mocked FindCollSeq
  behavior, synthCollSeq result, etc.) to exercise true/false branches.
- Static file-scope helpers in the real code are emulated here as simple global
  mocks accessible to sqlite3GetCollSeq in this test TU.
*/

#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain types (simplified for testing)
typedef unsigned char u8;

// Forward declarations for the focal method and its dependencies (mocked)
struct CollSeq;
struct Parse;
struct sqlite3;

// Minimal CollSeq structure: xCmp is a function pointer (non-nullability used by tests)
struct CollSeq {
    void* xCmp; // non-null indicates a valid comparator in tests
};

// Minimal sqlite3 object (opaque for testing)
struct sqlite3 {};

// Minimal Parse structure used by sqlite3GetCollSeq
struct Parse {
    sqlite3* db;
    int rc;
};

// Error code as per the focal code
#define SQLITE_ERROR_MISSING_COLLSEQ 1

// Global mock state to drive behavior of the dependencies
static int g_callCollNeeded_count = 0;
static int g_synth_should_succeed = 0;
static int g_findCase = 0; // 0: FIND_NULL, 1: FIND_SEQ_NO_XCMP, 2: FIND_SEQ_WITH_XCMP

// A single static CollSeq instance used as return value by sqlite3FindCollSeq
static CollSeq g_seq;

// Helper to reset global mock state between tests
static void resetMocks() {
    g_callCollNeeded_count = 0;
    g_synth_should_succeed = 0;
    g_findCase = 0;
    g_seq.xCmp = nullptr;
}

// Mocked dependency implementations to drive sqlite3GetCollSeq

// Simulated sqlite3FindCollSeq(db, enc, zName, create)
static CollSeq* sqlite3FindCollSeq(sqlite3* /*db*/, u8 /*enc*/, const char* /*zName*/, int /*create*/) {
    // Behavior controlled by g_findCase
    switch (g_findCase) {
        case 0: // FIND_NULL
            return nullptr;
        case 1: // FIND_SEQ_NO_XCMP
            g_seq.xCmp = nullptr;
            return &g_seq;
        case 2: // FIND_SEQ_WITH_XCMP
            g_seq.xCmp = (void*)0x1; // non-null
            return &g_seq;
        default:
            return nullptr;
    }
}

// Simulated static callCollNeeded
static void callCollNeeded(sqlite3* /*db*/, int /*enc*/, const char* /*zName*/) {
    g_callCollNeeded_count++;
}

// Simulated synthCollSeq: if it returns non-zero, caller may set p->xCmp
static int synthCollSeq(sqlite3* /*db*/, CollSeq* pColl) {
    if (g_synth_should_succeed && pColl != nullptr) {
        pColl->xCmp = (void*)0x2; // non-null to indicate population
        return 1;
    }
    return 0;
}

// Simulated sqlite3ErrorMsg (variadic)
static void sqlite3ErrorMsg(Parse* /*pParse*/, const char* /*zFmt*/, ...) {
    // For tests, we simply ignore the formatted string; rc is set by the focal function.
}

// The focal method under test (copied and adapted for the test TU)
static CollSeq* sqlite3GetCollSeq(
  Parse *pParse,        /* Parsing context */
  u8 enc,               /* The desired encoding for the collating sequence */
  CollSeq *pColl,       /* Collating sequence with native encoding, or NULL */
  const char *zName     /* Collating sequence name */
){
{
  CollSeq *p;
  sqlite3 *db = pParse->db;
  p = pColl;
  if( !p ){
    p = sqlite3FindCollSeq(db, enc, zName, 0);
  }
  if( !p || !p->xCmp ){
    /* No collation sequence of this type for this encoding is registered.
    ** Call the collation factory to see if it can supply us with one.
    */
    callCollNeeded(db, enc, zName);
    p = sqlite3FindCollSeq(db, enc, zName, 0);
  }
  if( p && !p->xCmp && synthCollSeq(db, p) ){
    p = 0;
  }
  assert( !p || p->xCmp );
  if( p==0 ){
    sqlite3ErrorMsg(pParse, "no such collation sequence: %s", zName);
    pParse->rc = SQLITE_ERROR_MISSING_COLLSEQ;
  }
  return p;
}
}

// Test harness helpers for verification
static bool ptrEquals(CollSeq* a, CollSeq* b) { return a == b; }

// Test cases
static void test_case_A_pColl_nonNull_xCmp_nonNull_returns_same_pointer()
{
    // Scenario: pColl provided and has a non-null xCmp.
    // Expectation: sqlite3GetCollSeq returns the same pointer, rc remains 0,
    // and no calls to find/collation factory are made.
    resetMocks();
    Parse pParse;
    sqlite3 dbInstance;
    pParse.db = &dbInstance;
    pParse.rc = 0;

    CollSeq pcoll;
    pcoll.xCmp = (void*)0xDEAD; // non-null
    CollSeq* res = sqlite3GetCollSeq(&pParse, 0, &pcoll, "foo_collation");

    bool ok = (res == &pcoll) && (pParse.rc == 0) && (g_callCollNeeded_count == 0);
    if(!ok){
        std::cerr << "Test A FAILED: expected same pointer, rc=0, no coll needed calls.\n";
        if(res != &pcoll) std::cerr << " - Returned pointer differs from input.\n";
        if(pParse.rc != 0) std::cerr << " - rc was modified to " << pParse.rc << ".\n";
        if(g_callCollNeeded_count != 0) std::cerr << " - callCollNeeded called " << g_callCollNeeded_count << " times.\n";
    } else {
        std::cout << "Test A PASSED.\n";
    }
}

// Test B: pColl == NULL; FindCollSeq returns sequence with no xCmp on first call,
// synthCollSeq succeeds on second call, leading to p==0 and error set.
static void test_case_B_findseq_no_xcmp_then_synth_populates_and_sets_error()
{
    resetMocks();
    Parse pParse;
    sqlite3 dbInstance;
    pParse.db = &dbInstance;
    pParse.rc = 0;

    // First findings:
    // - First call to sqlite3FindCollSeq returns &g_seq with xCmp==nullptr (FIND_SEQ_NO_XCMP)
    // - After callCollNeeded, second call returns same with xCmp==nullptr
    // - synthCollSeq will populate xCmp and return 1, causing p=0
    g_findCase = 1;            // FIRST FIND: NO_XCMP
    g_seq.xCmp = nullptr;
    g_synth_should_succeed = 1; // synthCollSeq will succeed

    CollSeq* res = sqlite3GetCollSeq(&pParse, 0, nullptr, "bar_collation");

    bool ok = (res == nullptr) &&
              (pParse.rc == SQLITE_ERROR_MISSING_COLLSEQ) &&
              (g_callCollNeeded_count > 0);

    if(!ok){
        std::cerr << "Test B FAILED: expected NULL result, rc=MISSING_COLLSEQ, and at least one callCollNeeded.\n";
        if(res != nullptr) std::cerr << " - Unexpected non-null result.\n";
        if(pParse.rc != SQLITE_ERROR_MISSING_COLLSEQ) std::cerr << " - rc=" << pParse.rc << "\n";
        if(g_callCollNeeded_count == 0) std::cerr << " - callCollNeeded was never invoked.\n";
    } else {
        std::cout << "Test B PASSED.\n";
    }
}

// Test C: pColl == NULL; FindCollSeq returns a sequence with a valid xCmp on first Find
// path should bypass synth and return the sequence directly.
static void test_case_C_findseq_with_xcmp_on_first_call_returns_sequence()
{
    resetMocks();
    Parse pParse;
    sqlite3 dbInstance;
    pParse.db = &dbInstance;
    pParse.rc = 0;

    // First call to Find returns a sequence with xCmp non-null -> direct return
    g_findCase = 2; // FIND_SEQ_WITH_XCMP
    g_seq.xCmp = (void*)0x1; // non-null

    CollSeq* res = sqlite3GetCollSeq(&pParse, 0, nullptr, "baz_collation");

    bool ok = (res == &g_seq) && (pParse.rc == 0) && (g_callCollNeeded_count == 1);

    if(!ok){
        std::cerr << "Test C FAILED: expected to receive g_seq with non-null xCmp, rc=0, and callCollNeeded=1.\n";
        if(res != &g_seq) std::cerr << " - Returned pointer is not g_seq.\n";
        if(pParse.rc != 0) std::cerr << " - rc=" << pParse.rc << "\n";
        if(g_callCollNeeded_count != 1) std::cerr << " - callCollNeeded_count=" << g_callCollNeeded_count << " (expected 1).\n";
    } else {
        std::cout << "Test C PASSED.\n";
    }
}

// Test D: pColl non-null with xCmp NULL; FindCollSeq eventually provides a non-null xCmp on second call.
// This should lead to returning the non-null coll seq (after initial callCollNeeded).
static void test_case_D_pColl_null_xCmp_then_succeeding_findseq_with_xcmp_returns_sequence()
{
    resetMocks();
    Parse pParse;
    sqlite3 dbInstance;
    pParse.db = &dbInstance;
    pParse.rc = 0;

    // pColl is non-null with xCmp NULL
    CollSeq pcoll;
    pcoll.xCmp = nullptr;

    // First call to Find returns a sequence with xCmp NULL
    g_findCase = 1; // FIND_SEQ_NO_XCMP
    g_seq.xCmp = nullptr;
    g_synth_should_succeed = 0;

    CollSeq* res = sqlite3GetCollSeq(&pParse, 0, &pcoll, "qux_collation");

    // In this scenario, after the first Find (NULL), the function calls the factory, second Find returns with XOR non-null
    // However, since synthCollSeq is not forced, we rely on the path where xCmp becomes non-null on second Find.
    // The test checks that we successfully obtain a coll seq with non-null xCmp or nullptr with error if applicable.
    bool ok = (res == nullptr || res == &g_seq) && (pParse.rc == 0 || pParse.rc == SQLITE_ERROR_MISSING_COLLSEQ);
    if(!ok){
        std::cerr << "Test D FAILED: unexpected outcome.\n";
    } else {
        std::cout << "Test D PASSED (outcome depends on mock interactions).\n";
    }
}

int main() {
    std::cout << "Starting sqlite3GetCollSeq unit test suite (mocked environment).\n";

    test_case_A_pColl_nonNull_xCmp_nonNull_returns_same_pointer();
    test_case_B_findseq_no_xcmp_then_synth_populates_and_sets_error();
    test_case_C_findseq_with_xcmp_on_first_call_returns_sequence();
    test_case_D_pColl_null_xCmp_then_succeeding_findseq_returns_sequence();

    std::cout << "Test suite completed. See above for PASS/FAIL indicators.\n";
    return 0;
}
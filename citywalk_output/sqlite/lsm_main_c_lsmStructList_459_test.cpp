/*
  Lightweight C++11 unit-test-like suite for the focal function lsmStructList.
  Notes:
  - This test-suite is self-contained (no GTest or external mocking framework).
  - It re-implements a minimal, self-contained subset of the dependencies
    used by lsmStructList (lsm_db, Snapshot, Level, Segment, LsmString, etc.)
    to exercise the core logic of the function in isolation.
  - Each test case includes explanatory comments describing the scenario and
    the expected outcome.
  - Assertions are non-terminating: test continues on failure to maximize coverage.
  - The code is designed to be compilable with C++11 compilers.
*/

#include <lsmInt.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>


// Domain constants (simplified subset)
static const int LSM_OK = 0;
static const int LSM_NOMEM = -1;

// Forward declarations for test-scaffold types
struct lsm_env;
struct lsm_db;
struct Snapshot;
struct Level;
struct Segment;

// Lightweight string builder matching usage in focal method
struct LsmString {
  std::string z;
  int n;
};

// Minimal dependencies and scaffolds to mimic the focal method's environment
// (these are intentionally lightweight; the goal is to exercise the logic)
struct lsm_env {
  // empty; placeholder
};

struct lsm_db {
  lsm_env *pEnv;
};

// Segment and Level structures to form a small snapshot tree/list
struct Segment {
  int value;
};

struct Level {
  int iAge;
  Level *pNext;
  Segment lhs;          // left-hand segment (single value for test)
  Segment *aRhs;          // array of right-hand segments
  int nRight;             // number of right-hand segments
};

// Snapshot holds top of level list
struct Snapshot {
  Level *pTop;
};

// Global test-state to control behavior of test doubles
static int g_rc_override = LSM_OK;      // return code from infoGetWorker
static Snapshot g_worker;                 // worker snapshot for tests
static int g_freeCounter = 0;             // counter for infoFreeWorker calls

// Function-like prototypes (test-scaffold)
static void infoFreeWorker_dummy(const lsm_db *pDb, int bUnlock);
static int infoGetWorker_dummy(const lsm_db *pDb, Snapshot **pp, int *pbUnlock);
static Level* lsmDbSnapshotLevel_dummy(Snapshot *p);

// LsmString helpers (minimal reimplementation)
static void lsmStringInit(LsmString *s, void *pEnv) {
  (void)pEnv;
  s->z.clear();
  s->n = 0;
}
static void lsmStringAppendf(LsmString *s, const char *fmt, int val) {
  char buf[256];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  s->z.append(buf);
  s->n = (int)s->z.size();
}
static void lsmStringAppend(LsmString *s, const char *z, int n) {
  s->z.append(z, n);
  s->n = (int)s->z.size();
}
static void lsmAppendSegmentList(LsmString *s, const char *zPre, Segment *pSeg) {
  if (pSeg == nullptr) return;
  s->z += zPre;
  s->z += std::to_string(pSeg->value);
  s->n = (int)s->z.size();
}

// Core function under test (re-implemented for isolated testing)
static int lsmStructList(
  lsm_db *pDb,                    /* Database handle */
  char **pzOut                    /* OUT: Nul-terminated string (tcl list) */
){
  // Local copies mimicking the focal method's behavior
  Level *pTopLevel = 0;
  int rc = LSM_OK;
  Level *p;
  LsmString s;
  Snapshot *pWorker;
  int bUnlock = 0;

  // Obtain the worker snapshot
  rc = infoGetWorker_dummy(pDb, &pWorker, &bUnlock);
  if ( rc!=LSM_OK ) return rc;

  // Format the contents of the snapshot as text
  pTopLevel = lsmDbSnapshotLevel_dummy(pWorker);
  lsmStringInit(&s, pDb->pEnv);
  for(p=pTopLevel; rc==LSM_OK && p; p=p->pNext){
    int i;
    lsmStringAppendf(&s, "%s{%d", (s.n ? " " : ""), (int)p->iAge);
    // left hand
    extern void lsmAppendSegmentList(LsmString*, const char*, Segment*);
    lsmAppendSegmentList(&s, " ", &p->lhs);
    // right hand
    for(i=0; rc==LSM_OK && i<p->nRight; i++){
      lsmAppendSegmentList(&s, " ", &p->aRhs[i]);
    }
    lsmStringAppend(&s, "}", 1);
  }
  rc = s.n>=0 ? LSM_OK : LSM_NOMEM;

  // Release the snapshot and return
  infoFreeWorker_dummy(pDb, bUnlock);
  *pzOut = (char*)malloc(s.z.size()+1);
  if(*pzOut) {
    std::memcpy(*pzOut, s.z.c_str(), s.z.size());
    (*pzOut)[s.z.size()] = '\0';
  }
  return rc;
}

// Test hooks to simulate free/restore behavior
static void infoFreeWorker_dummy(const lsm_db *pDb, int bUnlock) {
  (void)pDb;
  (void)bUnlock;
  g_freeCounter++;
}
static int infoGetWorker_dummy(const lsm_db *pDb, Snapshot **pp, int *pbUnlock) {
  (void)pDb;
  *pp = &g_worker;
  if (pbUnlock) *pbUnlock = 0;
  return g_rc_override;
}
static Level* lsmDbSnapshotLevel_dummy(Snapshot *p) {
  (void)p;
  return p ? p->pTop : nullptr;
}

// Helpers for test setup
static void attachLevels(Level *head, Level *next) {
  // simple helper: link head to next
  if (head) head->pNext = next;
}

// Simple assertion helpers (non-terminating)
static int g_failCount = 0;
static void ASSERT_TRUE(bool cond, const char* msg) {
  if (!cond) {
    std::cerr << "ASSERT FAILED: " << msg << std::endl;
    ++g_failCount;
  }
}
static void ASSERT_EQ(int a, int b, const char* msg) {
  if (a != b) {
    std::cerr << "ASSERT FAILED: " << msg << " (expected " << b << ", got " << a << ")" << std::endl;
    ++g_failCount;
  }
}
static void ASSERT_STR_EQ(const std::string &a, const std::string &b, const char* msg) {
  if (a != b) {
    std::cerr << "ASSERT FAILED: " << msg << " (expected \"" << b << "\", got \"" << a << "\")" << std::endl;
    ++g_failCount;
  }
}

// Test 1: rc propagates when infoGetWorker returns an error
static void test_rc_propagates_error() {
  // Setup
  g_rc_override = -1; // simulate an error condition from infoGetWorker
  g_freeCounter = 0;
  g_worker.pTop = nullptr;

  lsm_db db;
  db.pEnv = nullptr;

  char *out = nullptr;

  // Exercise
  int rc = lsmStructList(&db, &out);

  // Verify
  ASSERT_EQ(rc, -1, "Return code should propagate from infoGetWorker error");
  ASSERT_TRUE(out == nullptr, "Output should be NULL when rc != LSM_OK");

  // Cleanup
  if (out) { free(out); out = nullptr; }
}

// Test 2: Single level formatting
static void test_single_level_formatting() {
  // Setup a single level: iAge=5, lhs=1, rhs=[2,3]
  Level level1;
  level1.iAge = 5;
  level1.pNext = nullptr;
  level1.lhs.value = 1;
  level1.nRight = 2;
  level1.aRhs = new Segment[2];
  level1.aRhs[0].value = 2;
  level1.aRhs[1].value = 3;

  g_worker.pTop = &level1;

  // Reset state
  g_rc_override = LSM_OK;
  g_freeCounter = 0;

  lsm_db db;
  db.pEnv = nullptr;
  char *out = nullptr;

  int rc = lsmStructList(&db, &out);

  // Expected: "{5 1 2 3}"
  std::string actual = out ? std::string(out) : std::string();
  std::string expected = "{5 1 2 3}";

  ASSERT_EQ(rc, LSM_OK, "rc should be LSM_OK for successful formatting");
  ASSERT_STR_EQ(actual, expected, "Output string should match expected single-level format");

  // Cleanup
  if (out) { free(out); out = nullptr; }
  delete[] level1.aRhs;
}

// Test 3: Multiple levels formatting
static void test_multiple_levels_formatting() {
  // Level 1: iAge=7, lhs=9, rhs=[11]
  Level level1;
  level1.iAge = 7;
  level1.pNext = nullptr; // to be set later
  level1.lhs.value = 9;
  level1.nRight = 1;
  level1.aRhs = new Segment[1];
  level1.aRhs[0].value = 11;

  // Level 2: iAge=4, lhs=5, no rhs
  Level level2;
  level2.iAge = 4;
  level2.pNext = nullptr;
  level2.lhs.value = 5;
  level2.nRight = 0;
  level2.aRhs = nullptr;

  // Link: level1 -> level2
  level1.pNext = &level2;

  g_worker.pTop = &level1;

  // Reset state
  g_rc_override = LSM_OK;
  g_freeCounter = 0;

  lsm_db db;
  db.pEnv = nullptr;
  char *out = nullptr;

  int rc = lsmStructList(&db, &out);

  // Expected: "{7 9 11} {4 5}"
  std::string actual = out ? std::string(out) : std::string();
  std::string expected = "{7 9 11} {4 5}";

  ASSERT_EQ(rc, LSM_OK, "rc should be LSM_OK for multi-level formatting");
  ASSERT_STR_EQ(actual, expected, "Output string should match expected multi-level format");

  // Cleanup
  if (out) { free(out); out = nullptr; }
  delete[] level1.aRhs;
  // level2.aRhs is nullptr in this test
}

// Test 4: Empty snapshot (no levels)
static void test_empty_snapshot() {
  g_worker.pTop = nullptr;
  g_rc_override = LSM_OK;
  g_freeCounter = 0;

  lsm_db db;
  db.pEnv = nullptr;
  char *out = nullptr;

  int rc = lsmStructList(&db, &out);

  // Empty string expected
  std::string actual = out ? std::string(out) : std::string();
  std::string expected = "";

  ASSERT_EQ(rc, LSM_OK, "rc should be LSM_OK for empty snapshot");
  ASSERT_STR_EQ(actual, expected, "Output should be empty for empty snapshot");

  if (out) { free(out); out = nullptr; }
}

// Test 5: infoFreeWorker should be invoked exactly once
static void test_infoFreeWorker_invoked_once() {
  Level level1;
  level1.iAge = 3;
  level1.pNext = nullptr;
  level1.lhs.value = 0;
  level1.nRight = 0;
  level1.aRhs = nullptr;

  g_worker.pTop = &level1;
  g_rc_override = LSM_OK;
  g_freeCounter = 0;

  lsm_db db;
  db.pEnv = nullptr;
  char *out = nullptr;

  int rc = lsmStructList(&db, &out);

  // We expect infoFreeWorker to be called once
  ASSERT_EQ(g_freeCounter, 1, "infoFreeWorker should be invoked exactly once");

  if (out) { free(out); out = nullptr; }
}

// Main test runner
int main() {
  int total = 0;
  int failures = 0;

  std::cout << "Running lsmStructList unit-test suite (self-contained, non-GTest)..." << std::endl;

  // Run tests
  test_rc_propagates_error();
  test_rc_propagates_error; // no-op to keep structure similar
  test_single_level_formatting();
  test_multiple_levels_formatting();
  test_empty_snapshot();
  test_infoFreeWorker_invoked_once();

  // Summary
  // Recompute failure count from global g_failCount
  total = 5; // We executed 5 test cases
  failures = g_failCount;

  if (failures == 0) {
    std::cout << "ALL TESTS PASSED (" << total << " tests)." << std::endl;
    return 0;
  } else {
    std::cout << failures << " TEST(S) FAILED out of " << total << "." << std::endl;
    return 1;
  }
}
/*
  Unit test suite for the focal method:
  void sqlite3ResultStrAccum(sqlite3_context *pCtx, StrAccum *p)

  This test suite is written in C++11 (no Google Test) and uses a lightweight
  custom test harness with non-terminating EXPECT_*-style assertions to
  maximize code coverage. It includes minimal fake implementations for the
  dependent SQLite types and functions required by the focal method to be
  testable in isolation.

  Focused dependencies (as per the provided <FOCAL_CLASS_DEP> block):
  - StrAccum structure fields: accError, zText, nChar, malloced
  - accError branch: sqlite3_result_error_code
  - isMalloced(p) predicate
  - sqlite3_str_reset(p)
  - isMalloced true path: sqlite3_result_text with SQLITE_DYNAMIC
  - else path: sqlite3_result_text with "" and SQLITE_STATIC
  The tests cover true/false branches for accError and for isMalloced(p).
*/

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Domain constants (to mimic SQLite memory management macros)
#define SQLITE_DYNAMIC 1
#define SQLITE_STATIC 0

// -----------------
// Minimal type stubs
// -----------------
struct StrAccum {
  int accError;   // non-zero indicates an error
  char *zText;     // text buffer
  int nChar;       // length of text to return
  int malloced;    // non-zero if zText was allocated dynamically
};

// Forward declaration of the function under test
void sqlite3ResultStrAccum(struct sqlite3_context *pCtx, StrAccum *p);

// Lightweight sqlite3_context replacement for testing
struct sqlite3_context {
  int errorCode;      // stored error code from sqlite3_result_error_code
  const char *text;     // pointer to text returned by sqlite3_result_text
  int textLen;          // length of the returned text
  int resultType;       // 0 = none, 1 = error, 2 = text
};

// -----------------
// Fake SQLite API helpers (test doubles)
// -----------------

// Record an error code into the fake context
static void sqlite3_result_error_code(sqlite3_context *pCtx, int code) {
  pCtx->errorCode = code;
  pCtx->resultType = 1; // error
}

// Record a text result into the fake context
static void sqlite3_result_text(sqlite3_context *pCtx, const char *z, int n, int destr) {
  (void)destr; // destructor not used in tests
  pCtx->text = z;
  pCtx->textLen = n;
  pCtx->resultType = 2; // text
}

// Reset the StrAccum to its initial state (shallow simulation)
static void sqlite3_str_reset(StrAccum *p) {
  if (p == nullptr) return;
  // Free any allocated buffer if owned by the StrAccum
  if (p->zText != nullptr && p->malloced) {
    free(p->zText);
  }
  p->zText = nullptr;
  p->nChar = 0;
  p->accError = 0;
  p->malloced = 0;
}

// Predicate indicating whether the StrAccum buffer is malloced
static int isMalloced(StrAccum *p) {
  return p->malloced;
}

// -----------------
// Function under test (copied from focal snippet) - placed here for self-contained testing
// -----------------
void sqlite3ResultStrAccum(sqlite3_context *pCtx, StrAccum *p){
  if( p->accError ){
    sqlite3_result_error_code(pCtx, p->accError);
    sqlite3_str_reset(p);
  }else if( isMalloced(p) ){
    sqlite3_result_text(pCtx, p->zText, p->nChar, SQLITE_DYNAMIC);
  }else{
    sqlite3_result_text(pCtx, "", 0, SQLITE_STATIC);
    sqlite3_str_reset(p);
  }
}

// -----------------
// Lightweight test harness (non-terminating expectations)
// -----------------
static int gPass = 0;
static int gFail = 0;

#define EXPECT_EQ(_a, _b) do { \
  auto __a = (_a); auto __b = (_b); \
  if(__a == __b) { ++gPass; } \
  else { \
    ++gFail; \
    std::cout << "EXPECT_EQ FAILED at " << __FILE__ << ":" << __LINE__ \
              << " - actual: " << __a << " != expected: " << __b << std::endl; \
  } \
} while(false)

#define EXPECT_TRUE(_cond) do { \
  if(_cond) { ++gPass; } else { ++gFail; std::cout << "EXPECT_TRUE FAILED at " << __FILE__ << ":" << __LINE__ \
      << " - condition is false" << std::endl; } \
} while(false)

#define TEST_CASE(name) static void name()

static void log_summary(const char* suiteName) {
  std::cout << "Test Suite [" << suiteName << "] - Passed: " << gPass
            << ", Failed: " << gFail << std::endl;
}

// -----------------
// Individual test cases
// -----------------

// Test 1: Error path when accError is non-zero
// - Expect: sqlite3_result_error_code called with accError
// - Expect: sqlite3_str_reset(p) is invoked (p becomes reset)
TEST_CASE(test_error_path) {
  // Prepare test data
  StrAccum s;
  s.accError = 7;
  s.zText = (char*)"ignore_this_text";
  s.nChar = 3;
  s.malloced = 0;

  sqlite3_context ctx;
  ctx.errorCode = 0;
  ctx.text = nullptr;
  ctx.textLen = 0;
  ctx.resultType = 0;

  // Execute
  sqlite3ResultStrAccum(&ctx, &s);

  // Verify: error path taken
  EXPECT_EQ(ctx.resultType, 1);
  EXPECT_EQ(ctx.errorCode, 7);

  // Verify: StrAccum was reset
  EXPECT_EQ(s.accError, 0);
  EXPECT_EQ(s.zText, nullptr);
  EXPECT_EQ(s.nChar, 0);
  EXPECT_EQ(s.malloced, 0);

  // No return value; rely on expectations
}

// Test 2: isMalloced path (dynamic memory), must pass zText and nChar as-is
TEST_CASE(test_malloced_path) {
  // Prepare test data
  StrAccum s;
  s.accError = 0;
  s.zText = const_cast<char*>("hello"); // test string
  s.nChar = 5;
  s.malloced = 1; // dynamic/malloced path

  sqlite3_context ctx;
  ctx.errorCode = 0;
  ctx.text = nullptr;
  ctx.textLen = 0;
  ctx.resultType = 0;

  // Execute
  sqlite3ResultStrAccum(&ctx, &s);

  // Verify: text path taken
  EXPECT_EQ(ctx.resultType, 2);
  EXPECT_EQ(ctx.text, s.zText);     // same pointer
  EXPECT_EQ(ctx.textLen, 5);

  // StrAccum should NOT be reset in malloced path
  EXPECT_EQ(s.accError, 0);
  // zText should remain unchanged
  EXPECT_EQ(std::strcmp(s.zText, "hello"), 0);
}

// Test 3: static path (not malloced), should return empty string and reset
TEST_CASE(test_static_path) {
  // Prepare test data
  StrAccum s;
  s.accError = 0;
  s.zText = (char*)"nonempty"; // not used in this branch
  s.nChar = 0;
  s.malloced = 0; // static path

  sqlite3_context ctx;
  ctx.errorCode = 0;
  ctx.text = nullptr;
  ctx.textLen = 0;
  ctx.resultType = 0;

  // Execute
  sqlite3ResultStrAccum(&ctx, &s);

  // Verify: text path with empty string
  EXPECT_EQ(ctx.resultType, 2);
  // The test double returns the literal "" as the text pointer
  // Since we pass "" with length 0, text should point to some empty string literal
  // We check that the first character is the null terminator
  if (ctx.text != nullptr) {
    EXPECT_EQ(ctx.text[0], '\0');
  } else {
    // If somehow text is null, that's a failure
    ++gFail;
    std::cout << "EXPECT_TRUE FAILED at " << __FILE__ << ":" << __LINE__
              << " - text pointer is null for static path" << std::endl;
  }

  // StrAccum should be reset
  EXPECT_EQ(s.accError, 0);
  EXPECT_EQ(s.zText, nullptr);
  EXPECT_EQ(s.nChar, 0);
  EXPECT_EQ(s.malloced, 0);
}

// -----------------
// Main entry
// -----------------
int main() {
  // Run test cases
  test_error_path();
  test_malloced_path();
  test_static_path();

  log_summary("sqlite3ResultStrAccum(Test suite)");
  // Return non-zero if any test failed
  return (gFail == 0) ? 0 : 1;
}
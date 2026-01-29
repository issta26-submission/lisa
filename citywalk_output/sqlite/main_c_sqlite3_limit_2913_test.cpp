// Comprehensive C++11 unit test suite for sqlite3_limit (focal method) without GTest.
// This single translation unit provides a minimal test harness with non-terminating
// assertions (EXPECT_* macros) and a self-contained, compilable environment.
//
// The test mocks essential sqlite3 structures and constants and includes a local copy
// of the sqlite3_limit function under test, along with minimal dependencies.
// The goal is to achieve thorough coverage of the function's branches, including
// API-armor behavior, boundary clamping, and out-of-range handling.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Domain: Provide necessary constants and types to compile and test sqlite3_limit.

#define SQLITE_ENABLE_API_ARMOR 1     // Enable API armor branch to test early misuse exit path
#define SQLITE_MISUSE_BKPT 0          // Placeholder for misuse breakpoint value

// Forward declaration of the sqlite3 struct used by sqlite3_limit
struct sqlite3;

// Mocked safety check function (only used when API_ARMOR is enabled)
static int sqlite3SafetyCheckOk(sqlite3 *db);

// Bounds and limits (matching a subset of SQLite's API limits for testing)
enum {
  SQLITE_LIMIT_LENGTH = 0,
  SQLITE_LIMIT_SQL_LENGTH,
  SQLITE_LIMIT_COLUMN,
  SQLITE_LIMIT_EXPR_DEPTH,
  SQLITE_LIMIT_COMPOUND_SELECT,
  SQLITE_LIMIT_VDBE_OP,
  SQLITE_LIMIT_FUNCTION_ARG,
  SQLITE_LIMIT_ATTACHED,
  SQLITE_LIMIT_LIKE_PATTERN_LENGTH,
  SQLITE_LIMIT_VARIABLE_NUMBER,
  SQLITE_LIMIT_TRIGGER_DEPTH,
  SQLITE_LIMIT_WORKER_THREADS, // last index
  SQLITE_N_LIMIT               // number of limit categories
};

// Hard upper bounds (compile-time maxima) for each limit category.
// These are used to clamp newLimit values in sqlite3_limit.
#define SQLITE_MAX_LENGTH 16
#define SQLITE_MAX_SQL_LENGTH 16
#define SQLITE_MAX_COLUMN 200
#define SQLITE_MAX_EXPR_DEPTH 1000
#define SQLITE_MAX_COMPOUND_SELECT 64
#define SQLITE_MAX_VDBE_OP 1000
#define SQLITE_MAX_FUNCTION_ARG 127
#define SQLITE_MAX_ATTACHED 10
#define SQLITE_MAX_LIKE_PATTERN_LENGTH 64
#define SQLITE_MAX_VARIABLE_NUMBER 100000
#define SQLITE_MAX_TRIGGER_DEPTH 100
#define SQLITE_MAX_WORKER_THREADS 11  // Must equal SQLITE_N_LIMIT-1 according to code

// Compile-time validation that worker threads limit is the last index
// (In actual SQLite, this assertion holds: SQLITE_LIMIT_WORKER_THREADS == (SQLITE_N_LIMIT-1)).
static_assert(SQLITE_LIMIT_WORKER_THREADS == (SQLITE_N_LIMIT - 1), "Worker threads limit index must be the last limit.");

// Hard limit array (runtime-constant table filled with the maxima)
static int aHardLimit[SQLITE_N_LIMIT] = {
  SQLITE_MAX_LENGTH,
  SQLITE_MAX_SQL_LENGTH,
  SQLITE_MAX_COLUMN,
  SQLITE_MAX_EXPR_DEPTH,
  SQLITE_MAX_COMPOUND_SELECT,
  SQLITE_MAX_VDBE_OP,
  SQLITE_MAX_FUNCTION_ARG,
  SQLITE_MAX_ATTACHED,
  SQLITE_MAX_LIKE_PATTERN_LENGTH,
  SQLITE_MAX_VARIABLE_NUMBER,
  SQLITE_MAX_TRIGGER_DEPTH,
  SQLITE_MAX_WORKER_THREADS
};

// Minimal sqlite3 struct for testing sqlite3_limit
struct sqlite3 {
  int aLimit[SQLITE_N_LIMIT];
  int safetyOkFlag; // Used to simulate safety checks in API armor path
};

// Minimal sqlite3SafetyCheckOk implementation (used when API armor is enabled)
static int sqlite3SafetyCheckOk(sqlite3 *db) {
  // Return non-zero when safe; zero means misuse (as tested path)
  return db ? db->safetyOkFlag != 0 : 0;
}

// Implement the focal function under test (copied/adapted from the provided snippet)
extern "C" {
int sqlite3_limit(sqlite3 *db, int limitId, int newLimit){
  int oldLimit;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ){
    (void)SQLITE_MISUSE_BKPT;
    return -1;
  }
#endif
  /* EVIDENCE-OF: R-30189-54097 For each limit category SQLITE_LIMIT_NAME
  ** there is a hard upper bound set at compile-time by a C preprocessor
  ** macro called SQLITE_MAX_NAME. (The "_LIMIT_" in the name is changed to
  ** "_MAX_".)
  */
  assert( aHardLimit[SQLITE_LIMIT_LENGTH]==SQLITE_MAX_LENGTH );
  assert( aHardLimit[SQLITE_LIMIT_SQL_LENGTH]==SQLITE_MAX_SQL_LENGTH );
  assert( aHardLimit[SQLITE_LIMIT_COLUMN]==SQLITE_MAX_COLUMN );
  assert( aHardLimit[SQLITE_LIMIT_EXPR_DEPTH]==SQLITE_MAX_EXPR_DEPTH );
  assert( aHardLimit[SQLITE_LIMIT_COMPOUND_SELECT]==SQLITE_MAX_COMPOUND_SELECT);
  assert( aHardLimit[SQLITE_LIMIT_VDBE_OP]==SQLITE_MAX_VDBE_OP );
  assert( aHardLimit[SQLITE_LIMIT_FUNCTION_ARG]==SQLITE_MAX_FUNCTION_ARG );
  assert( aHardLimit[SQLITE_LIMIT_ATTACHED]==SQLITE_MAX_ATTACHED );
  assert( aHardLimit[SQLITE_LIMIT_LIKE_PATTERN_LENGTH]==
                                               SQLITE_MAX_LIKE_PATTERN_LENGTH );
  assert( aHardLimit[SQLITE_LIMIT_VARIABLE_NUMBER]==SQLITE_MAX_VARIABLE_NUMBER);
  assert( aHardLimit[SQLITE_LIMIT_TRIGGER_DEPTH]==SQLITE_MAX_TRIGGER_DEPTH );
  assert( aHardLimit[SQLITE_LIMIT_WORKER_THREADS]==SQLITE_MAX_WORKER_THREADS );
  assert( SQLITE_LIMIT_WORKER_THREADS==(SQLITE_N_LIMIT-1) );

  if( limitId<0 || limitId>=SQLITE_N_LIMIT ){
    return -1;
  }
  oldLimit = db->aLimit[limitId];
  if( newLimit>=0 ){                   /* IMP: R-52476-28732 */
    if( newLimit>aHardLimit[limitId] ){
      newLimit = aHardLimit[limitId];  /* IMP: R-51463-25634 */
    }else if( newLimit<1 && limitId==SQLITE_LIMIT_LENGTH ){
      newLimit = 1;
    }
    db->aLimit[limitId] = newLimit;
  }
  return oldLimit;                     /* IMP: R-53341-35419 */
}
}

// Minimal test harness and test cases
// Non-terminating assertions (EXPECT_*), with a simple reporter.

static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT_EQ(a,b) do { ++g_total; if((a)!=(b)) { \
  std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ << " " #a " != " #b \
            << " (actual=" << (a) << ", expected=" << (b) << ")\n"; \
  ++g_failed; } else { ++g_passed; } } while(0)

#define EXPECT_NE(a,b) do { ++g_total; if((a)==(b)) { \
  std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ << " " #a " == " #b \
            << " (actual=" << (a) << ")\n"; \
  ++g_failed; } else { ++g_passed; } } while(0)

#define EXPECT_TRUE(x) do { ++g_total; if(!(x)) { \
  std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ << " " #x " is not true\n"; \
  ++g_failed; } else { ++g_passed; } } while(0)

#define EXPECT_FALSE(x) do { ++g_total; if((x)) { \
  std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ << " " #x " is not false\n"; \
  ++g_failed; } else { ++g_passed; } } while(0)

// Helper to reset a db instance before each test
static void reset_db(sqlite3 &db, int initialLimitValue)
{
  for(int i = 0; i < SQLITE_N_LIMIT; ++i){
    db.aLimit[i] = initialLimitValue;
  }
  // Default safety flag is safe unless overridden per test
  db.safetyOkFlag = 1;
}

// Test 1: Basic within-bounds update for LENGTH (SQLITE_LIMIT_LENGTH)
static void test_basic_within_bounds_length()
{
  sqlite3 db;
  reset_db(db, 7); // initial oldLimit will be 7 for all just to be predictable
  // Call with newLimit within hard bound and not triggering min or clamp
  int old = sqlite3_limit(&db, SQLITE_LIMIT_LENGTH, 5);
  EXPECT_EQ(old, 7);           // old value should be as initialized
  EXPECT_EQ(db.aLimit[SQLITE_LIMIT_LENGTH], 5); // new value should be 5
}

// Test 2: Clamp to hard upper bound when newLimit exceeds hard limit for LENGTH
static void test_length_clamp_to_hard_upper()
{
  sqlite3 db;
  reset_db(db, 3);
  // newLimit exceeds MAX -> clamp to aHardLimit[SQLITE_LIMIT_LENGTH] which is SQLITE_MAX_LENGTH
  int old = sqlite3_limit(&db, SQLITE_LIMIT_LENGTH, aHardLimit[SQLITE_LIMIT_LENGTH] + 5);
  EXPECT_EQ(old, 3);
  EXPECT_EQ(db.aLimit[SQLITE_LIMIT_LENGTH], SQLITE_MAX_LENGTH);
}

// Test 3: Enforce minimum of 1 for LENGTH when newLimit < 1
static void test_length_min_one_enforced()
{
  sqlite3 db;
  reset_db(db, 9);
  int old = sqlite3_limit(&db, SQLITE_LIMIT_LENGTH, 0);
  EXPECT_EQ(old, 9);
  EXPECT_EQ(db.aLimit[SQLITE_LIMIT_LENGTH], 1);
}

// Test 4: Negative newLimit does not alter the current limit (except for API armor path)
static void test_negative_newlimit_no_change()
{
  sqlite3 db;
  reset_db(db, 12);
  int old = sqlite3_limit(&db, SQLITE_LIMIT_LENGTH, -5);
  EXPECT_EQ(old, 12);
  EXPECT_EQ(db.aLimit[SQLITE_LIMIT_LENGTH], 12);
}

// Test 5: Invalid limitId (out of range) returns -1 and does not change DB
static void test_limitid_out_of_range()
{
  sqlite3 db;
  reset_db(db, 15);
  int old = sqlite3_limit(&db, SQLITE_N_LIMIT, 99);
  EXPECT_EQ(old, -1);
  // Ensure no changes to existing limits
  EXPECT_EQ(db.aLimit[0], 15);
}

// Test 6: API armor safety check failure returns -1 and does not change DB
static void test_api_armor_safety_failure()
{
  sqlite3 db;
  reset_db(db, 6);
  db.safetyOkFlag = 0; // simulate unsafe db
  int old = sqlite3_limit(&db, SQLITE_LIMIT_LENGTH, 8);
  EXPECT_EQ(old,  -1); // should fail early due to safety check
  // And nothing should have changed
  EXPECT_EQ(db.aLimit[SQLITE_LIMIT_LENGTH], 6);
}

// Test 7: Update non-LENGTH limit clamps to its hard upper bound
static void test_non_length_limit_clamp()
{
  sqlite3 db;
  reset_db(db, 1);
  // Choose a non-LENGTH limit (VDBE_OP)
  int old = sqlite3_limit(&db, SQLITE_LIMIT_VDBE_OP, SQLITE_MAX_VDBE_OP + 123);
  EXPECT_EQ(old, 1);
  EXPECT_EQ(db.aLimit[SQLITE_LIMIT_VDBE_OP], SQLITE_MAX_VDBE_OP);
}

// Run all tests and report summary
int main()
{
  // Execute tests
  test_basic_within_bounds_length();
  test_length_clamp_to_hard_upper();
  test_length_min_one_enforced();
  test_negative_newlimit_no_change();
  test_limitid_out_of_range();
  test_api_armor_safety_failure();
  test_non_length_limit_clamp();

  // Print summary
  std::cout << "Tests run: " << g_total
            << ", Passed: " << g_passed
            << ", Failed: " << g_failed << std::endl;

  // Return non-zero if any test failed
  return g_failed ? 1 : 0;
}
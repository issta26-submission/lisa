// A lightweight C++11 test suite for the focal C function:
// int sqlite3SafetyCheckOk(sqlite3 *db)
// The tests are designed to be GTest-free and self-contained, using a minimal
// harness pattern. They rely on including the project's SQLite internal headers
// to obtain the real types and constants (e.g., sqlite3, SQLITE_STATE_OPEN).

#include <math.h>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Include the project's internal SQLite headers to access sqlite3, u8, and SQLITE_STATE_OPEN.
extern "C" {
}

// Forward declaration of the focal function (will be linked from the project object files)
extern "C" int sqlite3SafetyCheckOk(sqlite3 *db);

// Simple test harness
static int g_failures = 0;

#define ASSERT(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "ASSERT FAILED: " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
      ++g_failures; \
    } \
  } while(false)

// Helper to construct a minimal sqlite3 object with a specified eOpenState.
// The sqlite3 struct in sqliteInt.h is a plain POD; we can allocate and set fields directly.
static sqlite3* makeDbWithState(uint8_t state) {
  sqlite3* db = new sqlite3(); // value-initialized to zero for PODs
  // Cast to the internal type's field type to assign the open state.
  db->eOpenState = (u8)state;
  return db;
}

// Test 1: NULL database pointer should yield 0
void test_null_db_returns_zero() {
  int rc = sqlite3SafetyCheckOk(nullptr);
  ASSERT(rc == 0, "sqlite3SafetyCheckOk(nullptr) should return 0");
}

// Test 2: Database with OPEN state should yield 1
void test_open_state_open_returns_one() {
  // Use the real OPEN constant from the project
  sqlite3* db = makeDbWithState(SQLITE_STATE_OPEN);
  int rc = sqlite3SafetyCheckOk(db);
  ASSERT(rc == 1, "sqlite3SafetyCheckOk(db with OPEN) should return 1");
  delete db;
}

// Test 3: Database with not-OPEN state should yield 0
void test_open_state_not_open_returns_zero() {
  // Choose a not-open state value by flipping a bit of SQLITE_STATE_OPEN
  uint8_t notOpenValue = (uint8_t)(SQLITE_STATE_OPEN ^ 0x01);
  sqlite3* db = makeDbWithState(notOpenValue);
  int rc = sqlite3SafetyCheckOk(db);
  ASSERT(rc == 0, "sqlite3SafetyCheckOk(db with not-OPEN) should return 0");
  delete db;
}

int main() {
  // Run tests
  test_null_db_returns_zero();
  test_open_state_open_returns_one();
  test_open_state_not_open_returns_zero();

  // Summary
  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}
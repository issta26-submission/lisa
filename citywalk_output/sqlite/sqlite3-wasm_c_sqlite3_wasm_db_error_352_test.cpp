/*
  Test suite for the focal method:
  int sqlite3_wasm_db_error(sqlite3* db, int err_code, const char *zMsg)

  Strategy:
  - Use a lightweight, self-contained C++ test harness (no GoogleTest).
  - Rely on the included sqlite3-wasm.c (via the build system) to provide sqlite3_wasm_db_error
    and the underlying SQLite runtime implemented therein.
  - Exercise true/false branches of the method:
    1) db == nullptr (no-op, returns err_code)
    2) db != nullptr and zMsg != nullptr (non-empty string) -> ensure error code is set and message contains zMsg
    3) db != nullptr and zMsg == NULL -> ensure error code is set
  - Validate using sqlite3_errcode(db) and sqlite3_errmsg(db) to observe the effect.
  - Tests are designed to be executable under C++11, without GTest, using simple assertions and an
    internal non-terminating reporter.

  Notes:
  - The test relies on the embedded SQLite runtime within sqlite3-wasm.c. It opens an in-memory DB
    to verify the behavior.
  - We declare the necessary C interfaces with extern "C" to avoid C++ name mangling issues when linking.
  - All tests print per-test results and a final summary. Exit status is 0 when all tests pass.
*/

#include <iostream>
#include <assert.h>
#include <string>
#include <emscripten/wasmfs.h>


// Lightweight C interface declarations for the test harness.
// We forward-declare minimal types and prototypes required to invoke sqlite3_wasm_db_error
// and verify its effects via SQLite's public APIs.
extern "C" {

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_mutex sqlite3_mutex;

/* Core API used by tests to drive the focal function */
int sqlite3_open(const char* zName, sqlite3** ppDb);
void sqlite3_close(sqlite3*);

int sqlite3_errcode(sqlite3*);           // Retrieve the numeric error code
const char* sqlite3_errmsg(sqlite3*);    // Retrieve the error message string

/* Internal helpers used by sqlite3_wasm_db_error (need prototypes for linkage) */
int sqlite3Strlen30(const char*);        // length helper
struct sqlite3_mutex* sqlite3_db_mutex(sqlite3*); // get db mutex
void sqlite3_mutex_enter(struct sqlite3_mutex*);  // enter mutex
void sqlite3_mutex_leave(struct sqlite3_mutex*);  // leave mutex

// Focal function under test
int sqlite3_wasm_db_error(sqlite3* db, int err_code, const char *zMsg);

} // extern "C"

// Simple test harness state
static int tests_run = 0;
static int tests_passed = 0;

// Helper: non-terminating assertion/reporting
static void report_test(const std::string& test_name, bool ok) {
  ++tests_run;
  if(ok) {
    ++tests_passed;
    std::cout << "[OK] " << test_name << "\n";
  } else {
    std::cout << "[FAIL] " << test_name << "\n";
  }
}

// Test 1: db == nullptr should return the provided error code and do nothing else
// Purpose: Validate the null-database guard clause.
static void test_null_db() {
  const int ERR_CODE = 123;
  int ret = sqlite3_wasm_db_error(nullptr, ERR_CODE, "test message");
  bool ok = (ret == ERR_CODE);
  report_test("test_null_db: returns err_code when db is NULL", ok);
}

// Test 2: db != nullptr and zMsg != NULL (non-empty string)
// Expectation: errno/code is stored in the DB and the message contains zMsg
static void test_with_message() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if(rc != 0 || db == nullptr) {
    std::cout << "[WARN] test_with_message: could not create in-memory DB; skipping.\n";
    report_test("test_with_message: in-memory DB creation", db != nullptr);
    return;
  }

  const int TEST_ERR = 444;
  const char* zMsg = "boom!";
  int ret = sqlite3_wasm_db_error(db, TEST_ERR, zMsg);

  bool codeOk = (ret == TEST_ERR) && (sqlite3_errcode(db) == TEST_ERR);
  bool messageOk = false;
  if(codeOk) {
    const char* observed = sqlite3_errmsg(db);
    if(observed != nullptr) {
      std::string s(observed);
      // We expect the stored message to contain the provided zMsg portion
      messageOk = (s.find("boom") != std::string::npos) || (s.find("boom!") != std::string::npos);
    }
  }

  sqlite3_close(db);
  report_test("test_with_message: code and message store correctly", codeOk && messageOk);
}

// Test 3: db != nullptr and zMsg == NULL
// Expectation: code is stored; message content is not relied upon (could be NULL/empty)
static void test_with_null_message() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if(rc != 0 || db == nullptr) {
    std::cout << "[WARN] test_with_null_message: could not create in-memory DB; skipping.\n";
    report_test("test_with_null_message: in-memory DB creation", db != nullptr);
    return;
  }

  const int TEST_ERR = 555;
  int ret = sqlite3_wasm_db_error(db, TEST_ERR, NULL);

  bool ok = (ret == TEST_ERR) && (sqlite3_errcode(db) == TEST_ERR);
  // We avoid asserting on the specific error message content when zMsg is NULL.
  sqlite3_close(db);
  report_test("test_with_null_message: code is stored when zMsg is NULL", ok);
}

// Main: run all tests and summarize
int main() {
  test_null_db();
  test_with_message();
  test_with_null_message();

  std::cout << "Tests run: " << tests_run << ", Passed: " << tests_passed << std::endl;
  return (tests_run == tests_passed) ? 0 : 1;
}
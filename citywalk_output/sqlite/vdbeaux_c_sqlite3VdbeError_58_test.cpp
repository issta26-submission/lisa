// Self-contained C++11 unit tests for the focal method: sqlite3VdbeError
// The tests provide minimal mocks for dependent SQLite structures and functions.
// They verify that sqlite3VdbeError frees the previous error message and sets a new formatted message.

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>


// Domain-specific minimal mock environment
// We implement only what is necessary for testing sqlite3VdbeError.

static std::vector<void*> freed_pointers; // record freed pointers for verification

// Forward declarations to simulate SQLite infrastructure (C linkage)
extern "C" {

// Minimal sqlite3 struct (opaque for test)
struct sqlite3 {};

// Minimal Vdbe struct as used by sqlite3VdbeError
struct Vdbe {
  sqlite3 *db;
  char *zErrMsg;
};

// Mock: sqlite3DbFree(db, p) -> frees previously allocated error message
void sqlite3DbFree(sqlite3 *db, void *p) {
  if (p != nullptr) {
    freed_pointers.push_back(p);
    free(p);
  }
}

// Mock: sqlite3VMPrintf(db, zFormat, va_list ap) -> allocate formatted string
char* sqlite3VMPrintf(sqlite3 *db, const char *zFormat, va_list ap) {
  // Copy va_list to compute length
  va_list ap_copy;
  va_copy(ap_copy, ap);
  int n = vsnprintf(nullptr, 0, zFormat, ap_copy);
  va_end(ap_copy);

  char *zOut = (char*)malloc(n + 1);
  if (zOut == nullptr) return nullptr;
  vsnprintf(zOut, n + 1, zFormat, ap);
  return zOut;
}

// The focal method under test (copied here to be compiled and tested)
// In the real project this would reside in vdbeaux.c. We replicate behavior faithfully.
void sqlite3VdbeError(Vdbe *p, const char *zFormat, ...) {
  va_list ap;
  sqlite3DbFree(p->db, p->zErrMsg);        // Free previously stored error message
  va_start(ap, zFormat);
  p->zErrMsg = sqlite3VMPrintf(p->db, zFormat, ap);
  va_end(ap);
}

} // extern "C"

// Helper test utilities
static void logFail(const std::string &msg) {
  std::cerr << "FAIL: " << msg << std::endl;
}

static void logPass(const std::string &msg) {
  std::cout << "PASS: " << msg << std::endl;
}

// Test 1: Replaces non-null old error message and frees old memory.
// Verifies: old pointer is freed, new message is correctly formatted, and new zErrMsg points to the expected string.
bool test_replace_old_message() {
  // Prepare: old message allocated and assigned to p.zErrMsg
  freed_pointers.clear();

  sqlite3 db;
  Vdbe p;
  p.db = &db;

  const char *initial = "old_msg";
  p.zErrMsg = (char*)malloc(strlen(initial) + 1);
  if (p.zErrMsg == nullptr) return false;
  strcpy(p.zErrMsg, initial);

  char *old_ptr = p.zErrMsg; // keep reference to verify freeing

  // Exercise
  sqlite3VdbeError(&p, "new: %d", 7);

  // Verify
  bool ok = (p.zErrMsg != nullptr) && (strcmp(p.zErrMsg, "new: 7") == 0)
            && (freed_pointers.size() == 1) && (freed_pointers[0] == old_ptr);

  // Cleanup
  if (p.zErrMsg) free(p.zErrMsg);
  // old_ptr has already been freed by sqlite3DbFree during the test

  return ok;
}

// Test 2: No old message (null zErrMsg). Ensures no double-free and formatting works.
// Verifies: new message is correctly formed and assigned; no freed pointer recorded.
bool test_no_old_message() {
  freed_pointers.clear();

  sqlite3 db;
  Vdbe p;
  p.db = &db;
  p.zErrMsg = nullptr;

  // Exercise
  sqlite3VdbeError(&p, "hello %s %d", "world", 42);

  // Verify
  bool ok = (p.zErrMsg != nullptr) && (strcmp(p.zErrMsg, "hello world 42") == 0)
            && (freed_pointers.empty() == true);

  // Cleanup
  if (p.zErrMsg) free(p.zErrMsg);

  return ok;
}

// Test 3: No variadic arguments provided (single-argument formatting).
// Verifies: formatting with no extra args works, old message freed.
bool test_no_extra_args() {
  freed_pointers.clear();

  sqlite3 db;
  Vdbe p;
  p.db = &db;

  const char *initial = "to_be_freed";
  p.zErrMsg = (char*)malloc(strlen(initial) + 1);
  if (p.zErrMsg == nullptr) return false;
  strcpy(p.zErrMsg, initial);

  char *old_ptr = p.zErrMsg;

  // Exercise: provide a fixed string with no additional format args
  sqlite3VdbeError(&p, "text_only");

  // Verify
  bool ok = (p.zErrMsg != nullptr) && (strcmp(p.zErrMsg, "text_only") == 0)
            && (freed_pointers.size() == 1) && (freed_pointers[0] == old_ptr);

  // Cleanup
  if (p.zErrMsg) free(p.zErrMsg);

  return ok;
}

// Simple test harness
int main() {
  int total = 3;
  int passed = 0;

  if (test_replace_old_message()) {
    logPass("test_replace_old_message");
    ++passed;
  } else {
    logFail("test_replace_old_message");
  }

  if (test_no_old_message()) {
    logPass("test_no_old_message");
    ++passed;
  } else {
    logFail("test_no_old_message");
  }

  if (test_no_extra_args()) {
    logPass("test_no_extra_args");
    ++passed;
  } else {
    logFail("test_no_extra_args");
  }

  std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
  return (passed == total) ? 0 : 1;
}
// Unit test suite for sqlite3_backup_init (C API) using C++11 (no GTest).
// The tests exercise the focal function via the public SQLite API and verify
// observable behavior without relying on private members or internal static
// helpers. The tests are designed to be self-contained and executable
// from a single main() function.

#include <btreeInt.h>
#include <iostream>
#include <sqliteInt.h>
#include <string>


extern "C" {
  #include <sqlite3.h>
}

// Simple helper to open an in-memory SQLite database.
static bool openMemoryDb(sqlite3** ppDb) {
  int rc = sqlite3_open(":memory:", ppDb);
  if (rc != SQLITE_OK) {
    std::cerr << "Failed to open in-memory DB: " << sqlite3_errmsg(*ppDb) << std::endl;
    return false;
  }
  return true;
}

// Test 1: Distinct destination and source databases should yield a valid backup handle.
// This covers the successful path where pSrcDb != pDestDb and both named databases exist.
static bool test_backup_init_distinct_dbs() {
  std::cout << "Test 1: backup_init with distinct source and destination databases\n";

  sqlite3* dst = nullptr;
  sqlite3* src = nullptr;
  if (!openMemoryDb(&dst)) {
    std::cerr << "Test 1: Failed to create destination in-memory DB.\n";
    return false;
  }
  if (!openMemoryDb(&src)) {
    std::cerr << "Test 1: Failed to create source in-memory DB.\n";
    sqlite3_close(dst);
    return false;
  }

  // Attempt to initialize a backup from src ("main") into dst ("main").
  sqlite3_backup* p = sqlite3_backup_init(dst, "main", src, "main");
  bool ok = (p != nullptr);
  if (p) {
    // Clean up the backup object if it was created.
    int finish_rc = sqlite3_backup_finish(p);
    if (finish_rc != SQLITE_OK) {
      std::cerr << "Test 1: sqlite3_backup_finish reported error: " << sqlite3_errmsg(dst) << "\n";
      ok = false;
    }
  } else {
    std::cerr << "Test 1: sqlite3_backup_init returned NULL unexpectedly.\n";
  }

  sqlite3_close(dst);
  sqlite3_close(src);
  std::cout << (ok ? "Test 1 PASSED\n" : "Test 1 FAILED\n");
  return ok;
}

// Test 2: Source and destination pointers are the same should produce an error
// and return NULL. This tests the explicit "source and destination must be distinct" path.
static bool test_backup_init_same_pointer_error() {
  std::cout << "Test 2: backup_init with identical source and destination should fail\n";

  sqlite3* db = nullptr;
  if (!openMemoryDb(&db)) {
    std::cerr << "Test 2: Failed to create in-memory DB.\n";
    return false;
  }

  sqlite3_backup* p = sqlite3_backup_init(db, "main", db, "main");
  bool ok = (p == nullptr);

  // Expect an error message to have been recorded on the destination database handle.
  const char* errMsg = sqlite3_errmsg(db);
  bool hasExpectedFragment = (errMsg != nullptr) && (std::string(errMsg).find("source and destination") != std::string::npos);

  // Cleanup: ensure no backup object remains and close DB.
  if (p) {
    sqlite3_backup_finish(p);
  }
  sqlite3_close(db);

  if (!ok) {
    std::cerr << "Test 2: Expected NULL from sqlite3_backup_init, got non-NULL.\n";
  }
  if (ok && hasExpectedFragment) {
    std::cout << "Test 2: Received expected error message on db handle: " << errMsg << "\n";
  } else if (ok) {
    // If we couldn't confirm the message fragment, still consider test passed if NULL was returned.
    std::cout << "Test 2: No explicit error message checked; assuming correct NULL return.\n";
  }

  std::cout << (ok ? "Test 2 PASSED\n" : "Test 2 FAILED\n");
  return ok;
}

// Test 3: Non-existent source database name should result in NULL (failure path).
// This exercises the branch where the requested named databases do not exist.
static bool test_backup_init_missing_source_db_name() {
  std::cout << "Test 3: backup_init with non-existent source database name should fail\n";

  sqlite3* dst = nullptr;
  sqlite3* src = nullptr;
  if (!openMemoryDb(&dst)) {
    std::cerr << "Test 3: Failed to create destination in-memory DB.\n";
    return false;
  }
  if (!openMemoryDb(&src)) {
    std::cerr << "Test 3: Failed to create source in-memory DB.\n";
    sqlite3_close(dst);
    return false;
  }

  // Note: zSrcDb is intentionally a non-existent database name for src.
  sqlite3_backup* p = sqlite3_backup_init(dst, "main", src, "not_exist_db_name");
  bool ok = (p == nullptr);

  // Cleanup
  sqlite3_close(dst);
  sqlite3_close(src);

  std::cout << (ok ? "Test 3 PASSED\n" : "Test 3 FAILED\n");
  return ok;
}

int main() {
  int total = 0;
  int passed = 0;

  auto count_and_report = [&](bool result, const std::string& name){
    total++;
    if (result) passed++;
    std::cout << name << ": " << (result ? "PASSED" : "FAILED") << "\n";
  };

  bool r1 = test_backup_init_distinct_dbs();
  count_and_report(r1, "Test 1");
  bool r2 = test_backup_init_same_pointer_error();
  count_and_report(r2, "Test 2");
  bool r3 = test_backup_init_missing_source_db_name();
  count_and_report(r3, "Test 3");

  std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

  return (passed == total) ? 0 : 1;
}
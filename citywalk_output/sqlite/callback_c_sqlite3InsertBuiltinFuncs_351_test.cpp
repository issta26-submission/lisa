// High-coverage C++11 test harness for the focal C function:
// sqlite3InsertBuiltinFuncs in callback.c
// This test is designed to compile along with the C source in a typical
// C/C++ mixed build (no GoogleTest). It provides minimal, self-contained
// stubs for the required SQLite-dependent pieces and exercises both
// branches of sqlite3InsertBuiltinFuncs: presence and absence of an existing
// function with the same name in the builtin function hash table.
//
// Notes:
// - We implement a minimal FuncDef structure to mirror the fields used by
//   sqlite3InsertBuiltinFuncs: zName, funcFlags, pNext, and a union-like u.pHash.
// - We provide a minimal sqlite3BuiltinFunctions global with an a[] hash table.
// - We implement sqlite3Strlen30, SQLITE_FUNC_HASH, and sqlite3FunctionSearch with
//   lightweight, self-contained logic so the test does not depend on the real
//   SQLite implementation.
// - We implement a tiny, non-terminating assertion helper to collect failures
//   without aborting the test run (as requested: non-terminating expectations).
// - The tests are executed from main() with explanatory comments for each test.
//
// This file should be compiled together with callback.c (your focal implementation).
// Ensure the build defines C linkage for the C symbols as needed.

#include <vector>
#include <cerrno>
#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declarations and minimal stubs to allow linking with callback.c
extern "C" {

// Minimal FuncDef structure compatible with sqlite3InsertBuiltinFuncs usage
typedef struct FuncDef {
  const char *zName;     // function name
  int funcFlags;           // flags; must include SQLITE_FUNC_BUILTIN for test
  struct FuncDef *pNext;     // next in hash chain
  union {
    struct FuncDef *pHash;   // used in the hash table
  } u;
} FuncDef;

// Lightweight builtin function hash table (as the real project would provide)
typedef struct {
  FuncDef* a[256]; // heads of chains
} BuiltinFunctions;

BuiltinFunctions sqlite3BuiltinFunctions = {{nullptr}}; // zero-initialized

// Helper: emulate strlen with 30-char max as in sqlite3Strlen30
static int sqlite3Strlen30(const char *z) {
  if (z == nullptr) return 0;
  int n = 0;
  while (z[n] != '\0' && n < 30) n++;
  return n;
}

// Heuristic hash function: provide a stable, deterministic hash based on first char and length
static int SQLITE_FUNC_HASH(char c, int nName) {
  // Simple, deterministic combination (not SQLite's real hash)
  return (static_cast<unsigned int>(static_cast<unsigned char>(c)) << 8) ^ nName;
}

// Stub: search for a function by name in the builtin function table.
// In production SQLite this is more complex; for tests we implement a simple search.
static FuncDef* sqlite3FunctionSearch(int h, const char *zFunc) {
  (void)h; // h is unused in this stub except to mirror signature
  if (zFunc == nullptr) return nullptr;

  for (int i = 0; i < 256; ++i) {
    for (FuncDef* p = sqlite3BuiltinFunctions.a[i]; p != nullptr; p = p->pNext) {
      if (p->zName && strcmp(p->zName, zFunc) == 0) {
        return p;
      }
    }
  }
  return nullptr;
}

// Forward declare the function under test so this test file can call it.
// The real implementation is in callback.c; we link against it in build.
// We'll declare with the same signature for correct linkage.
void sqlite3InsertBuiltinFuncs(FuncDef *aDef, int nDef);

// Additional forward declarations that may be present in the file's local scope.
// These are provided to ensure the linker can resolve the symbols used in the
// focal function when callback.c is built together with this test.
typedef struct Parse Parse;
typedef struct CollSeq CollSeq;
typedef struct Schema Schema;
typedef struct Btree Btree;
typedef struct sqlite3 sqlite3; // opaque alias to satisfy sqlite3* parameters

} // extern "C"

// Lightweight test framework (non-terminating assertions)
static int g_test_total = 0;
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
  ++g_test_total; \
  if(!(cond)) { \
    g_test_failures++; \
    std::fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", msg); \
  } \
} while(0)

#define TEST(name) void name(); int main_run_##name = (name(), 0); void name()

// Helper to reset builtin table to a known empty state
static void reset_builtin_table() {
  for(int i = 0; i < 256; ++i) sqlite3BuiltinFunctions.a[i] = nullptr;
}

// Helper to compute same hash as the focal function for a given name
static int computeHashForName(const char* name) {
  int nName = sqlite3Strlen30(name);
  return SQLITE_FUNC_HASH(name[0], nName);
}

// Test 1: Inserting a def when no existing function with the same name exists.
// Expected: new def becomes head of the bucket, pNext = nullptr, u.pHash = nullptr.
TEST(test_insert_no_existing) {
  reset_builtin_table();

  // Prepare a single new FuncDef to insert
  static FuncDef newDef;
  const char* fname = "newFunc"; // non-empty name
  newDef.zName = fname;
  newDef.funcFlags = 1; // SQLITE_FUNC_BUILTIN
  newDef.pNext = nullptr;
  newDef.u.pHash = nullptr;

  FuncDef* apDef[1] = { &newDef };

  // Call the focal function (from callback.c)
  sqlite3InsertBuiltinFuncs(apDef, 1);

  int expectedHash = computeHashForName(fname);
  // After insertion with no existing function, head should be the newDef
  EXPECT_TRUE(sqlite3BuiltinFunctions.a[expectedHash] == &newDef,
              "Head of bucket should be newDef when no existing function found");
  EXPECT_TRUE(newDef.pNext == nullptr,
              "New def's pNext should be nullptr when there was no existing function");
  EXPECT_TRUE(newDef.u.pHash == nullptr,
              "New def's u.pHash should be the previous head (nullptr)");
}

// Test 2: Inserting a def when an existing function with the same name is already in the hash.
// Expected: existing function's pNext points to newDef; newDef's pNext points to old pNext (likely nullptr);
TEST(test_insert_existing) {
  reset_builtin_table();

  // Create an existing function with the same name as the new one
  static FuncDef existing;
  const char* fname = "existing"; // non-empty name
  existing.zName = fname;
  existing.funcFlags = 1;
  existing.pNext = nullptr;
  existing.u.pHash = nullptr;

  int h = computeHashForName(fname);
  // Insert existing into the head of the appropriate bucket
  existing.pNext = sqlite3BuiltinFunctions.a[h];
  sqlite3BuiltinFunctions.a[h] = &existing;

  // Prepare a new def with the same name
  static FuncDef newDef;
  newDef.zName = fname;
  newDef.funcFlags = 1;
  newDef.pNext = nullptr;
  newDef.u.pHash = nullptr;

  FuncDef* apDef[1] = { &newDef };

  // Call the focal function
  sqlite3InsertBuiltinFuncs(apDef, 1);

  // After insertion, existing should point to newDef
  EXPECT_TRUE(existing.pNext == &newDef,
              "Existing function's pNext should point to newly inserted def");
  // New def's pNext should be the old existing's pNext (nullptr in this setup)
  EXPECT_TRUE(newDef.pNext == nullptr,
              "New def's pNext should equal the old existing pNext (nullptr)");
  // New def's u.pHash remains its initial value (nullptr) since the path uses pNext insertion
  EXPECT_TRUE(newDef.u.pHash == nullptr,
              "New def's u.pHash should remain unchanged (nullptr) when existing function found");
  // Head of bucket should still be the existing function (not the newly inserted def)
  EXPECT_TRUE(sqlite3BuiltinFunctions.a[h] == &existing,
              "Bucket head should remain the existing function after insertion");
}

// Simple harness runner
TEST(run_all) {
  std::cout << "Starting sqlite3InsertBuiltinFuncs tests (non-GTest harness).\n";

  test_insert_no_existing();
  test_insert_existing();

  std::cout << "Tests completed. Total: " << g_test_total
            << ", Failures: " << g_test_failures << "\n";

  if (g_test_failures > 0) {
    std::cerr << "Some tests failed.\n";
  } else {
    std::cout << "All tests passed.\n";
  }
}

// Ensure the tests are invoked (non-GTest style)
int main() {
  // The test framework above defines a static main in this file via the TEST macro pattern.
  // Instead of relying on macro-to-main shenanigans, explicitly call the runner.
  // However, to preserve clarity, we simply call run_all() here.
  run_all();
  return g_test_failures == 0 ? 0 : 1;
}
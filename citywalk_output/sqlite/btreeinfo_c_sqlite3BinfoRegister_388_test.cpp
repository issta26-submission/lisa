// Unit test suite for sqlite3BinfoRegister (btreeinfo.c) using a lightweight, no-GTest approach.
// This test suite is written in C++11 and relies on a small, in-process mock of the SQLite extension API.
// It exercises the focal function by providing a fake sqlite3_create_module implementation and
// verifying how sqlite3BinfoRegister interacts with it.

#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <cstring>
#include <iostream>


// Step 1 (Conceptual): We identify the core dependencies needed for testing sqlite3BinfoRegister.
// - sqlite3 *db: the database handle passed to sqlite3_create_module.
// - sqlite3_module: the module descriptor passed to sqlite3_create_module (we do not inspect its internal fields here).
// - sqlite3_create_module: the API call invoked by sqlite3BinfoRegister to register the module.
// The focal function itself does not branch; it simply constructs a static module descriptor and calls sqlite3_create_module.
// Our test focuses on ensuring sqlite3_create_module is invoked with expected parameters and return value propagation.


// Step 2 (Test scaffolding): Provide minimal forward declarations so we can declare/define the test harness
// without requiring the full SQLite headers. We only need the types to exist for the function signatures.
struct sqlite3;           // Forward declaration to represent sqlite3 database handle
struct sqlite3_module;      // Forward declaration for the module descriptor

// Forward declare the function under test. The real implementation is in btreeinfo.c.
extern "C" int sqlite3BinfoRegister(struct sqlite3* db);

// Provide a lightweight mock of the SQLite extension API that sqlite3BinfoRegister will call.
// We implement sqlite3_create_module with C linkage so it matches the expected symbol.
extern "C" {

// Global hook to capture the last call parameters for verification.
struct LastModuleCall {
  struct sqlite3* db = nullptr;
  const char* zName = nullptr;
  const void* pModule = nullptr; // casted from const sqlite3_module*
  void* pAux = nullptr;
  int returnValue = -1;
} g_lastCall;

// Our mock implementation of sqlite3_create_module.
// It records the arguments and returns 0 if db is non-null, -1 otherwise (to simulate error on null db).
int sqlite3_create_module(struct sqlite3* db,
                          const char* zName,
                          const struct sqlite3_module* pModule,
                          void* pAux)
{
  g_lastCall.db = db;
  g_lastCall.zName = zName;
  g_lastCall.pModule = static_cast<const void*>(pModule);
  g_lastCall.pAux = pAux;
  g_lastCall.returnValue = (db == nullptr) ? -1 : 0;
  return g_lastCall.returnValue;
}
} // extern "C"

// We need a concrete (but minimal) sqlite3 object to pass to sqlite3BinfoRegister in tests.
// The actual internals are not used by our tests; we only verify that the mock receives expected
// values. A simple dummy struct instance suffices.
struct sqlite3Dummy {
  int placeholder;
};

// Expose a small wrapper to trigger tests from main() without any testing framework.
// This wrapper is intentionally lightweight and uses non-terminating checks (EXPECT_* style) to
// maximize code execution for coverage.
namespace TestHarness {

  // Non-terminating assertion helpers (do not exit on failure).
  static void EXPECT_EQ(int a, int b, const char* msg = "") {
    if (a != b) {
      std::cerr << "EXPECT_EQ failed: " << a << " != " << b;
      if (msg && std::strlen(msg) > 0) std::cerr << "  [" << msg << "]";
      std::cerr << std::endl;
    }
  }

  static void EXPECT_NE(const void* a, const void* b, const char* msg = "") {
    if (a == b) {
      std::cerr << "EXPECT_NE failed: " << a << " == " << b;
      if (msg && std::strlen(msg) > 0) std::cerr << "  [" << msg << "]";
      std::cerr << std::endl;
    }
  }

  // Test 1: Normal case - non-null db should cause sqlite3_create_module to be called and return 0.
  bool test_NormalCall() {
    g_lastCall = LastModuleCall(); // Reset mock state (default-initialized)
    sqlite3Dummy dummy;
    // Call the function under test with a non-null db pointer
    int ret = sqlite3BinfoRegister(reinterpret_cast<struct sqlite3*>(&dummy));

    // Verify return value indicates success (as per our mock contract)
    EXPECT_EQ(ret, 0, "NormalCall: return value should be 0 for non-null db");

    // Verify that the mock received a non-null db, the expected module name, and a non-null module pointer
    EXPECT_NE(reinterpret_cast<void*>(g_lastCall.db), reinterpret_cast<void*>(nullptr), "NormalCall: db should be non-null");
    if (g_lastCall.zName) {
      EXPECT_EQ(std::strcmp(g_lastCall.zName, "sqlite_btreeinfo"), 0, "NormalCall: module name should be 'sqlite_btreeinfo'");
    } else {
      std::cerr << "NormalCall: zName was null" << std::endl;
    }
    EXPECT_NE(g_lastCall.pModule, reinterpret_cast<const void*>(nullptr), "NormalCall: pModule should be non-null");
    EXPECT_EQ(g_lastCall.pAux, nullptr, "NormalCall: pAux should be null");

    // Return true if all expectations passed (best effort; non-terminating)
    return (ret == 0) &&
           (g_lastCall.db != nullptr) &&
           (g_lastCall.zName != nullptr) &&
           (g_lastCall.pModule != nullptr) &&
           (g_lastCall.pAux == nullptr);
  }

  // Test 2: Null db - should propagate error from sqlite3_create_module (mock returns -1).
  bool test_NullDb() {
    g_lastCall = LastModuleCall();
    int ret = sqlite3BinfoRegister(nullptr);

    // Verify return value indicates failure in the null-db scenario
    EXPECT_EQ(ret, -1, "NullDb: return value should be -1 for null db");
    // When db is null, mock should reflect that
    EXPECT_EQ(static_cast<int>(g_lastCall.db == nullptr), 1, "NullDb: db should be null in mock");

    return (ret == -1) && (g_lastCall.db == nullptr);
  }

  // Compile-time-friendly container to run all tests sequentially.
  void runAll() {
    bool ok = true;
    ok &= test_NormalCall();
    ok &= test_NullDb();

    if (ok) {
      std::cout << "All tests (non-terminating assertions) passed for sqlite3BinfoRegister." << std::endl;
    } else {
      std::cout << "Some tests reported failures (non-terminating assertions) for sqlite3BinfoRegister." << std::endl;
    }
  }

  // Helper to reset struct as needed (not strictly required due to per-test resets).
  struct LastModuleCall {
    sqlite3* db = nullptr;
    const char* zName = nullptr;
    const void* pModule = nullptr;
    void* pAux = nullptr;
    int returnValue = -1;
  };

} // namespace TestHarness

int main() {
  // Run the lightweight, non-GTest test suite for sqlite3BinfoRegister.
  TestHarness::runAll();
  return 0;
}
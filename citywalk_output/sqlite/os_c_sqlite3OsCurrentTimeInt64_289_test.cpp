// Comprehensive C++11 test suite for sqlite3OsCurrentTimeInt64 (os.c) without GTest.
// This test uses a lightweight, non-terminating assertion mechanism and a minimal fake
// sqlite3_vfs to cover both branches of sqlite3OsCurrentTimeInt64.
// The tests are designed to be linked against the C implementation in os.c.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Minimal declarations to mirror the SQLite-compatible types used by sqlite3OsCurrentTimeInt64.
// We do not include any external SQLite headers to keep the test self-contained.
typedef long long sqlite3_int64;

// Forward declaration of the focal function with C linkage (as implemented in os.c)
extern "C" int sqlite3OsCurrentTimeInt64(struct sqlite3_vfs *pVfs, sqlite3_int64 *pTimeOut);

// Lightweight, non-terminating assertion macro
static int g_failures = 0;
#define CHECK(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "[FAIL] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
      ++g_failures; \
    } \
  } while(0)


// Minimal representation of the sqlite3_vfs structure used by the focal method.
// We declare fields exactly as accessed by sqlite3OsCurrentTimeInt64.
struct sqlite3_vfs {
  int iVersion;
  // Pointers to the time-getting hooks (matching the sqlite3_vfs API)
  int (*xCurrentTimeInt64)(struct sqlite3_vfs *, sqlite3_int64 *);
  int (*xCurrentTime)(struct sqlite3_vfs *, double *);
};

// Forward declarations of test harness fake implementations (with C linkage)
extern "C" int kh_xCurrentTimeInt64_pass1(struct sqlite3_vfs *pVfs, sqlite3_int64 *pTimeOut);
extern "C" int kh_xCurrentTimeInt64_fail(struct sqlite3_vfs *pVfs, sqlite3_int64 *pTimeOut);
extern "C" int kh_xCurrentTime_pass(struct sqlite3_vfs *pVfs, double *pTimeOut);
extern "C" int kh_xCurrentTime_pass2(struct sqlite3_vfs *pVfs, double *pTimeOut);

// Test 1: Branch where iVersion >= 2 and xCurrentTimeInt64 is non-null.
// The focal function should delegate to xCurrentTimeInt64 and propagate its rc and value.
extern "C" int kh_xCurrentTimeInt64_pass1(struct sqlite3_vfs *pVfs, sqlite3_int64 *pTimeOut) {
  if(pTimeOut) {
    *pTimeOut = 12345; // sentinel value for verification
  }
  return 0; // rc
}

// Test 2: Branch where iVersion >= 2 but xCurrentTimeInt64 is NULL; use xCurrentTime path.
// The test fake will provide r = 12.34 days; expected pTimeOut = 12.34 * 86400000
extern "C" int kh_xCurrentTime_pass(struct sqlite3_vfs *pVfs, double *pTimeOut) {
  if(pTimeOut) {
    *pTimeOut = 12.34; // days
  }
  return 1; // rc
}

// Test 3: Branch where iVersion < 2; even if xCurrentTimeInt64 exists, xCurrentTime is used.
// The test fake will provide r = 7.5 days.
extern "C" int kh_xCurrentTime_pass2(struct sqlite3_vfs *pVfs, double *pTimeOut) {
  if(pTimeOut) {
    *pTimeOut = 7.5; // days
  }
  return 2; // rc
}

// Test 4: Branch where xCurrentTimeInt64 exists and returns an error rc.
// We verify both rc and that pTimeOut reflects the value set by the fake.
extern "C" int kh_xCurrentTimeInt64_fail(struct sqlite3_vfs *pVfs, sqlite3_int64 *pTimeOut) {
  if(pTimeOut) {
    *pTimeOut = 999; // sentinel to verify propagation
  }
  return -1; // error rc
}


// Helper function to run the 4 test cases
static bool run_all_tests() {
  // Test 1 setup
  {
    sqlite3_vfs vfs1;
    vfs1.iVersion = 2;
    vfs1.xCurrentTimeInt64 = &kh_xCurrentTimeInt64_pass1;
    vfs1.xCurrentTime = nullptr;

    sqlite3_int64 outTime = 0;
    int rc = sqlite3OsCurrentTimeInt64(&vfs1, &outTime);

    CHECK(rc == 0, "Test1: rc should be 0 (from xCurrentTimeInt64)");
    CHECK(outTime == 12345, "Test1: outTime should be the value provided by xCurrentTimeInt64 (12345)");
  }

  // Test 2 setup
  {
    sqlite3_vfs vfs2;
    vfs2.iVersion = 2;
    vfs2.xCurrentTimeInt64 = nullptr;
    vfs2.xCurrentTime = &kh_xCurrentTime_pass;

    sqlite3_int64 outTime = 0;
    int rc = sqlite3OsCurrentTimeInt64(&vfs2, &outTime);

    // expected = (sqlite3_int64)(12.34 * 86400000.0)
    sqlite3_int64 expected = (sqlite3_int64)(12.34 * 86400000.0);
    CHECK(rc == 1, "Test2: rc should be 1 (from xCurrentTime)");
    CHECK(outTime == expected, "Test2: outTime should be floor(12.34 days) in ms");
  }

  // Test 3 setup
  {
    sqlite3_vfs vfs3;
    vfs3.iVersion = 1; // less than 2 to force xCurrentTime path
    vfs3.xCurrentTimeInt64 = &kh_xCurrentTimeInt64_fail; // should be ignored
    vfs3.xCurrentTime = &kh_xCurrentTime_pass2;

    sqlite3_int64 outTime = 0;
    int rc = sqlite3OsCurrentTimeInt64(&vfs3, &outTime);

    sqlite3_int64 expected = (sqlite3_int64)(7.5 * 86400000.0);
    CHECK(rc == 2, "Test3: rc should be 2 (from xCurrentTime)");
    CHECK(outTime == expected, "Test3: outTime should reflect 7.5 days in ms");
  }

  // Test 4 setup
  {
    sqlite3_vfs vfs4;
    vfs4.iVersion = 2;
    vfs4.xCurrentTimeInt64 = &kh_xCurrentTimeInt64_fail;
    vfs4.xCurrentTime = nullptr;

    sqlite3_int64 outTime = -1;
    int rc = sqlite3OsCurrentTimeInt64(&vfs4, &outTime);

    CHECK(rc == -1, "Test4: rc should be -1 (from failing xCurrentTimeInt64)");
    CHECK(outTime == 999, "Test4: outTime should be the sentinel set by failing xCurrentTimeInt64");
  }

  return g_failures == 0;
}

// Entry point for the test executable
int main() {
  bool all_ok = run_all_tests();

  if(all_ok) {
    std::cout << "[PASS] All tests passed for sqlite3OsCurrentTimeInt64." << std::endl;
    return 0;
  } else {
    std::cerr << "[ERROR] Some tests failed. Failures: " << g_failures << std::endl;
    return 1;
  }
}
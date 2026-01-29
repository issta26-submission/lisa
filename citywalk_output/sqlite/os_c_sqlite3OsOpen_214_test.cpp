// Unit tests for the focal method sqlite3OsOpen
// This test suite re-creates the minimal dependencies needed to
// exercise sqlite3OsOpen in a self-contained C++11 test harness
// without relying on external testing frameworks (GTest, etc.).
// The tests verify key behavior: the masking of flags passed to the VFS
// and handling of the exclusive-path assertion predicate when zPath is NULL.
//
// Note: We re-implement a minimal replica of the relevant interfaces and
// constants to keep the test self-contained and compilable in C++11.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Minimal re-declarations to mimic sqlite3 types used by sqlite3OsOpen.
// These definitions are purpose-built for the test harness and do not
// represent the complete SQLite API.

typedef struct sqlite3_io_methods sqlite3_io_methods;
typedef struct sqlite3_file sqlite3_file;
typedef struct sqlite3_vfs sqlite3_vfs;

struct sqlite3_file {
    sqlite3_io_methods *pMethods; // Non-null to simulate an open file
};

// A minimal VFS structure with only the xOpen method needed by sqlite3OsOpen
struct sqlite3_vfs {
    int (*xOpen)(sqlite3_vfs*, const char*, sqlite3_file*, int, int*);
};

// Constants and flags used by sqlite3OsOpen
#define SQLITE_OK 0
#define SQLITE_OPEN_EXCLUSIVE 0x00000010
// The mask of flags that are passed down to the VFS in sqlite3OsOpen.
// This mirrors the value used in the original SQLite code.
#define SQLITE_OS_OPEN_FLAGS_MASK 0x1087f7f

// Define a no-op macro for the DO_OS_MALLOC_TEST
#define DO_OS_MALLOC_TEST(_ignored) ((void)0)

// Minimal replica of sqlite3OsOpen as described in the focal method.
// This is implemented to be used directly in the test harness.
int sqlite3OsOpen(
  sqlite3_vfs *pVfs,
  const char *zPath,
  sqlite3_file *pFile,
  int flags,
  int *pFlagsOut
){
  int rc;
  DO_OS_MALLOC_TEST(0);
  /* 0x87f7f is a mask of SQLITE_OPEN_ flags that are valid to be passed
  ** down into the VFS layer. Some SQLITE_OPEN_ flags (for example,
  ** SQLITE_OPEN_FULLMUTEX or SQLITE_OPEN_SHAREDCACHE) are blocked before
  ** reaching the VFS. */
  assert( zPath || (flags & SQLITE_OPEN_EXCLUSIVE) );
  rc = pVfs->xOpen(pVfs, zPath, pFile, flags & SQLITE_OS_OPEN_FLAGS_MASK, pFlagsOut);
  assert( rc==SQLITE_OK || pFile->pMethods==0 );
  return rc;
}

// ------------------- Test Harness -------------------

static int g_lastOpenFlags = -1;

// A test double for the VFS xOpen callback. It records the flags
// it receives to verify correct masking by sqlite3OsOpen.
// It always returns SQLITE_OK to avoid triggering rc assertion.
static int test_xOpen(sqlite3_vfs* pvfs, const char* zPath, sqlite3_file* pFile, int flags, int* pFlagsOut) {
    (void)pvfs;    // Unused in this test double
    (void)zPath;     // We only validate flag masking, not path content
    (void)pFlagsOut; // We'll not modify pFlagsOut in this test double
    g_lastOpenFlags = flags;
    // In a real environment, pFlagsOut would be set by the VFS if needed.
    // We keep it simple here.
    return SQLITE_OK;
}

// Utility to create a dummy non-null file methods pointer
static sqlite3_io_methods dummyIoMethods;
static sqlite3_file dummyFile;

// Test 1: Normal open path (zPath is non-null) with consistent masking
bool Test_Open_Normal_Path() {
    // Prepare VFS with our test double
    sqlite3_vfs testVfs;
    testVfs.xOpen = test_xOpen;

    // Prepare a dummy file with non-null pMethods to satisfy the assertion
    dummyFile.pMethods = &dummyIoMethods;

    g_lastOpenFlags = -1; // reset

    const char* zPath = "/tmp/test.db";
    int flags = 0x01020304; // some arbitrary flags; mask will apply
    int outFlags = -1;

    int rc = sqlite3OsOpen(&testVfs, zPath, &dummyFile, flags, &outFlags);

    // Expected: rc == SQLITE_OK, and the xOpen should have been called with
    // (flags & SQLITE_OS_OPEN_FLAGS_MASK)
    int expectedFlags = flags & 0x1087f7f;

    bool ok = (rc == SQLITE_OK) && (g_lastOpenFlags == expectedFlags);
    if (!ok) {
        std::cerr << "Test_Open_Normal_Path failed: rc=" << rc
                  << ", lastFlags=" << g_lastOpenFlags
                  << ", expected=" << expectedFlags << std::endl;
    }

    return ok;
}

// Test 2: zPath is NULL but EXCLUSIVE bit is set (satisfies the assert predicate)
// Ensure flags are masked and passed to xOpen correctly.
bool Test_Open_NullPath_With_Exclusive() {
    // Prepare VFS with our test double
    sqlite3_vfs testVfs;
    testVfs.xOpen = test_xOpen;

    // Non-null pMethods to avoid rc assertion
    dummyFile.pMethods = &dummyIoMethods;

    g_lastOpenFlags = -1; // reset

    const char* zPath = NULL;
    int flags = SQLITE_OPEN_EXCLUSIVE | 0x00000004; // include exclusive bit
    int outFlags = -1;

    int rc = sqlite3OsOpen(&testVfs, zPath, &dummyFile, flags, &outFlags);

    int expectedFlags = flags & 0x1087f7f;

    bool ok = (rc == SQLITE_OK) && (g_lastOpenFlags == expectedFlags);
    if (!ok) {
        std::cerr << "Test_Open_NullPath_With_Exclusive failed: rc=" << rc
                  << ", lastFlags=" << g_lastOpenFlags
                  << ", expected=" << expectedFlags << std::endl;
    }

    return ok;
}

// ------------------- Main Test Runner -------------------

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running unit tests for sqlite3OsOpen (self-contained harness)" << std::endl;

    // Test 1
    total++;
    if (Test_Open_Normal_Path()) {
        passed++;
        std::cout << "[PASS] Test_Open_Normal_Path" << std::endl;
    } else {
        std::cout << "[FAIL] Test_Open_Normal_Path" << std::endl;
    }

    // Test 2
    total++;
    if (Test_Open_NullPath_With_Exclusive()) {
        passed++;
        std::cout << "[PASS] Test_Open_NullPath_With_Exclusive" << std::endl;
    } else {
        std::cout << "[FAIL] Test_Open_NullPath_With_Exclusive" << std::endl;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
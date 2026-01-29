// Lightweight C++11 test suite for sqlite3VdbeMemSetRowSet
// This test suite targets the focal method from SQLite's VDBE memory utilities.
// It uses direct C interfaces from SQLite internal headers (accessible in a
// compiled SQLite source-tree or linked library with internal headers).
// No Google Test is used; tests are executed from main() with a small harness.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Ensure C linkage for SQLite internal APIs when included from C++
extern "C" {
  // Public API to create an in-memory DB for test setup
  #include <sqlite3.h>

  // Internal headers (may be provided in the same repository or test environment)
  // These headers declare Mem, RowSet, and internal helpers used by sqlite3VdbeMemSetRowSet
  #include "sqliteInt.h"
  #include "vdbeInt.h"

  // Forward declarations for internal functions used in tests
  // If these are already declared in the included headers, this block is harmless.
  // Do not rely on GTest; we implement a small harness around these calls.
  // Note: The actual function signatures come from the internal SQLite sources.
  int sqlite3VdbeMemIsRowSet(Mem *pMem);
  void sqlite3VdbeMemRelease(Mem *pMem);

  // The function under test
  int sqlite3VdbeMemSetRowSet(Mem *pMem);
  // Destructor used for RowSet memory
  void sqlite3RowSetDelete(void*);
  // RowSet type is defined in vdbeInt.h
  typedef struct RowSet RowSet;
  // RowSetInit returns a RowSet* or 0 on OOM
  RowSet *sqlite3RowSetInit(sqlite3 *db);
  // The destructor used for RowSet memory (as xDel)
  // (We rely on sqlite3RowSetDelete above)
}

// Simple test harness (non-terminating checks)
static int g_failures = 0;

static void record_failure(const char* note) {
    std::cerr << "TEST FAILURE: " << note << std::endl;
    ++g_failures;
}

#define CHECK(cond, note) \
    do { if(!(cond)) { record_failure(note); } } while(0)

static void test_sqlite3VdbeMemSetRowSet_success() {
    // Test that sqlite3VdbeMemSetRowSet correctly initializes a RowSet
    // and wires up Mem fields as expected when preconditions are satisfied.

    // Open an in-memory database for test isolation
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    CHECK(db != nullptr && rc == SQLITE_OK, "Unable to open in-memory DB for test");

    if(db == nullptr || rc != SQLITE_OK) {
        // Cannot proceed with this test without a valid DB
        return;
    }

    // Prepare Mem object
    Mem mem;
    std::memset(&mem, 0, sizeof(mem));
    mem.db = db;

    // Ensure preconditions: not already a RowSet
    // (sqlite3VdbeMemIsRowSet should return false for a fresh Mem)
    bool isRowSetBefore = sqlite3VdbeMemIsRowSet(&mem);
    CHECK(!isRowSetBefore, "Precondition: Mem should not be a RowSet before test");

    // Call the function under test
    int result = sqlite3VdbeMemSetRowSet(&mem);
    CHECK(result == SQLITE_OK, "sqlite3VdbeMemSetRowSet should return SQLITE_OK on success");

    // Post-conditions
    // mem.z should point to a RowSet
    RowSet* pRowSet = reinterpret_cast<RowSet*>(mem.z);
    CHECK(pRowSet != nullptr, "Mem.z should point to a non-null RowSet after SetRowSet");

    // Mem flags should be MEM_Blob | MEM_Dyn
    // Use bit-checks since exact value may be a combination
    CHECK((mem.flags & MEM_Blob) != 0, "Mem.flags should include MEM_Blob after SetRowSet");
    CHECK((mem.flags & MEM_Dyn) != 0, "Mem.flags should include MEM_Dyn after SetRowSet");

    // Destructor for the allocated RowSet should be sqlite3RowSetDelete
    CHECK(mem.xDel == sqlite3RowSetDelete, "Mem.xDel should be sqlite3RowSetDelete after SetRowSet");

    // The Mem should still reference the same db pointer
    CHECK(mem.db == db, "Mem.db should remain pointing to the same sqlite3* after SetRowSet");

    // Clean up: Release the Mem (which should free the RowSet via the xDel)
    // Do not explicitly free because sqlite3VdbeMemRelease will call xDel on mem.z
    sqlite3VdbeMemRelease(&mem);

    // Close DB
    sqlite3_close(db);
}

static void test_sqlite3VdbeMemSetRowSet_releasesOldMemory() {
    // Test that existing memory in Mem.z is released by sqlite3VdbeMemRelease
    // before setting a new RowSet, ensuring the old pointer is freed by the
    // old destructor (which we override to count frees).

    static int g_freedOldPtrCount = 0;
    auto myFree = [](void* p) {
        // Count how many times the old memory is freed and actually free it
        ++g_freedOldPtrCount;
        free(p);
    };

    // Open in-memory DB
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    CHECK(db != nullptr && rc == SQLITE_OK, "Unable to open in-memory DB for test 2");

    if(db == nullptr || rc != SQLITE_OK) {
        return;
    }

    // Prepare Mem with pre-existing allocation
    Mem mem;
    std::memset(&mem, 0, sizeof(mem));
    mem.db = db;

    // Allocate some memory to mimic existing content
    void* oldPtr = malloc(32);
    if(oldPtr == nullptr) {
        record_failure("malloc failed for oldPtr in test 2");
        sqlite3_close(db);
        return;
    }
    // Set pre-existing content
    mem.z = (char*)oldPtr;
    mem.flags = MEM_Blob | MEM_Dyn;
    mem.xDel = myFree; // custom destructor to track frees

    // Ensure preconditions: Mem is not yet a RowSet
    CHECK(!sqlite3VdbeMemIsRowSet(&mem), "Precondition: Mem should not be a RowSet before test 2");

    // Call the function under test
    int result = sqlite3VdbeMemSetRowSet(&mem);
    CHECK(result == SQLITE_OK, "sqlite3VdbeMemSetRowSet should return SQLITE_OK in test 2");

    // After call, oldPtr should have been freed using myFree
    CHECK(g_freedOldPtrCount == 1, "Old memory should be freed once via the custom destructor");

    // mem.z should now point to a RowSet
    RowSet* pRowSet = reinterpret_cast<RowSet*>(mem.z);
    CHECK(pRowSet != nullptr, "Mem.z should point to a RowSet after SetRowSet in test 2");

    // mem.xDel should be updated to sqlite3RowSetDelete
    CHECK(mem.xDel == sqlite3RowSetDelete, "Mem.xDel should be sqlite3RowSetDelete after SetRowSet in test 2");

    // membranes db pointer should still be valid
    CHECK(mem.db == db, "Mem.db should remain valid in test 2");

    // Release memory to avoid leaks
    sqlite3VdbeMemRelease(&mem);

    // Cleanup DB
    sqlite3_close(db);
}

// Entry point
int main() {
    std::cout << "Starting sqlite3VdbeMemSetRowSet unit tests (C++ harness)" << std::endl;

    test_sqlite3VdbeMemSetRowSet_success();
    test_sqlite3VdbeMemSetRowSet_releasesOldMemory();

    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
// C++11 unit tests for SQLite-like function: sqlite3_create_collation16
// This test suite is self-contained and does not rely on GTest.
// It provides minimal stubs of the required SQLite-like environment to exercise
// the focal method and its branches.
// Each test case is documented with comments explaining the intent and expectations.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain knowledge: use only standard library, avoid private/internal details from
// the original project. We simulate necessary components.

// Public constants (stand-ins for SQLite return codes)
#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_MISUSE_BKPT 2

// Encoding enum placeholder
#define SQLITE_UTF16NATIVE 1

// Forward declaration of the focal function (as implemented in this test harness)
extern "C" int sqlite3_create_collation16(
  struct sqlite3* db,
  const void *zName,
  int enc,
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*)
);

// Lightweight, test-oriented SQLite-like environment (in-test definitions)

typedef unsigned char u8;

// Simple mutex-like structure (no real threading)
struct sqlite3_mutex {
    int dummy;
};

// SQLite-like database structure used by the focal function
struct sqlite3 {
    sqlite3_mutex* mutex;
    bool mallocFailed;
};

// Global test state to verify interactions inside the focal method
static int g_safetyOkFlag = 1;                 // Controls sqlite3SafetyCheckOk result
static int g_mutexEnterCount = 0;
static int g_mutexLeaveCount = 0;
static int g_apiExitRC = SQLITE_OK;
static char* g_last_allocatedName8 = nullptr;   // zName8 allocated by sqlite3Utf16to8
static void* g_last_freed_name8 = nullptr;      // pointer freed by sqlite3DbFree
static const char* g_last_name_in_collation = nullptr;
static unsigned char g_last_enc_value = 0;
static void* g_last_pCtx = nullptr;
static int (*g_last_xCompare)(void*,int,const void*,int,const void*) = nullptr;
static void* g_last_xDel = nullptr;

// Forward declarations of test-supporting helpers (C linkage to mimic SQLite API)
extern "C" int sqlite3SafetyCheckOk(struct sqlite3*);      // armor check
extern "C" void sqlite3_mutex_enter(void*);               // mutex enter counter
extern "C" void sqlite3_mutex_leave(void*);               // mutex leave counter
extern "C" char* sqlite3Utf16to8(struct sqlite3*, const void*, int, int);
extern "C" void sqlite3DbFree(struct sqlite3*, void*);
extern "C" int sqlite3ApiExit(struct sqlite3*, int);
extern "C" int sqlite3CreateFunc_dummy(void);             // unused placeholder

// Implementations of the helper functions to drive the focal method

// Armor check stub: returns based on g_safetyOkFlag
extern "C" int sqlite3SafetyCheckOk(struct sqlite3* db){
    (void)db; // unused in test stub
    return g_safetyOkFlag ? 1 : 0;
}

// Mutex enter increments a counter for verification
extern "C" void sqlite3_mutex_enter(void*){
    ++g_mutexEnterCount;
}

// Mutex leave increments a counter for verification
extern "C" void sqlite3_mutex_leave(void*){
    ++g_mutexLeaveCount;
}

// UTF-16 to UTF-8 conversion stub
// For testing, zName is provided as a regular C-string; we copy it to a new buffer.
extern "C" char* sqlite3Utf16to8(struct sqlite3* db, const void* zName, int n, int enc){
    (void)db; (void)n; (void)enc;
    if(zName == nullptr) return nullptr;
    const char* src = static_cast<const char*>(zName);
    // Simulate a failure path for specific sentinel value "bad" to exercise NULL return
    if(std::strcmp(src, "bad") == 0){
        return nullptr;
    }
    // Allocate and copy the string to emulate SQLite behavior
    size_t len = std::strlen(src);
    char* dst = static_cast<char*>(std::malloc(len + 1));
    if(dst){
        std::strcpy(dst, src);
        g_last_allocatedName8 = dst;
    }
    return dst;
}

// Free memory allocated by sqlite3Utf16to8
extern "C" void sqlite3DbFree(struct sqlite3* db, void* p){
    (void)db;
    g_last_freed_name8 = p;
    std::free(p);
}

// Api exit hook to capture final rc
extern "C" int sqlite3ApiExit(struct sqlite3* db, int rc){
    (void)db;
    g_apiExitRC = rc;
    return rc;
}

// Static storage for createCollation simulation
static const char* g_last_name_for_collation = nullptr;

// Simulated internal "createCollation" used by sqlite3_create_collation16
static int createCollation(
  struct sqlite3* db,
  const char *zName,
  u8 enc,
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*),
  void(*xDel)(void*)
){
    (void)db; (void)xDel; // not used in test
    g_last_name_in_collation = zName;
    g_last_enc_value = enc;
    g_last_pCtx = pCtx;
    g_last_xCompare = xCompare;
    g_last_xDel = xDel;
    // Special case to simulate error propagation when name is "error"
    if(zName && std::strcmp(zName, "error") == 0){
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}

// The focal function under test (re-implemented to be self-contained for the test)
extern "C" int sqlite3_create_collation16(
  struct sqlite3* db,
  const void *zName,
  int enc,
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*)
){
{
  int rc = SQLITE_OK;
  char *zName8;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) || zName==0 ) return SQLITE_MISUSE_BKPT;
#endif
  sqlite3_mutex_enter(db->mutex);
  assert( !db->mallocFailed );
  zName8 = sqlite3Utf16to8(db, zName, -1, SQLITE_UTF16NATIVE);
  if( zName8 ){
    rc = createCollation(db, zName8, (u8)enc, pCtx, xCompare, 0);
    sqlite3DbFree(db, zName8);
  }
  rc = sqlite3ApiExit(db, rc);
  sqlite3_mutex_leave(db->mutex);
  return rc;
}
}

// Test framework utilities

static int g_test_count = 0;
static int g_test_failed = 0;

#define TEST_CASE(name) void name()
#define ASSERT_TRUE(cond, msg) \
    do { if(!(cond)){ std::cout << "[FAILED] " << msg << " (line " << __LINE__ << ")\n"; ++g_test_failed; } else { /* pass */ } } while(0)
#define ASSERT_EQ(a,b, msg) \
    do { if((a) != (b)){ std::cout << "[FAILED] " << msg << " Expected: " << (a) << " Got: " << (b) << " (line " << __LINE__ << ")\n"; ++g_test_failed; } } while(0)
#define ASSERT_PTR_EQ(a,b, msg) \
    do { if((void*)(a) != (void*)(b)){ std::cout << "[FAILED] " << msg << " Pointers differ. Expected: " << a << " Got: " << b << " (line " << __LINE__ << ")\n"; ++g_test_failed; } } while(0)
#define ASSERT_NOT_NULL(p, msg) \
    do { if((p) == nullptr){ std::cout << "[FAILED] " << msg << " is null (line " << __LINE__ << ")\n"; ++g_test_failed; } } while(0)
#define ASSERT_NULL(p, msg) \
    do { if((p) != nullptr){ std::cout << "[FAILED] " << msg << " is not null (line " << __LINE__ << ")\n"; ++g_test_failed; } } while(0)


// Helpers to construct and destroy a minimal sqlite3 database object for tests
static sqlite3* makeTestDb(){
    sqlite3* db = static_cast<sqlite3*>(std::malloc(sizeof(sqlite3)));
    db->mallocFailed = false;
    db->mutex = static_cast<sqlite3_mutex*>(std::malloc(sizeof(sqlite3_mutex)));
    db->mutex->dummy = 0;
    return db;
}

static void freeTestDb(sqlite3* db){
    if(db){
        if(db->mutex){
            std::free(db->mutex);
        }
        std::free(db);
    }
}

// Reset global state between tests
static void resetGlobalsForTest(){
    g_safetyOkFlag = 1;
    g_mutexEnterCount = 0;
    g_mutexLeaveCount = 0;
    g_apiExitRC = SQLITE_OK;
    if(g_last_allocatedName8){
        std::free(g_last_allocatedName8);
        g_last_allocatedName8 = nullptr;
    }
    g_last_freed_name8 = nullptr;
    g_last_name_in_collation = nullptr;
    g_last_enc_value = 0;
    g_last_pCtx = nullptr;
    g_last_xCompare = nullptr;
    g_last_xDel = nullptr;
}

// Test cases

TEST_CASE(test_normal_path){
    resetGlobalsForTest();

    // Arrange
    sqlite3* db = makeTestDb();
    // Provide non-null zName; the test uses a simple C-string as input
    const char* collName = "mycollation";
    int enc = 1;
    void* pCtx = (void*)0x1234;
    auto cmp = [](void*, int, const void*, int, const void*) -> int { return 0; };

    // Act
    int rc = sqlite3_create_collation16(db, collName, enc, pCtx, (int(*)(void*,int,const void*,int,const void*))cmp);

    // Assert
    ASSERT_EQ(rc, SQLITE_OK, "Expected SQLITE_OK from normal path");
    ASSERT_EQ(g_mutexEnterCount, 1, "mutex enter should be called exactly once");
    ASSERT_EQ(g_mutexLeaveCount, 1, "mutex leave should be called exactly once");
    ASSERT_NOT_NULL(g_last_allocatedName8, "zName8 should be allocated");
    ASSERT_PTR_EQ(g_last_pCtx, pCtx, "pCtx should be preserved in createCollation");
    ASSERT_PTR_EQ((void*)g_last_xCompare, (void*)cmp, "xCompare should be stored");
    // zName8 freed
    ASSERT_NOT_NULL(g_last_freed_name8, "zName8 should be freed");
    ASSERT_PTR_EQ(g_last_freed_name8, g_last_allocatedName8, "freed pointer should match allocated pointer");
    // Collation data should be recorded
    ASSERT_PTR_EQ((void*)g_last_name_in_collation, (void*)collName, "zName should be passed to createCollation");
    ASSERT_EQ(g_last_enc_value, static_cast<u8>(enc), "Encoding should be passed through as u8");

    freeTestDb(db);
    resetGlobalsForTest();
}

TEST_CASE(test_safety_check_fail){
    resetGlobalsForTest();

    // Arrange
    sqlite3* db = makeTestDb();
    g_safetyOkFlag = 0; // armor fails
    const char* collName = "name_should_not_exist";

    // Act
    int rc = sqlite3_create_collation16(db, collName, 1, nullptr, nullptr);

    // Assert
    ASSERT_EQ(rc, SQLITE_MISUSE_BKPT, "Armor failure should return MISUSE_BKPT");
    ASSERT_EQ(g_mutexEnterCount, 0, "mutex should not be entered on armor failure");
    ASSERT_NULL(g_last_allocatedName8, "No allocation should occur on armor failure");
    ASSERT_NULL(g_last_freed_name8, "No free should occur on armor failure");

    freeTestDb(db);
    resetGlobalsForTest();
}

TEST_CASE(test_null_name_armored){
    resetGlobalsForTest();

    // Arrange
    sqlite3* db = makeTestDb();
    g_safetyOkFlag = 1;
    const void* zName = nullptr;

    // Act
    int rc = sqlite3_create_collation16(db, zName ? (const void*)zName : nullptr, 1, nullptr, nullptr);

    // Assert
    ASSERT_EQ(rc, SQLITE_MISUSE_BKPT, "Null zName should trigger MISUSE_BKPT");
    ASSERT_EQ(g_mutexEnterCount, 0, "mutex should not be entered when zName is null");

    freeTestDb(db);
    resetGlobalsForTest();
}

TEST_CASE(test_utf16_conversion_failure){
    resetGlobalsForTest();

    // Arrange
    sqlite3* db = makeTestDb();
    const char* collName = "bad"; // triggers NULL from utf16to8 in our stub
    int enc = 1;

    // Act
    int rc = sqlite3_create_collation16(db, collName, enc, nullptr, nullptr);

    // Assert
    // When utf16to8 returns NULL, there is no createCollation and rc should be SQLITE_OK
    ASSERT_EQ(rc, SQLITE_OK, "Without zName8, rc should remain OK");
    // mutex interactions still occur though
    ASSERT_EQ(g_mutexEnterCount, 1, "mutex should still be entered even if zName8 is NULL");
    ASSERT_EQ(g_mutexLeaveCount, 1, "mutex should be left after operation");
    ASSERT_NULL(g_last_allocatedName8, "No allocation should occur for invalid utf16 input");
    ASSERT_NULL(g_last_freed_name8, "No free should occur because there was no allocation");

    freeTestDb(db);
    resetGlobalsForTest();
}

TEST_CASE(test_create_collation_error_propagation){
    resetGlobalsForTest();

    // Arrange
    sqlite3* db = makeTestDb();
    const char* collName = "error"; // triggers simulated error in createCollation
    int enc = 1;
    auto cmp = [](void*, int, const void*, int, const void*) -> int { return 0; };

    // Act
    int rc = sqlite3_create_collation16(db, collName, enc, nullptr, (int(*)(void*,int,const void*,int,const void*))cmp);

    // Assert
    ASSERT_EQ(rc, SQLITE_ERROR, "Error in createCollation should propagate as SQLITE_ERROR");
    // zName8 should have been allocated and freed
    ASSERT_NOT_NULL(g_last_allocatedName8, "zName8 should be allocated before error");
    ASSERT_NOT_NULL(g_last_freed_name8, "zName8 should be freed even on error");
    ASSERT_PTR_EQ((void*)g_last_freed_name8, (void*)g_last_allocatedName8, "Freed pointer should match allocated pointer");

    // Mutex usage still occurs
    ASSERT_EQ(g_mutexEnterCount, 1, "mutex enter should occur once before error");
    ASSERT_EQ(g_mutexLeaveCount, 1, "mutex leave should occur once before error");

    freeTestDb(db);
    resetGlobalsForTest();
}

// Main: run all tests and report summary
int main(){
    // Drive tests
    test_normal_path();
    test_safety_check_fail();
    test_null_name_armored();
    test_utf16_conversion_failure();
    test_create_collation_error_propagation();

    // Summary
    std::cout << "Tests run: " << g_test_count << " (Note: per-ASSERT counts not tracked individually in this simple harness.)\n";
    if(g_test_failed == 0){
        std::cout << "All tests PASSED.\n";
        return 0;
    }else{
        std::cout << "Tests FAILED: " << g_test_failed << "\n";
        return 1;
    }
}
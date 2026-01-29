// C++11 test suite for the focal function sqlite3rbu_db
// This test suite does not rely on GTest. It uses a small in-house
// non-terminating assertion framework to exercise the function under test.
//
// The focal method:
// sqlite3 *sqlite3rbu_db(sqlite3rbu *pRbu, int bRbu) {
//   sqlite3 *db = 0;
//   if( pRbu ){
//     db = (bRbu ? pRbu->dbRbu : pRbu->dbMain);
//   }
//   return db;
// }
//
// It returns either pRbu->dbRbu or pRbu->dbMain depending on bRbu, when pRbu is non-null.
// When pRbu is null, it returns 0.
//
// Step-by-step coverage:
// - pRbu == nullptr: verify returns nullptr regardless of bRbu.
// - pRbu != nullptr and bRbu == 0: verify returns pRbu->dbMain.
// - pRbu != nullptr and bRbu == 1: verify returns pRbu->dbRbu.

#include <string.h>
#include <windows.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <sqlite3rbu.h>
#include <cstdint>


// Include the C header within C linkage to ensure symbol names are unmangled.
extern "C" {
}

// Simple non-terminating test harness (similar to GoogleTest EXPECT_* but continues on failure)
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::printf("EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        std::printf("EXPECT_EQ failed: %s == %s (value = %ld vs %ld) at %s:%d\n", #a, #b, (long)(a), (long)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b) do { \
    if((void*)(a) != (void*)(b)) { \
        std::printf("EXPECT_PTR_EQ failed: %p != %p (addresses) at %s:%d\n", (void*)(a), (void*)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)


// Forward declarations of the focal function are provided by sqlite3rbu.h
// We will exercise sqlite3rbu_db through its C linkage.
static void test_null_pRbu_returns_null_for_bRbu_false_and_true() {
    // pRbu is null; expect 0 for both bRbu values.
    sqlite3 *res0 = sqlite3rbu_db(nullptr, 0);
    EXPECT_PTR_EQ(res0, nullptr);

    sqlite3 *res1 = sqlite3rbu_db(nullptr, 1);
    EXPECT_PTR_EQ(res1, nullptr);
}

static void test_pRbu_nonnull_bRbu_false_returns_dbMain() {
    // Prepare a mock sqlite3rbu with distinct dummy pointers for dbMain and dbRbu
    sqlite3rbu rbu;
    // Use distinct dummy pointers to verify correct field selection
    sqlite3 *mainPtr = reinterpret_cast<sqlite3*>(0x1001);
    sqlite3 *rbuPtr  = reinterpret_cast<sqlite3*>(0x2002);

    // Assign fields
    rbu.dbMain = mainPtr;
    rbu.dbRbu  = rbuPtr;

    sqlite3 *res = sqlite3rbu_db(&rbu, 0);
    EXPECT_PTR_EQ(res, mainPtr);
}

static void test_pRbu_nonnull_bRbu_true_returns_dbRbu() {
    sqlite3rbu rbu;
    sqlite3 *mainPtr = reinterpret_cast<sqlite3*>(0x3003);
    sqlite3 *rbuPtr  = reinterpret_cast<sqlite3*>(0x4004);

    rbu.dbMain = mainPtr;
    rbu.dbRbu  = rbuPtr;

    sqlite3 *res = sqlite3rbu_db(&rbu, 1);
    EXPECT_PTR_EQ(res, rbuPtr);
}

static void test_mixed_scenarios() {
    // Additional sanity: ensure that when both pointers are null but pRbu is non-null,
    // the function returns the corresponding field value (which could be null).
    sqlite3rbu rbu;
    rbu.dbMain = nullptr;
    rbu.dbRbu  = reinterpret_cast<sqlite3*>(0xDEADBEEF);

    sqlite3 *resMain = sqlite3rbu_db(&rbu, 0);
    EXPECT_PTR_EQ(resMain, nullptr);

    sqlite3 *resRbu = sqlite3rbu_db(&rbu, 1);
    EXPECT_PTR_EQ(resRbu, rbu.dbRbu);
}


int main() {
    // Run all tests
    test_null_pRbu_returns_null_for_bRbu_false_and_true();
    test_pRbu_nonnull_bRbu_false_returns_dbMain();
    test_pRbu_nonnull_bRbu_true_returns_dbRbu();
    test_mixed_scenarios();

    if(g_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("TESTS FAILED: %d failure(s)\n", g_failures);
        return 1;
    }
}
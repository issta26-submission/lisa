// Minimal C++11 test harness for sqlite3Fts3InitTok (no GoogleTest/GMock; use lightweight EXPECT_* macros)
// This test assumes the project under test is buildable with sqlite3 and provides sqlite3Fts3InitTok.
// The tests focus on exercising the normal path and ensuring non-crashing behavior with varying parameters.

#include <fts3Int.h>
#include <string.h>
#include <assert.h>
#include <sqlite3.h>
#include <iostream>


// Forward declaration of the focal function from the production code (C linkage)
extern "C" int sqlite3Fts3InitTok(sqlite3 *db, void* pHash, void(*xDestroy)(void*));

// Lightweight, non-terminating test assertions (do not abort on failure)
static int g_failures = 0;

#define TEST_PRINT(...) do { std::cout << __VA_ARGS__ << std::endl; } while(0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST] " << __func__ << ": EXPECT_TRUE(" #cond ") failed." << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        if (!((a) == (b))) { \
            std::cerr << "[TEST] " << __func__ << ": EXPECT_EQ(" #a ", " #b ") failed. Actual: " << (static_cast<long long>(a)) << " vs " << (static_cast<long long>(b)) << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Dummy destroy callback to exercise non-null function pointer path
static void dummyDestroy(void* /*p*/) {
    // Intentionally empty; existence tests the pointer is accepted
}

// Test 1: Normal path - register module on a fresh in-memory DB should succeed (SQLITE_OK)
static void test_InitTok_NormalPath_InMemoryDB() {
    TEST_PRINT("[TEST] Starting: test_InitTok_NormalPath_InMemoryDB");
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr);
    EXPECT_EQ(rc_open, SQLITE_OK);

    // Call focal function with null pHash (allowed) and null destroy callback
    int rc = sqlite3Fts3InitTok(db, nullptr, nullptr);
    EXPECT_EQ(rc, SQLITE_OK);

    sqlite3_close(db);
    TEST_PRINT("[TEST] Completed: test_InitTok_NormalPath_InMemoryDB");
}

// Test 2: Path with non-null xDestroy callback to ensure acceptance of callback parameter
static void test_InitTok_WithNonNullDestroyCallback() {
    TEST_PRINT("[TEST] Starting: test_InitTok_WithNonNullDestroyCallback");
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr);
    EXPECT_EQ(rc_open, SQLITE_OK);

    int rc = sqlite3Fts3InitTok(db, nullptr, dummyDestroy);
    EXPECT_EQ(rc, SQLITE_OK);

    sqlite3_close(db);
    TEST_PRINT("[TEST] Completed: test_InitTok_WithNonNullDestroyCallback");
}

// Test 3: Idempotency check - calling twice on a fresh DB should still succeed
static void test_InitTok_TwiceOnSameDB() {
    TEST_PRINT("[TEST] Starting: test_InitTok_TwiceOnSameDB");
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr);
    EXPECT_EQ(rc_open, SQLITE_OK);

    int rc_first = sqlite3Fts3InitTok(db, nullptr, nullptr);
    EXPECT_EQ(rc_first, SQLITE_OK);

    int rc_second = sqlite3Fts3InitTok(db, nullptr, nullptr);
    EXPECT_EQ(rc_second, SQLITE_OK);

    sqlite3_close(db);
    TEST_PRINT("[TEST] Completed: test_InitTok_TwiceOnSameDB");
}

// Test runner
int main() {
    TEST_PRINT("Running sqlite3Fts3InitTok test suite (C++11, non-GTest).");
    test_InitTok_NormalPath_InMemoryDB();
    test_InitTok_WithNonNullDestroyCallback();
    test_InitTok_TwiceOnSameDB();

    if (g_failures == 0) {
        TEST_PRINT("ALL TESTS PASSED.");
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}
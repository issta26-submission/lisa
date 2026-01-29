// High-coverage unit test suite for sqlite3_recover_config
// This test suite is written in C++11, uses a lightweight custom test harness
// (no Google Test). It exercises various branches of sqlite3_recover_config
// defined in sqlite3recover.c and relies on the public header sqlite3recover.h.
// The tests are designed to be non-terminating upon assertion failures, logging
// failures and continuing execution to maximize coverage.

// Notes:
// - We provide a minimal sqlite3_free implementation to satisfy linkage when the
//   test is built without the full SQLite library.
// - Tests instantiate sqlite3_recover objects with eState = RECOVER_STATE_INIT to
//   exercise the successful configuration paths.
// - All tests are self-contained and clean up allocated memory.

#include <string.h>
#include <assert.h>
#include <sqlite3recover.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Include the C header for sqlite3_recover structures and API.
// Since this is a C API, wrap the include to ensure correct linkage in C++.
extern "C" {
}

// Provide a minimal sqlite3_free implementation for test linkage.
// This is a simple wrapper around free; in a real environment, sqlite3_free is
// provided by the SQLite library.
extern "C" void sqlite3_free(void* ptr) {
    free(ptr);
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define LOG_PREFIX "RECOVER_CONFIG_TEST"
#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << LOG_PREFIX << ": EXPECT_TRUE failed at " \
                  << __FILE__ << ":" << __LINE__ << " -- " #cond "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_total_tests; \
    if((a) != (b)) { \
        ++g_failed_tests; \
        std::cerr << LOG_PREFIX << ": EXPECT_EQ failed at " \
                  << __FILE__ << ":" << __LINE__ << " -- " #a " != " #b \
                  << " (actual: " << (a) << ", " << (b) << ")\n"; \
    } \
} while(0)

#define EXPECT_STR_EQ(a, b) do { \
    ++g_total_tests; \
    if(((a) == nullptr && (b) != nullptr) || ((a) != nullptr && (b) == nullptr) || \
       ((a) && strcmp((a), (b)) != 0)) { \
        ++g_failed_tests; \
        std::cerr << LOG_PREFIX << ": EXPECT_STR_EQ failed at " \
                  << __FILE__ << ":" << __LINE__ << " -- \"" << (a) \
                  << "\" != \"" << (b) << "\"\n"; \
    } \
} while(0)

static sqlite3_recover* make_recover_init() {
    // Allocate and zero-initialize a recover object, set the initial state.
    sqlite3_recover *p = (sqlite3_recover*)calloc(1, sizeof(sqlite3_recover));
    if(p) {
        p->eState = RECOVER_STATE_INIT;
        p->zStateDb = nullptr;
        p->zLostAndFound = nullptr;
        p->bFreelistCorrupt = 0;
        p->bRecoverRowid = 0;
        p->bSlowIndexes = 0;
    }
    return p;
}

// Test: p == 0 should return SQLITE_NOMEM
static void test_null_pointer_returns_nOMEM() {
    int rc = sqlite3_recover_config(nullptr, 0, nullptr);
    EXPECT_EQ(rc, SQLITE_NOMEM);
}

// Test: p not in INIT state should return SQLITE_MISUSE
static void test_wrong_state_misuse() {
    sqlite3_recover *p = make_recover_init();
    if(p){
        // Move to a non-init state
        p->eState = RECOVER_STATE_DEAD; // Some non-init state (implementation-specific)
        int rc = sqlite3_recover_config(p, 123, nullptr);
        EXPECT_EQ(rc, SQLITE_MISUSE);
        free(p);
    } else {
        EXPECT_TRUE(false && "Failed to allocate recover object");
    }
}

// Test: op == 789 should set zStateDb from pArg via recoverMPrintf
static void test_op_789_sets_state_db() {
    sqlite3_recover *p = make_recover_init();
    if(!p){
        EXPECT_TRUE(false && "Failed to allocate recover object");
        return;
    }

    const char *arg = "stateName";
    int rc = sqlite3_recover_config(p, 789, (void*)arg);
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_TRUE(p->zStateDb != nullptr);
    if(p->zStateDb){
        EXPECT_STR_EQ(p->zStateDb, arg);
        sqlite3_free(p->zStateDb); // cleanup allocated string
        p->zStateDb = nullptr;
    }
    free(p);
}

// Test: LOST_AND_FOUND: zLostAndFound is freed and set to nullptr when arg is NULL
static void test_op_lost_and_found_null_arg() {
    sqlite3_recover *p = make_recover_init();
    if(!p){
        EXPECT_TRUE(false && "Failed to allocate recover object");
        return;
    }

    // Preallocate a string to be freed by the function
    p->zLostAndFound = strdup("old");
    int rc = sqlite3_recover_config(p, SQLITE_RECOVER_LOST_AND_FOUND, nullptr);
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_TRUE(p->zLostAndFound == nullptr); // should be cleared

    // ensure there is no memory leak (old memory freed by sqlite3_free)

    free(p);
}

// Test: LOST_AND_FOUND: zLostAndFound is set when non-null arg is provided
static void test_op_lost_and_found_non_null_arg() {
    sqlite3_recover *p = make_recover_init();
    if(!p){
        EXPECT_TRUE(false && "Failed to allocate recover object");
        return;
    }

    const char *arg = "newLost";
    int rc = sqlite3_recover_config(p, SQLITE_RECOVER_LOST_AND_FOUND, (void*)arg);
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_TRUE(p->zLostAndFound != nullptr);
    if(p->zLostAndFound){
        EXPECT_STR_EQ(p->zLostAndFound, arg);
        sqlite3_free(p->zLostAndFound);
        p->zLostAndFound = nullptr;
    }
    free(p);
}

// Test: FREELIST_CORRUPT copies the int value from pArg
static void test_op_freelist_corrupt() {
    sqlite3_recover *p = make_recover_init();
    if(!p){
        EXPECT_TRUE(false && "Failed to allocate recover object");
        return;
    }
    int v = 7;
    int rc = sqlite3_recover_config(p, SQLITE_RECOVER_FREELIST_CORRUPT, &v);
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_EQ(p->bFreelistCorrupt, 7);
    free(p);
}

// Test: ROWIDS copies the int value from pArg
static void test_op_rowids() {
    sqlite3_recover *p = make_recover_init();
    if(!p){
        EXPECT_TRUE(false && "Failed to allocate recover object");
        return;
    }
    int v = 3;
    int rc = sqlite3_recover_config(p, SQLITE_RECOVER_ROWIDS, &v);
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_EQ(p->bRecoverRowid, 3);
    free(p);
}

// Test: SLOWINDEXES copies the int value from pArg
static void test_op_slowindexes() {
    sqlite3_recover *p = make_recover_init();
    if(!p){
        EXPECT_TRUE(false && "Failed to allocate recover object");
        return;
    }
    int v = 1;
    int rc = sqlite3_recover_config(p, SQLITE_RECOVER_SLOWINDEXES, &v);
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_EQ(p->bSlowIndexes, 1);
    free(p);
}

// Test: default/unrecognized op should return SQLITE_NOTFOUND
static void test_default_not_found() {
    sqlite3_recover *p = make_recover_init();
    if(!p){
        EXPECT_TRUE(false && "Failed to allocate recover object");
        return;
    }
    int rc = sqlite3_recover_config(p, 0xDEADBEEF, nullptr);
    EXPECT_EQ(rc, SQLITE_NOTFOUND);
    free(p);
}

// Run all tests
static void run_all_tests() {
    test_null_pointer_returns_nOMEM();
    test_wrong_state_misuse();
    test_op_789_sets_state_db();
    test_op_lost_and_found_null_arg();
    test_op_lost_and_found_non_null_arg();
    test_op_freelist_corrupt();
    test_op_rowids();
    test_op_slowindexes();
    test_default_not_found();

    // Summary
    std::cout << "RECOVER_CONFIG_TEST SUMMARY: "
              << g_total_tests << " tests, "
              << g_failed_tests << " failures." << std::endl;
}

int main() {
    run_all_tests();
    return g_failed_tests != 0 ? 1 : 0;
}
// test_sqlite_dbmallocsize.cpp
// A lightweight unit test suite for sqlite3DbMallocSize
// - No Google Test required
// - Uses the real SQLite internal interfaces (header sqliteInt.h) when available
// - Focuses on covering the core branches inside sqlite3DbMallocSize
// - Uses a minimal test harness with non-terminating EXPECT-like checks

#include <vector>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// Bring in the SQLite internal declarations required for sqlite3DbMallocSize.
// We rely on the project's existing headers so this test integrates with the
// real build (when compiled into the same translation units as SQLite).
extern "C" {
}

// Very small test harness helpers (non-terminating assertions)
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static std::vector<TestResult> g_results;

static void record_result(const std::string &name, bool pass, const std::string &msg = "") {
    g_results.push_back({name, pass, msg});
    if (pass) {
        std::cerr << "[PASS] " << name << "\n";
    } else {
        std::cerr << "[FAIL] " << name << " - " << msg << "\n";
    }
}

// Simple expectations (non-terminating)
static void EXPECT_EQ_INT(const char* test_name, int a, int b) {
    if (a == b) {
        record_result(test_name, true);
    } else {
        record_result(test_name, false,
            "Expected " + std::to_string(b) + " but got " + std::to_string(a));
    }
}
static void EXPECT_EQ_PTR(const char* test_name, const void* a, const void* b) {
    if (a == b) {
        record_result(test_name, true);
    } else {
        record_result(test_name, false,
            "Expected pointer " + std::to_string(reinterpret_cast<uintptr_t>(b)) +
            " but got " + std::to_string(reinterpret_cast<uintptr_t>(a)));
    }
}

// Helper to create a mock sqlite3 database object with lookaside configuration
static sqlite3* create_fake_db_for_lookaside(void* pStart, void* pMiddle, void* pTrueEnd, int szTrue) {
    // Allocate a minimal sqlite3 object
    sqlite3* db = (sqlite3*)calloc(1, sizeof(sqlite3));
    if(!db) return nullptr;

    // Initialize lookaside region bounds
    db->lookaside.pStart = pStart;
    db->lookaside.pMiddle = pMiddle;
    db->lookaside.pTrueEnd = pTrueEnd;
    db->lookaside.szTrue = szTrue;

    // Create a real mutex so we can "hold" it in tests
    db->mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);

    return db;
}

// Test 1: p inside [pMiddle, pTrueEnd) should return LOOKASIDE_SMALL (mutex must be held)
static void test_sqlite3DbMallocSize_lookaside_small_hit() {
    const char* test_name = "sqlite3DbMallocSize Lookaside SMALL hit";

    // Setup lookaside region
    void* pStart = (void*)0x1000;
    void* pMiddle = (void*)0x2000;
    void* pTrueEnd = (void*)0x3000;
    int szTrue = 64;
    sqlite3* db = create_fake_db_for_lookaside(pStart, pMiddle, pTrueEnd, szTrue);
    if(!db) {
        record_result(test_name, false, "Failed to allocate mock sqlite3 db");
        return;
    }

    // Acquire the mutex to satisfy the internal assertion
    sqlite3_mutex_enter(db->mutex);

    void* p = (void*)0x2500; // >= pMiddle and < pTrueEnd
    int result = sqlite3DbMallocSize(db, p);

    // Expected: LOOKASIDE_SMALL
    EXPECT_EQ_INT(test_name, result, LOOKASIDE_SMALL);

    sqlite3_mutex_leave(db->mutex);
    free(db);
}

// Test 2: p inside [pStart, pMiddle) should return the true lookaside size (mutex must be held)
static void test_sqlite3DbMallocSize_lookaside_true_hit() {
    const char* test_name = "sqlite3DbMallocSize Lookaside SIZE hit";

    // Setup lookaside region
    void* pStart = (void*)0x1000;
    void* pMiddle = (void*)0x2000;
    void* pTrueEnd = (void*)0x3000;
    int szTrue = 128;
    sqlite3* db = create_fake_db_for_lookaside(pStart, pMiddle, pTrueEnd, szTrue);
    if(!db) {
        record_result(test_name, false, "Failed to allocate mock sqlite3 db");
        return;
    }

    // Acquire the mutex to satisfy the internal assertion
    sqlite3_mutex_enter(db->mutex);

    void* p = (void*)0x1800; // >= pStart and < pMiddle
    int result = sqlite3DbMallocSize(db, p);

    // Expected: db->lookaside.szTrue
    EXPECT_EQ_INT(test_name, result, szTrue);

    sqlite3_mutex_leave(db->mutex);
    free(db);
}

// Test 3: p outside lookaside range should delegate to sqlite3GlobalConfig.m.xSize
static void test_sqlite3DbMallocSize_outside_lookaside() {
    const char* test_name = "sqlite3DbMallocSize outside lookaside delegates to xSize";

    // Setup lookaside region (values chosen to ensure p is outside)
    void* pStart = (void*)0x1000;
    void* pMiddle = (void*)0x2000;
    void* pTrueEnd = (void*)0x3000;
    int szTrue = 16;
    sqlite3* db = create_fake_db_for_lookaside(pStart, pMiddle, pTrueEnd, szTrue);
    if(!db) {
        record_result(test_name, false, "Failed to allocate mock sqlite3 db");
        return;
    }

    // We want p outside the lookaside region
    void* p = (void*)0x4000;

    // Obtain expected value from the global xSize function
    int expected = sqlite3GlobalConfig.m.xSize(p);

    int result = sqlite3DbMallocSize(db, p);

    EXPECT_EQ_INT(test_name, result, expected);

    free(db);
}

// Test 4: db == NULL path should delegate to sqlite3GlobalConfig.m.xSize
static void test_sqlite3DbMallocSize_null_db_path() {
    const char* test_name = "sqlite3DbMallocSize db==NULL delegates to xSize";

    void* p = (void*)0x5000;
    int expected = sqlite3GlobalConfig.m.xSize(p);
    int result = sqlite3DbMallocSize(nullptr, p);

    EXPECT_EQ_INT(test_name, result, expected);
}

// Optional: Test 5 (edge-case) p just at pStart boundary
static void test_sqlite3DbMallocSize_at_start_boundary() {
    const char* test_name = "sqlite3DbMallocSize at pStart boundary returns szTrue";

    void* pStart = (void*)0x1000;
    void* pMiddle = (void*)0x2000;
    void* pTrueEnd = (void*)0x3000;
    int szTrue = 42;
    sqlite3* db = create_fake_db_for_lookaside(pStart, pMiddle, pTrueEnd, szTrue);
    if(!db) {
        record_result(test_name, false, "Failed to allocate mock sqlite3 db");
        return;
    }

    sqlite3_mutex_enter(db->mutex);
    void* p = pStart; // exactly at pStart
    int result = sqlite3DbMallocSize(db, p);

    // Expect szTrue since p >= pStart
    EXPECT_EQ_INT(test_name, result, szTrue);

    sqlite3_mutex_leave(db->mutex);
    free(db);
}

// Driver: run all tests
int main() {
    // Run tests
    test_sqlite3DbMallocSize_lookaside_small_hit();
    test_sqlite3DbMallocSize_lookaside_true_hit();
    test_sqlite3DbMallocSize_outside_lookaside();
    test_sqlite3DbMallocSize_null_db_path();
    test_sqlite3DbMallocSize_at_start_boundary();

    // Summary
    int passed = 0;
    int total = (int)g_results.size();
    for (const auto &r : g_results) if (r.passed) ++passed;
    std::cout << "\nTest Summary: " << passed << " / " << total << " passed.\n";

    // Return 0 if all tests passed, non-zero otherwise
    return (passed == total) ? 0 : 1;
}
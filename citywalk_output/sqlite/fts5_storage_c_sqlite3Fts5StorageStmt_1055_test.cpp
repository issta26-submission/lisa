// C++11 unit tests for sqlite3Fts5StorageStmt (no GTest, plain C++ test harness)

#include <sqlite3.h>
#include <iostream>
#include <cstring>
#include <fts5Int.h>


// Import necessary project dependencies (C-style headers)
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_total = 0;
static int g_tests_passed = 0;

#define TEST_MSG_PASS "[PASS] "
#define TEST_MSG_FAIL "[FAIL] "
#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_total; \
    if (static_cast<bool>(cond)) { \
        ++g_tests_passed; \
        std::cout << TEST_MSG_PASS << (msg) << std::endl; \
    } else { \
        std::cout << TEST_MSG_FAIL << (msg) << std::endl; \
    } \
} while(0)

// Helper to print summary
static void print_summary() {
    std::cout << "\nTest summary: " << g_tests_passed << "/" << g_tests_total << " tests passed." << std::endl;
}

// Test 1: Verify that in the normal (OK) path, the provided statement pointer is returned via *pp
// and the corresponding slot in p->aStmt is cleared to NULL after the call.
static void test_sqlite3Fts5StorageStmt_ok_path_ASC() {
    // Arrange
    Fts5Storage p;
    std::memset(&p, 0, sizeof(Fts5Storage));

    // Use a distinct sentinel pointer to identify the original statement
    sqlite3_stmt *sentinel = reinterpret_cast<sqlite3_stmt*>(0xDEADBEEF);
    p.aStmt[FTS5_STMT_SCAN_ASC] = sentinel; // pre-load the storage slot

    sqlite3_stmt *pp = nullptr;
    char *pzErrMsg = nullptr;

    // Act
    int rc = sqlite3Fts5StorageStmt(&p, FTS5_STMT_SCAN_ASC, &pp, &pzErrMsg);

    // Assert
    EXPECT_TRUE(rc == SQLITE_OK, "sqlite3Fts5StorageStmt should return SQLITE_OK on OK path for ASC");
    EXPECT_TRUE(pp == sentinel, "The *pp returned should be the same as p.aStmt[SCAN_ASC]");
    EXPECT_TRUE(p.aStmt[FTS5_STMT_SCAN_ASC] == nullptr, "p.aStmt[SCAN_ASC] should be cleared to NULL after OK path");
    EXPECT_TRUE(pzErrMsg == nullptr, "pzErrMsg should remain NULL on success");
}

// Test 2: Repeat the same OK-path validation for DESC
static void test_sqlite3Fts5StorageStmt_ok_path_DESC() {
    Fts5Storage p;
    std::memset(&p, 0, sizeof(Fts5Storage));

    sqlite3_stmt *sentinel = reinterpret_cast<sqlite3_stmt*>(0xC0FFEE);
    p.aStmt[FTS5_STMT_SCAN_DESC] = sentinel;

    sqlite3_stmt *pp = nullptr;
    char *pzErrMsg = nullptr;

    int rc = sqlite3Fts5StorageStmt(&p, FTS5_STMT_SCAN_DESC, &pp, &pzErrMsg);

    EXPECT_TRUE(rc == SQLITE_OK, "sqlite3Fts5StorageStmt should return SQLITE_OK on OK path for DESC");
    EXPECT_TRUE(pp == sentinel, "The *pp returned should be the same as p.aStmt[SCAN_DESC]");
    EXPECT_TRUE(p.aStmt[FTS5_STMT_SCAN_DESC] == nullptr, "p.aStmt[SCAN_DESC] should be cleared to NULL after OK path");
    EXPECT_TRUE(pzErrMsg == nullptr, "pzErrMsg should remain NULL on success");
}

// Test 3: Repeat the same OK-path validation for LOOKUP
static void test_sqlite3Fts5StorageStmt_ok_path_LOOKUP() {
    Fts5Storage p;
    std::memset(&p, 0, sizeof(Fts5Storage));

    sqlite3_stmt *sentinel = reinterpret_cast<sqlite3_stmt*>(0xB16B00B5);
    p.aStmt[FTS5_STMT_LOOKUP] = sentinel;

    sqlite3_stmt *pp = nullptr;
    char *pzErrMsg = nullptr;

    int rc = sqlite3Fts5StorageStmt(&p, FTS5_STMT_LOOKUP, &pp, &pzErrMsg);

    EXPECT_TRUE(rc == SQLITE_OK, "sqlite3Fts5StorageStmt should return SQLITE_OK on OK path for LOOKUP");
    EXPECT_TRUE(pp == sentinel, "The *pp returned should be the same as p.aStmt[LOOKUP]");
    EXPECT_TRUE(p.aStmt[FTS5_STMT_LOOKUP] == nullptr, "p.aStmt[LOOKUP] should be cleared to NULL after OK path");
    EXPECT_TRUE(pzErrMsg == nullptr, "pzErrMsg should remain NULL on success");
}

// Optional: Loop over all supported statements to ensure consistent behavior
static void test_sqlite3Fts5StorageStmt_ok_path_all() {
    Fts5Storage p;
    std::memset(&p, 0, sizeof(Fts5Storage));

    struct { int stmt; const char* name; } tests[] = {
        { FTS5_STMT_SCAN_ASC,  "ASC" },
        { FTS5_STMT_SCAN_DESC, "DESC" },
        { FTS5_STMT_LOOKUP,    "LOOKUP" }
    };

    for (const auto& t : tests) {
        sqlite3_stmt *sentinel = reinterpret_cast<sqlite3_stmt*>(0x1000 + t.stmt);
        p.aStmt[t.stmt] = sentinel;

        sqlite3_stmt *pp = nullptr;
        char *pzErrMsg = nullptr;

        int rc = sqlite3Fts5StorageStmt(&p, t.stmt, &pp, &pzErrMsg);

        EXPECT_TRUE(rc == SQLITE_OK, std::string("OK path for ") + t.name);
        EXPECT_TRUE(pp == sentinel, (std::string("pp equals sentinel for ") + t.name).c_str());
        EXPECT_TRUE(p.aStmt[t.stmt] == nullptr, ("aStmt[" + std::string(t.name) + "] cleared").c_str());
        EXPECT_TRUE(pzErrMsg == nullptr, ("pzErrMsg NULL for ") + t.name);
    }
}

// Main: run tests
int main() {
    // Run individual tests with explanatory comments
    std::cout << "Running tests for sqlite3Fts5StorageStmt (C API) in C++11 test harness...\n" << std::endl;

    test_sqlite3Fts5StorageStmt_ok_path_ASC();
    test_sqlite3Fts5StorageStmt_ok_path_DESC();
    test_sqlite3Fts5StorageStmt_ok_path_LOOKUP();
    test_sqlite3Fts5StorageStmt_ok_path_all();

    print_summary();
    return (g_tests_total == g_tests_passed) ? 0 : 1;
}
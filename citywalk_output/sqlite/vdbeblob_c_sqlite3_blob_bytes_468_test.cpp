// Test suite for sqlite3_blob_bytes from vdbeblob.c
// This test is designed to be compiled with a real build that includes the
// vdbeblob.c implementation. It uses a lightweight, framework-free test harness
// (no Google Test) and relies only on the C++ standard library.
//
// Step 1 (Context): The focal method.
// int sqlite3_blob_bytes(sqlite3_blob *pBlob) {
//   Incrblob *p = (Incrblob *)pBlob;
//   return (p && p->pStmt) ? p->nByte : 0;
// }
//
// Step 2 (Candidate Keywords): sqlite3_blob, sqlite3_blob_bytes, Incrblob,
// pBlob, pStmt, nByte.
//
// Step 3 (Domain Knowledge): We create a minimal Incrblob-like layout that matches
// the expected layout for the real Incrblob type (pStmt, nByte) and verify true/false
// branches of the predicate (p && p->pStmt). We use a trivial test harness following
// the constraints provided (no GTest, non-terminating assertions).

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdint>


// Forward declaration of the production type used by the focal function.
// We don't need its full definition here; the test only passes a pointer to a
// sqlite3_blob to the function under test.
struct sqlite3_blob;

// Declaration of the focal function under test with C linkage to match the
// production library's symbol (extern "C" to avoid name mangling in C++).
extern "C" int sqlite3_blob_bytes(struct sqlite3_blob *pBlob);

// Step 3 (Test Lava): Provide a minimal Incrblob layout used by the production code.
// We assume the following layout to be compatible with the library's actual Incrblob:
//   struct Incrblob {
//     sqlite3_stmt *pStmt;
//     sqlite3_int64 nByte;
//   };
// Since we can't rely on the exact production headers in this isolated test,
// we mirror the memory layout with a portable equivalent.
struct Incrblob {
    void* pStmt;        // pointer to statement (can be nullptr)
    long long nByte;    // number of bytes cached in the blob
};

// Global test failure counter
static int g_failures = 0;

// Simple non-terminating assertion helpers:
// - Do not abort on failure; instead, log and continue to improve coverage.
#define EXPECT_EQ(actual, expected) do {                          \
    if ((actual) != (expected)) {                                 \
        std::cerr << "[FAIL] " __FILE__ ":" << __LINE__           \
                  << " - Expected: " << (expected)               \
                  << ", Got: " << (actual) << std::endl;        \
        ++g_failures;                                              \
    }                                                             \
} while (0)

#define TEST_NOTE(msg) do {                                         \
    std::cout << "[NOTE] " << (msg) << std::endl;                   \
} while (0)

// Step 2 (Unit Test Generation): Three scenarios to cover:
// 1) Null pBlob -> should return 0 (false branch and null pointer handling)
// 2) Non-null pBlob but pStmt == nullptr -> should return 0 (false branch inside p->pStmt)
// 3) Non-null pBlob and pStmt != nullptr -> should return nByte (true branch)
static void test_blob_bytes_null_pointer() {
    // This test ensures that a null blob yields 0 without dereferencing.
    TEST_NOTE("Test: sqlite3_blob_bytes should return 0 for null blob pointer (pBlob == nullptr).");
    int result = sqlite3_blob_bytes(nullptr);
    EXPECT_EQ(result, 0);
}

static void test_blob_bytes_null_pstmt() {
    // Arrange: pBlob is non-null, but pStmt inside it is null.
    // We mirror the Incrblob layout expected by sqlite3_blob_bytes.
    Incrblob blob;
    blob.pStmt = nullptr;
    blob.nByte = 1234; // value should be ignored when pStmt is nullptr

    sqlite3_blob* pBlob = (sqlite3_blob*)&blob;
    TEST_NOTE("Test: sqlite3_blob_bytes should return 0 when pBlob is non-null but pStmt is null.");
    int result = sqlite3_blob_bytes(pBlob);
    EXPECT_EQ(result, 0);
}

static void test_blob_bytes_non_null_pstmt() {
    // Arrange: pBlob is non-null and pStmt is non-null; nByte should be returned.
    Incrblob blob;
    blob.pStmt = (void*)0x1; // non-null sentinel
    blob.nByte = 0x7A;       // chosen value within int range

    sqlite3_blob* pBlob = (sqlite3_blob*)&blob;
    TEST_NOTE("Test: sqlite3_blob_bytes should return nByte when pBlob is non-null and pStmt is non-null.");
    int result = sqlite3_blob_bytes(pBlob);
    EXPECT_EQ(result, static_cast<int>(blob.nByte));
}

// Simple test runner
static void run_all_tests() {
    test_blob_bytes_null_pointer();
    test_blob_bytes_null_pstmt();
    test_blob_bytes_non_null_pstmt();
}

// Main: Entry point for the test suite.
// We rely on a simple non-terminating harness, print summary and return the
// number of failures to allow integration with simple CI scripts.
int main() {
    TEST_NOTE("Starting sqlite3_blob_bytes unit tests (no GTest, C++11).");
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "[OK] All tests passed.\n";
    } else {
        std::cout << "[WARN] Tests completed with " << g_failures << " failure(s).\n";
    }
    // Return non-zero on failure to signal issues to external test runners
    return g_failures;
}
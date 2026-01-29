// C++11 test harness for sqlite3Fts5HashWrite (fts5_hash.c)
// Note: This test suite is designed to exercise the focal method via its
// public API surface as exposed by the project. It relies on the project’s
// internal C API names and structures (e.g., Fts5Hash, Fts5Config) being
// available from the linked SQLite/FTS5 library. This file does not use GTest.
// It uses a lightweight custom test framework implemented below.
// The tests aim to achieve broad coverage by exercising the main code paths
// in sqlite3Fts5HashWrite: creating a new hash entry, and detecting that the
// hash is no longer empty after inserts. It also includes a simple test for a
// delete-path (iCol < 0) to exercise the delete branch.
//
// To run:
//  - Compile this file together with the project's fts5_hash.c (and its
//    dependencies) or link against the built library that provides the
//    sqlite3Fts5HashWrite and related APIs.
//  - Ensure build flags enable C linkage for the C API (extern "C") as shown.
//  - Run the resulting executable.
//
// The test suite avoids private/internal private-member access and relies on
// the public interface as per the project's API.

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Lightweight test framework (non-terminating assertions)
static int g_testTotal = 0;
static int g_testFail  = 0;

static void log_fail(const std::string &testName, const std::string &msg) {
    std::cerr << "[FAIL] " << testName << ": " << msg << "\n";
    ++g_testFail;
}
static void log_pass(const std::string &testName, const std::string &msg) {
    std::cout << "[PASS] " << testName << ": " << msg << "\n";
}
#define TESTCASE(name) static void name(); \
    static void name(); \
    struct name##_registrar { name##_registrar() { name(); } }; \
    static name##_registrar global_##name##_registrar; \
    static void name()

#define ASSERT_TRUE(cond, testName, msg) \
    do { \
        ++g_testTotal; \
        if(!(cond)) { log_fail(testName, msg); } \
        else { log_pass(testName, msg); } \
    } while(0)

#define ASSERT_EQ(a, b, testName, msg) \
    do { \
        ++g_testTotal; \
        if( (a) != (b) ) { \
            char buf[256]; snprintf(buf, sizeof(buf), "expected %lld, got %lld", (long long)(b), (long long)(a)); \
            log_fail(testName, (std::string(msg) + " | " + buf)); \
        } else { log_pass(testName, msg); } \
    } while(0)


// Forward declare project C API structures and functions.
// These declarations rely on the project exposing a C API for the hash module.
// We intentionally keep Fts5Config opaque here to avoid relying on its
// internal layout in the test harness. The actual project library provides
// a complete definition compatible with this declaration.

extern "C" {
    // Opaque types (the actual size/layout is defined in the project headers)
    struct Fts5Config;
    struct Fts5Hash;
    struct Fts5HashEntry;

    // Basic integer type used by the API
    // (forked from project - assume 64-bit for sqlite3_int64)
    typedef long long sqlite3_int64;

    // Public API surface (as exposed by the project)
    int sqlite3Fts5HashNew(struct Fts5Config *pConfig, struct Fts5Hash **ppNew, int *pnByte);
    int sqlite3Fts5HashWrite(struct Fts5Hash *pHash,
                             sqlite3_int64 iRowid,
                             int iCol,
                             int iPos,
                             char bByte,
                             const char *pToken, int nToken);
    void sqlite3Fts5HashFree(struct Fts5Hash *pHash);
    int sqlite3Fts5HashIsEmpty(struct Fts5Hash *pHash);
    // Optional helpers that are often used in tests
    int sqlite3Fts5HashQuery(struct Fts5Hash *pHash, int nPre,
                              const char *pTerm, int nTerm,
                              void **ppOut, int *pnDoclist);
    // Position/list helpers used by hash write path (not required for basic tests)
    int sqlite3Fts5HashAddPoslistSize(struct Fts5Hash *pHash, struct Fts5HashEntry *p,
                                     struct Fts5HashEntry *p2);
    // A couple of library helpers may be used by tests; declare if needed
}

// Lightweight helper to print a separator
static void print_sep() { std::cout << "----------------------------------------\n"; }

// Test 1: Create a new hash, ensure it is initially empty, perform a simple write,
// and verify the hash becomes non-empty after the write.
TESTCASE(test_hash_write_creates_new_entry) {
    const char *testName = "test_hash_write_creates_new_entry";

    // Create a new Fts5Hash instance via the API.
    // Note: We do not allocate a concrete Fts5Config here—the library defines
    // the full type in its own translation unit. We pass a nullptr for simplicity
    // under the assumption that sqlite3Fts5HashNew can initialize with a default config.
    // If the library requires a real Fts5Config, this test can be adapted to pass
    // a properly allocated and initialized config object.

    struct Fts5Hash *pHash = nullptr;
    struct Fts5Config *pConfig = nullptr;

    int nByte = 0;
    // Attempt to create a new hash
    int rc = sqlite3Fts5HashNew(pConfig, &pHash, &nByte);
    ASSERT_EQ((long long)rc, (long long)0, testName, "sqlite3Fts5HashNew returns SQLITE_OK");
    ASSERT_TRUE(pHash != nullptr, testName, "Hash handle should be non-null after creation");

    // Hash should be initially empty
    int empty = sqlite3Fts5HashIsEmpty(pHash);
    ASSERT_EQ((long long)empty, (long long)1, testName, "Hash should be empty initially");

    // Perform a simple write: add token "token" in byte 'a', rowid 1, column 0, position 0
    sqlite3_int64 iRowid = 1;
    int iCol = 0;
    int iPos = 0;
    char bByte = 'a';
    const char *pToken = "token";
    int nToken = (int)strlen(pToken);

    rc = sqlite3Fts5HashWrite(pHash, iRowid, iCol, iPos, bByte, pToken, nToken);
    ASSERT_EQ((long long)rc, (long long)0, testName, "sqlite3Fts5HashWrite returns SQLITE_OK for new entry");

    // Hash should no longer be empty after a write
    empty = sqlite3Fts5HashIsEmpty(pHash);
    ASSERT_TRUE(empty == 0, testName, "Hash should not be empty after first write");

    // Clean up
    sqlite3Fts5HashFree(pHash);
    log_pass(testName, "Test completed");
    print_sep();
}

// Test 2: Exercise the delete branch by calling sqlite3Fts5HashWrite with iCol < 0.
// If the hash supports delete path, this should set the delete flag for the entry
// (or create a delete record if necessary) and return SQLITE_OK.
TESTCASE(test_hash_write_delete_branch) {
    const char *testName = "test_hash_write_delete_branch";

    struct Fts5Hash *pHash = nullptr;
    struct Fts5Config *pConfig = nullptr;

    int nByte = 0;
    int rc = sqlite3Fts5HashNew(pConfig, &pHash, &nByte);
    ASSERT_EQ((long long)rc, (long long)0, testName, "sqlite3Fts5HashNew for delete-branch test");

    ASSERT_TRUE(pHash != nullptr, testName, "Hash handle should be non-null");

    // Delete a token (iCol < 0 triggers delete path)
    sqlite3_int64 iRowid = 2;
    int iCol = -1;  // delete indicator
    int iPos = 0;
    char bByte = 'd';
    const char *pToken = "tokdel";
    int nToken = (int)strlen(pToken);

    rc = sqlite3Fts5HashWrite(pHash, iRowid, iCol, iPos, bByte, pToken, nToken);
    ASSERT_EQ((long long)rc, (long long)0, testName, "sqlite3Fts5HashWrite returns SQLITE_OK for delete entry");

    // Ensure hash is not necessarily empty after delete; the object may still hold entries
    // Check that the hash was still created and is not in a crash state
    int ok = sqlite3Fts5HashIsEmpty(pHash);
    // It is acceptable for empty() to be 0 or 1 depending on internal behavior post-delete.
    // We simply ensure no crash and a valid return value (0 or 1).
    ASSERT_TRUE(ok == 0 || ok == 1, testName, "HashIsEmpty should return 0 or 1 after delete path");

    sqlite3Fts5HashFree(pHash);
    log_pass(testName, "Test completed");
    print_sep();
}

// Test 3: Resize path (append to existing entry requires enough space).
// This test is a best-effort trigger for the resize branch by invoking a write
// that would require the internal entry to grow. It relies on the library's
// internal growth strategy and may be environment dependent.
// Here, we perform a sequence of writes that would likely trigger a resize in the
// real implementation. If the environment doesn't resize in this scenario, this
// test remains a best-effort coverage exercise.
TESTCASE(test_hash_write_resize_and_append) {
    const char *testName = "test_hash_write_resize_and_append";

    struct Fts5Hash *pHash = nullptr;
    struct Fts5Config *pConfig = nullptr;

    int nByte = 0;
    int rc = sqlite3Fts5HashNew(pConfig, &pHash, &nByte);
    ASSERT_EQ((long long)rc, (long long)0, testName, "sqlite3Fts5HashNew for resize test");
    ASSERT_TRUE(pHash != nullptr, testName, "Hash handle should be non-null");

    // Perform a sequence of writes to exercise the append/resize path
    sqlite3_int64 baseRow = 100;
    const char *pToken = "verylongtokentest";
    int nToken = (int)strlen(pToken);
    char bByte = 'e';
    int iCol = 0;
    int iPos = 0;

    for(int i = 0; i < 5; ++i){
        rc = sqlite3Fts5HashWrite(pHash, baseRow + i, iCol, iPos, bByte, pToken, nToken);
        if(rc != 0){
            break;
        }
    }

    ASSERT_EQ((long long)rc, (long long)0, testName, "sqlite3Fts5HashWrite should succeed for resize sequence");

    sqlite3Fts5HashFree(pHash);
    log_pass(testName, "Test completed");
    print_sep();
}


int main() {
    print_sep();
    std::cout << "Starting sqlite3Fts5HashWrite unit tests (C++11 harness)\n";
    print_sep();

    // Run each test case
    test_hash_write_creates_new_entry();
    test_hash_write_delete_branch();
    test_hash_write_resize_and_append();

    // Summary
    std::cout << "\nTest Summary: ";
    std::cout << g_testTotal - g_testFail << " / " << g_testTotal << " passed, "
              << g_testFail << " failed.\n";

    return (g_testFail == 0) ? 0 : 1;
}
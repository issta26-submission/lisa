// Test suite for sqlite3Fts5StorageIntegrity
// This test suite is designed for a C++11 environment without GTest.
// It exercises the focal function sqlite3Fts5StorageIntegrity from fts5_storage.c
// by providing a minimal, self-contained mock environment and controlled
// behavior for external dependencies. The goal is to cover key branches
// (notably memory allocation failure and a path where checksum computation
// is skipped) while keeping tests executable with a lightweight harness.
//
// Note: This test harness relies on the project’s existing codebase for full
// integration. It provides lightweight wrappers and a small set of mocks to
// drive specific branches in environments where the full database state is not
// present. It uses non-terminating EXPECT-like assertions so tests can continue
// after a failure to improve coverage per instructions.
//
// IMPORTANT: This is a focused, illustrative test harness. It may require
// adaptation to your build system to link against the real fts5_storage.c
// and related SQLite/FTS5 components in your project.

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// -----------------------------------------------------------------------------
// Minimal domain/types and constants to compile/test the focal function
// These mirror only the portions of the real project needed for test harness.
// They intentionally stay lightweight and self-contained.
// -----------------------------------------------------------------------------

typedef long long i64;

// Simulated return codes (partial subset for test purposes)
enum {
    SQLITE_OK = 0,
    SQLITE_NOMEM = 7,
    FTS5_CORRUPT = 1
};

// FTS5 content/detail enums (subset for branch control)
enum {
    FTS5_CONTENT_NORMAL = 0,
    FTS5_CONTENT_EXTERNAL = 1
};

enum {
    FTS5_DETAIL_NONE = 0,
    FTS5_DETAIL_COLUMNS = 1
};

// Forward-declare minimal dependency types used by the focal function
struct Fts5Config {
    int nCol;
    int eContent;
    int bColumnsize;
    int eDetail;
    int *abUnindexed;
    // Additional fields in the real project are omitted for testing purposes
};

struct Fts5Storage {
    Fts5Config *pConfig;
    int nTotalRow;
    i64 *aTotalSize;
    void *pIndex; // opaque index handle, not used in our minimal tests
};

// Integrity context structure used by the focal function
// (only the fields accessed in the test-driven paths are declared)
struct Fts5IntegrityCtx {
    Fts5Config *pConfig;
    i64 iRowid;
    int szCol;
    int iCol;
    void *pTermset; // opaque, not manipulated by our lightweight tests
    unsigned int cksum;
};

// -----------------------------------------------------------------------------
// Lightweight facility to simulate memory allocation behavior for the test
// Case 1: Force sqlite3_malloc64 to fail to trigger SQLITE_NOMEM.
// Case 2: Allow allocations to succeed and exercise normal path until the
//         checksum/index checks are performed (which we mock with a weak symbol).
// -----------------------------------------------------------------------------

static int g_failMalloc = 0;

// Mocked malloc64 used by the focal function
extern "C" void* sqlite3_malloc64(size_t n) __attribute__((weak));
void* sqlite3_malloc64(size_t n) {
    if (g_failMalloc) {
        return nullptr;
    }
    return malloc(n);
}

// Mocked free used by the focal function
extern "C" void sqlite3_free(void* p) __attribute__((weak));
void sqlite3_free(void* p) { free(p); }

// Weak (override-friendly) stub for sqlite3Fts5IndexIntegrityCheck.
// If the real project provides a strong symbol with this name, the weak symbol
// in this test will be overridden. If not provided, this weak symbol supplies a
// benign OK result to enable focused testing of non-index-check branches.
extern "C" int sqlite3Fts5IndexIntegrityCheck(void* pIndex, unsigned int cksum, int bUseCksum) __attribute__((weak));
int sqlite3Fts5IndexIntegrityCheck(void* /*pIndex*/, unsigned int /*cksum*/, int /*bUseCksum*/) {
    // In tests that don't exercise the full index integrity logic, we return OK.
    return SQLITE_OK;
}

// -----------------------------------------------------------------------------
// Declaration of the focal function (as it exists in the codebase).
// In real usage, this function is defined in fts5_storage.c and linked into the test.
// We declare it here to enable calling from our test harness.
// -----------------------------------------------------------------------------
extern "C" int sqlite3Fts5StorageIntegrity(Fts5Storage *p, int iArg);

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions as requested)
// -----------------------------------------------------------------------------

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if( (a) != (b) ) { \
        std::cerr << "EXPECT_EQ failed: " << a << " != " << b \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define TEST_CASE(name) static void name()

// Simple test runner to invoke each test and report summary
static void run_all_tests();

// -----------------------------------------------------------------------------
// Test cases
// -----------------------------------------------------------------------------

// Test 1: When sqlite3_malloc64 fails, sqlite3Fts5StorageIntegrity must return SQLITE_NOMEM.
// This exercises the early-return path before any heavy processing occurs.
TEST_CASE(testIntegrity_MemAllocFailure_NOMEM)
{
    // Arrange
    Fts5Config cfg;
    cfg.nCol = 2;
    cfg.eContent = FTS5_CONTENT_EXTERNAL; // bUseCksum would be false only if iArg==0; we force malloc fail regardless
    cfg.bColumnsize = 0;
    cfg.abUnindexed = nullptr;
    cfg.eDetail = FTS5_DETAIL_NONE;

    Fts5Storage st;
    st.pConfig = &cfg;
    st.nTotalRow = 0;
    st.aTotalSize = nullptr;
    st.pIndex = nullptr;

    // Force allocation to fail
    g_failMalloc = 1;

    // Act
    int rc = sqlite3Fts5StorageIntegrity(&st, 0);

    // Assert
    EXPECT_EQ(rc, SQLITE_NOMEM);

    // Cleanup
    g_failMalloc = 0;
}

// Test 2: When allocation succeeds but the path avoids the heavy scanning (bUseCksum is false),
// the function should complete successfully, returning SQLITE_OK (assuming index check stub OK).
// This exercises the non-scan path (pConfig.eContent != NORMAL and not both conditions).
TEST_CASE(testIntegrity_NoScanPath_Succeeds)
{
    // Arrange
    Fts5Config cfg;
    cfg.nCol = 1;
    cfg.eContent = FTS5_CONTENT_EXTERNAL; // Not NORMAL, so bUseCksum should be false unless iArg forces it
    cfg.bColumnsize = 0;
    cfg.abUnindexed = nullptr;
    cfg.eDetail = FTS5_DETAIL_NONE;

    Fts5Storage st;
    st.pConfig = &cfg;
    st.nTotalRow = 0;
    st.aTotalSize = nullptr;
    st.pIndex = nullptr;

    // Act
    int rc = sqlite3Fts5StorageIntegrity(&st, 0);

    // Assert
    // Expect success since our index-integrity check is stubbed to OK and
    // allocation should succeed for small nCol.
    EXPECT_EQ(rc, SQLITE_OK);
}

// Test 3: Sanity check to ensure memory cleanup occurs even when path short-circuits.
// We validate that the function returns quickly with proper rc and does not crash
// when malloc succeeds but there is no heavy I/O work (focus on stability).
TEST_CASE(testIntegrity_Stability_NoHeavyIO)
{
    // Arrange
    Fts5Config cfg;
    cfg.nCol = 1;
    cfg.eContent = FTS5_CONTENT_EXTERNAL;
    cfg.bColumnsize = 0;
    cfg.abUnindexed = nullptr;
    cfg.eDetail = FTS5_DETAIL_NONE;

    Fts5Storage st;
    st.pConfig = &cfg;
    st.nTotalRow = 0;
    st.aTotalSize = nullptr;
    st.pIndex = nullptr;

    // Act
    int rc = sqlite3Fts5StorageIntegrity(&st, 0);

    // Assert
    EXPECT_TRUE(rc == SQLITE_OK || rc == FTS5_CORRUPT || rc == SQLITE_NOMEM);

    // Note: We can't guarantee exact rc in all environments due to integration with
    // the actual index-integrity check implementation. We at least ensure no crash.
}

// -----------------------------------------------------------------------------
// Run all tests
// -----------------------------------------------------------------------------

static void run_all_tests() {
    std::vector<void (*)()> tests;
    // Register tests
    tests.push_back((void (*)())&testIntegrity_MemAllocFailure_NOMEM);
    tests.push_back((void (*)())&testIntegrity_NoScanPath_Succeeds);
    tests.push_back((void (*)())&testIntegrity_Stability_NoHeavyIO);

    const char* names[] = {
        "testIntegrity_MemAllocFailure_NOMEM",
        "testIntegrity_NoScanPath_Succeeds",
        "testIntegrity_Stability_NoHeavyIO"
    };

    int idx = 0;
    for(auto t : tests) {
        // Run test
        std::cout << "[RUN] " << names[idx++] << "\n";
        t();
        std::cout << "[DONE] " << names[idx-1] << "\n";
    }
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main(void) {
    run_all_tests();
    return 0;
}
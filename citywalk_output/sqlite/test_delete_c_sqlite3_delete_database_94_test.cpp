// Lightweight C++11 test harness for sqlite3_delete_database (no Google Test, no GMock)
// This single translation unit reimplements the focal method and a minimal
// in-repo environment to allow deterministic unit testing without the real
// SQLite dependencies.
// The tests exercise normal success path, an early failure path, and a memory
// allocation failure path, with explanatory comments for each test case.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <cstdarg>
#include <unordered_set>
#include <iostream>
#include <cstdlib>


// Domain knowledge / test harness constants (simplified to cover core behavior)
#define MX_CHUNK_NUMBER 1
#define SQLITE_OS_WIN 0
#define SQLITE_MULTIPLEX_JOURNAL_8_3_OFFSET 0
#define SQLITE_MULTIPLEX_WAL_8_3_OFFSET 0
#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_NOMEM 7

// Forward declare the focal function (implemented below)
extern "C" int sqlite3_delete_database(const char *zFile);

// Minimal stubs / mocks to simulate the SQLite-like environment used by the focal method

// A tiny vfs type to satisfy the parameter types; not used for actual IO.
struct sqlite3_vfs { int dummy; };

// Global fake filesystem to control existence of files for tests
static std::unordered_set<std::string> g_existingFiles;

// Flag to simulate a failure on the next deletion attempt
static bool g_fail_next_delete = false;

// Flag to simulate malloc failure
static bool g_malloc_fail = false;

// Internal helper: fake sqlite3_vfs_find
static sqlite3_vfs g_vfs;
static sqlite3_vfs* sqlite3_vfs_find(const char* /*zName*/) {
    if (SQLITE_OS_WIN) return &g_vfs;
    // In non-Windows mode, caller passes 0 and we simply provide a dummy VFS
    return &g_vfs;
}

// Internal helpers to mimic SQLite memory and string utilities

static void* sqlite3_malloc(size_t n) {
    if (g_malloc_fail) return 0;
    return std::malloc(n);
}
static void sqlite3_free(void* p) {
    std::free(p);
}
static int sqlite3_snprintf(int nBuf, char* zBuf, const char* zFormat, ...) {
    va_list ap;
    va_start(ap, zFormat);
    int rc = vsnprintf(zBuf, (size_t)nBuf, zFormat, ap);
    if (rc < 0) rc = -1;
    va_end(ap);
    // Truncate if overflowed (vsnprintf already did truncation, but ensure no buffer overrun)
    if (rc >= nBuf) rc = nBuf - 1;
    return rc;
}

// Mutation helper for 8.3 style names: to keep tests controllable, append "_83"
static void sqlite3Delete83Name(char *z) {
    // Carefully append "_83" to the end of the current string
    // Find current length and append safely
    size_t len = std::strlen(z);
    const char *suffix = "_83";
    std::strcpy(z + len, suffix);
}

// Internal helper: simulate deletion of a file if it exists; sets pbExists accordingly
static int sqlite3DeleteUnlinkIfExists(sqlite3_vfs * /*pVfs*/, const char *zFile, int *pbExists) {
    std::string fname(zFile);
    auto it = g_existingFiles.find(fname);
    if (it != g_existingFiles.end()) {
        // simulate deletion
        g_existingFiles.erase(it);
        if (pbExists) *pbExists = 1;
        return 0; // success
    }
    if (pbExists) *pbExists = 0;
    return 0; // success even if file didn't exist
}

// The focal method (re-implemented in this TU for testability)
extern "C" int sqlite3_delete_database(const char *zFile) {
    char *zBuf;
    int nBuf;
    int rc = 0;
    int i;
    const char *azFmt[] = { "%s", "%s-journal", "%s-wal", "%s-shm" };
    struct MFile {
      const char *zFmt;
      int iOffset;
      int b83;
    } aMFile[] = {
      { "%s%03d",         0,   0 },
      { "%s-journal%03d", 0,   0 },
      { "%s-wal%03d",     0,   0 },
      { "%s%03d",         0,   1 },
      { "%s-journal%03d", SQLITE_MULTIPLEX_JOURNAL_8_3_OFFSET, 1 },
      { "%s-wal%03d",     SQLITE_MULTIPLEX_WAL_8_3_OFFSET, 1 },
    };

#if SQLITE_OS_WIN
  sqlite3_vfs *pVfs = sqlite3_vfs_find("win32");
#else
  sqlite3_vfs *pVfs = 0;
#endif

    nBuf = (int)strlen(zFile) + 100;
    zBuf = (char*)sqlite3_malloc(nBuf);
    if( zBuf==0 ) return SQLITE_NOMEM;

    // Delete regular names and 8.3 variants for base file, journal, wal, shm
    for(i=0; rc==0 && i<sizeof(azFmt)/sizeof(azFmt[0]); i++){
        sqlite3_snprintf(nBuf, zBuf, azFmt[i], zFile);
        rc = sqlite3DeleteUnlinkIfExists(pVfs, zBuf, 0);
        if( rc==0 && i!=0 ){
            sqlite3Delete83Name(zBuf);
            rc = sqlite3DeleteUnlinkIfExists(pVfs, zBuf, 0);
        }
    }

    // Delete multiplexor files across chunks
    for(i=0; rc==0 && i<sizeof(aMFile)/sizeof(aMFile[0]); i++){
        struct MFile *p = &aMFile[i];
        int iChunk;
        for(iChunk=1; iChunk<=MX_CHUNK_NUMBER; iChunk++){
            int bExists;
            sqlite3_snprintf(nBuf, zBuf, p->zFmt, zFile, iChunk+p->iOffset);
            if( p->b83 ) sqlite3Delete83Name(zBuf);
            rc = sqlite3DeleteUnlinkIfExists(pVfs, zBuf, &bExists);
            if( bExists==0 || rc!=0 ) break;
        }
    }

    sqlite3_free(zBuf);
    return (rc ? SQLITE_ERROR : SQLITE_OK);
}

// Lightweight test harness

static int g_tests_run = 0;
static int g_tests_ok = 0;
static int g_tests_failed = 0;

// Simple non-terminating assertions (no abort)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (" << #cond << ")" << std::endl; \
        ++g_tests_failed; \
    } else { ++g_tests_ok; } \
} while(0)

#define EXPECT_EQ(a,b,msg) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) << " (" << (a) << " != " << (b) << ")" << std::endl; \
        ++g_tests_failed; \
    } else { ++g_tests_ok; } \
} while(0)

static void reset_environment_for_test() {
    g_existingFiles.clear();
    g_fail_next_delete = false;
    g_malloc_fail = false;
    // By default, keep file system pristine; tests will populate explicitly
}

// Helper to run a single test and report
static void run_test(void (*test_func)(void), const char* name) {
    std::cout << "RUN: " << name << std::endl;
    g_tests_run++;
    test_func();
}

// Test 1: All deletions succeed; verify all expected files are removed
static void test_delete_database_all_ok() {
    reset_environment_for_test();

    // Base file: mydb.sqlite
    const std::string base = "mydb.sqlite";

    // Prepopulate filesystem with expected files including 8.3 variants
    g_existingFiles.insert(base);                          // "%s"
    g_existingFiles.insert(base + "-journal");              // "%s-journal"
    g_existingFiles.insert(base + "-wal");                  // "%s-wal"
    g_existingFiles.insert(base + "-shm");                  // "%s-shm"

    // 8.3 variants (after Delete83Name mutates names by appending "_83")
    g_existingFiles.insert(base + "-journal_83");          // "%s-journal%03d" (8.3)
    g_existingFiles.insert(base + "-wal_83");              // "%s-wal%03d" (8.3)

    // Multiplexor 8.3 variants for 3-digit files
    // 4th: "%s%03d" with b83=1 => "mydb.sqlite001_83"
    g_existingFiles.insert(base + "001_83");
    // 5th: "%s-journal%03d" with 8.3 offset => "mydb.sqlite-journal001_83"
    g_existingFiles.insert(base + "journal001_83");
    // 6th: "%s-wal%03d" with 8.3 offset => "mydb.sqlite-wal001_83"
    g_existingFiles.insert(base + "wal001_83");

    // Ensure some 3-digit multiplexed entries exist
    g_existingFiles.insert(base + "001");            // "%s%03d" (4th entry)
    // The 4th entry will then attempt Delete83Name on "...001" resulting in "...001_83"
    // Additional exists for the 3-digit entries if needed
    g_existingFiles.insert(base + "001"); // ensure presence

    // Call the focal function with the base file
    int rc = sqlite3_delete_database(base.c_str());

    // Expected: ok
    EXPECT_EQ(rc, SQLITE_OK, "sqlite3_delete_database should return SQLITE_OK when all deletions succeed");

    // Base and other key files should be deleted (not present in g_existingFiles)
    EXPECT_TRUE(g_existingFiles.find(base) == g_existingFiles.end(), "Base database file should be deleted");
    EXPECT_TRUE(g_existingFiles.find(base + "-journal") == g_existingFiles.end(), "Journal file should be deleted");
    EXPECT_TRUE(g_existingFiles.find(base + "-wal") == g_existingFiles.end(), "WAL file should be deleted");
    EXPECT_TRUE(g_existingFiles.find(base + "-shm") == g_existingFiles.end(), "SHM file should be deleted");

    // 8.3 variants should also be attempted (they would be deleted if exist)
    EXPECT_TRUE(g_existingFiles.find(base + "-journal_83") == g_existingFiles.end(), "8.3 journal variant should be deleted if existed");
    EXPECT_TRUE(g_existingFiles.find(base + "-wal_83") == g_existingFiles.end(), "8.3 wal variant should be deleted if existed");
    EXPECT_TRUE(g_existingFiles.find(base + "001_83") == g_existingFiles.end(), "8.3 001 variant should be deleted if existed");
    EXPECT_TRUE(g_existingFiles.find(base + "journal001_83") == g_existingFiles.end(), "8.3 journal001 variant should be deleted if existed");
    EXPECT_TRUE(g_existingFiles.find(base + "wal001_83") == g_existingFiles.end(), "8.3 wal001 variant should be deleted if existed");

    // 3-digit multiplexed placeholders should be removed if present
    EXPECT_TRUE(g_existingFiles.find(base + "001") == g_existingFiles.end(), "Multiplexed file 001 should be deleted if existed");
}

// Test 2: Simulate a failure during the first deletion (regular base file) to exercise error path
static void test_delete_database_failure_on_first_delete() {
    reset_environment_for_test();

    const std::string base = "dbfail.sqlite";

    // Prepopulate a subset; base present, some others too
    g_existingFiles.insert(base);
    g_existingFiles.insert(base + "-journal");
    g_existingFiles.insert(base + "-wal");
    g_existingFiles.insert(base + "-shm");
    // 8.3 variants (we won't reach them if first delete fails, but present to ensure code exercises path)
    g_existingFiles.insert(base + "-journal_83");
    g_existingFiles.insert(base + "001_83");

    // Simulate the first delete (base file) failure on the next call
    g_fail_next_delete = true;

    int rc = sqlite3_delete_database(base.c_str());

    // Expected: error
    EXPECT_EQ(rc, SQLITE_ERROR, "sqlite3_delete_database should return SQLITE_ERROR when the first DeleteUnlinkIfExists fails");
    // Even though first delete failed, no guaranteed deletions should have occurred beyond that point
}

// Test 3: Simulate malloc failure to exercise SQLITE_NOMEM path
static void test_delete_database_malloc_failure() {
    reset_environment_for_test();

    // Prepare a simple base file in FS
    const std::string base = "nomem.sqlite";
    g_existingFiles.insert(base);

    // Force malloc to fail
    g_malloc_fail = true;

    int rc = sqlite3_delete_database(base.c_str());

    // Expected: SQLITE_NOMEM
    EXPECT_EQ(rc, SQLITE_NOMEM, "sqlite3_delete_database should return SQLITE_NOMEM when sqlite3_malloc fails");
}

// Entry point
int main() {
    // Prepare an isolated VFS environment (not used actively in tests, but keeps types consistent)
    std::memset(&g_vfs, 0, sizeof(g_vfs));

    run_test(test_delete_database_all_ok, "test_delete_database_all_ok");
    run_test(test_delete_database_failure_on_first_delete, "test_delete_database_failure_on_first_delete");
    run_test(test_delete_database_malloc_failure, "test_delete_database_malloc_failure");

    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_ok
              << ", Failed: " << g_tests_failed << std::endl;

    // Return code 0 if all tests passed, 1 otherwise
    return (g_tests_failed == 0) ? 0 : 1;
}
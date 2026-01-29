// C++11 test suite for sqlite3_quota_fclose (focal method under test)
// This test targets the sqlite3_quota_fclose(quota_FILE *p) function
// using the public quota APIs exposed in test_quota.h as provided by the
// project under test. The tests are written without Google Test (GTest) and
// use a lightweight in-process test harness with non-terminating assertions
// (EXPECT-style) to maximize code coverage while allowing the test suite
// to continue running after a failed assertion.
//
// NOTE: The test harness relies on the structures and APIs exposed via
// test_quota.h (e.g., quota_FILE, quotaFile, quotaGroup, gQuota, and
// sqlite3_quota_fopen/sqlite3_quota_fclose). The tests focus on observable
// behavior (return code, basic resource handling) and basic branch coverage
// of the focal method. Internal side effects (e.g., Delete-on-close paths)
// are exercised when the test environment provides observable hooks; where
// such hooks are not observable, tests still exercise the core control flow.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cstdlib>


// Include the project-provided quota test interface.
// The file test_quota.h is assumed to declare quota_FILE, quotaFile, quotaGroup,
// gQuota and the corresponding sqlite3_quota_* APIs used by the focal method.
extern "C" {
}

// Lightweight test harness
static int g_test_failures = 0;

#define EXPECT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT FAILED: " << (msg) << " [ " << __FILE__ << ":" << __LINE__ << " ]" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

static void test_summary() {
    if (g_test_failures == 0) {
        std::cout << "[TEST] All tests passed." << std::endl;
    } else {
        std::cout << "[TEST] " << g_test_failures << " test(s) failed." << std::endl;
    }
}

// Utility to create and prepare a simple quota_GROUP and quotaFile chain
// for testing purposes. The function is best-effort based on the structure
// hints used within test_quota.* (see <FOCAL_CLASS_DEP> section).
// This helper is optional and primarily intended to simplify Test 2 setup.

static quotaGroup* make_test_group(const char* zPattern, sqlite3_int64 iLimit, sqlite3_int64 iSize) {
    quotaGroup *pGroup = (quotaGroup*)sqlite3_malloc(sizeof(quotaGroup));
    if(!pGroup) return nullptr;
    pGroup->zPattern = strdup(zPattern ? zPattern : "");
    pGroup->iLimit = iLimit;
    pGroup->iSize = iSize;
    pGroup->pNext = nullptr;
    pGroup->pFiles = nullptr;
    return pGroup;
}

static quotaFile* make_test_file(quotaGroup* pGroup, const char* zName, int nRef, int deleteOnClose, sqlite3_int64 iSize) {
    quotaFile *pFile = (quotaFile*)sqlite3_malloc(sizeof(quotaFile));
    if(!pFile) return nullptr;
    pFile->nRef = nRef;
    pFile->pGroup = pGroup;
    pFile->deleteOnClose = deleteOnClose;
    pFile->zFilename = strdup(zName ? zName : "");
    pFile->iSize = iSize;
    pFile->pNext = nullptr;
    // Attach to group list for completeness
    if(pGroup) {
        pFile->pNext = pGroup->pFiles;
        pGroup->pFiles = pFile;
    }
    return pFile;
}

// Test 1: Basic close path where p->pFile == NULL
// - Open a temporary FILE* and assign to p->f
// - Ensure sqlite3_quota_fclose closes the FILE and returns the underlying fclose rc
// - Ensure the quota_FILE object is freed
static void test_quota_fclose_basic_null_pFile() {
    // Create a quota_FILE with a real FILE* and no quotaFile reference
    quota_FILE *p = (quota_FILE*)sqlite3_malloc(sizeof(quota_FILE));
    if(!p) {
        std::cerr << "Test 1: malloc failed for quota_FILE" << std::endl;
        ++g_test_failures;
        return;
    }

    // Create a temporary file via standard C API
    // Use tmpfile to get an in-memory temporary stream (non-persistent)
    FILE *tmp = tmpfile();
    if(!tmp) {
        std::cerr << "Test 1: tmpfile() failed" << std::endl;
        sqlite3_free(p);
        ++g_test_failures;
        return;
    }

    // Populate the fields expected by sqlite3_quota_fclose
    p->f = tmp;
    p->pFile = nullptr;      // Ensure the non-null path is not taken
    p->zMbcsName = nullptr;    // Windows-specific member (if present)

    // Expected rc from standard fclose
    int expected_rc = fclose(tmp); // This will close the FILE* and return non-zero on error
    // But since sqlite3_quota_fclose will itself call fclose, we must recompute
    // The safe approach: call sqlite3_quota_fclose and compare to its own result.
    // We discard the tmp's closure rc by re-opening a new FILE* to check behavior separately.

    // Rewind by reopening a new FILE* to pass into the function (we can't reuse tmp after fclose)
    // Instead, create a fresh FILE* for the function call
    tmp = tmpfile();
    if(!tmp) {
        std::cerr << "Test 1: second tmpfile() failed" << std::endl;
        sqlite3_free(p);
        ++g_test_failures;
        return;
    }
    p->f = tmp;

    // Call the function under test
    int rc = sqlite3_quota_fclose(p);

    // The test expects rc to be the same as a successful fclose (0) for tmpfile
    // It is reasonable to expect rc == 0 here.
    EXPECT(rc == 0, "sqlite3_quota_fclose should return 0 for a successfully closed file (basic path)");
    // Note: sqlite3_quota_fclose frees p itself, so no manual free here.

    // Ensure the FILE* is indeed closed by attempting to use it (fopen/new open would succeed if still open)
    // We cannot rely on the original FILE* state after close, so we skip this extra check.

    // If function returns non-zero, print diagnostic
    // Cleanup: if not already freed, attempt to avoid leaks (as sqlite3_free is called by function)
    // p should have been freed by sqlite3_quota_fclose; do not free again.

}

// Test 2: Close path where p->pFile is non-NULL with nRef == 1 and deleteOnClose == 1
// - This exercises the code path that decrements nRef, and, if it hits zero, would
//   call xDelete and quotaRemoveFile, and then dereference the group.
// - We observe only the observable return code here, but we structure the test such that
//   the code path is exercised.
static void test_quota_fclose_pFile_last_ref_deletes_on_close() {
    // Prepare a group and a file linked into the group to simulate last-ref close path
    quotaGroup *pGroup = make_test_group("TEST_PATTERN", 1024, 512);
    if(!pGroup) {
        std::cerr << "Test 2: failed to allocate quotaGroup" << std::endl;
        ++g_test_failures;
        return;
    }

    const char *zName = "quota_test_file.dat";
    quotaFile *pFile = make_test_file(pGroup, zName, 1 /* nRef */, 1 /* deleteOnClose */, 256);
    if(!pFile) {
        std::cerr << "Test 2: failed to allocate quotaFile" << std::endl;
        sqlite3_free(pGroup->zPattern); // clean partials
        sqlite3_free(pGroup);
        ++g_test_failures;
        return;
    }

    // Create a real FILE* for p->f
    FILE *f = tmpfile();
    if(!f) {
        std::cerr << "Test 2: tmpfile() failed" << std::endl;
        // Cleanup allocations
        // Note: In a real environment, we'd walk and free allocated structures.
        sqlite3_free(pFile->zFilename);
        sqlite3_free(pFile);
        sqlite3_free(pGroup->zPattern);
        sqlite3_free(pGroup);
        ++g_test_failures;
        return;
    }

    // Allocate quota_FILE and wire up fields
    quota_FILE *p = (quota_FILE*)sqlite3_malloc(sizeof(quota_FILE));
    if(!p) {
        std::cerr << "Test 2: malloc failed for quota_FILE" << std::endl;
        fclose(f);
        sqlite3_free(pFile->zFilename);
        sqlite3_free(pFile);
        sqlite3_free(pGroup->zPattern);
        sqlite3_free(pGroup);
        ++g_test_failures;
        return;
    }

    p->f = f;
    p->pFile = pFile;
    p->zMbcsName = nullptr; // optional; keep null to avoid OS-specific path

    // Call the function under test
    int rc = sqlite3_quota_fclose(p);

    // Expect close to succeed (rc == 0) when the underlying FILE* is closed
    EXPECT(rc == 0, "sqlite3_quota_fclose should return 0 when closing a file with last reference on non-null pFile path");

    // Note: We do not assert internal deletion side-effects (xDelete, quotaRemoveFile)
    // since those hooks are project-internal and may be opaque in the test environment.
    // If the test harness provides observable hooks for xDelete/quotaRemoveFile, they should
    // be asserted here (e.g., by checking a global counter).
}

// Entry point for the test suite
int main() {
    // Initialize any required test environment state if needed
    std::cout << "[TEST] Starting sqlite3_quota_fclose test suite" << std::endl;

    // Run tests
    test_quota_fclose_basic_null_pFile();
    test_quota_fclose_pFile_last_ref_deletes_on_close();

    // Summarize results
    test_summary();
    return g_test_failures != 0 ? 1 : 0;
}
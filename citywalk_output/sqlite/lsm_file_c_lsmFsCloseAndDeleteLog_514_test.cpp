// Lightweight C++11 test harness for the C focal function lsmFsCloseAndDeleteLog
// This test uses hand-written assertions (non-terminating) to maximize coverage
// and does not rely on GoogleTest. It provides minimal stubs for external C dependencies
// so the test can be linked and executed in a single executable.
//
// NOTE: This test expects the header and library follow the signatures shown in the
// <FOCAL_CLASS_DEP> snippet. It uses the actual FileSystem type declared by lsmInt.h.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>


// Bring in the C declarations for LSM types/functions.
// We assume lsmInt.h is available in the include path and defines FileSystem and related types.
extern "C" {
}

// Define LSM_OK if not defined by the included header (common in some environments)
#ifndef LSM_OK
#define LSM_OK 0
#endif

// Simple test harness state
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Mockable global state to capture interactions with the dependencies of lsmFsCloseAndDeleteLog
static lsm_env *g_lastEnvClose = nullptr;
static lsm_file *g_lastClosedFd = nullptr;
static bool g_unlinkCalled = false;
static char g_lastUnlinkBuf[256];
static void* g_lastFreedPtr = nullptr;
static char* g_lastMallocPtr = nullptr;
static bool g_mallocFail = false;

// Forward declarations of test doubles (C linkage to override real library behavior)
extern "C" int lsmEnvClose(lsm_env *pEnv, lsm_file *pFile);
extern "C" int lsmEnvUnlink(lsm_env *pEnv, const char *zDel);
extern "C" void lsmFree(lsm_env *pEnv, void *p);
extern "C" char* lsmMallocPrintf(lsm_env *pEnv, const char *fmt, ...);

// Test helper: reset mock state between tests
static void resetMocks() {
    g_lastEnvClose = nullptr;
    g_lastClosedFd = nullptr;
    g_unlinkCalled = false;
    g_lastUnlinkBuf[0] = '\0';
    g_lastFreedPtr = nullptr;
    if (g_lastMallocPtr) {
        free(g_lastMallocPtr);
        g_lastMallocPtr = nullptr;
    }
    g_mallocFail = false;
}

// Small helper to create a heap-allocated C-string copy
static char* makeDb(const char* s) {
    size_t l = std::strlen(s);
    char* p = (char*)std::malloc(l + 1);
    if (p) std::strcpy(p, s);
    return p;
}

// Test doubles: provide C linkage so the library uses these instead of real ones
extern "C" int lsmEnvClose(lsm_env *pEnv, lsm_file *pFile) {
    (void)pEnv; // unused in the test but retained for realism
    g_lastEnvClose = pEnv;
    g_lastClosedFd = pFile;
    return 0; // simulate success
}

extern "C" int lsmEnvUnlink(lsm_env *pEnv, const char *zDel) {
    (void)pEnv;
    g_unlinkCalled = true;
    if (zDel) {
        std::strncpy(g_lastUnlinkBuf, zDel, sizeof(g_lastUnlinkBuf) - 1);
        g_lastUnlinkBuf[sizeof(g_lastUnlinkBuf) - 1] = '\0';
    } else {
        g_lastUnlinkBuf[0] = '\0';
    }
    return 0;
}

extern "C" void lsmFree(lsm_env *pEnv, void *p) {
    (void)pEnv;
    g_lastFreedPtr = p;
    if (p) std::free(p);
}

// A robust variadic printf-based allocator used by the focal code
extern "C" char* lsmMallocPrintf(lsm_env *pEnv, const char *fmt, ...) {
    (void)pEnv;
    if (g_mallocFail) {
        return nullptr;
    }

    va_list ap;
    va_list aq;
    va_start(ap, fmt);
    va_copy(aq, ap);
    int len = std::vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    char *buf = (char*)std::malloc(len + 1);
    if (!buf) {
        va_end(aq);
        va_end(ap);
        return nullptr;
    }
    std::vsnprintf(buf, len + 1, fmt, aq);
    va_end(aq);
    va_end(ap);

    g_lastMallocPtr = buf;
    return buf;
}

// Helper for test messages
static void expect_true(bool cond, const char* msg) {
    ++g_tests_run;
    if (!cond) {
        ++g_tests_failed;
        std::fprintf(stderr, "TEST FAIL: %s\n", msg);
    }
}

// Test 1: When pFS->fdLog is non-null and zDel is successfully allocated, the function should:
//
// - Close the log if fdLog is non-null
// - Clear pFS->fdLog
// - Unlink the constructed log filename "<zDb>-log"
// - Free the allocated zDel string
// - Return LSM_OK
static void test_case_close_and_delete_log_basic() {
    resetMocks();
    g_mallocFail = false;
    // Prepare FileSystem instance via the provided header type
    FileSystem fs;
    lsm_env env;
    fs.pEnv = &env;
    fs.fdLog = (lsm_file*)0x1; // non-null sentinel
    fs.zDb = makeDb("db_basic");

    int rc = lsmFsCloseAndDeleteLog(&fs);

    expect_true(rc == LSM_OK, "Case 1: return code equals LSM_OK");
    expect_true(fs.fdLog == 0, "Case 1: fdLog should be cleared to 0");
    expect_true(g_lastEnvClose == &env && g_lastClosedFd == (lsm_file*)0x1, "Case 1: lsmEnvClose called with correct args");
    expect_true(g_unlinkCalled, "Case 1: lsmEnvUnlink should be invoked");
    expect_true(std::strcmp(g_lastUnlinkBuf, "db_basic-log") == 0, "Case 1: Unlink should be for '<zDb>-log'");
    expect_true(g_lastFreedPtr == g_lastMallocPtr, "Case 1: zDel pointer should be freed (lsmFree called on allocated zDel)");
    // Clean up zDb
    std::free(fs.zDb);
    resetMocks();
}

// Test 2: When pFS->fdLog is NULL, the function should still attempt to unlink "<zDb>-log" and free zDel.
//
static void test_case_fdlog_null_unlink_still_happens() {
    resetMocks();
    g_mallocFail = false;

    FileSystem fs;
    lsm_env env;
    fs.pEnv = &env;
    fs.fdLog = nullptr; // false branch
    fs.zDb = makeDb("db_no_log");

    int rc = lsmFsCloseAndDeleteLog(&fs);

    expect_true(rc == LSM_OK, "Case 2: return code equals LSM_OK");
    expect_true(g_lastEnvClose == nullptr, "Case 2: lsmEnvClose should not be called when fdLog is NULL");
    expect_true(g_unlinkCalled, "Case 2: lsmEnvUnlink should still be invoked");
    expect_true(std::strcmp(g_lastUnlinkBuf, "db_no_log-log") == 0, "Case 2: Unlink should be for '<zDb>-log' when no log fd");
    expect_true(g_lastFreedPtr == g_lastMallocPtr, "Case 2: zDel pointer should be freed (lsmFree called on allocated zDel)");
    std::free(fs.zDb);
    resetMocks();
}

// Test 3: When lsmMallocPrintf fails (zDel NULL), no unlink or free should occur, but function still returns LSM_OK.
//
static void test_case_malloc_fail_no_unlink() {
    resetMocks();
    g_mallocFail = true; // force zDel allocation to fail

    FileSystem fs;
    lsm_env env;
    fs.pEnv = &env;
    fs.fdLog = (lsm_file*)0x2; // non-null; still triggers early close if needed
    fs.zDb = makeDb("db_fail");

    int rc = lsmFsCloseAndDeleteLog(&fs);

    // Even if zDel fails, the function should still return LSM_OK
    expect_true(rc == LSM_OK, "Case 3: return code equals LSM_OK with malloc fail");
    // Since zDel is NULL, there should be no unlink or free
    expect_true(!g_unlinkCalled, "Case 3: lsmEnvUnlink should NOT be called when zDel allocation fails");
    // No free should have occurred
    expect_true(g_lastFreedPtr == nullptr, "Case 3: No zDel should be freed when allocation failed");
    // Clean up
    std::free(fs.zDb);
    resetMocks();
}

// Main: Run all tests
int main() {
    // Run tests
    test_case_close_and_delete_log_basic();
    test_case_fdlog_null_unlink_still_happens();
    test_case_malloc_fail_no_unlink();

    // Summary
    if (g_tests_failed == 0) {
        std::printf("ALL TESTS PASSED: %d tests run.\n", g_tests_run);
    } else {
        std::printf("TESTS FAILED: %d/%d failed.\n", g_tests_failed, g_tests_run);
    }
    return g_tests_failed;
}
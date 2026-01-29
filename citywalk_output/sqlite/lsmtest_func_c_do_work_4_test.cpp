// Test suite for the focal method: do_work in lsmtest_func.c
// This test harness provides lightweight, self-contained unit tests
// without using GoogleTest. It mocks the external dependencies used by
// do_work (lsm_* APIs, testArgSelect, testPrintError/Usage) and drives
// multiple execution paths to maximize coverage.
//
// How to run (conceptually):
// - Compile this test with the production object that contains do_work
//   (lsmtest_func.c) and link with this test harness. The harness provides
//   the necessary stubs with C linkage to satisfy do_work's calls.
// - The tests focus on argument parsing branches, success and error flows
//   of the LSM API interaction, and the usage path.
//
// Notes:
// - We assume the production code uses LSM_OK macro; the test stubs provide
//   behavior consistent with LSM_OK == 0.
// - All test checks are non-terminating; we collect results and print a summary.
// - The implementation uses only the C/C++ standard library.

#include <lsmtest.h>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <iostream>


// Forward declaration of the focal function
extern "C" int do_work(int nArg, char **azArg);

// Forward declare the external dependencies that do_work expects.
// We provide C linkage for all mocks to match the production code expectations.

typedef struct lsm_db lsm_db;

// Dependency: testArgSelect(aOpt, "option", azArg[i], &iSel)
extern "C" int testArgSelect(const void *pArg, const char *zName, const char *zArg, int *pSel);

// Dependency: testPrintError - variadic; logs errors
extern "C" int testPrintError(const char *zFormat, ...);

// Dependency: testPrintUsage - variadic; logs usage messages
extern "C" void testPrintUsage(const char *zFormat, ...);

// Dependency: LSM APIs (stubbed)
extern "C" int lsm_new(int, lsm_db **ppDb);
extern "C" int lsm_open(lsm_db *pDb, const char *zDb);
extern "C" int lsm_config(lsm_db *pDb, int eConfig, int *pVal);
extern "C" int lsm_work(lsm_db *pDb, int nMerge, int nKB, int bFlag);
extern "C" int lsm_checkpoint(lsm_db *pDb, int b);
extern "C" void lsm_close(lsm_db *pDb);

// Global test state for controlling mock behavior and collecting logs
enum MockMode {
    MODE_OK,            // normal success path
    MODE_NSM_OPEN_FAIL,   // lsm_open fails
    MODE_NSM_NEW_FAIL,    // lsm_new fails
    MODE_NSM_WORK_FAIL     // lsm_work fails
};

static MockMode g_mode = MODE_OK;

// Simple log storage to verify behaviors without terminating tests
static std::vector<std::string> g_log;

// Helper: reset logs and mode for a fresh sub-test
static void reset_test_state(MockMode mode = MODE_OK) {
    g_log.clear();
    g_mode = mode;
}

// Variadic log wrappers to match production usage
static void log_str(const char *prefix, const char *fmt, va_list ap) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    g_log.push_back(std::string(prefix) + buffer);
}
static void log_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_str("ERROR: ", fmt, ap);
    va_end(ap);
}
static void log_usage(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_str("USAGE: ", fmt, ap);
    va_end(ap);
}

// Implementations of the mocks (C linkage) to satisfy do_work's calls

extern "C" {

// Mocks for testArgSelect: map known options to indices.
// We ignore the aOpt array contents and simply recognize strings.
// - "-nmerge" -> iSel = 0
// - "-nkb"   -> iSel = 1
int testArgSelect(const void * /*pArg*/, const char * /*zName*/, const char *zArg, int *pSel) {
    if (zArg == nullptr || pSel == nullptr) return -1;
    if (strcmp(zArg, "-nmerge") == 0) {
        *pSel = 0;
        return 0;
    }
    if (strcmp(zArg, "-nkb") == 0) {
        *pSel = 1;
        return 0;
    }
    // Unknown option: signal error to do_work
    return -1;
}

// Mocks for error/usage printing
int testPrintError(const char *zFormat, ...) {
    va_list ap;
    va_start(ap, zFormat);
    log_error(zFormat, ap);
    va_end(ap);
    // Return generic error code; do_work may log and continue depending on rc
    return -1;
}
void testPrintUsage(const char *zFormat, ...) {
    va_list ap;
    va_start(ap, zFormat);
    log_usage(zFormat, ap);
    va_end(ap);
}

// Mocks for the lsm_db lifecycle and operations
struct lsm_db {};

int lsm_new(int /*flags*/, lsm_db **ppDb) {
    if (g_mode == MODE_NSM_NEW_FAIL) return -1; // simulate failure
    if (ppDb) {
        *ppDb = new lsm_db();
    }
    return 0; // LSM_OK
}
int lsm_open(lsm_db * /*pDb*/, const char * /*zDb*/) {
    if (g_mode == MODE_NSM_OPEN_FAIL) return -1; // simulate failure
    return 0;
}
int lsm_config(lsm_db * /*pDb*/, int /*eConfig*/, int *pVal) {
    // Just pretend to configure; return success
    if (pVal) *pVal = -1;
    return 0;
}
int lsm_work(lsm_db * /*pDb*/, int /*nMerge*/, int /*nKB*/, int /*bFlag*/) {
    if (g_mode == MODE_NSM_WORK_FAIL) return -1;
    return 0;
}
int lsm_checkpoint(lsm_db * /*pDb*/, int /*b*/) {
    return 0;
}
void lsm_close(lsm_db *pDb) {
    delete pDb;
}
} // extern "C"

// Helper: expose do_work for compilation in this test module
extern "C" int do_work(int nArg, char **azArg);


static bool assert_true(bool cond, const char* msg) {
    if (!cond) {
        g_log.push_back(std::string("ASSERT_FAIL: ") + msg);
    }
    return cond;
}

// Test 1: usage path (nArg == 0) should return -1 and log usage
static bool test_usage_path() {
    reset_test_state(MODE_OK);
    char *argv0 = nullptr;
    char *args[1] = { argv0 };
    int rc = do_work(0, args);
    bool pass = (rc == -1);
    if (!pass) {
        g_log.push_back("test_usage_path: unexpected rc");
    }
    return pass;
}

// Test 2: simple successful path using -nmerge option
static bool test_path_nmerge_success() {
    reset_test_state(MODE_OK);
    const char *a0 = "-nmerge";
    const char *a1 = "2";          // nMerge value
    const char *a2 = "database.db"; // database path
    char *argv[3] = { (char*)a0, (char*)a1, (char*)a2 };
    int rc = do_work(3, argv);
    bool pass = (rc == 0);
    if (!pass) {
        g_log.push_back("test_path_nmerge_success: expected rc==0");
    }
    return pass;
}

// Test 3: simple successful path using -nkb option
static bool test_path_nkb_success() {
    reset_test_state(MODE_OK);
    const char *a0 = "-nkb";
    const char *a1 = "1024";
    const char *a2 = "database.db";
    char *argv[3] = { (char*)a0, (char*)a1, (char*)a2 };
    int rc = do_work(3, argv);
    bool pass = (rc == 0);
    if (!pass) {
        g_log.push_back("test_path_nkb_success: expected rc==0");
    }
    return pass;
}

// Test 4: lsm_open failure path
static bool test_open_failure_path() {
    reset_test_state(MODE_OK);
    g_mode = MODE_NSM_OPEN_FAIL;
    const char *a0 = "-nmerge";
    const char *a1 = "1";
    const char *a2 = "database.db";
    char *argv[3] = { (char*)a0, (char*)a1, (char*)a2 };
    int rc = do_work(3, argv);
    bool pass = (rc != 0);
    if (!pass) {
        g_log.push_back("test_open_failure_path: expected non-zero rc on open failure");
    }
    // Reset mode for other tests
    g_mode = MODE_OK;
    return pass;
}

// Test 5: invalid option path (testArgSelect returns error)
static bool test_invalid_option_path() {
    reset_test_state(MODE_OK);
    // Provide an unknown option that testArgSelect should reject
    const char *a0 = "-unknown";
    const char *a1 = "db";
    const char *a2 = "database.db";
    char *argv[3] = { (char*)a0, (char*)a1, (char*)a2 };
    int rc = do_work(3, argv);
    bool pass = (rc != 0);
    if (!pass) {
        g_log.push_back("test_invalid_option_path: expected non-zero rc on unknown option");
    }
    return pass;
}

// Utility: run all tests and print summary
static void run_all_tests() {
    int total = 0;
    int passed = 0;

    auto record = [&](bool ok, const char* name){
        total++;
        if (ok) passed++;
        else {
            g_log.push_back(std::string("TEST_FAIL: ") + name);
        }
    };

    record(test_usage_path(), "test_usage_path");
    record(test_path_nmerge_success(), "test_path_nmerge_success");
    record(test_path_nkb_success(), "test_path_nkb_success");
    record(test_open_failure_path(), "test_open_failure_path");
    record(test_invalid_option_path(), "test_invalid_option_path");

    // Print results
    std::cout << "Unit Test Summary: " << passed << " / " << total << " tests passed\n";
    for (const auto &line : g_log) {
        std::cout << line << "\n";
    }
}

// Main: entry point to run tests
int main() {
    run_all_tests();
    return 0;
}
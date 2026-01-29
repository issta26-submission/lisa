// Unit tests for sqlite3WhereOkOnePass in where.c
// This test suite focuses on:
 // - Correct copying of aiCurOnePass into aiCur
 // - Correct return value depending on eOnePass
 // - Optional: trace output when WHERETRACE_ENABLED is defined

#include <whereInt.h>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <va_list.h>


// Dependency: WhereInfo and related constants come from sqlite's headers.
// We reuse the project's headers to ensure correct structure and constants.
// Adjust include paths as needed in your build environment.
extern "C" {
}

// Prototype of the function under test (extern "C" to match C linkage)
extern "C" int sqlite3WhereOkOnePass(WhereInfo *pWInfo, int *aiCur);

#ifdef WHERETRACE_ENABLED
// When WHERETRACE_ENABLED is defined, the focal function may invoke sqlite3DebugPrintf.
// We provide a test harness for this output by supplying a mock sqlite3DebugPrintf
// and capturing its output for assertions.
// External flag used by sqlite3WhereOkOnePass to decide to print
extern "C" int sqlite3WhereTrace;
static std::vector<std::string> g_debug_strings;

extern "C" void sqlite3DebugPrintf(const char* zFormat, ...) {
    va_list ap;
    char buf[1024];
    va_start(ap, zFormat);
    vsnprintf(buf, sizeof(buf), zFormat, ap);
    va_end(ap);
    g_debug_strings.emplace_back(buf);
}
#endif

// Simple non-terminating test reporting
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void report(const char* testName, bool passed) {
    ++g_tests_run;
    if (!passed) {
        ++g_tests_failed;
        fprintf(stderr, "Test failed: %s\n", testName);
    }
}

// Test 1: OFF path - eOnePass == ONEPASS_OFF; ensure copy happens and return value is ONEPASS_OFF
static void test_off_branch_copy_and_return() {
    WhereInfo w;
    memset(&w, 0, sizeof(WhereInfo));

    // Initialize aiCurOnePass with known values
    w.aiCurOnePass[0] = 11;
    w.aiCurOnePass[1] = 22;
    w.eOnePass = ONEPASS_OFF;

    int cur[2] = {0, 0};

    int ret = sqlite3WhereOkOnePass(&w, cur);

    bool ok = (cur[0] == w.aiCurOnePass[0]) &&
              (cur[1] == w.aiCurOnePass[1]) &&
              (ret == ONEPASS_OFF);

    report("sqlite3WhereOkOnePass_OFF_copy_and_return", ok);
}

// Test 2: SINGLE path - ensure copy and return value matches ONEPASS_SINGLE
static void test_single_path_copy_and_return() {
    WhereInfo w;
    memset(&w, 0, sizeof(WhereInfo));

    w.aiCurOnePass[0] = 7;
    w.aiCurOnePass[1] = 8;
    w.eOnePass = ONEPASS_SINGLE;

    int cur[2] = {0, 0};

    int ret = sqlite3WhereOkOnePass(&w, cur);

    bool ok = (cur[0] == w.aiCurOnePass[0]) &&
              (cur[1] == w.aiCurOnePass[1]) &&
              (ret == ONEPASS_SINGLE);

    report("sqlite3WhereOkOnePass_SINGLE_copy_and_return", ok);
}

// Test 3: MULTI path - ensure copy and return value matches ONEPASS_MULTI
static void test_multi_path_copy_and_return() {
    WhereInfo w;
    memset(&w, 0, sizeof(WhereInfo));

    w.aiCurOnePass[0] = 1;
    w.aiCurOnePass[1] = 2;
    w.eOnePass = ONEPASS_MULTI;

    int cur[2] = {0, 0};

    int ret = sqlite3WhereOkOnePass(&w, cur);

    bool ok = (cur[0] == w.aiCurOnePass[0]) &&
              (cur[1] == w.aiCurOnePass[1]) &&
              (ret == ONEPASS_MULTI);

    report("sqlite3WhereOkOnePass_MULTI_copy_and_return", ok);
}

// Test 4 (optional): TRACE path - when WHERETRACE_ENABLED is defined, verify trace output.
// Requires building with -DWHERETRACE_ENABLED.
// The test checks that a line containing the expected cursor labels and values is emitted.
#ifdef WHERETRACE_ENABLED
static void test_trace_output() {
    // Prepare trace capture
    g_debug_strings.clear();

    WhereInfo w;
    memset(&w, 0, sizeof(WhereInfo));

    w.aiCurOnePass[0] = 5;
    w.aiCurOnePass[1] = 6;
    w.eOnePass = ONEPASS_SINGLE;

    // Enable trace
    extern int sqlite3WhereTrace;
    sqlite3WhereTrace = 1;

    int cur[2] = {0, 0};
    int ret = sqlite3WhereOkOnePass(&w, cur);

    bool ok = (ret == ONEPASS_SINGLE) && (!g_debug_strings.empty());
    if (ok) {
        // Expect at least one debug line containing the cursor info
        const std::string &line = g_debug_strings[0];
        ok = (line.find("ONEPASS_SINGLE cursors: 5 6") != std::string::npos);
    }

    report("sqlite3WhereOkOnePass_TRACE_output", ok);
}
#endif

int main() {
    // Run tests
    test_off_branch_copy_and_return();
    test_single_path_copy_and_return();
    test_multi_path_copy_and_return();
#ifdef WHERETRACE_ENABLED
    test_trace_output();
#endif

    // Summary
    if (g_tests_failed == 0) {
        printf("All tests passed. Ran %d tests.\n", g_tests_run);
        return 0;
    } else {
        printf("Tests completed with failures: %d out of %d.\n",
               g_tests_failed, g_tests_run);
        return 1;
    }
}
// Test suite for Sqlitetest_mutex_Init in test_mutex.c
// This test suite does not rely on GTest. It uses simple C-style asserts and a small
// custom test harness to verify the focal method behavior via mocked Tcl API.
// The mocks intercept command registrations and variable link registrations performed
// by Sqlitetest_mutex_Init.

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <sqliteInt.h>
#include <cstring>


// Ensure SQLITE_OK is defined in case the included headers do not provide it.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif

// Forward declaration of the focal function under test.
// The actual implementation resides in test_mutex.c.
extern "C" int Sqlitetest_mutex_Init(struct Tcl_Interp *interp);

// Minimal Tcl-like type declarations to match the focal code's expectations.
extern "C" {
typedef struct Tcl_Interp Tcl_Interp;
typedef struct Tcl_Obj Tcl_Obj;
typedef int Tcl_ObjCmdProc(void *clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
}

// Lightweight, mockable storage for test assertions
static std::vector<std::string> g_createdCommands;      // Captured command names registered
static std::vector<std::string> g_linkedVarNames;      // Captured linked variable names
static std::vector<void*>        g_linkedVarAddresses;  // Captured addresses for linked vars

// Mock implementations of Tcl API used by the focal method.
// They are provided with C linkage so that the linker resolves them when linking
// with test_mutex.o (which contains the real Sqlitetest_mutex_Init).
extern "C" {

// Mock: emulate Tcl_CreateObjCommand by recording the registered command name.
int Tcl_CreateObjCommand(Tcl_Interp *interp, const char *zName,
                         Tcl_ObjCmdProc *xProc, void *clientData,
                         void *deleteCallback){
    (void)interp; (void)xProc; (void)clientData; (void)deleteCallback;
    if (zName) {
        g_createdCommands.emplace_back(zName);
    } else {
        g_createdCommands.emplace_back("<null>");
    }
    return 0; // Simulate a successful registration
}

// Mock: emulate Tcl_LinkVar by recording the var name and the address provided.
int Tcl_LinkVar(Tcl_Interp *interp, const char *varName, char *addr, int flags){
    (void)interp; (void)flags;
    if (varName) {
        g_linkedVarNames.emplace_back(varName);
        g_linkedVarAddresses.push_back((void*)addr);
    } else {
        g_linkedVarNames.emplace_back("<null>");
        g_linkedVarAddresses.push_back(nullptr);
    }
    return 0; // Simulate a successful link
}
} // extern "C"

// Helper to reset mock storage between tests
static void reset_mocks() {
    g_createdCommands.clear();
    g_linkedVarNames.clear();
    g_linkedVarAddresses.clear();
}

// Simple test harness utilities
static int g_test_failures = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        printf("  [FAIL] %s\n", msg); \
        ++g_test_failures; \
    } else { \
        printf("  [OK]   %s\n", msg); \
    } \
} while(0)

// Expected command list in exact order as defined in Sqlitetest_mutex_Init
static const char* expected_commands[] = {
    "sqlite3_shutdown",
    "sqlite3_initialize",
    "sqlite3_config",
    "enter_static_mutex",
    "leave_static_mutex",
    "enter_db_mutex",
    "leave_db_mutex",
    "alloc_dealloc_mutex",
    "install_mutex_counters",
    "read_mutex_counters",
    "clear_mutex_counters"
};
static const int EXPECTED_COMMAND_COUNT = sizeof(expected_commands) / sizeof(expected_commands[0]);

// Test 1: Verify that Sqlitetest_mutex_Init registers the exact set of 11 commands
static void test_mutex_Init_registers_all_commands() {
    printf("Test 1: Register all expected commands in correct order\n");
    reset_mocks();

    int ret = Sqlitetest_mutex_Init(nullptr); // Pass a null interpreter; mocks ignore it
    CHECK(ret == SQLITE_OK, "Sqlitetest_mutex_Init returns SQLITE_OK");

    // Verify count
    CHECK((int)g_createdCommands.size() == EXPECTED_COMMAND_COUNT,
          "Number of registered commands matches expected count");

    // Verify exact order and names
    for (int i = 0; i < EXPECTED_COMMAND_COUNT; ++i) {
        const char* got = g_createdCommands[i].c_str();
        const char* exp = expected_commands[i];
        bool eq = (strcmp(got, exp) == 0);
        char msg[128];
        snprintf(msg, sizeof(msg), "Command %d should be '%s' (got '%s')", i, exp, got);
        CHECK(eq, msg);
    }
    printf("\n");
}

// Test 2: Verify that Sqlitetest_mutex_Init links two variables with the expected names
// and that addresses are captured (not NULL). We don't rely on internal g state.
static void test_mutex_Init_links_expected_variables() {
    printf("Test 2: Link expected mutex-related variables with correct names\n");
    reset_mocks();

    int ret = Sqlitetest_mutex_Init(nullptr);
    CHECK(ret == SQLITE_OK, "Sqlitetest_mutex_Init returns SQLITE_OK");

    // Expect exactly two linked variables in the specified order
    CHECK((int)g_linkedVarNames.size() == 2, "Exactly two variables are linked via Tcl_LinkVar");

    const char* v1 = g_linkedVarNames.size() > 0 ? g_linkedVarNames[0].c_str() : "<none>";
    const char* v2 = g_linkedVarNames.size() > 1 ? g_linkedVarNames[1].c_str() : "<none>";

    CHECK(strcmp(v1, "disable_mutex_init") == 0,
          "First linked variable name should be 'disable_mutex_init'");

    CHECK(strcmp(v2, "disable_mutex_try") == 0,
          "Second linked variable name should be 'disable_mutex_try'");

    // Also ensure the addresses provided are non-null (sanity check)
    bool addrs_are_non_null = (g_linkedVarAddresses.size() >= 2) &&
                            (g_linkedVarAddresses[0] != nullptr) &&
                            (g_linkedVarAddresses[1] != nullptr);
    CHECK(addrs_are_non_null, "Addresses passed to Tcl_LinkVar are non-null");

    printf("\n");
}

// Test 3: Regression-like check: ensure function can be called multiple times without crashing
// and that it consistently registers the expected commands each time.
static void test_mutex_Init_idempotence() {
    printf("Test 3: Idempotence - calling Init twice registers two sets of commands\n");
    reset_mocks();

    int ret1 = Sqlitetest_mutex_Init(nullptr);
    CHECK(ret1 == SQLITE_OK, "First Init call returns SQLITE_OK");
    int count_after_first = (int)g_createdCommands.size();

    // Call again
    int ret2 = Sqlitetest_mutex_Init(nullptr);
    CHECK(ret2 == SQLITE_OK, "Second Init call returns SQLITE_OK");

    int count_after_second = (int)g_createdCommands.size();
    // We expect double the number of commands after two registrations
    CHECK(count_after_second == count_after_first * 2,
          "Second Init call registers the same number of commands again (idempotence)");

    printf("\n");
}

// Entry point: run all tests
int main() {
    printf("Starting Sqlitetest_mutex_Init test suite (no GTest). \n");

    test_mutex_Init_registers_all_commands();
    test_mutex_Init_links_expected_variables();
    test_mutex_Init_idempotence();

    if (g_test_failures == 0) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Tests completed with %d failure(s).\n", g_test_failures);
        return 1;
    }
}
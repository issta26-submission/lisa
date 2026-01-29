// C++ test suite for Sqlitetest1_Init (focal method) using real Tcl API.
// This test avoids GTest/GMock as requested and runs tests from main().
// It relies on the real Tcl library being available at link time.
// The test focuses on validating that Sqlitetest1_Init properly registers
// Tcl commands and links important TCL variables to C state as in the production code.

#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <sys/time.h>
#include <tcl.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <sys/resource.h>
#include <iostream>
#include <sqlite3userauth.h>
#include <cassert>


extern "C" {
}

// Forward declaration of focal function (as provided in test1.c)
extern "C" int Sqlitetest1_Init(Tcl_Interp *interp);

// Minimal dummy implementation for sqlite3BtreeSharedCacheReport symbol
// that Sqlitetest1_Init references in its command table.
extern "C" int sqlite3BtreeSharedCacheReport(void*, Tcl_Interp*, int, Tcl_Obj*CONST*) {
    // No-op for testing purposes
    return 0;
}

// Global extern-like variables used by Sqlitetest1_Init's linking.
// We provide actual definitions here so that addresses match the externs in test1.c.
int sqlite3_search_count       = 0;
int sqlite3_found_count        = 0;
int sqlite3_interrupt_count    = 0;
int sqlite3_open_file_count    = 0;
int sqlite3_sort_count         = 0;
int sqlite3_current_time       = 0;
int sqlite3_max_blobsize       = 0;

// If APPLE/LOCKING_STYLE path is compiled in, declare/define hostid symbol too.
#if defined(SQLITE_OS_UNIX) && defined(__APPLE__) && defined(SQLITE_ENABLE_LOCKING_STYLE)
int sqlite3_hostid_num = 0;
#endif

// Provide a tiny stub to satisfy the sqlite3BtreeSharedCacheReport symbol usage
// in the focal file (already done above as a dummy function).

// Helper macro for simple test assertions
#define ASSERT_TRUE(cond) do { \
    if(!(cond)) { std::cerr << "Assertion failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; exit(1); } \
} while(0)

#define ASSERT_EQ(a,b) do { \
    if(!((a)==(b))){ std::cerr << "Assertion failed: " << #a << " == " << #b \
        << " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        exit(1); } \
} while(0)


// Simple helper to call a Tcl command and check for success
static void callTclAndAssertOk(Tcl_Interp* interp, const char* cmd) {
    int rc = Tcl_Eval(interp, cmd);
    if (rc != TCL_OK) {
        const char* err = Tcl_GetString(Tcl_GetResult(interp));
        std::cerr << "Tcl command failed: " << cmd << " error: " << err << "\n";
        exit(1);
    }
}


// Test 1: Ensure Sqlitetest1_Init initializes interpreter and registers commands
// and links variables as expected. Validate a couple of representative facts using Tcl.
static void test_Sqlitetest1_Init_basic_registration(Tcl_Interp* interp) {
    // Call focal function
    int rc = Sqlitetest1_Init(interp);
    ASSERT_EQ(rc, TCL_OK);

    // Validate a couple of well-known commands exist after Init.
    // We query Tcl for command presence using Tcl_GetCommandInfo.
    Tcl_CmdInfo cmdInfo;
    int has_db_enter = Tcl_GetCommandInfo(interp, "db_enter", &cmdInfo);
    // The presence of at least the first command ensures registration happened.
    ASSERT_TRUE(has_db_enter != 0);

    int has_sqlite_libver = Tcl_GetCommandInfo(interp, "sqlite3_libversion_number", &cmdInfo);
    ASSERT_TRUE(has_sqlite_libver != 0);

    // Validate that the sqlite3_libversion_number command returns the correct C value.
    callTclAndAssertOk(interp, "sqlite3_libversion_number");
    Tcl_Obj* res = Tcl_GetObjResult(interp);
    int verFromTcl;
    int status = Tcl_GetIntFromObj(interp, res, &verFromTcl);
    ASSERT_TRUE(status == TCL_OK);
    // Compare with the actual library version function
    extern int sqlite3_libversion_number(void);
    int verFromC = sqlite3_libversion_number();
    ASSERT_EQ(verFromTcl, verFromC);
}

// Test 2: Validate TCL variables linked by Sqlitetest1_Init reflect C state
// and that changes in C variables are visible in Tcl (readonly variables can be read, and writable can be updated if not read-only).
static void test_Sqlitetest1_Init_linked_vars(Tcl_Interp* interp) {
    // Ensure variables are linked to our global C state
    // Initially, sqlite_search_count is 0
    sqlite3_search_count = 0; // explicit
    callTclAndAssertOk(interp, "set sqlite_search_count"); // read value
    int val;
    int st = Tcl_GetIntFromObj(interp, Tcl_GetObjResult(interp), &val);
    ASSERT_TRUE(st == TCL_OK);
    ASSERT_EQ(val, sqlite3_search_count);

    // Change C variable and verify Tcl sees the new value
    sqlite3_search_count = 42;
    callTclAndAssertOk(interp, "set sqlite_search_count");
    st = Tcl_GetIntFromObj(interp, Tcl_GetObjResult(interp), &val);
    ASSERT_TRUE(st == TCL_OK);
    ASSERT_EQ(val, sqlite3_search_count);
}

// Entry point for running tests
int main() {
    // Initialize Tcl
    Tcl_Interp* interp = Tcl_CreateInterp();
    if(!interp){
        std::cerr << "Failed to create Tcl interpreter\n";
        return 1;
    }

    // Run tests
    test_Sqlitetest1_Init_basic_registration(interp);
    test_Sqlitetest1_Init_linked_vars(interp);

    // If reached here, tests passed
    std::cout << "All tests for Sqlitetest1_Init passed.\n";

    // Clean up
    Tcl_DeleteInterp(interp);
    return 0;
}
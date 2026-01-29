/* sqlite_tcl.h - Minimal test stub for Tcl integration (used by test harness)
   This header provides just enough declarations for the focal function
   Sqlitetestrtreedoc_Init to compile in a standalone test environment.
*/
#ifndef SQLITE_TCL_H
#define SQLITE_TCL_H

#ifdef __cplusplus
extern "C" {
#endif

// Lightweight placeholders to mimic Tcl's types used by the focal code
typedef void* ClientData;
struct Tcl_Interp { int dummy; }; // opaque interpreter placeholder

// Function pointer type for Tcl command procedures
typedef int (*Tcl_ObjCmdProc)(ClientData, struct Tcl_Interp*, int, const char * const*);

// Optional delete callback type for command deletion (unused in tests)
typedef int (*Tcl_CmdDeleteProc)(ClientData);

// Prototype of the function used by Sqlitetestrtreedoc_Init
int Tcl_CreateObjCommand(struct Tcl_Interp* interp, const char* cmdName, Tcl_ObjCmdProc* proc,
                         ClientData clientData, Tcl_CmdDeleteProc* deleteProc);

// TCL_OK is used by the focal function as a success code
#define TCL_OK 0

#ifdef __cplusplus
}
#endif

#endif // SQLITE_TCL_H



/* test_rtreedoc_rt_test.cpp - RTREE enabled branch test
   This test compiles with SQLITE_ENABLE_RTREE defined to exercise the
   true-branch of the #ifdef in Sqlitetestrtreedoc_Init.
   It provides a fake Tcl_CreateObjCommand to capture the commands registered
   by the focal method and verifies that both expected commands are registered.
   Note: Build this test with -DSQLITE_ENABLE_RTREE and link with test_rtreedoc.c.
*/
#include <vector>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <sqlite_tcl.h>
#include <iostream>
#include <algorithm>


#define SQLITE_ENABLE_RTREE  // Ensure the RTREE branch is compiled in this TU


// Forward declaration of the function under test (C linkage)
extern "C" int Sqlitetestrtreedoc_Init(struct Tcl_Interp* interp);

// The focal code expects a Tcl_CreateObjCommand function.
// Provide a C linkage implementation so the linker can resolve it.
extern "C" int Tcl_CreateObjCommand(struct Tcl_Interp* /*interp*/, const char* cmdName,
                                   Tcl_ObjCmdProc* /*proc*/, ClientData /*clientData*/,
                                   Tcl_CmdDeleteProc* /*deleteProc*/) {
    // Record the command name for verification
    if (cmdName) {
        g_TclCreatedCmds.push_back(std::string(cmdName));
    } else {
        g_TclCreatedCmds.push_back(std::string("<NULL_COMMAND>"));
    }
    return 0;
}

// Expose a global container to collect created commands for assertions
std::vector<std::string> g_TclCreatedCmds;

// Simple, non-terminating test harness helpers
static void expect_true(bool cond, const std::string& msg) {
    if (cond) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cout << "[FAIL] " << msg << "\n";
    }
}

int main() {
    // Prepare a dummy interpreter
    struct Tcl_Interp interp;
    g_TclCreatedCmds.clear();

    // Call the focal function
    int rc = Sqlitetestrtreedoc_Init(&interp);

    // True branch coverage: when SQLITE_ENABLE_RTREE is defined, two commands should be registered
#ifdef SQLITE_ENABLE_RTREE
    const int expected_count = 2;
    bool names_ok = false;
    if (g_TclCreatedCmds.size() == static_cast<size_t>(expected_count)) {
        // Verify the exact command names registered
        bool has_geom = false, has_query = false;
        for (const auto& name : g_TclCreatedCmds) {
            if (name == "register_box_geom") has_geom = true;
            if (name == "register_box_query") has_query = true;
        }
        names_ok = has_geom && has_query;
    }
    expect_true(rc == TCL_OK && g_TclCreatedCmds.size() == expected_count && names_ok,
                "Sqlitetestrtreedoc_Init with RTREE: should return TCL_OK and register both commands");
#else
    // Should be unreachable in this TU due to macro, but still document behavior
    expect_true(false, "RTREE path not compiled in this TU due to macro");
#endif

    // Always print the number of created commands for visibility
    std::cout << "Registered commands (" << g_TclCreatedCmds.size() << "): ";
    for (const auto& s : g_TclCreatedCmds) std::cout << s << " ";
    std::cout << "\n";

    return 0;
}



/* test_rtreedoc_no_rt_test.cpp - NO RTREE branch test
   This test compiles without SQLITE_ENABLE_RTREE to exercise the false-branch
   of the #ifdef in Sqlitetestrtreedoc_Init.
   It confirms that no Tcl_CreateObjCommand calls are made and TCL_OK is returned.
   Note: Build this test without -DSQLITE_ENABLE_RTREE and link with test_rtreedoc.c.
*/


// Forward declaration of the function under test (C linkage)
extern "C" int Sqlitetestrtreedoc_Init(struct Tcl_Interp* interp);

// Provide the same Tcl_CreateObjCommand stub as in the RT test.
// It will record any potential registrations (should be none in this path).
extern "C" int Tcl_CreateObjCommand(struct Tcl_Interp* /*interp*/, const char* cmdName,
                                   Tcl_ObjCmdProc* /*proc*/, ClientData /*clientData*/,
                                   Tcl_CmdDeleteProc* /*deleteProc*/) {
    if (cmdName) {
        g_TclCreatedCmds.push_back(std::string(cmdName));
    } else {
        g_TclCreatedCmds.push_back(std::string("<NULL_COMMAND>"));
    }
    return 0;
}

// Vector to log created commands; separate from the RT test to reflect separate run
std::vector<std::string> g_TclCreatedCmds;

static void expect_true(bool cond, const std::string& msg) {
    if (cond) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cout << "[FAIL] " << msg << "\n";
    }
}

int main() {
    // Prepare dummy interpreter
    struct Tcl_Interp interp;
    g_TclCreatedCmds.clear();

    // Call the focal function (RTREE not enabled in this TU)
    int rc = Sqlitetestrtreedoc_Init(&interp);

    // False branch coverage: there should be zero Tcl_CreateObjCommand registrations
    const int expected_count = 0;
    bool ok = (rc == TCL_OK) && (g_TclCreatedCmds.size() == static_cast<size_t>(expected_count));

    expect_true(ok, "Sqlitetestrtreedoc_Init without RTREE: should return TCL_OK and register zero commands");

    std::cout << "Registered commands (" << g_TclCreatedCmds.size() << "): ";
    for (const auto& s : g_TclCreatedCmds) std::cout << s << " ";
    std::cout << "\n";

    return 0;
}
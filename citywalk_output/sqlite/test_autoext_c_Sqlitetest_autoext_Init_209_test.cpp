// test_sqlite_autoext_full.cpp
// This test exercises Sqlitetest_autoext_Init when SQLITE_OMIT_LOAD_EXTENSION is not defined.
// It mocks Tcl_CreateObjCommand to capture registered commands and verifies the expected
// 7 commands are registered (6 extension control commands + 1 reset command) and that
// the function returns TCL_OK.

#include <iostream>
#include <sqlite3ext.h>
#include <vector>
#include <string>


// Minimal stand-in declarations to allow linking with the focal C file test_autoext.c.
// We purposefully define only what is necessary for the signature compatibility.

extern "C" {

// Forward declarations to mirror the real Tcl types used by the focal function.
// These declarations intentionally avoid pulling in real Tcl headers to keep the test
// self-contained within the constraints described.
typedef struct Tcl_Interp Tcl_Interp;
typedef void* ClientData;
typedef struct Tcl_Obj Tcl_Obj;

// Function type for the Tcl command procedures used by Tcl_CreateObjCommand.
// The exact signature used by the real Tcl header is:
// typedef int Tcl_ObjCmdProc(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
// We mirror it here as a compatible function type.
typedef int Tcl_ObjCmdProc(ClientData, Tcl_Interp*, int, Tcl_Obj *CONST []);

// DeleteProc type for Tcl_CreateObjCommand
typedef void Tcl_CmdDeleteProc(ClientData);

// Prototype for the function under test. We declare it extern to link with the actual C file.
extern int Sqlitetest_autoext_Init(Tcl_Interp *interp);

// Mock for Tcl_CreateObjCommand to capture registrations.
// The real function returns void; we mirror the same signature except we implement a simple body here.
void Tcl_CreateObjCommand(Tcl_Interp* interp,
                          const char* cmdName,
                          Tcl_ObjCmdProc* proc,
                          ClientData clientData,
                          Tcl_CmdDeleteProc* deleteProc);
}

// Global storage to capture registered command names.
static std::vector<std::string> g_registered_cmds;

// Mock implementation: record each registered command name into g_registered_cmds.
extern "C" void Tcl_CreateObjCommand(Tcl_Interp* /*interp*/,
                                   const char* cmdName,
                                   Tcl_ObjCmdProc* /*proc*/,
                                   ClientData /*clientData*/,
                                   Tcl_CmdDeleteProc* /*deleteProc*/)
{
    if (cmdName) {
        g_registered_cmds.emplace_back(cmdName);
    }
}

// Helper: reset the registry before each test
static void reset_registry() {
    g_registered_cmds.clear();
}

// Lightweight dummy Tcl_Interp instance for passing to the function under test
static Tcl_Interp dummy_interp;

// Test: verify full command registration when SQLITE_OMIT_LOAD_EXTENSION is NOT defined
// This ensures all 7 commands are registered in the expected order.
static bool test_full_command_registration() {
    reset_registry();
    // Call the focal method with a dummy interpreter
    int ret = Sqlitetest_autoext_Init(&dummy_interp);

    // We assume TCL_OK is 0 in the TCL interface; the real code returns TCL_OK on success.
    const int TCL_OK = 0;
    bool ok = true;
    if (ret != TCL_OK) {
        std::cerr << "[FULL] Expected TCL_OK (" << TCL_OK << ") but got " << ret << "\n";
        ok = false;
    }

    // Expected seven registrations (six extension-related + one reset)
    const int expectedCount = 7;
    if ((int)g_registered_cmds.size() != expectedCount) {
        std::cerr << "[FULL] Expected " << expectedCount
                  << " commands to be registered, but got " << g_registered_cmds.size() << "\n";
        ok = false;
    }

    const char* expected[] = {
        "sqlite3_auto_extension_sqr",
        "sqlite3_auto_extension_cube",
        "sqlite3_auto_extension_broken",
        "sqlite3_cancel_auto_extension_sqr",
        "sqlite3_cancel_auto_extension_cube",
        "sqlite3_cancel_auto_extension_broken",
        "sqlite3_reset_auto_extension"
    };

    for (int i = 0; i < expectedCount && i < (int)g_registered_cmds.size(); ++i) {
        if (g_registered_cmds[i] != expected[i]) {
            std::cerr << "[FULL] Mismatch at index " << i
                      << ": expected '" << expected[i]
                      << "', got '" << g_registered_cmds[i] << "'\n";
            ok = false;
        }
    }

    if (!ok) {
        std::cerr << "[FULL] Registered commands (in order):\n";
        for (const auto& s : g_registered_cmds) std::cerr << "  " << s << "\n";
    }

    return ok;
}

int main() {
    bool result = test_full_command_registration();

    if (result) {
        std::cout << "TEST FULL: PASS\n";
        return 0;
    } else {
        std::cerr << "TEST FULL: FAIL\n";
        return 1;
    }
}


// Explanation:
// - The test defines a mock Tcl_CreateObjCommand that records the command names passed by the focal method.
// - We test the full path (no SQLITE_OMIT_LOAD_EXTENSION) by asserting that 7 commands are registered
//   in the exact sequence described by the source: six auto/cancel commands plus sqlite3_reset_auto_extension.
// - We also verify that the focal function returns TCL_OK (interpreted as 0 here).
// - If you want to run this test in isolation, ensure the build links in the focal C file test_autoext.c
//   and compile this file with SQLITE_OMIT_LOAD_EXTENSION not defined.


// test_sqlite_autoext_omit.cpp
// This test exercises Sqlitetest_autoext_Init when SQLITE_OMIT_LOAD_EXTENSION is defined.
// It mocks Tcl_CreateObjCommand to capture registrations and verifies that only the
// reset command is registered, and that the function still returns TCL_OK.


extern "C" {

typedef struct Tcl_Interp Tcl_Interp;
typedef void* ClientData;
typedef struct Tcl_Obj Tcl_Obj;
typedef int Tcl_ObjCmdProc(ClientData, Tcl_Interp*, int, Tcl_Obj *CONST []);
typedef void Tcl_CmdDeleteProc(ClientData);

extern int Sqlitetest_autoext_Init(Tcl_Interp*);

void Tcl_CreateObjCommand(Tcl_Interp* interp,
                          const char* cmdName,
                          Tcl_ObjCmdProc* proc,
                          ClientData clientData,
                          Tcl_CmdDeleteProc* deleteProc);
}

// Registry used by the mock to collect registered command names
static std::vector<std::string> g_registered_cmds;

// Mock implementation: capture the command names registered by the focal function
extern "C" void Tcl_CreateObjCommand(Tcl_Interp* /*interp*/,
                                   const char* cmdName,
                                   Tcl_ObjCmdProc* /*proc*/,
                                   ClientData /*clientData*/,
                                   Tcl_CmdDeleteProc* /*deleteProc*/)
{
    if (cmdName) {
        g_registered_cmds.emplace_back(cmdName);
    }
}

// Helper: reset registry
static void reset_registry() {
    g_registered_cmds.clear();
}

// Dummy interpreter instance
static Tcl_Interp dummy_interp;

// Test: verify only the reset command is registered when SQLITE_OMIT_LOAD_EXTENSION is defined
static bool test_omit_command_registration() {
    reset_registry();
    // Call the focal method with a dummy interpreter
    int ret = Sqlitetest_autoext_Init(&dummy_interp);

    // TCL_OK is assumed to be 0
    const int TCL_OK = 0;
    bool ok = true;
    if (ret != TCL_OK) {
        std::cerr << "[OMIT] Expected TCL_OK (" << TCL_OK << ") but got " << ret << "\n";
        ok = false;
    }

    const int expectedCount = 1;
    if ((int)g_registered_cmds.size() != expectedCount) {
        std::cerr << "[OMIT] Expected " << expectedCount
                  << " command to be registered, but got " << g_registered_cmds.size() << "\n";
        ok = false;
    }

    if (g_registered_cmds.size() > 0 && g_registered_cmds[0] != "sqlite3_reset_auto_extension") {
        std::cerr << "[OMIT] Expected only 'sqlite3_reset_auto_extension' to be registered, but got '"
                  << g_registered_cmds[0] << "'\n";
        ok = false;
    }

    if (!ok) {
        std::cerr << "[OMIT] Registered commands (in order):\n";
        for (const auto& s : g_registered_cmds) std::cerr << "  " << s << "\n";
    }

    return ok;
}

int main() {
    bool result = test_omit_command_registration();

    if (result) {
        std::cout << "TEST OMIT: PASS\n";
        return 0;
    } else {
        std::cerr << "TEST OMIT: FAIL\n";
        return 1;
    }
}


// Explanation:
// - The test defines a mock Tcl_CreateObjCommand that records the command names passed by the focal method.
// - We test the omission path (SQLITE_OMIT_LOAD_EXTENSION defined) by asserting that only the
//   sqlite3_reset_auto_extension command is registered and that the function returns TCL_OK (0).
// - As with the full test, ensure this file is compiled in a translation unit where the macro
//   SQLITE_OMIT_LOAD_EXTENSION is defined (e.g., by using -DSQLITE_OMIT_LOAD_EXTENSION in the compiler flags).
// Sqlitetest8_Init Unit Test Harness (C++11, no GTest)
// This harness tests the focal method Sqlitetest8_Init from test8.c by
// providing a tiny fake Tcl_CreateObjCommand implementation to record
// how many commands are registered and with what names.
// It relies on the compile-time macro SQLITE_OMIT_VIRTUALTABLE to
// select the code path inside Sqlitetest8_Init.
// Notes:
// - We do not call into real Tcl; we only verify that the function attempts
//   to register commands (or not) as expected.
// - This harness expects that test8.c is linked into the final binary.
// - Compile-time behavior:
//   - If SQLITE_OMIT_VIRTUALTABLE is NOT defined, two commands should be registered.
//   - If SQLITE_OMIT_VIRTUALTABLE is defined, no commands should be registered.

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <utility>


// Forward declaration of the focal function to test.
// The actual implementation resides in test8.c.
extern "C" int Sqlitetest8_Init(void *interp);

// Typedefs and stubs to mimic Tcl types for the test harness.
// We only need a minimal surface to capture command registrations.
typedef void Tcl_Interp;
typedef void* ClientData;

// Define the Tcl_ObjCmdProc function type as used by test8.c.
// It matches: int function(ClientData, Tcl_Interp*, int, Tcl_Obj *CONST objv[])
typedef int (Tcl_ObjCmdProc)(ClientData, Tcl_Interp*, int, void*[]);

// Prototype for the fake Tcl_CreateObjCommand to be picked up by test8.c.
// It records the command name and its associated clientData for verification.
extern "C" int Tcl_CreateObjCommand(Tcl_Interp *interp,
                                  const char *zName,
                                  Tcl_ObjCmdProc *proc,
                                  ClientData clientData,
                                  void *deleteProc);

// Global registry to capture commands registered by Sqlitetest8_Init.
static std::vector<std::pair<std::string, void*>> g_registeredCommands;

// Fake implementation of Tcl_CreateObjCommand that records registrations.
extern "C" int Tcl_CreateObjCommand(Tcl_Interp *interp,
                                 const char *zName,
                                 Tcl_ObjCmdProc *proc,
                                 ClientData clientData,
                                 void *deleteProc)
{
    // Record the command name and associated clientData for assertions.
    g_registeredCommands.emplace_back(std::string(zName ? zName : ""), clientData);
    // Return 0 to mimic TCL_OK-like behavior in this simplified harness.
    return 0;
}

// Simple helper to print test results and accumulate failure count.
static int g_failCount = 0;
#define FAIL(msg) do { std::cerr << "FAIL: " << msg << std::endl; ++g_failCount; } while(0)

int main() {
    // Test plan:
    // - Run the focal function Sqlitetest8_Init with a null interpreter.
    // - Verify TCL_OK-like return value (0) is produced.
    // - Verify command registrations depending on the compile-time macro:
    //     - If SQLITE_OMIT_VIRTUALTABLE is defined: 0 commands expected.
    //     - If not defined: 2 commands expected in the given order:
    //         1) register_echo_module
    //         2) sqlite3_declare_vtab
    // - Verify clientData for each registered command is 0 (NULL in this harness).

    // Clear previous registrations before each test run.
    g_registeredCommands.clear();

    // Placeholder interpreter (not used by the test harness logic).
    Tcl_Interp *interp = nullptr;

    // Execute the focal function.
    int rc = Sqlitetest8_Init(interp);

    // Expected value depends on compile-time macro SQLITE_OMIT_VIRTUALTABLE.
#ifdef SQLITE_OMIT_VIRTUALTABLE
    const size_t expectedCount = 0;
#else
    const size_t expectedCount = 2;
#endif

    // Assertion 1: Function should return TCL_OK (0 in this harness).
    if (rc != 0) {
        FAIL("Sqlitetest8_Init should return TCL_OK (0). Actual: " << rc);
    }

    // Assertion 2: Number of registered commands matches expectation.
    if (g_registeredCommands.size() != expectedCount) {
        FAIL("Expected " << expectedCount << " registered command(s), but got "
             << g_registeredCommands.size() << ".");
    }

    // Assertions for the non-omitted-virtualtable path (two commands).
#ifndef SQLITE_OMIT_VIRTUALTABLE
    if (g_registeredCommands.size() == 2) {
        // Command order should be as defined in the source: 0 -> "register_echo_module",
        // 1 -> "sqlite3_declare_vtab".
        if (g_registeredCommands[0].first != "register_echo_module") {
            FAIL("First registered command should be 'register_echo_module', got '"
                 << g_registeredCommands[0].first << "'.");
        }
        if (g_registeredCommands[1].first != "sqlite3_declare_vtab") {
            FAIL("Second registered command should be 'sqlite3_declare_vtab', got '"
                 << g_registeredCommands[1].first << "'.");
        }
        // clientData should be 0 for both entries, as per the source.
        if (g_registeredCommands[0].second != nullptr) {
            FAIL("First command clientData should be nullptr (0).");
        }
        if (g_registeredCommands[1].second != nullptr) {
            FAIL("Second command clientData should be nullptr (0).");
        }
    }
#endif

    // All assertions completed. Print final result.
    if (g_failCount == 0) {
        std::cout << "ALL TESTS PASSED: Sqlitetest8_Init behavior matches expectations for this build." << std::endl;
        return 0;
    } else {
        std::cerr << g_failCount << " TEST(S) FAILED." << std::endl;
        return 1;
    }
}

// Explanation:
// - This harness leverages a lightweight fake Tcl_CreateObjCommand to capture
//   registrations performed by Sqlitetest8_Init.
// - By compiling with and without SQLITE_OMIT_VIRTUALTABLE, we exercise both
//   code paths (two registrations vs zero registrations).
// - We verify the exact command names in the non-omitted path and ensure clientData
//   is passed as 0 (nullptr) as expected by the source.
// - The tests are designed to be non-terminating (they accumulate failures) and
//   can be extended to include additional assertions if needed.
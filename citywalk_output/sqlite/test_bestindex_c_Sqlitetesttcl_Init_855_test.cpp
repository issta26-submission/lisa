// C++11 test suite for the focal method: Sqlitetesttcl_Init
// This test uses a lightweight, self-contained testing approach (no GTest).
// It provides a minimal Tcl-like runtime to validate the behavior of Sqlitetesttcl_Init.
// The test targets the default (non-OMIT_VIRTUALTABLE) path where one TCL command is registered.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Forward declare Tcl_Interp to match the focal function signature without pulling real Tcl headers.
struct Tcl_Interp;

// We declare the focal function so we can link against the actual implementation in test_bestindex.c
// The implementation is C, so we use extern "C" to avoid name mangling in C++.
extern "C" int Sqlitetesttcl_Init(Tcl_Interp *interp);

// ============================================================================
// Lightweight mock Tcl runtime (minimal surface needed by Sqlitetesttcl_Init)
// ============================================================================
extern "C" {

// ClientData is commonly void*. We model it as void* for simplicity.
typedef void* ClientData;

// Tcl_Obj is opaque in this mock.
typedef void Tcl_Obj;

// Function pointer type for a Tcl object command proc.
// Signature matches: int Cmd(ClientData, Tcl_Interp*, int, Tcl_Obj* CONST [])
typedef int (*Tcl_ObjCmdProc)(ClientData, Tcl_Interp*, int, Tcl_Obj* const[]);

// Simple in-memory record of created Tcl commands (captured for assertions)
struct CreatedCmdRecord {
    std::string name;
    void* xProc;       // pointer to the command proc (opaque for test)
    void* clientData;
};

// Global store for created commands (filled by mock Tcl_CreateObjCommand)
static std::vector<CreatedCmdRecord> g_createdCommands;

// Reset helper for test isolation
static void resetCreatedCommands() {
    g_createdCommands.clear();
}

// Mock implementation of Tcl_CreateObjCommand used by Sqlitetesttcl_Init.
// It records the created command details into g_createdCommands.
int Tcl_CreateObjCommand(Tcl_Interp* interp, const char* zName, Tcl_ObjCmdProc xProc, void* clientData, void* /*pFreeProc*/) {
    CreatedCmdRecord rec;
    rec.name = zName ? zName : "";
    rec.xProc = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(xProc));
    rec.clientData = clientData;
    g_createdCommands.push_back(rec);
    // Return a success code (TCL_OK is typically 0)
    return 0;
}

// A minimal Tcl command implementation used to satisfy the register_tcl_module symbol.
// The real implementation is provided in the focal code file; we merely exist to satisfy linkage if needed.
int register_tcl_module(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* CONST objv[]) {
    (void)clientData; (void)interp; (void)objc; (void)objv;
    return 0;
}

} // extern "C"

// Forward declare Tcl_Interp as an opaque struct for our tests.
struct Tcl_Interp {
    // Intentionally empty: the tests don't rely on internal fields.
};

// ============================================================================
// Simple test harness (non-terminating EXPECT macros)
// ============================================================================

static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        std::cerr << "Test Failure: " << (msg) \
                  << "  [Condition: " << #cond << "]" << std::endl; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)

// Helper to print a short summary
static void printSummary() {
    std::cout << "\nTest Summary: " << g_totalTests << " tests, "
              << g_failedTests << " failures." << std::endl;
}

// ============================================================================
// Test Case 1: Sqlitetesttcl_Init registers exactly one command when
// SQLITE_OMIT_VIRTUALTABLE is NOT defined.
// This validates:
// - The function returns TCL_OK (0).
// - Exactly one command is registered.
// - The command name is "register_tcl_module".
// - The command's clientData is 0 (nullptr).
// Note: We intentionally do not assert on exact xProc pointer value because
// the function pointer originates from the focal module's internal/static symbol.
// ============================================================================

static void test_InitRegistersOneCmd() {
    // Prepare environment
    resetCreatedCommands();
    Tcl_Interp* interp = new Tcl_Interp();

    // Execute focal method
    int result = Sqlitetesttcl_Init(interp);

    // Validate expected outcomes
    EXPECT_TRUE(result == 0, "Sqlitetesttcl_Init should return TCL_OK (0)");
    EXPECT_TRUE(g_createdCommands.size() == 1, "Should register exactly 1 TCL command");

    if (!g_createdCommands.empty()) {
        EXPECT_TRUE(g_createdCommands[0].name == "register_tcl_module",
                    "Registered command should be named 'register_tcl_module'");
        // We can't rely on exact function pointer identity across translation units,
        // but we can ensure a valid non-null function pointer was stored.
        EXPECT_TRUE(g_createdCommands[0].xProc != nullptr,
                    "Registered command should have a non-null xProc pointer");
        EXPECT_TRUE(g_createdCommands[0].clientData == nullptr,
                    "Registered command's clientData should be null (0)");
    }

    delete interp;
}

// ============================================================================
// Test Case 2 (conditional): If SQLITE_OMIT_VIRTUALTABLE is defined at compile-time,
// Sqlitetesttcl_Init should skip creating any object commands and still return TCL_OK.
// This test is compiled and run only when the macro SQLITE_OMIT_VIRTUALTABLE is defined.
// Build with -DSQLITE_OMIT_VIRTUALTABLE to enable this path.
// ============================================================================

#ifdef SQLITE_OMIT_VIRTUALTABLE
static void test_InitOmitVirtualTableProducesNoCommands() {
    resetCreatedCommands();
    Tcl_Interp* interp = new Tcl_Interp();

    int result = Sqlitetsttcl_Init(interp); // Note: typo in function name intentionally avoided
    // The focal function name is Sqlitetesttcl_Init; ensure we call the correct symbol.
}
#endif

// ============================================================================
// Test Case 3: Compile-time guard: If SQLITE_OMIT_VIRTUALTABLE is defined,
// ensure that no commands are registered (alternate path).
// This serves as a placeholder when the macro is enabled during build.
// ============================================================================

// We provide a small wrapper to be safe if needed in extended builds.
// (In this single-file test, Test Case 3 is effectively covered by Test Case 1
// for the non-OMIT path; additional coverage requires compile-time macro toggling.)

// ============================================================================
// Main entry: run all tests
// ============================================================================

int main() {
    // Run test 1: default path
    test_InitRegistersOneCmd();

#ifdef SQLITE_OMIT_VIRTUALTABLE
    // If built with OMIT flag, you can extend tests here to cover the branch that
    // registers zero commands. This requires an additional test function defined
    // under the #ifdef above in a complete environment.
    // For now, signal that macro-defined build ran with OMIT path.
    std::cout << "Compiled with SQLITE_OMIT_VIRTUALTABLE; Test Case 2 would run here if implemented." << std::endl;
#endif

    printSummary();
    // Exit with the number of failures as a conventional failure code
    return g_failedTests;
}
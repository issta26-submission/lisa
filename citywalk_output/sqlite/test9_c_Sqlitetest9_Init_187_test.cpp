// C++11 test harness for the focal method Sqlitetest9_Init
// This harness mocks the Tcl command registration mechanism to verify
// that Sqlitetest9_Init registers the expected commands.
//
// Notes:
// - The real Sqlitetest9_Init is expected to be linked in from the provided C
//   source (test9.c). We supply a lightweight mock for Tcl_CreateObjCommand.
// - We avoid terminating assertions; instead, we collect and print a summary.

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Forward declare Tcl types to match the focal function's signature
struct Tcl_Interp; // opaque in this harness
struct Tcl_Obj;    // opaque in this harness

typedef void* ClientData;

// The type for Tcl_ObjCmdProc: int func(ClientData, Tcl_Interp*, int, Tcl_Obj* const*)
typedef int (*Tcl_ObjCmdProc)(ClientData, Tcl_Interp*, int, Tcl_Obj* const*);

// TCL_OK macro used by the focal method (sqlite's TCL_OK is typically 0)
#ifndef TCL_OK
#define TCL_OK 0
#endif

// -----------------------------------------------------------------------------------
// Mock Tcl integration
// Collect registered command names when Sqlitetest9_Init calls Tcl_CreateObjCommand.
// We provide a C-style API so the C source under test can call into our mock.
// -----------------------------------------------------------------------------------
static std::vector<std::string> g_registered_cmds;

// The mock implementation of Tcl_CreateObjCommand.
// This will be linked in place of the real Tcl_CreateObjCommand during tests.
extern "C" int Tcl_CreateObjCommand(Tcl_Interp* interp,
                                  const char* zName,
                                  Tcl_ObjCmdProc xProc,
                                  void* clientData,
                                  void* deleteProc)
{
    // Only register when interp is valid (simulates Tcl behavior)
    if (interp != nullptr && zName != nullptr) {
        g_registered_cmds.push_back(std::string(zName));
    }
    // Return a dummy command handle (not used in tests)
    return 0;
}

// Public C API for test harness access (to avoid leaking internal state)
extern "C" void reset_registered_cmds() { g_registered_cmds.clear(); }

extern "C" size_t created_cmds_count() { return g_registered_cmds.size(); }

extern "C" const char* created_cmd_name(size_t idx) {
    if (idx >= g_registered_cmds.size()) return nullptr;
    return g_registered_cmds[idx].c_str();
}

// -----------------------------------------------------------------------------------
// Test harness wiring for Sqlitetest9_Init
// We declare the external focal function so the harness can invoke it.
// -----------------------------------------------------------------------------------
extern "C" int Sqlitetest9_Init(Tcl_Interp *interp); // Provided by the C source under test

// -----------------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// We accumulate failures and print a final summary.
// -----------------------------------------------------------------------------------
static int g_total_checks = 0;
static int g_failed_checks = 0;

#define EXPECT(cond, desc) do { \
    ++g_total_checks; \
    if(!(cond)) { \
        ++g_failed_checks; \
        std::cerr << "[FAIL] " << __func__ << ": " << (desc) << std::endl; \
    } \
} while(0)

// -----------------------------------------------------------------------------------
// Tests
// 1) Basic registration: ensure exactly 3 commands registered in correct order
// 2) Null interpreter: ensure no commands registered when interp is null
// 3) Idempotence / multiple invocations: ensure repeated initializations accumulate registrations
// -----------------------------------------------------------------------------------
static void test_basic_registration() {
    reset_registered_cmds();
    struct Tcl_Interp dummy;
    int rc = Sqlitetest9_Init(&dummy);

    EXPECT(rc == TCL_OK, "Sqlitetest9_Init should return TCL_OK (0)");
    EXPECT(created_cmds_count() == 3, "Exactly 3 commands should be registered on first init");

    const char* expected[] = { "c_misuse_test", "c_realloc_test", "c_collation_test" };
    size_t n = created_cmds_count();
    for (size_t i = 0; i < 3 && i < n; ++i) {
        const char* name = created_cmd_name(i);
        EXPECT(name != nullptr, "Registered command name must be non-null");
        EXPECT(std::strcmp(name, expected[i]) == 0,
               std::string("Command at index ").append(std::to_string(i)).append(" should be ").append(expected[i]).c_str());
    }
    // If more than 3 were registered, this would be a problem for this test; capture as a fail if present
    EXPECT(n == 3, "No extra commands should be registered in the first init");
}

// 2) Null interpreter should not register any commands
static void test_null_interpreter() {
    reset_registered_cmds();
    int rc = Sqlitetest9_Init(nullptr);

    EXPECT(rc == TCL_OK, "Sqlitetest9_Init with nullptr interpreter should return TCL_OK");
    EXPECT(created_cmds_count() == 0, "No commands should be registered when interpreter is null");
}

// 3) Re-initialization should accumulate registrations (idempotence not assumed)
static void test_multiple_invocations() {
    reset_registered_cmds();
    struct Tcl_Interp dummy;
    int rc1 = Sqlitetest9_Init(&dummy);

    EXPECT(rc1 == TCL_OK, "First init should return TCL_OK");
    EXPECT(created_cmds_count() == 3, "First init registers 3 commands");

    int rc2 = Sqlitetest9_Init(&dummy);
    EXPECT(rc2 == TCL_OK, "Second init should return TCL_OK");

    // Expect accumulation of 6 commands after two calls
    EXPECT(created_cmds_count() == 6, "Second init should accumulate to 6 commands");
    // Sanity check that the first three names remain the same
    const char* first_name = created_cmd_name(0);
    EXPECT(first_name != nullptr, "First registered name should exist");
    EXPECT(std::strcmp(first_name, "c_misuse_test") == 0, "First registered name should be c_misuse_test");
}

// -----------------------------------------------------------------------------------
// Main runner
// -----------------------------------------------------------------------------------
int main() {
    // Run tests
    test_basic_registration();
    test_null_interpreter();
    test_multiple_invocations();

    // Summary
    std::cout << "Test Summary: " << g_total_checks << " checks, "
              << g_failed_checks << " failures." << std::endl;

    // Return non-zero if any test failed
    return g_failed_checks > 0 ? 1 : 0;
}
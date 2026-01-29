// Test suite for Sqlitetest_func_Init (no Google Test; lightweight custom harness)
// Focus: verify the focal function initializes Tcl commands and returns TCL_OK.
// This test is designed to be compiled with C++11 and linked against the original project
// (which provides Tcl and SQLite integrations in the tested binary).

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sstream>
#include <tcl.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Include Tcl and SQLite headers.
// Adjust include paths as necessary for your build environment.

extern "C" {
  // Focal function under test (C linkage)
  int Sqlitetest_func_Init(Tcl_Interp *interp);
}

// Provide a minimal C++-level test harness (non-terminating assertions)
static int g_test_failures = 0;
#define TEST_EXPECT_EQ(a, b) do { \
    auto _va = (a); \
    auto _vb = (b); \
    if (!(_va == _vb)) { \
        std::cerr << "EXPECT_EQ FAILED: " #a " == " #b " (" << _va << " != " << _vb << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define TEST_EXPECT_TRUE(x) do { \
    if (!(x)) { \
        std::cerr << "EXPECT_TRUE FAILED: " #x " is false" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define TEST_EXPECT_NOT_NULL(p) do { \
    if ((p) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL FAILED: " #p " is null" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)


// Namespace to group tests
namespace TestSqlitetestFuncInit {

////////////////////////////////////////////////////////////////////////////////
// Helper: Create a fresh Tcl interpreter for each test
// Returns a pointer to a new Tcl_Interp. Caller must delete via Tcl_DeleteInterp.
static Tcl_Interp* CreateInterp() {
    // Create a new interpreter
    Tcl_Interp* interp = Tcl_CreateInterp();
    // Minimal error check: interpreter should not be NULL
    TEST_EXPECT_NOT_NULL(interp);
    return interp;
}

////////////////////////////////////////////////////////////////////////////////
// Test 1: Return value should be TCL_OK
// Rationale: Sqlitetest_func_Init is expected to initialize and return TCL_OK on success.
static void Test_Init_Returns_OK() {
    Tcl_Interp* interp = CreateInterp();
    TEST_EXPECT_NOT_NULL(interp);

    int ret = Sqlitetest_func_Init(interp);
    // TCL_OK is defined by Tcl; compare integer value to expected constant.
    TEST_EXPECT_EQ(ret, TCL_OK);

    // Clean up interpreter
    if (interp) {
        Tcl_DeleteInterp(interp);
        interp = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Test 2: Registers the 3 expected Tcl commands
// Rationale: The focal method creates three Tcl commands: 
//   - autoinstall_test_functions
//   - abuse_create_function
//   - install_fts3_rank_function
// After Init, these commands should exist in the interpreter's command table.
static void Test_Commands_Registered() {
    Tcl_Interp* interp = CreateInterp();
    TEST_EXPECT_NOT_NULL(interp);

    int ret = Sqlitetest_func_Init(interp);
    TEST_EXPECT_EQ(ret, TCL_OK);

    // Check for existence of each command via Tcl_GetCommandInfo
    // If a command exists, Tcl_GetCommandInfo returns non-zero.
    Tcl_CmdInfo cmdInfo;
    int has_autoinstall = Tcl_GetCommandInfo(interp, "autoinstall_test_functions", &cmdInfo);
    TEST_EXPECT_TRUE(has_autoinstall);

    int has_abuse = Tcl_GetCommandInfo(interp, "abuse_create_function", &cmdInfo);
    TEST_EXPECT_TRUE(has_abuse);

    int has_rank = Tcl_GetCommandInfo(interp, "install_fts3_rank_function", &cmdInfo);
    TEST_EXPECT_TRUE(has_rank);

    // Optional: attempt to invoke one of the commands with zero arguments to confirm call path.
    // Not strictly required for coverage, but helps validate callability without crashing.
    // We guard with TCL_OK expectation; if the command is non-callable with no args, this could fail.
    if (has_autoinstall) {
        const char* evalAutoinstall = "autoinstall_test_functions";
        int evalRes = Tcl_Eval(interp, evalAutoinstall);
        // Allow non-fatal failure: we treat TCL_OK as a successful invocation.
        // If the command exists but requires arguments, the eval might fail; in that case, the command presence
        // still counts towards initialization coverage.
        // We still record a non-fatal note.
        if (evalRes != TCL_OK) {
            std::cerr << "NOTICE: autoinstall_test_functions invocation returned non-TCL_OK ("
                      << evalRes << "). This may indicate argc/argv expectations for the test.\n";
        }
    }

    // Cleanup
    if (interp) {
        Tcl_DeleteInterp(interp);
        interp = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Run all tests
static void RunAll() {
    std::cout << "Running TestSqlitetestFuncInit suite...\n";

    Test_Init_Returns_OK();
    Test_Commands_Registered();

    if (g_test_failures == 0) {
        std::cout << "All tests passed for Sqlitetest_func_Init.\n";
    } else {
        std::cout << "Sqlitetest_func_Init tests completed with "
                  << g_test_failures << " failure(s).\n";
    }
}

} // namespace TestSqlitetestFuncInit

////////////////////////////////////////////////////////////////////////////////
// Main: Entry point for test execution
int main() {
    // Run all tests in the dedicated namespace
    TestSqlitetestFuncInit::RunAll();
    // Return non-zero if any test failed
    return (TestSqlitetestFuncInit::g_test_failures != 0) ? 1 : 0;
}
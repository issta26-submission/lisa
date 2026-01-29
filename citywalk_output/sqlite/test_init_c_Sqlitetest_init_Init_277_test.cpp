/*
  Minimal C++11 test harness for the focal method:
  int Sqlitetest_init_Init(Tcl_Interp *interp)

  This test suite uses a lightweight, in-process mock of Tcl_CreateObjCommand to
  observe the behavior of Sqlitetest_init_Init without requiring the real Tcl library.
  It verifies that:
  - The function returns TCL_OK (0) on success.
  - It registers exactly four commands with the expected names and non-null function pointers.
  - Re-invocation (with a cleared registry) registers again four commands in the same order.

  Notes:
  - This test is written to be compiled together with the C source containing
    Sqlitetest_init_Init (as a C function) in a C++11 project without GTest.
  - We provide C-linkage stubs for Tcl integration points to allow linking against the
    C source file without needing the real Tcl library.
  - We follow the given domain guidance:
    - Use only standard C++11 facilities (no GTest).
    - Non-terminating style assertions are simulated via a simple in-test error collection.
    - Do not rely on private members; test the public-facing behavior of the focal function.
    - Comments explain the intent of each test.
*/

#include <string.h>
#include <vector>
#include <memory>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <utility>
#include <cassert>


// Domain-knowledge helpers: lightweight assertion collection (non-terminating style)
static std::vector<std::string> g_test_errors;

// Simple macro-like helpers for non-terminating checks
#define TEST_CHECK(cond, msg) do { if(!(cond)) { g_test_errors.push_back(std::string("Check failed: ") + (msg)); } } while(false)
#define TEST_REQUIRE(cond, msg) do { if(!(cond)) { g_test_errors.push_back(std::string("Requirement failed: ") + (msg)); } } while(false)

// -----------------------------------------------------------------------------
// Minimal C-wrapper declarations to allow linking with the focal C file
// (The real project would include Tcl headers; we provide minimal stubs here.)
#ifndef CONST
#define CONST const
#endif

typedef void* ClientData;

// Forward declarations for Tcl-like types
typedef struct Tcl_Interp Tcl_Interp;
typedef struct Tcl_Obj Tcl_Obj;

// Function type for Tcl command procedures
typedef int Tcl_ObjCmdProc(ClientData, Tcl_Interp*, int, Tcl_Obj *CONST[]);

// The focal function is declared with C linkage in the C source.
// We declare it here so that the C++ test can call it.
extern "C" int Sqlitetest_init_Init(Tcl_Interp *interp);

// Stub for Tcl_CreateObjCommand used by the focal function.
// We mirror the signature as used in the C source and record the registrations.
// We expose this with C linkage to match the C implementation's linkage expectations.
extern "C" void Tcl_CreateObjCommand(
    Tcl_Interp *interp,
    const char *zName,
    Tcl_ObjCmdProc *xProc,
    void *clientData,
    void (*deleteProc)(ClientData)
);

// Registry to capture created commands (name and associated xProc)
static std::vector<std::pair<std::string, void*>> g_registeredCommands;

// Implementation of the Tcl_CreateObjCommand stub.
// When the focal function calls it, we capture the command name and its function pointer.
extern "C" void Tcl_CreateObjCommand(
    Tcl_Interp * /*interp*/,
    const char *zName,
    Tcl_ObjCmdProc *xProc,
    void * /*clientData*/,
    void (*/*deleteProc*/)(ClientData)
) {
    if (zName) {
        g_registeredCommands.emplace_back(std::string(zName), reinterpret_cast<void*>(xProc));
    } else {
        g_registeredCommands.emplace_back(std::string(""), reinterpret_cast<void*>(xProc));
    }
}

// -----------------------------------------------------------------------------
// Lightweight dummy implementations to satisfy linkage in tests (no real Tcl behavior)
struct Tcl_Interp {
    // Empty placeholder; the focal code does not rely on actual Tcl state in tests.
};

// A minimal Tcl_Obj placeholder to satisfy type usage in signatures.
// The test does not exercise its content, only the pointer type presence.
struct Tcl_Obj {};

// -----------------------------------------------------------------------------
// Test helpers and test cases

// Test 1: Verify Sqlitetest_init_Init registers exactly four commands with expected names
// and non-null function pointers in a single invocation.
bool test_InitRegistersFourCommandsOnce() {
    g_registeredCommands.clear();

    // Create a dummy interpreter object
    Tcl_Interp interp_dummy;

    // Invoke the focal function under test
    int rc = Sqlitetest_init_Init(&interp_dummy);

    // Expect TCL_OK (0)
    TEST_CHECK(rc == 0, "Sqlitetest_init_Init should return TCL_OK (0).");

    // Expect exactly 4 registrations
    TEST_CHECK(g_registeredCommands.size() == 4, "Expected exactly 4 commands to be registered.");

    // Expected command names in defined order
    const char* expectedNames[4] = {
        "init_wrapper_install",
        "init_wrapper_query",
        "init_wrapper_uninstall",
        "init_wrapper_clear"
    };

    for (size_t i = 0; i < 4; ++i) {
        const auto &entry = g_registeredCommands[i];
        // Name must match the expected sequence
        TEST_CHECK(entry.first == expectedNames[i], std::string("Command name at index ") + std::to_string(i) +
                                                   std::string(" is '") + entry.first +
                                                   std::string("' but expected '") + expectedNames[i] + "'");
        // The function pointer must be non-null
        TEST_CHECK(entry.second != nullptr, "Registered command's xProc must be non-null.");
    }

    return g_test_errors.empty();
}

// Test 2: Verify that subsequent invocations register commands again (state reset by test harness)
bool test_InitRegistersFourCommandsTwice() {
    g_registeredCommands.clear();

    Tcl_Interp interp_dummy;

    // First registration
    int rc1 = Sqlitetest_init_Init(&interp_dummy);
    TEST_CHECK(rc1 == 0, "First call should return TCL_OK (0).");
    TEST_CHECK(g_registeredCommands.size() == 4, "First call should register 4 commands.");

    // Validate first batch briefly
    const char* expectedNames[4] = {
        "init_wrapper_install",
        "init_wrapper_query",
        "init_wrapper_uninstall",
        "init_wrapper_clear"
    };
    for (size_t i = 0; i < 4; ++i) {
        TEST_CHECK(g_registeredCommands[i].first == expectedNames[i], "First batch: name mismatch at index " + std::to_string(i));
        TEST_CHECK(g_registeredCommands[i].second != nullptr, "First batch: xProc non-null at index " + std::to_string(i));
    }

    // Clear and invoke again to simulate independent invocation
    g_registeredCommands.clear();
    int rc2 = Sqlitetest_init_Init(&interp_dummy);
    TEST_CHECK(rc2 == 0, "Second call should return TCL_OK (0).");
    TEST_CHECK(g_registeredCommands.size() == 4, "Second call should register 4 commands again.");

    for (size_t i = 0; i < 4; ++i) {
        TEST_CHECK(g_registeredCommands[i].first == expectedNames[i], "Second batch: name mismatch at index " + std::to_string(i));
        TEST_CHECK(g_registeredCommands[i].second != nullptr, "Second batch: xProc non-null at index " + std::to_string(i));
    }

    return g_test_errors.empty();
}

// -----------------------------------------------------------------------------
// Simple test runner

void runAllTests() {
    g_test_errors.clear();

    bool ok1 = test_InitRegistersFourCommandsOnce();
    if (!ok1) {
        std::cerr << "Test 1 failed with errors:\n";
        for (const auto &e : g_test_errors) std::cerr << "  - " << e << "\n";
    } else {
        std::cout << "Test 1 passed: Init registers four commands once.\n";
    }

    // Reset error list for the next test
    g_test_errors.clear();

    bool ok2 = test_InitRegistersFourCommandsTwice();
    if (!ok2) {
        std::cerr << "Test 2 failed with errors:\n";
        for (const auto &e : g_test_errors) std::cerr << "  - " << e << "\n";
    } else {
        std::cout << "Test 2 passed: Init registers four commands twice with independent invocations.\n";
    }

    if (!ok1 || !ok2) {
        std::cerr << "One or more tests failed. See above messages.\n";
    } else {
        std::cout << "All tests PASSED.\n";
    }
}

// -----------------------------------------------------------------------------
// Main

int main() {
    // Run the test suite. In absence of a test framework, we rely on console output
    // and a non-zero exit code if any test failed.
    runAllTests();

    // If any errors were recorded, exit with non-zero code to indicate failure.
    if (!g_test_errors.empty()) {
        return 1;
    }
    return 0;
}
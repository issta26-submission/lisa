// Test suite for Sqlitetestrtree_Init in test_rtree.c
// This test uses a lightweight, self-contained mock of the Tcl API to verify
// that Sqlitetestrtree_Init correctly registers the two commands:
//  - "register_cube_geom"
//  - "register_circle_geom"
//
// The tests are written in C++11 (no Google Test) and use non-terminating
// assertions (EXPECT-style) to maximize test execution coverage.
// We intentionally avoid touching private/static internals of the focal code.
// The mock Tcl API is provided here to keep the tests self-contained.

#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <map>
#include <iostream>
#include <algorithm>
#include <mutex>


// Domain knowledge and test strategy notes (embedded as comments):
// - Core dependency: Tcl_CreateObjCommand is invoked by Sqlitetestrtree_Init to
//   register two geometry registration commands.
// - The analysis identifies the key candidate keywords: Tcl_CreateObjCommand,
//   "register_cube_geom", "register_circle_geom", Sqlitetestrtree_Init, Tcl_Interp.
// - Static functions (register_cube_geom/register_circle_geom) are file-scope;
//   tests should not attempt to invoke or depend on their internals directly.
// - We only rely on the public surface used by Sqlitetestrtree_Init.
// - We implement a minimal mock of Tcl_Interp and Tcl_CreateObjCommand to
//   validate that the two command registrations occur under test conditions.
// - All tests are non-terminating: failures are recorded and reported, not
//   immediately aborting the test run.


// Mock Tcl API (C linkage) to simulate essential behavior for the focal function.
// This mock is designed to be linked with the C source containing Sqlitetestrtree_Init.

extern "C" {

// Forward declarations to match test_rtree.c types
struct Tcl_Interp; // opaque in test context
struct Tcl_Obj;    // opaque
typedef struct Tcl_Interp Tcl_Interp;
typedef struct Tcl_Obj Tcl_Obj;

// Function pointer type for Tcl command procedures
typedef int (*Tcl_ObjCmdProc)(void *clientData, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[]);

// Public mock API (used by Sqlitetestrtree_Init)
int Tcl_CreateObjCommand(Tcl_Interp *interp, const char *cmdName, Tcl_ObjCmdProc *proc, void *clientData, void *deleteProc);

// Convenience accessors used by tests
int Tcl_CommandExists(Tcl_Interp *interp, const char *name);
void Tcl_ResetAllCommands(); // clears all registered commands across interpreters
}

// Internal storage for registered commands per interpreter
// (Note: This is C++ under the hood, but exposed via extern "C" to satisfy C linkage)
static std::map<Tcl_Interp*, std::vector<std::string>> g_commands;
static std::mutex g_mutex;

extern "C" {

// Implementation of Tcl_CreateObjCommand for the mock
// We only need to record the command name; we do not execute the provided proc.
int Tcl_CreateObjCommand(Tcl_Interp *interp, const char *cmdName, Tcl_ObjCmdProc *proc, void *clientData, void *deleteProc) {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_commands[interp].push_back(std::string(cmdName ? cmdName : ""));
    return 0; // TCL_OK
}

// Check whether a command name has been registered for the given interpreter
int Tcl_CommandExists(Tcl_Interp *interp, const char *name) {
    if (interp == nullptr || name == nullptr) return 0;
    std::lock_guard<std::mutex> lock(g_mutex);
    auto it = g_commands.find(interp);
    if (it == g_commands.end()) return 0;
    const auto &vec = it->second;
    return std::any_of(vec.begin(), vec.end(), [&](const std::string &s){ return s == name; }) ? 1 : 0;
}

// Reset all recorded commands (useful between tests)
void Tcl_ResetAllCommands() {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_commands.clear();
}

} // extern "C"


// Declaration of the focal function under test (from test_rtree.c)
extern "C" int Sqlitetestrtree_Init(Tcl_Interp *interp);

// Lightweight test framework (non-terminating assertions)
class TestSuite {
public:
    void runAll() {
        failures.clear();

        test_InitRegistersTwoCommands();
        test_InitReturnsOk();
        test_InitIsIdempotent();

        report();
    }

private:
    std::vector<std::string> failures;

    // Helper: non-terminating assertion
    void expect(bool condition, const std::string &message) {
        if (!condition) {
            failures.push_back(message);
        }
    }

    // Test 1: After Init, both commands exist
    // Rationale: Core behavior of Sqlitetestrtree_Init is to register two commands.
    void test_InitRegistersTwoCommands() {
        Tcl_ResetAllCommands(); // ensure clean slate

        Tcl_Interp *interp = new Tcl_Interp; // opaque in this test context
        int ret = Sqlitetestrtree_Init(interp);

        bool cubeExists = Tcl_CommandExists(interp, "register_cube_geom");
        bool circleExists = Tcl_CommandExists(interp, "register_circle_geom");

        // Explanatory comments for the test case
        // - The two commands must be created and visible in the interpreter's command registry.
        expect(ret == 0, "Sqlitetestrtree_Init should return TCL_OK (0).");
        expect(cubeExists, "register_cube_geom should be registered by Sqlitetestrtree_Init.");
        expect(circleExists, "register_circle_geom should be registered by Sqlitetestrtree_Init.");

        delete interp;
    }

    // Test 2: Init should be idempotent w.r.t. return value (and not crash)
    // Rationale: Calling Init twice should still return TCL_OK and leave commands registered.
    void test_InitReturnsOk() {
        Tcl_ResetAllCommands();

        Tcl_Interp *interp = new Tcl_Interp;
        int firstRet = Sqlitetestrtree_Init(interp);
        int secondRet = Sqlitetestrtree_Init(interp);

        // Even if duplicate commands are produced, the function should return TCL_OK both times.
        expect(firstRet == 0, "First call: Sqlitetestrtree_Init should return TCL_OK (0).");
        expect(secondRet == 0, "Second call: Sqlitetestrtree_Init should return TCL_OK (0).");

        // Basic sanity: the needed commands still exist after the second call
        bool cubeExists = Tcl_CommandExists(interp, "register_cube_geom");
        bool circleExists = Tcl_CommandExists(interp, "register_circle_geom");
        expect(cubeExists, "register_cube_geom should remain registered after second Init call.");
        expect(circleExists, "register_circle_geom should remain registered after second Init call.");

        delete interp;
    }

    // Test 3: Idempotence of command existence (sanity check)
    // Rationale: Re-running Init should not crash and should still expose the commands.
    void test_InitIsIdempotent() {
        Tcl_ResetAllCommands();

        Tcl_Interp *interp = new Tcl_Interp;
        int ret = Sqlitetestrtree_Init(interp);
        // Call again to simulate repeated initialization in a long-running program
        int ret2 = Sqlitetestrtree_Init(interp);

        expect(ret == 0, "Init first call should succeed.");
        expect(ret2 == 0, "Init second call should succeed.");

        // Commands should exist after both calls
        bool cubeExists = Tcl_CommandExists(interp, "register_cube_geom");
        bool circleExists = Tcl_CommandExists(interp, "register_circle_geom");
        expect(cubeExists && circleExists, "Commands should exist after repeated Init calls.");

        delete interp;
    }

    void report() {
        if (failures.empty()) {
            std::cout << "[PASS] Sqlitetestrtree_Init tests passed.\n";
        } else {
            std::cout << "[FAIL] Sqlitetestrtree_Init tests failed:\n";
            for (auto &msg : failures) {
                std::cout << "  - " << msg << "\n";
            }
        }
    }
};


// Main entry: run the test suite
int main() {
    TestSuite suite;
    suite.runAll();
    return 0;
}
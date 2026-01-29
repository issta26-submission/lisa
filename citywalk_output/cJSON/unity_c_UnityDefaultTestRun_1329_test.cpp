// Test suite for UnityDefaultTestRun in Unity (unity.c) using C++11 (no GTest).
// This test harness focuses on exercising the UnityDefaultTestRun behavior by
// observing side effects inside the setUp, the test function, and tearDown calls.
// It does not rely on GTest or other frameworks and uses a lightweight in-process
// assertion mechanism suitable for C/C++ integration.
//
// Key ideas implemented:
// - Two test scenarios: normal path (Func executes, setUp and tearDown are invoked) and
//   path with a different test function name (to exercise parameter passing via FuncName).
// - Hooks setUp/tearDown are defined in C linkage to align with Unity's expectations.
// - The test suite uses a small sequence log (g_sequence) to verify the order of calls.
// - All tests are executed from main() without external test runners to comply with
//   the constraint of not using GTest and ensuring executability in a C++11 environment.
//
// Note: This file expects to be linked with the Unity library (unity.c, unity.h) in the
// build system. The UnityDefaultTestRun function is invoked directly to simulate test
// execution in a controlled environment.

#include <stddef.h>
#include <algorithm>
#include <unity.h>
#include <vector>
#include <cstdio>
#include <string>


// Forward declaration of the Unity-provided function under test.
// The function uses C linkage; therefore we declare it with extern "C".
extern "C" void UnityDefaultTestRun(void (*Func)(void), const char* FuncName, const int FuncLineNum);

// Global sequence log to observe the order of calls made by UnityDefaultTestRun.
// We use C++ STL for convenient insertion and comparison.
static std::vector<std::string> g_sequence;

// Expose setUp/tearDown with C linkage so UnityDefaultTestRun can call them.
extern "C" void setUp(void);
extern "C" void tearDown(void);

// Test function hooks (also with C linkage) to be invoked by UnityDefaultTestRun.
extern "C" void TestFunc_Normal(void);
extern "C" void TestFunc_Order2(void);

// Implementation of the test hooks.
// They append identifiable markers to g_sequence to verify call order.

extern "C" void setUp(void) {
    g_sequence.push_back("setUp");
}

extern "C" void tearDown(void) {
    g_sequence.push_back("tearDown");
}

extern "C" void TestFunc_Normal(void) {
    g_sequence.push_back("Func");  // Marker for the test function execution
}

extern "C" void TestFunc_Order2(void) {
    g_sequence.push_back("Func2"); // Marker for the second test function
}

// Simple helper to compare two sequences for equality.
static bool sequencesEqual(const std::vector<std::string>& a,
                           const std::vector<std::string>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Lightweight assertion helper that logs a failure but does not abort the process.
// This aligns with the requirement to use non-terminating assertions to maximize coverage.
static void expectTrue(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "ASSERTION FAILED: %s\n", message);
    }
}

// Test 1: Normal path execution.
// Verifies that setUp runs before the test function and tearDown runs after.
static void test_UnityDefaultTestRun_NormalPath() {
    // Prepare a fresh sequence log.
    g_sequence.clear();

    // Execute UnityDefaultTestRun with TestFunc_Normal and a representative name/line.
    UnityDefaultTestRun(TestFunc_Normal, "NormalPath", 101);

    // Expected order: setUp, Func, tearDown
    const std::vector<std::string> expected = { "setUp", "Func", "tearDown" };
    const bool ok = sequencesEqual(g_sequence, expected);

    if (ok) {
        std::printf("PASSED: UnityDefaultTestRun_NormalPath observed correct call order.\n");
    } else {
        std::fprintf(stderr, "FAILED: UnityDefaultTestRun_NormalPath call order mismatch. Expected: [setUp, Func, tearDown], Got: [");
        for (size_t i = 0; i < g_sequence.size(); ++i) {
            if (i) std::fprintf(stderr, ", ");
            std::fprintf(stderr, "%s", g_sequence[i].c_str());
        }
        std::fprintf(stderr, "]\n");
    }

    // Cleanup for next test
    g_sequence.clear();
    expectTrue(ok, "Normal path: call order should be setUp -> Func -> tearDown.");
}

// Test 2: Different FuncName/path order verification.
// Verifies that UnityDefaultTestRun accepts a different FuncName and still performs
// the standard lifecycle (setUp -> test function -> tearDown).
static void test_UnityDefaultTestRun_OrderWithDifferentName() {
    // Prepare a fresh sequence log.
    g_sequence.clear();

    // Execute UnityDefaultTestRun with TestFunc_Order2 and a different test name.
    UnityDefaultTestRun(TestFunc_Order2, "OrderTestName", 202);

    // Expected order: setUp, Func2, tearDown
    const std::vector<std::string> expected = { "setUp", "Func2", "tearDown" };
    const bool ok = sequencesEqual(g_sequence, expected);

    if (ok) {
        std::printf("PASSED: UnityDefaultTestRun_OrderWithDifferentName observed correct call order with alternate FuncName.\n");
    } else {
        std::fprintf(stderr, "FAILED: UnityDefaultTestRun_OrderWithDifferentName mismatch. Got: [");
        for (size_t i = 0; i < g_sequence.size(); ++i) {
            if (i) std::fprintf(stderr, ", ");
            std::fprintf(stderr, "%s", g_sequence[i].c_str());
        }
        std::fprintf(stderr, "]\n");
    }

    // Cleanup for potential future tests
    g_sequence.clear();
    expectTrue(ok, "Order test: call sequence should be setUp -> Func2 -> tearDown.");
}

// Entry point: execute all unit tests.
int main() {
    // Run Test 1: Normal path
    test_UnityDefaultTestRun_NormalPath();

    // Run Test 2: Alternate FuncName path
    test_UnityDefaultTestRun_OrderWithDifferentName();

    // If any assertion failed, the corresponding message would have been printed.
    // Return code 0 to indicate test runner completed (non-fatal assertion style).
    return 0;
}
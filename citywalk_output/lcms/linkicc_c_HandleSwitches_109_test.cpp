// Test suite for HandleSwitches in linkicc.c using a lightweight C++11 test harness.
// This test does not rely on GoogleTest. It uses a minimal assertion framework
// and calls the function under test with constructed argv arrays, checking
// the resulting side effects on global state variables.
// The test ensures true branches execute for valid options and that state is updated accordingly.
// Comments explain the purpose of each test case and the expected outcomes.

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <utils.h>
#include <cmath>


// Bring C linkage for the focal code and its global symbols
extern "C" {
    // Function under test
    void HandleSwitches(int argc, char *argv[]);

    // Lightweight stubs commonly provided by the original project (if not linked elsewhere,
    // providing these declarations helps with correct linkage).
    void Help(int);

    // FatalError is expected to be variadic; the test does not rely on its behavior beyond
    // not crashing during tests that use valid inputs.
    void FatalError(const char*, ...);

    // Global state variables accessed and mutated by HandleSwitches.
    // Types are inferred from usage in the focal method.
    extern double ObserverAdaptationState;
    extern int BlackPointCompensation;
    extern int PrecalcMode;
    extern char *Description;
    extern double InkLimit;
    extern int KeepLinearization;
    extern int NumOfGridPoints;
    extern char *cOutProf;
    extern double Version;
    extern int Intent;
    extern int Verbose;
    extern int lUse8bits;
    extern char *Copyright;
    extern int TagResult;
}

// Minimal test harness
static int gTotal = 0;
static int gFailed = 0;

static void LogFailure(const char* desc) {
    printf("TEST FAILURE: %s\n", desc);
    gFailed++;
}

static void AssertIntEq(int exp, int act, const char* desc) {
    gTotal++;
    if (exp != act) {
        printf("ASSERT FAIL (INT): %s. Expected %d, got %d\n", desc, exp, act);
        gFailed++;
    }
}

static void AssertDoubleEq(double exp, double act, double eps, const char* desc) {
    gTotal++;
    if (std::fabs(exp - act) > eps) {
        printf("ASSERT FAIL (DOUBLE): %s. Expected %f, got %f (eps=%f)\n", desc, exp, act, eps);
        gFailed++;
    }
}

static void AssertPtrEq(const void* exp, const void* act, const char* desc) {
    gTotal++;
    if (exp != act) {
        printf("ASSERT FAIL (PTR): %s. Expected %p, got %p\n", desc, exp, act);
        gFailed++;
    }
}

// Helpers to reset global state before each test
static void ResetGlobals() {
    // Initialize to safe defaults
    ObserverAdaptationState = 0.0;
    BlackPointCompensation = 0;
    PrecalcMode = 0;
    Description = nullptr;
    InkLimit = 0.0;
    KeepLinearization = 0;
    NumOfGridPoints = 0;
    cOutProf = nullptr;
    Version = 0.0;
    Intent = 0;
    Verbose = 0;
    lUse8bits = 0;
    Copyright = nullptr;
    TagResult = 0;
}

// Test 1: No arguments - ensure no changes are made to global state
static void test_NoArgs_NoChange() {
    ResetGlobals();
    const char* prog = "prog";
    char* argv[1];
    argv[0] = const_cast<char*>(prog);

    HandleSwitches(1, argv);

    // All should remain at defaults (0/NULL)
    AssertDoubleEq(0.0, ObserverAdaptationState, 1e-9, "ObserverAdaptationState remains 0");
    AssertIntEq(0, BlackPointCompensation, "BlackPointCompensation remains 0");
    AssertIntEq(0, PrecalcMode, "PrecalcMode remains 0");
    AssertPtrEq(nullptr, Description, "Description remains nullptr");
    AssertDoubleEq(0.0, InkLimit, 1e-9, "InkLimit remains 0");
    AssertIntEq(0, KeepLinearization, "KeepLinearization remains 0");
    AssertIntEq(0, NumOfGridPoints, "NumOfGridPoints remains 0");
    AssertPtrEq(nullptr, cOutProf, "cOutProf remains nullptr");
    AssertDoubleEq(0.0, Version, 1e-9, "Version remains 0");
    AssertIntEq(0, Intent, "Intent remains 0");
    AssertIntEq(0, Verbose, "Verbose remains 0");
    AssertIntEq(0, lUse8bits, "lUse8bits remains 0");
    AssertPtrEq(nullptr, Copyright, "Copyright remains nullptr");
    AssertIntEq(0, TagResult, "TagResult remains 0");
}

// Test 2: Set ObserverAdaptationState via -a
static void test_SetObserverAdaptationState() {
    ResetGlobals();
    const char* prog = "prog";
    const char* arg = "0.5";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-a");
    argv[2] = const_cast<char*>(arg);

    HandleSwitches(3, argv);

    AssertDoubleEq(0.5, ObserverAdaptationState, 1e-6, "ObserverAdaptationState should be 0.5");
}

// Test 3: Enable BlackPointCompensation with -b
static void test_SetBlackPointCompensation() {
    ResetGlobals();
    const char* prog = "prog";
    char* argv[2];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-b");

    HandleSwitches(2, argv);

    AssertIntEq(1, BlackPointCompensation, "BlackPointCompensation should be set to TRUE (1)");
}

// Test 4: Set PrecalcMode with -c
static void test_SetPrecalcMode() {
    ResetGlobals();
    const char* prog = "prog";
    const char* arg = "1";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-c");
    argv[2] = const_cast<char*>(arg);

    HandleSwitches(3, argv);

    AssertIntEq(1, PrecalcMode, "PrecalcMode should be 1");
}

// Test 5: Set NumOfGridPoints when PrecalcMode is 1 (-n)
static void test_SetNumOfGridPoints_WithPrecalcMode1() {
    ResetGlobals();
    PrecalcMode = 1; // Force the precondition
    const char* prog = "prog";
    const char* arg = "10";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-n");
    argv[2] = const_cast<char*>(arg);

    HandleSwitches(3, argv);

    AssertIntEq(10, NumOfGridPoints, "NumOfGridPoints should be 10 when PrecalcMode is 1");
}

// Test 6: Set InkLimit with -k
static void test_SetInkLimit() {
    ResetGlobals();
    const char* prog = "prog";
    const char* arg = "275.0";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-k");
    argv[2] = const_cast<char*>(arg);

    HandleSwitches(3, argv);

    AssertDoubleEq(275.0, InkLimit, 1e-6, "InkLimit should be 275.0");
}

// Test 7: Set Output Profile path with -o
static void test_SetOutputProfile() {
    ResetGlobals();
    const char* prog = "prog";
    const char* arg = "output.prof";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-o");
    argv[2] = const_cast<char*>(arg);

    HandleSwitches(3, argv);

    AssertPtrEq(reinterpret_cast<void*>(arg), reinterpret_cast<void*>(cOutProf), "cOutProf should point to the provided string");
}

// Test 8: Set Verbose level with -V
static void test_SetVerboseLevel() {
    ResetGlobals();
    const char* prog = "prog";
    const char* arg = "2";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-V");
    argv[2] = const_cast<char*>(arg);

    HandleSwitches(3, argv);

    AssertIntEq(2, Verbose, "Verbose should be set to 2");
}

// Test 9: Set lUse8bits with -8
static void test_Set8BitsFlag() {
    ResetGlobals();
    const char* prog = "prog";
    char* argv[2];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-8");

    HandleSwitches(2, argv);

    AssertIntEq(1, lUse8bits, "lUse8bits should be TRUE (1)");
}

// Test 10: Set TagResult with -x
static void test_SetTagResult() {
    ResetGlobals();
    const char* prog = "prog";
    char* argv[2];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-x");

    HandleSwitches(2, argv);

    AssertIntEq(1, TagResult, "TagResult should be TRUE (1)");
}

// Test 11: Set Copyright with -y
static void test_SetCopyright() {
    ResetGlobals();
    const char* prog = "prog";
    const char* arg = "© 2024 Example";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("-y");
    argv[2] = const_cast<char*>(arg);

    HandleSwitches(3, argv);

    AssertPtrEq(reinterpret_cast<void*>(arg), reinterpret_cast<void*>(Copyright), "Copyright should point to provided string");
}

// Run all tests
int main() {
    printf("Starting HandleSwitches unit tests...\n");

    test_NoArgs_NoChange();
    test_SetObserverAdaptationState();
    test_SetBlackPointCompensation();
    test_SetPrecalcMode();
    test_SetNumOfGridPoints_WithPrecalcMode1();
    test_SetInkLimit();
    test_SetOutputProfile();
    test_SetVerboseLevel();
    test_Set8BitsFlag();
    test_SetTagResult();
    test_SetCopyright();

    printf("Tests completed. Total: %d, Failed: %d\n", gTotal, gFailed);
    // Return non-zero if any test failed
    return (gFailed == 0) ? 0 : 1;
}
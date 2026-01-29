// Candidate Keywords extracted from the focal method and its class dependencies
// - xgetopt, xoptarg: option parsing engine and its argument holder
// - cInProf, cOutProf: input/output profile path strings
// - BlackPointCompensation: flag (TRUE/FALSE) for BT compensation
// - Intent: numeric interpretation setting (0..3)
// - Undecorated: flag for undecorated option
// - PrecalcMode: numeric mode selector (0..2) with validation
// - NumOfGridPoints: numeric parameter used when PrecalcMode == 1
// - Help(), FatalError(): utility functions called from switch handling
// - GenerateCSA(), GenerateCRD(): other class members not directly exercised here
// The tests use HandleSwitches(int argc, char *argv[]) as the focal method
// and verify the side effects on the above global/state variables.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <utils.h>


// Step 1: Declare the external symbols used by the focal method.
// We rely on the project providing the actual definitions; here we only declare
// the necessary interfaces to exercise HandleSwitches from C++ test code.

extern "C" {
    // Focal method to test
    void HandleSwitches(int argc, char *argv[]);

    // Globals used/modified by HandleSwitches
    extern char *cInProf;
    extern char *cOutProf;
    extern int BlackPointCompensation;
    extern int Intent;
    extern int Undecorated;
    extern int PrecalcMode;
    extern int NumOfGridPoints;

    // getopt-like helpers used by HandleSwitches
    extern char *xoptarg;
    int xgetopt(int argc, char *argv[], const char *optstring);
}

// Simple non-terminating assertions framework (non-GTest)
static int gTestsRun = 0;
static int gTestsPassed = 0;

#define TEST_PASS(name) \
    do { \
        ++gTestsRun; \
        std::cout << "[PASS] " << name << std::endl; \
        ++gTestsPassed; \
    } while (0)

#define TEST_FAIL(name) \
    do { \
        ++gTestsRun; \
        std::cout << "[FAIL] " << name << std::endl; \
    } while (0)

#define ASSERT_TRUE(cond, desc) \
    do { \
        if (cond) { TEST_PASS(desc); } else { TEST_FAIL(desc); } \
    } while (0)

#define ASSERT_STR_EQ(actual, expected, desc) \
    do { \
        bool ok = false; \
        if (actual == nullptr && expected == nullptr) ok = true; \
        else if (actual != nullptr && expected != nullptr) ok = (std::strcmp(actual, expected) == 0); \
        if (ok) { TEST_PASS(desc); } else { TEST_FAIL(desc); } \
    } while (0)


// Helper: reset all global state used by HandleSwitches to a clean baseline
static void resetGlobals() {
    cInProf = nullptr;
    cOutProf = nullptr;
    BlackPointCompensation = 0;
    Intent = 0;
    Undecorated = 0;
    PrecalcMode = 0;
    NumOfGridPoints = 0;
}

// Helper: create argv array from a vector of strings, ensuring modifiable buffers
static char** buildArgv(const std::vector<std::string>& args, int& argcOut) {
    // Create modifiable buffers to allow xgetopt to alter them if needed
    std::vector<char*> argv;
    argv.reserve(args.size());
    for (const auto& s : args) {
        char* buf = new char[s.size() + 1];
        std::strcpy(buf, s.c_str());
        argv.push_back(buf);
    }

    // Expose argc
    argcOut = static_cast<int>(argv.size());

    // Move pointers to a separate array that can be freed after use
    char** argvOut = new char*[argv.size()];
    for (size_t i = 0; i < argv.size(); ++i) argvOut[i] = argv[i];
    // Note: responsibility to free argvOut and its contained buffers lies with caller
    // We'll return a pointer to a contiguous array of char*; caller stores it and frees.
    // To keep code simple, we'll allocate a new copy to return.
    // But we already allocated argv; we need to return argvOut and ensure buffers are freed after test.
    return argvOut;
}

// Overloaded helper to simplify test code: build and return argv and argc
static void freeArgv(char** argv, int argc) {
    if (argv == nullptr) return;
    // Each argv[i] was allocated in buildArgv; free them
    // The content buffers are stored in separate strings' buffers; we allocated them in buildArgv
    // Recreate the same as in buildArgv to free in a reliable way is complex here.
    // Instead, we re-implement a straightforward version: in our buildArgv we'll manage both.
}

// To avoid confusion, implement a more explicit argv builder that returns both argc and a vector of buffers
static void buildArgvExplicit(const std::vector<std::string>& args,
                             int& argcOut,
                             std::vector<char*>& outBufs) {
    outBufs.clear();
    for (const auto& s : args) {
        char* buf = new char[s.size() + 1];
        std::strcpy(buf, s.c_str());
        outBufs.push_back(buf);
    }
    argcOut = static_cast<int>(outBufs.size());
}

static void freeArgvExplicit(std::vector<char*>& bufs) {
    for (char* p : bufs) {
        delete[] p;
    }
    bufs.clear();
}

// Test Case 1: Comprehensive option parsing with multiple options set
// -i in.prof, -O out.prof, -b, -t 2, -u, -c 1, -n 5
// Verifies that all corresponding globals are updated accordingly.
static bool test_HandleSwitches_Comprehensive() {
    resetGlobals();

    std::vector<std::string> args = {
        "testprog",     // argv[0]
        "-i", "in.prof",
        "-O", "out.prof",
        "-b",
        "-t", "2",
        "-u",
        "-c", "1",
        "-n", "5"
    };

    int argc;
    std::vector<char*> argvBuf;
    buildArgvExplicit(args, argc, argvBuf);

    // Build plain array of char* for the API
    std::vector<char*> argvVec = argvBuf;
    HandleSwitches(argc, argvVec.data());

    // Clean up buffers
    freeArgvExplicit(argvBuf);

    // Validate outcomes
    bool ok = true;
    ok &= (cInProf != nullptr && std::strcmp(cInProf, "in.prof") == 0);
    ok &= (cOutProf != nullptr && std::strcmp(cOutProf, "out.prof") == 0);
    ok &= (BlackPointCompensation == 1);
    ok &= (Intent == 2);
    ok &= (Undecorated == 1);
    ok &= (PrecalcMode == 1);
    ok &= (NumOfGridPoints == 5);

    ASSERT_TRUE(ok, "HandleSwitches: Comprehensive multi-option parsing");

    // Additionally, explicit string checks with detailed notes
    ASSERT_STR_EQ(cInProf, "in.prof", "HandleSwitches: cInProf should be 'in.prof'");
    ASSERT_STR_EQ(cOutProf, "out.prof", "HandleSwitches: cOutProf should be 'out.prof'");

    return ok;
}

// Test Case 2: Intent clamping when negative value is provided (-t -1 -> Intent becomes 0)
static bool test_HandleSwitches_IntentNegativeClamp() {
    resetGlobals();

    std::vector<std::string> args = {
        "testprog",
        "-t", "-1"
    };

    int argc;
    std::vector<char*> argvBuf;
    buildArgvExplicit(args, argc, argvBuf);

    HandleSwitches(argc, argvBuf.data());

    freeArgvExplicit(argvBuf);

    bool ok = (Intent == 0);
    ASSERT_TRUE(ok, "HandleSwitches: Intent should clamp to 0 when -t -1 is provided");
    // Also ensure other fields unaffected
    ASSERT_TRUE(cInProf == nullptr, "HandleSwitches: cInProf remains nullptr");
    return ok;
}

// Test Case 3: Intent upper bound clamping when value is 3 (within range, should be kept as 3)
static bool test_HandleSwitches_IntentUpperBound() {
    resetGlobals();

    std::vector<std::string> args = {
        "testprog",
        "-t", "3"
    };

    int argc;
    std::vector<char*> argvBuf;
    buildArgvExplicit(args, argc, argvBuf);

    HandleSwitches(argc, argvBuf.data());

    freeArgvExplicit(argvBuf);

    bool ok = (Intent == 3);
    ASSERT_TRUE(ok, "HandleSwitches: Intent should be 3 when -t 3 is provided");
    return ok;
}

// Test Case 4: Undecorated flag is set via -u
static bool test_HandleSwitches_UndecoratedFlag() {
    resetGlobals();

    std::vector<std::string> args = {
        "testprog",
        "-u"
    };

    int argc;
    std::vector<char*> argvBuf;
    buildArgvExplicit(args, argc, argvBuf);

    HandleSwitches(argc, argvBuf.data());

    freeArgvExplicit(argvBuf);

    bool ok = (Undecorated == 1);
    ASSERT_TRUE(ok, "HandleSwitches: Undecorated flag should be set with -u");
    return ok;
}

// Test Case 5: PrecalcMode = 0 (within range) via -c 0
static bool test_HandleSwitches_PrecalcModeZero() {
    resetGlobals();

    std::vector<std::string> args = {
        "testprog",
        "-c", "0"
    };

    int argc;
    std::vector<char*> argvBuf;
    buildArgvExplicit(args, argc, argvBuf);

    HandleSwitches(argc, argvBuf.data());

    freeArgvExplicit(argvBuf);

    bool ok = (PrecalcMode == 0);
    ASSERT_TRUE(ok, "HandleSwitches: PrecalcMode should be 0 when -c 0 is provided");
    return ok;
}

// Test Case 6: NumOfGridPoints is set only when PrecalcMode == 1
static bool test_HandleSwitches_NumpointsWhenPrecalcModeOne() {
    resetGlobals();

    std::vector<std::string> args = {
        "testprog",
        "-c", "1",
        "-n", "8"
    };

    int argc;
    std::vector<char*> argvBuf;
    buildArgvExplicit(args, argc, argvBuf);

    HandleSwitches(argc, argvBuf.data());

    freeArgvExplicit(argvBuf);

    bool ok = (PrecalcMode == 1) && (NumOfGridPoints == 8);
    ASSERT_TRUE(ok, "HandleSwitches: NumOfGridPoints should be set to 8 when PrecalcMode is 1");
    return ok;
}

// Step 3: Test runner (main entry) - executes all tests
int main() {
    std::cout << "Starting unit tests for HandleSwitches (C++11, no GTest)" << std::endl;

    bool t1 = test_HandleSwitches_Comprehensive();
    bool t2 = test_HandleSwitches_IntentNegativeClamp();
    bool t3 = test_HandleSwitches_IntentUpperBound();
    bool t4 = test_HandleSwitches_UndecoratedFlag();
    bool t5 = test_HandleSwitches_PrecalcModeZero();
    bool t6 = test_HandleSwitches_NumpointsWhenPrecalcModeOne();

    int total = 6;
    int passed = (t1?1:0) + (t2?1:0) + (t3?1:0) + (t4?1:0) + (t5?1:0) + (t6?1:0);

    std::cout << "Unit tests completed: " << passed << " / " << total << " passed." << std::endl;
    if (passed == total) return 0;
    return 1;
}
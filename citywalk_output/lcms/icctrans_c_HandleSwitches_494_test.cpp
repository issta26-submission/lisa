// C++11 test suite for the focal method HandleSwitches in icctrans.c
// This test suite uses a lightweight, non-Google-Test framework with custom checks.
// It relies on the global state and behavior exposed by the focal module's API.

#include <cstring>
#include <mex.h>
#include <stdarg.h>
#include <iostream>
#include <lcms2.h>
#include <string.h>


// Declare the focal function and its global variables with C linkage for linking against icctrans.c
extern "C" {
    void HandleSwitches(int argc, char *argv[]);
    // Global state expected by HandleSwitches (as used in the focal code)
    extern int PrecalcMode;
    extern int Intent;
    extern int ProofingIntent;
    extern int lIsDeviceLink;
    extern int lMultiProfileChain;
    extern char* cInProf;
    extern char* cOutProf;
    extern char* cProofing;
    extern int Verbose;
    extern int BlackPointCompensation;
}

// Lightweight test harness
static int gFailures = 0;

#define CHECK(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "TEST FAILURE: " << (msg) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++gFailures; \
        } \
    } while(0)

static void reset_globals() {
    // Initialize the global state to known defaults before each test
    // The specific default values depend on the project; we reset to safe placeholders.
    PrecalcMode = -1;
    Intent = 0;
    ProofingIntent = 0;
    lIsDeviceLink = 0;
    lMultiProfileChain = 0;
    cInProf = nullptr;
    cOutProf = nullptr;
    cProofing = nullptr;
    Verbose = 0;
    BlackPointCompensation = 0;
}

// Test 1: Verify that -c N properly sets PrecalcMode within valid range [0,3].
//
// This ensures the core parsing path for precalc mode is exercised.
static void test_PrecalcModeSet() {
    reset_globals();

    const char* prog = "prog";
    char arg1[] = "-c";
    char arg2[] = "2";
    char* argv[] = { const_cast<char*>(prog), arg1, arg2 };
    int argc = 3;

    HandleSwitches(argc, argv);

    CHECK(PrecalcMode == 2, "PrecalcMode should be set to 2 by -c 2");
}

// Test 2: Verify that -i <inprof> sets the input profile path when no device-link is active.
// This covers the typical path for specifying an input ICC profile.
static void test_InProfSet() {
    reset_globals();

    const char* prog = "prog";
    char arg1[] = "-i";
    char arg2[] = "input.icc";
    char* argv[] = { const_cast<char*>(prog), arg1, arg2 };
    int argc = 3;

    HandleSwitches(argc, argv);

    CHECK(cInProf != nullptr, "cInProf should be set after -i option");
    if (cInProf)
        CHECK(std::strcmp(cInProf, "input.icc") == 0, "cInProf should be 'input.icc'");
}

// Test 3: Verify that -o <outprof> sets the output profile path when no device-link is active.
// This ensures correct handling of output profile specification.
static void test_OutProfSet() {
    reset_globals();

    const char* prog = "prog";
    char arg1[] = "-o";
    char arg2[] = "output.icc";
    char* argv[] = { const_cast<char*>(prog), arg1, arg2 };
    int argc = 3;

    HandleSwitches(argc, argv);

    CHECK(cOutProf != nullptr, "cOutProf should be set after -o option");
    if (cOutProf)
        CHECK(std::strcmp(cOutProf, "output.icc") == 0, "cOutProf should be 'output.icc'");
}

// Test 4: Verify that -v and -b set Verbose and BlackPointCompensation respectively.
// This checks separate flags that should both be enabled in a single invocation.
static void test_VerboseAndBlackPoint() {
    reset_globals();

    const char* prog = "prog";
    char arg1[] = "-v";
    char arg2[] = "-b";
    char* argv[] = { const_cast<char*>(prog), arg1, arg2 };
    int argc = 3;

    HandleSwitches(argc, argv);

    CHECK(Verbose != 0, "Verbose should be TRUE after -v");
    CHECK(BlackPointCompensation != 0, "BlackPointCompensation should be TRUE after -b");
}

// Test 5: Verify that -m enables multiprofile chaining.
// This ensures the pathway for multi-profile transforms is activated.
static void test_MultiProfileSet() {
    reset_globals();

    const char* prog = "prog";
    char arg1[] = "-m";
    char* argv[] = { const_cast<char*>(prog), arg1 };
    int argc = 2;

    HandleSwitches(argc, argv);

    CHECK(lMultiProfileChain != 0, "lMultiProfileChain should be TRUE after -m");
}

// Test 6: Verify that -t <value> clamps negative values to 0.
// This ensures sane handling of negative intents.
static void test_IntentClampNegative() {
    reset_globals();

    const char* prog = "prog";
    char arg1[] = "-t";
    char arg2[] = "-1"; // negative intent
    char* argv[] = { const_cast<char*>(prog), arg1, arg2 };
    int argc = 3;

    HandleSwitches(argc, argv);

    CHECK(Intent == 0, "Intent should be clamped to 0 for -t -1");
}

// Test 7: Verify that -r <value> clamps negative values to 0.
// This ensures sane handling of negative proofing intents.
static void test_ProofingIntentClampNegative() {
    reset_globals();

    const char* prog = "prog";
    char arg1[] = "-r";
    char arg2[] = "-2"; // negative proofing intent
    char* argv[] = { const_cast<char*>(prog), arg1, arg2 };
    int argc = 3;

    HandleSwitches(argc, argv);

    CHECK(ProofingIntent == 0, "ProofingIntent should be clamped to 0 for -r -2");
}

// Runner
int main() {
    // Run the test cases
    test_PrecalcModeSet();
    test_InProfSet();
    test_OutProfSet();
    test_VerboseAndBlackPoint();
    test_MultiProfileSet();
    test_IntentClampNegative();
    test_ProofingIntentClampNegative();

    if (gFailures > 0) {
        std::cerr << gFailures << " test(s) failed\n";
        return 1;
    } else {
        std::cout << "All tests passed\n";
        return 0;
    }
}
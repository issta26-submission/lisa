// C++11 test harness for the focal main (jpgicc.c) using source-inclusion technique.
// We rename the C entry point to test_main, include the C source, and drive it
// via a small C++ test harness. The tests stub out external dependencies and
// verify the behavior of main under controlled scenarios.
//
// The test harness prints results and uses lightweight assertions that do not
// terminate all tests on a single failure, per domain guidance.

// Rename the C main to test_main and include the C implementation.
#define main test_main
#include <cstring>
#include <iccjpeg.h>
#include <jpgicc.c>
#include <jpeglib.h>
#include <iostream>
#include <utils.h>


// Lightweight, non-terminating assertion macro for test reporting.

static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

// Global test state that stubs will populate to verify behavior.
struct TestState {
    bool InitUtilsCalled = false;
    bool HandleSwitchesCalled = false;
    bool OpenInputCalled = false;
    bool OpenOutputCalled = false;
    bool TransformCalled = false;
    bool DoneCalled = false;
    bool HelpCalled = false;
    int HelpLevel = -1;
    const char* OpenInputArg = nullptr;
    const char* OpenOutputArg = nullptr;
    const char* TransformInp = nullptr;
    const char* TransformOut = nullptr;
};

// Instantiate global test state. The stubs in the included C file will modify this.
static TestState g_state;

// Forward declaration of the test main (renamed in the included C source).
extern int test_main(int argc, char* argv[]);

// Stubbed implementations of dependencies (C linkage) to satisfy the focal main.
// These stubs are visible to the included jpgicc.c and override real behavior.

extern "C" {

    // Global placeholders that the focal main expects to exist and be mutable.
    int xoptind = 1;      // First non-option argument index (typical getopt outcome).
    int Verbose = 0;      // Verbose flag used in the focal main.
    char* cInpProf = nullptr;
    char* cOutProf = nullptr;

    // Initialization mimic
    void InitUtils(const char* /*progName*/) {
        g_state.InitUtilsCalled = true;
    }

    // Option processing mimic
    void HandleSwitches(int /*argc*/, char* /*argv*/[]) {
        g_state.HandleSwitchesCalled = true;
        // Simulate typical behavior: after handling switches, xoptind points to
        // the first non-option arg. Tests set it to 1 to indicate two non-option args follow.
        xoptind = 1;
    }

    // Input/Output handling stubs
    int OpenInput(const char* FileName) {
        g_state.OpenInputCalled = true;
        g_state.OpenInputArg = FileName;
        return 1;
    }

    int OpenOutput(const char* FileName) {
        g_state.OpenOutputCalled = true;
        g_state.OpenOutputArg = FileName;
        return 1;
    }

    // Transform step stub
    int TransformImage(char* cDefInpProf, char* cOutputProf) {
        g_state.TransformCalled = true;
        g_state.TransformInp = cDefInpProf;
        g_state.TransformOut = cOutputProf;
        // Simulate success
        return 0;
    }

    // Finalize
    int Done(void) {
        g_state.DoneCalled = true;
        return 1;
    }

    // LittleCMS version primitive
    int cmsGetEncodedCMMversion(void) {
        // Return a fixed value that formats nicely in the header print.
        return 4000; // 4.000
    }

    // Help/usage printout
    void Help(int level) {
        g_state.HelpCalled = true;
        g_state.HelpLevel = level;
    }

} // extern "C"

// A small helper to reset test state before each test
static void reset_state() {
    g_state = TestState{};
    xoptind = 1;
    // Default fake inputs for a clean slate (not strictly required for all tests)
    cInpProf = nullptr;
    cOutProf = nullptr;
}

// Test 1: Normal flow with exactly two non-option args (in.jpg -> out.jpg)
static void test_normal_flow() {
    reset_state();

    // Prepare argv: program name, input, output
    char prog[] = "jpgicc";
    char inName[] = "in.jpg";
    char outName[] = "out.jpg";
    char* argv[] = { prog, inName, outName };
    int argc = 3;

    // Run the focal main (renamed to test_main in the included C file)
    int ret = test_main(argc, argv);

    // Assertions
    TEST_ASSERT(ret == 0, "main should return 0 on normal two-arg path");
    TEST_ASSERT(g_state.InitUtilsCalled, "InitUtils should be invoked");
    TEST_ASSERT(g_state.HandleSwitchesCalled, "HandleSwitches should be invoked");
    TEST_ASSERT(g_state.OpenInputCalled, "OpenInput should be invoked");
    TEST_ASSERT(g_state.OpenInputArg && std::strcmp(g_state.OpenInputArg, inName) == 0,
                "OpenInput should receive input filename");
    TEST_ASSERT(g_state.OpenOutputCalled, "OpenOutput should be invoked");
    TEST_ASSERT(g_state.OpenOutputArg && std::strcmp(g_state.OpenOutputArg, outName) == 0,
                "OpenOutput should receive output filename");
    TEST_ASSERT(g_state.TransformCalled, "TransformImage should be invoked");
    TEST_ASSERT(g_state.TransformInp != nullptr && g_state.TransformOut != nullptr,
                "TransformImage should receive input/output prof pointers");
    TEST_ASSERT(g_state.DoneCalled, "Done should be invoked");
    TEST_ASSERT(!g_state.HelpCalled, "Help should not be invoked in normal flow");
}

// Test 2: Wrong arg count triggers Help (but continues to open files as per the code path)
static void test_help_called_when_wrong_args() {
    reset_state();

    // Prepare argv: program name, in.jpg, out.jpg, extra (to trigger argc - xoptind != 2)
    char prog[] = "jpgicc";
    char inName[] = "in.jpg";
    char outName[] = "out.jpg";
    char extra[] = "extra";
    char* argv[] = { prog, inName, outName, extra };
    int argc = 4;

    // Run
    int ret = test_main(argc, argv);

    // Assertions
    TEST_ASSERT(ret == 0, "main should return 0 even when Help is triggered");
    TEST_ASSERT(g_state.HelpCalled, "Help should be invoked when arg count is not 2");
    TEST_ASSERT(g_state.HelpLevel == 0, "Help should be invoked with level 0 (as in source)");
    // And still proceed to typical flow after Help call
    TEST_ASSERT(g_state.OpenInputCalled, "OpenInput should be invoked even when Help is triggered");
    TEST_ASSERT(g_state.OpenOutputCalled, "OpenOutput should be invoked even when Help is triggered");
}

// Test runner orchestrating all tests and reporting summary
static void run_all_tests() {
    test_normal_flow();
    test_help_called_when_wrong_args();
}

// Public test entrypoint (the test harness's main)
int main() {
    run_all_tests();

    if (g_failures > 0) {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}
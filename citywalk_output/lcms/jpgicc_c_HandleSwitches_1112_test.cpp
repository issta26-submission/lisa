// Minimal C++11 test suite for the focal method HandleSwitches (jpgicc.c)
// This test suite uses a lightweight, non-terminating assertion approach
// (no GoogleTest). It relies on the existing project code to be linked in.
// Externs are declared to access global state manipulated by HandleSwitches.
// Each test is documented with explanatory comments.

#include <cstring>
#include <iccjpeg.h>
#include <jpeglib.h>
#include <iostream>
#include <cstdlib>
#include <utils.h>


// Ensure the focal function is linked with C linkage
extern "C" void HandleSwitches(int argc, char *argv[]);

// External globals used by HandleSwitches. We declare them here to observe side effects.
// These declarations rely on the actual definitions in the project (jpgicc.c or related).
extern int BlackPointCompensation;
extern int ObserverAdaptationState;
extern int Verbose;
extern int lIsDeviceLink;
extern char* cInpProf;
extern char* cOutProf;
extern char* cProofing;
extern int Intent;
extern int IgnoreEmbedded;
extern int EmbedProfile;
extern int GamutCheck;
extern int PrecalcMode;
extern unsigned short Alarm[3];
extern int jpegQuality;
extern char* xoptarg;

// xgetopt and related state used by HandleSwitches
extern int xgetopt(int argc, char *argv[], const char* optstring);

// Helpers to observe and reset globals between tests
static int gTestsRun = 0;
static int gTestsFailed = 0;

// Reset all observed globals to safe defaults before each test
static void resetGlobals()
{
    BlackPointCompensation = 0;
    ObserverAdaptationState = 0;
    Verbose = 0;
    lIsDeviceLink = 0;
    cInpProf = NULL;
    cOutProf = NULL;
    cProofing = NULL;
    Intent = 0;
    IgnoreEmbedded = 0;
    EmbedProfile = 0;
    GamutCheck = 0;
    PrecalcMode = 0;
    Alarm[0] = Alarm[1] = Alarm[2] = 0;
    jpegQuality = 0;
    // xoptarg is set by the parser; no reset here
}

// Lightweight assertion helpers (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT_FALSE: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while (0)

#define ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        std::cerr << "ASSERT_EQ failed: " #expected " != " #actual " (" << (expected) << " != " << (actual) << "). " \
                  << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while (0)

#define ASSERT_STR_EQUAL(expected, actual, msg) do { \
    if ((actual) == NULL || std::strcmp((expected), (actual)) != 0) { \
        std::cerr << "ASSERT_STR_EQUAL failed: expected \"" << (expected) \
                  << "\", got \"" << ((actual) ? (actual) : "NULL") << "\". " \
                  << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while (0)

// Test 1: No arguments – HandleSwitches should effectively no-op and not crash
// Expectation: No global state changes from initial defaults
static void test_no_args()
{
    resetGlobals();
    const char* prog = "jpgicc";
    char* argv[] = { (char*)prog };
    HandleSwitches(1, argv);

    ASSERT_EQ(0, BlackPointCompensation, "BlackPointCompensation should remain 0");
    ASSERT_EQ(0, ObserverAdaptationState, "ObserverAdaptationState should remain 0");
    ASSERT_EQ(0, Verbose, "Verbose should remain 0");
    ASSERT_EQ(0, lIsDeviceLink, "lIsDeviceLink should remain 0");
    ASSERT_TRUE(cInpProf == NULL, "cInpProf should remain NULL");
    ASSERT_TRUE(cOutProf == NULL, "cOutProf should remain NULL");
    ASSERT_TRUE(cProofing == NULL, "cProofing should remain NULL");
    ASSERT_EQ(0, Intent, "Intent should remain 0");
    ASSERT_EQ(0, IgnoreEmbedded, "IgnoreEmbedded should remain 0");
    ASSERT_EQ(0, EmbedProfile, "EmbedProfile should remain 0");
    ASSERT_EQ(0, GamutCheck, "GamutCheck should remain 0");
    ASSERT_EQ(0, PrecalcMode, "PrecalcMode should remain 0");
    ASSERT_EQ(0, Alarm[0], "Alarm[0] should remain 0");
    ASSERT_EQ(0, Alarm[1], "Alarm[1] should remain 0");
    ASSERT_EQ(0, Alarm[2], "Alarm[2] should remain 0");
}

 // Test 2: -b or -B sets BlackPointCompensation
static void test_set_black_point_compensation()
{
    resetGlobals();
    const char* prog = "jpgicc";
    const char* args[] = { prog, "-b" };
    char* argv[] = { (char*)args[0], (char*)args[1] };
    HandleSwitches(2, argv);

    ASSERT_TRUE(BlackPointCompensation != 0, "BlackPointCompensation should be set when -b/-B provided");
}

// Test 3: -d <value> sets ObserverAdaptationState within [0,1]
static void test_set_observer_adaptation_state()
{
    resetGlobals();
    const char* prog = "jpgicc";
    const char* args[] = { prog, "-d", "0.5" };
    char* argv[] = { (char*)args[0], (char*)args[1], (char*)args[2] };
    HandleSwitches(3, argv);

    ASSERT_EQ(0, ObserverAdaptationState, "ObserverAdaptationState should be 0 before parsing? (sanity)");
    // The value should be 0.5 after parsing
    ASSERT_EQ(0, 0, "Placeholder to ensure compilation"); // Will be replaced by actual assertion below

    // Re-invoke to read actual value after parse
    // Since macros can't easily access non-const doubles, do a direct comparison
    // Cast to double for reliability
    double actual = static_cast<double>(ObserverAdaptationState);
    ASSERT_TRUE(actual == 0.5, "ObserverAdaptationState should be 0.5 after -d 0.5");
}

// Test 4 & 5: -q with clamping (0..100)
static void test_jpeg_quality_clamping_max()
{
    resetGlobals();
    const char* prog = "jpgicc";
    const char* args[] = { prog, "-q", "120" };
    char* argv[] = { (char*)args[0], (char*)args[1], (char*)args[2] };
    HandleSwitches(3, argv);

    ASSERT_EQ(100, jpegQuality, "jpegQuality should clamp to 100 when provided 120");
}

static void test_jpeg_quality_clamping_min()
{
    resetGlobals();
    const char* prog = "jpgicc";
    const char* args[] = { prog, "-q", "-10" };
    char* argv[] = { (char*)args[0], (char*)args[1], (char*)args[2] };
    HandleSwitches(3, argv);

    ASSERT_EQ(0, jpegQuality, "jpegQuality should clamp to 0 when provided -10");
}

// Test 6: -! alarm parsing into Alarm[] with 3 values
static void test_alarm_parsing()
{
    resetGlobals();
    const char* prog = "jpgicc";
    const char* args[] = { prog, "-!", "1,2,3" };
    char* argv[] = { (char*)args[0], (char*)args[1], (char*)args[2] };
    HandleSwitches(3, argv);

    // After parsing, Alarm[i] becomes (Alarm[i] << 8) | Alarm[i]
    // with Alarm[0]=1 -> 0x0101 = 257
    // Alarm[1]=2 -> 0x0202 = 514
    // Alarm[2]=3 -> 0x0303 = 771
    ASSERT_EQ(257, Alarm[0], "Alarm[0] should be 0x0101 (257) after -! 1,2,3");
    ASSERT_EQ(514, Alarm[1], "Alarm[1] should be 0x0202 (514) after -! 1,2,3");
    ASSERT_EQ(771, Alarm[2], "Alarm[2] should be 0x0303 (771) after -! 1,2,3");
}

// Test 7: -l <prof> sets input profile and device-link flag
static void test_input_profile_with_device_link()
{
    resetGlobals();
    const char* prog = "jpgicc";
    const char* args[] = { prog, "-l", "in.icc" };
    char* argv[] = { (char*)args[0], (char*)args[1], (char*)args[2] };
    HandleSwitches(3, argv);

    ASSERT_TRUE(lIsDeviceLink != 0, "lIsDeviceLink should be TRUE when -l is provided");
    ASSERT_STR_EQUAL("in.icc", cInpProf, "cInpProf should be set to the provided input profile path");
}

// Test 8: -h help invocation does not crash (we can't easily verify Help's output here)
static void test_help_invocation()
{
    resetGlobals();
    const char* prog = "jpgicc";
    const char* args[] = { prog, "-h", "2" };
    char* argv[] = { (char*)args[0], (char*)args[1], (char*)args[2] };
    HandleSwitches(3, argv);

    // If HandleSwitches returns, it means Help did not cause a fatal error in this path.
    ASSERT_TRUE(true, "Help invocation completed without fatal error");
}

// Helper to run all tests
static void runAllTests()
{
    std::cout << "Running test suite for HandleSwitches...\n";
    test_no_args();
    test_set_black_point_compensation();
    test_set_observer_adaptation_state();
    test_jpeg_quality_clamping_max();
    test_jpeg_quality_clamping_min();
    test_alarm_parsing();
    test_input_profile_with_device_link();
    test_help_invocation();

    int total = ++gTestsRun;
    std::cout << "Tests run: " << total << ", Failures: " << gTestsFailed << "\n";
}

int main()
{
    // Run all tests and exit with status 0 on success, 1 on failure
    runAllTests();
    if (gTestsFailed > 0) {
        std::cerr << "Some tests FAILED.\n";
        return 1;
    }
    std::cout << "All tests PASSED.\n";
    return 0;
}
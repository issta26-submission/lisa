// Test suite for the focal method GetFlags() in icctrans.c
// This test suite is designed to be compiled with a C++11 compiler.
// It exercises all meaningful branches of GetFlags() by varying the
// global dependencies PrecalcMode and BlackPointCompensation.
// The tests do not terminate the process on assertion failure and
// report results via standard output.

// Domain/Step notes (embedded as comments):
// - Candidate Keywords: PrecalcMode, BlackPointCompensation, cmsFLAGS_NOOPTIMIZE,
//   cmsFLAGS_HIGHRESPRECALC, cmsFLAGS_LOWRESPRECALC, cmsFLAGS_BLACKPOINTCOMPENSATION,
//   GetFlags, FatalError
// - We access static/global state declared in icctrans.c via extern declarations.
// - We use cmsFLAGS_* macros from lcms2.h for portable bitmask checks.
// - We avoid private members; we only touch external symbols defined in the focal source.
// - We implement a small test harness without GTest (no termination on assertion).
// - All tests are runnable in a single main().

#include <mex.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <lcms2.h>
#include <string.h>


// Import necessary types/macros from the provided library header.
// This is required to compute expected bitmasks portably.

extern "C" {
    // Focal function under test (C linkage)
    cmsUInt32Number GetFlags();

    // Global dependencies presumed to be defined in icctrans.c
    // Use ints for compatibility with the switch in GetFlags().
    extern int PrecalcMode;
    extern int BlackPointCompensation;
}

// Simple non-terminating test harness helpers
// Helper to run a single test and compare actual vs expected flags.
// The test prints outcome and details if mismatch.
static bool RunAndCheck(int mode, int bpc, const std::string& testName)
{
    // Set up the external/global test state.
    PrecalcMode = mode;
    BlackPointCompensation = bpc;

    // Call the focal method.
    cmsUInt32Number actual = GetFlags();

    // Compute the expected flags according to the method's logic.
    cmsUInt32Number expected = 0;
    switch (mode) {
        case 0: expected = cmsFLAGS_NOOPTIMIZE; break;
        case 2: expected = cmsFLAGS_HIGHRESPRECALC; break;
        case 3: expected = cmsFLAGS_LOWRESPRECALC; break;
        case 1: /* falls through to 0 (no flags set in this case) */ break;
        default: /* Unknown precalculation mode -> FatalError called, but we still expect 0 or BPC if set */ break;
    }
    if (bpc) expected |= cmsFLAGS_BLACKPOINTCOMPENSATION;

    bool ok = (actual == expected);
    std::cout << testName << ": " << (ok ? "PASS" : "FAIL")
              << " | mode=" << mode << " bpc=" << bpc
              << " | actual=0x" << std::hex << actual << " expected=0x" << expected
              << std::dec << std::endl;
    if (!ok) {
        std::cerr << "  Detailed: actual=" << actual << ", expected=" << expected
                  << ", mode=" << mode << ", bpc=" << bpc << std::endl;
    }
    return ok;
}

// Individual tests with descriptive comments revealing test intent.

// Test 1: Mode 0 (NOOPTIMIZE) without BlackPointCompensation
// Verifies that in mode 0 we get cmsFLAGS_NOOPTIMIZE and no BPC flag when BPC is false.
static bool Test_Mode0_BPC0()
{
    return RunAndCheck(0, 0, "Test_Mode0_BPC0");
}

// Test 2: Mode 0 (NOOPTIMIZE) with BlackPointCompensation
// Verifies that in mode 0 the BPC flag is OR'ed when BPC is true.
static bool Test_Mode0_BPC1()
{
    return RunAndCheck(0, 1, "Test_Mode0_BPC1");
}

// Test 3: Mode 1 (no change) with BlackPointCompensation = false
// Verifies that mode 1 yields no switch-set flag and no BPC when BPC is false.
static bool Test_Mode1_BPC0()
{
    return RunAndCheck(1, 0, "Test_Mode1_BPC0");
}

// Test 4: Mode 1 with BlackPointCompensation = true
// Verifies that BPC is OR'ed even when mode 1 does not set an explicit flag.
static bool Test_Mode1_BPC1()
{
    return RunAndCheck(1, 1, "Test_Mode1_BPC1");
}

// Test 5: Mode 2 (HIGHRESPRECALC) with BPC = false
// Verifies HIGHRESPRECALC is set without BPC.
static bool Test_Mode2_BPC0()
{
    return RunAndCheck(2, 0, "Test_Mode2_BPC0");
}

// Test 6: Mode 2 with BPC = true
// Verifies HIGHRESPRECALC OR BPC when BPC is true.
static bool Test_Mode2_BPC1()
{
    return RunAndCheck(2, 1, "Test_Mode2_BPC1");
}

// Test 7: Mode 3 (LOWRESPRECALC) with BPC = false
// Verifies LOWRESPRECALC is set without BPC.
static bool Test_Mode3_BPC0()
{
    return RunAndCheck(3, 0, "Test_Mode3_BPC0");
}

// Test 8: Mode 3 with BPC = true
// Verifies LOWRESPRECALC OR BPC when BPC is true.
static bool Test_Mode3_BPC1()
{
    return RunAndCheck(3, 1, "Test_Mode3_BPC1");
}

// Test 9: Default path (Unknown precalculation mode) with BPC = false
// Verifies default branch produces no flag when BPC is false.
static bool Test_Default_BPC0()
{
    return RunAndCheck(99, 0, "Test_Default_BPC0");
}

// Test 10: Default path with BPC = true
// Verifies default branch ORs BPC flag when BPC is true.
static bool Test_Default_BPC1()
{
    return RunAndCheck(99, 1, "Test_Default_BPC1");
}

// Main: executes all tests and reports overall success/failure.
int main()
{
    bool all_passed =
        Test_Mode0_BPC0() &
        Test_Mode0_BPC1() &
        Test_Mode1_BPC0() &
        Test_Mode1_BPC1() &
        Test_Mode2_BPC0() &
        Test_Mode2_BPC1() &
        Test_Mode3_BPC0() &
        Test_Mode3_BPC1() &
        Test_Default_BPC0() &
        Test_Default_BPC1();

    if (all_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
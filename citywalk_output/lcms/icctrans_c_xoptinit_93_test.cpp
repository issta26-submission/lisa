// Test suite for the focal method xoptinit (from icctrans.c)
// Notes:
// - The tests are written in C++11 and do not rely on GTest.
// - The tests assume the following C linkage globals/functions declared in the ICCTRANS context:
//     extern "C" void xoptinit();
//     extern "C" int xoptind;
//     extern "C" int xopterr;
//     extern "C" char* letP;
// - This test focuses on the observed behavior of xoptinit: it should set
//   xoptind to 1, xopterr to 0, and letP to NULL.
// - The test harness uses a lightweight, non-terminating assertion style.

#include <mex.h>
#include <functional>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <lcms2.h>
#include <string.h>


// Bind to the C functions/variables declared in icctrans.c
extern "C" {
    void xoptinit();
    extern int xoptind;
    extern int xopterr;
    extern char* letP;
}

// Lightweight test harness
static int g_totalTests = 0;
static int g_passedTests = 0;

static void runTest(const std::string& name, std::function<bool()> testBody) {
    ++g_totalTests;
    bool ok = testBody();
    if (ok) ++g_passedTests;
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << std::endl;
}

// Test 1: Basic reset behavior
// Purpose: Verify that after calling xoptinit, the global state is reset to
//          xoptind == 1, xopterr == 0, and letP == NULL.
static bool test_xoptinit_basic_reset() {
    // Pre-condition: set to non-default values
    xoptind = 123;
    xopterr = 7;
    letP = reinterpret_cast<char*>(0x1234); // non-NULL placeholder

    // Act
    xoptinit();

    // Assert (non-terminating)
    bool ok = (xoptind == 1) && (xopterr == 0) && (letP == nullptr);
    if (!ok) {
        std::cerr << "  Expected: xoptind=1, xopterr=0, letP=NULL\n"
                  << "  Got:     xoptind=" << xoptind
                  << ", xopterr=" << xopterr
                  << ", letP=" << static_cast<void*>(letP) << "\n";
    }
    return ok;
}

// Test 2: Idempotence / repeated invocation
// Purpose: Ensure repeated calls to xoptinit keep state canonical.
// We expect that calling xoptinit multiple times leaves the state at the canonical defaults.
static bool test_xoptinit_idempotence() {
    // First invocation with arbitrary state
    xoptind = 7;
    xopterr = 2;
    letP = reinterpret_cast<char*>(0xAA55);

    xoptinit();
    bool firstOk = (xoptind == 1) && (xopterr == 0) && (letP == nullptr);

    // Second invocation with another arbitrary state
    xoptind = 9;
    xopterr = 5;
    letP = reinterpret_cast<char*>(0xDEAD);

    xoptinit();
    bool secondOk = (xoptind == 1) && (xopterr == 0) && (letP == nullptr);

    if (!(firstOk && secondOk)) {
        std::cerr << "  Idempotence failure:\n"
                  << "  After 1st call: xoptind=" << xoptind
                  << ", xopterr=" << xopterr
                  << ", letP=" << static_cast<void*>(letP) << "\n"
                  << "  After 2nd call: xoptind=" << xoptind
                  << ", xopterr=" << xopterr
                  << ", letP=" << static_cast<void*>(letP) << "\n";
    }
    return firstOk && secondOk;
}

// Test 3: Stress test with multiple interleaved resets
// Purpose: Ensure that regardless of prior state, xoptinit always resets the state to canonical defaults on each call.
static bool test_xoptinit_stress() {
    // Step 1: set non-default values
    xoptind = 42;
    xopterr = -1;
    letP = reinterpret_cast<char*>(0xBEEF);

    // Step 2: first reset
    xoptinit();
    bool afterFirst = (xoptind == 1) && (xopterr == 0) && (letP == nullptr);

    // Step 3: mutate state again
    xoptind = 0;
    xopterr = 99;
    letP = reinterpret_cast<char*>(0xC0DE);

    // Step 4: second reset
    xoptinit();
    bool afterSecond = (xoptind == 1) && (xopterr == 0) && (letP == nullptr);

    if (!(afterFirst && afterSecond)) {
        std::cerr << "  Stress test failing:\n"
                  << "  After 1st reset: xoptind=" << xoptind
                  << ", xopterr=" << xopterr
                  << ", letP=" << static_cast<void*>(letP) << "\n"
                  << "  After 2nd reset: xoptind=" << xoptind
                  << ", xopterr=" << xopterr
                  << ", letP=" << static_cast<void*>(letP) << "\n";
    }
    return afterFirst && afterSecond;
}

int main() {
    // Run tests
    runTest("xoptinit_basic_reset should set canonical defaults", test_xoptinit_basic_reset);
    runTest("xoptinit_idempotence should keep canonical defaults on repeated calls", test_xoptinit_idempotence);
    runTest("xoptinit_stress should reset state on consecutive calls", test_xoptinit_stress);

    // Summary
    std::cout << "Summary: " << g_passedTests << " / " << g_totalTests << " tests passed." << std::endl;
    return (g_passedTests == g_totalTests) ? 0 : 1;
}
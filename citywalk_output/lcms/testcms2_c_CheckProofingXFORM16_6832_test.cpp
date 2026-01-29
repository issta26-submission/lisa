// Test suite for the focal method: CheckProofingXFORM16
// Language: C++11
// Purpose: Provide a lightweight, non-GTest unit test harness to verify
// the behavior of CheckProofingXFORM16 as exercised by the existing C codebase.
// We avoid private/internal details and use only the public interfaces provided
// by the project and the Little CMS library where applicable.

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <lcms2.h>


// Bring in the Little CMS core types and constants (assumed to be available in the build env)

// The focal function and its helpers live in C code in testcms2.c.
// We declare the relevant C-accessible interfaces here with C linkage to avoid
// name mangling issues when linking from C++.
extern "C" {
    // Factory function to create the "Above RGB" test profile used by the focal method.
    cmsHPROFILE Create_AboveRGB(void);

    // Create a proofing transform with thr (thread-local) context.
    cmsHTRANSFORM cmsCreateProofingTransformTHR(
        cmsContext ContextID,
        cmsHPROFILE InputProfile,
        cmsUInt32Number InputFormat,
        cmsHPROFILE OutputProfile,
        cmsUInt32Number OutputFormat,
        cmsHPROFILE ProofingProfile,
        cmsUInt32Number InputIntent,
        cmsUInt32Number OutputIntent,
        cmsUInt32Number Flags);

    // Utility functions used by the test harness and the focal method:
    void cmsCloseProfile(cmsHPROFILE);
    cmsInt32Number Check16linearXFORM(cmsHTRANSFORM xform, cmsInt32Number nChan);

    // Cleanup
    void cmsDeleteTransform(cmsHTRANSFORM xform);

    // Debug/context helper (signature matches the project; actual type is opaque)
    cmsContext DbgThread(void);
}

// Simple, non-terminating assertion helpers for a lightweight test harness
static int g_failures = 0;
static std::vector<std::string> g_logs;

// Log a passing test case
static void log_pass(const char* test_name) {
    std::string s = std::string("[PASS] ") + test_name;
    g_logs.push_back(s);
    std::cout << s << std::endl;
}

// Log a failing test case with a message
static void log_fail(const char* test_name, const char* message) {
    std::string s = std::string("[FAIL] ") + test_name + ": " + (message ? message : "");
    g_logs.push_back(s);
    std::cerr << s << std::endl;
    ++g_failures;
}

// Convenience macro-like helpers
#define TOSTR2(x) #x
#define TOSTR(x) TOSTR2(x)

// Test 1: Check that CheckProofingXFORM16 returns a non-crashing result and is deterministic (best-effort)
static void test_CheckProofingXFORM16_ReturnsZero() {
    const char* test_name = "CheckProofingXFORM16_ReturnsZero";

    // The focal function is expected to return a non-negative rc; often 0 indicates perfect success in this test suite.
    cmsInt32Number rc = CheckProofingXFORM16();

    if (rc == 0) {
        log_pass(test_name);
    } else {
        // Not asserting exact 0 value due to potential environment differences; still record as failure if non-zero
        log_fail(test_name, "Expected rc == 0, got non-zero value");
    }
}

// Test 2: Determinism test - calling the focal method twice should yield the same rc
static void test_CheckProofingXFORM16_Deterministic() {
    const char* test_name = "CheckProofingXFORM16_Deterministic";

    cmsInt32Number rc1 = CheckProofingXFORM16();
    cmsInt32Number rc2 = CheckProofingXFORM16();

    if (rc1 == rc2) {
        log_pass(test_name);
    } else {
        // If nondeterministic, record a failure but allow continued execution
        log_fail(test_name, "Inconsistent rc between consecutive invocations");
        // Note: We do not abort to maximize test coverage
    }
}

// Test 3: Direct equivalence test with a hand-constructed transform sequence
// Reproduces the steps inside CheckProofingXFORM16 and compares results with the focal method.
static void test_CheckProofingXFORM16_DirectVsFocalEquality() {
    const char* test_name = "CheckProofingXFORM16_DirectVsFocalEquality";

    // Build a transform exactly as in the focal method, then obtain rc
    cmsHPROFILE hAbove = Create_AboveRGB();
    cmsHTRANSFORM xform = cmsCreateProofingTransformTHR(
        DbgThread(),               // context
        hAbove, TYPE_RGB_16,       // input: RGB_16
        hAbove, TYPE_RGB_16,       // output: RGB_16
        hAbove,                     // profile for softproofing
        INTENT_RELATIVE_COLORIMETRIC,
        INTENT_RELATIVE_COLORIMETRIC,
        cmsFLAGS_SOFTPROOFING | cmsFLAGS_NOCACHE
    );
    cmsCloseProfile(hAbove);

    cmsInt32Number rc_direct = Check16linearXFORM(xform, 3);
    cmsDeleteTransform(xform);

    cmsInt32Number rc_focal = CheckProofingXFORM16(); // call the focal method

    if (rc_direct == rc_focal) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "Direct transform rc and focal method rc do not match");
    }
}

// Helper to run the suite and report final status
static int run_all_tests() {
    test_CheckProofingXFORM16_ReturnsZero();
    test_CheckProofingXFORM16_Deterministic();
    test_CheckProofingXFORM16_DirectVsFocalEquality();

    // Summary
    std::cout << "Test Summary: " << (g_failures ? "FAILURES" : "ALL TESTS PASSED") << std::endl;
    for (const auto& s : g_logs) {
        // Already logged individually; this loop ensures all messages are shown in order
        (void)s;
    }
    return g_failures;
}

// Entry point
int main(int argc, char* argv[]) {
    // Basic environment info
    std::cout << "Running CheckProofingXFORM16 unit tests (C++11 harness)" << std::endl;

    // Run test suite
    int failures = run_all_tests();

    // Return non-zero if any tests failed
    return failures != 0 ? 1 : 0;
}
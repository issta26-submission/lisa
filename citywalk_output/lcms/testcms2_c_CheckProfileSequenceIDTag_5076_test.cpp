// Lightweight C++11 unit tests for CheckProfileSequenceIDTag
// This test suite is designed to work with the provided focal method
// CheckProfileSequenceIDTag and its dependencies as declared in testcms2.h/c.
// It avoids GoogleTest and uses a small in-file harness with non-terminating checks.

#include <iostream>
#include <string>
#include <testcms2.h>


// Bring C declarations into C++ world
extern "C" {
    #include "testcms2.h"  // Provides cmsInt32Number, cmsHPROFILE, cmsSigProfileSequenceIdTag, etc.
}

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void report(const std::string& name, bool ok) {
    ++g_tests_run;
    if (ok) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        ++g_tests_failed;
        std::cout << "[FAIL] " << name << "\n";
    }
}

// Test 1: Write then read sequence via CheckProfileSequenceIDTag
// Purpose: Validate the positive path for Pass = 1 (write) followed by Pass = 2 (read)
// Expected: Pass 1 should succeed (return 1) and Pass 2 should succeed (return 1) when using a valid profile.
static bool test_CheckProfileSequenceIDTag_WriteThenRead(void) {
    // Create a test profile handle. The test environment provides Create_AboveRGB() as a stubbed
    // fixture profile; use it to obtain a cmsHPROFILE handle.
    cmsHPROFILE hProfile = Create_AboveRGB(); // May return NULL in stubbed environments; test should handle accordingly.

    // Ensure we have a non-null profile before proceeding
    if (hProfile == NULL) {
        // If the fixture cannot create a profile, treat as a soft skip (not a failure of logic).
        std::cout << "[WARN] Create_AboveRGB() returned NULL. Skipping Write-Read test.\n";
        return true;
    }

    // Case 1: Write a profile sequence description into the profile (Pass = 1)
    cmsInt32Number r1 = CheckProfileSequenceIDTag(1, hProfile);
    if (r1 != 1) {
        report("CheckProfileSequenceIDTag Pass=1 (write) should return 1", false);
        return false;
    }

    // Case 2: Read back the previously written sequence (Pass = 2)
    cmsInt32Number r2 = CheckProfileSequenceIDTag(2, hProfile);
    if (r2 != 1) {
        report("CheckProfileSequenceIDTag Pass=2 (read) should return 1", false);
        return false;
    }

    // Cleanup could be performed here if required by the fixture; rely on underlying test harness.
    report("CheckProfileSequenceIDTag_WriteThenRead", true);
    return true;
}

// Test 2: Null profile handle should fail gracefully (Pass = 1 path)
// Purpose: Ensure function handles invalid input without crashing and returns 0.
static bool test_CheckProfileSequenceIDTag_NullProfile(void) {
    cmsHPROFILE hProfile = nullptr;

    cmsInt32Number res = CheckProfileSequenceIDTag(1, hProfile);
    bool ok = (res == 0);
    report("CheckProfileSequenceIDTag with NULL profile (Pass=1) should return 0", ok);
    return ok;
}

// Test 3: Invalid Pass value should return 0 (default case in switch)
// Purpose: Exercise default path of the switch case inside the function.
static bool test_CheckProfileSequenceIDTag_InvalidPass(void) {
    // Create a valid-ish profile to isolate the Pass value behavior
    cmsHPROFILE hProfile = Create_AboveRGB();
    if (hProfile == NULL) {
        // If fixture fails, still consider as skip
        std::cout << "[WARN] Create_AboveRGB() returned NULL. Skipping InvalidPass test.\n";
        return true;
    }

    cmsInt32Number res = CheckProfileSequenceIDTag(99, hProfile); // undefined Pass for switch
    bool ok = (res == 0);
    report("CheckProfileSequenceIDTag Invalid Pass (Pass=99) should return 0", ok);
    return ok;
}

// Test 4: Ensure that non-write/read flow does not crash when used repeatedly in a single run
// This test invokes Pass 1 twice to simulate repeated usage on the same profile.
static bool test_CheckProfileSequenceIDTag_RepeatedWrite(void) {
    cmsHPROFILE hProfile = Create_AboveRGB();
    if (hProfile == NULL) {
        std::cout << "[WARN] Create_AboveRGB() returned NULL. Skipping RepeatedWrite test.\n";
        return true;
    }

    cmsInt32Number a = CheckProfileSequenceIDTag(1, hProfile);
    bool ok = (a == 1);
    if (!ok) {
        report("CheckProfileSequenceIDTag RepeatedWrite: first write failed", false);
        return false;
    }

    cmsInt32Number b = CheckProfileSequenceIDTag(1, hProfile);
    ok = ok && (b == 1);
    if (!ok) {
        report("CheckProfileSequenceIDTag RepeatedWrite: second write failed", false);
        return false;
    }

    report("CheckProfileSequenceIDTag_RepeatedWrite", true);
    return true;
}

// Entry point
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Unused parameters in this minimal harness

    // Run tests
    test_CheckProfileSequenceIDTag_WriteThenRead();
    test_CheckProfileSequenceIDTag_NullProfile();
    test_CheckProfileSequenceIDTag_InvalidPass();
    test_CheckProfileSequenceIDTag_RepeatedWrite();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";
    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
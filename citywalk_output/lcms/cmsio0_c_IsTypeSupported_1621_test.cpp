// C++11 unit test suite for IsTypeSupported in cmsio0.c
// This test focuses on cmsBool IsTypeSupported(cmsTagDescriptor* TagDescriptor, cmsTagTypeSignature Type)
// It uses the public definitions from the project's headers and links against the object
// that implements IsTypeSupported (cmsio0.c should be compiled and linked with this test).

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdint>


// Import project dependencies to obtain type definitions.
// We rely on the project's public CMS headers being available in the include path.

// Declare the focal function with C linkage for the C++ test linker to resolve.
extern "C" cmsBool IsTypeSupported(cmsTagDescriptor* TagDescriptor, cmsTagTypeSignature Type);

// Lightweight test harness helpers (non-terminating style). We'll report failures but continue execution.
static void logFail(const char* testName, const char* message) {
    std::cerr << "[FAIL] " << testName << " - " << message << std::endl;
}
static void logPass(const char* testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

// Test 1: When nSupportedTypes == 0, no types are supported -> Should return FALSE for any Type.
static bool Test_IsTypeSupported_NoTypes() {
    const char* testName = "IsTypeSupported_NoTypes";

    cmsTagDescriptor desc;
    // Initialize only fields used by IsTypeSupported
    desc.nSupportedTypes = 0;

    cmsTagTypeSignature anyType = (cmsTagTypeSignature)0x1234ABCD;
    cmsBool result = IsTypeSupported(&desc, anyType);
    bool ok = (result == FALSE);
    if (!ok) {
        logFail(testName, "Expected FALSE when nSupportedTypes is 0, but got TRUE.");
    } else {
        logPass(testName);
    }
    return ok;
}

// Test 2: When there is exactly one supported type, and it matches Type -> Should return TRUE.
static bool Test_IsTypeSupported_SingleMatch() {
    const char* testName = "IsTypeSupported_SingleMatch";

    cmsTagDescriptor desc;
    desc.nSupportedTypes = 1;
    desc.SupportedTypes[0] = (cmsTagTypeSignature)0xABCD1234;

    cmsTagTypeSignature Type = (cmsTagTypeSignature)0xABCD1234;
    cmsBool result = IsTypeSupported(&desc, Type);
    bool ok = (result == TRUE);
    if (!ok) {
        logFail(testName, "Expected TRUE when Type matches the sole supported type.");
    } else {
        logPass(testName);
    }
    return ok;
}

// Test 3: When nSupportedTypes equals MAX_TYPES_IN_LCMS_PLUGIN and Type is the last allowed entry -> Should return TRUE (cap logic exercised).
static bool Test_IsTypeSupported_MaxCap_AllPresent() {
    const char* testName = "IsTypeSupported_MaxCap_AllPresent";

    cmsTagDescriptor desc;
    // Use the maximum types supported by the plugin as defined by the header
    desc.nSupportedTypes = MAX_TYPES_IN_LCMS_PLUGIN;

    // Fill the SupportedTypes with unique values: 1..MAX
    for (unsigned i = 0; i < MAX_TYPES_IN_LCMS_PLUGIN; ++i) {
        desc.SupportedTypes[i] = (cmsTagTypeSignature)(i + 1);
    }

    cmsTagTypeSignature Type = (cmsTagTypeSignature)MAX_TYPES_IN_LCMS_PLUGIN; // last value
    cmsBool result = IsTypeSupported(&desc, Type);
    bool ok = (result == TRUE);
    if (!ok) {
        logFail(testName, "Expected TRUE when Type equals last supported type within cap.");
    } else {
        logPass(testName);
    }
    return ok;
}

// Test 4: When nSupportedTypes > MAX_TYPES_IN_LCMS_PLUGIN, cap is applied; verify that a valid Type within first MAX is found.
static bool Test_IsTypeSupported_CapWithOverflow() {
    const char* testName = "IsTypeSupported_CapWithOverflow";

    cmsTagDescriptor desc;
    // overflowed nSupportedTypes
    desc.nSupportedTypes = MAX_TYPES_IN_LCMS_PLUGIN + 5;

    // Populate the first MAX entries with values 1..MAX
    for (unsigned i = 0; i < MAX_TYPES_IN_LCMS_PLUGIN; ++i) {
        desc.SupportedTypes[i] = (cmsTagTypeSignature)(i + 1);
    }

    // Choose a Type that exists within the first MAX entries
    cmsTagTypeSignature Type = (cmsTagTypeSignature)MAX_TYPES_IN_LCMS_PLUGIN; // equals last filled entry
    cmsBool result = IsTypeSupported(&desc, Type);
    bool ok = (result == TRUE);
    if (!ok) {
        logFail(testName, "Expected TRUE when overflowed count but Type is within first MAX entries.");
    } else {
        logPass(testName);
    }
    return ok;
}

// Test 5: Type not present within the first MAX entries -> Should return FALSE, even if a large nSupportedTypes exists.
static bool Test_IsTypeSupported_NotPresent() {
    const char* testName = "IsTypeSupported_NotPresent";

    cmsTagDescriptor desc;
    desc.nSupportedTypes = MAX_TYPES_IN_LCMS_PLUGIN;
    for (unsigned i = 0; i < MAX_TYPES_IN_LCMS_PLUGIN; ++i) {
        // Fill with values that do not include 0xDEAD_BEEF
        desc.SupportedTypes[i] = (cmsTagTypeSignature)(i + 1);
    }

    cmsTagTypeSignature Type = (cmsTagTypeSignature)0xDEAD_BEEF;
    cmsBool result = IsTypeSupported(&desc, Type);
    bool ok = (result == FALSE);
    if (!ok) {
        logFail(testName, "Expected FALSE when Type is not among the supported types.");
    } else {
        logPass(testName);
    }
    return ok;
}

// Main runner
int main() {
    bool all_ok = true;

    all_ok &= Test_IsTypeSupported_NoTypes();
    all_ok &= Test_IsTypeSupported_SingleMatch();
    all_ok &= Test_IsTypeSupported_MaxCap_AllPresent();
    all_ok &= Test_IsTypeSupported_CapWithOverflow();
    all_ok &= Test_IsTypeSupported_NotPresent();

    // Summary
    if (all_ok) {
        std::cout << "[SUMMARY] All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY] Some tests FAILED." << std::endl;
        return 1;
    }
}
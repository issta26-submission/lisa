/*
Unit test suite for the focal method:
    void* Type_Signature_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n)

Key idea:
- Create a minimal _cms_typehandler_struct with a ContextID.
- Prepare an input array of cmsSignature values (representing ICC tags).
- Call Type_Signature_Dup and verify that the returned memory contains an exact copy of the input data.
- Use cmsFree to release the duplicated memory.
- Provide multiple test cases to exercise normal operation, zero-length input, and independence (dup should not alias the original memory).

Notes:
- The tests are written in plain C++11 without Google Test (as requested).
- Non-terminating assertions are used: tests report failures but continue execution.
- Include paths assume lcms2 and internal headers are available in the build environment.
*/

#include <cstring>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Public API headers (adjust include paths as needed by your build system)

// Prototypes with C linkage to avoid name mangling when including from C++
extern "C" void* Type_Signature_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Simple non-terminating assertion helpers
static int g_testFailures = 0;
static void fail(const char* testName, const char* message) {
    std::cerr << "[FAIL] " << testName << ": " << message << std::endl;
    ++g_testFailures;
}
static void pass(const char* testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

/*
Test 1: Basic duplication
- Prepare a small array of cmsSignature values.
- Duplicate and verify content byte-for-byte.
*/
bool test_Type_Signature_Dup_basic() {
    const char* testName = "test_Type_Signature_Dup_basic";

    // Set up a minimal handler with a ContextID
    struct _cms_typehandler_struct handler;
    handler.ContextID = 0; // default context

    // Prepare test data
    const cmsSignature input[4] = {
        0x61626364, // 'abcd'
        0x31323334, // '1234'
        0x41424344, // 'ABCD'
        0x55555555  // 'UUUU'
    };
    cmsUInt32Number n = 4;

    void* dup = Type_Signature_Dup(&handler, input, n);
    if (dup == nullptr) {
        fail(testName, "Dup returned null for non-zero input.");
        return false;
    }

    cmsSignature* out = static_cast<cmsSignature*>(dup);
    bool ok = true;
    for (cmsUInt32Number i = 0; i < n; ++i) {
        if (out[i] != input[i]) {
            ok = false;
            break;
        }
    }

    cmsFree(dup);

    if (!ok) {
        fail(testName, "Duplicated memory content does not match input.");
        return false;
    }

    pass(testName);
    return true;
}

/*
Test 2: Zero-length duplication
- n = 0 should result in a non-null pointer (or a valid allocation of zero size).
- Ensure no crash and memory is safely freed if non-null.
*/
bool test_Type_Signature_Dup_zero_length() {
    const char* testName = "test_Type_Signature_Dup_zero_length";

    struct _cms_typehandler_struct handler;
    handler.ContextID = 0;

    const cmsSignature input[0] = {};
    cmsUInt32Number n = 0;

    void* dup = Type_Signature_Dup(&handler, input, n);
    if (dup == nullptr) {
        // If library returns null for zero-length dup, that's also acceptable as a valid path.
        // We'll treat null as a non-fatal condition here but log the outcome.
        pass(testName);
        return true;
    }

    // If non-null, we expect a zero-sized allocation; still must be safe to free.
    cmsFree(dup);
    pass(testName);
    return true;
}

/*
Test 3: Independence of duplicated memory
- Ensure that modifying the original input after the duplication does not affect the duplicated memory.
*/
bool test_Type_Signature_Dup_independence() {
    const char* testName = "test_Type_Signature_Dup_independence";

    struct _cms_typehandler_struct handler;
    handler.ContextID = 0;

    cmsSignature input[3] = { 0x11112222, 0x33334444, 0x55556666 };
    cmsUInt32Number n = 3;

    void* dup = Type_Signature_Dup(&handler, input, n);
    if (dup == nullptr) {
        fail(testName, "Dup returned null for non-zero input.");
        return false;
    }

    cmsSignature* out = static_cast<cmsSignature*>(dup);

    // Mutate the original after duplication
    input[0] = 0xAAAAAAAA;
    input[1] = 0xBBBBBBBB;
    input[2] = 0xCCCCCCCC;

    bool stillEqual = true;
    for (cmsUInt32Number i = 0; i < n; ++i) {
        if (out[i] != (cmsSignature) ( (i==0)?0x11112222 : (i==1)?0x33334444 : 0x55556666 )) {
            stillEqual = false;
            break;
        }
    }

    cmsFree(dup);

    if (!stillEqual) {
        fail(testName, "Dup memory reflects changes to the source input (not independent).");
        return false;
    }

    pass(testName);
    return true;
}

/*
Test 4: Large n duplication
- Validate behavior for larger arrays to exercise memory copying across more elements.
*/
bool test_Type_Signature_Dup_largeN() {
    const char* testName = "test_Type_Signature_Dup_largeN";

    struct _cms_typehandler_struct handler;
    handler.ContextID = 0;

    const cmsSignature input[16] = {
        0x00010001, 0x00020002, 0x00030003, 0x00040004,
        0x00050005, 0x00060006, 0x00070007, 0x00080008,
        0x00090009, 0x000A000A, 0x000B000B, 0x000C000C,
        0x000D000D, 0x000E000E, 0x000F000F, 0x00100010
    };
    cmsUInt32Number n = 16;

    void* dup = Type_Signature_Dup(&handler, input, n);
    if (dup == nullptr) {
        fail(testName, "Dup returned null for large non-zero input.");
        return false;
    }

    cmsSignature* out = static_cast<cmsSignature*>(dup);
    bool ok = true;
    for (cmsUInt32Number i = 0; i < n; ++i) {
        if (out[i] != input[i]) {
            ok = false;
            break;
        }
    }

    cmsFree(dup);

    if (!ok) {
        fail(testName, "Duplicated memory content mismatch for large input.");
        return false;
    }

    pass(testName);
    return true;
}

int main() {
    // Run all tests; do not terminate on first failure to maximize coverage.
    bool results[4] = { false, false, false, false };

    results[0] = test_Type_Signature_Dup_basic();
    results[1] = test_Type_Signature_Dup_zero_length();
    results[2] = test_Type_Signature_Dup_independence();
    results[3] = test_Type_Signature_Dup_largeN();

    // Summary
    int total = 4;
    int passed = 0;
    for (int i = 0; i < total; ++i) if (results[i]) ++passed;

    std::cout << "Test Summary: " << passed << " / " << total << " tests passed." << std::endl;

    if (g_testFailures > 0) {
        std::cerr << g_testFailures << " failures detected." << std::endl;
    }

    // Return non-zero if any test failed to aid integration with build systems.
    return (g_testFailures == 0) ? 0 : 1;
}
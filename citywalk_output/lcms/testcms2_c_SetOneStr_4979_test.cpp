// C++11 test suite for the focal method SetOneStr in testcms2.c
// - This test suite relies on the provided test harness declarations (testcms2.h).
// - It performs non-terminating checks by returning success/failure per test and reports in main.
// - The tests focus on ensuring SetOneStr allocates a cmsMLU object and handles basic/unicode/large inputs.

#include <string>
#include <cstdio>
#include <testcms2.h>
#include <cwchar>


// Bring in the test harness declarations. The test harness defines cmsMLU, cmsBool, and
// the CMS utility functions used by SetOneStr. Wrapping in extern "C" to avoid name mangling.
extern "C" {
}

// Declaration of the focal method from testcms2.c so we can call it from C++ tests.
// The function signature matches the one in the source file.
extern "C" void SetOneStr(cmsMLU** mlu, const wchar_t* s1, const wchar_t* s2);

// Forward declaration of cmsMLUfree to ensure proper cleanup (provided by the harness/LCMS).
// If the function has a slightly different linkage in your environment, rely on the prototype
// already declared in testcms2.h via the extern "C" block above.
extern "C" void cmsMLUfree(cmsMLU* mlu);

// Test 1: Basic allocation check
// Purpose: Ensure SetOneStr allocates a non-null cmsMLU object and does not crash for normal inputs.
// Outline: Call SetOneStr with simple strings and verify mlu is non-null. Cleanup allocated memory.
static bool test_SetOneStr_basic_allocation() {
    cmsMLU* mlu = nullptr;

    // Use simple ASCII wide strings; verify basic memory allocation path
    SetOneStr(&mlu, L"Hello", L"World");
    if (mlu == nullptr) {
        std::printf("test_SetOneStr_basic_allocation: FAILED (nullptr mlu)\n");
        return false;
    }

    // Cleanup
    cmsMLUfree(mlu);
    return true;
}

// Test 2: Distinct allocations across multiple calls
// Purpose: Verify that multiple invocations allocate distinct cmsMLU objects (no reuse of pointer).
// Outline: Allocate two separate MLU objects and ensure their pointers differ. Cleanup both.
static bool test_SetOneStr_reallocates_distinct_mlU() {
    cmsMLU* a = nullptr;
    cmsMLU* b = nullptr;

    SetOneStr(&a, L"A1", L"B1");
    SetOneStr(&b, L"A2", L"B2");

    if (a == nullptr || b == nullptr) {
        std::printf("test_SetOneStr_reallocates_distinct_mlU: FAILED (null mlu)\n");
        if (a) cmsMLUfree(a);
        if (b) cmsMLUfree(b);
        return false;
    }

    bool diff = (a != b);

    cmsMLUfree(a);
    cmsMLUfree(b);

    if (!diff) {
        std::printf("test_SetOneStr_reallocates_distinct_mlU: FAILED (identical pointers)\n");
        return false;
    }
    return true;
}

// Test 3: Unicode handling
// Purpose: Ensure SetOneStr can handle Unicode/wide-character inputs without crashing.
// Outline: Use Unicode strings for s1 and s2; verify that no crash occurs and a valid mlu is allocated.
static bool test_SetOneStr_handles_unicode_strings() {
    cmsMLU* mlu = nullptr;

    // Unicode strings including non-ASCII characters
    SetOneStr(&mlu, L"こんにちは", L"Hola");
    if (mlu == nullptr) {
        std::printf("test_SetOneStr_handles_unicode_strings: FAILED (nullptr mlu)\n");
        return false;
    }

    cmsMLUfree(mlu);
    return true;
}

// Test 4: Handling of large strings
// Purpose: Validate behavior when very large inputs are provided to s1 and s2.
// Outline: Pass long wide strings and ensure allocation succeeds without crash.
static bool test_SetOneStr_with_large_strings() {
    cmsMLU* mlu = nullptr;

    // Create large wide strings (1000 characters each)
    std::wstring long1(1000, L'A');
    std::wstring long2(1000, L'B');

    SetOneStr(&mlu, long1.c_str(), long2.c_str());
    if (mlu == nullptr) {
        std::printf("test_SetOneStr_with_large_strings: FAILED (nullptr mlu)\n");
        return false;
    }

    cmsMLUfree(mlu);
    return true;
}

// Simple test runner macros (not using any external testing framework)
#define RUN_TEST(name) do { \
    bool ok = name(); \
    if (ok) { std::printf("[PASS] %s\n", #name); } \
    else { std::printf("[FAIL] %s\n", #name); } \
} while (0)

int main(int argc, char* argv[]) {
    std::printf("Running SetOneStr unit tests (C++17-compatible, C linkage needed)...\n");

    // Execute tests
    RUN_TEST(test_SetOneStr_basic_allocation);
    RUN_TEST(test_SetOneStr_reallocates_distinct_mlU);
    RUN_TEST(test_SetOneStr_handles_unicode_strings);
    RUN_TEST(test_SetOneStr_with_large_strings);

    // Optional: report overall status
    // Note: The individual tests print PASS/FAIL; the main function could aggregate results if needed.
    std::printf("SetOneStr test suite finished.\n");
    return 0;
}
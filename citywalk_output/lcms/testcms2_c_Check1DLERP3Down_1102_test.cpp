/*
Unit test suite for the focal method:
- Focal method under test: cmsInt32Number Check1DLERP3Down(void)
- Implementation insight: It returns the result of Check1D(3, TRUE, 1)
- Test goals:
  - Validate that Check1DLERP3Down delegates to Check1D with arguments (3, TRUE, 1)
  - Ensure consistent behavior across multiple invocations
  - Maintain non-terminating assertions (test continues after a failure and reports at the end)
- Notes:
  - Since the project is C-based, we expose the necessary C APIs to C++ via extern "C".
  - We provide minimal typedefs if needed to compile standalone, but rely on the actual signatures for real linking.
  - This test suite does not rely on GoogleTest; it uses a small in-house test harness.
*/

#include <iostream>
#include <testcms2.h>


// Minimal type aliases to allow compilation in absence of the header.
// If the actual project defines these in a header, the real types will align with them at link time.
typedef int cmsInt32Number;
typedef int cmsBool;

// Ensure C linkage for the FOCAL_METHOD and its dependencies when linked with C code.
extern "C" {
    // Prototypes for the focal method and its dependency used in test
    cmsInt32Number Check1D(cmsInt32Number nNodesToCheck, cmsBool Down, cmsInt32Number max_err);
    cmsInt32Number Check1DLERP3Down(void);
}

// Simple non-terminating assertion helper
static int g_failures = 0;
#define CHECK(cond, message) do { if(!(cond)) { std::cerr << "FAIL: " << message << " (Line " << __LINE__ << ")" << std::endl; ++g_failures; } } while(0)

// Test 1: Check that Check1DLERP3Down delegates to Check1D(3, TRUE, 1)
void test_Check1DLERP3Down_Equals_DirectCheck1D()
{
    // Call the focal function
    cmsInt32Number res_from_focal = Check1DLERP3Down();

    // Direct call to Check1D with the same semantics (3, TRUE, 1)
    cmsInt32Number res_direct = Check1D(3, 1, 1);

    CHECK(res_from_focal == res_direct,
          "Check1DLERP3Down should return the same value as Check1D(3, TRUE, 1)");
}

// Test 2: Repeated invocations should be consistent (non-changing delegation behavior)
void test_Check1DLERP3Down_MultipleInvocationsConsistency()
{
    for (int i = 0; i < 5; ++i) {
        cmsInt32Number a = Check1DLERP3Down();
        cmsInt32Number b = Check1D(3, 1, 1);
        CHECK(a == b,
              "Repeated invocation of Check1DLERP3Down must match Check1D(3, TRUE, 1) on iteration " << i);
    }
}

// Entry point for the test suite: executes all tests and reports summary
int main()
{
    std::cout << "Running unit tests for Check1DLERP3Down (C++ test harness, no GTest)" << std::endl;

    test_Check1DLERP3Down_Equals_DirectCheck1D();
    test_Check1DLERP3Down_MultipleInvocationsConsistency();

    if (g_failures != 0) {
        std::cerr << "Total failures: " << g_failures << std::endl;
        return 1; // Non-zero on failure
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
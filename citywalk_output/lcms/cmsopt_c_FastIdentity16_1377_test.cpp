/*
  Lightweight C++11 unit test suite for the focal function:
  FastIdentity16(In, Out, D)

  Notes:
  - The tests are designed to be self-contained and do not rely on GTest.
  - We declare a minimal compatible signature for FastIdentity16 and a minimal
    cmsPipeline structure, focusing only on the member InputChannels that the
    function uses. This keeps the tests robust against the actual large
    cmsPipeline layout in the real library, by ensuring the first member is used.
  - Tests are invoked from main(), per the provided domain knowledge guidelines.
  - Each test includes explanatory comments describing its purpose and coverage.
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Macro guard to avoid missing macro definitions if included from the real library header.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Typedefs matching the expected CMS type aliases (sufficient for test harness).
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Declaration of the focal function under test.
// Using a C-style linkage to match the actual implementation in cmsopt.c.
extern "C" void FastIdentity16(CMSREGISTER const cmsUInt16Number In[],
                               CMSREGISTER cmsUInt16Number Out[],
                               CMSREGISTER const void* D);

// Minimal stub of the cmsPipeline structure sufficient for tests.
// We place InputChannels as the first member to align with how the focal
// function accesses Lut->InputChannels.
struct cmsPipeline {
    cmsUInt32Number InputChannels;
    // Real cmsPipeline has more fields, but the tests only rely on InputChannels.
};

// Lightweight test harness state
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Simple assertion helper that reports failure but does not abort tests
#define TEST_ASSERT(cond, msg)                                      \
    do {                                                              \
        if (!(cond)) {                                              \
            std::cerr << "[TEST FAILED] " << (msg)                 \
                      << " (line " << __LINE__ << ")\n";           \
            ++g_tests_failed;                                       \
        } else {                                                    \
            std::cout << "[TEST PASSED] " << (msg)                 \
                      << " (line " << __LINE__ << ")\n";           \
        }                                                           \
        ++g_tests_run;                                              \
    } while (0)

// Test 1: Basic copy behavior with a non-zero positive channel count.
// Verifies that FastIdentity16 copies all In[i] to Out[i] for i in [0, InputChannels).
void Test_BasicCopy() {
    // Setup: Input with 4 channels
    cmsPipeline lut;
    lut.InputChannels = 4;

    uint16_t In[4]  = { 10, 20, 30, 40 };
    uint16_t Out[4] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF }; // initial dummy data

    // Act
    FastIdentity16(In, Out, &lut);

    // Assert: Out must equal In for first 4 elements
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        if (Out[i] != In[i]) {
            ok = false;
            break;
        }
    }
    TEST_ASSERT(ok, "BasicCopy: Out equals In for all InputChannels (4)");
}

// Test 2: Zero-input channels should not modify the output array.
void Test_ZeroChannelsNoWrite() {
    // Setup: InputChannels = 0
    cmsPipeline lut;
    lut.InputChannels = 0;

    uint16_t In[1]  = { 12345 }; // value should be ignored
    uint16_t Out[1] = { 0xAA55 }; // initial sentinel

    // Act
    FastIdentity16(In, Out, &lut);

    // Assert: Out should remain unchanged
    bool ok = (Out[0] == 0xAA55);
    TEST_ASSERT(ok, "ZeroChannelsNoWrite: Out remains unchanged when InputChannels is 0");
}

// Test 3: Partial copy with extra tail elements existing in Out.
// Ensures that only the first InputChannels elements are written.
void Test_PartialCopy_NoOverflowBeyondChannels() {
    // Setup: InputChannels = 2
    cmsPipeline lut;
    lut.InputChannels = 2;

    uint16_t In[4]  = { 111, 222, 333, 444 };
    uint16_t Out[4] = { 9, 9, 9, 9 }; // pre-fill with distinct values

    // Act
    FastIdentity16(In, Out, &lut);

    // Assert: Out[0..1] should be In[0..1], Out[2..3] unchanged
    bool ok = true;
    if (Out[0] != In[0] || Out[1] != In[1]) ok = false;
    if (Out[2] != 9 || Out[3] != 9) ok = false;

    TEST_ASSERT(ok, "PartialCopyNoOverflow: Only first 2 outputs updated; others unchanged");
}

// Test 4: Large channel count copy to verify scalability and correctness up to 8 channels.
// Verifies that multiple consecutive elements are copied correctly.
void Test_LargeChannelCopy() {
    // Setup: InputChannels = 8
    cmsPipeline lut;
    lut.InputChannels = 8;

    uint16_t In[8]  = { 1,2,3,4,5,6,7,8 };
    uint16_t Out[8] = { 0 };

    // Act
    FastIdentity16(In, Out, &lut);

    // Assert: All 8 elements copied
    bool ok = true;
    for (int i = 0; i < 8; ++i) {
        if (Out[i] != In[i]) {
            ok = false;
            break;
        }
    }
    TEST_ASSERT(ok, "LargeChannelCopy: Out equals In for 8 channels");
}

// Entry point to run all tests
int main() {
    std::cout << "Starting FastIdentity16 unit tests (C++11, no GTest)..." << std::endl;

    Test_BasicCopy();
    Test_ZeroChannelsNoWrite();
    Test_PartialCopy_NoOverflowBeyondChannels();
    Test_LargeChannelCopy();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failed: " << g_tests_failed << std::endl;
    if (g_tests_failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
}
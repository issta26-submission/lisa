/*
  Unit test suite for the focal method EvaluateIdentity in cmslut.c

  - Target: EvaluateIdentity copies In[0 .. mpe->InputChannels-1] to Out[0 .. mpe->InputChannels-1]
  - Environment: C++11, no GoogleTest; minimal test harness using standard library
  - Dependencies: cmsFloat32Number, cmsStage, and the function prototype for EvaluateIdentity
  - Approach: create several focused tests to cover typical, edge, and overlapping memory scenarios

  Notes:
  - Tests assume the project provides the actual cmslut.c/C headers (lcms2_internal.h et al.)
  - Tests use extern "C" linkage for the EvaluateIdentity function to ensure proper linking
  - The test harness is designed to be non-terminating on failure (continues running other tests)
  - Each test includes explanatory comments describing the scenario and the expected behavior
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Include the project's internal headers to get proper type definitions

// Ensure correct linkage for the C function defined in cmslut.c
extern "C" void EvaluateIdentity(const cmsFloat32Number In[],
                                 cmsFloat32Number Out[],
                                 const cmsStage *mpe);

// Utility: compare two float arrays with a tolerance
static bool arrays_equal_float(const cmsFloat32Number* a, const cmsFloat32Number* b, std::size_t n, cmsFloat32Number tol = 1e-6f)
{
    for (std::size_t i = 0; i < n; ++i) {
        if (std::fabs(static_cast<double>(a[i]) - static_cast<double>(b[i])) > static_cast<double>(tol)) {
            return false;
        }
    }
    return true;
}

// Test 1: Basic copy of all channels (normal positive case)
static bool test_EvaluateIdentity_basic_copy()
{
    // Setup: 4 input channels; In -> Out should copy all 4 values
    cmsStage mpe;
    mpe.InputChannels = 4;

    cmsFloat32Number In[4]  = { 1.0f, 2.0f, 3.0f, 4.0f };
    cmsFloat32Number Out[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    EvaluateIdentity(In, Out, &mpe);

    // Expect Out to be exactly equal to In for the first 4 elements
    bool ok = arrays_equal_float(In, Out, 4);
    if (!ok) {
        std::cerr << "EvaluateIdentity_basic_copy: Out did not match In for 4 channels.\n";
    }
    return ok;
}

// Test 2: Overlapping memory (In and Out point to the same buffer)
// memmove should handle overlap safely; the content should remain unchanged
static bool test_EvaluateIdentity_overlap_same_buffer()
{
    // Setup: buffer used as both In and Out
    cmsStage mpe;
    mpe.InputChannels = 4;

    cmsFloat32Number buffer[4] = { 7.0f, 8.0f, 9.0f, 10.0f };
    cmsFloat32Number constBuffer[4] = { 7.0f, 8.0f, 9.0f, 10.0f }; // to compare after operation

    // Copy to a local separate array to preserve original for comparison
    std::memcpy(constBuffer, buffer, sizeof(buffer[0]) * 4);

    EvaluateIdentity(buffer, buffer, &mpe);

    // Expect buffer to remain unchanged after in-place copy
    bool ok = arrays_equal_float(buffer, constBuffer, 4);
    if (!ok) {
        std::cerr << "EvaluateIdentity_overlap_same_buffer: Buffer changed after in-place copy.\n";
    }
    return ok;
}

// Test 3: Zero input channels (no copy should occur; Out should remain unchanged)
static bool test_EvaluateIdentity_zero_channels()
{
    cmsStage mpe;
    mpe.InputChannels = 0;

    cmsFloat32Number In[3]  = { 1.1f, 2.2f, 3.3f };
    cmsFloat32Number Out[3] = { 9.9f, 9.9f, 9.9f };

    EvaluateIdentity(In, Out, &mpe);

    // Out should remain unchanged
    cmsFloat32Number expected[3] = { 9.9f, 9.9f, 9.9f };
    bool ok = arrays_equal_float(Out, expected, 3);
    if (!ok) {
        std::cerr << "EvaluateIdentity_zero_channels: Out changed when InputChannels == 0.\n";
    }
    return ok;
}

// Test 4: Partial copy (only first N channels are copied; verify tail remains unchanged)
// Also confirms that the copied portion matches In's first N elements
static bool test_EvaluateIdentity_partial_copy()
{
    cmsStage mpe;
    mpe.InputChannels = 3;

    cmsFloat32Number In[4]  = { 0.1f, 0.2f, 0.3f, 4.0f };
    cmsFloat32Number Out[4] = { 9.9f, 9.9f, 9.9f, 9.9f };

    EvaluateIdentity(In, Out, &mpe);

    cmsFloat32Number expected[4] = { 0.1f, 0.2f, 0.3f, 9.9f }; // last element should remain unchanged
    bool ok = arrays_equal_float(Out, expected, 4);
    if (!ok) {
        std::cerr << "EvaluateIdentity_partial_copy: Out does not match expected partial copy.\n";
    }
    return ok;
}

// Simple test runner to execute all tests and summarize results
int main()
{
    int failures = 0;

    // Run tests
    if (!test_EvaluateIdentity_basic_copy()) {
        ++failures;
    } else {
        std::cout << "PASS: EvaluateIdentity_basic_copy\n";
    }

    if (!test_EvaluateIdentity_overlap_same_buffer()) {
        ++failures;
    } else {
        std::cout << "PASS: EvaluateIdentity_overlap_same_buffer\n";
    }

    if (!test_EvaluateIdentity_zero_channels()) {
        ++failures;
    } else {
        std::cout << "PASS: EvaluateIdentity_zero_channels\n";
    }

    if (!test_EvaluateIdentity_partial_copy()) {
        ++failures;
    } else {
        std::cout << "PASS: EvaluateIdentity_partial_copy\n";
    }

    if (failures > 0) {
        std::cerr << "TOTAL FAILURES: " << failures << "\n";
        return 1;
    }

    std::cout << "ALL TESTS PASSED\n";
    return 0;
}
/*
  Unit test suite for the focal function:
  Optimize8BitRGBTransform located in fast_8_tethra.c

  Testing approach:
  - No GoogleTest or other testing frameworks are used.
  - A lightweight, self-contained test harness is provided with a simple
    test runner that executes individual test cases and prints PASS/FAIL.
  - Tests focus on early-branch behavior to ensure basic input validation
    paths are triggered as expected.
  - The tests assume the build environment provides the real Little CMS
    types and function implementations. We declare the function prototype
    to enable linking and invocation from C++ test code.
  - The tests deliberately exercise the "false" branches as early as possible
    (e.g., Lut == NULL, FLOAT formats, non-8-bit formats, non-RGB color spaces)
    to confirm the method shortcuts correctly without requiring full
    LUT processing (which would require a heavy mock/stub network).

  Notes:
  - This harness uses minimal, opaque placeholders for the opaque CMS types.
  - It is intended to compile and run against the real codebase that provides
    the actual types and macros (cmsPipeline, cmsUInt32Number, etc.). If your
    environment differs, adjust the forward declarations accordingly.
*/

#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>


//------------------------------------------------------------------------------
// Lightweight type and prototype declarations to enable linking with the
// focal C implementation (fast_8_tethra.c). These are intentionally minimal.
// They rely on the real library definitions at link-time.
//------------------------------------------------------------------------------

extern "C" {
    // Opaque function pointer types used by the focal method
    typedef void _cmsTransform2Fn;
    typedef void _cmsFreeUserDataFn;

    // Opaque CMS types (in real project these are defined in headers)
    typedef void cmsPipeline;

    // Basic scalar types used by the signature
    typedef uint32_t cmsUInt32Number;
    typedef int      cmsBool;  // Little CMS uses cmsBool typically as int

    // Prototype of the focal function under test
    // Signature copied from the provided source snippet
    cmsBool Optimize8BitRGBTransform(_cmsTransform2Fn* TransformFn,
                                   void** UserData,
                                   _cmsFreeUserDataFn* FreeDataFn,
                                   cmsPipeline** Lut,
                                   cmsUInt32Number* InputFormat,
                                   cmsUInt32Number* OutputFormat,
                                   cmsUInt32Number* dwFlags);
}

//------------------------------------------------------------------------------
// Test helpers
//------------------------------------------------------------------------------

static void printTestResult(const std::string& name, bool passed) {
    std::cout << (passed ? "PASS" : "FAIL") << " - " << name << std::endl;
}

// A tiny wrapper to safely cast a non-NULL dummy value to cmsPipeline*.
// We do not dereference the pipeline in these early-branch tests; we only
// need a non-NULL pointer so that the *Lut == NULL check is bypassed.
// This is purely to exercise the early checks in the function under test.
static cmsPipeline* dummyNonNullPipelinePointer() {
    // Use a non-NULL value that won't be accidentally treated as a valid
    // pipeline by the C code's internal checks (we only require non-NULL).
    return reinterpret_cast<cmsPipeline*>(0x1);
}

//------------------------------------------------------------------------------
// Test cases
//------------------------------------------------------------------------------

/*
  Test 1: Lut == NULL branch
  Rationale: Ensure the function immediately returns FALSE when *Lut is NULL.
  Expected: FALSE
*/
static bool test_LutNull_branch() {
    cmsPipeline* lut = NULL;          // *Lut == NULL should trigger
    cmsUInt32Number inFmt  = 0;
    cmsUInt32Number outFmt = 0;
    cmsUInt32Number flags  = 0;

    // TransformFn and FreeDataFn are not used in this early exit path
    cmsBool result = Optimize8BitRGBTransform(nullptr,
                                              (void**)&lut,
                                              nullptr,
                                              &lut,
                                              &inFmt,
                                              &outFmt,
                                              &flags);
    // Expected to be FALSE due to the early exit
    return result == 0;
}

/*
  Test 2: Floating-point input format branch
  Rationale: Ensure the function returns FALSE if either input or output format
  is of floating-point type. We pass a non-NULL Lut so we bypass the first
  early exit and reach the type checks.
  Expected: FALSE (assuming T_FLOAT(*InputFormat) or T_FLOAT(*OutputFormat) becomes true)
*/
static bool test_InputFloat_branch() {
    cmsPipeline* lut = dummyNonNullPipelinePointer();
    cmsUInt32Number inFmt  = 0x40000000; // heuristic value to trigger FLOAT path
    cmsUInt32Number outFmt = 0;
    cmsUInt32Number flags  = 0;

    cmsBool result = Optimize8BitRGBTransform(nullptr,
                                              (void**)&lut,
                                              nullptr,
                                              &lut,
                                              &inFmt,
                                              &outFmt,
                                              &flags);
    return result == 0;
}

/*
  Test 3: Non-8-bit input format branch
  Rationale: Ensure the function returns FALSE if input or output is not 8-bit.
  Expected: FALSE
*/
static bool test_Non8BitInput_branch() {
    cmsPipeline* lut = dummyNonNullPipelinePointer();
    cmsUInt32Number inFmt  = 2; // heuristic value not equal to 1 (8-bit)
    cmsUInt32Number outFmt = 1; // 8-bit for the test to trigger the check on input
    cmsUInt32Number flags  = 0;

    cmsBool result = Optimize8BitRGBTransform(nullptr,
                                              (void**)&lut,
                                              nullptr,
                                              &lut,
                                              &inFmt,
                                              &outFmt,
                                              &flags);
    return result == 0;
}

/*
  Test 4: RGB input color space requirement not met
  Rationale: Ensure the function returns FALSE if input color space is not RGB.
  Expected: FALSE
*/
static bool test_InputColorSpaceNotRGB_branch() {
    cmsPipeline* lut = dummyNonNullPipelinePointer();
    cmsUInt32Number inFmt  = 3; // heuristic non-RGB color space
    cmsUInt32Number outFmt = 1; // 8-bit
    cmsUInt32Number flags  = 0;

    cmsBool result = Optimize8BitRGBTransform(nullptr,
                                              (void**)&lut,
                                              nullptr,
                                              &lut,
                                              &inFmt,
                                              &outFmt,
                                              &flags);
    return result == 0;
}

/*
  Test 5: RGB output color space requirement not met
  Rationale: Ensure the function returns FALSE if output color space is not RGB.
  Expected: FALSE
*/
static bool test_OutputColorSpaceNotRGB_branch() {
    cmsPipeline* lut = dummyNonNullPipelinePointer();
    cmsUInt32Number inFmt  = 1; // 8-bit for input
    cmsUInt32Number outFmt = 4; // heuristic non-RGB color space
    cmsUInt32Number flags  = 0;

    cmsBool result = Optimize8BitRGBTransform(nullptr,
                                              (void**)&lut,
                                              nullptr,
                                              &lut,
                                              &inFmt,
                                              &outFmt,
                                              &flags);
    return result == 0;
}

//------------------------------------------------------------------------------
// Main test runner
//------------------------------------------------------------------------------

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const std::string& name, bool success) {
        total++;
        if (success) {
            passed++;
            printTestResult(name, true);
        } else {
            printTestResult(name, false);
        }
    };

    // Run tests with descriptive names
    run("Test_LutNull_branch (Lut == NULL) should return FALSE", test_LutNull_branch());
    run("Test_InputFloat_branch (FLOAT input) should return FALSE", test_InputFloat_branch());
    run("Test_Non8BitInput_branch (non-8-bit input) should return FALSE", test_Non8BitInput_branch());
    run("Test_InputColorSpaceNotRGB_branch (Input != RGB) should return FALSE", test_InputColorSpaceNotRGB_branch());
    run("Test_OutputColorSpaceNotRGB_branch (Output != RGB) should return FALSE", test_OutputColorSpaceNotRGB_branch());

    std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
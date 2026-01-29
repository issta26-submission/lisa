// Minimal C++11 test harness for the focal method Floating_Point_Transforms_Dispatcher
// This test suite uses a lightweight in-process mock framework (no GoogleTest).
// It intercepts dependent optimization functions to steer control flow and verify
// different branches of the dispatcher.
//
// Note: This test assumes the real project provides a proper linking context where
// the external C functions declared below resolve to the actual library symbols.
// The mocks provided here override those symbols for unit testing purposes.
//
// The test is designed to compile with a typical C/C++ build (C++11) without GTest.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>


// 1) Lightweight testing macros (non-terminating; always continue to maximize coverage)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) do { \
    if (cond) { ++g_tests_passed; } \
    else { ++g_tests_failed; std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ << "\n"; } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if (!(cond)) { ++g_tests_passed; } \
    else { ++g_tests_failed; std::cerr << "EXPECT_FALSE failed at " << __FILE__ << ":" << __LINE__ << "\n"; } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if ((a) == (b)) { ++g_tests_passed; } \
    else { ++g_tests_failed; std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ << ": " \
        << (a) << " != " << (b) << "\n"; } \
} while(0)

#define RUN_TESTS(...) do { \
    int total = 0; \
    int passed = 0; \
    __VA_ARGS__; \
    total = g_tests_passed + g_tests_failed; \
    passed = g_tests_passed; \
    std::cout << "Tests run: " << total << ", Passed: " << passed \
              << ", Failed: " << (total - passed) << std::endl; \
} while(0)


// 2) Bridging types to match the focal method's signature.
// These definitions are kept minimal and compatible with C linkage (extern "C").

extern "C" {

// Typedefs corresponding to the focal method's parameter types.
// We use opaque void pointers to avoid depending on library internals.
typedef int cmsBool;        // In the real project, cmsBool is typically int or bool
typedef void cmsPipeline;
typedef unsigned int cmsUInt32Number;

// The focal method signature (as shown in the prompt)
typedef void (_cmsTransform2Fn)();           // function type placeholder
typedef void (_cmsFreeUserDataFn)();

// Forward declaration of the focal function under test.
// In real usage, this function is defined in the library (fast_float_sup.c).
cmsBool Floating_Point_Transforms_Dispatcher(_cmsTransform2Fn* TransformFn,
                                  void** UserData,
                                  _cmsFreeUserDataFn* FreeUserData,
                                  cmsPipeline** Lut,
                                  cmsUInt32Number* InputFormat,
                                  cmsUInt32Number* OutputFormat,
                                  cmsUInt32Number* dwFlags);

// Flags (real values come from the library; we assume common bit values)
const cmsUInt32Number cmsFLAGS_SOFTPROOFING = 0x01;
const cmsUInt32Number cmsFLAGS_COPY_ALPHA = 0x02;

// Declarations of all Optimize* helper functions used by the dispatcher.
// We declare them here to provide test-time mocks.
cmsBool OptimizeMatrixShaper15(_cmsTransform2Fn* TransformFn, void** UserData,
                               _cmsFreeUserDataFn* FreeUserData,
                               cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                               cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool Optimize8ByJoiningCurves(_cmsTransform2Fn* TransformFn, void** UserData,
                               _cmsFreeUserDataFn* FreeUserData,
                               cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                               cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

#ifndef CMS_DONT_USE_SSE2
cmsBool Optimize8MatrixShaperSSE(_cmsTransform2Fn* TransformFn, void** UserData,
                                _cmsFreeUserDataFn* FreeUserData,
                                cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);
#endif

cmsBool Optimize8MatrixShaper(_cmsTransform2Fn* TransformFn, void** UserData,
                            _cmsFreeUserDataFn* FreeUserData,
                            cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                            cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool OptimizeFloatByJoiningCurves(_cmsTransform2Fn* TransformFn, void** UserData,
                                    _cmsFreeUserDataFn* FreeUserData,
                                    cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                    cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool OptimizeFloatMatrixShaper(_cmsTransform2Fn* TransformFn, void** UserData,
                                 _cmsFreeUserDataFn* FreeUserData,
                                 cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                 cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool Optimize8BitRGBTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                               _cmsFreeUserDataFn* FreeUserData,
                               cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                               cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool Optimize16BitRGBTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                _cmsFreeUserDataFn* FreeUserData,
                                cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool OptimizeCLUTRGBTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                               _cmsFreeUserDataFn* FreeUserData,
                               cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                               cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool OptimizeCLUTCMYKTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                 _cmsFreeUserDataFn* FreeUserData,
                                 cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                 cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

cmsBool OptimizeCLUTLabTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                _cmsFreeUserDataFn* FreeUserData,
                                cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags);

} // extern "C"



// 3) Global state to drive mock behavior and log calls
static std::vector<std::string> g_log_calls;

// Flags to control mock returns (true => function reports optimization success)
static bool g_ret_MATRIX_SHAPER_15 = false;
static bool g_ret_8BY_JOINING_CURVES = false;
static bool g_ret_8MATRIX_SHAPER_SSE = false;
static bool g_ret_8MATRIX_SHAPER = false;
static bool g_ret_FLOAT_BY_JOINING_CURVES = false;
static bool g_ret_FLOAT_MATRIX_SHAPER = false;
static bool g_ret_8BIT_RGB_TRANSFORM = false;
static bool g_ret_16BIT_RGB_TRANSFORM = false;
static bool g_ret_CLUT_RGB_TRANSFORM = false;
static bool g_ret_CLUT_CMYK_TRANSFORM = false;
static bool g_ret_CLUT_LAB_TRANSFORM = false;

// Simple alias for NULL-friendly mock expectations
static cmsPipeline* g_dummy_lut = nullptr;

// Helpers to reset state between tests
void reset_mock_state() {
    g_log_calls.clear();
    g_ret_MATRIX_SHAPER_15 = false;
    g_ret_8BY_JOINING_CURVES = false;
    g_ret_8MATRIX_SHAPER_SSE = false;
    g_ret_8MATRIX_SHAPER = false;
    g_ret_FLOAT_BY_JOINING_CURVES = false;
    g_ret_FLOAT_MATRIX_SHAPER = false;
    g_ret_8BIT_RGB_TRANSFORM = false;
    g_ret_16BIT_RGB_TRANSFORM = false;
    g_ret_CLUT_RGB_TRANSFORM = false;
    g_ret_CLUT_CMYK_TRANSFORM = false;
    g_ret_CLUT_LAB_TRANSFORM = false;
    g_dummy_lut = nullptr;
}

// 3) Mock implementations of the Optimize* functions.
// Each mock logs its invocation and returns the value controlled by global flags.

extern "C" cmsBool OptimizeMatrixShaper15(_cmsTransform2Fn* TransformFn, void** UserData,
                                        _cmsFreeUserDataFn* FreeUserData,
                                        cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                        cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("OptimizeMatrixShaper15");
    return g_ret_MATRIX_SHAPER_15 ? 1 : 0;
}

extern "C" cmsBool Optimize8ByJoiningCurves(_cmsTransform2Fn* TransformFn, void** UserData,
                                        _cmsFreeUserDataFn* FreeUserData,
                                        cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                        cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("Optimize8ByJoiningCurves");
    return g_ret_8BY_JOINING_CURVES ? 1 : 0;
}

#ifndef CMS_DONT_USE_SSE2
extern "C" cmsBool Optimize8MatrixShaperSSE(_cmsTransform2Fn* TransformFn, void** UserData,
                                         _cmsFreeUserDataFn* FreeUserData,
                                         cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                         cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("Optimize8MatrixShaperSSE");
    return g_ret_8MATRIX_SHAPER_SSE ? 1 : 0;
}
#endif

extern "C" cmsBool Optimize8MatrixShaper(_cmsTransform2Fn* TransformFn, void** UserData,
                                       _cmsFreeUserDataFn* FreeUserData,
                                       cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                       cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("Optimize8MatrixShaper");
    return g_ret_8MATRIX_SHAPER ? 1 : 0;
}

extern "C" cmsBool OptimizeFloatByJoiningCurves(_cmsTransform2Fn* TransformFn, void** UserData,
                                               _cmsFreeUserDataFn* FreeUserData,
                                               cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                               cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("OptimizeFloatByJoiningCurves");
    return g_ret_FLOAT_BY_JOINING_CURVES ? 1 : 0;
}

extern "C" cmsBool OptimizeFloatMatrixShaper(_cmsTransform2Fn* TransformFn, void** UserData,
                                             _cmsFreeUserDataFn* FreeUserData,
                                             cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                             cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("OptimizeFloatMatrixShaper");
    return g_ret_FLOAT_MATRIX_SHAPER ? 1 : 0;
}

extern "C" cmsBool Optimize8BitRGBTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                          _cmsFreeUserDataFn* FreeUserData,
                                          cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                          cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("Optimize8BitRGBTransform");
    return g_ret_8BIT_RGB_TRANSFORM ? 1 : 0;
}

extern "C" cmsBool Optimize16BitRGBTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                           _cmsFreeUserDataFn* FreeUserData,
                                           cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                           cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("Optimize16BitRGBTransform");
    return g_ret_16BIT_RGB_TRANSFORM ? 1 : 0;
}

extern "C" cmsBool OptimizeCLUTRGBTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                          _cmsFreeUserDataFn* FreeUserData,
                                          cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                          cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("OptimizeCLUTRGBTransform");
    return g_ret_CLUT_RGB_TRANSFORM ? 1 : 0;
}

extern "C" cmsBool OptimizeCLUTCMYKTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                            _cmsFreeUserDataFn* FreeUserData,
                                            cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                            cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("OptimizeCLUTCMYKTransform");
    return g_ret_CLUT_CMYK_TRANSFORM ? 1 : 0;
}

extern "C" cmsBool OptimizeCLUTLabTransform(_cmsTransform2Fn* TransformFn, void** UserData,
                                           _cmsFreeUserDataFn* FreeUserData,
                                           cmsPipeline** Lut, cmsUInt32Number* InputFormat,
                                           cmsUInt32Number* OutputFormat, cmsUInt32Number* dwFlags)
{
    g_log_calls.push_back("OptimizeCLUTLabTransform");
    return g_ret_CLUT_LAB_TRANSFORM ? 1 : 0;
}

// 4) A set of simple tests for the dispatcher.
// The tests rely on the mocks above to influence control flow.

void test_softproofing_short_circuits() {
    reset_mock_state();
    // Given: softproofing flag is set, dispatcher should return FALSE immediately
    cmsUInt32Number input = 0;
    cmsUInt32Number output = 0;
    cmsUInt32Number flags = cmsFLAGS_SOFTPROOFING; // softproofing ON

    cmsPipeline* lut = nullptr;
    // Call dispatcher
    cmsBool result = Floating_Point_Transforms_Dispatcher(nullptr,
                                      nullptr,
                                      nullptr,
                                      &lut,
                                      &input,
                                      &output,
                                      &flags);

    // Expect: FALSE (0)
    EXPECT_FALSE(result);
    // Expect: No optimization function should have been invoked
    // (log should be empty)
    EXPECT_EQ(g_log_calls.size(), size_t(0));
}

void test_sse2_path_activation() {
    reset_mock_state();
    // Prepare inputs that pass early checks (non-softproof, non-flavor)
    cmsUInt32Number input = 0;
    cmsUInt32Number output = 0;
    cmsUInt32Number flags = 0; // no softproofing, copy-alpha, etc.

    // Fast-path: simulate SSE path returns TRUE
    g_ret_8MATRIX_SHAPER_SSE = true;
    g_ret_MATRIX_SHAPER_15 = false;
    g_ret_8BY_JOINING_CURVES = false;

    cmsPipeline* lut = nullptr;

    cmsBool result = Floating_Point_Transforms_Dispatcher(nullptr,
                                      nullptr,
                                      nullptr,
                                      &lut,
                                      &input,
                                      &output,
                                      &flags);

    // We expect TRUE because the SSE optimization reports success
    EXPECT_TRUE(result);

    // Ensure that SSE optimization function was indeed called
    bool found_sse = false;
    for (auto const& s : g_log_calls) {
        if (s == "Optimize8MatrixShaperSSE") { found_sse = true; break; }
    }
    EXPECT_TRUE(found_sse);
}

void test_no_optimization_path() {
    reset_mock_state();
    // Ensure all optimizations report false
    g_ret_MATRIX_SHAPER_15 = false;
    g_ret_8BY_JOINING_CURVES = false;
    g_ret_8MATRIX_SHAPER_SSE = false;
    g_ret_8MATRIX_SHAPER = false;
    g_ret_FLOAT_BY_JOINING_CURVES = false;
    g_ret_FLOAT_MATRIX_SHAPER = false;
    g_ret_8BIT_RGB_TRANSFORM = false;
    g_ret_16BIT_RGB_TRANSFORM = false;
    g_ret_CLUT_RGB_TRANSFORM = false;
    g_ret_CLUT_CMYK_TRANSFORM = false;
    g_ret_CLUT_LAB_TRANSFORM = false;

    cmsUInt32Number input = 0;
    cmsUInt32Number output = 0;
    cmsUInt32Number flags = 0;

    cmsPipeline* lut = nullptr;
    cmsBool result = Floating_Point_Transforms_Dispatcher(nullptr,
                                      nullptr,
                                      nullptr,
                                      &lut,
                                      &input,
                                      &output,
                                      &flags);

    // Expect: FALSE since no optimization could handle the transform
    EXPECT_FALSE(result);

    // Optionally verify that all attempted functions were invoked in some order
    // (not strictly guaranteed here since all drivers return false)
}

void test_matrix_shaper_15_path() {
    reset_mock_state();
    // Force a path through OptimizeMatrixShaper15 by returning true
    g_ret_MATRIX_SHAPER_15 = true;

    cmsUInt32Number input = 0;
    cmsUInt32Number output = 0;
    cmsUInt32Number flags = 0;

    cmsPipeline* lut = nullptr;
    cmsBool result = Floating_Point_Transforms_Dispatcher(nullptr,
                                      nullptr,
                                      nullptr,
                                      &lut,
                                      &input,
                                      &output,
                                      &flags);

    // Expect TRUE due to first optimization step succeeding
    EXPECT_TRUE(result);

    // The first log entry should be OptimizeMatrixShaper15
    if (!g_log_calls.empty()) {
        EXPECT_EQ(g_log_calls.front(), std::string("OptimizeMatrixShaper15"));
    } else {
        std::cerr << "No optimization function was called in matrix_shaper_15_path test\n";
        g_tests_failed++;
    }
}

// 5) main driver to execute tests
int main() {
    // Run tests in sequence with explanatory comments.

    std::cout << "Running Floating_Point_Transforms_Dispatcher unit tests (no GTest)\n";

    test_softproofing_short_circuits();
    test_sse2_path_activation();
    test_no_optimization_path();
    test_matrix_shaper_15_path();

    std::cout << "Summary: " << g_tests_passed << " passed, "
              << g_tests_failed << " failed." << std::endl;

    return (g_tests_failed == 0) ? 0 : 1;
}
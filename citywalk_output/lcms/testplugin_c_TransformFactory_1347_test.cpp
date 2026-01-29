// Test suite for the TransformFactory focal method (in testplugin.c).
// This test suite is designed for C++11 compilation without GTest.
// It uses a tiny, self-contained test harness with simple assertions.
// The tests rely on the TransformFactory function provided by the project under test
// and the TrancendentalTransform symbol defined in the linked C module.
// Note: We declare the necessary types and function prototypes here to compile
// against the C implementation in testplugin.c.

#include <cstddef>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Step 1: Candidate Keywords inferred from the focal method
// - TYPE_GRAY_8: specific OutputFormat value triggering the branch
// - _cmsTransformFn: function pointer type for transforms
// - TrancendentalTransform: function assigned by TransformFactory when OUTPUT is TYPE_GRAY_8
// - cmsPipeline: opaque type representing a pipeline (Lut)
// - _cmsFreeUserDataFn: optional cleanup callback
// - cmsBool, TRUE, FALSE: boolean semantics
// These keywords guide the test coverage for true/false branches and pointer assignment.

// ---------------------------
// Minimal type declarations to allow
// linking with the C implementation in testplugin.c
// (These mirror the signatures expected by TransformFactory)
// ---------------------------

// Opaque types (as used by the focal function)
typedef struct _cmstransform_struct _cmstransform_struct;
typedef struct _cmsPipeline cmsPipeline;

// Boolean and integer types (typical in cms-like APIs)
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;

// Constants (matching the focal method's comparison)
#define TYPE_GRAY_8 1
#define TRUE 1
#define FALSE 0

// Function pointer types as used by the focal method
typedef void (*_cmsTransformFn)(struct _cmstransform_struct*, const void*, void*, cmsUInt32Number, cmsUInt32Number);
typedef void (*_cmsFreeUserDataFn)(void*);

// Forward declaration of the transform function implemented in the C layer.
// We declare it here so tests can compare the produced function pointer against it.
// The actual definition is expected to be provided in testplugin.c.
extern "C" void TrancendentalTransform(struct _cmstransform_struct* CMM,
                                      const void* InputBuffer,
                                      void* OutputBuffer,
                                      cmsUInt32Number Size,
                                      cmsUInt32Number Stride);

// Declaration of the focal method under test with C linkage to match the C implementation.
extern "C" cmsBool TransformFactory(_cmsTransformFn* xformPtr,
                                  void** UserData,
                                  _cmsFreeUserDataFn* FreePrivateDataFn,
                                  cmsPipeline** Lut,
                                  cmsUInt32Number* InputFormat,
                                  cmsUInt32Number* OutputFormat,
                                  cmsUInt32Number* dwFlags);

// ---------------------------
// Lightweight test harness
// ---------------------------

static bool g_allTestsPassed = true;

// Simple assertion macro that prints a message on failure but does not terminate program.
// This enables multiple tests to run in a single execution flow.
#define ASSERT(condition, message) do { \
    if (!(condition)) { \
        std::cerr << "ASSERTION FAILED: " << (message) << " [" #condition "]" << std::endl; \
        g_allTestsPassed = false; \
    } \
} while (0)

// Test 1: When OutputFormat points to TYPE_GRAY_8, the transform pointer should be set
// to TrancendentalTransform and the function should return TRUE.
bool test_TransformFactory_Gray8_SetsTransformPtr()
{
    // Arrange
    cmsBool result = FALSE;
    _cmsTransformFn* xformPtr = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn* FreePrivateDataFn = nullptr;
    cmsPipeline* Lut = nullptr;
    cmsUInt32Number InputFormat = 0;
    cmsUInt32Number OutputFormat = TYPE_GRAY_8; // trigger true branch
    cmsUInt32Number dwFlags = 0;

    // Act
    result = TransformFactory(&xformPtr, &UserData, FreePrivateDataFn, &Lut,
                              &InputFormat, &OutputFormat, &dwFlags);

    // Assert
    ASSERT(result == TRUE, "TransformFactory should return TRUE when OutputFormat == TYPE_GRAY_8");
    ASSERT(xformPtr == TrancendentalTransform, "TransformFactory should set xformPtr to TrancendentalTransform");
    // The Lut pointer should remain unchanged (we started with nullptr)
    ASSERT(Lut == nullptr, "Lut should remain unchanged (nullptr) in this test");

    return g_allTestsPassed;
}

// Test 2: When OutputFormat is not TYPE_GRAY_8, the function should return FALSE
// and should not modify the xformPtr.
bool test_TransformFactory_NotGray8_ReturnsFalseAndNoPointerSet()
{
    // Arrange
    cmsBool result = FALSE;
    _cmsTransformFn* xformPtr = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn* FreePrivateDataFn = nullptr;
    cmsPipeline* Lut = nullptr;
    cmsUInt32Number InputFormat = 0;
    cmsUInt32Number OutputFormat = 0; // not TYPE_GRAY_8
    cmsUInt32Number dwFlags = 0;

    // Act
    result = TransformFactory(&xformPtr, &UserData, FreePrivateDataFn, &Lut,
                              &InputFormat, &OutputFormat, &dwFlags);

    // Assert
    ASSERT(result == FALSE, "TransformFactory should return FALSE when OutputFormat != TYPE_GRAY_8");
    ASSERT(xformPtr == nullptr, "xformPtr should remain NULL when OutputFormat != TYPE_GRAY_8");
    return g_allTestsPassed;
}

// Test 3: Ensure that the function does not modify Lut when OutputFormat == TYPE_GRAY_8
// by feeding a sentinel pointer for Lut and verifying it remains unchanged.
bool test_TransformFactory_Gray8_LutUnmodified()
{
    // Arrange
    cmsBool result = FALSE;
    _cmsTransformFn* xformPtr = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn* FreePrivateDataFn = nullptr;
    cmsPipeline* sentinelLut = reinterpret_cast<cmsPipeline*>(0xDEADBEEF);
    cmsPipeline** Lut = &sentinelLut;
    cmsUInt32Number InputFormat = 0;
    cmsUInt32Number OutputFormat = TYPE_GRAY_8; // trigger true branch
    cmsUInt32Number dwFlags = 0;

    // Act
    result = TransformFactory(&xformPtr, &UserData, FreePrivateDataFn, Lut,
                              &InputFormat, &OutputFormat, &dwFlags);

    // Assert
    ASSERT(result == TRUE, "TransformFactory should return TRUE when OutputFormat == TYPE_GRAY_8");
    ASSERT(xformPtr == TrancendentalTransform, "TransformFactory should set xformPtr to TrancendentalTransform");
    ASSERT(*Lut == sentinelLut, "Lut pointer should remain unchanged after call");

    return g_allTestsPassed;
}

// ---------------------------
// Main: Run all tests and report results.
// ---------------------------

int main() {
    // Run tests
    bool t1 = test_TransformFactory_Gray8_SetsTransformPtr();
    bool t2 = test_TransformFactory_NotGray8_ReturnsFalseAndNoPointerSet();
    bool t3 = test_TransformFactory_Gray8_LutUnmodified();

    // Summary
    std::cout << "\nTransformFactory Test Summary:\n";
    std::cout << "  Test 1 (Gray8 sets transform ptr): " << (t1 ? "PASSED" : "FAILED") << "\n";
    std::cout << "  Test 2 (Not Gray8 returns false and leaves ptr NULL): " << (t2 ? "PASSED" : "FAILED") << "\n";
    std::cout << "  Test 3 (Gray8 does not modify Lut pointer): " << (t3 ? "PASSED" : "FAILED") << "\n";

    // Overall status
    if (t1 && t2 && t3 && g_allTestsPassed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}
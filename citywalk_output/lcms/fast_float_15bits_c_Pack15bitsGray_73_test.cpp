// Test suite for Pack15bitsGray in fast_float_15bits.c
// This test targets the focal method Pack15bitsGray and its basic contract:
// - It writes From16To15(Values[0]) into the start of Buffer as a 16-bit value.
// - It returns Buffer + 2.
// - It ignores CMMcargo and Stride (as unused parameters).

#include <cstring>
#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>


// Include project-specific types and declarations.
// The real project provides cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, etc.

// Forward declare the transform struct used by the function signature.
struct _cmstransform_struct;

// Prototype of the function under test (to enable linking with the compiled object).
extern "C" cmsUInt8Number* Pack15bitsGray(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                               CMSREGISTER cmsUInt16Number Values[],
                               CMSREGISTER cmsUInt8Number*  Buffer,
                               CMSREGISTER cmsUInt32Number  Stride);

// Helper: we rely on the real From16To15 available from the project headers.
// If From16To15 is inline in the header, this call will resolve correctly.
extern "C" cmsUInt16Number From16To15(cmsUInt16Number x16);

// Simple non-terminating test reporting
static int g_failures = 0;
static void reportFailure(const char* msg) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
    ++g_failures;
}

// Macro-like helpers for assertions that do not abort on failure
#define ASSERT_EQ_UINT16(expected, actual, msg)                                         \
    do {                                                                                  \
        if ((actual) != (expected)) {                                                   \
            reportFailure(std::string("ASSERT_EQ_UINT16: ").append(msg)                  \
                          .append(" | expected: ").append(std::to_string(expected))      \
                          .append(", actual: ").append(std::to_string(actual)).str()); \
        } else {                                                                          \
            std::cout << "PASS: " << (msg) << std::endl;                                \
        }                                                                                 \
    } while (0)

#define ASSERT_PTR_EQ(expected_ptr, actual_ptr, msg)                                      \
    do {                                                                                   \
        if ((actual_ptr) != (expected_ptr)) {                                             \
            reportFailure(std::string("ASSERT_PTR_EQ: ").append(msg)                       \
                          .append(" | expected: ").append(std::to_string(reinterpret_cast<uintptr_t>(expected_ptr))) \
                          .append(", actual: ").append(std::to_string(reinterpret_cast<uintptr_t>(actual_ptr))).str()); \
        } else {                                                                           \
            std::cout << "PASS: " << (msg) << std::endl;                                 \
        }                                                                                  \
    } while (0)


// Test 1: Basic contract - Pack15bitsGray writes From16To15(Values[0]) and returns Buffer+2
// Explanatory: Verify that the first two bytes of Buffer reflect From16To15(Values[0])
// and that the returned pointer equals Buffer + 2.
void test_Pack15bitsGray_WritesExpectedAndReturnsPointer() {
    // Arrange
    cmsUInt16Number values[1] = { 0x1234 };
    cmsUInt8Number buffer[4] = { 0, 0, 0, 0 };
    struct _cmstransform_struct* cargo = nullptr;
    cmsUInt32Number stride = 0;

    // Act
    cmsUInt8Number* ret = Pack15bitsGray(cargo, values, buffer, stride);

    // The expected value is From16To15(values[0])
    cmsUInt16Number expected = From16To15(values[0]);
    cmsUInt16Number actual;
    std::memcpy(&actual, buffer, sizeof(actual)); // read the 2-byte value regardless of alignment

    // Assert
    ASSERT_EQ_UINT16(static_cast<cmsUInt16Number>(expected), actual,
                     "Pack15bitsGray should store From16To15(Values[0]) into Buffer");

    // The function should return Buffer + 2
    ASSERT_PTR_EQ(buffer + 2, ret, "Pack15bitsGray should return Buffer + 2");
}

// Test 2: Multiple invocations do not rely on stale state
// Explanatory: Ensure successive calls overwrite Buffer correctly and keep returning Buffer+2.
void test_Pack15bitsGray_MultipleCallsOverwriteBuffer() {
    // Arrange
    cmsUInt8Number buffer[6];
    cmsUInt16Number values[1];
    struct _cmstransform_struct* cargo = nullptr;
    cmsUInt32Number stride = 0;

    // First call
    values[0] = 0x00A0;
    std::memset(buffer, 0xAA, sizeof(buffer));
    cmsUInt8Number* ret1 = Pack15bitsGray(cargo, values, buffer, stride);
    cmsUInt16Number expected1 = From16To15(values[0]);
    cmsUInt16Number actual1;
    std::memcpy(&actual1, buffer, sizeof(actual1));
    // Second call with a different value
    values[0] = 0x7FFF;
    cmsUInt8Number* ret2 = Pack15bitsGray(cargo, values, buffer, stride);
    cmsUInt16Number expected2 = From16To15(values[0]);
    cmsUInt16Number actual2;
    std::memcpy(&actual2, buffer, sizeof(actual2));

    // Assert first write correctness
    ASSERT_EQ_UINT16(static_cast<cmsUInt16Number>(expected1), actual1,
                     "First call: Buffer should contain From16To15(0x00A0)");
    // Assert return pointer correctness for first call
    ASSERT_PTR_EQ(buffer + 2, ret1, "First call: return Buffer + 2");

    // Assert second write correctness
    ASSERT_EQ_UINT16(static_cast<cmsUInt16Number>(expected2), actual2,
                     "Second call: Buffer should contain From16To15(0x7FFF)");
    // Assert return pointer correctness for second call
    ASSERT_PTR_EQ(buffer + 2, ret2, "Second call: return Buffer + 2");
}

// Test 3: Unused parameters do not affect behavior
// Explanatory: Verify that providing non-null CMMcargo and non-zero Stride does not change outcome.
void test_Pack15bitsGray_IgnoresUnusedParameters() {
    // Arrange
    cmsUInt16Number values[1] = { 0x1A2B };
    cmsUInt8Number buffer[4];
    // Provide non-null cargo and a non-zero stride
    struct _cmstransform_struct dummyCargo;
    cmsUInt32Number stride = 12345;

    // Act
    cmsUInt8Number* ret = Pack15bitsGray(&dummyCargo, values, buffer, stride);

    // Assert
    cmsUInt16Number expected = From16To15(values[0]);
    cmsUInt16Number actual;
    std::memcpy(&actual, buffer, sizeof(actual));
    ASSERT_EQ_UINT16(static_cast<cmsUInt16Number>(expected), actual,
                     "Ignores CMMcargo and Stride: buffer must reflect From16To15(Values[0])");
    ASSERT_PTR_EQ(buffer + 2, ret, "Return value should still be Buffer + 2");
}

// Test 4: Edge case - zero value
void test_Pack15bitsGray_EdgeCase_ZeroValue() {
    // Arrange
    cmsUInt16Number values[1] = { 0x0000 };
    cmsUInt8Number buffer[4] = { 0x11, 0x22, 0x33, 0x44 };
    struct _cmstransform_struct* cargo = nullptr;
    cmsUInt32Number stride = 0;

    // Act
    cmsUInt8Number* ret = Pack15bitsGray(cargo, values, buffer, stride);

    // Assert
    cmsUInt16Number expected = From16To15(values[0]);
    cmsUInt16Number actual;
    std::memcpy(&actual, buffer, sizeof(actual));
    ASSERT_EQ_UINT16(static_cast<cmsUInt16Number>(expected), actual,
                     "Zero input should produce From16To15(0) in Buffer");
    ASSERT_PTR_EQ(buffer + 2, ret, "Return value for zero input should be Buffer + 2");
}


// Main launcher
int main() {
    std::cout << "Running Pack15bitsGray test suite..." << std::endl;

    test_Pack15bitsGray_WritesExpectedAndReturnsPointer();
    test_Pack15bitsGray_MultipleCallsOverwriteBuffer();
    test_Pack15bitsGray_IgnoresUnusedParameters();
    test_Pack15bitsGray_EdgeCase_ZeroValue();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
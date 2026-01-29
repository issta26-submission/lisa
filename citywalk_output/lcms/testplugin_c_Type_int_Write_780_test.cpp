// Auto-generated unit test suite for the focal method
// Type_int_Write in testplugin.c
// Notes:
// - This test suite uses a lightweight in-process test framework (no GTest).
// - It mocks _cmsWriteUInt32Number via a C-linkage replacement to capture the
//   value that Type_int_Write passes to it. This allows verifying that the
//   dereferenced value from Ptr is indeed forwarded correctly.
// - The tests are designed to be portable with C++11 and rely only on the C
//   standard library plus the provided method under test.

#include <cstring>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Domain-specific: provide minimal, compatible type declarations to link with
// the C focal method in testplugin.c without pulling in the entire CMS library.
// We only need opaque types and the exact signatures used by Type_int_Write.

extern "C" {
    // Forward declaration of the type used by the focal method
    struct _cms_typehandler_struct;

    // Forward declaration of I/O handler type (opaque for testing)
    struct cmsIOHANDLER;

    // Typedefs matching the CMS codebase
    typedef uint32_t cmsUInt32Number;
    typedef int cmsBool;

    // Focal method under test (declaration to enable linkage)
    cmsBool Type_int_Write(struct _cms_typehandler_struct* self,
                           struct cmsIOHANDLER* io,
                           void* Ptr,
                           cmsUInt32Number nItems);

    // We declare _cmsWriteUInt32Number here so we can mock/override it in tests.
    // The actual implementation is provided by the library; in this test we
    // supply a replacement with C linkage to intercept the argument.
    cmsBool _cmsWriteUInt32Number(struct cmsIOHANDLER* io, cmsUInt32Number value);
}

// Global to capture the value passed to the mocked _cmsWriteUInt32Number
static cmsUInt32Number g_last_written_value = 0;

// Mocked _cmsWriteUInt32Number: captures the value that would be written by Type_int_Write
// and returns a success indicator (true/1).
extern "C" cmsBool _cmsWriteUInt32Number(struct cmsIOHANDLER* /*io*/, cmsUInt32Number value) {
    g_last_written_value = value;
    return 1; // success
}

// Lightweight fake CMS IO handler (opaque in the consuming function; we don't rely on its internals)
static struct cmsIOHANDLER fake_io;

// Simple in-test assertion framework (non-terminating)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "ASSERT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_tests_failed; \
        } else { \
            ++g_tests_passed; \
        } \
    } while (0)

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "ASSERT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_tests_failed; \
        } else { \
            ++g_tests_passed; \
        } \
    } while (0)

// Test 1: Basic correctness - ensure that the value pointed to by Ptr is correctly dereferenced
// and forwarded to _cmsWriteUInt32Number. We verify both the returned status and the forwarded value.
static void test_Type_int_Write_basic_forwarding() {
    // Prepare a test value
    cmsUInt32Number testVal = 0x12345678;
    g_last_written_value = 0x0; // reset capture
    // No actual CMS context is required for this focal test; self can be null
    cmsBool ret = Type_int_Write(nullptr, &fake_io, &testVal, 1);
    // The focal method should return whatever _cmsWriteUInt32Number returns (mocked as true)
    ASSERT_TRUE(ret == 1);
    // The value forwarded must match the dereferenced Ptr
    ASSERT_EQ(g_last_written_value, testVal);
}

// Test 2: Multiple values and pointer variations
// Validate that Type_int_Write handles different inputs and memory locations correctly.
static void test_Type_int_Write_multiple_values_and_ptr_variants() {
    // Array of test values to check multiple scenarios
    const cmsUInt32Number values[] = {
        0x00000000,
        0x00000001,
        0xABCD1234,
        0xFFFFFFFF
    };

    const size_t count = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < count; ++i) {
        g_last_written_value = 0;
        cmsUInt32Number v = values[i];
        cmsBool ret = Type_int_Write(nullptr, &fake_io, &v, 1);
        ASSERT_TRUE(ret == 1);
        ASSERT_EQ(g_last_written_value, v);
    }

    // Additional check: ensure that Ptr can point to elements within an array
    cmsUInt32Number arr[2] = { 0x11111111, 0x22222222 };
    g_last_written_value = 0;
    cmsBool ret0 = Type_int_Write(nullptr, &fake_io, &arr[0], 1);
    ASSERT_TRUE(ret0 == 1);
    ASSERT_EQ(g_last_written_value, arr[0]);

    g_last_written_value = 0;
    cmsBool ret1 = Type_int_Write(nullptr, &fake_io, &arr[1], 1);
    ASSERT_TRUE(ret1 == 1);
    ASSERT_EQ(g_last_written_value, arr[1]);
}

// Main entry: run all tests and report summary
int main() {
    // Zero-initialize the fake IO structure (no fields accessed by the focal test)
    std::memset(&fake_io, 0, sizeof(fake_io));

    // Run tests with explanatory comments
    std::cout << "Running Type_int_Write unit tests (no GTest) ..." << std::endl;

    // Test 1: basic forwarding correctness
    test_Type_int_Write_basic_forwarding();

    // Test 2: multiple values and pointer variations
    test_Type_int_Write_multiple_values_and_ptr_variants();

    // Summary
    std::cout << "Tests passed: " << g_tests_passed << std::endl;
    std::cout << "Tests failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
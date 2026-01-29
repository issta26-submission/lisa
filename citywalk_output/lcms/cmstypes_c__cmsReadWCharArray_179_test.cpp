// Unit-test suite for cmsBool _cmsReadWCharArray(cmsIOHANDLER* io, cmsUInt32Number n, wchar_t* Array)
// This test suite is designed to be used in environments where the focal function is
// provided by the project (cmstypes.c) and where internal helpers (_cmsReadUInt16Number,
// convert_utf16_to_utf32) may be overridden in tests via weak/redirectable symbols.
// The tests are written as a small, self-contained harness (no GTest) using a minimal
// non-terminating assertion style to maximize code coverage.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight, self-contained type aliases to model the cms types used by the focal function.
// In the real project these are provided by the library headers, but for the test harness we
// provide minimal, compatible definitions.
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef int cmsBool;
typedef int cmsInt32Number;

// Forward declare a minimal cmsIOHANDLER so the focal function can be linked in the test environment.
// In the actual project, cmsIOHANDLER is defined by the library headers.
struct cmsIOHANDLER;

// Prototype of the focal function under test.
// We assume cmsBool is an int in this test environment.
extern "C" cmsBool _cmsReadWCharArray(cmsIOHANDLER* io, cmsUInt32Number n, wchar_t* Array);

// Overridden/mockable internal helpers used by _cmsReadWCharArray.
// The test harness provides lightweight stubs for these to control behavior deterministically.
extern "C" cmsBool _cmsReadUInt16Number(cmsIOHANDLER* io, cmsUInt16Number* value);
extern "C" cmsBool convert_utf16_to_utf32(cmsIOHANDLER* io, cmsInt32Number n, wchar_t* output);

// A dummy concrete type for cmsIOHANDLER to satisfy addressability in tests.
struct cmsIOHANDLER {
    int dummy;
};

// Global knobs to control stub behavior from tests.
static int g_read_mode = 0;          // 0 = normal (return TRUE and supply values), 1 = fail (return FALSE)
static int g_convert_called = 0;     // number of times convert_utf16_to_utf32_stub was invoked

// Internal state for the _cmsReadUInt16Number_stub: a tiny sequence of 16-bit values to feed into Array.
static int g_seq_index = 0;
static const cmsUInt16Number g_seq_values[] = { 0x0041, 0x0042, 0x0043, 0x0044, 0x0045 }; // 'A','B','C','D','E'

// Stub: emulate reading a 16-bit number from the io handle.
// This overrides the library's internal function used by _cmsReadWCharArray.
extern "C" cmsBool _cmsReadUInt16Number(cmsIOHANDLER* io, cmsUInt16Number* value) {
    if (g_read_mode == 1) {
        // Simulate a read failure in the middle of reading
        if (value) { (void)0; } // suppress unused warnings
        return 0;
    }
    // Return the next value in the sequence, wrap around if necessary
    cmsUInt16Number v = g_seq_values[g_seq_index % (sizeof(g_seq_values)/sizeof(g_seq_values[0]))];
    g_seq_index++;
    if (value) {
        *value = v;
    }
    return 1;
}

// Stub: emulate converting a UTF-16 sequence to UTF-32 and writing to the output buffer.
// This path is exercised when wchar_t is wider than cmsUInt16Number (is32 == true) and Array != NULL.
extern "C" cmsBool convert_utf16_to_utf32(cmsIOHANDLER* io, cmsInt32Number n, wchar_t* output) {
    (void)io; // unused in the stub
    g_convert_called++;
    if (!output) return 1;
    // Fill output with a simple increasing sequence starting at 'A'
    for (cmsInt32Number i = 0; i < n; ++i) {
        output[i] = static_cast<wchar_t>(L'A' + i);
    }
    return 1;
}

// Small non-terminating assertion framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg)                                  \
    do {                                                        \
        ++g_tests_run;                                          \
        if (!(cond)) {                                          \
            ++g_tests_failed;                                   \
            std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; \
        }                                                       \
    } while (0)

#define EXPECT_EQ(lhs, rhs, msg)                                \
    do {                                                         \
        ++g_tests_run;                                           \
        if (!((lhs) == (rhs))) {                               \
            ++g_tests_failed;                                    \
            std::cerr << "EXPECT_EQ failed: " << (msg) << " (lhs=" \
                      << (lhs) << " != rhs=" << (rhs) << ")\n";  \
        }                                                        \
    } while (0)

#define ASSERT_NON_NULL(ptr, msg)                               \
    do {                                                        \
        ++g_tests_run;                                         \
        if ((ptr) == nullptr) {                                \
            ++g_tests_failed;                                  \
            std::cerr << "ASSERT_NON_NULL failed: " << (msg) << "\n"; \
        }                                                        \
    } while (0)

// Test driver entry
int main() {
    // Basic setup: a dummy io handle (non-null as required by _cmsReadWCharArray)
    cmsIOHANDLER dummy_io;
    dummy_io.dummy = 0;

    // Determine compile-time branch: is32 indicates wchar_t > 16 bits
    bool is32 = (sizeof(wchar_t) > sizeof(cmsUInt16Number));

    // Test Case 1: Normal path, Array != NULL, reads values via _cmsReadUInt16Number
    {
        g_read_mode = 0;
        g_convert_called = 0;
        g_seq_index = 0;

        std::vector<wchar_t> arr(3, 0);
        cmsBool ok = _cmsReadWCharArray(&dummy_io, 3, arr.data());

        // Expect TRUE
        EXPECT_TRUE(ok, "Case1: _cmsReadWCharArray should return TRUE on successful reads");

        // Expected content depends on whether 32-bit wchar_t is used
        if (is32) {
            // convert_utf16_to_utf32 is used; we expect A,B,C (from convert_utf16_to_utf32 stub)
            // However, note our convert_utf16_to_utf32 stub writes A,B,C
            EXPECT_EQ(arr[0], static_cast<wchar_t>(L'A'), "Case1: arr[0] should be 'A' when 32-bit wchar_t");
            EXPECT_EQ(arr[1], static_cast<wchar_t>(L'B'), "Case1: arr[1] should be 'B' when 32-bit wchar_t");
            EXPECT_EQ(arr[2], static_cast<wchar_t>(L'C'), "Case1: arr[2] should be 'C' when 32-bit wchar_t");
            EXPECT_TRUE(g_convert_called == 1, "Case1: convert_utf16_to_utf32 should be invoked exactly once");
        } else {
            // Non-32-bit wchar_t: values come from _cmsReadUInt16Number sequence and cast to wchar_t
            EXPECT_EQ(arr[0], static_cast<wchar_t>('A'), "Case1: arr[0] should be 'A' on 16-bit wchar_t");
            EXPECT_EQ(arr[1], static_cast<wchar_t>('B'), "Case1: arr[1] should be 'B' on 16-bit wchar_t");
            EXPECT_EQ(arr[2], static_cast<wchar_t>('C'), "Case1: arr[2] should be 'C' on 16-bit wchar_t");
        }
    }

    // Test Case 2: Array == NULL path should still return TRUE if reads succeed (no storage)
    {
        g_read_mode = 0;
        g_seq_index = 0;
        cmsBool ok;
        ok = _cmsReadWCharArray(&dummy_io, 5, nullptr);
        EXPECT_TRUE(ok, "Case2: _cmsReadWCharArray should return TRUE when Array is NULL and reads succeed");
    }

    // Test Case 3: Failure path (simulate read failure)
    {
        g_read_mode = 1; // force failure in _cmsReadUInt16Number
        g_seq_index = 0;

        std::vector<wchar_t> arr(3, 0);
        cmsBool ok = _cmsReadWCharArray(&dummy_io, 3, arr.data());

        EXPECT_TRUE(!ok, "Case3: _cmsReadWCharArray should return FALSE on read failure");
    }

#if (sizeof(wchar_t) > sizeof(cmsUInt16Number))
    // Test Case 4 (only meaningful if is32 path is taken): ensure the convert_utf16_to_utf32 path is invoked.
    {
        g_read_mode = 0;
        g_convert_called = 0;
        g_seq_index = 0;
        // Reset: memory should be overwritten by convert_utf16_to_utf32
        std::vector<wchar_t> arr(3, 0);
        cmsBool ok = _cmsReadWCharArray(&dummy_io, 3, arr.data());

        EXPECT_TRUE(ok, "Case4: _cmsReadWCharArray should return TRUE and use convert_utf16_to_utf32 when 32-bit wchar_t");
        EXPECT_TRUE(g_convert_called == 1, "Case4: convert_utf16_to_utf32 should be invoked exactly once in 32-bit path");
        // Validate that array contains A,B,C as per stub
        EXPECT_EQ(arr[0], static_cast<wchar_t>(L'A'), "Case4: arr[0] should be 'A' after convert_utf16_to_utf32");
        EXPECT_EQ(arr[1], static_cast<wchar_t>(L'B'), "Case4: arr[1] should be 'B' after convert_utf16_to_utf32");
        EXPECT_EQ(arr[2], static_cast<wchar_t>(L'C'), "Case4: arr[2] should be 'C' after convert_utf16_to_utf32");
    }
#endif

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";
    return (g_tests_failed == 0) ? 0 : 1;
}
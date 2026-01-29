// Lightweight unit test suite for Pack15bitsPlanar (C++11, no GoogleTest)
// This test provides minimal stubs to exercise the focal function without
// requiring the full original project build. It verifies the core behavior
// of Pack15bitsPlanar, including channel handling, DoSwap behavior, and
// pointer/stride arithmetic.

// Include necessary headers
#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>


// Typedefs and small stubs to imitate the project environment
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef bool     cmsBool;

// Macro stubs (matching the usage in the focal method)
#define CMSREGISTER

// Minimal xform head and transform struct to satisfy the focal function cast
struct _xform_head {
    cmsUInt32Number OutputFormat;
};

// The focal function expects a struct whose first member matches _xform_head.
// We provide a tiny wrapper so we can pass a pointer to _cmstransform_struct.
struct _cmstransform_struct {
    _xform_head head;
};

// Domain-specific macros used by the focal function
// We model a fixed, small configuration: 2 channels for simplicity in tests.
#define T_CHANNELS(fmt)  (2)
#define T_DOSWAP(fmt)    ((fmt) & 0x01)      // bit 0
#define T_FLAVOR(fmt)     ((fmt) & 0x02)      // bit 1
#define T_ENDIAN16(fmt)   ((fmt) & 0x04)      // bit 2
#define T_EXTRA(fmt)      ((fmt) & 0x08)      // bit 3

#define CMSREGISTER

// Inline helper: minimal PackOne used by Pack15bitsPlanar
static cmsUInt16Number PackOne(cmsUInt16Number x, cmsBool /*Reverse*/, cmsBool /*SwapEndian*/)
{
    // For testing stability, keep output deterministic and simple (identity)
    (void)Reverse;
    (void)SwapEndian;
    return x;
}
#define cmsINLINE inline

// The focal function under test (copied/adapted here for testharness)
// It writes nChan 16-bit values (from wOut[]) into the output buffer, handling
// optional swapping/ordering according to OutputFormat.
cmsUInt8Number* Pack15bitsPlanar(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                                 CMSREGISTER cmsUInt16Number wOut[],
                                 CMSREGISTER cmsUInt8Number* output,
                                 CMSREGISTER cmsUInt32Number Stride)
{
{
       _xform_head* head = (_xform_head*)CMMcargo;
       int nChan = T_CHANNELS(head->OutputFormat);
       int DoSwap = T_DOSWAP(head->OutputFormat);
       int Reverse = T_FLAVOR(head->OutputFormat);
       int SwapEndian = T_ENDIAN16(head->OutputFormat);
       CMSREGISTER int i;
       cmsUInt8Number* Init = output;

       if (DoSwap) {
              output += T_EXTRA(head->OutputFormat) * Stride * 2;
       }
       for (i = 0; i < nChan; i++) {
              int index = DoSwap ? (nChan - i - 1) : i;
              *(cmsUInt16Number*)output = PackOne(wOut[index], Reverse, SwapEndian);
              output += (Stride * sizeof(cmsUInt16Number));
       }
       return (Init + sizeof(cmsUInt16Number));
}
}

// End of focal function

// Lightweight unit-test harness (non-terminating assertions)
static std::vector<std::string> g_failures;

static void record_failure(const std::string& msg) {
    g_failures.push_back(msg);
}

// Helper to print test results
static void report_results() {
    if (g_failures.empty()) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << g_failures.size() << " test(s) failed:\n";
        for (size_t i = 0; i < g_failures.size(); ++i) {
            std::cout << "[" << i+1 << "] " << g_failures[i] << "\n";
        }
    }
}

// Test 1: No swap, no extra, two channels. Expect wOut[0] at offset 0 and wOut[1] at offset 2.
// This covers the false branch for DoSwap.
static void test_Pack15bitsPlanar_no_swap_two_channels() {
    _cmstransform_struct cargo;
    cargo.head.OutputFormat = 0x00; // DoSwap=0, Extra=0, Reverse=0, Endian=0

    cmsUInt16Number wOut[2] = { 0x1111, 0x2222 };

    // Prepare a buffer large enough to hold data for 2 channels with Stride=1
    cmsUInt8Number buffer[16];
    std::memset(buffer, 0xAA, sizeof(buffer)); // fill with sentinel

    cmsUInt8Number* res = Pack15bitsPlanar(&cargo, wOut, buffer, 1);

    // Read the first two 16-bit values written
    cmsUInt16Number got0;
    cmsUInt16Number got1;
    std::memcpy(&got0, buffer + 0, sizeof(got0));
    std::memcpy(&got1, buffer + 2, sizeof(got1));

    // Expected: res == buffer + 2
    if (res != buffer + 2) {
        record_failure("test_Pack15bitsPlanar_no_swap_two_channels: return pointer mismatch");
    }
    // Expected values
    if (got0 != wOut[0] || got1 != wOut[1]) {
        record_failure("test_Pack15bitsPlanar_no_swap_two_channels: data mismatch");
    }
}

// Test 2: DoSwap=true, Extra=0, two channels. Expect reversed order in wOut due to index calculation.
static void test_Pack15bitsPlanar_swap_two_channels() {
    _cmstransform_struct cargo;
    cargo.head.OutputFormat = 0x01; // DoSwap=1, Extra=0, Reverse=0, Endian=0

    cmsUInt16Number wOut[2] = { 0x1111, 0x2222 };

    cmsUInt8Number buffer[16];
    std::memset(buffer, 0x00, sizeof(buffer));

    cmsUInt8Number* res = Pack15bitsPlanar(&cargo, wOut, buffer, 1);

    cmsUInt16Number got0;
    cmsUInt16Number got1;
    std::memcpy(&got0, buffer + 0, sizeof(got0));
    std::memcpy(&got1, buffer + 2, sizeof(got1));

    // Expected: first written is wOut[1], then wOut[0]
    if (res != buffer + 2) {
        record_failure("test_Pack15bitsPlanar_swap_two_channels: return pointer mismatch");
    }
    if (got0 != wOut[1] || got1 != wOut[0]) {
        record_failure("test_Pack15bitsPlanar_swap_two_channels: data mismatch");
    }
}

// Test 3: DoSwap=true, Extra=1, two channels. Tests pointer offset with Extra and swap behavior.
static void test_Pack15bitsPlanar_swap_with_extra() {
    _cmstransform_struct cargo;
    cargo.head.OutputFormat = 0x09; // DoSwap=1, Extra=1, Reverse=0, Endian=0

    cmsUInt16Number wOut[2] = { 0x1234, 0xABCD };

    cmsUInt8Number buffer[24];
    std::memset(buffer, 0x00, sizeof(buffer));

    cmsUInt8Number* res = Pack15bitsPlanar(&cargo, wOut, buffer, 1);

    cmsUInt16Number got0;
    cmsUInt16Number got1;
    std::memcpy(&got0, buffer + 2, sizeof(got0)); // first two bytes after offset
    std::memcpy(&got1, buffer + 4, sizeof(got1));

    // Expected: with Extra, initial two bytes should be untouched (0s if buffer was zero)
    // and first written value should be wOut[1], then wOut[0] at subsequent position
    if (res != buffer + 2) {
        record_failure("test_Pack15bitsPlanar_swap_with_extra: return pointer mismatch");
    }
    if (got0 != wOut[1] || got1 != wOut[0]) {
        record_failure("test_Pack15bitsPlanar_swap_with_extra: data mismatch");
    }
}

// Main: run all tests and report
int main() {
    test_Pack15bitsPlanar_no_swap_two_channels();
    test_Pack15bitsPlanar_swap_two_channels();
    test_Pack15bitsPlanar_swap_with_extra();

    report_results();
    return g_failures.empty() ? 0 : 1;
}
// Lightweight C++11 test suite for the focal method Pack15bitsChunky
// This test suite provides minimal scaffolding to compile and run the targeted function
// without relying on external testing frameworks (GTest, etc.).
// It mocks the necessary types, macros, and the dependent helper PackOne function
// to exercise the control flow and data handling in Pack15bitsChunky.
//
// Notes:
// - Tests focus on true/false branches of predicates derived from OutputFormat.
// - Buffer manipulation, DoSwap, Reverse, and SwapEndian behavior are exercised.
// - A small test harness is provided in main() to run all tests and report results.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cstdint>


// Minimal typedefs to mirror the focal code's types
typedef unsigned char      cmsUInt8Number;
typedef unsigned short     cmsUInt16Number;
typedef unsigned int       cmsUInt32Number;
typedef int                cmsBool;

// Macro placeholders (no special attributes in this simplified test)
#define CMSREGISTER
#define UNUSED_PARAMETER(x) (void)(x)

// Forward declaration to mimic the project's struct dependency
struct _cmstransform_struct;

// Definition of the minimal internal structures used by Pack15bitsChunky
struct _xform_head {
    cmsUInt32Number OutputFormat;
};

// The test harness casts CMMcargo to _xform_head*, so ensure compatible type exists
struct _cmstransform_struct {
    // Intentionally empty for test harness; the real project has a richer structure.
};

// Helper macros to extract fields from OutputFormat
// Encoding convention used by tests (mirroring the focal code's expectations)
#define T_CHANNELS(fmt) ((fmt) & 0xFF)
#define T_DOSWAP(fmt)   (((fmt) >> 8) & 0x1)
#define T_FLAVOR(fmt)    (((fmt) >> 9) & 0x1)     // Reverse flag
#define T_ENDIAN16(fmt)  (((fmt) >> 10) & 0x1)    // SwapEndian flag
#define T_EXTRA(fmt)     (((fmt) >> 11) & 0x7F)   // Extra word count for offset

// Prototype of Pack15bitsChunky as in the focal code
cmsUInt8Number* Pack15bitsChunky(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                           CMSREGISTER cmsUInt16Number Values[],
                           CMSREGISTER cmsUInt8Number*  Buffer,
                           CMSREGISTER cmsUInt32Number  Stride);

// Minimal implementation of PackOne used by Pack15bitsChunky
// This provides a deterministic transformation to validate buffer content.
// Reverse: swap bytes within the 16-bit word.
// SwapEndian: swap bytes again (effectively a second potential transformation).
static cmsUInt16Number PackOne(cmsUInt16Number x, cmsBool Reverse, cmsBool SwapEndian)
{
    cmsUInt16Number v = x;
    if (Reverse) {
        v = (v >> 8) | (v << 8); // Swap bytes
    }
    if (SwapEndian) {
        v = (v << 8) | (v >> 8); // Swap bytes again
    }
    return v;
}

// Implementation of Pack15bitsChunky per the focal method
cmsUInt8Number* Pack15bitsChunky(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                           CMSREGISTER cmsUInt16Number Values[],
                           CMSREGISTER cmsUInt8Number*  Buffer,
                           CMSREGISTER cmsUInt32Number  Stride)
{
    {
       _xform_head* head = (_xform_head*)CMMcargo;
       int nChan = T_CHANNELS(head->OutputFormat);
       int DoSwap = T_DOSWAP(head->OutputFormat);
       int Reverse = T_FLAVOR(head->OutputFormat);
       int SwapEndian = T_ENDIAN16(head->OutputFormat);
       CMSREGISTER int i;
       UNUSED_PARAMETER(Stride);
       if (DoSwap) {
              Buffer += T_EXTRA(head->OutputFormat) * 2;
       }
       for (i = 0; i < nChan; i++) {
              int index = DoSwap ? (nChan - i - 1) : i;
              *(cmsUInt16Number*)Buffer = PackOne(Values[index], Reverse, SwapEndian);
              Buffer += 2;
       }
       return Buffer;
    }
}

// Simple test harness for regression-style unit tests
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Utility to read 16-bit word from a byte buffer (machine endianness-aware)
static cmsUInt16Number ReadWord(const cmsUInt8Number* p)
{
    return *(const cmsUInt16Number*)p;
}

// Assertion helpers that do not abort, but report failures and continue
#define ASSERT_EQ(actual, expected, msg) do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
        ++g_failed_tests; \
        std::cerr << "FAIL: " << msg << " | Expected: " << (expected) \
                  << "  Got: " << (actual) << std::endl; \
    } \
} while(0)

#define ASSERT_PTR_EQ(actual, expected, msg) do { \
    ++g_total_tests; \
    if ((void*)(actual) != (void*)(expected)) { \
        ++g_failed_tests; \
        std::cerr << "FAIL: " << msg << " | Pointers differ: " \
                  << actual << " vs " << expected << std::endl; \
    } \
} while(0)

static void test_Pack15bitsChunky_no_swap_no_reverse()
{
    // Description:
    // - DoSwap = 0, Reverse = 0, SwapEndian = 0
    // - nChan = 3, values written in natural order
    // - Buffer should contain PackOne(Values[i], 0, 0) == Values[i]
    CmsUInt32Number nChan = 3;
    cmsUInt32Number fmt = nChan;                    // channels = 3
    // Ensure DoSwap/Reverse/Endian are 0
    // Bits: DoSwap at bit 8, Reverse at bit 9, Endian at bit 10, Extra at [11..]
    // All zero except channels.
    _xform_head head = { fmt };
    cmsUInt16Number Values[3] = { 0x1234, 0xABCD, 0x0F0F };

    std::vector<cmsUInt8Number> Buffer(2 * nChan);
    cmsUInt8Number* start = Buffer.data();

    // Call the focal function
    Pack15bitsChunky((CMSREGISTER struct _cmstransform_struct*)&head,
                     Values,
                     start,
                     0);

    // Validate: read back as 16-bit words
    cmsUInt16Number w0 = ReadWord(start + 0);
    cmsUInt16Number w1 = ReadWord(start + 2);
    cmsUInt16Number w2 = ReadWord(start + 4);

    ASSERT_EQ(w0, Values[0], "Pack15bitsChunky: first channel should equal input value (no swap)");
    ASSERT_EQ(w1, Values[1], "Pack15bitsChunky: second channel should equal input value (no swap)");
    ASSERT_EQ(w2, Values[2], "Pack15bitsChunky: third channel should equal input value (no swap)");
    // Return value is the end pointer; it should be start + 6
    cmsUInt8Number* endPtr = Pack15bitsChunky((CMSREGISTER struct _cmstransform_struct*)&head,
                                             Values,
                                             start,
                                             0);
    ASSERT_PTR_EQ(endPtr, start + 6, "Pack15bitsChunky: end pointer after 3 channels (no swap)");
}

static void test_Pack15bitsChunky_doSwap_reversed_order()
{
    // Description:
    // - DoSwap = 1 (reverse channel order)
    // - Reverse (bit-order reversal) = 1
    // - SwapEndian = 0
    // - nChan = 2
    // - Values: [0x1111, 0x2222], expected: [PackOne(Values[1]), PackOne(Values[0])]
    cmsUInt32Number nChan = 2;
    cmsUInt32Number fmt = nChan | (1u << 8) | (1u << 9) | (0u << 10);
    _xform_head head = { fmt };
    cmsUInt16Number Values[2] = { 0x1111, 0x2222 };
    std::vector<cmsUInt8Number> Buffer(4);
    cmsUInt8Number* start = Buffer.data();

    // Perform single call
    Pack15bitsChunky((CMSREGISTER struct _cmstransform_struct*)&head,
                     Values,
                     start,
                     0);

    // Expected values after one call
    cmsUInt16Number expected0 = PackOne(Values[1], 1, 0);
    cmsUInt16Number expected1 = PackOne(Values[0], 1, 0);

    cmsUInt16Number got0 = ReadWord(start + 0);
    cmsUInt16Number got1 = ReadWord(start + 2);

    ASSERT_EQ(got0, expected0, "Pack15bitsChunky: first channel with swap/reverse");
    ASSERT_EQ(got1, expected1, "Pack15bitsChunky: second channel with swap/reverse");

    // Validate pointer advancement
    cmsUInt8Number* endPtr = Pack15bitsChunky((CMSREGISTER struct _cmstransform_struct*)&head,
                                             Values,
                                             start,
                                             0);
    ASSERT_PTR_EQ(endPtr, start + 4, "Pack15bitsChunky: end pointer after 2 channels (doSwap)");
}

static void test_Pack15bitsChunky_with_extra_offset()
{
    // Description:
    // - DoSwap = 1, Extra = 2 (Offset by 4 bytes at start)
    // - nChan = 2, values reversed order due to DoSwap
    // - Reverse = 0, SwapEndian = 0
    cmsUInt32Number nChan = 2;
    cmsUInt32Number Extra = 2; // extra words
    cmsUInt32Number fmt = nChan | (1u << 8) | (0u << 9) | (0u << 10) | (Extra << 11);
    _xform_head head = { fmt };
    cmsUInt16Number Values[2] = { 0xAAAA, 0xBBBB };

    std::vector<cmsUInt8Number> Buffer(4 + 4); // extra space + needed space
    cmsUInt8Number* start = Buffer.data();

    // Call function
    cmsUInt8Number* endPtr = Pack15bitsChunky((CMSREGISTER struct _cmstransform_struct*)&head,
                                             Values,
                                             start,
                                             0);

    // Since DoSwap is active, starting buffer is advanced by Extra*2 = 4 bytes
    cmsUInt8Number* expectedStart = start + (Extra * 2);

    // After two channels, end pointer should be expectedStart + 4
    ASSERT_PTR_EQ(endPtr, expectedStart + 4, "Pack15bitsChunky: end pointer with extra offset");

    // Validate that the two 16-bit words (from expectedStart) are PackOne(Values[index], 0, 0)
    cmsUInt16Number w0 = ReadWord(expectedStart + 0);
    cmsUInt16Number w1 = ReadWord(expectedStart + 2);

    cmsUInt16Number expected0 = PackOne(Values[1], 0, 0); // index = nChan-1
    cmsUInt16Number expected1 = PackOne(Values[0], 0, 0);

    ASSERT_EQ(w0, expected0, "Pack15bitsChunky: first filled word after offset (extra)");
    ASSERT_EQ(w1, expected1, "Pack15bitsChunky: second filled word after offset (extra)");
}

static void test_Pack15bitsChunky_reverse_and_swap_case()
{
    // Description:
    // - DoSwap = 0, Reverse = 1, SwapEndian = 1
    // - nChan = 3
    // - Values chosen to verify PackOne transformations apply per index
    cmsUInt32Number nChan = 3;
    cmsUInt32Number fmt = nChan | (0u << 8) | (1u << 9) | (1u << 10);
    _xform_head head = { fmt };
    cmsUInt16Number Values[3] = { 0x0A0A, 0x1B1B, 0x2C2C };

    std::vector<cmsUInt8Number> Buffer(6);
    cmsUInt8Number* start = Buffer.data();

    Pack15bitsChunky((CMSREGISTER struct _cmstransform_struct*)&head,
                     Values,
                     start,
                     0);

    // Expected order is i = 0..2 with index = i
    cmsUInt16Number expected0 = PackOne(Values[0], 1, 1);
    cmsUInt16Number expected1 = PackOne(Values[1], 1, 1);
    cmsUInt16Number expected2 = PackOne(Values[2], 1, 1);

    cmsUInt16Number w0 = ReadWord(start + 0);
    cmsUInt16Number w1 = ReadWord(start + 2);
    cmsUInt16Number w2 = ReadWord(start + 4);

    ASSERT_EQ(w0, expected0, "Pack15bitsChunky: first channel with reverse+swap");
    ASSERT_EQ(w1, expected1, "Pack15bitsChunky: second channel with reverse+swap");
    ASSERT_EQ(w2, expected2, "Pack15bitsChunky: third channel with reverse+swap");
}

// Simple runner
static void run_all_tests()
{
    test_Pack15bitsChunky_no_swap_no_reverse();
    test_Pack15bitsChunky_doSwap_reversed_order();
    test_Pack15bitsChunky_with_extra_offset();
    test_Pack15bitsChunky_reverse_and_swap_case();
}

int main()
{
    std::cout << "Starting Pack15bitsChunky test suite (custom minimal harness)" << std::endl;
    run_all_tests();
    std::cout << "Tests run: " << g_total_tests << "  Failures: " << g_failed_tests << std::endl;
    if (g_failed_tests > 0) {
        std::cout << "Some tests FAILED. Review the error messages above." << std::endl;
        return 1;
    }
    std::cout << "All tests PASSED." << std::endl;
    return 0;
}
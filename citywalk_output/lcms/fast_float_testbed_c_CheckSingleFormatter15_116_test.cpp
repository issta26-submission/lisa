/*
This is a self-contained C++11 test harness that reproduces and exercises
the focal method: CheckSingleFormatter15 as described in the prompt.
Because the original project details (external headers, full implementations
of cms types, Formatter_15Bit_Factory, etc.) are not available here, this file
provides minimal, self-contained mocks and stubs to allow unit-testing of the
described control flow and behavior.

Step 1 - Program Understanding (high-level keywords)
- Core components: cmsContext, cmsUInt32Number, cmsUInt16Number, cmsUInt8Number
- Dependent helpers/macros: Formatter_15Bit_Factory, T_CHANNELS, T_BYTES
- Data flow: write-then-read roundtrip via 16-bit formatter (Input -> Buffer -> Output)
- Branch coverage: missing formatter (Fmt16 == NULL), normal path where values roundtrip
- Side effects: Fail(...) used to report test failures

Candidate Keywords (represented in code):
- cmsFormatter, Fmt16, Formatter_15Bit_Factory
- CMS_PACK_FLAGS_16BITS, cmsFormatterInput, cmsFormatterOutput
- T_CHANNELS, T_BYTES, Values, Buffer, info (_xform_head)
- Fail, UNUSED_PARAMETER, memcpy/memset-like behavior
- CheckSingleFormatter15 (the focal method)
- Test scenarios: valid formatter path, missing formatter path, varying Type to cover channels
*/

/* 
Note: This test harness is self-contained and does not depend on GTest or any
external test framework. It re-implements a minimal subset of the environment
needed to exercise the focal method logic, while keeping the surface area small
and deterministic.
*/

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cstdarg>
#include <memory.h>
#include <stdlib.h>
#include <cstdint>


// Domain-specific typedefs and constants (mocked for the test harness)
using cmsContext = void*;
using cmsUInt32Number = uint32_t;
using cmsUInt16Number = uint16_t;
using cmsUInt8Number  = uint8_t;
using cmsInt32Number  = int;

#define cmsMAXCHANNELS 8
#define CMS_PACK_FLAGS_16BITS 0x01
#define cmsFormatterInput 0
#define cmsFormatterOutput 1

// Forward-declare opaque types used by the focal method
struct _cmstransform_struct {};
typedef void (*Fmt16Func)(struct _cmstransform_struct*, cmsUInt16Number*, cmsUInt8Number*, cmsInt32Number);

// Minimal formatter struct mimicking the library's shape
struct cmsFormatter {
    Fmt16Func Fmt16;
};

// _xform_head used by the focal method
struct _xform_head {
    cmsUInt32Number InputFormat;
    cmsUInt32Number OutputFormat;
};

// Externals from the provided focal class dependencies (mocked)
static cmsInt32Number g_nChannels = 2;        // number of active channels (set by Formatter_15Bit_Factory)
static cmsUInt32Number g_TypeForFactory = 0; // last Type passed to factory
static int g_ModeForFactory = cmsFormatterInput; // cmsFormatterInput or cmsFormatterOutput
static bool g_fail = false;
static int g_failCount = 0;

// Utility to mimic non-terminating assertion style in tests
#define UNUSED_PARAMETER(x) (void)(x)

// Minimal Fail implementation used by the focal code
static void Fail(const char* frm, ...)
{
    va_list ap;
    va_start(ap, frm);
    fprintf(stderr, "Fail: ");
    vfprintf(stderr, frm, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    g_fail = true;
    g_failCount++;
}

// Other stubs (not exercised in these tests but provided for completeness)
static void trace(const char* frm, ...)
{
    (void)frm;
    // no-op
}
static void FatalErrorQuit(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text)
{
    (void)ContextID; (void)ErrorCode; (void)Text;
}
static struct cmsHPROFILE_t { int dummy; }* CreateCurves(void)
{
    return nullptr;
}
typedef struct cmsHPROFILE_t cmsHPROFILE;

// ------------- Mocked Factory and helpers -------------
// Forward declare formatters
static void Fmt16_15bit(struct _cmstransform_struct* /*p*/, cmsUInt16Number* Values, cmsUInt8Number* Buffer, cmsInt32Number n);
static cmsFormatter Formatter_15Bit_Factory(cmsUInt32Number Type, int Direction, cmsUInt32Number Flags);

// A simple mapping from Type to channel count (to exercise multiple paths)
static void update_nChannels_from_Type(cmsUInt32Number Type)
{
    switch (Type) {
        case 1: g_nChannels = 2; break;
        case 2: g_nChannels = 3; break;
        case 3: g_nChannels = 4; break;
        default: g_nChannels = 2; break;
    }
}

// The format function used by f and b (input copies Values -> Buffer, output copies Buffer -> Values)
static void Fmt16_15bit(struct _cmstransform_struct* /*p*/, cmsUInt16Number* Values, cmsUInt8Number* Buffer, cmsInt32Number /*n*/)
{
    if (g_ModeForFactory == cmsFormatterInput) {
        // Copy input values into Buffer (interpreted as 16-bit values)
        cmsUInt16Number* Buff16 = reinterpret_cast<cmsUInt16Number*>(Buffer);
        for (cmsInt32Number i = 0; i < g_nChannels; ++i) {
            Buff16[i] = Values[i];
        }
    } else {
        // Copy Buffer back into Values
        cmsUInt16Number* Buff16 = reinterpret_cast<cmsUInt16Number*>(Buffer);
        for (cmsInt32Number i = 0; i < g_nChannels; ++i) {
            Values[i] = Buff16[i];
        }
    }
}

// Factory returns a cmsFormatter with a proper Fmt16 pointer.
// For a special Type==99 we simulate "no formatter" by returning Fmt16==NULL.
static cmsFormatter Formatter_15Bit_Factory(cmsUInt32Number Type, int Direction, cmsUInt32Number Flags)
{
    cmsFormatter ret;
    ret.Fmt16 = nullptr; // default

    // Track Type and mode
    g_TypeForFactory = Type;
    g_ModeForFactory = Direction;

    // Map Type to channel count
    update_nChannels_from_Type(Type);

    if (Type == 99) {
        // Simulate missing formatter
        ret.Fmt16 = nullptr;
        return ret;
    }

    // Assign the proper Fmt16 function
    ret.Fmt16 = &Fmt16_15bit;
    return ret;
}

// ------------- The focal method under test (reproduced) -------------
// We reproduce the exact logic flow described in the prompt using our mocks.
// This mirrors the signature and control flow, but relies on the mocked
// environment above for behavior.
extern "C" void CheckSingleFormatter15(cmsContext id, cmsUInt32Number Type, const char* Text)
{
    {
        cmsUInt16Number Values[cmsMAXCHANNELS];
        cmsUInt8Number Buffer[1024];
        cmsFormatter f, b;
        cmsInt32Number i, j, nChannels, bytes;
        struct _xform_head info;
        UNUSED_PARAMETER(id);
        memset(&info, 0, sizeof(info));
        info.InputFormat = info.OutputFormat = Type;

        // Get functions to go forth and back
        f = Formatter_15Bit_Factory(Type, cmsFormatterInput, CMS_PACK_FLAGS_16BITS);
        b = Formatter_15Bit_Factory(Type, cmsFormatterOutput, CMS_PACK_FLAGS_16BITS);

        if (f.Fmt16 == NULL || b.Fmt16 == NULL) {
            Fail("no formatter for %s", Text);
            return;
        }

        nChannels = g_nChannels; // T_CHANNELS(Type) -> g_nChannels
        bytes = nChannels * 2;   // T_BYTES(Type) -> g_nChannels * 2

        for (j = 0; j < 5; j++) {
            for (i = 0; i < nChannels; i++) {
                Values[i] = (cmsUInt16Number)(i + j) << 1;
            }
            b.Fmt16((struct _cmstransform_struct*) &info, Values, Buffer, 1);
            memset(Values, 0, sizeof(Values));
            f.Fmt16((struct _cmstransform_struct*) &info, Values, Buffer, 1);
            for (i = 0; i < nChannels; i++) {
                if (Values[i] != ((i + j) << 1)) {
                    Fail("%s failed", Text);
                    return;
                }
            }
        }
    }
}

// ------------- Minimal test harness and assertions -------------
static void test_description(const char* name)
{
    std::cout << "[TEST] " << name << std::endl;
}

// Test 1: Basic success path for a known Type with multiple channels
static void test_CheckSingleFormatter15_basic_success_type2()
{
    // true path: Type=2 => 3 channels
    g_fail = false;
    const char* label = "CheckSingleFormatter15_Type2"; // name label
    test_description(label);
    CheckSingleFormatter15(nullptr, 2, "Type2");
    if (g_fail) {
        std::cout << "  -> FAILED: expected success but a failure was reported." << std::endl;
    } else {
        std::cout << "  -> PASSED: formatter roundtrip succeeded." << std::endl;
    }
}

// Test 2: Basic success path for another Type with different channels
static void test_CheckSingleFormatter15_basic_success_type3()
{
    // true path: Type=3 => 4 channels
    g_fail = false;
    const char* label = "CheckSingleFormatter15_Type3"; // name label
    test_description(label);
    CheckSingleFormatter15(nullptr, 3, "Type3");
    if (g_fail) {
        std::cout << "  -> FAILED: expected success but a failure was reported." << std::endl;
    } else {
        std::cout << "  -> PASSED: formatter roundtrip succeeded." << std::endl;
    }
}

// Test 3: Negative path - missing formatter should trigger Fail
static void test_CheckSingleFormatter15_missing_formatter()
{
    g_fail = false;
    const char* label = "CheckSingleFormatter15_MissingFormatter";
    test_description(label);
    // Type==99 is used by our mock to emit a NULL Fmt16
    CheckSingleFormatter15(nullptr, 99, "MissingFormatterType99");
    if (g_fail) {
        std::cout << "  -> PASSED: missing formatter correctly reported (Fail called)." << std::endl;
    } else {
        std::cout << "  -> FAILED: expected a failure due to missing formatter but none occurred." << std::endl;
    }
}

// Main runner to execute all tests
int main(int /*argc*/, char** /*argv*/)
{
    std::cout << "Starting CheckSingleFormatter15 unit tests (self-contained harness)..." << std::endl;

    test_CheckSingleFormatter15_basic_success_type2();
    test_CheckSingleFormatter15_basic_success_type3();
    test_CheckSingleFormatter15_missing_formatter();

    std::cout << "Tests finished. Fail count: " << g_failCount << std::endl;
    return (g_failCount == 0) ? 0 : 1;
}
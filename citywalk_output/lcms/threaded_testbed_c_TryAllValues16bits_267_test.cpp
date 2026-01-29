// Lightweight C++11 test harness for the focal method TryAllValues16bits
// This test suite mocks the minimal CMS-like API used by the focal function
// to enable isolated unit testing without the real LittleCMS library.
// The goal is to exercise typical success and failure paths and to maximize
// coverage with non-terminating style where possible.
// Note: This file provides an in-repo stand-in for the CMS API and the
// TryAllValues16bits implementation (adapted to the test harness) to avoid
// external dependencies.

#include <cstring>
#include <threaded_internal.h>
#include <iostream>
#include <stdexcept>
#include <time.h>
#include <cstdarg>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cstdint>


// ------------------ Mock CMS-like API (minimalist, header-like declarations) ------------------

typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef int   cmsInt32Number;
typedef uint32_t cmsUInt32Number;
typedef void* cmsHTRANSFORM;

// Pixel type used by the focal method
struct Scanline_rgba16bits {
    uint16_t r, g, b, a;
};

// Basic constants used by TryAllValues16bits
#define TYPE_RGBA_16 0
#define FLAGS 0
#define cmsFLAGS_NOCACHE 0
#define cmsFLAGS_COPY_ALPHA 0

// Macro used in input population (8-bit to 16-bit expansion)
#define FROM_8_TO_16(x) ( (uint16_t)(( (x) << 8 ) | (uint16_t)(x) ) )

// Forward declaration of the focal method (we implement a test version in this file)
extern "C" void TryAllValues16bits(cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut, cmsInt32Number Intent);

// Internal helpers and state for the mock environment
namespace TestMockCMS {

static int g_forceNullFirstTransform = 0;  // If non-zero, force the first transform creation to fail (NULL)
static int g_outputDifference        = 0;  // If non-zero, cause a deliberate difference between RAW vs PLUGIN outputs
static int g_contextSequential       = 0;  // Tracks creation order for context IDs

static const cmsContext RAW_CONTEXT_ID    = (cmsContext)0x1001;
static const cmsContext PLUGIN_CONTEXT_ID = (cmsContext)0x1002;

// Simple error mechanism to emulate "Fail" behavior in the focal code.
// We throw an exception to abort the current test path but allow the test harness
// to catch and report the outcome (non-terminating in the sense of the test flow).
class FailException : public std::exception {
public:
    explicit FailException(const char* msg) : _msg(msg) {}
    const char* what() const noexcept { return _msg; }
private:
    const char* _msg;
};

static void Fail(const char* frm, ...)
{
    // Build a formatted string from variadic args, then throw
    char buf[512];
    va_list ap;
    va_start(ap, frm);
    vsnprintf(buf, sizeof(buf), frm, ap);
    va_end(ap);
    throw FailException(buf);
}

// Mocked contexts creator
extern "C" cmsContext cmsCreateContext(cmsContext, cmsContext)
{
    // Return distinct context IDs for first and second calls
    ++g_contextSequential;
    if (g_contextSequential == 1) {
        return RAW_CONTEXT_ID;    // first context: RAW
    }
    else {
        return PLUGIN_CONTEXT_ID; // second context: PLUGIN
    }
}

// Mocked transform handle type
typedef struct {
    cmsContext ctx;
    int isRaw;
} MockTransform;
typedef void* cmsHTRANSFORM;

// Simple storage to moderate transform lifecycle
static inline MockTransform* toMockTransform(cmsHTRANSFORM xform)
{
    return (MockTransform*)xform;
}

// Mocked transform creator
extern "C" cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ctx,
                                            cmsHPROFILE /*in*/, int /*inType*/,
                                            cmsHPROFILE /*out*/, int /*outType*/,
                                            int /*Intent*/, unsigned int /*flags*/)
{
    if (TestMockCMS::g_forceNullFirstTransform) {
        // Simulate a NULL transform on the first call
        // We only force on the very first transform creation
        static int s_seenFirst = 0;
        if (s_seenFirst == 0) {
            s_seenFirst++;
            return NULL;
        }
        // After first forced NULL, behave normally
        s_seenFirst++;
    }

    // Create a simple transform object that records whether it's RAW or PLUGIN
    MockTransform* t = new MockTransform();
    t->ctx  = ctx;
    t->isRaw = (ctx == RAW_CONTEXT_ID); // RAW if context matches RAW_CONTEXT_ID
    return (cmsHTRANSFORM)t;
}

// Mocked DoTransform: copies input to output; with an option to introduce a difference for testing
extern "C" void cmsDoTransform(cmsHTRANSFORM xform,
                             const void* input, void* output,
                             cmsUInt32Number npixels)
{
    MockTransform* t = toMockTransform(xform);
    const Scanline_rgba16bits* in  = (const Scanline_rgba16bits*)input;
    Scanline_rgba16bits* out       = (Scanline_rgba16bits*)output;

    if (TestMockCMS::g_outputDifference && t->isRaw) {
        // Copy and then create a tiny intentional difference to test the comparison
        for (cmsUInt32Number i = 0; i < npixels; ++i) {
            out[i] = in[i];
        }
        if (npixels > 0) {
            out[0].r ^= 1; // flip LSB to guarantee mismatch against the plugin path
        }
    } else {
        // Exact copy
        for (cmsUInt32Number i = 0; i < npixels; ++i) {
            out[i] = in[i];
        }
    }
}

// Mocked close/delete/context lifecycle
extern "C" void cmsCloseProfile(cmsHPROFILE /*p*/) { /* no-op in test harness */ }
extern "C" void cmsDeleteTransform(cmsHTRANSFORM xform) { delete toMockTransform(xform); }
extern "C" void cmsDeleteContext(cmsContext /*ctx*/) { /* no-op in test harness */ }

// Other CMS hooks used by the focal code are no-ops or trivial
```
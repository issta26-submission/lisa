/*
  Lightweight C++11 test suite for the focal function:
  Type_Curve_Read from the LittleCMS-like code in cmstypes.c

  Notes:
  - This test suite uses a minimal, self-contained test harness (no GoogleTest).
  - It relies on the public LittleCMS API where possible (e.g., cmsEvalToneCurve) to verify outcomes.
  - It uses a small in-memory mock of cmsIOHANDLER to feed bytes to the internal _cmsRead* helpers invoked by Type_Curve_Read.
  - The tests exercise true/false branches of the Count predicate and the array read path.
  - The test suite is designed to be compiled alongside the project (cmstypes.c and headers like lcms2.h).
  - Static helpers in the test harness mimic the expected internal IO pattern without exposing private state.

  Important: If your build environment uses slightly different internal IO signatures, adjust the MockIORead signature and the fields used on cmsIOHANDLER accordingly.

  Build (example):
  g++ -std=c++11 -I<path_to_lcms_headers> -c test_Type_Curve_Read.cpp
  g++ -std=c++11 test_Type_Curve_Read.o cmstypes.c -lc -o test_Type_Curve_Read
*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include LittleCMS headers (adjust include paths as needed)

// Forward declare the focal function (as it appears in cmstypes.c)
extern "C" void *Type_Curve_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag);

// Minimal helper to compare doubles with tolerance
static bool almost_equal(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Simple, compact test harness
static int g_failures = 0;
static void fail(const char* file, int line, const char* cond) {
    std::fprintf(stderr, "TEST FAIL: %s:%d: %s\n", file, line, cond);
    ++g_failures;
}
#define ASSERT_TRUE(x) \
    do { if(!(x)) fail(__FILE__, __LINE__, #x); } while(0)
#define ASSERT_FALSE(x) \
    do { if((x)) fail(__FILE__, __LINE__, ("!(" #x ")")); } while(0)
#define ASSERT_NEAR(a,b,tol) \
    do { if(!almost_equal((a),(b),tol)) { char buf[128]; std::snprintf(buf, sizeof(buf), "expected %.12f ~= %.12f", (a), (b)); fail(__FILE__, __LINE__, buf); } } while(0)
#define ASSERT_NOT_NULL(p) \
    do { if((p) == nullptr) fail(__FILE__, __LINE__, "pointer is null"); } while(0)
#define ASSERT_NULL(p) \
    do { if((p) != nullptr) fail(__FILE__, __LINE__, "pointer is not null"); } while(0)

// In-memory IO mock to feed _cmsReadUInt** helpers
struct MockIOState {
    std::vector<uint8_t> data;
    size_t pos = 0;
};

// The actual cmsIOHANDLER struct layout is implementation-specific.
// We assume a layout compatible with cmsIOHANDLER{ Read, Write, ContextID }.
// If your environment uses a slightly different layout, adjust the member names accordingly.
typedef cmsBool (*CMSIOReadFn)(cmsIOHANDLER*, void*, cmsUInt32Number);

static cmsBool MockIORead(struct _cmsIOHANDLER_struct* io, void* Buffer, cmsUInt32Number Size) {
    MockIOState* s = reinterpret_cast<MockIOState*>(io->ContextID); // ContextID points to MockIOState
    if (!s) return 0;
    if (s->pos + Size > s->data.size()) return 0; // not enough data
    std::memcpy(Buffer, s->data.data() + s->pos, (size_t)Size);
    s->pos += Size;
    return 1;
}

// Helper to set up a cmsIOHANDLER with a given MockIOState
static void SetupMockIO(cmsIOHANDLER* io, MockIOState* state) {
    // The exact field names depend on your header; adjust if necessary.
    io->Read = MockIORead;
    io->Write = nullptr;      // not used in tests
    io->Seek = nullptr;       // if present, leave NULL for read-only tests
    io->ContextID = static_cast<void*>(state);
}

// Test 1: Count == 0 (Linear)
static void test_Curve_Read_Count0_Linear() {
    // Prepare IO: Count = 0
    MockIOState ioState;
    ioState.data = {
        // Count = 0 (uint32 little-endian)
        0x00, 0x00, 0x00, 0x00
    };
    cmsIOHANDLER io;
    SetupMockIO(&io, &ioState);

    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0; // dummy context id

    cmsUInt32Number nItems = 0;
    void* curve = Type_Curve_Read(&self, &io, &nItems, 0);

    ASSERT_NOT_NULL(curve);
    ASSERT_TRUE(nItems == 1);

    // Verify linear behavior: evaluate at 0.5 should be ~0.5
    cmsFloat64Number val = cmsEvalToneCurve((cmsToneCurve*)curve, 0.5);
    ASSERT_NEAR(val, 0.5, 1e-6);

    // Cleanup
    cmsFreeToneCurve(curve);
}

// Test 2: Count == 1 with gamma fixed-point (2.0)
static void test_Curve_Read_Count1_GammaFixed() {
    // Prepare IO: Count = 1, GammaFixed = 2.0 encoded as 0x0200 (Fixed 8.8)
    MockIOState ioState;
    ioState.data = {
        // Count = 1
        0x01, 0x00, 0x00, 0x00,
        // GammaFixed = 2.0 -> 0x0200 in little-endian
        0x00, 0x02
    };
    cmsIOHANDLER io;
    SetupMockIO(&io, &ioState);

    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0; // dummy context id

    cmsUInt32Number nItems = 0;
    void* curve = Type_Curve_Read(&self, &io, &nItems, 0);

    ASSERT_NOT_NULL(curve);
    ASSERT_TRUE(nItems == 1);

    // For gamma = 2.0, f(x) = x^2. Check at 0.5 -> ~0.25
    cmsFloat64Number val = cmsEvalToneCurve((cmsToneCurve*)curve, 0.5);
    ASSERT_NEAR(val, 0.25, 1e-6);

    cmsFreeToneCurve(curve);
}

// Test 3: Count > 0x7FFF should fail (return NULL)
static void test_Curve_Read_CountTooLarge() {
    // Prepare IO: Count = 0x8000
    MockIOState ioState;
    ioState.data = {
        // Count = 0x8000
        0x00, 0x80, 0x00, 0x00
    };
    cmsIOHANDLER io;
    SetupMockIO(&io, &ioState);

    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0;

    cmsUInt32Number nItems = 0;
    void* curve = Type_Curve_Read(&self, &io, &nItems, 0);

    ASSERT_NULL(curve);
}

// Test 4: Count = 3 with a simple 3-entry tabulated curve
// Values: 0, 32768, 65535 (linearly mapped 0.0, 0.5, 1.0)
static void test_Curve_Read_Count3_Tabulated() {
    MockIOState ioState;
    ioState.data = {
        // Count = 3
        0x03, 0x00, 0x00, 0x00,
        // Table16[0] = 0x0000
        0x00, 0x00,
        // Table16[1] = 0x8000 (32768)
        0x00, 0x80,
        // Table16[2] = 0xFFFF (65535)
        0xFF, 0xFF
    };
    cmsIOHANDLER io;
    SetupMockIO(&io, &ioState);

    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0;

    cmsUInt32Number nItems = 0;
    void* curve = Type_Curve_Read(&self, &io, &nItems, 0);

    ASSERT_NOT_NULL(curve);
    ASSERT_TRUE(nItems == 1);

    // Check endpoints and midpoint via evaluation
    cmsFloat64Number v0  = cmsEvalToneCurve((cmsToneCurve*)curve, 0.0);
    cmsFloat64Number v05 = cmsEvalToneCurve((cmsToneCurve*)curve, 0.5);
    cmsFloat64Number v1   = cmsEvalToneCurve((cmsToneCurve*)curve, 1.0);

    ASSERT_NEAR(v0, 0.0, 1e-6);
    ASSERT_NEAR(v05, 0.5, 1e-2); // allow a small interpolation tolerance
    ASSERT_NEAR(v1, 1.0, 1e-6);

    cmsFreeToneCurve(curve);
}

// Entry point
int main() {
    // Run tests
    test_Curve_Read_Count0_Linear();
    test_Curve_Read_Count1_GammaFixed();
    test_Curve_Read_CountTooLarge();
    test_Curve_Read_Count3_Tabulated();

    if (g_failures == 0) {
        std::cout << "All Type_Curve_Read tests PASSED." << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cerr << g_failures << " Type_Curve_Read test(s) FAILED." << std::endl;
        return EXIT_FAILURE;
    }
}
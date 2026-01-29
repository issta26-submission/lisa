// Test suite for the focal method: Check8linearXFORM
// This test suite provides a minimal, self-contained environment to exercise
// the Check8linearXFORM function without requiring the full original
// Little CMS test harness. It mocks the necessary types and dependencies,
// and uses a light-weight test harness with non-terminating assertions.

/*
   Key testing approach:
   - Provide a fake transform object (FakeXForm) and a mock cmsDoTransform function.
   - Use a controllable offset in the mock transform to simulate exact (pass) and
     lossy (fail) transforms.
   - Validate:
     - Pass scenario: identity transform (offset = 0) should return 1 and not call Fail.
     - Fail scenario: transform with offset > 2 should return 0 and invoke Fail.
     - Corner case: zero channels should return 1, and not call Fail.
   - All tests are implemented in C++11 and do not rely on a specific test framework.
*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <testcms2.h>
#include <algorithm>
#include <cstdarg>


// ---------------------------------------------------------------------------
// Minimal type and function stubs to compile the focal method in a standalone test
// environment. These mimic just enough of the original lcMS types to compile.
// ---------------------------------------------------------------------------

typedef int cmsInt32Number;              // signed 32-bit int
typedef unsigned char cmsUInt8Number;      // 8-bit unsigned
typedef unsigned int cmsUInt32Number;     // 32-bit unsigned
typedef void* cmsHTRANSFORM;               // opaque handle

// Maximum number of channels typically supported; chosen conservatively for test
const int cmsMAXCHANNELS = 16;

// ---------------------------------------------------------------------------
// Global hook to capture Fail messages from the focal function
// ---------------------------------------------------------------------------

static int gFailCalled = 0;
static const char* gFailedFmt = nullptr;

// Variadic Fail function used by the focal code
extern "C" void Fail(const char* frm, ...)
{
    gFailCalled = 1;
    gFailedFmt = frm;
    // Minimal formatting: print to stdout for debugging, but do not abort.
    va_list ap;
    va_start(ap, frm);
    vprintf(frm, ap);
    printf("\n");
    va_end(ap);
}

// ---------------------------------------------------------------------------
// Mocked transform interface
// ---------------------------------------------------------------------------

struct FakeXForm {
    int id;
    int offset; // how much to offset the input value during transform
};

// Forward declare the mock cmsDoTransform used by the focal method
extern "C" void cmsDoTransform(cmsHTRANSFORM xform, cmsUInt8Number* In, cmsUInt8Number* Out, cmsUInt32Number n);

// The mock implementation of cmsDoTransform.
// Out[i] = In[i] + offset, clipped to [0, 255].
extern "C" void cmsDoTransform(cmsHTRANSFORM xform, cmsUInt8Number* In, cmsUInt8Number* Out, cmsUInt32Number n)
{
    FakeXForm* f = static_cast<FakeXForm*>(xform);
    int off = (f != nullptr) ? f->offset : 0;

    for (cmsUInt32Number i = 0; i < n; ++i) {
        int v = static_cast<int>(In[i]);
        int r = v + off;
        if (r < 0) r = 0;
        if (r > 255) r = 255;
        Out[i] = static_cast<cmsUInt8Number>(r);
    }
}

// ---------------------------------------------------------------------------
// Focal function under test (copied/adapted for standalone testing)
// Note: The signature and logic mirror the snippet provided in the prompt.
// Verbose comments are included to aid understanding in this isolated context.
// ---------------------------------------------------------------------------

extern "C" cmsInt32Number Check8linearXFORM(cmsHTRANSFORM xform, cmsInt32Number nChan)
{
{
    cmsInt32Number n2, i, j;
    cmsUInt8Number Inw[cmsMAXCHANNELS], Outw[cmsMAXCHANNELS];
    n2=0;
    for (j=0; j < 0xFF; j++) {
        memset(Inw, j, sizeof(Inw));
        cmsDoTransform(xform, Inw, Outw, 1);
        for (i = 0; i < nChan; i++) {
           // compute absolute difference without relying on std::abs for portability
           cmsInt32Number dif = Outw[i] - j;
           if (dif < 0) dif = -dif;
           if (dif > n2) n2 = dif;
        }
    }
    // We allow 2 contone of difference on 8 bits
    if (n2 > 2) {
        Fail("Differences too big (%x)", n2);
        return 0;
    }
    return 1;
}
}

// ---------------------------------------------------------------------------
// Additional (stubs) to satisfy potential linker references within the focal code.
// They are not used by our tests but are provided to resemble the project environment.
// ---------------------------------------------------------------------------

extern "C" cmsInt32Number Compare8bitXFORM(cmsHTRANSFORM xform1, cmsHTRANSFORM xform2, cmsInt32Number nChan) { return 0; }
extern "C" cmsInt32Number Check16linearXFORM(cmsHTRANSFORM xform, cmsInt32Number nChan) { return 0; }
extern "C" cmsInt32Number Compare16bitXFORM(cmsHTRANSFORM xform1, cmsHTRANSFORM xform2, cmsInt32Number nChan) { return 0; }
extern "C" cmsInt32Number CheckFloatlinearXFORM(cmsHTRANSFORM xform, cmsInt32Number nChan) { return 0; }
extern "C" cmsInt32Number CompareFloatXFORM(cmsHTRANSFORM xform1, cmsHTRANSFORM xform2, cmsInt32Number nChan) { return 0; }

// ---------------------------------------------------------------------------
// Test harness
// ---------------------------------------------------------------------------

static void test_printf(const char* msg) {
    printf("%s\n", msg);
}

// Helper macro for non-terminating checks
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        printf("UNEXPECTED FAIL: %s\n", msg); \
    } else { \
        printf("PASS: %s\n", msg); \
    } \
} while(0)

static void test_reset_state() {
    gFailCalled = 0;
    gFailedFmt = nullptr;
}

// Test 1: Pass scenario with identity transform (offset = 0), nChan > 0
void test_Check8linearXFORM_PassIdentity() {
    test_reset_state();
    // Create a transform with zero offset to simulate perfect linear mapping
    FakeXForm* f = new FakeXForm{0, 0}; // id=0, offset=0
    cmsHTRANSFORM h = static_cast<cmsHTRANSFORM>(f);

    cmsInt32Number nChan = 4; // test multiple channels
    cmsInt32Number res = Check8linearXFORM(h, nChan);

    // Expect a successful result and no fail calls
    bool passed = (res == 1) && (gFailCalled == 0);
    EXPECT_TRUE(passed, "Check8linearXFORM_PassIdentity: should return 1 and not call Fail");
    delete f;
}

// Test 2: Fail scenario with non-zero offset causing large differences (> 2)
void test_Check8linearXFORM_FailOffset3() {
    test_reset_state();
    // Create a transform with a non-zero offset to induce differences
    FakeXForm* f = new FakeXForm{1, 4}; // id=1, offset=4 -> diffs around 4
    cmsHTRANSFORM h = static_cast<cmsHTRANSFORM>(f);

    cmsInt32Number nChan = 6;
    cmsInt32Number res = Check8linearXFORM(h, nChan);

    // Expect a failure (return 0) and that Fail was invoked
    bool failed = (res == 0) && (gFailCalled != 0);
    EXPECT_TRUE(failed, "Check8linearXFORM_FailOffset3: should return 0 and call Fail");
    delete f;
}

// Test 3: Edge case with zero channels; function should return 1 and not call Fail
void test_Check8linearXFORM_ZeroChannels() {
    test_reset_state();
    FakeXForm* f = new FakeXForm{2, 2}; // arbitrary
    cmsHTRANSFORM h = static_cast<cmsHTRANSFORM>(f);

    cmsInt32Number nChan = 0;
    cmsInt32Number res = Check8linearXFORM(h, nChan);

    bool ok = (res == 1) && (gFailCalled == 0);
    EXPECT_TRUE(ok, "Check8linearXFORM_ZeroChannels: with nChan=0 should return 1 and not call Fail");
    delete f;
}

// Additional test: ensure that even with multiple repeated calls, the function behaves deterministically.
// This test uses identity transform repeatedly to ensure stability.
void test_Check8linearXFORM_RepeatedIdentity() {
    test_reset_state();
    FakeXForm* f = new FakeXForm{3, 0}; // id=3, offset=0
    cmsHTRANSFORM h = static_cast<cmsHTRANSFORM>(f);

    for (int t = 0; t < 5; ++t) {
        gFailCalled = 0;
        cmsInt32Number res = Check8linearXFORM(h, 8);
        if (res != 1 || gFailCalled != 0) {
            printf("UNEXPECTED FAIL: Check8linearXFORM_RepeatedIdentity at iteration %d\n", t);
        } else {
            printf("PASS: Check8linearXFORM_RepeatedIdentity iteration %d\n", t);
        }
    }
    delete f;
}

// Main function: run all tests
int main() {
    printf("Starting unit tests for Check8linearXFORM (standalone test harness)\n\n");

    test_Check8linearXFORM_PassIdentity();
    test_Check8linearXFORM_FailOffset3();
    test_Check8linearXFORM_ZeroChannels();
    test_Check8linearXFORM_RepeatedIdentity();

    printf("\nUnit tests completed.\n");
    return 0;
}
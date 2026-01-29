// Comprehensive C++11 test harness for Type_MHC2_Dup (conceptual unit tests without GTest)
// This test mocks minimal dependencies to exercise the focal function logic in a self-contained way.
// Notes:
// - This is a standalone test harness that re-implements the essential parts of the production
//   environment needed to exercise Type_MHC2_Dup (structures, _cmsDupMem, and memory-freeing).
// - The tests cover: successful duplication and failure paths (failure on RedCurve, GreenCurve, BlueCurve).
// - Non-terminating assertions are implemented via a lightweight CHECK macro that logs failures
//   but does not abort the test run, allowing multiple checks per test to execute.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain-like typedefs (subset to keep test self-contained)
typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;
typedef int cmsContext;      // fake context id
typedef int cmsBool;          // 0/1 for false/true

// Minimal production-like structures (subset)
struct _cms_typehandler_struct {
    cmsUInt32Number ContextID;
};

// Production type used by Type_MHC2_Dup
typedef struct {
    cmsUInt32Number CurveEntries;
    cmsFloat64Number* RedCurve;
    cmsFloat64Number* GreenCurve;
    cmsFloat64Number* BlueCurve;
} cmsMHC2Type;

// Globals used to drive simulated memory behavior in tests
static int g_dup_call_seq = 0;          // tracks number of _cmsDupMem calls
static int g_fail_on_call = -1;         // which call should fail (-1 means never fail)
static std::vector<void*> g_allocations; // track allocations for optional leaks observation

// Lightweight CHECK macro to emulate non-terminating assertions
static int g_tests_failed = 0;
#define CHECK(cond, msg) do { if(!(cond)) { std::cerr << "CHECK FAILED: " << msg << "\n"; ++g_tests_failed; } } while(0)

// Mocked memory duplication function used by Type_MHC2_Dup
void* _cmsDupMem(cmsContext ContextID, const void* Ptr, cmsUInt32Number size) {
    // Increment call count to simulate different stages (dup of mhc2 struct, then curves)
    ++g_dup_call_seq;
    if (g_fail_on_call == g_dup_call_seq) {
        // Simulate allocation/duplication failure for this call
        return NULL;
    }

    if (size == 0) {
        // Nothing to duplicate
        return NULL;
    }

    void* dst = std::malloc(size);
    if (!dst) {
        return NULL;
    }
    if (Ptr) {
        std::memcpy(dst, Ptr, size);
    } else {
        // If source is NULL, zero-filled block to mimic behavior (not strictly required here)
        std::memset(dst, 0, size);
    }
    // Track allocation for potential leak checks (not strictly required for correctness)
    g_allocations.push_back(dst);
    return dst;
}

// Production-like freeing function for Type_MHC2_Dup failures or successful dupes
void Type_MHC2_Free(struct _cms_typehandler_struct* self, void* Ptr);

// Forward declaration of the focal dup function (re-implemented for test)
void* Type_MHC2_Dup(struct _cms_typehandler_struct* self, const void* Ptr, cmsUInt32Number n);

// Implementations mirroring the focal code (adapted to test environment)
void Type_MHC2_Free(struct _cms_typehandler_struct* self, void* Ptr) {
    if (Ptr == nullptr) return;
    cmsMHC2Type* mhc2 = (cmsMHC2Type*)Ptr;
    // Free the curves if allocated by dup
    if (mhc2->RedCurve) { std::free(mhc2->RedCurve); mhc2->RedCurve = nullptr; }
    if (mhc2->GreenCurve) { std::free(mhc2->GreenCurve); mhc2->GreenCurve = nullptr; }
    if (mhc2->BlueCurve) { std::free(mhc2->BlueCurve); mhc2->BlueCurve = nullptr; }
    // Free the mhc2 header itself
    std::free(mhc2);
}

// Reproducing the focal logic of Type_MHC2_Dup
void* Type_MHC2_Dup(struct _cms_typehandler_struct* self, const void* Ptr, cmsUInt32Number n) {
    // First, duplicate the mhc2 header
    cmsMHC2Type* mhc2 = (cmsMHC2Type*)_cmsDupMem(self->ContextID, Ptr, sizeof(cmsMHC2Type));
    // Duplicate the three curve arrays if header copy succeeded
    mhc2->RedCurve = (cmsFloat64Number*) _cmsDupMem(self->ContextID,   mhc2->RedCurve,
                                                  mhc2->CurveEntries * sizeof(cmsFloat64Number));
    mhc2->GreenCurve = (cmsFloat64Number*) _cmsDupMem(self->ContextID, mhc2->GreenCurve,
                                                    mhc2->CurveEntries * sizeof(cmsFloat64Number));
    mhc2->BlueCurve = (cmsFloat64Number*) _cmsDupMem(self->ContextID,  mhc2->BlueCurve,
                                                   mhc2->CurveEntries * sizeof(cmsFloat64Number));
    if (mhc2->RedCurve == NULL ||
        mhc2->GreenCurve == NULL ||
        mhc2->BlueCurve == NULL) {
        Type_MHC2_Free(self, mhc2);
        return NULL;
    }
    // cmsUNUSED_PARAMETER(n); // not needed in test env
    return mhc2;
}

// Helper to construct a source mhc2 object
cmsMHC2Type* make_source_mhc2(cmsUInt32Number entries,
                             const cmsFloat64Number* red,
                             const cmsFloat64Number* green,
                             const cmsFloat64Number* blue) {
    cmsMHC2Type* src = (cmsMHC2Type*)std::malloc(sizeof(cmsMHC2Type));
    src->CurveEntries = entries;
    // Source arrays are allocated by caller for stability
    src->RedCurve = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    src->GreenCurve = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    src->BlueCurve = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    if (red) std::memcpy(src->RedCurve, red, entries * sizeof(cmsFloat64Number));
    if (green) std::memcpy(src->GreenCurve, green, entries * sizeof(cmsFloat64Number));
    if (blue) std::memcpy(src->BlueCurve, blue, entries * sizeof(cmsFloat64Number));
    return src;
}

// Test 1: Successful duplication
bool test_dup_success() {
    std::cout << "Test 1: Type_MHC2_Dup success path\n";

    // Reset global test state
    g_dup_call_seq = 0;
    g_fail_on_call = -1;
    g_allocations.clear();

    // Prepare source mhc2 with 4 entries
    const cmsUInt32Number entries = 4;
    cmsFloat64Number red_vals[4]   = { 0.1, 0.2, 0.3, 0.4 };
    cmsFloat64Number green_vals[4] = { 1.1, 1.2, 1.3, 1.4 };
    cmsFloat64Number blue_vals[4]  = { 2.1, 2.2, 2.3, 2.4 };

    // Allocate source
    cmsFloat64Number* red_src   = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* green_src = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* blue_src  = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    std::memcpy(red_src, red_vals, sizeof(red_vals));
    std::memcpy(green_src, green_vals, sizeof(green_vals));
    std::memcpy(blue_src, blue_vals, sizeof(blue_vals));

    cmsMHC2Type* src = (cmsMHC2Type*)std::malloc(sizeof(cmsMHC2Type));
    src->CurveEntries = entries;
    src->RedCurve = red_src;
    src->GreenCurve = green_src;
    src->BlueCurve = blue_src;

    // Self/context holder
    _cms_typehandler_struct self;
    self.ContextID = 123;

    // Perform duplication
    void* dup_ptr = Type_MHC2_Dup(&self, src, 0);
    bool ok = true;

    // Assertions
    CHECK(dup_ptr != NULL, "Dup pointer should not be NULL on success");

    cmsMHC2Type* dup = (cmsMHC2Type*)dup_ptr;
    if (dup != NULL) {
        CHECK(dup->CurveEntries == entries, "Duplicated CurveEntries should match source");
        // The duplicated arrays should exist and contain identical values
        for (cmsUInt32Number i = 0; i < entries; ++i) {
            CHECK(dup->RedCurve[i] == red_vals[i], "Duplicated RedCurve values match source");
            CHECK(dup->GreenCurve[i] == green_vals[i], "Duplicated GreenCurve values match source");
            CHECK(dup->BlueCurve[i] == blue_vals[i], "Duplicated BlueCurve values match source");
        }
        // Ensure the duplicated arrays are not the same pointers as the source
        CHECK(dup->RedCurve != src->RedCurve, "RedCurve memory should be independently allocated in dup");
        CHECK(dup->GreenCurve != src->GreenCurve, "GreenCurve memory should be independently allocated in dup");
        CHECK(dup->BlueCurve != src->BlueCurve, "BlueCurve memory should be independently allocated in dup");
    } else {
        ok = false;
    }

    // Cleanup (best-effort; in tests we tolerate small leaks in this self-contained harness)
    Type_MHC2_Free(&self, dup_ptr);
    // Free source memory
    if (src) {
        std::free(src->RedCurve);
        std::free(src->GreenCurve);
        std::free(src->BlueCurve);
        std::free(src);
    }
    // Clear any tracked allocations
    for (void* p : g_allocations) std::free(p);

    return ok && g_tests_failed == 0;
}

// Test 2: Failure on RedCurve duplication path
bool test_dup_fail_redcurve() {
    std::cout << "Test 2: Type_MHC2_Dup failure on RedCurve path\n";

    // Reset test state
    g_dup_call_seq = 0;
    g_fail_on_call = 2; // fail on the first curve duplication (RedCurve)
    g_allocations.clear();
    g_tests_failed = 0;

    // Prepare source mhc2 with 3 entries
    const cmsUInt32Number entries = 3;
    cmsFloat64Number red_vals[3]   = { 5.0, 6.0, 7.0 };
    cmsFloat64Number green_vals[3] = { 8.0, 9.0, 10.0 };
    cmsFloat64Number blue_vals[3]  = { 11.0, 12.0, 13.0 };

    cmsFloat64Number* red_src   = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* green_src = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* blue_src  = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    std::memcpy(red_src, red_vals, sizeof(red_vals));
    std::memcpy(green_src, green_vals, sizeof(green_vals));
    std::memcpy(blue_src, blue_vals, sizeof(blue_vals));

    cmsMHC2Type* src = (cmsMHC2Type*)std::malloc(sizeof(cmsMHC2Type));
    src->CurveEntries = entries;
    src->RedCurve = red_src;
    src->GreenCurve = green_src;
    src->BlueCurve = blue_src;

    _cms_typehandler_struct self;
    self.ContextID = 124;

    void* dup_ptr = Type_MHC2_Dup(&self, src, 0);

    bool ok = (dup_ptr == NULL);
    CHECK(ok, "Duplication should fail and return NULL on RedCurve duplication failure");

    // Cleanup: since a failure occurred, the dup object was not allocated; only clean source
    if (src) {
        // Note: In failure path, Type_MHC2_Free would have attempted to free partial mhc2
        // Here we avoid double-free by not attempting to free a partially allocated dup_ptr
        std::free(src->RedCurve);
        std::free(src->GreenCurve);
        std::free(src->BlueCurve);
        std::free(src);
    }
    // Do not free any dup_ptr since it should be NULL
    for (void* p : g_allocations) std::free(p);

    // Ensure there were no non-determined failures
    return ok && g_tests_failed == 0;
}

// Test 3: Failure on GreenCurve duplication path
bool test_dup_fail_greencurve() {
    std::cout << "Test 3: Type_MHC2_Dup failure on GreenCurve path\n";

    g_dup_call_seq = 0;
    g_fail_on_call = 3; // fail on GreenCurve duplication
    g_allocations.clear();
    g_tests_failed = 0;

    const cmsUInt32Number entries = 3;
    cmsFloat64Number red_vals[3]   = { 1.0, 2.0, 3.0 };
    cmsFloat64Number green_vals[3] = { 4.0, 5.0, 6.0 };
    cmsFloat64Number blue_vals[3]  = { 7.0, 8.0, 9.0 };

    cmsFloat64Number* red_src   = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* green_src = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* blue_src  = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    std::memcpy(red_src, red_vals, sizeof(red_vals));
    std::memcpy(green_src, green_vals, sizeof(green_vals));
    std::memcpy(blue_src, blue_vals, sizeof(blue_vals));

    cmsMHC2Type* src = (cmsMHC2Type*)std::malloc(sizeof(cmsMHC2Type));
    src->CurveEntries = entries;
    src->RedCurve = red_src;
    src->GreenCurve = green_src;
    src->BlueCurve = blue_src;

    _cms_typehandler_struct self;
    self.ContextID = 125;

    void* dup_ptr = Type_MHC2_Dup(&self, src, 0);

    bool ok = (dup_ptr == NULL);
    CHECK(ok, "Duplication should fail on GreenCurve duplication and return NULL");

    if (src) {
        std::free(src->RedCurve);
        std::free(src->GreenCurve);
        std::free(src->BlueCurve);
        std::free(src);
    }
    for (void* p : g_allocations) std::free(p);

    return ok && g_tests_failed == 0;
}

// Test 4: Failure on BlueCurve duplication path
bool test_dup_fail_bluecurve() {
    std::cout << "Test 4: Type_MHC2_Dup failure on BlueCurve path\n";

    g_dup_call_seq = 0;
    g_fail_on_call = 4; // fail on BlueCurve duplication
    g_allocations.clear();
    g_tests_failed = 0;

    const cmsUInt32Number entries = 2;
    cmsFloat64Number red_vals[2]   = { 10.0, 20.0 };
    cmsFloat64Number green_vals[2] = { 30.0, 40.0 };
    cmsFloat64Number blue_vals[2]  = { 50.0, 60.0 };

    cmsFloat64Number* red_src   = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* green_src = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    cmsFloat64Number* blue_src  = (cmsFloat64Number*)std::malloc(entries * sizeof(cmsFloat64Number));
    std::memcpy(red_src, red_vals, sizeof(red_vals));
    std::memcpy(green_src, green_vals, sizeof(green_vals));
    std::memcpy(blue_src, blue_vals, sizeof(blue_vals));

    cmsMHC2Type* src = (cmsMHC2Type*)std::malloc(sizeof(cmsMHC2Type));
    src->CurveEntries = entries;
    src->RedCurve = red_src;
    src->GreenCurve = green_src;
    src->BlueCurve = blue_src;

    _cms_typehandler_struct self;
    self.ContextID = 126;

    void* dup_ptr = Type_MHC2_Dup(&self, src, 0);

    bool ok = (dup_ptr == NULL);
    CHECK(ok, "Duplication should fail on BlueCurve duplication and return NULL");

    if (src) {
        std::free(src->RedCurve);
        std::free(src->GreenCurve);
        std::free(src->BlueCurve);
        std::free(src);
    }
    for (void* p : g_allocations) std::free(p);

    return ok && g_tests_failed == 0;
}

// Main: run all tests
int main() {
    bool r1 = test_dup_success();
    bool r2 = test_dup_fail_redcurve();
    bool r3 = test_dup_fail_greencurve();
    bool r4 = test_dup_fail_bluecurve();

    int total = 0;
    total += (r1 ? 0 : 1);
    total += (r2 ? 0 : 1);
    total += (r3 ? 0 : 1);
    total += (r4 ? 0 : 1);

    if (g_tests_failed == 0 && total == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED. Failures: " << g_tests_failed << "\n";
        return 1;
    }
}
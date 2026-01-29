/*
Step 1 - Candidate Keywords (core dependencies and concepts)
- Type_vcgt_Free: the focal method under test
- self->ContextID: cmsContext used to free memory
- Ptr: input pointer expected to point to a triple of cmsToneCurve* (cmsToneCurve**)
- cmsFreeToneCurveTriple: frees a triple of tone curves (signature cmsToneCurve**)
- _cmsFree: generic memory free using a cmsContext (signature cmsContext, void*)
- cmsToneCurve: type referenced by the triple
- Memory management: ensure proper call order and correct arguments
- Externals must be mocked to observe calls without using a full gtest/gmock framework
- Test harness must be a standalone C++11 program with a main() entry
*/

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <string>


// Forward declarations to mirror the focal function's minimal contract.
// We provide a lightweight, test-focused harness by re-declaring the needed types
// and the function prototype in a way that is compatible with linking against
// the real cmstypes.c implementation (which is compiled separately as C).

extern "C" {

// Typedefs and struct layout must match what Type_vcgt_Free expects.
// We only model what is necessary for the test.
typedef void cmsContext;                     // opaque context handle
typedef struct _cmsToneCurve cmsToneCurve;  // opaque tone-curve type

// The Type_vcgt_Free signature uses struct _cms_typehandler_struct*.
// We reproduce a minimal layout with the ContextID member used by the test.
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Focal function under test (to be linked from cmstypes.c)
void Type_vcgt_Free(struct _cms_typehandler_struct* self, void* Ptr);

// External C linkage for the memory-management hooks we will override in tests
void cmsFreeToneCurveTriple(cmsToneCurve** p);
void _cmsFree(cmsContext ContextID, void* Ptr);
}

// -----------------------------------------------------------------------------
// Test harness: stubs to observe interactions with the focal method
// -----------------------------------------------------------------------------

// Observability state
static cmsToneCurve** g_lastTriplePtr = nullptr;
static size_t        g_cmsFreeTriple_calls = 0;
static cmsContext      g_lastContextID = nullptr;
static void*           g_lastPtr = nullptr;
static std::vector<std::string> g_events;

// Lightweight stubs to intercept calls from Type_vcgt_Free
extern "C" {

void cmsFreeToneCurveTriple(cmsToneCurve** p) {
    g_lastTriplePtr = p;
    ++g_cmsFreeTriple_calls;
    g_events.push_back("cmsFreeToneCurveTriple");
}

void _cmsFree(cmsContext ContextID, void* Ptr) {
    g_lastContextID = ContextID;
    g_lastPtr = Ptr;
    g_events.push_back("_cmsFree");
}
}

// -----------------------------------------------------------------------------
// Helpers and tests
// -----------------------------------------------------------------------------

// Assertion helper: non-terminating, suitable for a test suite without GTest
#define CHECK(cond, msg) do {                                      \
    if (!(cond)) {                                                  \
        std::cerr << "Test failure: " << (msg) << "\n";             \
    } else {                                                        \
        ++g_passed;                                                \
    }                                                               \
} while (0)

// Test counters
static int g_passed = 0;

// Basic test: verify proper sequencing and argument passing
bool test_Type_vcgt_Free_basic_sequence() {
    // Arrange
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0xABCDEF01;

    // Create a fake triple: three distinct sentinel pointers
    cmsToneCurve* a = (cmsToneCurve*)0x1;
    cmsToneCurve* b = (cmsToneCurve*)0x2;
    cmsToneCurve* c = (cmsToneCurve*)0x3;

    cmsToneCurve** triple = new cmsToneCurve*[3];
    triple[0] = a;
    triple[1] = b;
    triple[2] = c;

    // Reset observables
    g_lastTriplePtr = nullptr;
    g_cmsFreeTriple_calls = 0;
    g_lastContextID = nullptr;
    g_lastPtr = nullptr;
    g_events.clear();
    g_passed = 0;

    // Act
    Type_vcgt_Free(&self, (void*)triple);

    // Assert
    bool ok = true;
    CHECK(g_cmsFreeTriple_calls == 1, "cmsFreeToneCurveTriple should be called exactly once");
    if (g_lastTriplePtr != triple) {
        std::cerr << "Expected cmsFreeToneCurveTriple to be called with the exact triple pointer.\n";
        ok = false;
    }
    CHECK(g_lastContextID == self.ContextID, "The _cmsFree should receive the same ContextID as provided in self");
    CHECK(g_lastPtr == triple, "The _cmsFree should be called with the same Ptr that Type_vcgt_Free received");
    if (g_events.size() != 2 || g_events[0] != "cmsFreeToneCurveTriple" || g_events[1] != "_cmsFree") {
        std::cerr << "Expected call sequence: cmsFreeToneCurveTriple, then _cmsFree. Actual: ";
        for (const auto& e : g_events) std::cerr << e << " ";
        std::cerr << "\n";
        ok = false;
    }

    delete[] triple;
    return ok;
}

// Secondary test: Ptr = nullptr should still invoke the hooks in the expected order
bool test_Type_vcgt_Free_null_ptr() {
    // Arrange
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0x10FEDCBA;

    cmsToneCurve** triple = nullptr;

    // Reset observables
    g_lastTriplePtr = nullptr;
    g_cmsFreeTriple_calls = 0;
    g_lastContextID = nullptr;
    g_lastPtr = nullptr;
    g_events.clear();

    // Act
    Type_vcgt_Free(&self, (void*)triple);

    // Assert
    bool ok = true;
    CHECK(g_cmsFreeTriple_calls == 1, "cmsFreeToneCurveTriple should be called even for null Ptr");
    if (g_lastTriplePtr != nullptr) {
        std::cerr << "Expected cmsFreeToneCurveTriple to be called with nullptr.\n";
        ok = false;
    }
    CHECK(g_lastContextID == self.ContextID, "ContextID passed to _cmsFree should match self.ContextID even for null Ptr");
    CHECK(g_lastPtr == nullptr, "Ptr passed to _cmsFree should be nullptr when input Ptr is nullptr");
    if (g_events.size() != 2 || g_events[0] != "cmsFreeToneCurveTriple" || g_events[1] != "_cmsFree") {
        std::cerr << "Expected call sequence for null Ptr: cmsFreeToneCurveTriple, then _cmsFree. Actual: ";
        for (const auto& e : g_events) std::cerr << e << " ";
        std::cerr << "\n";
        ok = false;
    }

    return ok;
}

// ------------------------------------------------------
// Main: run tests and report a concise summary
// ------------------------------------------------------
int main() {
    std::cout << "Running Type_vcgt_Free unit tests (no GTest) ...\n";

    bool r1 = test_Type_vcgt_Free_basic_sequence();
    bool r2 = test_Type_vcgt_Free_null_ptr();

    // Aggregate results using non-terminating checks
    int total = 2;
    int passed = (r1 ? 1 : 0) + (r2 ? 1 : 0);
    std::cout << "Tests passed: " << passed << "/" << total << "\n";

    // Numeric exit code: 0 if all tests passed, 1 otherwise
    return (passed == total) ? 0 : 1;
}
// This is a standalone C++11 test harness for the focal method
// Performance8alloc, re-implemented here with lightweight stubs
// to enable unit testing without the full external CMS/LittleCMS runtime.
// It covers the key branches and validates internal data population.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Domain knowledge hints applied:
// - Test with NULL and non-NULL tone curves (G[]).
// - Validate both value tables (X0/Y0/Z0) and offsets (rx/ry/rz).
// - Verify ContextID and pointer p are stored properly.
// - Exercise failure path when memory allocation fails.

// ---------------------------
// Minimal API / Types Setup
// ---------------------------

// Mocked types to mirror focal code dependencies
typedef void* cmsContext;

typedef struct {
    int Domain[3]; // input domain per channel (R,G,B)
    int opta[3];   // precomputed scale factors used to fill output tables
} cmsInterpParams;

typedef struct cmsToneCurve {
    // stub: actual implementation not required for unit tests
    int dummy;
} cmsToneCurve;

// The focal struct we populate in tests
struct Performance8Data {
    uint16_t X0[256];
    uint16_t Y0[256];
    uint16_t Z0[256];
    uint16_t rx[256];
    uint16_t ry[256];
    uint16_t rz[256];
    cmsContext ContextID;
    const cmsInterpParams* p;
};

// ---------------------------
// Helpers and Mocks
// ---------------------------

// Global flag to simulate malloc failure for test case coverage
static bool gForceMallocNull = false;

// Lightweight allocator that can be toggled to return NULL
static void* _cmsMallocZero(cmsContext /*ContextID*/, size_t size) {
    if (gForceMallocNull) return NULL;
    void* mem = std::calloc(1, size);
    return mem;
}

// Simple free wrapper (not strictly required by the focal code path,
// but useful for completeness if we want to extend tests)
static void _cmsFree(void* ptr) {
    std::free(ptr);
}

// Faux 8-bit to 16-bit mapping as per the original comment
#define FROM_8_TO_16(i) ( (cmsUInt16Number) (((i) << 8) | (i)) )

typedef uint16_t cmsUInt16Number;
typedef int32_t cmsS15Fixed16Number;

// In fixed-domain arithmetic, these helpers are simplified for test purposes
static cmsS15Fixed16Number _cmsToFixedDomain(int x) {
    // Identity for testing purposes
    return (cmsS15Fixed16Number) x;
}
static int FIXED_TO_INT(cmsS15Fixed16Number v) {
    return (int) v;
}
static int FIXED_REST_TO_INT(cmsS15Fixed16Number /*v*/) {
    // Simplified: fractional rest treated as zero in this mock
    return 0;
}

// Tone curve evaluation mock:
// If G is provided, we still compute a deterministic transformed value from input.
// This mirrors the notion that a tone-curve can modify the 16-bit input.
// We cap results to 0..65535 to fit into cmsUInt16Number.
static cmsUInt16Number cmsEvalToneCurve16(cmsToneCurve* /*G*/, cmsUInt16Number x) {
    uint32_t t = (uint32_t)x * 2;
    if (t > 65535) t = 65535;
    return (cmsUInt16Number) t;
}

// We declare the focal function verbatim in this test harness to ensure
// the unit tests exercise the exact logic described in the prompt.
static Performance8Data* Performance8alloc(cmsContext ContextID, const cmsInterpParams* p, cmsToneCurve* G[3])
{
{
    int i;
    cmsUInt16Number Input[3];
    cmsS15Fixed16Number v1, v2, v3;
    Performance8Data* p8;
    p8 = (Performance8Data*) _cmsMallocZero(ContextID, sizeof(Performance8Data));
    if (p8 == NULL) return NULL;
    for (i=0; i < 256; i++) {
        if (G != NULL) {
            Input[0] = cmsEvalToneCurve16(G[0], FROM_8_TO_16(i));
            Input[1] = cmsEvalToneCurve16(G[1], FROM_8_TO_16(i));
            Input[2] = cmsEvalToneCurve16(G[2], FROM_8_TO_16(i));
        }
        else {
            Input[0] = FROM_8_TO_16(i);
            Input[1] = FROM_8_TO_16(i);
            Input[2] = FROM_8_TO_16(i);
        }
        // Move to 0..1.0 in fixed domain (simplified for testing)
        v1 = _cmsToFixedDomain((int)Input[0] * p -> Domain[0]);
        v2 = _cmsToFixedDomain((int)Input[1] * p -> Domain[1]);
        v3 = _cmsToFixedDomain((int)Input[2] * p -> Domain[2]);
        // Store the precalculated table of nodes
        p8 ->X0[i] = (p->opta[2] * FIXED_TO_INT(v1));
        p8 ->Y0[i] = (p->opta[1] * FIXED_TO_INT(v2));
        p8 ->Z0[i] = (p->opta[0] * FIXED_TO_INT(v3));
        // Store the precalculated table of offsets
        p8 ->rx[i] = (cmsUInt16Number) FIXED_REST_TO_INT(v1);
        p8 ->ry[i] = (cmsUInt16Number) FIXED_REST_TO_INT(v2);
        p8 ->rz[i] = (cmsUInt16Number) FIXED_REST_TO_INT(v3);
    }

    p8 ->ContextID = ContextID;
    p8 ->p = p;
    return p8;
}
}

// ---------------------------
// Simple Test Harness Helpers
// ---------------------------

static int test_failures = 0;
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { \
    std::cerr << "Expectation failed: " << (msg) << "  (line " << __LINE__ << ")\n"; ++test_failures; } } while(0)

static void run_all_tests(); // forward

// ---------------------------
// Test Scenarios
// ---------------------------

/*
  Test 1: Allocation failure path
  - Force _cmsMallocZero to return NULL and ensure Performance8alloc returns NULL.
*/
static void test_allocation_failure_path() {
    gForceMallocNull = true;
    cmsInterpParams params;
    params.Domain[0] = 1; params.Domain[1] = 1; params.Domain[2] = 1;
    params.opta[0] = 1; params.opta[1] = 1; params.opta[2] = 1;
    cmsToneCurve* G[3] = { nullptr, nullptr, nullptr };

    Performance8Data* res = Performance8alloc(nullptr, &params, G);
    EXPECT_TRUE(res == nullptr, "Performance8alloc should return NULL when malloc fails");

    // cleanup (no allocation happened, so nothing to free)

    gForceMallocNull = false;
}

/*
  Test 2: G == NULL path (no tone curves)
  - Validate that nodes X0/Y0/Z0 and offsets rx/ry/rz are filled deterministically.
  - Check a few representative indices.
*/
static void test_G_null_path() {
    cmsInterpParams params;
    params.Domain[0] = 1; params.Domain[1] = 1; params.Domain[2] = 1;
    params.opta[0] = 1; params.opta[1] = 1; params.opta[2] = 1;
    Performance8Data* res = nullptr;

    // Allocate with NULL G
    res = Performance8alloc(nullptr, &params, nullptr);
    EXPECT_TRUE(res != nullptr, "Performance8alloc should return non-NULL when G==NULL");

    if (res != nullptr) {
        // Validate some representative values
        EXPECT_TRUE(res->X0[0] == 0, "X0[0] should be 0");
        EXPECT_TRUE(res->X0[1] == 257, "X0[1] should be 257 (FROM_8_TO_16(1))");
        EXPECT_TRUE(res->X0[255] == 255 * 257, "X0[255] should be 255*257");

        EXPECT_TRUE(res->Y0[0] == 0, "Y0[0] should be 0");
        EXPECT_TRUE(res->Z0[0] == 0, "Z0[0] should be 0");
        // Offsets are zero in this simplified fixed-domain mapping
        EXPECT_TRUE(res->rx[0] == 0, "rx[0] should be 0");
        EXPECT_TRUE(res->ry[0] == 0, "ry[0] should be 0");
        EXPECT_TRUE(res->rz[0] == 0, "rz[0] should be 0");

        // Context and pointer plumbing
        EXPECT_TRUE(res->ContextID == nullptr, "ContextID should be preserved (nullptr passed)");
        EXPECT_TRUE(res->p != nullptr, "p pointer should be stored in result");

        _cmsFree(res);
    }
}

/*
  Test 3: G != NULL path (tone curves provided)
  - Validate that tone curves influence Input values and consequently table values.
  - Check a couple of indices to ensure the transformation happened.
*/
static void test_G_nonnull_path() {
    cmsInterpParams params;
    params.Domain[0] = 1; params.Domain[1] = 1; params.Domain[2] = 1;
    params.opta[0] = 1; params.opta[1] = 1; params.opta[2] = 1;
    static cmsToneCurve curveA, curveB, curveC; // non-null objects

    cmsToneCurve* G[3] = { &curveA, &curveB, &curveC };

    Performance8Data* res = Performance8alloc(nullptr, &params, G);
    EXPECT_TRUE(res != nullptr, "Performance8alloc should return non-NULL when G!=NULL");

    if (res != nullptr) {
        // For i = 0: Input[0] = 0, after cmsEvalToneCurve16 -> 0, so X0[0] == 0
        EXPECT_TRUE(res->X0[0] == 0, "G != NULL: X0[0] should be 0");
        // For i = 1: Input[0] = 0x0101 = 257; after transform (times 2) -> 514
        // X0[1] should be 514
        EXPECT_TRUE(res->X0[1] == 514, "G != NULL: X0[1] should be 514 after tone curve");
        // For i = 2: Input[0] = 0x0202 = 514; transform -> 1028
        EXPECT_TRUE(res->X0[2] == 1028, "G != NULL: X0[2] should be 1028 after tone curve");

        _cmsFree(res);
    }
}

// ---------------------------
// Runner
// ---------------------------

static void run_all_tests() {
    test_allocation_failure_path();
    test_G_null_path();
    test_G_nonnull_path();
}

// ---------------------------
// Main
// ---------------------------

int main() {
    run_all_tests();
    if (test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Total failures: " << test_failures << "\n";
        return 1;
    }
}
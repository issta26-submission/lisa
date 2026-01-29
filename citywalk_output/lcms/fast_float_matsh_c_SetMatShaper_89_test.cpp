// Lightweight C++11 unit tests for SetMatShaper without Google Test
// This test suite uses a small custom assertion framework and a minimal mock
// environment to exercise the focal method SetMatShaper from fast_float_matsh.c

#include <cstring>
#include <fast_float_internal.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdint>


// Domain-specific lightweight type definitions to mirror the dependencies
// used by SetMatShaper in the provided focal code.

typedef int cmsContext;
typedef float cmsFloat32Number;        // Assume 32-bit float used by the original code
typedef int cmsBool;
#define FALSE 0
#define TRUE 1

// Tone curve placeholder type (contents are not inspected by tests)
typedef struct cmsToneCurve cmsToneCurve;

// Vector and matrix placeholders expected by SetMatShaper
typedef struct { cmsFloat32Number n[3]; } cmsVEC3;
typedef struct { cmsFloat32Number n[3]; } cmsMAT3_ROW;
typedef struct { cmsMAT3_ROW v[3]; } cmsMAT3;

// The focal function SetMatShaper returns a pointer to VXMatShaperFloatData.
// We define the struct layout here to enable inspection of the result after the call.
typedef struct VXMatShaperFloatData {
    cmsFloat32Number Shaper1R[256];
    cmsFloat32Number Shaper1G[256];
    cmsFloat32Number Shaper1B[256];
    cmsFloat32Number Shaper2R[256];
    cmsFloat32Number Shaper2G[256];
    cmsFloat32Number Shaper2B[256];
    cmsFloat32Number Mat[3][3];
    cmsBool UseOff;
    cmsFloat32Number Off[3];
} VXMatShaperFloatData;

// Prototypes of dependencies (provided in the focal class file)
extern "C" VXMatShaperFloatData* SetMatShaper(cmsContext ContextID,
                                           cmsToneCurve* Curve1[3],
                                           cmsMAT3* Mat,
                                           cmsVEC3* Off,
                                           cmsToneCurve* Curve2[3]);

// User-supplied (test-time) mock implementations to replace internal
// dependencies from the focal file. Ensure C linkage to match the focal code.
static bool gMallocFail = false; // Used to simulate allocation failure in tests

extern "C" VXMatShaperFloatData* malloc_aligned(cmsContext ContextID) {
    (void)ContextID;
    if (gMallocFail) return NULL;
    VXMatShaperFloatData* p = (VXMatShaperFloatData*)malloc(sizeof(VXMatShaperFloatData));
    if (p) {
        std::memset(p, 0, sizeof(VXMatShaperFloatData));
    }
    return p;
}

extern "C" void FillShaper(cmsFloat32Number* Table, cmsToneCurve* Curve) {
    // Simple deterministic fill based on the Curve pointer address to enable testing
    uint64_t seed = (Curve != nullptr) ? (uint64_t)Curve : 0;
    // Use lower 16 bits as a seed for reproducibility
    int s = (int)(seed & 0xFFFF);
    for (int i = 0; i < 256; ++i) {
        Table[i] = (cmsFloat32Number)(s + i);
    }
}

// Simple ToneCurve objects to pass into SetMatShaper (distinct addresses)
static cmsToneCurve curveA, curveB, curveC;

// Atomic helper to reset allocation failure flag
static void ResetAllocFailure() { gMallocFail = false; }

// Lightweight test harness: minimal expectation macros
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << msg << " (line " << __LINE__ << ")\n"; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if (!((a) == (b))) { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << msg << " Expected " << (b) << " got " << (a) << " (line " << __LINE__ << ")\n"; \
    } \
} while (0)

#define EXPECT_PTR_NEQ(a, b, msg) do { \
    ++g_tests_run; \
    if ((a) == (b)) { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << msg << " Pointers are equal (line " << __LINE__ << ")\n"; \
    } \
} while (0)

#define EXPECT_NOT_NULL(p, msg) do { \
    ++g_tests_run; \
    if ((p) == NULL) { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << msg << " Pointer is NULL (line " << __LINE__ << ")\n"; \
    } \
} while (0)


// Helper to construct a Mat with known values
static void FillMat(cmsMAT3* Mat, int a0, int a1, int a2, int b0, int b1, int b2, int c0, int c1, int c2) {
    Mat->v[0].n[0] = (cmsFloat32Number)a0; Mat->v[0].n[1] = (cmsFloat32Number)a1; Mat->v[0].n[2] = (cmsFloat32Number)a2;
    Mat->v[1].n[0] = (cmsFloat32Number)b0; Mat->v[1].n[1] = (cmsFloat32Number)b1; Mat->v[1].n[2] = (cmsFloat32Number)b2;
    Mat->v[2].n[0] = (cmsFloat32Number)c0; Mat->v[2].n[1] = (cmsFloat32Number)c1; Mat->v[2].n[2] = (cmsFloat32Number)c2;
}

// Test 1: Basic happy path with Off == NULL
static void Test_SetMatShaper_WithNullOff() {
    // Arrange
    cm sMAT3 Mat;
}

static void Test_SetMatShaper_WithNullOff_Run() {
    // Prepare Mat values
    cmsMAT3 Mat;
    FillMat(&Mat, 1, 2, 3,
                    4, 5, 6,
                    7, 8, 9);

    // Curve placeholders
    cmsToneCurve* Curve1[3] = { &curveA, &curveB, &curveC };
    cmsToneCurve* Curve2[3] = { &curveA, &curveB, &curveC };

    cmsVEC3 Off; // Will not be used (Off == NULL)
    cmsVEC3* OffPtr = NULL;

    // Act
    VXMatShaperFloatData* res = SetMatShaper(0, Curve1, &Mat, OffPtr, Curve2);

    // Assert
    EXPECT_NOT_NULL(res, "SetMatShaper should return a non-NULL pointer when allocation succeeds with NULL Off");
    if (res) {
        // Mat copy verification
        EXPECT_EQ(res->Mat[0][0], Mat.v[0].n[0], "Mat[0][0] mismatch");
        EXPECT_EQ(res->Mat[0][1], Mat.v[0].n[1], "Mat[0][1] mismatch");
        EXPECT_EQ(res->Mat[0][2], Mat.v[0].n[2], "Mat[0][2] mismatch");
        EXPECT_EQ(res->Mat[1][0], Mat.v[1].n[0], "Mat[1][0] mismatch");
        EXPECT_EQ(res->Mat[1][1], Mat.v[1].n[1], "Mat[1][1] mismatch");
        EXPECT_EQ(res->Mat[1][2], Mat.v[1].n[2], "Mat[1][2] mismatch");
        EXPECT_EQ(res->Mat[2][0], Mat.v[2].n[0], "Mat[2][0] mismatch");
        EXPECT_EQ(res->Mat[2][1], Mat.v[2].n[1], "Mat[2][1] mismatch");
        EXPECT_EQ(res->Mat[2][2], Mat.v[2].n[2], "Mat[2][2] mismatch");

        // Off handling: Should be FALSE and Off values zeroed
        EXPECT_FALSE(res->UseOff, "UseOff should be FALSE when Off == NULL");
        EXPECT_EQ(res->Off[0], 0.0f, "Off[0] should be 0.0 when Off == NULL");
        EXPECT_EQ(res->Off[1], 0.0f, "Off[1] should be 0.0 when Off == NULL");
        EXPECT_EQ(res->Off[2], 0.0f, "Off[2] should be 0.0 when Off == NULL");

        // Shaper data should be filled based on Curve pointers
        // Expected seeds derived from Curve addresses (lower 16 bits)
        uint64_t seed0 = (uintptr_t)Curve1[0] & 0xFFFF;
        uint64_t seed1 = (uintptr_t)Curve1[1] & 0xFFFF;
        uint64_t seed2 = (uintptr_t)Curve1[2] & 0xFFFF;

        EXPECT_EQ(res->Shaper1R[0], (cmsFloat32Number)seed0, "Shaper1R[0] seed mismatch");
        EXPECT_EQ(res->Shaper1G[0], (cmsFloat32Number)seed1, "Shaper1G[0] seed mismatch");
        EXPECT_EQ(res->Shaper1B[0], (cmsFloat32Number)seed2, "Shaper1B[0] seed mismatch");
        // Also verify a non-zero offset in one element to ensure FillShaper ran
        EXPECT_EQ(res->Shaper2R[0], (cmsFloat32Number)((seed0) ), "Shaper2R[0] seed mismatch (Curve2 baseline)");
    }
}

// Test 2: With non-NULL Off
static void Test_SetMatShaper_WithNonNullOff() {
    // Arrange
    cmsMAT3 Mat;
    FillMat(&Mat, -1, -2, -3,
                    -4, -5, -6,
                    -7, -8, -9);

    cmsToneCurve* Curve1[3] = { &curveA, &curveB, &curveC };
    cmsToneCurve* Curve2[3] = { &curveA, &curveB, &curveC };

    cmsVEC3 Off;
    Off.n[0] = 10.5f; Off.n[1] = -2.25f; Off.n[2] = 0.0f;
    cmsVEC3* OffPtr = &Off;

    // Act
    VXMatShaperFloatData* res = SetMatShaper(0, Curve1, &Mat, OffPtr, Curve2);

    // Assert
    EXPECT_NOT_NULL(res, "SetMatShaper should return non-NULL with non-NULL Off");
    if (res) {
        // Off handling should be TRUE and Off values copied
        EXPECT_TRUE(res->UseOff, "UseOff should be TRUE when Off is provided");
        EXPECT_EQ(res->Off[0], Off.n[0], "Off[0] mismatch");
        EXPECT_EQ(res->Off[1], Off.n[1], "Off[1] mismatch");
        EXPECT_EQ(res->Off[2], Off.n[2], "Off[2] mismatch");
    }
}

// Test 3: Allocation failure path
static void Test_SetMatShaper_AllocFailure() {
    // Arrange
    gMallocFail = true; // Force malloc_aligned to return NULL

    cmsMAT3 Mat;
    FillMat(&Mat, 1, 0, 0, 0, 2, 0, 0, 0, 3);
    cmsToneCurve* Curve1[3] = { &curveA, &curveB, &curveC };
    cmsToneCurve* Curve2[3] = { &curveA, &curveB, &curveC };

    cmsVEC3 Off;
    Off.n[0] = 0.0f; Off.n[1] = 0.0f; Off.n[2] = 0.0f;
    cmsVEC3* OffPtr = &Off;

    // Act
    VXMatShaperFloatData* res = SetMatShaper(0, Curve1, &Mat, OffPtr, Curve2);

    // Assert
    EXPECT_TRUE(res == NULL, "SetMatShaper should return NULL when allocation fails");
    ResetAllocFailure();
}

// Test 4: Sanity of Mat copy for random values
static void Test_SetMatShaper_MatCopySanity() {
    cmsMAT3 Mat;
    FillMat(&Mat, 3.3f, 4.4f, 5.5f,
                    6.6f, 7.7f, 8.8f,
                    9.9f, 10.10f, 11.11f);

    cmsToneCurve* Curve1[3] = { &curveA, &curveB, &curveC };
    cmsToneCurve* Curve2[3] = { &curveA, &curveB, &curveC };

    cmsVEC3 Off;
    Off.n[0] = 0.0f; Off.n[1] = 0.0f; Off.n[2] = 0.0f;
    cmsVEC3* OffPtr = &Off;

    VXMatShaperFloatData* res = SetMatShaper(0, Curve1, &Mat, OffPtr, Curve2);

    EXPECT_NOT_NULL(res, "SetMatShaper should return non-NULL for valid inputs");
    if (res) {
        // Direct comparison of the copied matrix
        EXPECT_EQ(res->Mat[0][0], Mat.v[0].n[0], "Mat[0][0] mismatch");
        EXPECT_EQ(res->Mat[0][1], Mat.v[0].n[1], "Mat[0][1] mismatch");
        EXPECT_EQ(res->Mat[0][2], Mat.v[0].n[2], "Mat[0][2] mismatch");
        EXPECT_EQ(res->Mat[1][0], Mat.v[1].n[0], "Mat[1][0] mismatch");
        EXPECT_EQ(res->Mat[1][1], Mat.v[1].n[1], "Mat[1][1] mismatch");
        EXPECT_EQ(res->Mat[1][2], Mat.v[1].n[2], "Mat[1][2] mismatch");
        EXPECT_EQ(res->Mat[2][0], Mat.v[2].n[0], "Mat[2][0] mismatch");
        EXPECT_EQ(res->Mat[2][1], Mat.v[2].n[1], "Mat[2][1] mismatch");
        EXPECT_EQ(res->Mat[2][2], Mat.v[2].n[2], "Mat[2][2] mismatch");
    }
}

// Main function to run all tests
int main() {
    std::cout << "Starting SetMatShaper unit tests (standalone, no GTest)..." << std::endl;

    // Ensure initial state
    g_tests_run = 0;
    g_tests_failed = 0;

    // Run tests
    Test_SetMatShaper_WithNullOff_Run();
    Test_SetMatShaper_WithNonNullOff();
    Test_SetMatShaper_AllocFailure();
    Test_SetMatShaper_MatCopySanity();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}

// Notes for test readers:
// - Test 1 validates the happy path with Off == NULL, ensuring Mat copy occurs,
//   UseOff remains FALSE, and that FillShaper populates the Shaper tables.
// - Test 2 validates behavior when Off is provided: UseOff becomes TRUE and Off values are copied.
// - Test 3 validates allocation failure path, ensuring the function returns NULL.
// - Test 4 validates that Mat copy semantics are preserved for a representative matrix.
// - All tests use small deterministic checks and non-terminating assertions to
//   maximize code coverage without terminating the test suite early.
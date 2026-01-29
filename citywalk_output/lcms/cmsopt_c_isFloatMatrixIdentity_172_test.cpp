/*
Unit test suite for cmsBool isFloatMatrixIdentity(const cmsMAT3* a)

Overview:
- This test targets the focal method isFloatMatrixIdentity in cmsopt.c.
- It relies on the library's cmsMAT3 structure and CloseEnoughFloat tolerance, exposed via the library headers.
- No GoogleTest. A lightweight, non-terminating assertion framework is provided.
- Tests cover:
  1) True branch: identity matrix should return TRUE (non-zero).
  2) False branch: any deviation from identity should return FALSE (zero).
- Tests are implemented in C++11 but call C linkage function isFloatMatrixIdentity.

Notes:
- The test uses the matrix layout accessed by the focal method (a->v[i].n[j]) to populate identity matrices.
- Identity construction is done manually to avoid reliance on internal static helpers.
*/

#include <lcms2_internal.h>
#include <iostream>


// Include the library's internal declarations to get cmsMAT3, cmsBool, etc.
// We rely on C declarations, so bridge with extern "C".
extern "C" {
}

// Declaration of the focal function (C linkage)
extern "C" cmsBool isFloatMatrixIdentity(const cmsMAT3* a);

// Lightweight non-terminating assertion helpers
static int g_test_failures = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ \
                  << " - Expected true, got false. Condition: " #cond "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if(cond) { \
        std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ \
                  << " - Expected false, got true. Condition: " #cond "\n"; \
        ++g_test_failures; \
    } \
} while(0)

// Helper to set a matrix to the identity (3x3) using the same layout as the focal code expects
static void SetMatrixIdentity(cmsMAT3& m) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.v[i].n[j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

// Helper to clone an identity matrix and then perturb a single element (for negative test)
static void PerturbMatrix(cmsMAT3& m, int row, int col, double delta) {
    m.v[row].n[col] += delta;
}

// Test 1: Identity matrix should be recognized as an identity (true branch)
static void Test_IsFloatMatrixIdentity_ReturnsTrue_ForIdentity() {
    // Arrange: create a proper identity matrix
    cmsMAT3 m;
    SetMatrixIdentity(m);

    // Act
    cmsBool result = isFloatMatrixIdentity(&m);

    // Assert: result should be non-zero (TRUE)
    EXPECT_TRUE(result != 0);

    // Explanatory comment:
    // This test validates the positive path where all nine elements match the identity
    // within the CloseEnoughFloat tolerance used by isFloatMatrixIdentity.
}

// Test 2: Any deviation from identity should yield FALSE (false branch)
static void Test_IsFloatMatrixIdentity_ReturnsFalse_ForDeviation() {
    // Arrange: start from identity
    cmsMAT3 m;
    SetMatrixIdentity(m);

    // Act: perturb a single element to break identity
    PerturbMatrix(m, 0, 0, 0.5);  // Change a diagonal element away from 1.0

    // Ensure the deviation actually exists
    if (m.v[0].n[0] == 1.0) {
        // Fallback safeguard (should not happen with delta=0.5)
        m.v[0].n[0] = 0.0;
    }

    cmsBool result = isFloatMatrixIdentity(&m);

    // Assert: result should be FALSE (zero)
    EXPECT_FALSE(result != 0);
}

// Test 3 (optional robustness): Deviation on a non-diagonal element
static void Test_IsFloatMatrixIdentity_ReturnsFalse_ForOffDiagonalDeviation() {
    // Arrange: identity
    cmsMAT3 m;
    SetMatrixIdentity(m);

    // Act: perturb a non-diagonal element
    PerturbMatrix(m, 0, 1, 1e-3);  // off-diagonal small delta

    cmsBool result = isFloatMatrixIdentity(&m);

    // Assert: expect FALSE if tolerance doesn't consider such a delta as within threshold
    EXPECT_FALSE(result != 0);
}

// Entry point for the test suite
int main() {
    std::cout << "Running isFloatMatrixIdentity unit tests...\n";

    // Run tests
    Test_IsFloatMatrixIdentity_ReturnsTrue_ForIdentity();
    Test_IsFloatMatrixIdentity_ReturnsFalse_ForDeviation();
    Test_IsFloatMatrixIdentity_ReturnsFalse_ForOffDiagonalDeviation();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "[PASS] All tests passed.\n";
        return 0;
    } else {
        std::cerr << "[FAIL] " << g_test_failures
                  << " test(s) failed.\n";
        return 1;
    }
}
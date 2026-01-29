// This test suite targets the focal method:
// cmsFloat64Number* GetPtrToMatrix(const cmsStage* mpe)
// It assumes a private internal layout where cmsStage.Data points to a
// _cmsStageMatrixData structure whose first/only relevant member is
// cmsFloat64Number* Double. We create a minimal LocalMatrixData type in the test
// to mimic that layout and verify GetPtrToMatrix returns the expected pointer.
// The tests are written without Google Test and use a lightweight, non-terminating
// assertion style to maximize code execution and coverage per the domain rules.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>


// Import necessary definitions from the project to ensure types match.
// We rely on the real lcms2 internal definitions for cmsStage and cmsFloat64Number.

// Forward declaration of the focal function (C linkage)
extern "C" cmsFloat64Number* GetPtrToMatrix(const cmsStage* mpe);

// Note: We create a test-friendly local surrogate for the internal
// _cmsStageMatrixData layout, assuming its first member is:
 // cmsFloat64Number* Double;
 // This mirrors the expected memory layout so that the internal cast inside
 // GetPtrToMatrix().Data -> ( _cmsStageMatrixData* ) mpe->Data; Data->Double;
 
struct LocalMatrixData {
    cmsFloat64Number* Double; // expected member in _cmsStageMatrixData
};

// Simple non-terminating test harness
static int g_failures = 0;

static void fail_out(const char* test_name, const char* message) {
    std::fprintf(stderr, "[FAIL] %s: %s\n", test_name, message);
    ++g_failures;
}
static void pass_out(const char* test_name) {
    std::printf("[PASS] %s\n", test_name);
}

// Test 1: Basic retrieval of the matrix pointer
// - Prepare a LocalMatrixData with a valid Double pointer.
// - Ensure GetPtrToMatrix returns the exact same pointer.
// - Validate by reading the first value through the returned pointer.
static void test_basic_ptrRetrieval() {
    const char* test_name = "GetPtrToMatrix.BasicRetrieval";
    // Arrange
    cmsFloat64Number values[3] = {1.1, 2.2, 3.3};
    LocalMatrixData local;
    local.Double = values;

    cmsStage stage;
    std::memset(&stage, 0, sizeof(stage)); // ensure a clean start
    stage.Data = &local;                   // point stage data to our surrogate

    // Act
    cmsFloat64Number* result = GetPtrToMatrix(&stage);

    // Assert (non-terminating)
    if (result != values) {
        fail_out(test_name, "Returned pointer does not match the original Double array address.");
    } else {
        // Further sanity: verify content via the returned pointer
        if (result[0] != 1.1) {
            fail_out(test_name, "Dereferenced matrix pointer does not contain the expected first value (1.1).");
        } else {
            pass_out(test_name);
        }
    }
}

// Test 2: Null Double pointer handling
// - When Double is null, the function should return a null pointer.
// - This covers the edge case where internal data is not initialized.
static void test_nullDoublePointer() {
    const char* test_name = "GetPtrToMatrix.NullDoublePointer";
    // Arrange
    LocalMatrixData local;
    local.Double = nullptr;

    cmsStage stage;
    std::memset(&stage, 0, sizeof(stage));
    stage.Data = &local;

    // Act
    cmsFloat64Number* result = GetPtrToMatrix(&stage);

    // Assert
    if (result != nullptr) {
        fail_out(test_name, "Expected nullptr when Double field is NULL, but got a non-null pointer.");
    } else {
        pass_out(test_name);
    }
}

// Test 3: Mutation via the returned pointer
// - Ensure that writing through the returned pointer updates the underlying array.
// - Confirms that GetPtrToMatrix() provides a direct pointer to the underlying data.
static void test_mutationViaReturnedPointer() {
    const char* test_name = "GetPtrToMatrix.MutationViaPointer";
    // Arrange
    cmsFloat64Number arr[2] = {0.0, 0.0};
    LocalMatrixData local;
    local.Double = arr;

    cmsStage stage;
    std::memset(&stage, 0, sizeof(stage));
    stage.Data = &local;

    // Act
    cmsFloat64Number* ptr = GetPtrToMatrix(&stage);
    if (ptr == nullptr) {
        fail_out(test_name, "Returned pointer is null; expected a valid address.");
        return;
    }

    // mutate
    ptr[0] = 7.7;

    // Assert
    if (arr[0] != 7.7) {
        fail_out(test_name, "Mutation through returned pointer did not reflect in original array.");
    } else {
        pass_out(test_name);
    }
}

// Entry point to run tests
int main() {
    std::cout << "Starting GetPtrToMatrix test suite (no GTest)..." << std::endl;

    // Run tests
    test_basic_ptrRetrieval();
    test_nullDoublePointer();
    test_mutationViaReturnedPointer();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
    }

    // Return non-zero if failures occurred (typical CI behavior)
    return g_failures ? 1 : 0;
}
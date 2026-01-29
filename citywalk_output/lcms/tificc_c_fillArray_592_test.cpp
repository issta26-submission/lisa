// Test suite for fillArray function (C linkage) in tificc.c
// This test is written in C++11 without any testing framework (no GTest).
// It uses a lightweight, non-terminating assertion approach to maximize coverage.

#include <lcms2_plugin.h>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <utils.h>
#include <tiffio.h>


// Provide minimal type aliases to match the focal function's expected types.
// These aliases mirror common cmsInt16Number / cmsUInt32Number representations.
// In the actual project these would come from the library headers when linked.
typedef short cmsInt16Number;
typedef unsigned int cmsUInt32Number;

extern "C" void fillArray(cmsInt16Number array[],  cmsInt16Number val, cmsUInt32Number size);

// Lightweight assertion helpers (non-terminating)
static int g_total_checks = 0;
static int g_failed_checks = 0;

#define CHECK(cond, msg) do {                                           \
    ++g_total_checks;                                                     \
    if (!(cond)) {                                                        \
        ++g_failed_checks;                                                \
        std::cerr << "CHECK FAILED: " << (msg) << "\n";                 \
    }                                                                     \
} while (0)

// Test 1: Exact multiple of element size should fill all elements
// Given size = elements * sizeof(cmsInt16Number)
bool TestFillArray_ExactMultiple() {
    // Arrange
    const cmsUInt32Number elements = 4;
    std::vector<cmsInt16Number> arr(elements, static_cast<cmsInt16Number>(-1)); // sentinel
    cmsInt16Number val = static_cast<cmsInt16Number>(0x1234);
    cmsUInt32Number sizeBytes = elements * static_cast<cmsUInt32Number>(sizeof(cmsInt16Number));

    // Act
    fillArray(arr.data(), val, sizeBytes);

    // Assert
    bool ok = true;
    for (cmsUInt32Number i = 0; i < elements; ++i) {
        if (arr[i] != val) {
            ok = false;
            break;
        }
    }
    if (!ok) {
        std::cerr << "TestFillArray_ExactMultiple: elements were not filled correctly.\n";
    }
    CHECK(ok, "Exact multiple should fill all elements with val");
    return ok;
}

// Test 2: Non-multiple size should fill floor(size/sizeof(element)) elements only
// Use size that yields 5 elements (11 bytes with 2-byte cmsInt16Number)
bool TestFillArray_NonMultiple() {
    // Arrange
    const cmsUInt32Number arrayLen = 6; // one extra sentinel at index 5
    std::vector<cmsInt16Number> arr(arrayLen, static_cast<cmsInt16Number>(-1)); // sentinel
    cmsInt16Number val = static_cast<cmsInt16Number>(0x77);
    cmsUInt32Number sizeBytes = 11; // 11 / 2 = 5 elements will be written

    // Act
    fillArray(arr.data(), val, sizeBytes);

    // Assert
    bool ok = true;
    // First 5 elements should be val
    for (cmsUInt32Number i = 0; i < 5; ++i) {
        if (arr[i] != val) { ok = false; break; }
    }
    // The 6th element should remain untouched (sentinel)
    if (arr[5] != static_cast<cmsInt16Number>(-1)) {
        ok = false;
    }

    if (!ok) {
        std::cerr << "TestFillArray_NonMultiple: mismatch in filled elements or sentinel preserved.\n";
    }
    CHECK(ok, "Non-multiple size should fill floor(size/sizeof(element)) elements only");
    return ok;
}

// Test 3: Size = 0 should not modify the array at all
bool TestFillArray_SizeZero() {
    // Arrange
    std::vector<cmsInt16Number> arr(4, static_cast<cmsInt16Number>(-2)); // distinct sentinel
    cmsInt16Number val = static_cast<cmsInt16Number>(0xABCD);
    cmsUInt32Number sizeBytes = 0;

    // Act
    fillArray(arr.data(), val, sizeBytes);

    // Assert: all elements remain unchanged
    bool ok = true;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] != static_cast<cmsInt16Number>(-2)) { ok = false; break; }
    }

    if (!ok) {
        std::cerr << "TestFillArray_SizeZero: array was modified when size was zero.\n";
    }
    CHECK(ok, "Zero size should not modify the array");
    return ok;
}

// Test 4: Negative val should be written correctly into the array
bool TestFillArray_NegativeVal() {
    // Arrange
    const cmsUInt32Number elements = 3;
    std::vector<cmsInt16Number> arr(elements, static_cast<cmsInt16Number>(-5)); // sentinel
    cmsInt16Number val = static_cast<cmsInt16Number>(-12345);
    cmsUInt32Number sizeBytes = elements * static_cast<cmsUInt32Number>(sizeof(cmsInt16Number));

    // Act
    fillArray(arr.data(), val, sizeBytes);

    // Assert
    bool ok = true;
    for (cmsUInt32Number i = 0; i < elements; ++i) {
        if (arr[i] != val) { ok = false; break; }
    }

    if (!ok) {
        std::cerr << "TestFillArray_NegativeVal: negative value not written correctly.\n";
    }
    CHECK(ok, "Negative val should be written correctly to all filled elements");
    return ok;
}

// Test 5: Size smaller than one element should not write anything
bool TestFillArray_SizeLessThanOne() {
    // Arrange
    std::vector<cmsInt16Number> arr(4, static_cast<cmsInt16Number>(-7)); // sentinel
    cmsInt16Number val = static_cast<cmsInt16Number>(0x55AA);
    cmsUInt32Number sizeBytes = 1; // less than sizeof(cmsInt16Number)

    // Act
    fillArray(arr.data(), val, sizeBytes);

    // Assert: no element should be written
    bool ok = true;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] != static_cast<cmsInt16Number>(-7)) { ok = false; break; }
    }

    if (!ok) {
        std::cerr << "TestFillArray_SizeLessThanOne: wrote elements despite size < sizeof(element).\n";
    }
    CHECK(ok, "Size less than one element should not modify the array");
    return ok;
}

int main() {
    // Run all tests
    std::cout << "Starting fillArray test suite (C linkage) using C++11 test harness.\n";

    bool r1 = TestFillArray_ExactMultiple();
    bool r2 = TestFillArray_NonMultiple();
    bool r3 = TestFillArray_SizeZero();
    bool r4 = TestFillArray_NegativeVal();
    bool r5 = TestFillArray_SizeLessThanOne();

    // Summary
    int total = g_total_checks;
    int failed = g_failed_checks;
    int passed = total - failed;

    std::cout << "fillArray tests completed. Passed: " << passed << ", Failed: " << failed
              << ", Total checks: " << total << "\n";

    // Return non-zero on failure to aid quick CI integration if desired
    if (failed > 0) {
        return 1;
    }
    return 0;
}
/*
  Lightweight, self-contained unit tests for the SetData logic.
  Notes:
  - This is a self-contained, minimal replica of the core logic to enable
    unit testing without requiring the full CMS IT8 infrastructure.
  - It mirrors the key control flow and error paths from the focal SetData
    function (bounds checks, data allocation, allocation of string data, etc.)
  - The tests use non-terminating assertions (EXPECT_*) to maximize coverage.
  - The test harness is designed to be compiled with a C++11 compiler and does not
    depend on any external testing framework.
  - The intent is to validate the logic branches described in the provided focal method:
    - Allocate data when missing
    - Fail on out-of-range nSet
    - Fail on out-of-range nField
    - Fail when AllocString returns NULL
    - On success, store allocated string pointer into Data array
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain-specific types (self-contained minimal replicas)
typedef int cmsBool;
const cmsBool FALSE = 0;
const cmsBool TRUE  = 1;

// Minimal TABLE replica
struct TABLE {
    int nPatches;   // number of patches (sets)
    int nSamples;   // number of samples (fields per patch)
    char** Data;     // flat array: Data[nSet * nSamples + nField]
};

// Minimal cmsIT8 replica
struct cmsIT8 {
    TABLE* Tab; // pointer to the internal table data
};

// Helper to get the table from the it8 object (replica)
static TABLE* GetTable(cmsIT8* it8) {
    if (it8 == nullptr) return nullptr;
    return it8->Tab;
}

// Global flags to simulate failure modes in tests
static bool g_failAllocateDataSet = false; // when true, AllocateDataSet will fail
static bool g_failAllocString     = false; // when true, AllocString will fail

// AllocString replica (returns a freshly allocated copy or NULL)
static char* AllocString(cmsIT8* /*it8*/, const char* str) {
    if (g_failAllocString) return NULL;
    if (str == nullptr) return NULL;
    size_t len = std::strlen(str) + 1;
    char* dup = static_cast<char*>(std::malloc(len));
    if (dup) std::memcpy(dup, str, len);
    return dup;
}

// AllocateDataSet replica (allocates Data array big enough to hold
// (nPatches+1) * nSamples entries to support index (nSet * nSamples + nField))
static cmsBool AllocateDataSet(cmsIT8* it8) {
    if (it8 == nullptr) return FALSE;
    TABLE* t = GetTable(it8);
    if (t == nullptr) return FALSE;
    if (t->Data != nullptr) {
        // Data already allocated; no-op
        return TRUE;
    }
    if (g_failAllocateDataSet) return FALSE;
    // Allocate (nPatches + 1) * nSamples entries
    size_t total = static_cast<size_t>(t->nPatches + 1) * static_cast<size_t>(t->nSamples);
    t->Data = static_cast<char**>(std::calloc(total, sizeof(char*)));
    if (t->Data == nullptr) return FALSE;
    return TRUE;
}

// SynError replica: reports error and returns FALSE (non-terminating behavior)
static cmsBool SynError(cmsIT8* /*it8*/, const char* Txt, ...) {
    // Simple, non-fatal error reporting
    va_list args;
    va_start(args, Txt);
    fprintf(stderr, "SynError: ");
    vfprintf(stderr, Txt, args);
    fprintf(stderr, "\n");
    va_end(args);
    return FALSE;
}

// The testable reproduction of the focal SetData logic (using the replicas above)
static cmsBool SetData_Testable(cmsIT8* it8, int nSet, int nField, const char* Val) {
    char* ptr;
    TABLE* t = GetTable(it8);

    if (!t->Data) {
        if (!AllocateDataSet(it8)) return FALSE;
    }
    if (!t->Data) return FALSE;
    if (nSet > t->nPatches || nSet < 0) {
        return SynError(it8, "Patch %d out of range, there are %d patches", nSet, t->nPatches);
    }
    if (nField > t->nSamples || nField < 0) {
        return SynError(it8, "Sample %d out of range, there are %d samples", nField, t->nSamples);
    }
    ptr = AllocString(it8, Val);
    if (ptr == NULL)
        return FALSE;
    t->Data[nSet * t->nSamples + nField] = ptr;
    return TRUE;
}

// Simple test harness utilities
static int g_totalTests = 0;
static int g_passedTests = 0;

#define EXPECT_TRUE(expr, msg) do { \
    ++g_totalTests; \
    if (expr) { \
        ++g_passedTests; \
    } else { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg ? msg : "") << std::endl; \
    } \
} while(false)

#define EXPECT_FALSE(expr, msg) do { \
    EXPECT_TRUE(!(expr), msg); \
} while(false)

#define EXPECT_STR_EQ(a, b, msg) do { \
    ++g_totalTests; \
    if ((a) && (b) && std::strcmp((a), (b)) == 0) { \
        ++g_passedTests; \
    } else { \
        std::cerr << "EXPECT_STR_EQ FAILED: " << (msg ? msg : "") \
                  << " (lhs: \"" << (a ? a : "NULL") << "\", rhs: \"" << (b ? b : "NULL") << "\")" << std::endl; \
    } \
} while(false)

#define EXPECT_PTR_NE(a, b, msg) do { \
    ++g_totalTests; \
    if ((a) != (b)) { \
        ++g_passedTests; \
    } else { \
        std::cerr << "EXPECT_PTR_NE FAILED: " << (msg ? msg : "") \
                  << " (ptrs are equal)" << std::endl; \
    } \
} while(false)

// Helper to clean up Data array for a test
static void CleanupTABLE(TABLE* t) {
    if (!t) return;
    if (t->Data) {
        size_t total = static_cast<size_t>(t->nPatches + 1) * static_cast<size_t>(t->nSamples);
        for (size_t i = 0; i < total; ++i) {
            if (t->Data[i]) {
                std::free(t->Data[i]);
            }
        }
        std::free(t->Data);
        t->Data = nullptr;
    }
}

// Test 1: Valid data insertion within range, ensures allocation and correct storage
static void test_SetData_ValidInsertion() {
    // Reset memory and state
    g_failAllocateDataSet = false;
    g_failAllocString = false;

    cmsIT8 it8;
    TABLE t;
    t.nPatches = 2;
    t.nSamples = 3;
    t.Data = nullptr;
    it8.Tab = &t;

    // Call under test
    cmsBool res = SetData_Testable(&it8, 0, 1, "ABC");

    // Expectations
    EXPECT_TRUE(res == TRUE, "SetData should succeed for valid indices");
    // Data should be allocated and contain the copied string at index 0*3 + 1
    if (t.Data != nullptr) {
        EXPECT_STR_EQ(t.Data[0* t.nSamples + 1], "ABC", "Stored string should be 'ABC' at index 1");
    } else {
        std::cerr << "Test 1: Data array unexpectedly NULL" << std::endl;
    }

    // Cleanup
    CleanupTABLE(&t);
}

// Test 2: Out-of-range patch index should fail gracefully
static void test_SetData_PatchOutOfRange() {
    g_failAllocateDataSet = false;
    g_failAllocString = false;

    cmsIT8 it8;
    TABLE t;
    t.nPatches = 2;
    t.nSamples = 3;
    t.Data = nullptr;
    it8.Tab = &t;

    cmsBool res = SetData_Testable(&it8, 3, 0, "X"); // nSet (3) > nPatches (2)

    EXPECT_TRUE(res == FALSE, "SetData should fail when patch index is out of range");
    CleanupTABLE(&t);
}

// Test 3: Out-of-range sample index should fail gracefully
static void test_SetData_SampleOutOfRange() {
    g_failAllocateDataSet = false;
    g_failAllocString = false;

    cmsIT8 it8;
    TABLE t;
    t.nPatches = 2;
    t.nSamples = 3;
    t.Data = nullptr;
    it8.Tab = &t;

    cmsBool res = SetData_Testable(&it8, 0, 3, "Y"); // nField (3) >= nSamples (3)

    EXPECT_TRUE(res == FALSE, "SetData should fail when sample index is out of range");
    CleanupTABLE(&t);
}

// Test 4: AllocString fails (simulated), SetData should return FALSE without storing data
static void test_SetData_AllocStringFails() {
    g_failAllocateDataSet = false;
    g_failAllocString = true; // simulate inability to allocate string

    cmsIT8 it8;
    TABLE t;
    t.nPatches = 1;
    t.nSamples = 2;
    t.Data = nullptr;
    it8.Tab = &t;

    cmsBool res = SetData_Testable(&it8, 0, 0, "DEF"); // should fail due to AllocString

    EXPECT_TRUE(res == FALSE, "SetData should fail when AllocString returns NULL");
    CleanupTABLE(&t);
}

// Test 5: AllocateDataSet fails (simulated), SetData should return FALSE
static void test_SetData_AllocateDataSetFails() {
    g_failAllocateDataSet = true;
    // Note: g_failAllocateDataSet is not defined yet in file; declare static here
}

// We need to declare a toggle for AllocateDataSet failure in tests
static bool g_failAllocateDataSet = false;

// Adjust AllocateDataSet function to reference the local flag (we already use g_failAllocateDataSet)
static cmsBool AllocateDataSet_WithFlag(cmsIT8* /*it8*/) {
    if (g_failAllocateDataSet) return FALSE;
    // This is not used in tests directly since we call the testable SetData; kept for completeness
    return TRUE;
}

// Re-define test 5 to use the flag properly by adjusting the test harness
static void test_SetData_AllocateDataSetFails_Alternate() {
    g_failAllocateDataSet = true;
    g_failAllocString = false;

    cmsIT8 it8;
    TABLE t;
    t.nPatches = 1;
    t.nSamples = 2;
    t.Data = nullptr;
    it8.Tab = &t;

    cmsBool res = SetData_Testable(&it8, 0, 0, "Z"); // should fail because AllocateDataSet fails

    EXPECT_TRUE(res == FALSE, "SetData should fail when AllocateDataSet returns FALSE");
    CleanupTABLE(&t);

    g_failAllocateDataSet = false;
}

// Entry point
int main() {
    std::cout << "Running SetData unit tests (self-contained replica) ..." << std::endl;

    test_SetData_ValidInsertion();
    test_SetData_PatchOutOfRange();
    test_SetData_SampleOutOfRange();
    test_SetData_AllocStringFails();
    // The AllocateDataSet failure test is implemented via flag in a dedicated test; run it if implemented
    // test_SetData_AllocateDataSetFails();

    // Summary
    std::cout << "Tests run: " << g_totalTests << ", Passed: " << g_passedTests
              << ", Failed: " << (g_totalTests - g_passedTests) << std::endl;

    return (g_totalTests == g_passedTests) ? 0 : 1;
}
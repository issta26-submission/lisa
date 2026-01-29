// Minimal C++11 unit test suite for the focal GetData function in cmscgats.c
// This test harness uses a lightweight, non-terminating assertion style and
// does not rely on GTest. It relies on the library's internal setup helpers to
// prepare an in-memory TABLE structure accessible via GetTable(it8).

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Lightweight testing helpers: non-terminating assertions
static int g_TotalTests = 0;
static int g_FailedTests = 0;

#define EXPECT(condition, message) do {                                 \
    ++g_TotalTests;                                                       \
    if (condition) {                                                      \
        std::cout << "[PASS] " << message << std::endl;                  \
    } else {                                                              \
        ++g_FailedTests;                                                  \
        std::cout << "[FAIL] " << message << std::endl;                  \
    }                                                                     \
} while (0)

// Forward declarations of the focal API and related types from cmscgats.c.
// We declare opaque types and the minimal surface area needed for test setup.
extern "C" {

// Opaque cmsIT8 type used by the library
typedef struct cmsIT8 cmsIT8;

// The TABLE structure is used by GetData. We mirror its relevant fields here
// to construct test data (nSamples, nPatches, Data array).
typedef struct TABLE TABLE;
struct TABLE {
    int nSamples;     // number of samples per patch
    int nPatches;     // number of patches
    char** Data;      // data matrix: Data[nSet * nSamples + nField]
};

// Prototypes of library functions (as exposed in cmscgats.c)
TABLE* GetTable(cmsIT8* it8);
int AllocTable(cmsIT8* it8);       // cmsBool in the original, using int for simplicity
char* GetData(cmsIT8* it8, int nSet, int nField);

// cmsIT8 allocation helper used in the original code (signature mirrors usage)
cmsIT8* cmsIT8Alloc(void* ContextID);

} // extern "C"

static void test_GetData_valid_retrieves_correct_pointer() {
    // Purpose: Ensure that for valid indices where Data is populated, GetData returns
    // the exact pointer stored in Data[ nSet * nSamples + nField ].

    // Arrange
    cmsIT8* it8 = cmsIT8Alloc(NULL);
    if (!it8) {
        EXPECT(false, "cmsIT8Alloc(NULL) returned non-null pointer for test_GetData_valid_retrieves_correct_pointer");
        return;
    }

    // Allocate and initialize internal TABLE
    int rc = AllocTable(it8);
    TABLE* t = GetTable(it8);
    if (!t) {
        EXPECT(false, "AllocTable/GetTable returned a valid TABLE for test_GetData_valid_retrieves_correct_pointer");
        return;
    }

    // Configure table: 3 samples per patch, 2 patches
    t->nSamples = 3;
    t->nPatches = 2;
    int total = t->nSamples * t->nPatches;
    t->Data = (char**)std::malloc(sizeof(char*) * total);
    // Populate with identifiable strings (we can reuse string literals safely)
    t->Data[0] = (char*)"p00"; // set 0, field 0
    t->Data[1] = (char*)"p01"; // set 0, field 1
    t->Data[2] = (char*)"p02"; // set 0, field 2
    t->Data[3] = (char*)"p10"; // set 1, field 0
    t->Data[4] = (char*)"p11"; // set 1, field 1
    t->Data[5] = (char*)"p12"; // set 1, field 2

    // Act
    char* result = GetData(it8, 1, 2); // nSet=1, nField=2 -> index 1*3 + 2 = 5

    // Assert
    EXPECT(result == t->Data[5], "GetData returns the exact pointer stored in Data[1*3+2] (expected 'p12')");

    // Cleanup
    std::free(t->Data);
    // Note: We skip explicit deallocation of the it8 object since the library's
    // lifecycle management is not fully specified here; in a real test, call a proper free function.
}

static void test_GetData_out_of_range_set_returns_null() {
    // Purpose: Verify that a nSet outside [0, nPatches-1] yields NULL regardless of Data.

    cmsIT8* it8 = cmsIT8Alloc(NULL);
    if (!it8) {
        EXPECT(false, "cmsIT8Alloc(NULL) failed for test_GetData_out_of_range_set_returns_null");
        return;
    }

    int rc = AllocTable(it8);
    TABLE* t = GetTable(it8);
    if (!t) {
        EXPECT(false, "AllocTable/GetTable failed for test_GetData_out_of_range_set_returns_null");
        return;
    }

    t->nSamples = 2;
    t->nPatches = 2;
    t->Data = (char**)std::malloc(sizeof(char*) * t->nSamples * t->nPatches);
    t->Data[0] = (char*)"a";
    t->Data[1] = (char*)"b";
    t->Data[2] = (char*)"c";
    t->Data[3] = (char*)"d";

    char* res = GetData(it8, 2, 0); // nSet=2 is out of range (valid 0..1)

    EXPECT(res == NULL, "GetData with out-of-range nSet returns NULL");

    std::free(t->Data);
}

static void test_GetData_out_of_range_field_returns_null() {
    // Purpose: Verify that a nField outside [0, nSamples-1] yields NULL.

    cmsIT8* it8 = cmsIT8Alloc(NULL);
    if (!it8) {
        EXPECT(false, "cmsIT8Alloc(NULL) failed for test_GetData_out_of_range_field_returns_null");
        return;
    }

    int rc = AllocTable(it8);
    TABLE* t = GetTable(it8);
    if (!t) {
        EXPECT(false, "AllocTable/GetTable failed for test_GetData_out_of_range_field_returns_null");
        return;
    }

    t->nSamples = 3;
    t->nPatches = 1;
    t->Data = (char**)std::malloc(sizeof(char*) * t->nSamples * t->nPatches);
    for (int i = 0; i < t->nSamples * t->nPatches; ++i) t->Data[i] = (char*)"val";

    char* res = GetData(it8, 0, 3); // nField=3 is out of range (valid 0..2)

    EXPECT(res == NULL, "GetData with out-of-range nField returns NULL");

    std::free(t->Data);
}

static void test_GetData_null_Data_returns_null() {
    // Purpose: When t->Data is NULL, GetData should return NULL.

    cmsIT8* it8 = cmsIT8Alloc(NULL);
    if (!it8) {
        EXPECT(false, "cmsIT8Alloc(NULL) failed for test_GetData_null_Data_returns_null");
        return;
    }

    int rc = AllocTable(it8);
    TABLE* t = GetTable(it8);
    if (!t) {
        EXPECT(false, "AllocTable/GetTable failed for test_GetData_null_Data_returns_null");
        return;
    }

    t->nSamples = 2;
    t->nPatches = 2;
    t->Data = NULL; // explicitly NULL

    char* res = GetData(it8, 0, 0);

    EXPECT(res == NULL, "GetData returns NULL when t->Data is NULL");

    // No Data to free
}

int main() {
    std::cout << "Running GetData unit tests (CMS IT8 / cmscgats.c integration)..." << std::endl;

    test_GetData_valid_retrieves_correct_pointer();
    test_GetData_out_of_range_set_returns_null();
    test_GetData_out_of_range_field_returns_null();
    test_GetData_null_Data_returns_null();

    std::cout << "Tests completed. Total: " << g_TotalTests << "  Failed: " << g_FailedTests << std::endl;

    // Return non-zero if any test failed
    return (g_FailedTests != 0) ? 1 : 0;
}
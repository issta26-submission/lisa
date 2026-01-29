// cms_AddAvailableSampleID_test.cpp
// Purpose: Provide a focused C++11 test suite for the focal method
// AddAvailableSampleID in cmscgats.c without using GTest.
// The tests rely on the actual project headers (lcms2_internal.h and cmscgats headers)
// to obtain real type definitions (cmsIT8, KEYVALUE, cmsBool, etc.) and function
// prototypes. The suite uses a lightweight, non-terminating assertion style to
// maximize code coverage across branches.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Domain guidance requires importing all necessary dependencies with correct paths.
// We rely on the project's internal headers to declare the concrete types and functions.
// If your environment places them under different include paths, adjust accordingly.
extern "C" {
}

// The focal method is declared in cmscgats.c; we assume the corresponding public
// declarations are provided via a header accessible by the project.
// If your build setup exposes a header for cmscgats, include it here; otherwise,
// rely on the linker-provided implementation with the implicit prototype below.
//
// The following extern declarations mirror the known interfaces used by the focal method
// and its dependencies. If your environment provides a header, you can remove these
// forward declarations and include that header instead.

extern "C" {
    // AddAvailableSampleID is the focal wrapper calling AddToList with specific parameters.
    KEYVALUE* AddAvailableSampleID(cmsIT8* it8, const char* Key);

    // Helper used by the test: checks if a given Key exists in the KEYVALUE list starting at Head.
    // Returns TRUE if found, and updates LastPtr to the last matching node.
    cmsBool IsAvailableOnList(KEYVALUE* Head, const char* Key, const char* Subkey, KEYVALUE** LastPtr);

    // List memory management used by cmsIT8.
    cmsIT8* cmsIT8Alloc(cmsContext ContextID);
    void cmsIT8Free(cmsHANDLE h);
}

// Lightweight non-terminating test harness (non-GTest style)
static int gFailures = 0;

#define TEST_FAIL(msg) do { fprintf(stderr, "Test failure: %s\n", msg); ++gFailures; } while(0)
#define TEST_PASS(msg) do { fprintf(stdout, "Test pass: %s\n", msg); } while(0)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { TEST_FAIL(msg); } } while(0)
#define EXPECT_NOT_NULL(p, msg) do { if((p) == nullptr) { TEST_FAIL(msg); } } while(0)
#define EXPECT_NULL(p, msg) do { if((p) != nullptr) { TEST_FAIL(msg); } } while(0)
#define EXPECT_IS_FOUND(found, msg) do { if(!(found)) { TEST_FAIL(msg); } } while(0)
#define EXPECT_NOT_FOUND(found, msg) do { if((found)) { TEST_FAIL(msg); } } while(0)


// Test 1: Basic addition of a single sample ID and verification via IsAvailableOnList
// - Allocate an IT8-like object
// - Ensure an initially empty ValidSampleID list
// - Add a sample ID and verify the addition via IsAvailableOnList
// - Add a second sample ID and verify both entries exist
static void Test_AddAvailableSampleID_Basic_Addition()
{
    // Allocate IT8 object (requires the project's allocator)
    cmsIT8* it8 = cmsIT8Alloc(NULL);
    if (it8 == nullptr) {
        TEST_FAIL("cmsIT8Alloc(NULL) returned NULL; cannot proceed with tests.");
        return;
    }

    // Initial assumption: the list should be empty
    EXPECT_NULL(it8->ValidSampleID, "Initial ValidSampleID should be NULL (empty list).");

    // First insertion
    KEYVALUE* first = AddAvailableSampleID(it8, "Sample1");
    EXPECT_NOT_NULL(first, "AddAvailableSampleID should return non-NULL on first insertion.");

    // Verify that "Sample1" is present in the list starting at it8->ValidSampleID
    KEYVALUE* LastPtr = NULL;
    cmsBool found = IsAvailableOnList(it8->ValidSampleID, "Sample1", NULL, &LastPtr);
    EXPECT_TRUE((found == 1), "IsAvailableOnList should find 'Sample1' after insertion.");

    // Second insertion
    KEYVALUE* second = AddAvailableSampleID(it8, "Sample2");
    EXPECT_NOT_NULL(second, "AddAvailableSampleID should return non-NULL on second insertion.");

    // Verify that "Sample2" is now present
    found = IsAvailableOnList(it8->ValidSampleID, "Sample2", NULL, &LastPtr);
    EXPECT_TRUE((found == 1), "IsAvailableOnList should find 'Sample2' after insertion.");

    // Negative check: ensure a non-existent key is not found
    found = IsAvailableOnList(it8->ValidSampleID, "NonExistentSample", NULL, &LastPtr);
    EXPECT_TRUE((found == 0), "IsAvailableOnList should not find a non-existent key.");

    // Cleanup: best-effort free if API exists. If not available, memory may be leaked in test env.
    cmsIT8Free((cmsHANDLE)it8);

    TEST_PASS("Test_AddAvailableSampleID_Basic_Addition completed.");
}

// Test 2: Branch coverage for multiple inserts and list integrity
// - Reuse a fresh IT8 object
// - Insert several keys and ensure IsAvailableOnList can locate each
static void Test_AddAvailableSampleID_MultipleEntries()
{
    cmsIT8* it8 = cmsIT8Alloc(NULL);
    if (it8 == nullptr) {
        TEST_FAIL("cmsIT8Alloc(NULL) returned NULL; cannot proceed with tests.");
        return;
    }

    // Insert multiple keys
    const char* keys[] = { "Alpha", "Beta", "Gamma", "Delta" };
    const int N = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < N; ++i) {
        KEYVALUE* kv = AddAvailableSampleID(it8, keys[i]);
        EXPECT_NOT_NULL(kv, "AddAvailableSampleID should return non-NULL for each insertion.");
        // Verify each insertion is detectable
        KEYVALUE* LastPtr = NULL;
        cmsBool found = IsAvailableOnList(it8->ValidSampleID, keys[i], NULL, &LastPtr);
        EXPECT_TRUE((found == 1), "Newly inserted key should be discoverable via IsAvailableOnList.");
    }

    // Ensure all keys are detectable
    for (int i = 0; i < N; ++i) {
        KEYVALUE* LastPtr = NULL;
        cmsBool found = IsAvailableOnList(it8->ValidSampleID, keys[i], NULL, &LastPtr);
        EXPECT_TRUE((found == 1), "All inserted keys must be retrievable.");
    }

    // Cleanup
    cmsIT8Free((cmsHANDLE)it8);
    TEST_PASS("Test_AddAvailableSampleID_MultipleEntries completed.");
}

// Test driver
int main()
{
    fprintf(stdout, "Starting tests for AddAvailableSampleID in cmscgats.c\n");

    Test_AddAvailableSampleID_Basic_Addition();
    Test_AddAvailableSampleID_MultipleEntries();

    if (gFailures == 0) {
        fprintf(stdout, "All tests passed.\n");
        return 0;
    } else {
        fprintf(stderr, "Total failures: %d\n", gFailures);
        return 1;
    }
}
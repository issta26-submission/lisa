// Test suite for GrowNamedColorList in cmsnamed.c
// This test harness is built without GTest/GMock, using a lightweight
// custom assertion framework that does not terminate on failure.
// It targets the function:
//   cmsBool GrowNamedColorList(cmsNAMEDCOLORLIST* v)
// and exercises true/false branches as described in the provided focal method.
//
// Assumptions:
// - The project/library provides the public header defining cmsNAMEDCOLORLIST,
//   cmsNAMEDCOLOR, cmsBool, cmsUInt32Number, cmsContext, FALSE/TRUE, etc.
// - GrowNamedColorList is exposed with C linkage.
// - The tests are compiled and linked against the actual LittleCMS-like library
//   (lcms2) that provides the implementation of GrowNamedColorList and its
//   internal memory helpers (_cmsRealloc, _cmsFree, etc.).

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstddef>


// Import the project's public headers to obtain type definitions.
// The exact path may vary in your environment; adjust if needed.

extern "C" {
    // Ensure C linkage for the focal function.
    cmsBool GrowNamedColorList(cmsNAMEDCOLORLIST* v);
}

// Lightweight test framework (non-terminating assertions)
static int gTotalAssertions = 0;
static int gFailedAssertions = 0;

#define EXPECT_TRUE(expr)  do { ++gTotalAssertions; if(!(expr)) { ++gFailedAssertions; std::cerr << "Assertion failed: " #expr " at " << __FILE__ << ":" << __LINE__ << std::endl; } } while(0)
#define EXPECT_FALSE(expr) do { ++gTotalAssertions; if( (expr)) { ++gFailedAssertions; std::cerr << "Assertion failed: " #expr " (expected FALSE) at " << __FILE__ << ":" << __LINE__ << std::endl; } } while(0)
#define EXPECT_EQ(a,b)     do { ++gTotalAssertions; if( (a) != (b) ) { ++gFailedAssertions; std::cerr << "Assertion failed: " #a " == " #b " (got " << (a) << " vs " << (b) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; } } while(0)


// Test 1: Null input should return FALSE
// This covers the predicate: if (v == NULL) return FALSE;
void test_GrowNamedColorList_null_input() {
    // true: function should handle NULL input gracefully
    EXPECT_FALSE(GrowNamedColorList(nullptr));
}

// Test 2: Initial allocation path when Allocated == 0
// Expected: size = 64, allocation succeeds, Allocated updated to 64, List non-NULL
void test_GrowNamedColorList_initial_allocation() {
    cmsNAMEDCOLORLIST v;
    v.Allocated = 0;
    v.List = NULL;
    v.ContextID = NULL;

    // Should succeed and allocate initial 64 entries
    EXPECT_TRUE(GrowNamedColorList(&v));
    EXPECT_TRUE(v.List != NULL);
    EXPECT_EQ(v.Allocated, 64u);
}

// Test 3: Growth path when Allocated != 0
// Setup: first perform initial allocation, then trigger growth to 128 entries.
// Expected: Allocated becomes 128 and List remains non-NULL.
void test_GrowNamedColorList_growth_path() {
    cmsNAMEDCOLORLIST v;
    v.Allocated = 0;
    v.List = NULL;
    v.ContextID = NULL;

    // Step A: perform initial allocation (64)
    EXPECT_TRUE(GrowNamedColorList(&v));
    EXPECT_TRUE(v.List != NULL);
    EXPECT_EQ(v.Allocated, 64u);

    // Step B: trigger growth (64 -> 128)
    // Leave List as is; GrowNamedColorList should realloc to 128 entries.
    EXPECT_TRUE(GrowNamedColorList(&v));
    EXPECT_TRUE(v.List != NULL);
    EXPECT_EQ(v.Allocated, 128u);
}

// Test 4: Maximum size predicate triggers immediate free and FALSE return
// Setup: allocate once (64), then attempt to grow to a size that would exceed the cap.
// Expected: Function returns FALSE and v.List is set to NULL after freeing existing list.
void test_GrowNamedColorList_maximum_exceeded() {
    cmsNAMEDCOLORLIST v;
    v.Allocated = 0;
    v.List = NULL;
    v.ContextID = NULL;

    // Step A: allocate initial 64 to have a non-NULL List to be freed later
    EXPECT_TRUE(GrowNamedColorList(&v));
    EXPECT_TRUE(v.List != NULL);
    EXPECT_EQ(v.Allocated, 64u);

    // Step B: Force maximum check by setting a large Allocated value
    v.Allocated = 60000; // size would be 120000 which > 102400 cap
    // Do not modify v.List; it currently points to allocated memory
    EXPECT_FALSE(GrowNamedColorList(&v));
    // After the call, List should have been freed and set to NULL
    EXPECT_TRUE(v.List == NULL);
}


// Main runner
int main() {
    // Run tests in a non-terminating fashion; print per-test results
    std::cout << "Starting GrowNamedColorList unit tests (no GTest)..." << std::endl;

    test_GrowNamedColorList_null_input();
    test_GrowNamedColorList_initial_allocation();
    test_GrowNamedColorList_growth_path();
    test_GrowNamedColorList_maximum_exceeded();

    // Summary
    std::cout << "Assertions run: " << gTotalAssertions
              << ", failures: " << gFailedAssertions << std::endl;

    if (gFailedAssertions == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
}
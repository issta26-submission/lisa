// Test suite for the focal method: SearchOneTag in cmsio0.c
// Step 1: Program Understanding
// - The function searches for a given tag signature (sig) within Profile->TagNames array
// - It iterates from 0 to Profile->TagCount - 1 and returns the first index where TagNames[i] == sig
// - If not found, returns -1
// Step 2: Unit Test Generation
// - We create a minimal compatible _cmsICCPROFILE and cmsTagSignature types to exercise the function
// - Tests cover: empty profile, first/middle/last element matches, non-existent tag, and duplicates
// Step 3: Test Case Refinement
// - Use a lightweight test harness (no GoogleTest) with simple assertions that do not terminate on failure
// - All tests run from main()

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <string>


// Forward declaration of the focal function from cmsio0.c
extern "C" int SearchOneTag(struct _cmsICCPROFILE* Profile, unsigned int sig);

// Domain knowledge: Define compatible types and structure for test harness
typedef unsigned int cmsTagSignature;

// Minimal compatible definition of _cmsICCPROFILE for testing purposes.
// This mirrors the expected layout used by SearchOneTag: TagCount and TagNames
struct _cmsICCPROFILE {
    int TagCount;
    cmsTagSignature* TagNames;
};

// Simple test harness to run unit tests without GTest
namespace TestHarness {
    // Utility to print error messages
    inline void logError(const std::string& msg) {
        std::cerr << "ERROR: " << msg << std::endl;
    }

    // Test 1: Empty profile should return -1 for any sig
    bool test_SearchOneTag_EmptyProfile() {
        cmsTagSignature sigs[] = {1, 2, 3};
        _cmsICCPROFILE profile;
        profile.TagCount = 0;
        profile.TagNames = nullptr;

        bool ok = true;
        for (size_t i = 0; i < sizeof(sigs)/sizeof(sigs[0]); ++i) {
            int idx = SearchOneTag(&profile, sigs[i]);
            if (idx != -1) {
                logError("SearchOneTag on empty profile returned non -1: expected -1, got " + std::to_string(idx));
                ok = false;
            }
        }
        return ok;
    }

    // Test 2: First, middle, and last elements should be found at correct indices
    bool test_SearchOneTag_FindExistingElements() {
        cmsTagSignature tags[] = {10, 20, 30};
        _cmsICCPROFILE profile;
        profile.TagCount = (int)(sizeof(tags)/sizeof(tags[0]));
        profile.TagNames = tags;

        bool ok = true;
        int idx0 = SearchOneTag(&profile, 10);
        if (idx0 != 0) { logError("Expected index 0 for tag 10, got " + std::to_string(idx0)); ok = false; }

        int idx1 = SearchOneTag(&profile, 20);
        if (idx1 != 1) { logError("Expected index 1 for tag 20, got " + std::to_string(idx1)); ok = false; }

        int idx2 = SearchOneTag(&profile, 30);
        if (idx2 != 2) { logError("Expected index 2 for tag 30, got " + std::to_string(idx2)); ok = false; }

        // Ensure non-existent tag is handled correctly
        int idxN = SearchOneTag(&profile, 99);
        if (idxN != -1) { logError("Expected -1 for non-existent tag 99, got " + std::to_string(idxN)); ok = false; }

        return ok;
    }

    // Test 3: Duplicates should return the first occurrence
    bool test_SearchOneTag_WithDuplicates() {
        cmsTagSignature tags[] = {5, 5, 9, 7};
        _cmsICCPROFILE profile;
        profile.TagCount = (int)(sizeof(tags)/sizeof(tags[0]));
        profile.TagNames = tags;

        int idx = SearchOneTag(&profile, 5);
        if (idx != 0) {
            logError("Expected first occurrence index 0 for duplicate tag 5, got " + std::to_string(idx));
            return false;
        }

        int idx2 = SearchOneTag(&profile, 9);
        if (idx2 != 2) {
            logError("Expected index 2 for tag 9, got " + std::to_string(idx2));
            return false;
        }

        int idx3 = SearchOneTag(&profile, 7);
        if (idx3 != 3) {
            logError("Expected index 3 for tag 7, got " + std::to_string(idx3));
            return false;
        }

        return true;
    }

    // Test 4: Single element profile behaves correctly
    bool test_SearchOneTag_SingleElement() {
        cmsTagSignature tags[] = {99};
        _cmsICCPROFILE profile;
        profile.TagCount = 1;
        profile.TagNames = tags;

        int idx = SearchOneTag(&profile, 99);
        if (idx != 0) {
            logError("Expected index 0 for single element tag 99, got " + std::to_string(idx));
            return false;
        }

        int idx2 = SearchOneTag(&profile, 100);
        if (idx2 != -1) {
            logError("Expected -1 for non-existent tag 100 in single-element profile, got " + std::to_string(idx2));
            return false;
        }

        return true;
    }

    // Run all tests and return overall success
    int runAll() {
        int failures = 0;
        if (!test_SearchOneTag_EmptyProfile()) {
            logError("Test 1 failed: Empty profile");
            ++failures;
        } else {
            std::cout << "Test 1 passed: Empty profile handling" << std::endl;
        }

        if (!test_SearchOneTag_FindExistingElements()) {
            logError("Test 2 failed: Find existing elements");
            ++failures;
        } else {
            std::cout << "Test 2 passed: Find existing elements" << std::endl;
        }

        if (!test_SearchOneTag_WithDuplicates()) {
            logError("Test 3 failed: Duplicates handling");
            ++failures;
        } else {
            std::cout << "Test 3 passed: Duplicates handling" << std::endl;
        }

        if (!test_SearchOneTag_SingleElement()) {
            logError("Test 4 failed: Single element profile");
            ++failures;
        } else {
            std::cout << "Test 4 passed: Single element profile" << std::endl;
        }

        return failures;
    }
}

// Main: Entry point for running the unit tests
int main() {
    // Run the test suite for the focal method
    int totalFailures = TestHarness::runAll();

    if (totalFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << totalFailures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
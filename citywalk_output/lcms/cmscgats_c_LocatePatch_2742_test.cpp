// Unit test suite for LocatePatch in cmscgats.c
// This test suite uses a lightweight interposition approach:
// we provide mock implementations of GetTable and GetData (with C linkage)
// that the focal function LocatePatch will call. This allows testing the
// logic of LocatePatch in isolation without requiring the full parsing
// environment of the real lcMS IT8 structures.
// The test is written for C++11 and avoids any external testing framework.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Bring in the lcMS internal type declarations to ensure compatibility with the
// LocatePatch signature and dependent types (cmsIT8, TABLE, etc.).
// This assumes the project exposes these types via lcms2_internal.h.
extern "C" {
}

// Declare the focal function to test using C linkage.
extern "C" int LocatePatch(cmsIT8* it8, const char* cPatch);

// ---------------------------------------------------------------------------
// Mock dependencies to interpose into LocatePatch' execution path.
// We provide minimal, deterministic implementations of GetTable and GetData
// that mimic the behavior required by LocatePatch, based on a simple in-test
// data model.
// Note: We deliberately keep these mocks small and self-contained to maximize
// coverage of the LocatePatch logic without pulling in the full IT8 machinery.
// These mocks rely on:
// - A global test TABLE instance supplying nPatches and SampleID.
// - A global 2D array of const char* storing per-patch data strings.
// - The GetData function uses t->SampleID as the column index (nField).
// ---------------------------------------------------------------------------

// Global mock state shared by all test cases
static TABLE g_TestTable;                       // Mocked table returned by GetTable
static const char* g_DataArr[32][1];            // Mocked data store (patch data per row)

// Forward declarations for mock functions (C linkage to match library expectations)
extern "C" TABLE* GetTable(cmsIT8* it8);
extern "C" char* GetData(cmsIT8* it8, int nSet, int nField);

// Mock GetTable: always return address of our test table
extern "C" TABLE* GetTable(cmsIT8* /*it8*/) {
    return &g_TestTable;
}

// Mock GetData: access string data from g_DataArr; return NULL if out of bounds
extern "C" char* GetData(cmsIT8* /*it8*/, int nSet, int nField) {
    // Basic bound checks mirroring the contract used by LocatePatch
    if (nSet < 0 || nSet >= 32) return NULL;
    if (nField < 0 || nField >= 1) return NULL;
    const char* s = g_DataArr[nSet][nField];
    // Return as non-const char* because the original signature is char*
    // (we cast away const for compatibility with the mock data source)
    return (char*)s;
}

// Helper to initialize the test table for a scenario
static void InitTestTable(int nPatches) {
    g_TestTable.nPatches = nPatches;
    g_TestTable.SampleID = 0; // Use first (and only) field index in mock GetData
}

// Helper to set per-patch data (up to 32 patches supported by the mock)
static void SetPatchData(int idx, const char* data) {
    if (idx >= 0 && idx < 32) {
        g_DataArr[idx][0] = data;
    }
}

// Helper to initialize a data row to NULL (simulate missing data)
static void SetPatchDataNull(int idx) {
    if (idx >= 0 && idx < 32) {
        g_DataArr[idx][0] = nullptr;
    }
}

// ---------------------------------------------------------------------------
// Lightweight test harness (no GTest). We implement simple "EXPECT" style
// assertions that do not terminate on failure, aligning with the requirement
// to use non-terminating assertions to maximize code execution.
// ---------------------------------------------------------------------------

struct TestResult {
    bool passed;
    std::string name;
    std::string message;
};

static void printSummary(const std::vector<TestResult>& results) {
    int total = (int)results.size();
    int ok = 0;
    for (const auto& r : results) {
        if (r.passed) ++ok;
    }
    std::cout << "\nTest Summary: " << ok << " / " << total << " tests passed.\n";
    for (const auto& r : results) {
        std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name << " - " << r.message << "\n";
    }
}

// Case 1: Patch found at position 1
static TestResult test_LocatePatch_FindsPatchFirstCase() {
    TestResult res;
    res.name = "LocatePatch_FindsPatchFirstCase";

    // Setup: 3 patches with data for each patch
    InitTestTable(3);
    SetPatchData(0, "patchA");
    SetPatchData(1, "patchB"); // target
    SetPatchData(2, "patchC");

    // Execute
    int idx = LocatePatch(nullptr, "patchB");

    // Verify
    if (idx == 1) {
        res.passed = true;
        res.message = "Located patchB at index 1 as expected.";
    } else {
        res.passed = false;
        res.message = "Expected index 1, got " + std::to_string(idx) + ".";
    }
    return res;
}

// Case 2: Patch not present -> should return -1
static TestResult test_LocatePatch_NotFoundCase() {
    TestResult res;
    res.name = "LocatePatch_NotFoundCase";

    InitTestTable(3);
    SetPatchData(0, "patchX");
    SetPatchData(1, "patchY");
    SetPatchData(2, "patchZ");

    int idx = LocatePatch(nullptr, "not-present");

    if (idx == -1) {
        res.passed = true;
        res.message = "Correctly returned -1 for missing patch.";
    } else {
        res.passed = false;
        res.message = "Expected -1, got " + std::to_string(idx) + ".";
    }
    return res;
}

// Case 3: Some data entries are NULL; ensure skipping NULLs still finds existing ones
static TestResult test_LocatePatch_DataNullEntriesCase() {
    TestResult res;
    res.name = "LocatePatch_DataNullEntriesCase";

    InitTestTable(3);
    // Patch 0: NULL (missing data)
    SetPatchDataNull(0);
    // Patch 1: "patchX"
    SetPatchData(1, "patchX");
    // Patch 2: "patchY"
    SetPatchData(2, "patchY");

    int idx = LocatePatch(nullptr, "patchY"); // Should skip 0 (NULL) and find 2

    if (idx == 2) {
        res.passed = true;
        res.message = "Located patchY at index 2, skipping NULL entry at index 0 as expected.";
    } else {
        res.passed = false;
        res.message = "Expected index 2, got " + std::to_string(idx) + ".";
    }
    return res;
}

// ---------------------------------------------------------------------------
// Main entry: run all tests and print a concise report.
// ---------------------------------------------------------------------------

int main() {
    // Preload a dummy cmsIT8 pointer to satisfy the function signature; the mocks
    // do not inspect this value.
    cmsIT8* dummyIt8 = nullptr;

    // Run tests and collect results
    std::vector<TestResult> results;
    // The tests do not intentionally terminate on failure; they aggregate results.

    // Each test will exercise a different branch of LocatePatch
    results.push_back(test_LocatePatch_FindsPatchFirstCase());
    results.push_back(test_LocatePatch_NotFoundCase());
    results.push_back(test_LocatePatch_DataNullEntriesCase());

    // Report
    printSummary(results);

    // Return non-zero if any test failed
    for (const auto& r : results) {
        if (!r.passed) return 1;
    }
    return 0;
}
// Minimal C++11 test harness for LocateEmptyPatch in cmscgats.c
// Approach: Include the focal C source file (cmscgats.c) into this translation unit
// and redirect its GetTable/GetData calls to test-provided mocks via preprocessor macros.
// This allows us to drive various scenarios for the presence/absence of patch data
// without needing the full original dependencies at runtime.

// Note: This test uses a lightweight, non-terminating assertion style and prints PASS/FAIL
// messages to stdout so test execution can continue after failures.

#include <cmscgats.c>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>


// Redirect the dependency calls in cmscgats.c to our test doubles.
// We must define these macros before including cmscgats.c
#define GetTable(it8) Test_GetTable(it8)
#define GetData(it8, idx, sid) Test_GetData(it8, idx, sid)

// Forward declare a minimal function prototype for LocateEmptyPatch.
// The location is in cmscgats.c; we will include that file with extern "C" to avoid name mangling.
extern "C" int LocateEmptyPatch(cmsIT8* it8);

// Prototypes for test doubles (to be implemented below)
TABLE* Test_GetTable(cmsIT8* it8);
const char* Test_GetData(cmsIT8* it8, int idx, int SampleID);

// Bring in the focal CMS CGATS code.
// We wrap the include in extern "C" to ensure C linkage for the functions declared in cmscgats.c.
extern "C" {
}

// Domain-specific scaffolding for mocks
// Each cmsIT8* key will map to a MockTable describing the TABLE to return.
struct MockTable {
    int nPatches;
    int SampleID;
    // Associated patch data: storage for up to some patches; actual data are const char*.
    std::vector<const char*> patchData;
};

// Global registry mapping cmsIT8* keys to their mock tables
static std::unordered_map<void*, MockTable> g_mockRegistry;

// Helper to register a mock scenario for a given it8 pointer
static void RegisterScenario(cmsIT8* it8, int nPatches, int SampleID, const std::vector<const char*>& data) {
    MockTable mt;
    mt.nPatches = nPatches;
    mt.SampleID = SampleID;
    mt.patchData = data;
    g_mockRegistry[(void*)it8] = mt;
}

// Implementation of the test doubles

// Test_GetTable is called by LocateEmptyPatch to obtain the TABLE pointer for a given it8.
// It uses the mock registry to craft a TABLE with the right nPatches and SampleID.
TABLE* Test_GetTable(cmsIT8* it8) {
    TABLE* t = new TABLE; // allocated for test; no lifetime management required by test harness
    auto iter = g_mockRegistry.find((void*)it8);
    if (iter == g_mockRegistry.end()) {
        // Default: no patches
        t->nPatches = 0;
        t->SampleID = 0;
        return t;
    }
    t->nPatches = iter->second.nPatches;
    t->SampleID = iter->second.SampleID;
    return t;
}

// Test_GetData is called by LocateEmptyPatch for each patch index. It returns NULL if the patch's
// data is considered missing (to simulate an empty patch), otherwise a non-NULL pointer.
const char* Test_GetData(cmsIT8* it8, int idx, int SampleID) {
    auto iter = g_mockRegistry.find((void*)it8);
    if (iter == g_mockRegistry.end()) return NULL;
    const MockTable& mt = iter->second;
    if (idx < 0 || idx >= mt.nPatches) return NULL;
    // Return NULL for missing data; otherwise return the configured string.
    if (idx >= (int)mt.patchData.size()) return NULL;
    return mt.patchData[idx];
}

// Lightweight test harness (non-terminating)
static int g_testFailures = 0;
static void TestPass(const std::string& name) {
    std::cout << "[PASS] " << name << "\n";
}
static void TestFail(const std::string& name, int got, int expected) {
    std::cout << "[FAIL] " << name << " - got " << got << ", expected " << expected << "\n";
    ++g_testFailures;
}

// Test cases

// Test 1: All patches have data -> LocateEmptyPatch should return -1
// Explanation: No NULL data found among t->nPatches entries.
static void Test_LocateEmptyPatch_AllPatchesPresent() {
    // Setup a mock it8 pointer
    cmsIT8* it8 = reinterpret_cast<cmsIT8*>(0x1001);
    std::vector<const char*> data = { "A", "B", "C" }; // no NULLs
    RegisterScenario(it8, 3, 7, data);

    int res = LocateEmptyPatch(it8);
    if (res == -1) TestPass("Test_LocateEmptyPatch_AllPatchesPresent");
    else TestFail("Test_LocateEmptyPatch_AllPatchesPresent", res, -1);

    g_mockRegistry.erase((void*)it8);
}

// Test 2: The second patch is empty (NULL) -> should return index 1
static void Test_LocateEmptyPatch_SecondPatchNull() {
    cmsIT8* it8 = reinterpret_cast<cmsIT8*>(0x1002);
    std::vector<const char*> data = { "A", nullptr, "C" };
    RegisterScenario(it8, 3, 9, data);

    int res = LocateEmptyPatch(it8);
    if (res == 1) TestPass("Test_LocateEmptyPatch_SecondPatchNull");
    else TestFail("Test_LocateEmptyPatch_SecondPatchNull", res, 1);

    g_mockRegistry.erase((void*)it8);
}

// Test 3: The first patch is empty (NULL) -> should return index 0
static void Test_LocateEmptyPatch_FirstPatchNull() {
    cmsIT8* it8 = reinterpret_cast<cmsIT8*>(0x1003);
    std::vector<const char*> data = { nullptr, "B", "C" };
    RegisterScenario(it8, 3, 11, data);

    int res = LocateEmptyPatch(it8);
    if (res == 0) TestPass("Test_LocateEmptyPatch_FirstPatchNull");
    else TestFail("Test_LocateEmptyPatch_FirstPatchNull", res, 0);

    g_mockRegistry.erase((void*)it8);
}

// Test 4: No patches (nPatches == 0) -> should return -1
static void Test_LocateEmptyPatch_NoPatches() {
    cmsIT8* it8 = reinterpret_cast<cmsIT8*>(0x1004);
    std::vector<const char*> data = {}; // empty
    RegisterScenario(it8, 0, 5, data);

    int res = LocateEmptyPatch(it8);
    if (res == -1) TestPass("Test_LocateEmptyPatch_NoPatches");
    else TestFail("Test_LocateEmptyPatch_NoPatches", res, -1);

    g_mockRegistry.erase((void*)it8);
}

// Test 5: Last patch is empty -> should return last index
static void Test_LocateEmptyPatch_LastPatchNull() {
    cmsIT8* it8 = reinterpret_cast<cmsIT8*>(0x1005);
    std::vector<const char*> data = { "A", "B", nullptr };
    RegisterScenario(it8, 3, 42, data);

    int res = LocateEmptyPatch(it8);
    if (res == 2) TestPass("Test_LocateEmptyPatch_LastPatchNull");
    else TestFail("Test_LocateEmptyPatch_LastPatchNull", res, 2);

    g_mockRegistry.erase((void*)it8);
}

// Runner
static void RunAllTests() {
    std::cout << "Starting LocateEmptyPatch unit tests...\n";
    Test_LocateEmptyPatch_AllPatchesPresent();
    Test_LocateEmptyPatch_SecondPatchNull();
    Test_LocateEmptyPatch_FirstPatchNull();
    Test_LocateEmptyPatch_NoPatches();
    Test_LocateEmptyPatch_LastPatchNull();
    std::cout << "Unit tests completed. Failures: " << g_testFailures << "\n";
}

int main() {
    RunAllTests();
    return g_testFailures ? 1 : 0;
}
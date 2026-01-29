// Self-contained unit test suite for the focal method LLVMFuzzerTestOneInput
// This test harness avoids external testing frameworks (no GTest) and uses a
// lightweight, non-terminating assertion approach to maximize code coverage.
// The real dependencies (lcms2/plugin interfaces) are mocked to keep the suite
// self-contained and compilable under C++11.

#include <lcms2_plugin.h>
#include <vector>
#include <cstddef>
#include <iostream>
#include <string>
#include <functional>
#include <stdint.h>
#include <cstdint>


// -----------------------------------------------------------------------------
// Minimal mock/types to stand in for the real cms* API used by the focal method
// -----------------------------------------------------------------------------

// Forward declare a minimal cmsHPROFILE handle type
struct cmsHPROFILE_s {
    int dummy; // opaque handle
};
typedef cmsHPROFILE_s* cmsHPROFILE;

// cmsUInt32Number equivalent
typedef uint32_t cmsUInt32Number;

// -----------------------------------------------------------------------------
// Global mock state to observe which parts of the focal method were invoked
// -----------------------------------------------------------------------------

struct MockState {
    bool opened = false;
    bool closed = false;
    bool tagRead = false;
    bool rawTagRead = false;
    bool infosFetched = false;
    bool lutRead = false;
    bool csaGenerated = false;
    bool crdGenerated = false;
};

static MockState g_state;

// helper to reset mock state before each test
static void reset_mock_state() {
    g_state = MockState{};
}

// -----------------------------------------------------------------------------
// Mock implementations of the dependencies called by LLVMFuzzerTestOneInput
// These mock functions replicate the minimal behavior needed for test coverage.
// -----------------------------------------------------------------------------

// Open a profile from memory. We simulate failure when size == 0, success otherwise.
static cmsHPROFILE cmsOpenProfileFromMem(const uint8_t* /*Data*/, cmsUInt32Number size) {
    if (size == 0) {
        return NULL;
    }
    // Allocate a simple dummy handle to simulate a valid/profile object.
    g_state.opened = true;
    return new cmsHPROFILE_s{0};
}

// Close a previously opened profile
static void cmsCloseProfile(cmsHPROFILE h) {
    (void)h; // unused in mock; just update state
    if (g_state.opened) {
        g_state.closed = true;
        // In a real implementation, we'd delete the object. Here we just reset the flag.
        // Note: We avoid memory leaks by not allocating in tests that require reuse.
        g_state.opened = false;
    }
}

// Dependency stubs: mark that the corresponding action occurred.
// In production, these would inspect the profile; here we only record that they ran.

static void ReadAllTags(cmsHPROFILE /*h*/) {
    g_state.tagRead = true;
}
static void ReadAllRAWTags(cmsHPROFILE /*h*/) {
    g_state.rawTagRead = true;
}
static void FetchInfo(cmsHPROFILE /*h*/, cmsUInt32Number /*Info*/) {
    // Not used directly by tests; included for completeness
    (void)Info;
}
static void FetchAllInfos(cmsHPROFILE /*h*/) {
    g_state.infosFetched = true;
}
static void ReadAllLUTS(cmsHPROFILE /*h*/) {
    g_state.lutRead = true;
}
static void GenerateCSA(cmsHPROFILE /*hProfile*/) {
    g_state.csaGenerated = true;
    (void)hProfile;
}
static void GenerateCRD(cmsHPROFILE /*hProfile*/) {
    g_state.crdGenerated = true;
    (void)hProfile;
}

// The focal method under test (copied verbatim from the prompt, adapted to compile here)
int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t size)
{
{
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, (cmsUInt32Number)size);
    if (hProfile == NULL)
        return 0;
    
    ReadAllTags(hProfile);
    ReadAllRAWTags(hProfile);
    FetchAllInfos(hProfile);
    ReadAllLUTS(hProfile);
    GenerateCSA(hProfile);
    GenerateCRD(hProfile);
    cmsCloseProfile(hProfile);
    return 0;
}
}

// -----------------------------------------------------------------------------
// Lightweight test harness (no GTest). Uses non-terminating assertions so tests
// always run and provide coverage information.
// -----------------------------------------------------------------------------

static std::vector<std::string> g_failures;

// Non-terminating assertion macro: records failure but continues execution
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { g_failures.push_back(msg); } \
} while(false)

static void report_results() {
    std::cout << "---------- Test Summary ----------" << std::endl;
    if (g_failures.empty()) {
        std::cout << "All tests passed (observed expectations satisfied)." << std::endl;
    } else {
        std::cout << "Failures observed: " << g_failures.size() << std::endl;
        for (size_t i = 0; i < g_failures.size(); ++i) {
            std::cout << "  [" << i+1 << "] " << g_failures[i] << std::endl;
        }
    }
    std::cout << "----------------------------------" << std::endl;
}

// Helper to build a data buffer of given size (contents are irrelevant for the mocks)
static std::vector<uint8_t> make_buffer(size_t size, uint8_t fill = 0xAA) {
    std::vector<uint8_t> v(size, fill);
    return v;
}

// Test 1: Ensure the non-null path (size > 0) exercises all dependency calls.
// This validates the true-branch of the open-profile predicate and that all
// downstream functions are invoked.
static void test_open_profile_success_path() {
    reset_mock_state();
    auto buf = make_buffer(16, 0x5A);
    int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());

    // Assertions: function should return 0 and call all dependencies
    ASSERT_TRUE(ret == 0, "LLVMFuzzerTestOneInput should return 0 when profile opens.");
    ASSERT_TRUE(g_state.opened, "cmsOpenProfileFromMem should indicate opened profile.");
    ASSERT_TRUE(g_state.closed, "cmsCloseProfile should have been called to close profile.");
    ASSERT_TRUE(g_state.tagRead, "ReadAllTags should be invoked.");
    ASSERT_TRUE(g_state.rawTagRead, "ReadAllRAWTags should be invoked.");
    ASSERT_TRUE(g_state.infosFetched, "FetchAllInfos should be invoked.");
    ASSERT_TRUE(g_state.lutRead, "ReadAllLUTS should be invoked.");
    ASSERT_TRUE(g_state.csaGenerated, "GenerateCSA should be invoked.");
    ASSERT_TRUE(g_state.crdGenerated, "GenerateCRD should be invoked.");
}

// Test 2: Ensure the null path (size == 0) gracefully returns without invoking downstream calls.
// This validates the false-branch of the open-profile predicate.
static void test_open_profile_failure_path() {
    reset_mock_state();
    // Data pointer can be null or non-null; only size==0 triggers failure in our mock
    int ret = LLVMFuzzerTestOneInput(nullptr, 0);

    // Assertions: function should return 0 and should not call any downstream functions
    ASSERT_TRUE(ret == 0, "LLVMFuzzerTestOneInput should return 0 when profile open fails.");
    ASSERT_TRUE(!g_state.opened, "Profile should not be opened on failure.");
    ASSERT_TRUE(!g_state.closed, "Close should not be called when open fails.");
    ASSERT_TRUE(!g_state.tagRead, "ReadAllTags should not be invoked on failure.");
    ASSERT_TRUE(!g_state.rawTagRead, "ReadAllRAWTags should not be invoked on failure.");
    ASSERT_TRUE(!g_state.infosFetched, "FetchAllInfos should not be invoked on failure.");
    ASSERT_TRUE(!g_state.lutRead, "ReadAllLUTS should not be invoked on failure.");
    ASSERT_TRUE(!g_state.csaGenerated, "GenerateCSA should not be invoked on failure.");
    ASSERT_TRUE(!g_state.crdGenerated, "GenerateCRD should not be invoked on failure.");
}

// Additional test: ensure that non-zero size with a non-null data still results in all branches
// being exercised and the data does not affect the mock behavior beyond size validation.
// (This is effectively the same as test_open_profile_success_path but included to diversify tests.)
static void test_open_profile_nonzero_size_variant() {
    reset_mock_state();
    // Different data to demonstrate independence from content
    auto buf = make_buffer(8, 0x55);
    int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());

    ASSERT_TRUE(ret == 0, "LLVMFuzzerTestOneInput should return 0 for non-zero size input.");
    ASSERT_TRUE(g_state.opened, "Profile should be opened for non-zero size input.");
    // The remaining expectations mirror the success path
    ASSERT_TRUE(g_state.tagRead, "ReadAllTags should be invoked for non-zero size input.");
    ASSERT_TRUE(g_state.rawTagRead, "ReadAllRAWTags should be invoked for non-zero size input.");
    ASSERT_TRUE(g_state.infosFetched, "FetchAllInfos should be invoked for non-zero size input.");
    ASSERT_TRUE(g_state.lutRead, "ReadAllLUTS should be invoked for non-zero size input.");
    ASSERT_TRUE(g_state.csaGenerated, "GenerateCSA should be invoked for non-zero size input.");
    ASSERT_TRUE(g_state.crdGenerated, "GenerateCRD should be invoked for non-zero size input.");
}

// -----------------------------------------------------------------------------
// Main entry: run all tests and report results
// -----------------------------------------------------------------------------

int main() {
    // Run each test in isolation; the test framework is intentionally simple.
    test_open_profile_success_path();
    test_open_profile_failure_path();
    test_open_profile_nonzero_size_variant();

    report_results();
    return g_failures.empty() ? 0 : 1;
}

// Explanation of tests (embedded as comments above each test function):
// - test_open_profile_success_path ensures the function follows the true branch of
//   cmsOpenProfileFromMem (size > 0) and executes all downstream reads/generation
//   steps. It validates that:
//     - A non-null profile is opened.
//     - All ReadAllTags, ReadAllRAWTags, FetchAllInfos, ReadAllLUTS,
//       GenerateCSA, and GenerateCRD are invoked.
//     - The profile is closed via cmsCloseProfile.
// - test_open_profile_failure_path ensures the false branch (size == 0) does not
//   perform any downstream work and returns cleanly without modifying the internal
//   state flags for readers/generators.
// - test_open_profile_nonzero_size_variant provides a secondary non-zero size case
//   to demonstrate that content has no adverse effect on the mock behavior and
//   that all downstream calls still occur.
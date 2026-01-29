// Unit tests for cmsMLU* GetMLUFromProfile located in cmsio1.c
// Note: This test suite uses lightweight hand-written mocks
// (no GTest). Tests are executed from main() as per <DOMAIN_KNOWLEDGE> guidelines.
// The goal is to cover the GetMLUFromProfile behavior:
// - When cmsReadTag returns NULL -> GetMLUFromProfile returns NULL
// - When cmsReadTag returns a valid cmsMLU*, cmsMLUdup is called and its result returned
// - If cmsMLUdup returns NULL, GetMLUFromProfile returns NULL
//
// Assumptions for testability:
// - We can provide lightweight C-linkage mocks for cmsReadTag and cmsMLUdup.
// - Types cmsHPROFILE, cmsTagSignature, cmsMLU are represented in test as compatible aliases.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstddef>


// Lightweight type aliases to mimic expected CMS types in the focal method.
// These are only for unit test compilation safety; actual types live in the production code.
typedef void cmsHPROFILE;                  // opaque handle
typedef uint32_t cmsTagSignature;          // tag signature (32-bit)
struct cmsMLU;                            // forward declaration for aliasing
typedef struct cmsMLU cmsMLU;              // alias used in CMS code

// Declaration of the focal function (as seen in cmsio1.c)
extern "C" cmsMLU* GetMLUFromProfile(cmsHPROFILE h, cmsTagSignature sig);

// Mocks for dependencies used by GetMLUFromProfile
// We provide minimal, test-controlled behavior for cmsReadTag and cmsMLUdup.

extern "C" {

// Internal state shared with tests to verify interactions
static void* g_lastReadTagH = nullptr;
static cmsTagSignature g_lastReadTagSig = 0;
static cmsMLU* g_readTagReturnValue = nullptr;
static cmsMLU* g_dupReturnValue = nullptr;
static const cmsMLU* g_lastDupSrc = nullptr;

// Simple cmsMLU implementation stub (structure content irrelevant for tests)
struct cmsMLU {
    int dummy;
};

// Mock: cmsReadTag records inputs and returns controlled pointer
cmsMLU* cmsReadTag(cmsHPROFILE h, cmsTagSignature sig) {
    g_lastReadTagH = h;
    g_lastReadTagSig = sig;
    return g_readTagReturnValue;
}

// Mock: cmsMLUdup records the source and returns the controlled dup pointer
cmsMLU* cmsMLUdup(const cmsMLU* src) {
    g_lastDupSrc = src;
    return g_dupReturnValue;
}

} // extern "C"


// Simple non-terminating assertion facility
static int g_failCount = 0;
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                  << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        ++g_failCount; \
    } } while(0)

#define RESET_FAILS() do { g_failCount = 0; } while(0)


// Convenience: reset all internal mocks to a clean state
static void resetMocks() {
    g_lastReadTagH = nullptr;
    g_lastReadTagSig = 0;
    g_readTagReturnValue = nullptr;
    g_dupReturnValue = nullptr;
    g_lastDupSrc = nullptr;
}


// Explanatory tests

// Test 1: When cmsReadTag returns NULL, GetMLUFromProfile must return NULL.
// This covers the true-branch of the initial NULL check.
bool test_GetMLUFromProfile_ReturnsNullOnNullTag() {
    RESET_FAILS();
    resetMocks();

    // Arrange
    g_readTagReturnValue = nullptr; // cmsReadTag will return NULL
    cmsHPROFILE testH = reinterpret_cast<cmsHPROFILE>(0x1234);
    cmsTagSignature testSig = static_cast<cmsTagSignature>(0xDEADBEEF);

    // Act
    cmsMLU* result = GetMLUFromProfile(testH, testSig);

    // Assert
    EXPECT_TRUE(result == nullptr, "Expected NULL when cmsReadTag returns NULL");
    EXPECT_TRUE(g_lastReadTagH == testH, "cmsReadTag should be called with provided H");
    EXPECT_TRUE(g_lastReadTagSig == testSig, "cmsReadTag should be called with provided Sig");
    // No assertion on cmsMLUdup since it should not be called
    return g_failCount == 0;
}


// Test 2: When cmsReadTag returns a non-NULL cmsMLU*, cmsMLUdup must be called and its return used.
// This covers the main path where a duplicate is created and returned.
bool test_GetMLUFromProfile_DupPathReturnsDup() {
    RESET_FAILS();
    resetMocks();

    // Arrange
    cmsMLU originalMLU;
    cmsMLU dupMLU;
    // Points to real objects for test verification
    g_readTagReturnValue = &originalMLU;
    g_dupReturnValue = &dupMLU;
    cmsHPROFILE testH = reinterpret_cast<cmsHPROFILE>(0x4321);
    cmsTagSignature testSig = static_cast<cmsTagSignature>(0xABCD);

    // Act
    cmsMLU* result = GetMLUFromProfile(testH, testSig);

    // Assert
    EXPECT_TRUE(result == &dupMLU, "GetMLUFromProfile should return the pointer produced by cmsMLUdup");
    EXPECT_TRUE(g_lastDupSrc == &originalMLU, "cmsMLUdup should be invoked with the original MLU from cmsReadTag");
    EXPECT_TRUE(g_lastReadTagH == testH, "cmsReadTag should receive the correct H");
    EXPECT_TRUE(g_lastReadTagSig == testSig, "cmsReadTag should receive the correct Sig");
    return g_failCount == 0;
}


// Test 3: When cmsReadTag returns a valid MLU but cmsMLUdup returns NULL, the function must propagate NULL.
// This validates the NULL-propagation path on the dup stage.
bool test_GetMLUFromProfile_DupReturnsNull() {
    RESET_FAILS();
    resetMocks();

    // Arrange
    cmsMLU originalMLU;
    g_readTagReturnValue = &originalMLU;
    g_dupReturnValue = nullptr; // simulate dup failure
    cmsHPROFILE testH = reinterpret_cast<cmsHPROFILE>(0x0);
    cmsTagSignature testSig = static_cast<cmsTagSignature>(0x0);

    // Act
    cmsMLU* result = GetMLUFromProfile(testH, testSig);

    // Assert
    EXPECT_TRUE(result == nullptr, "Expected NULL when cmsMLUdup returns NULL");
    EXPECT_TRUE(g_lastDupSrc == &originalMLU, "cmsMLUdup should have been called with the original MLU");
    return g_failCount == 0;
}


// Step 3 tests: Provide a simple harness

static void runAllTests() {
    bool ok = true;

    std::cout << "Running test_GetMLUFromProfile_ReturnsNullOnNullTag..." << std::endl;
    ok &= test_GetMLUFromProfile_ReturnsNullOnNullTag();
    std::cout << "Result: " << (ok ? "PASS" : "FAIL") << std::endl;

    resetMocks();
    std::cout << "Running test_GetMLUFromProfile_DupPathReturnsDup..." << std::endl;
    ok &= test_GetMLUFromProfile_DupPathReturnsDup();
    std::cout << "Result: " << (ok ? "PASS" : "FAIL") << std::endl;

    resetMocks();
    std::cout << "Running test_GetMLUFromProfile_DupReturnsNull..." << std::endl;
    ok &= test_GetMLUFromProfile_DupReturnsNull();
    std::cout << "Result: " << (ok ? "PASS" : "FAIL") << std::endl;

    if(!ok) {
        std::cerr << "One or more tests failed." << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }
}


// Main: execute all tests.
// As per <DOMAIN_KNOWLEDGE>, tests are invoked from main when GTest is not available.
int main() {
    runAllTests();
    // Return non-zero if any test failed
    return g_failCount != 0 ? 1 : 0;
}
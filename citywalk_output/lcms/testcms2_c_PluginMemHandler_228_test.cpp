// Unit test suite for the focal method PluginMemHandler located in testcms2.c
// This test suite is crafted for C++11, without Google Test.
// It uses a lightweight in-file test harness to maximize coverage and execution
// of the code under test.
//
// Step 1 (Conceptual): Candidate Keywords and dependencies drawn from the focal code
// - PluginMemHandler: the function under test (returns a void* to a memory handler).
// - DebugMemHandler: the actual memory handler instance whose address PluginMemHandler returns.
// - Memory-related helpers in testCMS2: DebugMalloc, DebugFree, etc., indicating a memory-management theme.
// - CMS context/types: cmsContext, cmsUInt32Number, etc. (used by the surrounding memory hooks).
// - The test focuses on PluginMemHandler's behavior (non-null, stable/address consistency), not on the internal of DebugMemHandler.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Forward declaration to ensure C linkage for the focal function under test.
// We avoid pulling in the entire C header to keep the test self-contained and compilable
// in a typical C++11 project setup. If testcms2.h provides a declaration for PluginMemHandler,
// this line will be compatible with it (extern "C" ensures no C++ name mangling).
extern "C" void* PluginMemHandler(void);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "TEST FAIL: " << msg << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    } \
} while(0)

#define EXPECT_EQ(a,b, msg) do { \
    ++g_total_tests; \
    if(!((a) == (b))) { \
        ++g_failed_tests; \
        std::cerr << "TEST FAIL: " << msg \
                  << " (expected equality: " << (a) << " == " << (b) \
                  << ") [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    } \
} while(0)

#define RUN_TEST(fn) do { \
    std::cout << "RUNNING: " << #fn << std::endl; \
    fn(); \
} while(0)

// Test 1: PluginMemHandler_Returns_NonNull
// Ensures that PluginMemHandler returns a valid (non-null) pointer.
// Rationale: The function is expected to provide a valid memory handler reference.
void test_PluginMemHandler_Returns_NonNull() {
    // Act
    void* handler = PluginMemHandler();

    // Assert
    EXPECT_TRUE(handler != nullptr, "PluginMemHandler should not return NULL");
}

// Test 2: PluginMemHandler_Returns_Stable_Address
// Ensures that consecutive invocations yield the same address (pointer stability).
// Rationale: The focal implementation returns the address of a static or global handler.
void test_PluginMemHandler_Returns_Stable_Address() {
    // Act
    void* first  = PluginMemHandler();
    void* second = PluginMemHandler();

    // Assert
    EXPECT_TRUE(first != nullptr, "First call must not be NULL");
    EXPECT_TRUE(second != nullptr, "Second call must not be NULL");
    EXPECT_EQ(first, second, "PluginMemHandler should return the same address on consecutive calls");
}

// Test 3: PluginMemHandler_Stability_Over_Multiple_Calls
// Ensures stability across a small sequence of calls (not just two).
void test_PluginMemHandler_Stability_Over_Multiple_Calls() {
    void* baseline = PluginMemHandler();
    EXPECT_TRUE(baseline != nullptr, "Baseline address must not be NULL");

    for (int i = 0; i < 5; ++i) {
        void* current = PluginMemHandler();
        EXPECT_EQ(current, baseline, "Address should remain stable across multiple calls");
    }
}

// Main runner
int main() {
    std::cout << "Starting PluginMemHandler unit tests (C++11, no GTest)" << std::endl;

    RUN_TEST(test_PluginMemHandler_Returns_NonNull);
    RUN_TEST(test_PluginMemHandler_Returns_Stable_Address);
    RUN_TEST(test_PluginMemHandler_Stability_Over_Multiple_Calls);

    // Summary
    std::cout << "Unit test summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures." << std::endl;

    if (g_failed_tests > 0) {
        std::cerr << "Some tests failed. See details above." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}
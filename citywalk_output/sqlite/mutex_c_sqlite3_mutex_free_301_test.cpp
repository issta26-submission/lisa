// Test suite for sqlite3_mutex_free using a lightweight, non-terminating test framework.
// This test targets the focal method:
//   void sqlite3_mutex_free(sqlite3_mutex *p)
// which, if p is non-null, invokes the global mutex's xMutexFree function pointer.
//
// Notes:
// - The tests rely on the project-provided sqliteInt.h to obtain
//   sqlite3_mutex and sqlite3GlobalConfig types.
// - We use non-terminating EXPECT_* style checks to maximize coverage without aborting.
// - We test true/false branches of the main predicate "if (p)".
// - We avoid testing the assertion path (xMutexFree == nullptr) to prevent process termination.
// - The tests are executed from main() as requested by the domain knowledge when GTest is not allowed.

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>


// bridge to the actual C function under test
extern "C" void sqlite3_mutex_free(sqlite3_mutex *p);

// Access to the project-wide global configuration.
// The sqlite3GlobalConfig symbol is a C-global object in the sqlite project.
// Declaring it here allows us to hook the xMutexFree callback for testing.
extern "C" sqlite3GlobalConfig sqlite3GlobalConfig;

// Lightweight non-terminating test framework
static int g_test_failures = 0;
static int g_expected_calls = 0;
static sqlite3_mutex* g_last_mutex_free_arg = nullptr;

// Simple non-terminating assertion helpers
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at line " << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " != " #b " at line " << __LINE__ \
                  << " (" << static_cast<void*>(nullptr) << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)


// Test 1: When p is non-null and xMutexFree is provided, sqlite3_mutex_free should call it exactly once with p.
static void test_mutex_free_calls_user_callback_with_non_null_pointer() {
    // Prepare test state
    g_last_mutex_free_arg = nullptr;
    g_expected_calls = 0;

    // Local callback to track invocation
    auto callback = [](sqlite3_mutex* m) {
        g_last_mutex_free_arg = m;
        ++g_expected_calls;
    };

    // Install our callback into the global config
    sqlite3GlobalConfig.mutex.xMutexFree = callback;

    // Create a dummy mutex object and invoke the function under test
    sqlite3_mutex m;
    sqlite3GlobalConfig.mutex.xMutexFree = callback; // ensure non-null to avoid asserts
    sqlite3_mutex_free(&m);

    // Verify the callback was invoked exactly once with the correct argument
    EXPECT_TRUE(g_last_mutex_free_arg == &m);
    EXPECT_TRUE(g_expected_calls == 1);
}

// Test 2: When p is NULL, sqlite3_mutex_free should do nothing (no crash, no callback).
static void test_mutex_free_with_null_pointer() {
    // Reset test state
    g_last_mutex_free_arg = reinterpret_cast<sqlite3_mutex*>(nullptr);
    g_expected_calls = 0;

    // Ensure xMutexFree is non-null to bypass assertion in the focal implementation
    // The actual branch only runs when p != NULL; we are testing the false branch here.
    sqlite3GlobalConfig.mutex.xMutexFree = [](sqlite3_mutex* /*p*/) {
        // This should not be invoked in this test.
        ++g_expected_calls;
    };

    // Call with NULL
    sqlite3_mutex_free(nullptr);

    // Expect no callback invocation
    EXPECT_TRUE(g_last_mutex_free_arg == nullptr);
    EXPECT_TRUE(g_expected_calls == 0);
}

// Note: Testing the path where xMutexFree == nullptr would trigger an assertion in the
// production code. To avoid terminating the test process, that path is intentionally skipped.
// If needed, this could be tested by compiling with NDEBUG or by a separate test harness
// that captures aborts, but such an approach is beyond the scope of this lightweight suite.

int main() {
    std::cout << "Running sqlite3_mutex_free unit tests (non-GTest)." << std::endl;

    // Run tests
    test_mutex_free_calls_user_callback_with_non_null_pointer();
    test_mutex_free_with_null_pointer();

    // Summary
    if(g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with " << g_test_failures << " failure(s)." << std::endl;
        return 1;
    }
}
// Unit test suite for sqlite3_mutex_enter focusing on minimal, reversible behavior.
// This test suite is written for C++11 and does not depend on GTest. It uses a small
// in-file test harness and mocks to verify the focal method behavior.

#include <iostream>
#include <sqliteInt.h>
#include <cassert>


namespace mutex_enter_tests {

    // Lightweight, fake sqlite3_mutex type to simulate SQLite mutex objects.
    struct sqlite3_mutex {};

    // Minimal stand-in for the global configuration structure used by sqlite3MutexEnter.
    // We replicate only the parts used by the focal function.
    struct sqlite3GlobalConfig_t {
        struct {
            void (*xMutexEnter)(sqlite3_mutex*); // Function pointer to be called when entering a mutex
        } mutex;
    };

    // Global configuration instance used by the focal function.
    // Initialized with a null function pointer to reflect the default uninitialized state.
    static sqlite3GlobalConfig_t sqlite3GlobalConfig = { { nullptr } };

    // ----------------- FOCAL METHOD UNDER TEST -----------------
    // Re-create the core method in this test file to exercise it directly.
    // The real implementation resides in mutex.c; here we mirror its logic for testing.
    void sqlite3_mutex_enter(sqlite3_mutex *p) {
        if( p ){
            assert( sqlite3GlobalConfig.mutex.xMutexEnter );
            sqlite3GlobalConfig.mutex.xMutexEnter(p);
        }
    }

    // ----------------- TEST FIXTURES AND UTILITIES -----------------
    // Flags and storage to verify that the fake enter hook was invoked with the correct argument.
    static bool g_enter_called = false;
    static sqlite3_mutex* g_last_enter_arg = nullptr;

    // A fake xMutexEnter implementation to capture calls made by sqlite3_mutex_enter.
    static void fake_xMutexEnter(sqlite3_mutex* m) {
        g_enter_called = true;
        g_last_enter_arg = m;
    }

    // Helper to reset test state between tests.
    static void reset_test_state() {
        g_enter_called = false;
        g_last_enter_arg = nullptr;
    }

    // ----------------- TEST 1: Non-null path should call the hook with the same pointer -----------------
    // This test covers the true branch of the if (p) condition by providing a non-null
    // sqlite3_mutex pointer and ensuring that:
    //  - the configured xMutexEnter function is invoked, and
    //  - the argument passed to xMutexEnter is exactly the same pointer.
    bool test_enter_calls_when_non_null() {
        reset_test_state();
        // Arrange: hook the fake xMutexEnter
        sqlite3GlobalConfig.mutex.xMutexEnter = &fake_xMutexEnter;

        // Act: call the focal method with a non-null mutex
        static sqlite3_mutex m;
        sqlite3_mutex_enter(&m);

        // Assert: verify that the hook was called and received the correct argument
        bool result = g_enter_called && (g_last_enter_arg == &m);

        // Cleanup: detach the hook to avoid side effects on other tests
        sqlite3GlobalConfig.mutex.xMutexEnter = nullptr;
        return result;
    }

    // ----------------- TEST 2: Null path should not call any hook and should not crash -----------------
    // This test covers the false branch of the if (p) condition by passing a null pointer.
    // Expected behavior: no call to xMutexEnter should be made, and the function should return cleanly.
    bool test_enter_not_called_on_null() {
        reset_test_state();
        // Arrange: hook the fake xMutexEnter (should not be called for null p)
        sqlite3GlobalConfig.mutex.xMutexEnter = &fake_xMutexEnter;

        // Act: call the focal method with a null pointer
        sqlite3_mutex_enter(nullptr);

        // Assert: ensure the hook was not called
        bool result = (g_enter_called == false);

        // Cleanup
        sqlite3GlobalConfig.mutex.xMutexEnter = nullptr;
        return result;
    }

    // ----------------- SIMPLE TEST RUNNER -----------------
    int run_all() {
        int failures = 0;

        std::cout << "Running sqlite3_mutex_enter tests...\n";

        // Test 1
        bool t1 = test_enter_calls_when_non_null();
        if (!t1) {
            std::cerr << "Failure: test_enter_calls_when_non_null\n";
            ++failures;
        } else {
            std::cout << "Passed: test_enter_calls_when_non_null\n";
        }

        // Test 2
        bool t2 = test_enter_not_called_on_null();
        if (!t2) {
            std::cerr << "Failure: test_enter_not_called_on_null\n";
            ++failures;
        } else {
            std::cout << "Passed: test_enter_not_called_on_null\n";
        }

        return failures;
    }

} // namespace mutex_enter_tests

// ----------------- MAIN -----------------
int main() {
    // Run tests and translate failures into a non-zero exit code.
    int failures = mutex_enter_tests::run_all();
    if (failures != 0) {
        std::cerr << "mutex_enter_tests completed with " << failures << " failure(s).\n";
    } else {
        std::cout << "mutex_enter_tests completed successfully.\n";
    }
    return failures;
}
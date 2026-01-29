/*
  Unit test suite for sqlite3OsClose(sqlite3_file *pId)

  Context:
  - The focal method is defined in os.c as:
      void sqlite3OsClose(sqlite3_file *pId){
        if( pId->pMethods ){
          pId->pMethods->xClose(pId);
          pId->pMethods = 0;
        }
      }

  - We do not rely on the real SQLite headers. Instead, we provide minimal
    test doubles (mock structures) that model the necessary parts of sqlite3_file
    and its pMethods to exercise the behavior of sqlite3OsClose.

  - Tests cover:
    1) Branch when pMethods is non-null: xClose is invoked and pMethods is cleared.
    2) Branch when pMethods is null: xClose is not invoked and no change occurs.
    3) Idempotence / multiple calls: subsequent calls do not call xClose again since pMethods becomes null.

  - The tests are implemented in C++11 plain unit-test style (no gtest/gmock).
    They use a small, non-terminating assertion style and print results to stdout/stderr.
*/ 

#include <iostream>
#include <sqliteInt.h>


// Forward declarations to match the expected C interface used by sqlite3OsClose.
// We provide lightweight mocks that are sufficient for testing the behavior.

struct sqlite3_file;      // forward declaration
struct sqlite3_io_methods {
    void (*xClose)(sqlite3_file*);
};

// Minimal sqlite3_file layout sufficient for sqlite3OsClose logic.
// We only rely on the pMethods pointer in this test.
struct sqlite3_file {
    sqlite3_io_methods *pMethods;
};

// Declare the function under test with C linkage to ensure correct linkage
extern "C" void sqlite3OsClose(sqlite3_file *pId);

// Test infrastructure (simple, non-terminating assertions)
static int g_test_failures = 0; // global failure counter for the test run

#define TEST_CHECK(cond, msg)                                                        \
    do {                                                                             \
        if(!(cond)) {                                                                \
            std::cerr << "Test assertion failed: " << (msg) << " (in " << __func__ \
                      << ")\n";                                                     \
            ++g_test_failures;                                                       \
        }                                                                            \
    } while(0)

// Global state used by the mocked xClose to verify invocation details
static int g_xCloseCallCount = 0;
static sqlite3_file *g_xCloseLastArg = nullptr;

// Mock implementation of xClose that sqlite3OsClose will call.
// It records its invocation details for verification.
static void mocked_xClose(sqlite3_file *pFile) {
    ++g_xCloseCallCount;
    g_xCloseLastArg = pFile;
}

// Candidate keywords extracted from the focal method and dependencies:
// pId, pMethods, xClose, memset/assignment to 0, invocation side-effect, idempotence.

// Test 1: When pId->pMethods is non-null, xClose should be invoked once and pId->pMethods should be cleared.
static bool test_close_with_pMethods_invokes_xClose_and_clears_pointer() {
    std::cout << "Running test_close_with_pMethods_invokes_xClose_and_clears_pointer..." << std::endl;

    // Reset state
    g_xCloseCallCount = 0;
    g_xCloseLastArg = nullptr;

    // Set up mock methods and file
    static sqlite3_io_methods methods = { mocked_xClose };
    static sqlite3_file file;
    file.pMethods = &methods;

    // Act
    sqlite3OsClose(&file);

    // Assert
    bool ok = (g_xCloseCallCount == 1) &&
              (g_xCloseLastArg == &file) &&
              (file.pMethods == nullptr);

    TEST_CHECK(ok, "xClose called exactly once, argument is the file, and pMethods cleared to null");
    if(!ok) {
        std::cerr << "Details: xCloseCallCount=" << g_xCloseCallCount
                  << ", lastArg=" << static_cast<void*>(g_xCloseLastArg)
                  << ", file.pMethods=" << static_cast<void*>(file.pMethods) << std::endl;
    }
    return ok;
}

// Test 2: When pId->pMethods is null, sqlite3OsClose should not call xClose and should leave pMethods as null.
static bool test_close_with_null_pMethods_does_nothing() {
    std::cout << "Running test_close_with_null_pMethods_does_nothing..." << std::endl;

    // Reset state
    g_xCloseCallCount = 0;
    g_xCloseLastArg = nullptr;

    // Set up file with null pMethods
    static sqlite3_file file;
    file.pMethods = nullptr;

    // Act
    sqlite3OsClose(&file);

    // Assert
    bool ok = (g_xCloseCallCount == 0) &&
              (g_xCloseLastArg == nullptr) &&
              (file.pMethods == nullptr);

    TEST_CHECK(ok, "No xClose invocation when pMethods is null, and pMethods remains null");
    if(!ok) {
        std::cerr << "Details: xCloseCallCount=" << g_xCloseCallCount
                  << ", lastArg=" << static_cast<void*>(g_xCloseLastArg)
                  << ", file.pMethods=" << static_cast<void*>(file.pMethods) << std::endl;
    }
    return ok;
}

// Test 3: Multiple calls should only invoke xClose on the first call since pMethods is cleared afterward.
static bool test_close_is_idempotent_across_multiple_calls() {
    std::cout << "Running test_close_is_idempotent_across_multiple_calls..." << std::endl;

    // Reset state
    g_xCloseCallCount = 0;
    g_xCloseLastArg = nullptr;

    // Set up file with non-null pMethods
    static sqlite3_io_methods methods = { mocked_xClose };
    static sqlite3_file file;
    file.pMethods = &methods;

    // Act - first call
    sqlite3OsClose(&file);

    // Capture state after first call
    bool firstCallOk = (g_xCloseCallCount == 1) &&
                       (g_xCloseLastArg == &file) &&
                       (file.pMethods == nullptr);

    // Act - second call
    sqlite3OsClose(&file);

    // Assert: no additional xClose call occurs
    bool secondCallNoAdditionalCall = (g_xCloseCallCount == 1);

    bool ok = firstCallOk && secondCallNoAdditionalCall;
    TEST_CHECK(ok, "Second call does not invoke xClose again and pMethods remains null");
    if(!ok) {
        std::cerr << "Details: firstCallOk=" << firstCallOk
                  << ", secondCallNoAdditionalCall=" << secondCallNoAdditionalCall
                  << ", xCloseCallCount=" << g_xCloseCallCount << std::endl;
    }
    return ok;
}

// Main test runner
int main() {
    std::cout << "Starting sqlite3OsClose unit tests (no GTest)." << std::endl;

    bool allPassed = true;
    allPassed &= test_close_with_pMethods_invokes_xClose_and_clears_pointer();
    allPassed &= test_close_with_null_pMethods_does_nothing();
    allPassed &= test_close_is_idempotent_across_multiple_calls();

    if(allPassed && g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed. Failures: " << g_test_failures << std::endl;
        return 1;
    }
}
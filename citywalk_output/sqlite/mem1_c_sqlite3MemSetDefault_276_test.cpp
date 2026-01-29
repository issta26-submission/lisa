// unit_test_memset_default.cpp
// Purpose: Provide a lightweight, self-contained unit test suite for the focal method
//          sqlite3MemSetDefault located in mem1.c. The tests are designed to be
//          compiled together with mem1.c under C++11, without using GTest.
// Notes on how this test is wired:
//  - We define a C-linkage mock for sqlite3_config to capture the invocation from
//    sqlite3MemSetDefault. The function is variadic in the real code, but we only
//    extract the sqlite3_mem_methods* argument (which is what sqlite3MemSetDefault
//    passes when SQLITE_CONFIG_MALLOC is configured).
//  - We compile with __APPLE__ and SQLITE_SYSTEM_MALLOC defined (to exercise the
//    code path inside mem1.c). The last field in sqlite3_mem_methods is set to 0
//    by the initializer, which we cannot inspect directly here; we instead verify
//    that sqlite3_config was invoked and that the passed pointer is non-null.
//  - The tests are designed to be non-terminating: they accumulate results and report
//    a final status in main().
//  - This file assumes mem1.c is compiled in the same translation unit/linkage path.

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/sysctl.h>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <libkern/OSAtomic.h>
#include <malloc/malloc.h>


// Forward declaration of the opaque SQLite memory methods struct.
// We only need a pointer to it for the variadic argument in sqlite3_config.
struct sqlite3_mem_methods;

// Forward declaration of the focal function (defined in mem1.c under the
// macros described in the task). We rely on C linkage for interoperability.
extern "C" void sqlite3MemSetDefault(void);

// Provide a C-linkage mock for sqlite3_config to capture its usage.
// The real function in SQLite uses a variadic signature: int sqlite3_config(int, ...);
// We implement a minimal variant that extracts the sqlite3_mem_methods* when invoked
// with SQLITE_CONFIG_MALLOC. We do not rely on the exact value of the first argument
// for the assertions; we only require that the call happens and the methods pointer is non-null.

extern "C" int sqlite3_config(int op, ...) {
    // Capture the variadic arguments and extract the sqlite3_mem_methods* assuming
    // that op == SQLITE_CONFIG_MALLOC (as used by sqlite3MemSetDefault).
    va_list ap;
    va_start(ap, op);
    sqlite3_mem_methods* m = va_arg(ap, sqlite3_mem_methods*);
    va_end(ap);

    // Record the invocation in a place accessible to the tests.
    // Note: We declare these as file-scope globals below.
    g_config_call_count++;
    g_last_config_methods = m;
    // Returning 0 to mimic a successful configuration in sqlite3_config.
    return 0;
}

// Global state used by tests to verify behavior of sqlite3MemSetDefault.
static int g_config_call_count = 0;
static sqlite3_mem_methods* g_last_config_methods = nullptr;

// Helper: Reset global state before each test case.
static void reset_state() {
    g_config_call_count = 0;
    g_last_config_methods = nullptr;
}

// A simple test harness structure to accumulate results without terminating early on
// first failure. This helps maximize code coverage across multiple scenarios.
struct TestCase {
    std::string name;
    bool passed;
    std::string reason;
};

// Test 1: Basic sanity - calling sqlite3MemSetDefault should invoke sqlite3_config exactly once
//           and pass a non-null sqlite3_mem_methods* to it.
static bool test_basic_single_call(std::string &out_reason) {
    reset_state();
    // Call the focal function. It should trigger sqlite3_config via the mock above.
    sqlite3MemSetDefault();

    if (g_config_call_count != 1) {
        out_reason = "Expected sqlite3_config to be called exactly once.";
        return false;
    }
    if (g_last_config_methods == nullptr) {
        out_reason = "Expected a non-null sqlite3_mem_methods* passed to sqlite3_config.";
        return false;
    }
    return true;
}

// Test 2: Idempotence/Repeatability - calling sqlite3MemSetDefault twice should result in two
//         invocations of sqlite3_config, increasing the call count accordingly.
static bool test_basic_double_call(std::string &out_reason) {
    reset_state();
    sqlite3MemSetDefault(); // first call
    sqlite3MemSetDefault(); // second call

    if (g_config_call_count != 2) {
        out_reason = "Expected sqlite3_config to be called twice after two invocations.";
        return false;
    }
    if (g_last_config_methods == nullptr) {
        out_reason = "Expected a non-null sqlite3_mem_methods* passed to sqlite3_config on second call.";
        return false;
    }
    return true;
}

// Test 3: Ensure static linkage behavior (compile-time constraint imitation)
// Since we cannot directly access internal static functions from mem1.c, we verify that
// the configuration pointer remains non-null across repeated calls. This provides a coverage
// metric for the branch where the defaultMethods is registered.
static bool test_static_linkage_emulation(std::string &out_reason) {
    reset_state();
    sqlite3MemSetDefault();
    if (g_config_call_count != 1 || g_last_config_methods == nullptr) {
        out_reason = "First call failed to set sqlite3_config with non-null methods.";
        return false;
    }

    // Call again to exercise another configuration path.
    sqlite3MemSetDefault();
    if (g_config_call_count != 2) {
        out_reason = "Second call did not increment invocation count as expected.";
        return false;
    }
    if (g_last_config_methods == nullptr) {
        out_reason = "Second call passed a null sqlite3_mem_methods pointer unexpectedly.";
        return false;
    }
    return true;
}

// Entry point for running all tests.
// We guard the test invocation with macros to ensure the build environment is compatible.
// The following preprocessor conditions reflect the required runtime environment described
// in the task: we want to exercise the code path inside mem1.c that is compiled under
// __APPLE__ and SQLITE_SYSTEM_MALLOC. If those macros are not defined, the test is a no-op.
int main() {
#if defined(__APPLE__) && defined(SQLITE_SYSTEM_MALLOC)
    std::vector<TestCase> tests;
    std::string msg;

    // Test 1
    tests.push_back({"test_basic_single_call",
                     test_basic_single_call(msg),
                     "Basic single invocation should call sqlite3_config once and pass non-null methods."});
    if (!tests.back().passed) {
        std::cerr << "FAILED: " << tests.back().name << " - " << tests.back().reason << "\n";
    } else {
        std::cout << "PASSED: " << tests.back().name << "\n";
    }

    // Test 2
    tests.push_back({"test_basic_double_call",
                     test_basic_double_call(msg),
                     "Double invocation should result in two sqlite3_config calls."});
    if (!tests.back().passed) {
        std::cerr << "FAILED: " << tests.back().name << " - " << tests.back().reason << "\n";
    } else {
        std::cout << "PASSED: " << tests.back().name << "\n";
    }

    // Test 3
    tests.push_back({"test_static_linkage_emulation",
                     test_static_linkage_emulation(msg),
                     "Emulate static linkage behavior by ensuring non-null methods on repeated calls."});
    if (!tests.back().passed) {
        std::cerr << "FAILED: " << tests.back().name << " - " << tests.back().reason << "\n";
    } else {
        std::cout << "PASSED: " << tests.back().name << "\n";
    }

    // Summary
    int failures = 0;
    for (const auto &t : tests) if (!t.passed) ++failures;
    if (failures) {
        std::cout << "Tests completed with " << failures << " failure(s).\n";
        return EXIT_FAILURE;
    } else {
        std::cout << "All tests passed.\n";
        return EXIT_SUCCESS;
    }
#else
    // Environment not configured to compile mem1.c test path.
    // Provide a graceful message and exit.
    std::cerr << "Test environment not configured: __APPLE__ and SQLITE_SYSTEM_MALLOC must be defined to exercise sqlite3MemSetDefault.\n";
    return EXIT_FAILURE;
#endif
}
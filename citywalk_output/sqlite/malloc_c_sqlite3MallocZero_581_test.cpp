// Lightweight C++11 test harness for sqlite3MallocZero
// This test suite mimics the behavior of the focal method by providing
// a mock sqlite3Malloc that allows us to observe whether sqlite3MallocZero
// correctly zeros the allocated memory when allocation succeeds, and
// returns NULL when allocation fails.

// Note: This test is designed to be compiled with the surrounding project
// that provides the actual sqlite3MallocZero function. Here, we provide a
// standalone, self-contained version of the method under test (sqlite3MallocZero)
// for controlled testing in the absence of the full project setup. The mock
// sqlite3Malloc is used to simulate success/failure of memory allocation.

#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


using u64 = uint64_t;

// Global knobs to drive the mock allocator's behavior
static bool g_mockReturnNull = false; // When true, sqlite3Malloc should return NULL
static std::size_t g_lastRequestedN = 0; // Track last allocation request size (for debugging)

// Forward declaration of the function under test (as provided in the codebase)
extern "C" void *sqlite3MallocZero(u64 n);

// Mocked C linkage allocator to replace the real sqlite3Malloc during tests
extern "C" void *sqlite3Malloc(uint64_t n) {
    g_lastRequestedN = static_cast<std::size_t>(n);
    if (g_mockReturnNull) {
        // Simulate allocation failure
        return nullptr;
    }
    // Simulate a successful allocation using the standard malloc
    void *ptr = std::malloc(static_cast<std::size_t>(n));
    if (ptr) {
        // Pre-fill with a recognizable pattern to observe zeroing later
        std::memset(ptr, 0xAB, static_cast<std::size_t>(n));
    }
    return ptr;
}

// Implementation of the focal method under test (sqlite3MallocZero)
// This mirrors the logic shown in the problem statement.
extern "C" void *sqlite3MallocZero(u64 n) {
    void *p = sqlite3Malloc(n);
    if (p) {
        std::memset(p, 0, static_cast<size_t>(n));
    }
    return p;
}

// Simple test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define LOG_TEST_START(name) \
    std::cout << "[TEST] " << name << " ... ";

// Test 1: When sqlite3Malloc succeeds, sqlite3MallocZero should return a non-null pointer
// and the allocated memory should be initialized to zero.
bool test_sqlite3MallocZero_allocates_and_zeros() {
    LOG_TEST_START("test_sqlite3MallocZero_allocates_and_zeros");
    g_mockReturnNull = false; // Ensure allocation succeeds
    g_lastRequestedN = 0;

    const std::size_t N = 16;
    void *ptr = sqlite3MallocZero(N);

    bool ok = true;
    if (ptr == nullptr) {
        ok = false;
        std::cerr << " FAILED: sqlite3MallocZero returned NULL on success path.\n";
    } else {
        // Verify memory contents are all zeros
        unsigned char *bytes = static_cast<unsigned char*>(ptr);
        for (std::size_t i = 0; i < N; ++i) {
            if (bytes[i] != 0) {
                ok = false;
                std::cerr << " FAILED: Memory not zeroed at index " << i
                          << " (byte value = " << static_cast<int>(bytes[i]) << ").\n";
                break;
            }
        }
        std::free(ptr);
    }

    ++g_tests_run;
    if (ok) {
        ++g_tests_passed;
        std::cout << " PASSED\n";
    } else {
        std::cout << " FAILED\n";
    }
    return ok;
}

// Test 2: When sqlite3Malloc fails (returns NULL), sqlite3MallocZero should propagate NULL
bool test_sqlite3MallocZero_handles_null_allocation() {
    LOG_TEST_START("test_sqlite3MallocZero_handles_null_allocation");
    g_mockReturnNull = true; // Force allocation to fail

    void *ptr = sqlite3MallocZero(8);

    bool ok = (ptr == nullptr);
    if (!ok) {
        std::cerr << " FAILED: Expected NULL return when sqlite3Malloc fails.\n";
    }

    ++g_tests_run;
    if (ok) {
        ++g_tests_passed;
        std::cout << " PASSED\n";
    } else {
        std::cout << " FAILED\n";
    }
    return ok;
}

// Main entry point: run all tests and report a summary
int main() {
    bool a = test_sqlite3MallocZero_allocates_and_zeros();
    bool b = test_sqlite3MallocZero_handles_null_allocation();

    // Summary
    std::cout << "\nTest summary: " << g_tests_passed << " / " << g_tests_run
              << " tests passed.\n";

    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
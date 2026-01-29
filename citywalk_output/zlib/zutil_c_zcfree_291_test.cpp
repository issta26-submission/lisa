// Test suite for the focal method: zcfree defined in zutil.c
// This test is written for C++11 and does not rely on GTest.
// It uses a minimal non-terminating assertion mechanism (EXPECT) to collect failures
// and runs tests from main() to maximize execution coverage.

#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <zutil.h>
#include <stdlib.h>
#include <cstdlib>


// -----------------------------------------------------------------------------
// Domain/Interface Assumptions for the focal method
// The focal method is:
//   void ZLIB_INTERNAL zcfree(voidpf opaque, voidpf ptr) {
//       (void)opaque;
//       free(ptr);
//   }
//
// Where: voidpf is a typedef for void* (as in zlib's zutil.h).
// The function ignores the opaque parameter and simply frees the provided pointer.
// It is defined within a #ifdef ZLIB_DEBUG block in the original source file.
// For testing, we declare the prototype here to ensure linkage with zcfree.
// -----------------------------------------------------------------------------

extern "C" {
    // In zlib, voidpf is typically a typedef to void*. We mirror that here for the prototype.
    typedef void* voidpf;
    // Declaration of the focal function (linkage is C style to avoid name mangling)
    void zcfree(voidpf opaque, voidpf ptr);
}

// Global failure collection to implement non-terminating assertions
static std::vector<std::string> g_failures;

// Lightweight non-terminating assertion macro.
// If the condition is false, the provided description is recorded and execution continues.
#define EXPECT(condition, description)                                      \
    do {                                                                      \
        if (!(condition)) {                                                   \
            g_failures.push_back(std::string("EXPECT_FAILED: ") + (description)); \
        }                                                                     \
    } while (0)

// Helper to run a single test and report status without terminating the process.
static void run_one_test(const std::string& test_name, void (*test_func)()) {
    g_failures.clear();
    test_func();
    if (g_failures.empty()) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        std::cerr << "[FAIL] " << test_name << std::endl;
        for (const auto& msg : g_failures) {
            std::cerr << "       " << msg << std::endl;
        }
    }
}

// Test 1: zcfree should handle nullptr ptr gracefully (opaque can be anything; here null)
static void test_zcfree_nullptr_ptr_nullptr() {
    // The opaque parameter is explicitly ignored by zcfree.
    // Freeing a null pointer should be a no-op in C (and thus safe).
    zcfree(nullptr, nullptr);
    EXPECT(true, "zcfree(nullptr, nullptr) completed without crash");
}

// Test 2: zcfree should correctly handle a non-null pointer with a null opaque
static void test_zcfree_allocated_ptr_null_opaque() {
    void* p = std::malloc(64);
    EXPECT(p != nullptr, "malloc returned non-null pointer for 64 bytes");
    if (p) {
        // Touch the memory to ensure it's writable before freeing
        std::memset(p, 0xA5, 64);
        zcfree(nullptr, p);
        // If zcfree crashes or misbehaves, the test would fail by terminating the process;
        // otherwise, we conservatively log success.
        EXPECT(true, "zcfree(nullptr, allocated_ptr) completed without crash");
    } else {
        // If allocation failed, report as a failure to aid debugging.
        g_failures.push_back("Test setup failed: malloc returned nullptr for 64 bytes");
    }
}

// Test 3: zcfree should handle a non-null pointer with a non-null opaque
static void test_zcfree_allocated_ptr_nonnull_opaque() {
    void* p = std::malloc(32);
    EXPECT(p != nullptr, "malloc returned non-null pointer for 32 bytes");
    if (p) {
        std::memset(p, 0x5A, 32);
        // Provide a non-null opaque value to ensure the function ignores it (as per design)
        zcfree(reinterpret_cast<void*>(0xDEADBEEF), p);
        EXPECT(true, "zcfree(opaque, allocated_ptr) completed without crash");
    } else {
        g_failures.push_back("Test setup failed: malloc returned nullptr for 32 bytes");
    }
}

// Main entry point to run all tests
int main() {
    std::cout << "Running zcfree unit tests (C++11)..." << std::endl;

    // Step 2: Unit Test Generation
    // Execute the generated tests with non-terminating assertions.
    run_one_test("test_zcfree_nullptr_ptr_nullptr", test_zcfree_nullptr_ptr_nullptr);
    run_one_test("test_zcfree_allocated_ptr_null_opaque", test_zcfree_allocated_ptr_null_opaque);
    run_one_test("test_zcfree_allocated_ptr_nonnull_opaque", test_zcfree_allocated_ptr_nonnull_opaque);

    // Step 3: Test Case Refinement
    // The above tests exercise true/false branches conceptually:
    // - true path (no crash) and potential failure modes (invalid allocations),
    // - and a non-null opaque path to ensure the opaque value is ignored as designed.

    // Summary
    if (g_failures.empty()) {
        std::cout << "All zcfree tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures.size() << " test(s) failed." << std::endl;
        // Optional: print a final summary of failures
        for (const auto& msg : g_failures) {
            std::cerr << "  - " << msg << std::endl;
        }
        return 1;
    }
}
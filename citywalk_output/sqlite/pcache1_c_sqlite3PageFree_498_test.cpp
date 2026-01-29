// Unit test suite for sqlite3PageFree (wrapper around pcache1Free)
// FOCAL_METHOD under test:
//   void sqlite3PageFree(void *p){ pcache1Free(p); }
//
// NOTE:
// - The static pcache1Free(void*) in pcache1.c is defined as an empty function in the provided dependencies.
// - Therefore, sqlite3PageFree delegates to pcache1Free, but there is no observable side effect to verify.
// - The tests below focus on ensuring the function can be called safely with various inputs and does not crash.
// - A lightweight, self-contained test harness is used (no external testing framework).

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>


// Declaration of the focal function with C linkage so it can be linked against the C implementation.
extern "C" void sqlite3PageFree(void *p);

// Lightweight test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating test reporter
static void report_test(const char* test_name, bool passed, const char* reason = "") {
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << test_name;
        if (reason && reason[0] != '\0') {
            std::cerr << " - " << reason;
        }
        std::cerr << std::endl;
    }
}

/*
  Test 1: sqlite3PageFree should handle a null pointer without crashing.
  - Input: nullptr
  - Expected: Function call completes without exception.
*/
static void test_sqlite3PageFree_NullPointer() {
    const char* test_name = "sqlite3PageFree_NullPointer";
    bool ok = true;
    try {
        sqlite3PageFree(nullptr);
    } catch (...) {
        ok = false;
    }
    report_test(test_name, ok, ok ? "" : "Exception thrown when passing nullptr");
}

/*
  Test 2: sqlite3PageFree should handle a valid allocated pointer without crashing.
  - Input: Pointer obtained from malloc
  - Expected: Function call completes without exception.
  - Cleanup: free the allocated memory after the call (since sqlite3PageFree is a wrapper, it should not free the memory itself).
*/
static void test_sqlite3PageFree_WithAllocatedPointer() {
    const char* test_name = "sqlite3PageFree_WithAllocatedPointer";
    bool ok = true;
    void *p = std::malloc(128);
    if (!p) {
        report_test(test_name, false, "malloc failed");
        return;
    }
    try {
        sqlite3PageFree(p);
    } catch (...) {
        ok = false;
    }
    std::free(p);
    report_test(test_name, ok, ok ? "" : "Exception thrown when passing allocated pointer");
}

/*
  Test 3: sqlite3PageFree can be called multiple times on the same pointer without crashing.
  - Input: Pointer from malloc; call sqlite3PageFree twice
  - Expected: Both calls complete without exception; memory is freed by test after usage.
*/
static void test_sqlite3PageFree_DoubleFree() {
    const char* test_name = "sqlite3PageFree_DoubleFree";
    bool ok = true;
    void *p = std::malloc(64);
    if (!p) {
        report_test(test_name, false, "malloc failed");
        return;
    }
    try {
        sqlite3PageFree(p);
        // Second call on the same pointer
        sqlite3PageFree(p);
    } catch (...) {
        ok = false;
    }
    std::free(p);
    report_test(test_name, ok, ok ? "" : "Exception thrown on double free call");
}

int main() {
    // Run tests
    test_sqlite3PageFree_NullPointer();
    test_sqlite3PageFree_WithAllocatedPointer();
    test_sqlite3PageFree_DoubleFree();

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << " | Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests;
}
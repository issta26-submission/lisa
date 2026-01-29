// C++11 test suite for the focal method testMalloc in lsmtest_main.c
// This test suite is designed to compile with C++11, without GoogleTest (GTest).
// It uses a lightweight non-terminating assertion mechanism to maximize coverage.
//
// Focal method under test (extern "C" for C linkage):
// void *testMalloc(int n){
 // u8 *p = (u8*)malloc(n + 8);
 // memset(p, 0, n+8);
 // *(int*)p = n;
 // return (void*)&p[8];
// }
//
// Key behavior to verify:
// - Allocates n+8 bytes
// - Zeroes entire block
// - Stores n in the first 4 bytes of the allocated block
// - Returns a pointer to the memory immediately after the 8-byte header
//   i.e., (void*)&p[8], so the base pointer is p and header occupies p[0..7].

#include <lsmtest.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <cstring>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Declare the focal function from the C file with C linkage
extern "C" void* testMalloc(int n);

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

// Record test result with a short descriptive name
static void recordResult(const char* testName, bool passed) {
    ++g_total;
    if (passed) {
        ++g_passed;
        std::cout << "[PASS] " << testName << "\n";
    } else {
        ++g_failed;
        std::cerr << "[FAIL] " << testName << "\n";
    }
}

// Test 1: Basic behavior with n = 0
// - Expect a non-null pointer
// - Expect the header (first 4 bytes before the returned pointer) to contain 0
// - Expect the 4 bytes following the header (offsets 4..7) to be 0
// - Free the allocated block afterwards
static void testMalloc_basicZeroLength() {
    const int n = 0;
    void* res = testMalloc(n);
    bool ok = false;

    if (res != nullptr) {
        unsigned char* r = (unsigned char*)res;
        void* base = (void*)(r - 8);
        int headerVal = *(int*)base;
        int midVal = *(int*)((char*)base + 4); // bytes 4..7
        bool headerOk = (headerVal == n);
        bool midOk = (midVal == 0);
        bool memAligned = (reinterpret_cast<uintptr_t>(res) % 8) == 0; // 8-byte alignment expectation
        ok = headerOk && midOk && memAligned;

        // Free the underlying block
        free(base);
    } else {
        ok = false;
    }

    recordResult("testMalloc_basicZeroLength: header==0, middle==0, non-null", ok);
}

// Test 2: Non-zero length (e.g., n = 16) behavior
// - Ensure header stores 16
// - Ensure the 4 bytes after the header are still 0
// - Ensure the returned memory region (n bytes) is zero-initialized
// - Ensure alignment and correct independence from other allocations
static void testMalloc_nonZeroLength() {
    const int n = 16;
    void* res = testMalloc(n);
    bool ok = false;

    if (res != nullptr) {
        unsigned char* r = (unsigned char*)res;
        void* base = (void*)(r - 8);
        int headerVal = *(int*)base;
        int midVal = *(int*)((char*)base + 4);
        bool headerOk = (headerVal == n);
        bool midOk = (midVal == 0);

        // Check the allocated region is zeroed
        bool memZero = true;
        for (int i = 0; i < n; ++i) {
            if (r[i] != 0) { memZero = false; break; }
        }

        bool alignedOk = (reinterpret_cast<uintptr_t>(res) % 8) == 0;

        ok = headerOk && midOk && memZero && alignedOk;

        // Free the underlying block
        free(base);
    }

    recordResult("testMalloc_nonZeroLength: header stores n, middle bytes zero, memory zero-initialized, aligned", ok);
}

// Test 3: Multiple allocations are independent (different base blocks)
// - Allocate two blocks of the same size and ensure they do not alias
// - Each block's header should store the correct size
static void testMalloc_multiple_calls_independent() {
    const int n = 8;
    void* r1 = testMalloc(n);
    void* r2 = testMalloc(n);

    bool ok = false;
    if (r1 && r2 && r1 != r2) {
        void* base1 = (void*)((unsigned char*)r1 - 8);
        void* base2 = (void*)((unsigned char*)r2 - 8);
        int h1 = *(int*)base1;
        int h2 = *(int*)base2;
        bool headerOk = (h1 == n) && (h2 == n);
        bool distinctBases = base1 != base2;
        ok = headerOk && distinctBases;
        // Free the underlying blocks
        free(base1);
        free(base2);
    }

    recordResult("testMalloc_multiple_calls_independent: distinct allocations with correct headers", ok);
}

// Test 4: Large n and boundary checks (optional extra coverage)
// - Verifies that a larger allocation still stores the correct header value and is zeroed
static void testMalloc_largeN_boundary() {
    const int n = 64;
    void* res = testMalloc(n);
    bool ok = false;
    if (res != nullptr) {
        unsigned char* r = (unsigned char*)res;
        void* base = (void*)(r - 8);
        int headerVal = *(int*)base;
        bool headerOk = (headerVal == n);
        bool middleZero = (*(int*)((char*)base + 4) == 0);
        bool memZero = true;
        for (int i = 0; i < n; ++i) {
            if (r[i] != 0) { memZero = false; break; }
        }
        bool alignedOk = (reinterpret_cast<uintptr_t>(res) % 8) == 0;
        ok = headerOk && middleZero && memZero && alignedOk;
        free(base);
    }

    recordResult("testMalloc_largeN_boundary: header, middle, memory zeroed, alignment", ok);
}

// Main test runner
int main() {
    std::cout << "Starting testMalloc unit tests (C++11 harness):\n";

    testMalloc_basicZeroLength();
    testMalloc_nonZeroLength();
    testMalloc_multiple_calls_independent();
    testMalloc_largeN_boundary();

    // Summary
    std::cout << "Test summary: " << g_passed << "/" << g_total << " passed, "
              << g_failed << " failed.\n";

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}
// Lightweight C++11 test suite for the focal method sqlite3RCStrNew
// This test suite does not rely on GTest. It uses a small, non-terminating
// assertion approach suitable for stepping through tests from main().
//
// Focal method under test (from provided snippet):
//   char *sqlite3RCStrNew(u64 N){
//     RCStr *p = sqlite3_malloc64( N + sizeof(*p) + 1 );
//     if( p==0 ) return 0;
//     p->nRCRef = 1;
//     return (char*)&p[1];
//   }
//
// Important notes for tests:
// - We cannot directly inspect the RCStr internals without knowledge of its layout.
// - Therefore tests focus on allocation outcome (success/failure) and basic
//   memory accessibility of the returned region (i.e., that we can write into it).
// - The memory allocator function sqlite3_malloc64 is provided by the test
//   harness (extern "C") to allow simulating success/failure.
// - This approach exercises the observable behavior of sqlite3RCStrNew without
//   depending on static/private internals.

#include <cstdio>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstdint>


// Declaration of the focal function under test.
// The function uses unsigned 64-bit size (u64) in the original C code; we map it to uint64_t.
extern "C" char* sqlite3RCStrNew(uint64_t N);

// Global flag to simulate allocation failure in sqlite3_malloc64
static bool simulateOOM = false;

// Provide the allocator that the focal method will call.
// We expose sqlite3_malloc64 with C linkage so the linker resolves to this symbol
// when sqlite3RCStrNew is built in the same test binary.
extern "C" void* sqlite3_malloc64(size_t n) {
    if (simulateOOM) return nullptr;
    return std::malloc(n);
}

// Lightweight, non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "EXPECT_TRUE FAILED: %s (in %s:%d) - %s\n", #cond, __FILE__, __LINE__, (msg)); \
            ++g_failed_tests; \
        } \
        ++g_total_tests; \
    } while (0)

#define EXPECT_NOT_NULL(ptr, msg) \
    EXPECT_TRUE((ptr) != nullptr, (msg))

#define EXPECT_NULL(ptr, msg) \
    EXPECT_TRUE((ptr) == nullptr, (msg))

#define EXPECT_EQ(a, b, format, msg) \
    do { \
        if (!((a) == (b))) { \
            fprintf(stderr, "EXPECT_EQ FAILED: " format " != " format " (%s) (in %s:%d) - %s\\n", \
                    (unsigned long long)(a), (unsigned long long)(b), #a, __FILE__, __LINE__, (msg)); \
            ++g_failed_tests; \
        } \
        ++g_total_tests; \
    } while (0)

// Test 1: Allocation succeeds for a typical non-zero size and returned pointer is usable.
// This validates the success path and basic memory accessibility of the returned region.
bool test_succeeds_allocation_and_memory_access() {
    // Ensure allocator is in success mode
    simulateOOM = false;

    const uint64_t N = 16; // typical small size
    char *ret = sqlite3RCStrNew(N);

    // Expect non-null on success
    EXPECT_NOT_NULL(ret, "sqlite3RCStrNew(N) should return non-null on successful allocation");

    if (ret != nullptr) {
        // Basic sanity: the returned region should be writable for at least N bytes.
        // The function returns a pointer to the memory just after the RCStr header, so
        // there should be at least N bytes available to use (as allocated: N + header + 1).
        for (uint64_t i = 0; i < N; ++i) {
            ret[i] = static_cast<char>(i & 0xFF);
        }
        // No crashes or writes beyond allocated memory should occur in this simple test.
    }

    return ret != nullptr;
}

// Test 2: Allocation zero size (N == 0) should still allocate something
// (since we allocate N + sizeof(*p) + 1 bytes). We verify non-null and basic usability.
bool test_zero_size_allocation() {
    simulateOOM = false;

    const uint64_t N = 0;
    char *ret = sqlite3RCStrNew(N);

    EXPECT_NOT_NULL(ret, "sqlite3RCStrNew(0) should return non-null (minimum header + 1) even for N==0");

    if (ret != nullptr) {
        // The region should be writable at least for one byte
        ret[0] = 'Z';
    }

    return ret != nullptr;
}

// Test 3: Allocation failure path (OOM) returns NULL
// This exercises the false branch in the focal method.
bool test_allocation_failure_returns_null() {
    simulateOOM = true;
    const uint64_t N = 8;
    char *ret = sqlite3RCStrNew(N);

    EXPECT_NULL(ret, "sqlite3RCStrNew should return NULL when allocator fails (OOM simulated)");
    // Reset for subsequent tests
    simulateOOM = false;
    return ret == nullptr;
}

// Test 4: Multiple consecutive allocations succeed
// Ensures that repeated calls do not interfere with each other under normal conditions.
bool test_multiple_consecutive_allocations() {
    simulateOOM = false;

    const uint64_t N1 = 4;
    const uint64_t N2 = 2;

    char *ret1 = sqlite3RCStrNew(N1);
    char *ret2 = sqlite3RCStrNew(N2);

    EXPECT_NOT_NULL(ret1, "First allocation (N1) should succeed");
    EXPECT_NOT_NULL(ret2, "Second allocation (N2) should succeed");

    // Basic usability checks on both regions if both succeeded
    if (ret1) {
        for (uint64_t i = 0; i < N1; ++i) ret1[i] = static_cast<char>(i);
    }
    if (ret2) {
        for (uint64_t i = 0; i < N2; ++i) ret2[i] = static_cast<char>(i + 100);
    }

    return ret1 != nullptr && ret2 != nullptr;
}

// Main: Run all tests from this single translation unit
int main() {
    fprintf(stdout, "Starting sqlite3RCStrNew unit tests (C++11, no GTest).\n");

    bool ok;

    ok = test_succeeds_allocation_and_memory_access();
    if (!ok) {
        fprintf(stderr, "Test 1 failed.\n");
    }

    ok = test_zero_size_allocation();
    if (!ok) {
        fprintf(stderr, "Test 2 failed.\n");
    }

    ok = test_allocation_failure_returns_null();
    if (!ok) {
        fprintf(stderr, "Test 3 failed.\n");
    }

    ok = test_multiple_consecutive_allocations();
    if (!ok) {
        fprintf(stderr, "Test 4 failed.\n");
    }

    fprintf(stdout, "Tests completed: %d run, %d failed.\n", g_total_tests, g_failed_tests);

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}
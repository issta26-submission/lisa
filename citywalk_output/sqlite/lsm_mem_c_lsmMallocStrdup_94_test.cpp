// Unit test suite for the focal method lsmMallocStrdup (in lsm_mem.c)
// This test is crafted for C++11 without Google Test.
// It uses a lightweight, non-terminating assertion mechanism and a small
// mock of the lsmMalloc allocator to drive the desired test scenarios.

#include <cstdlib>
#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Provide C-linkage for the focal function and its dependencies to allow
// linking with the C file lsm_mem.c.
extern "C" {

// Forward declaration of the environment type as an opaque struct.
// The real project would provide a complete type; for tests we only need a
// type to pass around as a pointer (no dereferencing required).
typedef struct lsm_env lsm_env;

// Prototypes matching the focal method and its allocator/deallocator used by it.
char *lsmMallocStrdup(lsm_env *pEnv, const char *zIn);
void *lsmMalloc(lsm_env *pEnv, size_t N);
void lsmFree(lsm_env *pEnv, void *p);

}

// Lightweight, deterministic mock for lsmMalloc to drive test scenarios.
// - Succeeds for small allocations (returns a valid pointer, like malloc).
// - Fails (returns nullptr) for allocations larger than a threshold to test
//   the "allocation failure" branch in lsmMallocStrdup.
static const size_t kMallocFailThreshold = 16; // N > 16 will fail in tests.

extern "C" void *lsmMalloc(lsm_env *pEnv, size_t N) {
    (void)pEnv; // Unused in tests; we only care about allocation result.
    if (N > kMallocFailThreshold) {
        return nullptr; // Simulate allocation failure for large requests.
    }
    return std::malloc(N);
}

extern "C" void lsmFree(lsm_env *pEnv, void *p) {
    (void)pEnv; // Unused in tests; we only care about free behavior.
    std::free(p);
}

// Lightweight assertion framework (non-terminating).
static int g_test_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

static void test_empty_string_duplication() {
    // Test 1: Duplicating an empty string should allocate 1 byte and copy the terminator.
    const char *input = "";
    char *dup = lsmMallocStrdup(nullptr, input);
    ASSERT_TRUE(dup != nullptr, "Duplicating empty string should not return NULL");
    if (dup) {
        // Content should be identical (empty string -> just terminator)
        ASSERT_TRUE(dup[0] == '\0', "Duplicated empty string should start with null terminator");
        // The terminator should be properly copied (length of input is 0, so position 0 is '\0')
        // Clean up
        lsmFree(nullptr, dup);
    }
}

static void test_basic_duplication() {
    // Test 2: Basic duplication of a regular string.
    const char *input = "hello, world";
    char *dup = lsmMallocStrdup(nullptr, input);
    ASSERT_TRUE(dup != nullptr, "Duplicating a normal string should not return NULL");
    if (dup) {
        // Content must match
        ASSERT_TRUE(std::strcmp(dup, input) == 0, "Duplicated string content must match input");
        // Ensure distinct memory (not the same pointer as input literal)
        ASSERT_TRUE(dup != input, "Duplicated string should reside at a different memory address");
        // Ensure the copied string is null-terminated and terminator copied
        size_t len = std::strlen(input);
        ASSERT_TRUE(dup[len] == '\0', "Duplicated string must be null-terminated after copied content");
        lsmFree(nullptr, dup);
    }
}

static void test_duplication_failure_path() {
    // Test 3: Allocation failure path should return NULL without crashing.
    // Use a string whose length forces N > kMallocFailThreshold.
    // For N = strlen(input) + 1 to be > 16, we pick a sufficiently long string.
    const char *input = "1234567890abcdef"; // length 16
    // N will be 17, which should trigger failure per our mock.
    // We ensure that lsmMallocStrdup returns NULL and does not crash.
    char *dup = lsmMallocStrdup(nullptr, input);
    // Since we designed the allocator to fail for N > 16, 17 should fail.
    ASSERT_TRUE(dup == nullptr, "Duplication should return NULL on allocation failure");
    // No cleanup needed since dup is NULL.
}

static void test_terminator_copy_inclusion() {
    // Test 4: Ensure the copied content includes the null terminator at the correct position.
    const char *input = "abc";
    char *dup = lsmMallocStrdup(nullptr, input);
    ASSERT_TRUE(dup != nullptr, "Duplicating small string should succeed");
    if (dup) {
        size_t n = std::strlen(input);
        // The position after the last character should be the terminator
        ASSERT_TRUE(dup[n] == '\0', "Zero terminator must be copied into the correct position");
        // Ensure content correctness
        ASSERT_TRUE(std::strcmp(dup, input) == 0, "Duplicated content should equal input");
        lsmFree(nullptr, dup);
    }
}

int main() {
    // Run all tests
    test_empty_string_duplication();
    test_basic_duplication();
    test_duplication_failure_path();
    test_terminator_copy_inclusion();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
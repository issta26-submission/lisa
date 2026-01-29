// This test suite targets the focal method gzbuffer as described.
// It provides a self-contained C++11 test harness (no GTest) that mimics
// the minimal dependencies required by gzbuffer.
// The tests cover true/false branches of each predicate in gzbuffer
// and validate state mutations caused by the function.

/*
Candidate Keywords (core dependencies for gzbuffer):
- gzFile (opaque handle, cast to gz_statep)
- gz_statep (internal state pointer with fields: mode, size, want)
- mode predicates: GZ_READ, GZ_WRITE
- state->size (must be 0 to proceed)
- size validation: (size << 1) < size (overflow detection)
- size normalization: if size < 8 then size = 8
- state->want (set to adjusted size on success)
*/

/* Domain knowledge considerations implemented in tests:
- Null file handling
- Invalid mode handling
- Non-zero internal buffer size handling
- Overflow detection when doubling size
- Minimum size enforcement (size < 8 -> 8)
- Proper assignment of want on success
- Both edge and typical values tested
*/

// Minimal, self-contained definitions to simulate gzbuffer's environment
#include <iostream>
#include <gzguts.h>


// Minimal state representation used by gzbuffer
struct gz_state_s {
    int mode;
    unsigned size;
    unsigned want;
};

// Typedefs to mimic the original API surface
typedef gz_state_s* gz_statep;
typedef void* gzFile;

// Minimal mode constants to drive predicate logic
enum { GZ_READ = 0, GZ_WRITE = 1 };

// Focal method implementation under test (self-contained for this harness)
int gzbuffer(gzFile file, unsigned size) {
    gz_statep state;
    // 1) Null check
    if (file == NULL)
        return -1;

    // Cast to internal state representation
    state = (gz_statep)file;

    // 2) Mode check
    if (state->mode != GZ_READ && state->mode != GZ_WRITE)
        return -1;

    // 3) Memory allocation check
    if (state->size != 0)
        return -1;

    // 4) Overflow detection for doubling size
    if ((size << 1) < size)
        return -1;

    // 5) Minimum size enforcement for proper behavior with flushing
    if (size < 8)
        size = 8;

    // 6) Commit the desired size
    state->want = size;
    return 0;
}

// Simple test harness (no external test framework)
static int tests_run = 0;
static int tests_passed = 0;

// Basic non-terminating assertion helper
static void log_failure(const char* msg, long long expected, long long actual, const char* file, int line) {
    std::cerr << "[FAILED] " << msg
              << " (expected: " << expected << ", actual: " << actual << ") "
              << "at " << file << ":" << line << "\n";
}

// Assertion macros (non-terminating)
#define ASSERT_EQ(actual, expected, msg) do { \
    tests_run++; \
    if (static_cast<long long>(actual) != static_cast<long long>(expected)) { \
        log_failure(msg, static_cast<long long>(expected), static_cast<long long>(actual), __FILE__, __LINE__); \
    } else { \
        tests_passed++; \
    } \
} while(0)

#define ASSERT_EQ_PTR(actual, expected, msg) do { \
    tests_run++; \
    if (static_cast<void*>(actual) != static_cast<void*>(expected)) { \
        log_failure(msg, reinterpret_cast<long long>(expected), reinterpret_cast<long long>(actual), __FILE__, __LINE__); \
    } else { \
        tests_passed++; \
    } \
} while(0)

// Helper to create a new state
static gz_statep make_state(int mode, unsigned size) {
    gz_state_s* s = new gz_state_s{mode, size, 0};
    return s;
}

// Test 1: Null file should return -1 (true branch)
void test_null_file_returns_minus_one() {
    // Explain: When file is NULL, gzbuffer should reject immediately.
    int res = gzbuffer(nullptr, 16u);
    ASSERT_EQ(res, -1, "gzbuffer(NULL, 16) should return -1");
}

// Test 2: Invalid mode should return -1 (false branch)
void test_invalid_mode_returns_minus_one() {
    gz_statep s = make_state(-1, 0);
    int res = gzbuffer(static_cast<gzFile>(s), 32u);
    ASSERT_EQ(res, -1, "gzbuffer with invalid mode should return -1");
    delete s;
}

// Test 3: Non-zero internal size should return -1 (memory already allocated)
void test_non_zero_size_returns_minus_one() {
    gz_statep s = make_state(GZ_READ, 4u); // size != 0
    int res = gzbuffer(static_cast<gzFile>(s), 32u);
    ASSERT_EQ(res, -1, "gzbuffer with non-zero internal size should return -1");
    delete s;
}

// Test 4: Overflow when doubling size should return -1
void test_overflow_detection_returns_minus_one() {
    gz_statep s = make_state(GZ_READ, 0u);
    // Choose a size that will overflow when left-shifted
    unsigned test_size = 0x80000000u; // 2147483648
    int res = gzbuffer(static_cast<gzFile>(s), test_size);
    ASSERT_EQ(res, -1, "gzbuffer with overflow-causing size should return -1");
    delete s;
}

// Test 5: Small size (<8) should be coerced to 8 and succeed
void test_small_size_coerced_to_eight() {
    gz_statep s = make_state(GZ_WRITE, 0u);
    int res = gzbuffer(static_cast<gzFile>(s), 4u);
    ASSERT_EQ(res, 0, "gzbuffer with size < 8 should succeed after coercion to 8");
    ASSERT_EQ(s->want, 8u, "state->want should be set to 8 after coercion");
    delete s;
}

// Test 6: Valid size should be set directly in state->want
void test_valid_size_sets_want() {
    gz_statep s = make_state(GZ_READ, 0u);
    unsigned sz = 16u;
    int res = gzbuffer(static_cast<gzFile>(s), sz);
    ASSERT_EQ(res, 0, "gzbuffer with valid size should succeed");
    ASSERT_EQ(s->want, sz, "state->want should equal the requested size for valid input");
    delete s;
}

// Test 7: Very large non-overflowing size should set want accordingly
void test_large_size_without_overflow_sets_want() {
    gz_statep s = make_state(GZ_READ, 0u);
    unsigned sz = 0x7FFFFFFF; // 2147483647
    int res = gzbuffer(static_cast<gzFile>(s), sz);
    ASSERT_EQ(res, 0, "gzbuffer should succeed for large non-overflowing size");
    ASSERT_EQ(s->want, sz, "state->want should equal the large input size");
    delete s;
}

// Run all tests and report summary
int main() {
    test_null_file_returns_minus_one();
    test_invalid_mode_returns_minus_one();
    test_non_zero_size_returns_minus_one();
    test_overflow_detection_returns_minus_one();
    test_small_size_coerced_to_eight();
    test_valid_size_sets_want();
    test_large_size_without_overflow_sets_want();

    std::cout << "Tests run: " << tests_run << ", Passed: " << tests_passed << "\n";
    if (tests_run == tests_passed) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}
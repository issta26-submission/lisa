// Unit test suite for the focal method been_here (reproduced locally for testing purposes).
// Note: This test harness mirrors the core logic of the provided been_here function
// and its global state (g), but is self-contained so it can be compiled with a C++11
// compiler without GTest. It uses a lightweight, non-terminating assertion style
// (EXPECT_* macros) to maximize code coverage across true/false branches.
// The tests are designed to exercise the vector growth logic and bit manipulation.

#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <cassert>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>


// Reproduced core dependencies (local translations of the FOCAL_METHOD's environment)
static void string_clear(void *) {}
static void string_init(void *) {}
static void string_free(void *) {}
static void string_printf(void *, const char *, ...) {}
static inline size_t map(int, int, int) { return 0; } // map() stub: force index 0 for tests
static void cleanup(void) {}
static size_t count(int, int, int) { return 0; }

// Global state mirroring the production code's g structure
struct VecEntry {
    size_t len;
    unsigned char *vec;
};
struct Glob {
    int root;
    VecEntry *done;
} g = {0, nullptr};

// Simple assertion helpers (non-terminating)
static bool EXPECT_TRUE(bool cond, const char *msg) {
    if (!cond) {
        std::cerr << "EXPECT_TRUE failed: " << (msg ? msg : "") << std::endl;
        return false;
    }
    return true;
}
static bool EXPECT_EQ(int a, int b, const char *msg) {
    if (a != b) {
        std::cerr << "EXPECT_EQ failed: " << a << " != " << b
                  << "  // " << (msg ? msg : "") << std::endl;
        return false;
    }
    return true;
}
static bool EXPECT_PTR_EQ(void *a, void *b, const char *msg) {
    if (a != b) {
        std::cerr << "EXPECT_PTR_EQ failed: " << a << " != " << b
                  << "  // " << (msg ? msg : "") << std::endl;
        return false;
    }
    return true;
}

// Reproduced been_here logic (local copy to enable thorough unit testing in C++)
static int been_here_test(int syms, int left, int len, int mem, int rem) {
    size_t index = map(syms, left, len);
    mem -= 1 << g.root;
    mem >>= 1;
    rem >>= 1;
    size_t offset = (mem >> 3) + rem;
    offset = ((offset * (offset + 1)) >> 1) + rem;
    int bit = 1 << (mem & 7);
    size_t length = g.done[index].len;
    if (offset < length && (g.done[index].vec[offset] & bit) != 0)
        return 1;
    if (length <= offset) {
        unsigned char *vector;
        if (length) {
            do {
                length <<= 1;
            } while (length <= offset);
            vector = (unsigned char*)realloc(g.done[index].vec, length);
            assert(vector != NULL && "out of memory");
            memset(vector + g.done[index].len, 0, length - g.done[index].len);
        } else {
            length = 16;
            while (length <= offset)
                length <<= 1;
            vector = (unsigned char*)calloc(length, 1);
            assert(vector != NULL && "out of memory");
        }
        g.done[index].len = length;
        g.done[index].vec = vector;
    }
    g.done[index].vec[offset] |= bit;
    return 0;
}

// Helper to reset global state before each test
static void reset_globals(size_t done_size) {
    // Free any previously allocated vectors
    if (g.done) {
        for (size_t i = 0; i < (size_t)done_size; ++i) {
            if (g.done[i].vec) {
                free(g.done[i].vec);
                g.done[i].vec = nullptr;
            }
        }
        free(g.done);
        g.done = nullptr;
    }
    g.root = 0;
    g.done = (VecEntry*)calloc(done_size, sizeof(VecEntry));
    for (size_t i = 0; i < done_size; ++i) {
        g.done[i].len = 0;
        g.done[i].vec = nullptr;
    }
}

// Unit test 1: Initial insertion should create a new vector and set the bit
// Expected: returns 0 and vec[0] has bit set (bit depends on mem).
static bool test_initial_insertion() {
    reset_globals(1); // index 0 will be used
    // mem=4 -> after calculations: offset=0, bit=1<<( (mem-1)>>1 & 7 ) -> mem-1 = 3; >>1 =>1 ; bit = 1<<1 = 2
    int ret = been_here_test(0, 0, 0, 4, 0);
    bool ok = true;
    ok &= EXPECT_EQ(ret, 0, "Initial call should return 0 (not seen before)");
    ok &= EXPECT_TRUE(g.done[0].len == 16 || g.done[0].len == 32, "Vector should be grown to a non-zero length");
    ok &= EXPECT_TRUE(g.done[0].vec[0] == 2, "vec[0] should have bit 2 set after initial insertion");
    return ok;
}

// Unit test 2: Revisit with same parameters should detect 'done this'
static bool test_revisit_same_params() {
    // Already prepared by test 1, reuse
    int ret = been_here_test(0, 0, 0, 4, 0);
    bool ok = true;
    ok &= EXPECT_EQ(ret, 1, "Second call with same params should return 1 (already done)");
    return ok;
}

// Unit test 3: Trigger vector growth path then verify vector length and bit set at offset
static bool test_vector_growth_and_offset() {
    reset_globals(1);
    // Use mem_in that yields offset=21 (requires m = floor((mem-1)/2) in [48,55], i.e., mem in [97,111])
    int ret = been_here_test(0, 0, 0, 97, 0);
    bool ok = true;
    ok &= EXPECT_EQ(ret, 0, "Growth path should return 0 on first insert");
    // After growth, length should be at least 32 (we start from 16, double to 32)
    ok &= EXPECT_TRUE(g.done[0].len == 32, "Vector length should have grown to 32");
    // offset computed to 21, so vec[21] should have bit set to 1
    ok &= EXPECT_TRUE((g.done[0].vec[21] & 1) != 0, "Bit at vec[21] should be set after growth");
    return ok;
}

// Unit test 4: Revisit after growth with same parameters should be detected
static bool test_growth_revisit() {
    int ret = been_here_test(0, 0, 0, 97, 0);
    bool ok = true;
    ok &= EXPECT_EQ(ret, 1, "Second visit after growth should return 1");
    return ok;
}

// Unit test 5: Separate path with a small offset to verify non-root bit setting without growth
static bool test_small_offset_bitsetting() {
    reset_globals(1);
    // mem_in=3 yields m=(3-1)>>1 = 1; offset=0; bit=1<<(1)=2
    int ret = been_here_test(0, 0, 0, 3, 0);
    bool ok = true;
    ok &= EXPECT_EQ(ret, 0, "Small offset first insert should return 0");
    ok &= EXPECT_TRUE(g.done[0].vec[0] == 2, "vec[0] should have bit 2 set after first small-offset insert");
    // Second call should return 1
    ret = been_here_test(0, 0, 0, 3, 0);
    ok &= EXPECT_EQ(ret, 1, "Second small-offset visit should return 1");
    return ok;
}

// Helper to run all tests and print summary
int main() {
    bool all_passed = true;

    std::cout << "Running test_initial_insertion...\n";
    all_passed &= test_initial_insertion();
    std::cout << "Test initial_insertion " << (test_initial_insertion() ? "PASSED" : "FAILED") << "\n";

    std::cout << "Running test_revisit_same_params...\n";
    all_passed &= test_revisit_same_params();
    std::cout << "Test revisit_same_params " << (test_revisit_same_params() ? "PASSED" : "FAILED") << "\n";

    std::cout << "Running test_vector_growth_and_offset...\n";
    all_passed &= test_vector_growth_and_offset();
    std::cout << "Test vector_growth_and_offset " << (test_vector_growth_and_offset() ? "PASSED" : "FAILED") << "\n";

    std::cout << "Running test_growth_revisit...\n";
    all_passed &= test_growth_revisit();
    std::cout << "Test growth_revisit " << (test_growth_revisit() ? "PASSED" : "FAILED") << "\n";

    std::cout << "Running test_small_offset_bitsetting...\n";
    all_passed &= test_small_offset_bitsetting();
    std::cout << "Test small_offset_bitsetting " << (test_small_offset_bitsetting() ? "PASSED" : "FAILED") << "\n";

    // Cleanup
    reset_globals(1);

    std::cout << "\nSummary: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";
    return all_passed ? 0 : 1;
}
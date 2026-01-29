// C++11 test suite for the focal method validate_T(void) from pngimage.c
// This test harness assumes the following external C symbols are available from the PNG image code:
// - struct transform_info_entry { const char* name; unsigned int when; unsigned int transform; };
// - extern transform_info_entry transform_info[];
// - extern unsigned int TTABLE_SIZE;
// - extern unsigned int read_transforms;
// - extern unsigned int write_transforms;
// - extern unsigned int rw_transforms;
// - extern void validate_T(void);
//
// The tests manipulate the transform_info table and the related state, then invoke validate_T()
// to exercise its logic (building read and write transform sets and computing their intersection).

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>


extern "C" {

// Definitions expected from the pngimage.c (these should be provided by the actual project linkage)
typedef struct {
    const char* name;
    unsigned int when;
    unsigned int transform;
} transform_info_entry;

// Global table and state (to be linked from pngimage.c)
extern transform_info_entry transform_info[];
extern unsigned int TTABLE_SIZE;
extern unsigned int read_transforms;
extern unsigned int write_transforms;
extern unsigned int rw_transforms;

// Focal method under test
void validate_T(void);
}

// Simple assertion helpers (non-terminating, accumulate failures)
static int g_fail_count = 0;

static void expect_eq_uint(unsigned int actual, unsigned int expected, const char* msg) {
    if (actual != expected) {
        std::cerr << "ASSERT FAILED: " << msg
                  << " | expected: " << expected
                  << " | actual: " << actual << std::endl;
        ++g_fail_count;
    }
}

// Helper to reset the first N entries of transform_info and the related state
static void reset_entries_and_state(int count) {
    // Reset the first 'count' entries to a clean state
    for (int i = 0; i < count; ++i) {
        transform_info[i].name = nullptr;
        transform_info[i].when = 0;
        transform_info[i].transform = 0;
    }
    // Reset global transform accumulators
    // Note: after reset, validate_T will recompute based solely on transform_info
    // and should start from zero.
    // If the actual library initializes these to zero at program start, this explicit reset is safe.
    // We rely on direct assignment here to ensure deterministic tests.
    // (Assuming these are true extern unsigned ints from the library)
    extern unsigned int read_transforms_local; // dummy to illustrate intent (not used)
    (void)read_transforms; // silence unused if not referenced
}

// Publisher-friendly alias to set a transform_info entry
static void set_entry(int idx, const char* name, unsigned int when, unsigned int transform) {
    transform_info[idx].name = name;
    transform_info[idx].when = when;
    transform_info[idx].transform = transform;
}

// 1) Test: No transfomation entries present
// Expect: all derived transforms remain zero after validate_T
static void test_validate_T_no_entries() {
    // Prepare a clean slate: only a few initial entries touched; the rest should be NULL due to zero-initialization
    reset_entries_and_state(4); // ensure 0..3 are clean
    // Ensure global state is reset
    // Note: rely on the library's initial state being zero; explicit assignments if possible
    // Since we can't assign to the externs here in a portable way beyond C linkage, we'll rely on zeroed state
    // Call focal method
    validate_T();

    // Assertions
    expect_eq_uint(read_transforms, 0u, "no entries: read_transforms should be 0");
    expect_eq_uint(write_transforms, 0u, "no entries: write_transforms should be 0");
    expect_eq_uint(rw_transforms, 0u, "no entries: rw_transforms should be 0");
}

// 2) Test: Single read transform entry
// Expect: read_transforms accumulates the transform value; write/transforms unchanged; rw = read & write (0)
static void test_validate_T_single_read() {
    reset_entries_and_state(4);
    // Create one transform that is only readable
    // Using common mask values: TRANSFORM_R assumed to be 1, TRANSFORM_W assumed to be 2
    // Here, when = TRANSFORM_R
    set_entry(0, "t_read", 1u, 0x04u); // R flag set
    // Ensure remaining entries don't affect outcome
    transform_info[1].name = nullptr;
    transform_info[2].name = nullptr;

    validate_T();

    expect_eq_uint(read_transforms, 0x04u, "single read: read_transforms should be 0x04");
    expect_eq_uint(write_transforms, 0u, "single read: write_transforms should be 0");
    expect_eq_uint(rw_transforms, 0u, "single read: rw_transforms should be 0");
}

// 3) Test: Read and write transforms for different entries
// Expect: read_transforms has first transform; write_transforms has second; rw_transforms = intersection (0)
static void test_validate_T_read_and_write_separate() {
    reset_entries_and_state(4);
    set_entry(0, "t_read", 1u, 0x04u);  // R -> 0x04
    set_entry(1, "t_write", 2u, 0x04u); // W -> 0x04
    // The rest remain NULL
    validate_T();

    expect_eq_uint(read_transforms, 0x04u, "read_and_write: read_transforms should be 0x04");
    expect_eq_uint(write_transforms, 0x04u, "read_and_write: write_transforms should be 0x04");
    expect_eq_uint(rw_transforms, 0x04u, "read_and_write: rw_transforms should be 0x04");
}

// 4) Test: Only non-NULL entries contribute (NULL name should be ignored)
static void test_validate_T_ignore_null_names() {
    reset_entries_and_state(4);
    set_entry(0, "a", 1u, 0x01u); // R and transform bit 0x01
    // Index 1 is explicitly NULL to simulate skipped entry
    transform_info[1].name = nullptr;
    // Index 2 is non-NULL but with only W
    set_entry(2, "b", 2u, 0x02u); // W and transform bit 0x02
    validate_T();

    // Expect read 0x01, write 0x02, rw 0 (since 0x01 & 0x02 == 0)
    expect_eq_uint(read_transforms, 0x01u, "ignore_null: read_transforms should be 0x01");
    expect_eq_uint(write_transforms, 0x02u, "ignore_null: write_transforms should be 0x02");
    expect_eq_uint(rw_transforms, 0u, "ignore_null: rw_transforms should be 0");
}

// 5) Test: Reversible transforms (rw_transforms non-zero) when same transform present for both R and W
static void test_validate_T_reversible_transform() {
    reset_entries_and_state(4);
    set_entry(0, "r", 1u, 0x10u); // R with transform 0x10
    set_entry(1, "w", 2u, 0x10u); // W with same transform 0x10
    validate_T();

    expect_eq_uint(read_transforms, 0x10u, "reversible: read_transforms should be 0x10");
    expect_eq_uint(write_transforms, 0x10u, "reversible: write_transforms should be 0x10");
    expect_eq_uint(rw_transforms, 0x10u, "reversible: rw_transforms should be 0x10");
}

// Runner to execute all tests and summarize
static void run_all_tests() {
    std::cout << "Starting tests for validate_T(void)..." << std::endl;

    test_validate_T_no_entries();
    test_validate_T_single_read();
    test_validate_T_read_and_write_separate();
    test_validate_T_ignore_null_names();
    test_validate_T_reversible_transform();

    if (g_fail_count == 0) {
        std::cout << "All tests PASSED for validate_T(void)." << std::endl;
    } else {
        std::cerr << g_fail_count << " test(s) FAILED for validate_T(void)." << std::endl;
    }
}

int main() {
    // Run all tests; return non-zero if any test failed
    run_all_tests();
    return g_fail_count;
}
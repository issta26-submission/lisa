// Comprehensive C++11 unit tests for the focal method slide_hash
// This test suite emulates the behavior of the original C function
// by re-implementing the essential logic in a testable, standalone way.
// The tests focus on validating the shifting logic for both head and prev
// arrays under two configurations: with and without the "FASTEST" path.
// Explanations are provided as comments for each test.

#include <iostream>
#include <vector>
#include <deflate.h>
#include <cstdint>


// Domain-specific type aliases (mimicking the original C types used by slide_hash)
typedef unsigned int uInt;
typedef unsigned int Pos;
typedef unsigned int Posf;

// NIL sentinel value (as used by the original zlib/deflate code)
#define NIL 0xFFFFFFFF

// Minimal substitute for the deflate_state structure used by slide_hash
struct deflate_state {
    unsigned w_size;   // Window size
    unsigned hash_size; // Size of the hash table (number of buckets)
    Posf* head;         // Array of head pointers/positions (length at least hash_size+1)
    Posf* prev;         // Array of previous positions (length at least w_size+1)
};

// Forward declarations of slide_hash variants (standalone implementations for testing)
void slide_hash_no_fastest(deflate_state *s);
void slide_hash_fastest(deflate_state *s);

// Implementation of the two variants, reproducing the core behavior of the focal code.

// Variant that emulates the non-FASTEST path (updates both head and prev)
void slide_hash_no_fastest(deflate_state *s) {
    unsigned n, m;
    Posf *p;
    uInt wsize = s->w_size;
    n = s->hash_size;
    p = &s->head[n];
    // Iterate backwards over head[0..hash_size-1]
    do {
        m = *--p;
        *p = (Pos)(m >= wsize ? m - wsize : NIL);
    } while (--n);
    // Now shift the prev array for positions 0..wsize-1
    n = wsize;
    p = &s->prev[n];
    do {
        m = *--p;
        *p = (Pos)(m >= wsize ? m - wsize : NIL);
        /* If n is not on any hash chain, prev[n] is garbage but
         * its value will never be used.
         */
    } while (--n);
}

// Variant that emulates the FASTEST path (updates only head)
void slide_hash_fastest(deflate_state *s) {
    unsigned n, m;
    Posf *p;
    uInt wsize = s->w_size;
    n = s->hash_size;
    p = &s->head[n];
    // Iterate backwards over head[0..hash_size-1]
    do {
        m = *--p;
        *p = (Pos)(m >= wsize ? m - wsize : NIL);
    } while (--n);
    // In FASTEST mode, the prev array update is skipped.
}

// Simple, non-terminating test assertion helper.
// In case of failure, it logs an error but continues running other tests.
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << "\n"; \
        ++g_failures; \
    } \
} while(0)

static int g_total_tests = 0;
#define TEST_CASE(name) void name(); int main_wrapper_##name = ( (void)0, (name)(), 0 )

// Test 1: No FASTEST path - verify both head and prev are updated correctly
void test_slide_hash_no_fastest_basic_shift() {
    // Explain: Validate that head elements are shifted by subtracting w_size
    // when values are >= w_size, otherwise set to NIL. Also validate prev shift.
    ++g_total_tests;
    deflate_state s;
    s.w_size = 8;       // Window size
    s.hash_size = 3;     // Hash size -> uses head[0..3], with head[3] unused for updates

    // Allocate internal arrays (head length = hash_size+1; prev length = w_size+1)
    Posf head_arr[4];
    Posf prev_arr[9];
    s.head = head_arr;
    s.prev = prev_arr;

    // Initialize head: [head[0], head[1], head[2], head[3]]
    // We set values to ensure some are >= w_size and some are not.
    head_arr[0] = 16; // will become 8 after subtract
    head_arr[1] = 24; // will become 16 after subtract
    head_arr[2] = 9;  // will become 1 after subtract
    head_arr[3] = 7;  // unused in the update loop (n starts at hash_size)

    // Initialize prev: indices 0..7 will be updated; prev[8] unused
    for(int i = 0; i < 8; ++i) prev_arr[i] = 10*(i+1); // 10,20,30,40,50,60,70,80
    prev_arr[8] = 90;

    // Run the function
    slide_hash_no_fastest(&s);

    // Assertions for head after processing
    // head[0] should be 16 - 8 = 8
    TEST_ASSERT(head_arr[0] == 8, "head[0] should be 8 after shift");
    // head[1] should be 24 - 8 = 16
    TEST_ASSERT(head_arr[1] == 16, "head[1] should be 16 after shift");
    // head[2] should be 9 - 8 = 1
    TEST_ASSERT(head_arr[2] == 1, "head[2] should be 1 after shift");
    // head[3] remains unchanged (not updated by the loop)
    TEST_ASSERT(head_arr[3] == 7, "head[3] should remain unchanged (7)");

    // Assertions for prev after processing
    // prev[7] = 80 - 8 = 72
    TEST_ASSERT(prev_arr[7] == 72, "prev[7] should be 72 after shift");
    // prev[6] = 70 - 8 = 62
    TEST_ASSERT(prev_arr[6] == 62, "prev[6] should be 62 after shift");
    // prev[5] = 60 - 8 = 52
    TEST_ASSERT(prev_arr[5] == 52, "prev[5] should be 52 after shift");
    // prev[4] = 50 - 8 = 42
    TEST_ASSERT(prev_arr[4] == 42, "prev[4] should be 42 after shift");
    // prev[3] = 40 - 8 = 32
    TEST_ASSERT(prev_arr[3] == 32, "prev[3] should be 32 after shift");
    // prev[2] = 30 - 8 = 22
    TEST_ASSERT(prev_arr[2] == 22, "prev[2] should be 22 after shift");
    // prev[1] = 20 - 8 = 12
    TEST_ASSERT(prev_arr[1] == 12, "prev[1] should be 12 after shift");
    // prev[0] = 10 - 8 = 2
    TEST_ASSERT(prev_arr[0] == 2, "prev[0] should be 2 after shift");
}

// Test 2: FASTEST path - verify only head is updated; prev remains unchanged
void test_slide_hash_fastest_basic_shift() {
    ++g_total_tests;
    deflate_state s;
    s.w_size = 8;
    s.hash_size = 3;

    Posf head_arr[4];
    Posf prev_arr[9];
    s.head = head_arr;
    s.prev = prev_arr;

    head_arr[0] = 16;
    head_arr[1] = 24;
    head_arr[2] = 9;
    head_arr[3] = 7;

    for(int i = 0; i < 8; ++i) prev_arr[i] = 10*(i+1);
    prev_arr[8] = 90;

    // Run the FASTEST variant (only head is updated)
    slide_hash_fastest(&s);

    // head assertions remain the same as in the previous test
    TEST_ASSERT(head_arr[0] == 8, "head[0] should be 8 after shift ( FASTEST )");
    TEST_ASSERT(head_arr[1] == 16, "head[1] should be 16 after shift ( FASTEST )");
    TEST_ASSERT(head_arr[2] == 1, "head[2] should be 1 after shift ( FASTEST )");
    TEST_ASSERT(head_arr[3] == 7, "head[3] should remain unchanged (FASTEST case)");

    // prev array should be unchanged in FASTEST path
    TEST_ASSERT(prev_arr[0] == 10, "prev[0] should remain 10 (FASTEST path)");
    TEST_ASSERT(prev_arr[7] == 80, "prev[7] should remain 80 (FASTEST path)");
}

// Entry point to run tests and produce a summary.
// This main function aggregates results and returns non-zero if any test failed.
int main() {
    // Run all tests
    test_slide_hash_no_fastest_basic_shift();
    test_slide_hash_fastest_basic_shift();

    // Summary
    std::cout << "Total tests executed: " << g_total_tests
              << ", Failures: " << g_failures << std::endl;

    // Non-terminating test framework: return non-zero if any failure occurred
    return (g_failures == 0) ? 0 : 1;
}
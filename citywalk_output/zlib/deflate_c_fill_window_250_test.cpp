// Auto-generated C++11 unit-test suite for the focal method: fill_window
// This test suite reproduces a minimal environment to exercise selected paths
// inside fill_window without relying on GoogleTest. It uses a lightweight
// custom assertion mechanism and simple harness to validate internal state
// machine behavior of the C-like deflate_state driven logic.
//
// Notes:
// - The test harness provides minimal stubs for dependencies (slide_hash, read_buf, etc.)
// - The focus is on exercising these branches: basic execution, high_water zeroing,
//   and the sliding branch (copy within the window).
// - The code is self-contained and compiles as C++11; it does not rely on GTest.

#include <cstdio>
#include <cstring>
#include <deflate.h>
#include <vector>
#include <cstdlib>


// -------------------- Minimal Dependency Environment --------------------
// We port a tiny subset of the required C code environment into C++ for testing.

#define local static
#define localextern // unused in tests

typedef unsigned char Bytef;
typedef unsigned int uInt;
typedef unsigned long ulg;
typedef unsigned long long ULL;
typedef unsigned int Pos;
typedef int int_t;

// Basic zeroing/copy primitives (to mimic zmemzero/zmemcpy used by fill_window)
static void zmemcpy(Bytef* dst, const Bytef* src, unsigned size) {
    std::memmove(dst, src, size);
}
static void zmemzero(Bytef* dst, unsigned size) {
    std::memset(dst, 0, size);
}

// Forward declaration for the test environment
struct deflate_state;

// z_streamp and related stubs
struct z_stream {
    unsigned int avail_in;
};
typedef struct z_stream* z_streamp;

// Hash-related constants (simplified for test)
static const int HASH_SIZE = 32;

// Simple no-op slide_hash (real logic not required for test scenarios here)
static void slide_hash(deflate_state *s) {
    // no-op for tests
    (void)s;
}

// Simple no-op update_hash (we do not rely on real hashing in tests)
static void update_hash_stub(deflate_state *s, unsigned int h, int c) {
    (void)s; (void)h; (void)c;
}
// Macro used by the focal method to update hash; redirected to our stub
#define UPDATE_HASH(s, h, c) update_hash_stub((s), (h), (c))

// Simple read_buf implementation for tests: fills buffer with nonzero data
static unsigned read_buf(z_streamp strm, Bytef *buf, unsigned size) {
    if (strm == nullptr) return 0;
    unsigned n = size;
    if (strm->avail_in < n) n = strm->avail_in;
    if (n > 0) {
        // Fill with a recognizable pattern
        for (unsigned i = 0; i < n; ++i) buf[i] = (Bytef)(0xAB + (i & 0xFF));
        strm->avail_in -= n;
    }
    return n;
}

// -------------------- Focal State Structure and Helpers --------------------

// Minimal deflate_state mirroring the fields used by fill_window
struct deflate_state {
    unsigned int w_size;           // actual "window" size in use
    unsigned int window_size;        // total window buffer size
    Bytef* window;                   // window buffer
    unsigned int strstart;           // current position in window
    unsigned int lookahead;          // number of bytes of lookahead in window
    unsigned int insert;             // length of string to insert into the hash
    unsigned int ins_h;              // current hash value
    unsigned int w_mask;               // mask for hash table indexing (simulate)
    unsigned int *head;              // hash head table
    unsigned int *prev;              // hash previous index per bucket
    unsigned int match_start;        // start position of last match
    long block_start;                // start of current data block
    z_streamp strm;                   // associated stream
    unsigned int high_water;           // high-water mark in window
};

// Typedef Position type used in the original code
typedef unsigned int Pos;

// Macros/constants used by fill_window (simplified for tests)
#define MIN_MATCH 3
#define MIN_LOOKAHEAD 2
#define WIN_INIT 32
// MAX_DIST(s) simplified for test; we can control behavior by making it depend on w_size
#define MAX_DIST(s) ((unsigned)((s)->w_size))

#define ASSERT(cond, msg) // runtime asserts are implemented via test harness checks

// Relaxed Assert to keep tests running
static bool test_assert_fail_flag = false;
static void test_assert_check(bool cond, const char* msg, int line) {
    if (!cond) {
        printf("Assertion failed at line %d: %s\n", line, msg);
        test_assert_fail_flag = true;
    }
}
#define Assert(cond, msg) test_assert_check((cond), (msg), __LINE__)

// -------------------- Focal Method (Adapted for Test) --------------------
// We'll implement the focal method as static to avoid name mangling in C++
#define local static

local void fill_window(deflate_state *s) {
    unsigned n;
    unsigned more;    /* Amount of free space at the end of the window. */
    // In tests we provide w_size; compute wsize as in original code
    unsigned int wsize = s->w_size;
    Assert(s->lookahead < MIN_LOOKAHEAD, "already enough lookahead");
    do {
        more = (unsigned)(s->window_size -(ulg)s->lookahead -(ulg)s->strstart);
        /* Deal with !@#$% 64K limit: */
        if (sizeof(int) <= 2) {
            if (more == 0 && s->strstart == 0 && s->lookahead == 0) {
                more = wsize;
            } else if (more == (unsigned)(-1)) {
                /* Very unlikely, but possible on 16 bit machine if
                 * strstart == 0 && lookahead == 1 (input done a byte at time)
                 */
                more--;
            }
        }
        /* If the window is almost full and there is insufficient lookahead,
         * move the upper half to the lower one to make room in the upper half.
         */
        if (s->strstart >= wsize + MAX_DIST(s)) {
            zmemcpy(s->window, s->window + wsize, (unsigned)wsize - more);
            s->match_start -= wsize;
            s->strstart    -= wsize; /* we now have strstart >= MAX_DIST */
            s->block_start -= (long) wsize;
            if (s->insert > s->strstart)
                s->insert = s->strstart;
            slide_hash(s);
            more += wsize;
        }
        if (s->strm->avail_in == 0) break;
        /* If there was no sliding:
         *    strstart <= WSIZE+MAX_DIST-1 && lookahead <= MIN_LOOKAHEAD - 1 &&
         *    more == window_size - lookahead - strstart
         * => more >= window_size - (MIN_LOOKAHEAD-1 + WSIZE + MAX_DIST-1)
         * => more >= window_size - 2*WSIZE + 2
         * In the BIG_MEM or MMAP case (not yet supported),
         *   window_size == input_size + MIN_LOOKAHEAD  &&
         *   strstart + s->lookahead <= input_size => more >= MIN_LOOKAHEAD.
         * Otherwise, window_size == 2*WSIZE so more >= 2.
         * If there was sliding, more >= WSIZE. So in all cases, more >= 2.
         */
        Assert(more >= 2, "more < 2");
        n = read_buf(s->strm, s->window + s->strstart + s->lookahead, more);
        s->lookahead += n;
        /* Initialize the hash value now that we have some input: */
        if (s->lookahead + s->insert >= MIN_MATCH) {
            uInt str = s->strstart - s->insert;
            s->ins_h = s->window[str];
            UPDATE_HASH(s, s->ins_h, s->window[str + 1]);
#if MIN_MATCH != 3
            /* In test setup MIN_MATCH == 3 so this path is excluded. */
            // Call UPDATE_HASH() MIN_MATCH-3 times (omitted in tests)
#endif
            while (s->insert) {
                UPDATE_HASH(s, s->ins_h, s->window[str + MIN_MATCH-1]);
#ifndef FASTEST
                s->prev[str & s->w_mask] = s->head[s->ins_h];
#endif
                s->head[s->ins_h] = (Pos)str;
                str++;
                s->insert--;
                if (s->lookahead + s->insert < MIN_MATCH)
                    break;
            }
        }
        /* If the whole input has less than MIN_MATCH bytes, ins_h is garbage,
         * but this is not important since only literal bytes will be emitted.
         */
    } while (s->lookahead < MIN_LOOKAHEAD && s->strm->avail_in != 0);
    /* If the WIN_INIT bytes after the end of the current data have never been
     * written, then zero those bytes in order to avoid memory check reports of
     * the use of uninitialized (or uninitialised as Julian writes) bytes by
     * the longest match routines.  Update the high water mark for the next
     * time through here.  WIN_INIT is set to MAX_MATCH since the longest match
     * routines allow scanning to strstart + MAX_MATCH, ignoring lookahead.
     */
    if (s->high_water < s->window_size) {
        ulg curr = s->strstart + (ulg)(s->lookahead);
        ulg init;
        if (s->high_water < curr) {
            /* Previous high water mark below current data -- zero WIN_INIT
             * bytes or up to end of window, whichever is less.
             */
            init = s->window_size - curr;
            if (init > WIN_INIT)
                init = WIN_INIT;
            zmemzero(s->window + curr, (unsigned)init);
            s->high_water = curr + init;
        }
        else if (s->high_water < (ulg)curr + WIN_INIT) {
            /* High water mark at or above current data, but below current data
             * plus WIN_INIT -- zero out to current data plus WIN_INIT, or up
             * to end of window, whichever is less.
             */
            init = (ulg)curr + WIN_INIT - s->high_water;
            if (init > s->window_size - s->high_water)
                init = s->window_size - s->high_water;
            zmemzero(s->window + s->high_water, (unsigned)init);
            s->high_water += init;
        }
    }
    Assert((ulg)s->strstart <= s->window_size - MIN_LOOKAHEAD,
           "not enough room for search");
}

// -------------------- Test Harness and Test Cases --------------------

// Helper to record test failures
static int test_failures = 0;

// Simple per-test assertion wrapper
static void test_assert(bool cond, const char* msg) {
    if (!cond) {
        printf("Test assertion failed: %s\n", msg);
        test_failures++;
    }
}

// Test 1: Basic path with no available input; ensure no crash and invariant holds
static void test_basic_run_no_input() {
    // Setup environment
    deflate_state s;
    s.w_size = 16;
    s.window_size = 48;
    s.window = new Bytef[s.window_size];
    // Initialize window content
    for (unsigned i = 0; i < s.window_size; ++i) s.window[i] = (Bytef)(i & 0xFF);

    s.strstart = 0;
    s.lookahead = 0;
    s.insert = 0;
    s.ins_h = 0;
    s.w_mask = (unsigned)(s.w_size - 1);

    // Hash tables
    s.head = new unsigned int[HASH_SIZE];
    s.prev = new unsigned int[s.w_mask + 1];
    std::memset(s.head, 0xFF, HASH_SIZE * sizeof(unsigned int)); // sentinel like empty
    std::memset(s.prev, 0, (s.w_mask + 1) * sizeof(unsigned int));

    s.match_start = 0;
    s.block_start = 0;
    s.strm = new z_stream;
    s.strm->avail_in = 0;
    s.high_water = 0;

    // Call fill_window
    fill_window(&s);

    // Assertions: ensure not exceeding window space
    test_assert((unsigned long)s.strstart <= (unsigned long)s.window_size - MIN_LOOKAHEAD,
                "not enough room for search in basic run");

    // Cleanup
    delete[] s.window;
    delete[] s.head;
    delete[] s.prev;
    delete s.strm;
}

// Test 2: High-water memory zeroing path
static void test_high_water_zeroing() {
    deflate_state s;
    s.w_size = 16;
    s.window_size = 48;
    s.window = new Bytef[s.window_size];
    // Fill with a known pattern
    for (unsigned i = 0; i < s.window_size; ++i) s.window[i] = (Bytef)(i);

    s.strstart = 5;
    s.lookahead = 0;
    s.insert = 0;
    s.ins_h = 0;
    s.w_mask = (unsigned)(s.w_size - 1);

    s.head = new unsigned int[HASH_SIZE];
    s.prev = new unsigned int[s.w_mask + 1];
    std::memset(s.head, 0xFF, HASH_SIZE * sizeof(unsigned int));
    std::memset(s.prev, 0, (s.w_mask + 1) * sizeof(unsigned int));

    s.match_start = 0;
    s.block_start = 0;
    s.strm = new z_stream;
    s.strm->avail_in = 0; // no new input
    s.high_water = 0;

    // Run
    fill_window(&s);

    // Expected: high_water should be advanced by init (window_size - curr, capped by WIN_INIT)
    ulg curr = s.strstart + (ulg)(s.lookahead); // 5
    ulg expected_init = s.window_size - curr;
    if (expected_init > WIN_INIT) expected_init = WIN_INIT;
    ulg expected_high = curr + expected_init;

    // We can't easily observe exact high_water if code path differs, but we can check invariant
    // that high_water equals expected_high or is at least not less than curr
    test_assert(s.high_water >= curr, "high_water should be >= curr in high-water path");
    // If init computed, compare near equality with expected_high (allow exact match)
    test_assert(s.high_water == expected_high, "high_water did not match expected value in high-water path");

    // To aid visual validation, ensure the window bytes were zeroed in the range tail
    if (s.window_size > (unsigned)(curr + expected_init)) {
        for (unsigned i = (unsigned)curr; i < (unsigned)(curr + expected_init); ++i) {
            // The code zeroes only until init bytes
            test_assert(s.window[i] == 0, "high-water zeroing should zero window tail bytes");
        }
    }

    delete[] s.window;
    delete[] s.head;
    delete[] s.prev;
    delete s.strm;
}

// Test 3: Sliding branch (copies upper half to lower half when strstart >= wsize + MAX_DIST(s))
static void test_sliding_branch_copy() {
    deflate_state s;
    s.w_size = 16;
    // window_size must be large enough to host src/dst regions
    s.window_size = 40;
    s.window = new Bytef[s.window_size];
    // Prepare window with deterministic content
    for (unsigned i = 0; i < s.window_size; ++i) s.window[i] = (Bytef)i;

    s.strstart = 32;     // triggers sliding: 32 >= 16 + MAX_DIST(s) where MAX_DIST(s) = w_size
    s.lookahead = 0;
    s.insert = 0;
    s.ins_h = 0;
    s.w_mask = (unsigned)(s.w_size - 1);

    s.head = new unsigned int[HASH_SIZE];
    s.prev = new unsigned int[s.w_mask + 1];
    std::memset(s.head, 0xFF, HASH_SIZE * sizeof(unsigned int));
    std::memset(s.prev, 0, (s.w_mask + 1) * sizeof(unsigned int));

    s.match_start = 0;
    s.block_start = 0;
    s.strm = new z_stream;
    s.strm->avail_in = 4; // ensure we enter the read_buf path
    s.high_water = 0;

    // Pre-fill the portion that will be copied: window[32..39] has known data
    // Our earlier plan: after sliding, window[24..31] should become old [32..39]
    // We'll keep 32..39 values as 32,33,...,39
    // They already match this pattern from initialization.

    // Call fill_window
    fill_window(&s);

    // Validate that a copy occurred: window[24..31] should equal previous window[32..39]
    bool copy_ok = true;
    for (unsigned i = 0; i < 8; ++i) {
        unsigned idx_dst = 24 + i;
        unsigned idx_src = 32 + i;
        if (idx_dst >= s.window_size || idx_src >= s.window_size) { copy_ok = false; break; }
        if (s.window[idx_dst] != (Bytef)idx_src) { copy_ok = false; break; }
    }
    test_assert(copy_ok, "Sliding branch copy did not transfer expected bytes from upper to lower half");

    // Strstart should have decreased by w_size
    test_assert(s.strstart == 16, "strstart should be decreased by w_size after sliding");

    delete[] s.window;
    delete[] s.head;
    delete[] s.prev;
    delete s.strm;
}

// ------------------------- Main Test Runner -------------------------

int main() {
    printf("Starting fill_window unit tests (no GTest) [C++11]...\n");

    test_failures = 0;

    test_basic_run_no_input();
    test_high_water_zeroing();
    test_sliding_branch_copy();

    if (test_failures == 0) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Tests FAILED: %d\n", test_failures);
        return 1;
    }
}
// A self-contained C++11 test harness for the focal method: longest_match in deflate.c
// This test suite uses a minimal, custom test framework (no Google Test).
// It relies on the project code exposing the focal function via a non-static linkage.
// To ensure accessibility, we redefine the local macro to nothing and include the deflate.c
// source directly in this translation unit.

#define local // expose non-static linkage for testing
#include <cstring>
#include <deflate.h>
#include <iostream>
#include <deflate.c>
#include <vector>
#include <iomanip>



// Lightweight test harness
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failed; \
        } else { \
            ++g_passed; \
        } \
    } while (0)

#define EXPECT_FALSE(cond) \
    do { \
        if (cond) { \
            std::cerr << "EXPECT_FALSE failed at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failed; \
        } else { \
            ++g_passed; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                      << "  " #a " != " #b " -> " << (a) << " != " << (b) << "\n"; \
            ++g_failed; \
        } else { \
            ++g_passed; \
        } \
    } while (0)

#define EXPECT_LE(a, b) \
    do { \
        if (!((a) <= (b))) { \
            std::cerr << "EXPECT_LE failed at " << __FILE__ << ":" << __LINE__ \
                      << "  " #a " > " #b " -> " << (a) << " > " << (b) << "\n"; \
            ++g_failed; \
        } else { \
            ++g_passed; \
        } \
    } while (0)


// Helper: initialize a minimal deflate_state with safe defaults
static void init_state_for_test(deflate_state &s,
                                std::size_t window_size = 1024,
                                int prefix_strstart = 4)
{
    // Basic zero-init
    std::memset(&s, 0, sizeof(deflate_state));

    // Allocate a window large enough for testing
    s.window_size = static_cast<uInt>(window_size);
    s.window = new Bytef[s.window_size +  MAX_MATCH]; // a bit extra for safety

    // Build a simple, deterministic window content; test cases will override specific bytes as needed
    for (std::size_t i = 0; i < s.window_size + MAX_MATCH; ++i)
        s.window[i] = static_cast<Bytef>(i & 0xFF);

    // Basic state required by longest_match
    s.strstart = static_cast<IPos>(prefix_strstart);
    s.max_chain_length = 1;  // to simplify single-iteration behavior in tests
    s.prev_length = 1;       // so best_len starts at 1
    s.nice_match = 6;          // default threshold
    s.lookahead = 5;           // default small lookahead
    s.w_mask = 0xFFFF;
    s.window_size = static_cast<uInt>(s.window_size);
    s.hash_bits = 8;
    s.good_match = 1000;

    // Prev array: point to no future matches for a deterministic single-step test
    s.prev = new Posf[s.window_size];
    for (std::size_t i = 0; i < s.window_size; ++i)
        s.prev[i] = NIL; // typically 0 in zlib; ensure "no further matches" for initial test

    // For safety, ensure MAX_DIST(s) etc do not trigger assertion in tests
    // (we avoid stressing long chains by using small max_chain_length above)
}

// Cleanup helper
static void cleanup_state(deflate_state &s)
{
    if (s.window) delete[] s.window;
    if (s.prev) delete[] s.prev;
    s.window = nullptr;
    s.prev = nullptr;
}


// Test 1: Basic behavior with no meaningful matches in the window
// Expectation: best_len remains at or near s.prev_length, which is initialized to 1,
// and since lookahead is 5, function should return 1 (best_len <= lookahead).
static void test_longest_match_no_match()
{
    deflate_state s;
    init_state_for_test(s, 1024, 4);

    // Ensure there is no simple match by carefully adjusting window content around
    // the current scan and cur_match (0). We make sure the first few bytes at window[0..4]
    // do not align with the current string at window[4..].
    // Setup: window around index 0 and 4 are distinct.
    s.window[0] = static_cast<Bytef>('X');
    s.window[1] = static_cast<Bytef>('Y');
    s.window[2] = static_cast<Bytef>('Z');
    s.window[3] = static_cast<Bytef>('W');
    s.window[4] = static_cast<Bytef>('A'); // start of the current string
    s.window[5] = static_cast<Bytef>('B');
    s.window[6] = static_cast<Bytef>('C');

    // Narrow down lookahead to 5 to keep within one potential-length bound
    s.lookahead = 5;
    s.prev_length = 1;
    s.nice_match = 10;
    s.strstart = 4;
    // The prev chain at index 0 will terminate immediately
    s.prev[0] = NIL;

    uInt res = longest_match(&s, 0);

    // Expecting no significant match: result should be <= lookahead and equal to best_len (1)
    EXPECT_EQ(res, (uInt)1);

    cleanup_state(s);
}

// Test 2: Simple match of length 2 found by scanning cur_match = 0
// We craft the window so that the first two bytes at position 0 match the current string at position 4.
// We expect the function to discover a match of length 2 and return 2 (assuming lookahead >= 2).
static void test_longest_match_simple_two_byte_match()
{
    deflate_state s;
    init_state_for_test(s, 1024, 4);

    // Craft a two-byte match:
    // window[4]..window[5] is the current string: 'A','B'
    // window[0]..window[1] is the candidate: 'A','B'
    s.window[0] = static_cast<Bytef>('A');
    s.window[1] = static_cast<Bytef>('B');
    // Ensure remaining bytes align similarly to create a consistent 2-byte match
    s.window[2] = static_cast<Bytef>('C');
    s.window[3] = static_cast<Bytef>('D');
    s.window[4] = static_cast<Bytef>('A');
    s.window[5] = static_cast<Bytef>('B');
    // Fill the rest to avoid accidental longer matches in this simplistic test
    for (std::size_t i = 6; i < s.window_size + MAX_MATCH; ++i)
        s.window[i] = static_cast<Bytef>('X');

    s.lookahead = 5;    // allow at least 2 bytes of match
    s.prev_length = 1;  // start at 1
    s.nice_match = 6;   // not too aggressive
    s.strstart = 4;
    s.prev[0] = NIL;

    uInt res = longest_match(&s, 0);

    // Expect to return 2 if a 2-byte match is found and not longer than lookahead
    EXPECT_EQ(res, (uInt)2);

    cleanup_state(s);
}


// Test 3: Ensure function respects the lookahead cap
// We simulate a scenario where a longer actual match would be possible, but lookahead is small.
// The function should return lookahead in this case.
static void test_longest_match_respects_lookahead_cap()
{
    deflate_state s;
    init_state_for_test(s, 1024, 4);

    // Prepare a scenario where a longer match might exist, but limit with small lookahead
    // We'll craft a 3-byte match possibility but cap lookahead to 2.
    s.window[0] = static_cast<Bytef>('A');
    s.window[1] = static_cast<Bytef>('B');
    s.window[2] = static_cast<Bytef>('C');
    s.window[3] = static_cast<Bytef>('D');
    s.window[4] = static_cast<Bytef>('A');
    s.window[5] = static_cast<Bytef>('B');
    // The 2-byte match condition should still be detected; provide a small lookahead:
    s.lookahead = 2;
    s.prev_length = 1;
    s.nice_match = 10;
    s.strstart = 4;
    s.prev[0] = NIL;

    uInt res = longest_match(&s, 0);

    // When best_len <= lookahead, function returns best_len; otherwise returns lookahead.
    // Given lookahead = 2, we expect result to be at most 2.
    EXPECT_LE(res, (uInt)2);

    cleanup_state(s);
}


// Entrypoint
int main()
{
    std::cout << "Running longest_match unit tests (non-GTest):\n";

    test_longest_match_no_match();
    test_longest_match_simple_two_byte_match();
    test_longest_match_respects_lookahead_cap();

    std::cout << "Tests completed. Passed: " << g_passed << ", Failed: " << g_failed << "\n";

    // Clean up any final state if needed (the tests do cleanup per-case)
    return (g_failed == 0) ? 0 : 1;
}
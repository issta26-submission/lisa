// Test suite for the focal method _tr_tally in trees.c
// This test harness is designed for C++11 environments without GTest.
// It relies on the zlib deflate state structures and the _tr_tally symbol
// being available via the included deflate.h header.
//
// The tests cover both branches of the _tr_tally function:
// - dist == 0 (unmatched char) and dist > 0 (match) paths.
// - Both configurations: with LIT_MEM defined (uses d_buf/l_buf) and without (uses sym_buf).
//
// Notes:
// - The tests aim for true/false branch coverage and data-state verification.
// - Static functions and internal linkage are managed by the test via extern "C".
// - We assume the environment provides deflate.h and the symbols _tr_tally and _tr_init.

#include <iostream>
#include <deflate.h>
#include <cstring>


// Bring in C declarations for the deflate state and _tr_tally
extern "C" {
}

// Declare the functions to ensure linkage from C++ test code
extern "C" int _tr_tally(deflate_state *s, unsigned dist, unsigned lc);
extern "C" void _tr_init(deflate_state *s);

// Global test failure counter
static int g_failures = 0;

// Helper macro for reporting failures
#define REPORT_FAIL(msg) do { \
    std::cerr << "Test failure: " << msg << std::endl; \
    ++g_failures; \
} while (0)

// Test 1 (no LIT_MEM): dist == 0path
// Verifies that for dist == 0, a single symbol is recorded and dyn_ltree frequency is updated.
#ifndef LIT_MEM
void test_tr_tally_dist_zero_no_LIT_MEM() {
    std::cout << "Running test_tr_tally_dist_zero_no_LIT_MEM..." << std::endl;

    deflate_state s;
    std::memset(&s, 0, sizeof(s)); // quick reset of the state (only used fields must be valid)

    s.sym_next = 0;
    s.sym_end  = 1024;

    // Initialize internal structures if available
    _tr_init(&s);

    const unsigned lc = 5; // random valid literal/length code (0..255)
    int ret = _tr_tally(&s, 0 /* dist == 0 */, lc);

    // Expectation: end flag is false (sym_end not reached)
    if (ret != 0) {
        REPORT_FAIL("Expected return value 0 when not reaching end (dist==0).");
    }

    // dyn_ltree[lc].Freq should be incremented by 1
    if (s.dyn_ltree[lc].Freq != 1) {
        REPORT_FAIL("Expected dyn_ltree[lc].Freq to be 1 after dist==0 tally.");
    }

    // Check data buffer writes depending on LIT_MEM (no_LIT_MEM path)
    // In this branch, we expect 3 bytes written to sym_buf: dist(0), dist>>8(0), lc
    unsigned char d0 = s.sym_buf[0];
    unsigned char d1 = s.sym_buf[1];
    unsigned char d2 = s.sym_buf[2];

    if (d0 != static_cast<unsigned char>(0) ||
        d1 != static_cast<unsigned char>(0) ||
        d2 != static_cast<unsigned char>(lc)) {
        REPORT_FAIL("Expected sym_buf to contain [0, 0, lc] after tally(dist==0).");
    }

    // Ensure sym_next advanced by 3 for non-LIT_MEM
    if (s.sym_next != 3) {
        REPORT_FAIL("Expected sym_next to be 3 after tally with dist==0 (non-LIT_MEM).");
    }

    std::cout << "test_tr_tally_dist_zero_no_LIT_MEM passed." << std::endl;
}
#endif

// Test 2 (no LIT_MEM): dist > 0 path
// Verifies that a match updates matches, dynamic trees, and symbol buffers appropriately.
#ifndef LIT_MEM
void test_tr_tally_dist_nonzero_no_LIT_MEM() {
    std::cout << "Running test_tr_tally_dist_nonzero_no_LIT_MEM..." << std::endl;

    deflate_state s;
    std::memset(&s, 0, sizeof(s));

    s.sym_next = 0;
    s.sym_end  = 1024;

    _tr_init(&s);

    const unsigned dist = 3; // positive distance
    const unsigned lc   = 6; // some length/char code

    int ret = _tr_tally(&s, dist, lc);

    // End flag should be false since we started with sym_next=0 and sym_end is large
    if (ret != 0) {
        REPORT_FAIL("Expected return value 0 for non-end condition (dist>0).");
    }

    // A match should have increased s.matches by 1
    if (s.matches != 1) {
        REPORT_FAIL("Expected s.matches == 1 after tally(dist>0).");
    }

    // For non-LIT_MEM path: dist was not decremented in a visible buffer, but used to update buffers and trees
    unsigned d_after = dist - 1; // dist is decremented inside _tr_tally
    int idx_ltree = _length_code[lc] + LITERALS + 1;
    if (s.dyn_ltree[idx_ltree].Freq != 1) {
        REPORT_FAIL("Expected dyn_ltree[index] to be incremented for the match length code.");
    }

    int idx_dtree = d_code(d_after);
    if (s.dyn_dtree[idx_dtree].Freq != 1) {
        REPORT_FAIL("Expected dyn_dtree[d_code(dist-1)].Freq to be incremented for the match distance.");
    }

    // Data written to sym_buf: [dist, dist>>8, lc]
    unsigned char b0 = s.sym_buf[0];
    unsigned char b1 = s.sym_buf[1];
    unsigned char b2 = s.sym_buf[2];

    if (b0 != static_cast<unsigned char>(dist) ||
        b1 != static_cast<unsigned char>(dist >> 8) ||
        b2 != static_cast<unsigned char>(lc)) {
        REPORT_FAIL("Expected sym_buf to contain [dist, dist>>8, lc] after tally(dist>0).");
    }

    // sym_next should have advanced by 3
    if (s.sym_next != 3) {
        REPORT_FAIL("Expected sym_next to be 3 after tally(dist>0) (non-LIT_MEM).");
    }

    std::cout << "test_tr_tally_dist_nonzero_no_LIT_MEM passed." << std::endl;
}
#endif

// Test 3 (LIT_MEM): dist == 0 path
// Verifies that for dist == 0, with LIT_MEM defined, d_buf and l_buf receive the values and dyn_ltree is updated.
#ifdef LIT_MEM
void test_tr_tally_dist_zero_LIT_MEM() {
    std::cout << "Running test_tr_tally_dist_zero_LIT_MEM..." << std::endl;

    deflate_state s;
    std::memset(&s, 0, sizeof(s));

    s.sym_next = 0;
    s.sym_end  = 1024;

    _tr_init(&s);

    const unsigned lc = 5;
    int ret = _tr_tally(&s, 0, lc);

    // End flag check
    if (ret != 0) {
        REPORT_FAIL("Expected return value 0 when not reaching end (dist==0, LIT_MEM).");
    }

    // dyn_ltree updated
    if (s.dyn_ltree[lc].Freq != 1) {
        REPORT_FAIL("Expected dyn_ltree[lc].Freq to be 1 (LIT_MEM, dist==0).");
    }

    // d_buf and l_buf should hold dist and lc
    if (s.d_buf[0] != static_cast<unsigned short>(0)) {
        REPORT_FAIL("Expected d_buf[0] to store dist==0 (LIT_MEM path).");
    }
    if (s.l_buf[0] != static_cast<unsigned char>(lc)) {
        REPORT_FAIL("Expected l_buf[0] to store lc (LIT_MEM path).");
    }

    // sym_next increment should be 1 in LIT_MEM path
    if (s.sym_next != 1) {
        REPORT_FAIL("Expected sym_next == 1 after tally with dist==0 (LIT_MEM).");
    }

    std::cout << "test_tr_tally_dist_zero_LIT_MEM passed." << std::endl;
}
#endif

// Test 4 (LIT_MEM): dist > 0 path
// Verifies that a match updates state in LIT_MEM mode: matches, dyn_ltree, dyn_dtree and d_buf/l_buf.
#ifdef LIT_MEM
void test_tr_tally_dist_nonzero_LIT_MEM() {
    std::cout << "Running test_tr_tally_dist_nonzero_LIT_MEM..." << std::endl;

    deflate_state s;
    std::memset(&s, 0, sizeof(s));

    s.sym_next = 0;
    s.sym_end  = 1024;

    _tr_init(&s);

    const unsigned dist = 3;
    const unsigned lc = 6;

    int ret = _tr_tally(&s, dist, lc);

    // End flag
    if (ret != 0) {
        REPORT_FAIL("Expected return value 0 for non-end condition (dist>0) in LIT_MEM.");
    }

    // matches increment
    if (s.matches != 1) {
        REPORT_FAIL("Expected s.matches == 1 after tally(dist>0) in LIT_MEM.");
    }

    // dyn_ltree update for length code + literals offset
    int idx_ltree = _length_code[lc] + LITERALS + 1;
    if (s.dyn_ltree[idx_ltree].Freq != 1) {
        REPORT_FAIL("Expected dyn_ltree[index] to be incremented for the match length (LIT_MEM).");
    }

    // dyn_dtree for distance code
    unsigned d_after = dist - 1;
    int idx_dtree = d_code(d_after);
    if (s.dyn_dtree[idx_dtree].Freq != 1) {
        REPORT_FAIL("Expected dyn_dtree[d_code(dist-1)].Freq to be incremented (LIT_MEM).");
    }

    // d_buf and l_buf
    if (s.d_buf[0] != static_cast<unsigned short>(dist)) {
        REPORT_FAIL("Expected d_buf[0] == dist (LIT_MEM).");
    }
    if (s.l_buf[0] != static_cast<unsigned char>(lc)) {
        REPORT_FAIL("Expected l_buf[0] == lc (LIT_MEM).");
    }

    // sym_next increment
    if (s.sym_next != 1) {
        REPORT_FAIL("Expected sym_next == 1 after tally(dist>0) (LIT_MEM).");
    }

    std::cout << "test_tr_tally_dist_nonzero_LIT_MEM passed." << std::endl;
}
#endif

int main() {
    // Run tests depending on compile-time LIT_MEM switch
#ifndef LIT_MEM
    test_tr_tally_dist_zero_no_LIT_MEM();
    test_tr_tally_dist_nonzero_no_LIT_MEM();
#else
    test_tr_tally_dist_zero_LIT_MEM();
    test_tr_tally_dist_nonzero_LIT_MEM();
#endif

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return g_failures;
    }
}
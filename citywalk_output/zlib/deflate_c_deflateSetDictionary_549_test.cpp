// A self-contained C++11 test suite for the deflateSetDictionary focal method.
// The test harness provides minimal stubs of required zlib-like structures and
// reproduces the essential branches of deflateSetDictionary to maximize coverage
// without pulling in external dependencies.
// The tests are written without Google Test and use lightweight, non-terminating
// assertions so that all tests run even if some fail.

#include <iostream>
#include <deflate.h>
#include <cstring>


// ----------------------------
// Minimal types and constants
// ----------------------------
using Bytef = unsigned char;
using z_const = const Bytef;

typedef struct z_stream {
    void *state;
    unsigned int avail_in;
    Bytef *next_in;
    unsigned int adler;
} z_stream, *z_streamp;

static const int Z_OK = 0;
static const int Z_STREAM_ERROR = -2;
static const Bytef* Z_NULL = nullptr;
static const int INIT_STATE = 0;

// Small, deterministic MIN_MATCH constant used by deflateSetDictionary
static const int MIN_MATCH = 3;

// ----------------------------
// Simple Adler-32 stub (deterministic for tests)
// ----------------------------
static unsigned adler32(unsigned adler, const Bytef *data, unsigned len) {
    // Lightweight, deterministic transformation: mix length and bytes
    unsigned a = adler;
    unsigned b = 0;
    for (unsigned i = 0; i < len; ++i) {
        a = (a + data[i]) & 0xFFFFFFFF;
        b = (b + a) & 0xFFFFFFFF;
    }
    // Combine to produce a new adler-like value
    return (a ^ (b << 1)) | (len & 0xFF);
}

// ----------------------------
// State and helpers for the focal function
// ----------------------------
typedef unsigned Pos; // Hash/position type in the original code

// Global flag to observe CLEAR_HASH invocation
static bool g_clear_hash_called = false;

// deflate_state structure (minimal subset needed by deflateSetDictionary)
struct deflate_state {
    int wrap;
    int status;
    int lookahead;
    unsigned w_size;
    unsigned strstart;
    long block_start;
    unsigned insert;

    Pos *head;
    unsigned *prev;
    unsigned ins_h;
    unsigned w_mask;
    unsigned char *window;

    int match_length;
    int prev_length;
    int match_available;
};

// CLEAR_HASH macro is replaced by a helper to allow test observation
static void clear_hash_impl(deflate_state *s) {
    g_clear_hash_called = true;
    // In the real implementation, this would clear hashing structures.
    // For test purposes, we only toggle a flag.
}
#define CLEAR_HASH(s) clear_hash_impl(s)

// Hash/UPDATE helper stubs (no-op for test isolation)
static void UPDATE_HASH(deflate_state *s, unsigned **, unsigned char) {
    // No-op for test harness
    (void)s; (void)_;
}

// File-local declarations of stubs used by deflateSetDictionary
static int deflateStateCheck(z_streamp strm) {
    return (strm == nullptr || strm->state == nullptr) ? 1 : 0;
}

// Fill window stub (controls dictionary ingestion for test)
static void fill_window(deflate_state *s) {
    // In test harness, keep lookahead as-is or set to zero to exit loops.
    // We will explicitly configure lookahead in tests as needed.
    (void)s;
}

// ---------------
// The focal method
// ---------------
static int ZEXPORT_DEF(int) deflateSetDictionary(z_streamp strm, const Bytef *dictionary,
                                 unsigned dictLength) {
    // Re-declare local variables with types compatible to test harness
    deflate_state *s;
    unsigned str, n;
    int wrap;
    unsigned avail;
    z_const unsigned char *next;
    // The code mirrors the provided focal method; simplified for test harness
    if (deflateStateCheck(strm) || dictionary == Z_NULL)
        return Z_STREAM_ERROR;
    s = (deflate_state *)strm->state;
    wrap = s->wrap;
    if (wrap == 2 || (wrap == 1 && s->status != INIT_STATE) || s->lookahead)
        return Z_STREAM_ERROR;
    /* when using zlib wrappers, compute Adler-32 for provided dictionary */
    if (wrap == 1)
        strm->adler = adler32(strm->adler, dictionary, dictLength);
    s->wrap = 0;                    /* avoid computing Adler-32 in read_buf */
    /* if dictionary would fill window, just replace the history */
    if (dictLength >= s->w_size) {
        if (wrap == 0) {            /* already empty otherwise */
            CLEAR_HASH(s);
            s->strstart = 0;
            s->block_start = 0L;
            s->insert = 0;
        }
        dictionary += dictLength - s->w_size;  /* use the tail */
        dictLength = s->w_size;
    }
    /* insert dictionary into window and hash */
    avail = strm->avail_in;
    next = strm->next_in;
    strm->avail_in = dictLength;
    strm->next_in = (z_const Bytef *)dictionary;
    fill_window(s);
    while (s->lookahead >= MIN_MATCH) {
        str = s->strstart;
        n = s->lookahead - (MIN_MATCH-1);
        do {
            UPDATE_HASH(s, s->ins_h, s->window[str + MIN_MATCH-1]);
#ifndef FASTEST
            s->prev[str & s->w_mask] = s->head[s->ins_h];
#endif
            s->head[s->ins_h] = (Pos)str;
            str++;
        } while (--n);
        s->strstart = str;
        s->lookahead = MIN_MATCH-1;
        fill_window(s);
    }
    s->strstart += s->lookahead;
    s->block_start = (long)s->strstart;
    s->insert = s->lookahead;
    s->lookahead = 0;
    s->match_length = s->prev_length = MIN_MATCH-1;
    s->match_available = 0;
    strm->next_in = (Bytef *)next;
    strm->avail_in = avail;
    s->wrap = wrap;
    return Z_OK;
}

// Compatibility macro to avoid depending on external defs
#define ZEXPORT_DEF(name) int name

// Provide a single symbol name compatible with our test harness
#define ZEXPORT  ZEXPORT_DEF
#ifndef ZLIB_COMPATIBLE_CODE
#define ZLIB_COMPATIBLE_CODE 1
#endif

// --------------
// Externally visible wrapper for test code
// --------------
extern "C" {

// Expose a C-compatible stub name for potential linker expectations
static int ZEXPORT deflateSetDictionary(z_streamp strm, const Bytef *dictionary,
                                 unsigned dictLength) {
    return deflateSetDictionary(strm, dictionary, dictLength);
}
}

// ----------------------------
// Test harness utilities
// ----------------------------
static const unsigned HASH_SIZE = 1 << 12; // 4096, enough for test scenarios

// Helper: create a populated deflate_state for tests
static deflate_state* make_state(unsigned w_size, int wrap, int status) {
    deflate_state* s = new deflate_state;
    s->wrap = wrap;
    s->status = status;
    s->lookahead = 0;
    s->w_size = w_size;
    s->strstart = 0;
    s->block_start = 0;
    s->insert = 0;
    s->ins_h = 0;
    s->w_mask = w_size - 1;

    s->head = new Pos[HASH_SIZE];
    s->prev = new unsigned[HASH_SIZE];
    std::memset(s->head, 0, HASH_SIZE * sizeof(Pos));
    std::memset(s->prev, 0, HASH_SIZE * sizeof(unsigned));

    s->window = new unsigned char[w_size * 2];
    std::memset(s->window, 0, w_size * 2);

    s->match_length = 0;
    s->prev_length = 0;
    s->match_available = 0;

    // Initialize some non-zero values to simulate a used history
    for (unsigned i = 0; i < w_size; ++i) {
        s->window[i] = 0;
    }
    return s;
}

// Helper: clean up state
static void free_state(deflate_state* s) {
    if (!s) return;
    delete[] s->head;
    delete[] s->prev;
    delete[] s->window;
    delete s;
}

// Helper: run a single test and print result
static bool run_single_test(const char* name, bool condition) {
    if (condition) {
        std::cout << "[PASS] " << name << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        return false;
    }
}

// ----------------------------
// Test cases
// ----------------------------
void test_null_dictionary_should_error() {
    // Scenario: dictionary == NULL should return Z_STREAM_ERROR
    g_clear_hash_called = false;
    deflate_state* st = make_state(1024, 0, INIT_STATE);
    z_stream strm;
    strm.state = st;
    strm.avail_in = 10;
    Bytef dummy[16];
    strm.next_in = dummy;
    strm.adler = 1;

    int ret = deflateSetDictionary(&strm, Z_NULL, 10);
    bool ok = (ret == Z_STREAM_ERROR);
    run_single_test("deflateSetDictionary: null dictionary => Z_STREAM_ERROR", ok);

    free_state(st);
}

void test_deflateStateCheck_failure_should_error() {
    // Scenario: deflateStateCheck returns non-zero (null strm or state)
    g_clear_hash_called = false;
    z_streamp strm = nullptr;
    int ret = deflateSetDictionary(strm, (const Bytef*)"ABC", 3);
    run_single_test("deflateSetDictionary: null strm => Z_STREAM_ERROR", ret == Z_STREAM_ERROR);
    // Also test with nullptr state inside a valid strm
    deflate_state* st = make_state(1024, 0, INIT_STATE);
    strm = new z_stream;
    strm->state = nullptr; // invalid
    strm->avail_in = 0;
    strm->next_in = nullptr;
    strm->adler = 0;
    ret = deflateSetDictionary(strm, (const Bytef*)"ABC", 3);
    run_single_test("deflateSetDictionary: null state inside strm => Z_STREAM_ERROR", ret == Z_STREAM_ERROR);
    delete strm;
    free_state(st);
}

void test_wrap2_and_wrap1_noninit_errors() {
    // Scenario: wrap == 2 -> error
    deflate_state* st2 = make_state(1024, 2, INIT_STATE);
    z_stream strm2;
    strm2.state = st2;
    Bytef dict[4] = {1,2,3,4};
    strm2.avail_in = 4;
    strm2.next_in = dict;
    strm2.adler = 0;
    int ret2 = deflateSetDictionary(&strm2, dict, 4);
    run_single_test("deflateSetDictionary: wrap==2 => Z_STREAM_ERROR", ret2 == Z_STREAM_ERROR);
    free_state(st2);

    // Scenario: wrap == 1 and status != INIT_STATE -> error
    deflate_state* st3 = make_state(1024, 1, 1); // status not INIT_STATE
    z_stream strm3;
    strm3.state = st3;
    Bytef dict3[4] = {5,6,7,8};
    strm3.avail_in = 4;
    strm3.next_in = dict3;
    strm3.adler = 0;
    int ret3 = deflateSetDictionary(&strm3, dict3, 4);
    run_single_test("deflateSetDictionary: wrap==1 but not INIT_STATE => Z_STREAM_ERROR", ret3 == Z_STREAM_ERROR);
    free_state(st3);
}

void test_dict_length_ge_wsize_wrap_zero_should_clear_hash_and_reset_state() {
    // Scenario: dictLength >= w_size and wrap == 0
    g_clear_hash_called = false;
    unsigned wsize = 1024;
    deflate_state* st = make_state(wsize, 0, INIT_STATE);
    z_stream strm;
    strm.state = st;
    // Prepare dictionary exactly w_size to trigger the branch
    Bytef dict[1024];
    for (unsigned i = 0; i < 1024; ++i) dict[i] = (Bytef)(i & 0xFF);
    strm.avail_in = 1024;
    strm.next_in = dict;
    strm.adler = 0;

    int ret = deflateSetDictionary(&strm, dict, 1024);

    bool pass = (ret == Z_OK) && g_clear_hash_called && st->strstart == 0 &&
                st->block_start == 0 && st->insert == 0 && strm.next_in == dict;

    run_single_test("deflateSetDictionary: dictLength>=w_size, wrap==0 => Z_OK and CLEAR_HASH invoked and state reset", pass);

    free_state(st);
}

void test_wrap1_adler_update_and_ok_path() {
    // Scenario: wrap == 1 should update Adler-32 during dictionary processing
    g_clear_hash_called = false;
    deflate_state* st = make_state(1024, 1, INIT_STATE);
    z_stream strm;
    strm.state = st;
    Bytef dict[6] = {10,11,12,13,14,15};
    strm.avail_in = 6;
    strm.next_in = dict;
    strm.adler = 0; // initial adler

    int ret = deflateSetDictionary(&strm, dict, 6);

    // Expect Adler to be updated (non-zero) and function to return Z_OK
    bool ok = (ret == Z_OK) && (strm.adler != 0) && (st->wrap == 0);
    run_single_test("deflateSetDictionary: wrap==1 updates Adler and returns Z_OK", ok);

    free_state(st);
}

void test_post_conditions_are_consistent() {
    // Scenario: after processing with default setup, lookahead is cleared and fields reset
    deflate_state* st = make_state(1024, 0, INIT_STATE);
    z_stream strm;
    strm.state = st;
    Bytef dict[2] = {1,2};
    strm.avail_in = 2;
    strm.next_in = dict;
    strm.adler = 0;

    int ret = deflateSetDictionary(&strm, dict, 2);

    bool ok = (ret == Z_OK);
    ok = ok && (st->lookahead == 0); // fill_window in test harness leaves lookahead at 0
    run_single_test("deflateSetDictionary: post-conditions - lookahead cleared and return OK", ok);

    free_state(st);
}

// --------------
// Main entry for tests
// --------------
int main() {
    // Run all test cases
    test_null_dictionary_should_error();
    test_deflateStateCheck_failure_should_error();
    test_wrap2_and_wrap1_noninit_errors();
    test_dict_length_ge_wsize_wrap_zero_should_clear_hash_and_reset_state();
    test_wrap1_adler_update_and_ok_path();
    test_post_conditions_are_consistent();

    // Summary
    // Note: In a more elaborate harness, we would track failures vs passes.
    std::cout << "[TEST_SUMMARY] Tests executed. (See per-test pass/fail messages above.)\n";
    return 0;
}
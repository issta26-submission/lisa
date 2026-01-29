// Test suite for the focal method _tr_flush_block in trees.c
// This test harness uses the actual focal implementation as provided by the
// project (deflate_state and related types come from the project's headers).
// To enable unit-testing without GTest, we implement lightweight C-style stubs
// for dependent functions and collect lightweight "soft" assertions to keep
// tests non-terminating while still reporting failures at the end.

#include <iostream>
#include <deflate.h>
#include <cstring>


// Include project headers to obtain the real type definitions.
// This header is expected to declare deflate_state, tree_desc, ct_data, charf, ulg, etc.
extern "C" {
}

// Ensure the C API is visible with C linkage for the real _tr_flush_block symbol
extern "C" void _tr_flush_block(deflate_state *s, charf *buf, ulg stored_len, int last);
extern "C" void _tr_stored_block(deflate_state *s, charf *buf, ulg stored_len, int last);
extern "C" void build_tree(deflate_state *s, tree_desc *desc);
extern "C" int build_bl_tree(deflate_state *s);
extern "C" void send_bits(deflate_state *s, unsigned value, int length);
extern "C" void compress_block(deflate_state *s, const ct_data *ltree, const ct_data *dtree);
extern "C" int detect_data_type(deflate_state *s);
extern "C" void init_block(deflate_state *s);
extern "C" void bi_windup(deflate_state *s);

// Lightweight test scaffolding
static bool stored_block_called = false;
static bool last_windup_called = false;
static unsigned test_id = 0;

// Simple soft assertions (non-terminating). Keeps track of failures.
static int test_fail_count = 0;
#define SOFT_VERIFY(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test " << test_id << " FAILED: " << msg << std::endl; \
        ++test_fail_count; \
    } \
} while(0)


// Stub implementations for project dependencies (to drive _tr_flush_block behavior)
// These stubs replace heavy logic with deterministic, test-friendly side effects.

extern "C" void _tr_stored_block(deflate_state *s, charf *buf, ulg stored_len, int last) {
    // Indicate this branch was taken
    stored_block_called = true;
    // No actual encoding; keep bits/buffer state consistent for the assertion
    (void)s; (void)buf; (void)stored_len; (void)last;
}

extern "C" void init_block(deflate_state *s) {
    // Reset bit counters for the next block
    if (s) {
        s->bits_sent = 0;
        s->compressed_len = 0;
    }
}

extern "C" int detect_data_type(deflate_state *s) {
    // Keep simple: unknown by default
    (void)s;
    return Z_UNKNOWN;
}

extern "C" void build_tree(deflate_state *s, tree_desc *desc) {
    // No-op: in tests, we manipulate opt_len/static_len directly in tests.
    (void)s; (void)desc;
}

extern "C" int build_bl_tree(deflate_state *s) {
    // No-op: return 0 as a safe default
    (void)s;
    return 0;
}

extern "C" void send_bits(deflate_state *s, unsigned value, int length) {
    // Update bit counters to reflect written bits, and keep compressed_len in sync
    (void)value;
    s->bits_sent += static_cast<unsigned long>(length);
    s->compressed_len = s->bits_sent;
}

extern "C" void compress_block(deflate_state *s, const ct_data *ltree, const ct_data *dtree) {
    // No-op: indicate which trees would have been used by validating the inputs
    (void)s; (void)ltree; (void)dtree;
}

extern "C" int ZLIB_INTERNAL _tr_tally(deflate_state *s, unsigned dist, unsigned lc) {
    // Not used in tests
    (void)s; (void)dist; (void)lc;
    return 0;
}

// Helper: reset test flags
static void reset_flags() {
    stored_block_called = false;
    last_windup_called = false;
    test_id = 0;
}

// Stub for bi_windup to capture the finalization event (called when last==1)
extern "C" void bi_windup(deflate_state *s) {
    (void)s;
    last_windup_called = true;
}

// Helper: wrap test ID for reporting
static void set_test_id(const char* name) {
    // Simple mapping for human-friendly output
    std::cout << "Running test: " << name << std::endl;
}

// Test 1: Level 0 path should not call stored block in this configuration.
// We choose opt_len and static_len to be equal so the static path is chosen.
static void test_static_path_level0() {
    test_id = 1;
    set_test_id("static_path_level0");

    reset_flags();

    deflate_state s;
    // Minimal initialization required by _tr_flush_block usage
    std::memset(&s, 0, sizeof(deflate_state));
    z_stream_s strm;
    std::memset(&strm, 0, sizeof(z_stream_s));
    s.strm = &strm;
    strm.data_type = Z_UNKNOWN;

    // Force level 0 path
    s.level = 0;
    // Force static path by making opt_len and static_len equal
    s.opt_len = 0;
    s.static_len = 0;
    // Ensure a non-null buffer to satisfy asserts (for level 0 path)
    char dummy_buf[16] = {0};
    // Call focal method
    _tr_flush_block(&s, reinterpret_cast<charf*>(dummy_buf), 0, 0);

    // Expect that a stored-block path was not taken
    SOFT_VERIFY(!stored_block_called, "level0 static path should not call _tr_stored_block");
    // Since last==0, bi_windup should not be called
    SOFT_VERIFY(!last_windup_called, "level0 static path should not call bi_windup when last==0");
}

// Test 2: Level > 0 with stored block path (trigger _tr_stored_block)
static void test_stored_block_path_level1() {
    test_id = 2;
    set_test_id("stored_block_path_level1");

    reset_flags();

    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));
    z_stream_s strm;
    std::memset(&strm, 0, sizeof(z_stream_s));
    s.strm = &strm;
    strm.data_type = Z_UNKNOWN;

    // Configure for stored block path
    s.level = 1;
    s.strm = &strm;
    s.opt_len = 10000;    // large enough to ensure opt_lenb is big
    s.static_len = 20000; // static_lenb larger but ultimately, static_lenb <= opt_lenb will not hold due to choices
    s.strategy = 0;

    // stored_len chosen to satisfy stored-block condition: stored_len + 4 <= opt_lenb
    ulg stored_len = 4;
    // Non-null buffer to satisfy the buf check
    char buf[8];
    _tr_flush_block(&s, reinterpret_cast<charf*>(buf), stored_len, 0);

    // Expect that stored-block path was taken
    SOFT_VERIFY(stored_block_called, "level1 with stored_len should call _tr_stored_block");
    // last==0, so no windup
    SOFT_VERIFY(!last_windup_called, "stored-block path should not trigger bi_windup when last==0");
}

// Test 3: Level > 0 with dynamic path (no stored block; dynamic trees)
static void test_dynamic_path_level1() {
    test_id = 3;
    set_test_id("dynamic_path_level1");

    reset_flags();

    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));
    z_stream_s strm;
    std::memset(&strm, 0, sizeof(z_stream_s));
    s.strm = &strm;
    strm.data_type = Z_UNKNOWN;

    // Configure for dynamic path
    s.level = 1;
    // Choose values so that stored_len + 4 <= opt_lenb is false
    // and static_lenb != opt_lenb to avoid static path
    s.opt_len = 1000;   // opt_lenb ~ 126
    s.static_len = 5000; // static_lenb ~ 626
    s.strategy = 0;

    // Make stored_len large to avoid stored-block path
    ulg stored_len = 1000;
    char buf[8];
    _tr_flush_block(&s, reinterpret_cast<charf*>(buf), stored_len, 0);

    // Expect that stored-block path was NOT taken
    SOFT_VERIFY(!stored_block_called, "dynamic-path should not call _tr_stored_block");
    // last==0, so no windup
    SOFT_VERIFY(!last_windup_called, "dynamic-path should not trigger bi_windup when last==0");
}

// Test 4: Last block triggers bi_windup (regardless of path)
static void test_last_triggers_bi_windup() {
    test_id = 4;
    set_test_id("last_triggers_bi_windup");

    reset_flags();

    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));
    z_stream_s strm;
    std::memset(&strm, 0, sizeof(z_stream_s));
    s.strm = &strm;
    strm.data_type = Z_UNKNOWN;

    // Use dynamic-path configuration
    s.level = 1;
    s.opt_len = 1000;
    s.static_len = 5000;
    s.strategy = 0;

    ulg stored_len = 1000;
    char buf[8];
    _tr_flush_block(&s, reinterpret_cast<charf*>(buf), stored_len, 1);

    // Expect that bi_windup was called due to last==1
    SOFT_VERIFY(last_windup_called, "last==1 should trigger bi_windup at block end");
}

// Main: run all tests and report summary
int main() {
    std::cout << "Starting _tr_flush_block unit tests (no GTest)..." << std::endl;

    test_static_path_level0();
    test_stored_block_path_level1();
    test_dynamic_path_level1();
    test_last_triggers_bi_windup();

    if (test_fail_count == 0) {
        std::cout << "All tests passed (soft assertions satisfied)." << std::endl;
        return 0;
    } else {
        std::cerr << "Total failures: " << test_fail_count << std::endl;
        return 1;
    }
}
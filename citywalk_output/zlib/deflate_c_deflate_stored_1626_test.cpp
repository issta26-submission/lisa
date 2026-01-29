// Unit test suite for the focal method: deflate_stored (in deflate.c)
// Targeted to compile under C++11 without GTest. A lightweight in-file test harness
// is provided that relies on the project-provided definitions (deflate.h, deflate.c).
// Each test is documented with intent and the assertions performed.

// Note: The tests assume the project exposes the necessary structures and
// interfaces (deflate_state, z_streamp, block_state, etc.) via deflate.h.
// The tests are designed to be non-terminating (they report failures but continue
// execution to maximize code coverage).

#include <iostream>
#include <cassert>
#include <deflate.h>
#include <cstring>


// Include the project headers that declare the focal method and its dependencies.
// We rely on the real project types/definitions here to ensure compatibility.

// Lightweight assertion helper to enable non-terminating checks.
// If an assertion fails, log the failure but keep running to improve coverage.
static bool any_failure = false;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        any_failure = true; \
    } \
} while(false)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        std::cerr << "[FAIL] " #a " == " #b " (value: " << (a) << " vs " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        any_failure = true; \
    } \
} while(false)

// Utility to print a short banner for each test
static void banner(const char* name) {
    std::cout << "\n=== Test: " << name << " ===\n";
}

// Helper to allocate and initialize a minimal deflate_state suitable for testing.
// The real project defines deflate_state in deflate.h; we configure a light-weight
// instance with values that trigger various branches in deflate_stored.
// This function intentionally does not rely on internal project initialization paths
// to keep tests self-contained and repeatable.
static deflate_state* create_test_state(size_t w_size, size_t pending_buf_size,
                                      unsigned avail_in, unsigned avail_out,
                                      const unsigned char* next_in_buf)
{
    // Allocate a fresh state object (definition provided by the project)
    deflate_state *s = new deflate_state;
    std::memset(s, 0, sizeof(deflate_state));

    // Basic window/pending buffers and related controls
    s->w_size = (unsigned)w_size;
    s->window_size = (unsigned)(w_size * 2);
    s->strstart = 0;
    s->block_start = 0;
    s->pending_buf_size = (unsigned)pending_buf_size;

    // Allocate buffers (project-provided layout assumed)
    s->window = new unsigned char[s->window_size];
    s->pending_buf = new unsigned char[s->pending_buf_size];
    // Fill window with deterministic data for testing copy-paths
    if (s->window && w_size > 0) {
        for (unsigned i = 0; i < s->window_size; ++i) {
            s->window[i] = (unsigned char)(i & 0xFF);
        }
    }

    // Prepare pending tracking
    s->pending = 4; // leave room for dummy header, as deflate_stored uses pending-4..pending-1
    std::memset(s->pending_buf, 0, s->pending_buf_size);

    // strm (z_stream) stub
    s->strm = new z_streamp;
    *s->strm = new z_stream;
    (*s->strm)->avail_in = avail_in;
    (*s->strm)->avail_out = avail_out;

    // Link input requirement (for read_buf to fetch data)
    if (avail_in > 0 && next_in_buf != nullptr) {
        unsigned char* in_copy = new unsigned char[avail_in];
        std::memcpy(in_copy, next_in_buf, avail_in);
        (*s->strm)->next_in = in_copy;
    } else {
        (*s->strm)->next_in = nullptr;
    }

    // Output pointer bucket (next_out) will be provided by test when invoking
    // deflate_stored, so we allocate a separate buffer for safety as needed.

    // Basic parameters used by deflate_stored
    s->bi_valid = 0;
    s->matches = 0;
    s->high_water = 0;
    s->insert = 0;
    s->insert = s->strstart; // typical initial condition
    s->strstrm = nullptr;    // (not used in tests)
    return s;
}

// Cleanup helper for test-state
static void destroy_test_state(deflate_state* s)
{
    if (!s) return;
    if (s->window) delete[] s->window;
    if (s->pending_buf) delete[] s->pending_buf;
    if (s->strm) {
        if ((*s->strm)->next_in) delete[] (*s->strm)->next_in;
        delete *s->strm;
        delete s->strm;
    }
    delete s;
}

// Test 1: Ensure deflate_stored gracefully handles when there is insufficient space
// in the output buffer to emit a header. We expect the function to break early from
// the copy loop and return a "need_more" state after the loop.
static bool test_deflate_stored_insufficient_header_space()
{
    banner("deflate_stored: insufficient header space");

    // Initialize minimal state: small pending buffer, small w_size
    unsigned w_size = 32;                      // small window
    unsigned pending_buf_size = 256;           // small header footprint
    // No input
    unsigned avail_in = 0;
    unsigned avail_out = 4;                    // less than header bytes (have ~5)
    unsigned char dummy_in[0] = {};

    // Create state
    deflate_state* s = create_test_state(w_size, pending_buf_size, avail_in, avail_out, dummy_in);

    // Call focal function
    int flush = Z_NO_FLUSH;
    block_state res = deflate_stored(s, flush);

    // We expect a "need_more" (unfinished) state due to header space constraint
    // The exact numeric enum value depends on project; here we compare to expected enum.
    EXPECT_TRUE(res == need_more);

    // Cleanup
    destroy_test_state(s);
    return !any_failure;
}

// Test 2: Validate that when there is data in the window (left > 0) and
// sufficient output space, a stored block can be copied from the window to next_out.
// We verify that the copied bytes are identical to the window contents.
static bool test_deflate_stored_copy_from_window()
{
    banner("deflate_stored: copy from window to next_out");

    // Setup a scenario where left (bytes in window) > 0
    unsigned w_size = 64;
    unsigned pending_buf_size = 1024;
    unsigned avail_in = 0;                       // no new input
    unsigned avail_out = 1024;                   // ample space for copying
    unsigned char input_bytes[4] = { 'A', 'B', 'C', 'D' }; // not used for this path, but kept for realism

    deflate_state* s = create_test_state(w_size, pending_buf_size, avail_in, avail_out, input_bytes);

    // Prepare the structure to ensure there are bytes left in the window to copy.
    // We'll simulate that by setting strstart and block_start so that left > 0
    s->strstart = 40;
    s->block_start = 0;

    // Also ensure the bi_valid header space is enough
    s->bi_valid = 0;
    // Ensure the strm->next_out buffer exists and is large enough
    unsigned char* next_out_buf = new unsigned char[256];
    s->strm->next_out = next_out_buf;

    // Ensure there is window data at block_start
    // window is already filled with deterministic data in create_test_state

    int flush = Z_NO_FLUSH;
    block_state res = deflate_stored(s, flush);

    // We expect some copy to next_out to occur (not necessarily finish yet)
    EXPECT_TRUE(res == need_more || res == block_done || res == finish_started);

    // Verify that the first 40 bytes copied from window to next_out (if copied)
    // For deterministic check, compare the first 16 bytes if available.
    if (s->strm->next_out) {
        size_t copied = 0;
        // Determine how many bytes could be copied: min(left, avail_out)
        unsigned left = s->strstart - s->block_start; // should be 40
        if (left > 0 && next_out_buf) {
            copied = (left <= 256) ? left : 256;
            // First 'copied' bytes of next_out should match window[start .. start+copied)
            bool match = std::memcmp(next_out_buf, s->window + s->block_start, copied) == 0;
            EXPECT_TRUE(match);
        }
    }

    // Cleanup
    if (next_out_buf) delete[] next_out_buf;
    destroy_test_state(s);
    return !any_failure;
}

// Test 3: Ensure finishing path (flush == Z_FINISH) with last block triggers finish_done.
// We simulate a complete block by ensuring left >= min_block and len == left + avail_in.
// Then the function should return finish_done.
static bool test_deflate_stored_finish_path()
{
    banner("deflate_stored: finish path on last block");

    unsigned w_size = 64;
    unsigned pending_buf_size = 1024;
    unsigned avail_in = 0;
    unsigned avail_out = 1024;
    unsigned char inbuf[0] = {};

    deflate_state* s = create_test_state(w_size, pending_buf_size, avail_in, avail_out, inbuf);

    // Force a scenario where a large left exists to satisfy left >= min_block
    s->strstart = 40;
    s->block_start = 0;
    s->bi_valid = 0;
    s->high_water = 0;

    // Provide a sufficiently large window to copy from
    unsigned char* next_out_buf = new unsigned char[256];
    s->strm->next_out = next_out_buf;

    // Set flush to Z_FINISH so last=true in deflate_stored
    int flush = Z_FINISH;
    block_state res = deflate_stored(s, flush);

    // Expect a finish_done return state
    EXPECT_TRUE(res == finish_done);

    // Cleanup
    if (next_out_buf) delete[] next_out_buf;
    destroy_test_state(s);
    return !any_failure;
}

// Main test runner
int main()
{
    // Run individual tests
    test_deflate_stored_insufficient_header_space();
    test_deflate_stored_copy_from_window();
    test_deflate_stored_finish_path();

    if (any_failure) {
        std::cerr << "One or more tests FAILED.\n";
        return 1;
    } else {
        std::cout << "\nAll tests PASSED.\n";
        return 0;
    }
}
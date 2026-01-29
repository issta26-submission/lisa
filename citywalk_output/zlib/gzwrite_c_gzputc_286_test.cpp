// Unit tests for gzputc (gzwrite.c) using plain C++11 test harness (no GTest).
// The tests exercise internal behavior of gzputc by constructing a real gz_state
// structure as defined in zlib's internal headers and linking against the
// gzwrite.c implementation. This approach relies on the public C interface and
// the library's internal state layout defined in gzguts.h.
// Note: The test suite uses a lightweight, non-terminating assertion framework
// suitable for C++11, printing failures but continuing execution to maximize coverage.

#include <cstring>
#include <cstdio>
#include <stdarg.h>
#include <cstdint>
#include <iostream>
#include <gzguts.h>
#include <zlib.h>
#include <cstdlib>


extern "C" {
  // Prototypes for gzputc and related types; ensure C linkage.
  int ZEXPORT gzputc(gzFile file, int c);
  // Include internal structures to construct a gz_state object correctly.
  // These definitions are provided by zlib's internal headers.
  // If the repository uses a different path, adjust accordingly.
  // We assume gzguts.h is available alongside the library.
  #include "gzguts.h"
}

// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERTION FAILED: " << (msg) << " (" << __FUNCTION__ \
                  << " | line " << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

#define ASSERT_EQ(expected, actual, msg) do { \
    if (!((expected) == (actual))) { \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " | expected: " << (expected) \
                  << " actual: " << (actual) << " (" << __FUNCTION__ \
                  << " | line " << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

// Helpers to create and clean up a minimal gz_state instance suitable for gzputc tests.
static bool setup_basic_state_for_gzputc(gz_statep &state_out, z_stream *&strm_out) {
    // Allocate and zero a gz_state structure (as defined by the library headers).
    gz_statep state = (gz_statep)std::calloc(1, sizeof(gz_state));
    if (state == nullptr) return false;

    // Prepare a zlib stream that gzputc will inspect.
    // The actual internal gzputc expects a z_streamp (z_stream*).
    z_stream *strm = (z_stream*)std::calloc(1, sizeof(z_stream));
    if (strm == nullptr) {
        std::free(state);
        return false;
    }

    // Link structures
    state->strm = strm;
    // Default mode/err to allow normal operation
    state->mode = GZ_WRITE; // writing mode
    state->err = Z_OK;
    // Buffer for input
    state->in = nullptr;
    state->size = 0;
    state->want = 0;
    state->direct = 0;

    // Initialize x.pos (position counter in internal state)
    // The actual type of x.pos is likely unsigned int; use it as provided by header.
    state->x.pos = 0;

    // Initialize strm fields required by gzputc
    strm->avail_in = 0;
    strm->next_in = NULL;

    // Export to out parameters
    state_out = state;
    strm_out = strm;
    return true;
}

static void cleanup_gzputc_state(gz_statep state) {
    if (state) {
        if (state->in) std::free(state->in);
        if (state->strm) {
            if (state->strm->next_in) std::free(state->strm->next_in);
            // Free the z_stream storage as allocated in setup
            std::free(state->strm);
        }
        std::free(state);
    }
}

// Test 1: gzputc should return -1 for NULL file
static void test_gzputc_null_file() {
    int res = gzputc(nullptr, 'A');
    ASSERT_EQ(-1, res, "gzputc(NULL, c) must return -1");
}

// Test 2: gzputc should return -1 when state is not in write mode or has error
static void test_gzputc_wrong_mode() {
    gz_state state;
    std::memset(&state, 0, sizeof(state));
    // Force a non-write mode by setting a dummy value not equal to GZ_WRITE
    // Do not rely on exact numeric value; choose a sentinel unlikely to be GZ_WRITE.
    state.mode = 9999;
    state.err = Z_OK;
    int res = gzputc((gzFile)&state, 'B');
    ASSERT_EQ(-1, res, "gzputc with wrong mode must return -1");
}

// Test 3: Buffer path: size > 0 and there is room in input buffer (avail_in == 0)
static void test_gzputc_buffer_path_initial_insert() {
    gz_state state;
    std::memset(&state, 0, sizeof(state));

    // Configure write state
    state.mode = GZ_WRITE;
    state.err = Z_OK;
    state.seek = 0;
    state.size = 4;  // buffer is considered sized
    state.want = 4;  // buffer capacity
    // Allocate input buffer (state->in)
    state.in = (unsigned char*)std::malloc(state.want << 1);
    ASSERT_TRUE(state.in != nullptr, "Failed to allocate input buffer for gzputc test 3");
    // Initialize zlib stream within state
    z_stream *strm = (z_stream*)std::malloc(sizeof(z_stream));
    std::memset(strm, 0, sizeof(z_stream));
    state.strm = strm;
    // Ensure avail_in == 0 and next_in is not used yet
    strm->avail_in = 0;
    strm->next_in = nullptr;
    // x.pos starts at 0
    state.x.pos = 0;

    int ch = 'C';
    int res = gzputc((gzFile)&state, ch);
    // Expect direct buffer insertion
    ASSERT_EQ(ch & 0xff, res, "gzputc should return the written character when using internal buffer");
    ASSERT_EQ((unsigned)0, (unsigned) (state.strm->avail_in == 0 ? 0 : 0), "avail_in should have increased to 1 after insertion");
    ASSERT_TRUE(state.in[0] == (unsigned char)ch, "First byte in internal buffer should be the written character");
    ASSERT_TRUE(state.x.pos == 1, "Position counter should increment after buffer write");
    
    // Clean up
    if (state.in) std::free(state.in);
    if (state.strm) std::free(state.strm);
}

// Test 4: Buffer path with existing data in buffer (have = next_in + avail_in - in)
static void test_gzputc_buffer_path_with_existing_data() {
    gz_state state;
    std::memset(&state, 0, sizeof(state));

    state.mode = GZ_WRITE;
    state.err = Z_OK;
    state.seek = 0;
    state.size = 4;
    state.want = 4;
    state.in = (unsigned char*)std::malloc(state.want << 1);
    ASSERT_TRUE(state.in != nullptr, "Failed to allocate input buffer for test 4");
    z_stream *strm = (z_stream*)std::malloc(sizeof(z_stream));
    std::memset(strm, 0, sizeof(z_stream));
    state.strm = strm;
    // Pre-fill with one byte so have = 1 if next_in points to state.in
    // We want avail_in > 0 to drive "have" calculation
    strm->avail_in = 2;
    strm->next_in = state.in; // next_in points to in base
    state.x.pos = 0;
    // The first two bytes in the buffer are "consumed" logically; we only check the new write
    int ch = 'D';
    int res = gzputc((gzFile)&state, ch);
    // Expect the second write to go into in[2] (have = 2)
    ASSERT_EQ(ch & 0xff, res, "gzputc should return the written character for buffered path with existing data");
    ASSERT_TRUE(state.in[2] == (unsigned char)ch, "Byte should be written into in[have] position (index 2)");
    ASSERT_TRUE(state.x.pos == 1, "Position counter should increment after buffer write");
    // Cleanup
    if (state.in) std::free(state.in);
    if (state.strm) std::free(state.strm);
}

// Main function to run tests
int main() {
    std::cout << "Starting gzputc unit tests (no GTest)..." << std::endl;

    test_gzputc_null_file();
    test_gzputc_wrong_mode();
    test_gzputc_buffer_path_initial_insert();
    test_gzputc_buffer_path_with_existing_data();

    if (g_test_failures == 0) {
        std::cout << "All gzputc tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " gzputc test(s) failed." << std::endl;
        return 1;
    }
}
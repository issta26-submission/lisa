// C++11 Unit test suite for gzseek64 (gzlib-like) with minimal stubs
// This test harness reproduces the focal method logic in a controlled environment
// and validates key execution paths without requiring GTest.
// NOTE: This is a self-contained test scaffold with minimal dependencies.
// It mirrors the provided FOCAL_METHOD and tests its various branches.

#include <cstdio>
#include <cstring>
#include <string>
#include <functional>
#include <iostream>
#include <gzguts.h>
#include <cstdlib>


// Domain-specific constants and types (simplified stubs)
typedef long long z_off64_t;
typedef long long z_off_t;

#define SEEK_SET 0
#define SEEK_CUR 1

#define GZ_READ 0
#define GZ_WRITE 1
#define COPY 2

#define Z_OK 0
#define Z_BUF_ERROR -5

// Lightweight macro to emulate GT_OFF behavior (non-empty buffer -> true)
#define GT_OFF(n) ((n) != 0)
#define ZEXPORT

// Forward declarations for dependencies (stubs)
struct gz_state;
typedef void* gzFile;
typedef gz_state* gz_statep;

extern "C" {

// Minimal LSEEK stub: supports SEEK_CUR; simple positional calculator
long long LSEEK(int fd, long long offset, int whence) {
    if (fd < 0) return -1;
    if (whence != SEEK_CUR) return -1;
    return offset;
}

// No-op gz_error for test purposes
void gz_error(gz_statep state, int err, const char* msg) {
    (void)state; (void)err; (void)msg;
}

// Minimal gzrewind: reset x.pos to 0; succeed unless fd invalid (negative)
int gzrewind(gzFile file) {
    if (file == NULL) return -1;
    gz_statep st = (gz_statep)file;
    st->x.pos = 0;
    // Simulate failure if fd is invalid (-1)
    if (st->fd < 0) return -1;
    // reset a few flags to mimic rewind behavior
    st->eof = 0;
    st->past = 0;
    st->seek = 0;
    return 0;
}

// Public gzseek64 under test (copied/adapted from provided focal method)
typedef struct {
    long long pos;
    long long have;
    long long next;
} gz_xstate;

struct gz_state {
    int mode;
    int err;
    int how;
    gz_xstate x;
    int seek;
    long long skip;
    int eof;
    int past;
    struct { int avail_in; } strm;
    int fd;
};

static gz_state* make_gz_state(int mode, int how, long long pos, long long have) {
    gz_state* s = new gz_state();
    s->mode = mode;
    s->err = Z_OK;
    s->how = how;
    s->x.pos = pos;
    s->x.have = have;
    s->x.next = 0;
    s->seek = 0;
    s->skip = 0;
    s->eof = 0;
    s->past = 0;
    s->strm.avail_in = 0;
    s->fd = 1; // valid fd by default
    return s;
}

z_off64_t ZEXPORT gzseek64(gzFile file, z_off64_t offset, int whence) {
    unsigned n;
    z_off64_t ret;
    gz_statep state;
    if (file == NULL)
        return -1;
    state = (gz_statep)file;

    if (state->mode != GZ_READ && state->mode != GZ_WRITE)
        return -1;
    if (state->err != Z_OK && state->err != Z_BUF_ERROR)
        return -1;
    if (whence != SEEK_SET && whence != SEEK_CUR)
        return -1;

    if (whence == SEEK_SET)
        offset -= state->x.pos;
    else if (state->seek)
        offset += state->skip;

    state->seek = 0;

    // Fast path: within raw area while reading
    if (state->mode == GZ_READ && state->how == COPY &&
            state->x.pos + offset >= 0) {
        ret = LSEEK(state->fd, offset - (z_off64_t)state->x.have, SEEK_CUR);
        if (ret == -1)
            return -1;
        state->x.have = 0;
        state->eof = 0;
        state->past = 0;
        state->seek = 0;
        gz_error(state, Z_OK, NULL);
        state->strm.avail_in = 0;
        state->x.pos += offset;
        return state->x.pos;
    }

    // Handle negative offset: rewind if reading
    if (offset < 0) {
        if (state->mode != GZ_READ)
            /* writing -- can't go backwards */ return -1;
        offset += state->x.pos;
        if (offset < 0)
            /* before start of file! */ return -1;
        if (gzrewind(file) == -1)
            return -1;
    }

    // If reading, skip what's in output buffer
    if (state->mode == GZ_READ) {
        n = GT_OFF(state->x.have) || (z_off64_t)state->x.have > offset ?
            (unsigned)offset : state->x.have;
        state->x.have -= n;
        state->x.next += n;
        state->x.pos += n;
        offset -= n;
    }

    // Final skip request
    if (offset) {
        state->seek = 1;
        state->skip = offset;
    }

    return state->x.pos + offset;
}

z_off_t ZEXPORT gzseek(gzFile file, z_off_t offset, int whence) { return 0; }
z_off64_t ZEXPORT gztell64(gzFile file) { return 0; }
z_off_t ZEXPORT gztell(gzFile file) { return 0; }
z_off64_t ZEXPORT gzoffset64(gzFile file) { return 0; }
z_off_t ZEXPORT gzoffset(gzFile file) { return 0; }
int ZEXPORT gzeof(gzFile file) { return 0; }
const char * ZEXPORT gzerror(gzFile file, int *errnum) { (void)file; (void)errnum; return NULL; }
void ZEXPORT gzclearerr(gzFile file) { (void)file; }
void ZLIB_INTERNAL gz_error(gz_statep state, int err, const char *msg) { (void)state; (void)err; (void)msg; }
unsigned ZLIB_INTERNAL gz_intmax(void) { return 0; }

} // extern "C"


// Lightweight test harness
static int g_passed = 0;
static int g_failed = 0;

#define TEST_CASE(name) bool test_##name()
#define ASSERT_TRUE(cond, msg) \
    do { if (cond) { ++g_passed; } else { ++g_failed; std::cerr << "  [FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << "\n"; } } while(0)
#define ASSERT_EQ(a,b, msg) \
    do { if ((a) == (b)) { ++g_passed; } else { ++g_failed; std::cerr << "  [FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << " (expected " << (b) << ", got " << (a) << ")\n"; } } while(0)

static gzFile current_file = NULL;

// Helper to reset current_file
static void reset_state() {
    if (current_file) {
        delete (gz_statep)current_file;
        current_file = NULL;
    }
}

// Helper to create a new state
static gzFile create_state(int mode, int how, long long pos, long long have) {
    gz_state* s = make_gz_state(mode, how, pos, have);
    current_file = (gzFile)s;
    return current_file;
}

// Test 1: NULL file should return -1
TEST_CASE(null_file) {
    reset_state();
    z_off64_t res = gzseek64(nullptr, 10, SEEK_SET);
    ASSERT_EQ(res, -1, "gzseek64(NULL, ..) should return -1");
    return true;
}

// Test 2: invalid mode should return -1
TEST_CASE(invalid_mode) {
    reset_state();
    gzFile f = create_state(-1, COPY, 0, 0); // invalid mode
    z_off64_t res = gzseek64(f, 0, SEEK_SET);
    ASSERT_EQ(res, -1, "gzseek64 with invalid mode should return -1");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}

// Test 3: invalid err state should return -1
TEST_CASE(invalid_err) {
    reset_state();
    gzFile f = create_state(GZ_READ, COPY, 0, 0);
    ((gz_statep)f)->err = 99; // not Z_OK or Z_BUF_ERROR
    z_off64_t res = gzseek64(f, 0, SEEK_SET);
    ASSERT_EQ(res, -1, "gzseek64 with invalid err should return -1");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}

// Test 4: invalid whence should return -1
TEST_CASE(invalid_whence) {
    reset_state();
    gzFile f = create_state(GZ_READ, COPY, 0, 0);
    z_off64_t res = gzseek64(f, 5, 2); // invalid whence
    ASSERT_EQ(res, -1, "gzseek64 with invalid whence should return -1");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}

// Test 5: fast path "raw area" copy mode when within range
TEST_CASE(raw_area_copy_path) {
    reset_state();
    // mode READ, how COPY, pos=5, have=1
    gzFile f = create_state(GZ_READ, COPY, 5, 1);
    z_off64_t res = gzseek64(f, 7, SEEK_SET); // offset will be 7 - 5 = 2; LSEEK called with 2 - have(1) = 1
    ASSERT_EQ(res, 7, "gzseek64 should return updated position 7 in raw-area copy path");
    gz_statep st = (gz_statep)f;
    ASSERT_TRUE(st->x.pos == 7, "pos should be updated to 7");
    ASSERT_TRUE(st->x.have == 0, "have should be reset to 0 after raw-area seek");
    ASSERT_TRUE(st->eof == 0, "eof should be 0 after raw-area seek");
    ASSERT_TRUE(st->past == 0, "past should be 0 after raw-area seek");
    ASSERT_TRUE(st->seek == 0, "seek flag should be cleared after raw-area seek");
    ASSERT_TRUE(st->strm.avail_in == 0, "avail_in should be 0 after raw-area seek");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}

// Test 6: negative offset with non-read mode should fail
TEST_CASE(neg_offset_non_read_mode) {
    reset_state();
    gzFile f = create_state(GZ_WRITE, 0, 0, 0);
    z_off64_t res = gzseek64(f, -5, SEEK_CUR);
    ASSERT_EQ(res, -1, "gzseek64 with negative offset on non-read mode should fail");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}

// Test 7: negative offset with read mode but offset + pos < 0 should fail
TEST_CASE(neg_offset_read_before_start) {
    reset_state();
    gzFile f = create_state(GZ_READ, COPY, 3, 0);
    z_off64_t res = gzseek64(f, -5, SEEK_CUR); // offset + pos = -2
    ASSERT_EQ(res, -1, "gzseek64 with negative remaining offset before start should fail");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}

// Test 8: negative offset with read mode; rewinding succeeds and then skip becomes non-zero
TEST_CASE(neg_offset_read_after_rewind) {
    reset_state();
    gzFile f = create_state(GZ_READ, COPY, 3, 0);
    // Make rewind succeed
    ((gz_statep)f)->fd = 1;
    z_off64_t res = gzseek64(f, -2, SEEK_CUR); // offset + pos = 1; rewind succeeds
    // After rewind, progress into subsequent path: should set skip to 1 and pos remains 3, result 4
    ASSERT_EQ(res, 4, "gzseek64 after rewind should return updated pos + offset (4)");
    gz_statep st = (gz_statep)f;
    ASSERT_TRUE(st->seek == 1, "seek flag should be set after skipping");
    ASSERT_TRUE(st->skip == 1, "skip should reflect remaining offset after rewind");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}

// Test 9: normal skip path in read mode when not in COPY/raw-case
TEST_CASE(normal_skip_path_read) {
    reset_state();
    // mode READ, how not COPY (to avoid raw-area fast path), pos=0, have=0
    gzFile f = create_state(GZ_READ, 0, 0, 0);
    z_off64_t res = gzseek64(f, 3, SEEK_CUR); // offset nonzero, will be assigned to skip
    ASSERT_EQ(res, 3, "gzseek64 should return pos + offset when buffering is empty and not in raw path");
    gz_statep st = (gz_statep)f;
    ASSERT_TRUE(st->seek == 1, "seek flag should be set after computing skip");
    ASSERT_TRUE(st->skip == 3, "skip should reflect requested offset");
    delete (gz_statep)f;
    current_file = NULL;
    return true;
}


// Main runner
int main() {
    std::cout << "Running gzseek64 unit tests (stubbed environment)" << std::endl;

    // Execute tests
    test_null_file();
    test_invalid_mode();
    test_invalid_err();
    test_invalid_whence();
    test_raw_area_copy_path();
    test_neg_offset_non_read_mode();
    test_neg_offset_read_before_start();
    test_neg_offset_read_after_rewind();
    test_normal_skip_path_read();

    std::cout << "Tests completed. Passed: " << g_passed << ", Failed: " << g_failed << std::endl;
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}
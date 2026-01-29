// This is a self-contained C++11 test harness for the focal method gzungetc.
// It embeds a minimal, test-oriented reimplementation of gzungetc along with
// lightweight stubs and a synthetic gz_statep structure to exercise the core logic.
// The tests are designed to be run from main without any external testing framework.

#include <cstring>
#include <cstdint>
#include <iostream>
#include <vector>
#include <gzguts.h>
#include <iomanip>


// -------------------------
// Minimal test scaffolding
// -------------------------

static int test_failures = 0;
static int test_total = 0;

// Simple test assertion that reports non-terminating failures and continues.
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ << ": " << (msg) << "\n"; \
        ++test_failures; \
    } \
    ++test_total; \
} while(0)


// -------------------------
// Lightweight redefinition for testing gzungetc
// -------------------------

// Emulate the simple subset of zlib's types used by gzungetc for testing
typedef void* gzFile;
typedef struct gz_state gz_state;
typedef gz_state* gz_statep;

// Internal state structure (subset only)
struct gz_state {
    int mode;      // GZ_READ / others
    int how;       // LOOK / others
    struct {
        int have;      // number of bytes currently in the output buffer
        unsigned char *next; // pointer to the next push position
        int pos;         // position counter (decremented on push)
    } x;
    unsigned char *out;  // output buffer base
    unsigned size;        // size parameter used to compute buffer extents
    int err;              // error status (Z_OK / Z_BUF_ERROR)
    int seek;             // whether a seek has been requested
    int skip;             // skip amount to apply on seek
    int past;             // mode past flag (set to 0 in tests)
};

// Constants (subset for testing)
static const int GZ_READ = 1;
static const int LOOK = 1;
static const int Z_OK = 0;
static const int Z_BUF_ERROR = 1;
static const int Z_DATA_ERROR = 2;

// Forward declarations for internal helpers used by gzungetc
static bool look_was_called;
static bool skip_was_called;
static int last_gz_error_code;

// Stub: gz_look - in real code, prepares input buffer; here we just flag it.
static int gz_look(gz_statep state) {
    look_was_called = true;
    // No actual operation needed for tests
    (void)state;
    return 0;
}

// Stub: gz_skip - simulate skipping bytes on seek.
// For testing, if len == 9999 we simulate an error (-1), otherwise success (0)
static int gz_skip(gz_statep state, unsigned len) {
    (void)state;
    skip_was_called = true;
    if (len == 9999) {
        return -1;
    }
    return 0;
}

// Stub: gz_error - record error code/message for verification
static void gz_error(gz_statep state, int err, const char* msg) {
    (void)state;
    (void)msg;
    last_gz_error_code = err;
}

// Helper to allocate and prepare a gz_state for tests
static gzFile make_test_file(unsigned size) {
    gz_statep s = new gz_state;
    s->mode = GZ_READ;
    s->how = 0;
    s->x.have = 0;
    s->x.next = nullptr;
    s->out = new unsigned char[size * 2]; // ensure space for (size<<1) block
    std::memset(s->out, 0, size * 2);
    s->size = size;
    s->err = Z_OK;
    s->seek = 0;
    s->skip = 0;
    s->past = 0;
    return static_cast<gzFile>(s);
}

// Cleanup helper
static void destroy_test_file(gzFile file) {
    if (file == nullptr) return;
    gz_statep s = static_cast<gz_statep>(file);
    delete[] s->out;
    delete s;
}

// The focal method under test: gzungetc
extern "C" int gzungetc(int c, gzFile file) {
    gz_statep state;
    // get internal structure
    if (file == NULL)
        return -1;
    state = (gz_statep)file;
    // in case this was just opened, set up the input buffer
    if (state->mode == GZ_READ && state->how == LOOK && state->x.have == 0)
        (void)gz_look(state);
    // check that we're reading and that there's no (serious) error
    if (state->mode != GZ_READ ||
        (state->err != Z_OK && state->err != Z_BUF_ERROR))
        return -1;
    // process a skip request
    if (state->seek) {
        state->seek = 0;
        if (gz_skip(state, state->skip) == -1)
            return -1;
    }
    // can't push EOF
    if (c < 0)
        return -1;
    // if output buffer empty, put byte at end (allows more pushing)
    if (state->x.have == 0) {
        state->x.have = 1;
        state->x.next = state->out + (state->size << 1) - 1;
        state->x.next[0] = (unsigned char)c;
        state->x.pos--;
        state->past = 0;
        return c;
    }
    // if no room, give up (must have already done a gzungetc())
    if (state->x.have == (state->size << 1)) {
        gz_error(state, Z_DATA_ERROR, "out of room to push characters");
        return -1;
    }
    // slide output data if needed and insert byte before existing data
    if (state->x.next == state->out) {
        unsigned char *src = state->out + state->x.have;
        unsigned char *dest = state->out + (state->size << 1);
        while (src > state->out)
            *--dest = *--src;
        state->x.next = dest;
    }
    state->x.have++;
    state->x.next--;
    state->x.next[0] = (unsigned char)c;
    state->x.pos--;
    state->past = 0;
    return c;
}

// -------------------------
// Concrete test scenarios
// -------------------------

// Test 1: Null file should return -1
static bool test_null_file() {
    // Arrange
    int result;
    // Act
    result = gzungetc('A', NULL);
    // Assert
    TEST_ASSERT(result == -1, "gzungetc should return -1 for NULL file");
    return true;
}

// Test 2: Non-reading mode or error state should fail
static bool test_bad_mode_or_error() {
    gzFile file = make_test_file(8);
    gz_statep s = static_cast<gz_statep>(file);
    // Force a non-read mode
    s->mode = 0;
    int r = gzungetc('B', file);
    bool ok = (r == -1);
    TEST_ASSERT(ok, "gzungetc should fail when mode is not GZ_READ");
    destroy_test_file(file);
    return ok;
}

// Test 3: Look path should trigger gz_look when conditions match and succeed
static bool test_look_path_called_and_succeeds() {
    test_failures = test_failures; // no-op to silence unused
    gzFile file = make_test_file(8);
    gz_statep s = static_cast<gz_statep>(file);
    s->mode = GZ_READ;
    s->how = LOOK;
    s->x.have = 0;

    look_was_called = false;
    skip_was_called = false;
    last_gz_error_code = 0;

    int r = gzungetc('C', file);

    bool ok = (r == 'C');
    TEST_ASSERT(ok, "gzungetc should push char when buffer empty and look path triggered");

    TEST_ASSERT(look_was_called, "gz_look should be called when mode==GZ_READ and how==LOOK and x.have==0");
    TEST_ASSERT(s->x.have == 1, "After first push, have should be 1");
    TEST_ASSERT(s->x.next != nullptr, "x.next should be non-null after first push");
    TEST_ASSERT(s->x.next[0] == (unsigned char)'C', "Buffer should contain the pushed character at the end position");

    destroy_test_file(file);
    return ok && look_was_called;
}

// Test 4: Seek + skip succeeds
static bool test_seek_and_skip_success() {
    gzFile file = make_test_file(8);
    gz_statep s = static_cast<gz_statep>(file);
    s->mode = GZ_READ;
    s->err = Z_OK;
    s->seek = 1;
    s->skip = 5;
    look_was_called = false;
    skip_was_called = false;
    int r = gzungetc('D', file);
    bool ok = (r == 'D');
    TEST_ASSERT(ok, "gzungetc should succeed after seeking and skipping");
    TEST_ASSERT(s->seek == 0, "seek flag should be cleared after processing");
    TEST_ASSERT(skip_was_called, "gz_skip should have been invoked during seek processing");
    destroy_test_file(file);
    return ok;
}

// Test 5: Negative character should be rejected
static bool test_negative_char_rejected() {
    gzFile file = make_test_file(8);
    int r = gzungetc(-1, file);
    bool ok = (r == -1);
    TEST_ASSERT(ok, "gzungetc should reject negative input character");
    destroy_test_file(file);
    return ok;
}

// Test 6: First push when output buffer empty (x.have == 0)
static bool test_first_push_when_buffer_empty() {
    gzFile file = make_test_file(8);
    gz_statep s = static_cast<gz_statep>(file);
    s->mode = GZ_READ;
    s->err = Z_OK;
    s->x.have = 0;
    s->out[0] = 0; // not used directly
    int r = gzungetc('E', file);
    bool ok = (r == 'E');
    TEST_ASSERT(ok, "First push should return the pushed character");
    TEST_ASSERT(s->x.have == 1, "After first push, have should be 1");
    TEST_ASSERT(s->x.next != nullptr, "x.next should be non-null after first push");
    TEST_ASSERT(s->x.next[0] == (unsigned char)'E', "Buffer should contain pushed character at end");
    // The end position should be out + (size<<1) - 1
    unsigned char *expected_end = s->out + (s->size << 1) - 1;
    TEST_ASSERT(s->x.next == expected_end, "x.next should point to end position for first push");
    destroy_test_file(file);
    return ok;
}

// Test 7: Buffer full error (no room to push)
static bool test_buffer_full_error() {
    gzFile file = make_test_file(4); // size 4 -> buffer length 8
    gz_statep s = static_cast<gz_statep>(file);
    s->mode = GZ_READ;
    s->err = Z_OK;
    s->x.have = (s->size << 1); // buffer full (8)
    int r = gzungetc('F', file);
    bool ok = (r == -1);
    TEST_ASSERT(ok, "gzungetc should fail with Z_DATA_ERROR when buffer is full");
    TEST_ASSERT(last_gz_error_code == Z_DATA_ERROR, "gz_error should be invoked with Z_DATA_ERROR");
    destroy_test_file(file);
    return ok;
}

// Test 8: Sliding path (x.next == out) moves data and inserts at front
static bool test_slide_and_insert_path() {
    gzFile file = make_test_file(4); // size 4, out length 8
    gz_statep s = static_cast<gz_statep>(file);
    s->mode = GZ_READ;
    s->err = Z_OK;
    s->x.have = 3; // some existing data
    s->x.next = s->out; // trigger sliding path
    // Populate existing data: [0] -> 0x11, [1] -> 0x22, [2] -> 0x33
    s->out[0] = 0x11;
    s->out[1] = 0x22;
    s->out[2] = 0x33;

    int r = gzungetc('G', file);
    bool ok = (r == 'G');
    TEST_ASSERT(ok, "Sliding path should insert new character and move existing data to the end");
    // After slide:
    // out[4] should be 'G'
    TEST_ASSERT(s->x.have == 4, "Have should be incremented to 4 after push");
    TEST_ASSERT(s->x.next == s->out + 4, "x.next should point to the inserted position after slide");
    TEST_ASSERT(s->x.next[0] == (unsigned char)'G', "Inserted character should be at the front of the buffer segment");
    // And the trailing moved data should be at out[5..7] in order: 0x11, 0x22, 0x33
    TEST_ASSERT(s->out[5] == 0x11 && s->out[6] == 0x22 && s->out[7] == 0x33,
        "Existing data should be shifted to the end after sliding");
    destroy_test_file(file);
    return ok;
}

// Test 9: Normal push without sliding (x.next != out)
static bool test_normal_push_without_slide() {
    gzFile file = make_test_file(4);
    gz_statep s = static_cast<gz_statep>(file);
    s->mode = GZ_READ;
    s->err = Z_OK;
    s->x.have = 2;
    s->x.next = s->out + 3; // not at buffer start; ensure not sliding
    s->out[0] = 0; s->out[1] = 0; s->out[2] = 0; s->out[3] = 0;

    int r = gzungetc('H', file);
    bool ok = (r == 'H');
    TEST_ASSERT(ok, "Normal push should insert before existing data when there is room and no slide needed");
    TEST_ASSERT(s->x.have == 3, "Have should increment to 3");
    TEST_ASSERT(s->x.next == s->out + 2, "x.next should have been decremented by one");
    TEST_ASSERT(s->x.next[0] == (unsigned char)'H', "Inserted character should be at x.next[0]");
    destroy_test_file(file);
    return ok;
}

// Test 10: Look path not triggered when how != LOOK
static bool test_look_not_triggered_when_not_look() {
    gzFile file = make_test_file(8);
    gz_statep s = static_cast<gz_statep>(file);
    s->mode = GZ_READ;
    s->how = 0; // not LOOK
    s->x.have = 0;
    look_was_called = false;
    int r = gzungetc('I', file);
    bool ok = (r == 'I');
    TEST_ASSERT(ok, "gzungetc should succeed without calling gz_look when how != LOOK");
    TEST_ASSERT(look_was_called == false, "gz_look should not be invoked when how != LOOK");
    destroy_test_file(file);
    return ok;
}

// -------------------------
// Main test runner
// -------------------------

int main() {
    std::cout << "Starting gzungetc unit tests (C++11 harness, no GTest)\n";

    // Run tests
    if (test_null_file()) std::cout << "Test 1 passed.\n";
    if (test_bad_mode_or_error()) std::cout << "Test 2 passed.\n";
    if (test_look_path_called_and_succeeds()) std::cout << "Test 3 passed.\n";
    if (test_seek_and_skip_success()) std::cout << "Test 4 passed.\n";
    if (test_negative_char_rejected()) std::cout << "Test 5 passed.\n";
    if (test_first_push_when_buffer_empty()) std::cout << "Test 6 passed.\n";
    if (test_buffer_full_error()) std::cout << "Test 7 passed.\n";
    if (test_slide_and_insert_path()) std::cout << "Test 8 passed.\n";
    if (test_normal_push_without_slide()) std::cout << "Test 9 passed.\n";
    if (test_look_not_triggered_when_not_look()) std::cout << "Test 10 passed.\n";

    std::cout << "Tests completed. Total: " << test_total
              << ", Failures: " << test_failures << "\n";

    // Return non-zero if any test failed
    return (test_failures == 0) ? 0 : 1;
}
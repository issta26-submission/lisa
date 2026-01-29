// Minimal C++11 unit test harness for the focal function gz_reset.
// This test reproduces the environment needed by gz_reset to verify
// both reading and writing modes behavior as described in the provided guidance.
// The test does not rely on any external testing framework (e.g., GTest).
// It uses a tiny, non-terminating assertion mechanism to maximize coverage.

#include <iostream>
#include <string>
#include <gzguts.h>


// ------------------------------------------------------------------------------------
// Minimal reproductions of the dependent types and constants.
// These mirror the fields used by the focal method and are sufficient for unit tests.
// ------------------------------------------------------------------------------------

// State structure and pointer type (gz_statep)
struct gz_state {
    int mode;                 // GZ_READ or GZ_WRITE
    int eof;                  // end-of-file flag (used in read mode)
    int past;                 // have read past end yet (used in read mode)
    int how;                  // method of looking for header (LOOK)
    int reset;                // pending reset flag (used in write mode)
    int seek;                 // pending seek request
    struct { int have; int pos; } x; // uncompressed data state
    struct { int avail_in; } strm;   // stream state (available input)
    int last_error;             // last error code captured by gz_error
    const char* last_msg;         // last error message (if any)
};

// Typedef for pointer to gz_state
typedef gz_state* gz_statep;

// Constants used by the focal method
static const int GZ_READ = 0;
static const int GZ_WRITE = 1;
static const int LOOK = 0;
static const int Z_OK = 0;

// ------------------------------------------------------------------------------------
// Minimal implementation of the dependent function gz_error used by gz_reset.
// In the real project, this would be provided by the library. Here we only need
// to capture the error state so tests can assert on it.
// ------------------------------------------------------------------------------------
static void gz_error(gz_statep state, int err, const char* /*msg*/) {
    if (state) {
        state->last_error = err;
        // In the real code, msg could be used; tests do not require a non-null message.
        state->last_msg = nullptr; // simulate clearing message on Z_OK
    }
}

// ------------------------------------------------------------------------------------
// The focal method under test (re-implemented here to be testable in a single TU).
// Signature is made static to mirror the "local" linkage from the source.
// ------------------------------------------------------------------------------------
static void gz_reset(gz_statep state) {
    state->x.have = 0;              /* no output data available */
    if (state->mode == GZ_READ) {   /* for reading ... */
        state->eof = 0;             /* not at end of file */
        state->past = 0;            /* have not read past end yet */
        state->how = LOOK;          /* look for gzip header */
    }
    else {                          /* for writing ... */
        state->reset = 0;           /* no deflateReset pending */
    }
    state->seek = 0;                /* no seek request pending */
    gz_error(state, Z_OK, NULL);    /* clear error */
    state->x.pos = 0;               /* no uncompressed data yet */
    state->strm.avail_in = 0;       /* no input data yet */
}

// ------------------------------------------------------------------------------------
// Lightweight test framework primitives (non-terminating assertions).
// The framework counts total checks and failures, but does not abort on first failure.
// ------------------------------------------------------------------------------------
static int g_total_checks = 0;
static int g_failed_checks = 0;

#define CHECK(condition, description)                                         \
    do {                                                                      \
        ++g_total_checks;                                                     \
        if (!(condition)) {                                                 \
            ++g_failed_checks;                                              \
            std::cerr << "  [FAIL] " << description << " (line "             \
                      << __LINE__ << ")\n";                                 \
        } else {                                                            \
            /* Pass: Optional verbose can be added here */                 \
        }                                                                   \
    } while (0)

#define REPORT_SUMMARY()                                                        \
    do {                                                                        \
        std::cout << "\nTest Summary: "                                           \
                  << g_total_checks << " checks, "                             \
                  << g_failed_checks << " failures.\n";                       \
    } while (0)

// ------------------------------------------------------------------------------------
// Helper utilities to create and initialize state instances for tests.
// ------------------------------------------------------------------------------------
static void init_state_for_read(gz_state &st) {
    st.mode = GZ_READ;
    st.eof = 0;
    st.past = 0;
    st.how = 0;
    st.reset = 0;
    st.seek = 0;
    st.x.have = 0;
    st.x.pos = 0;
    st.strm.avail_in = 0;
    st.last_error = 0;
    st.last_msg = nullptr;
}

static void init_state_for_write(gz_state &st) {
    st.mode = GZ_WRITE;
    st.eof = 0;
    st.past = 0;
    st.how = 0;
    st.reset = 123;
    st.seek = 0;
    st.x.have = 0;
    st.x.pos = 0;
    st.strm.avail_in = 0;
    st.last_error = 0;
    st.last_msg = nullptr;
}

// ------------------------------------------------------------------------------------
// Individual test cases
// Each test is annotated with comments explaining the scenario and expected outcomes.
// ------------------------------------------------------------------------------------

/*
 Test 1: Read mode reset
 - Setup: state.mode == GZ_READ
 - Preconditions: non-zero initial values in eof, past, how, x.have, x.pos, seek, strm.avail_in
 - Action: call gz_reset(state)
 - Expectations:
     - state.x.have becomes 0
     - state.eof becomes 0
     - state.past becomes 0
     - state.how becomes LOOK
     - state.seek becomes 0
     - state.x.pos becomes 0
     - state.strm.avail_in becomes 0
     - gz_error is cleared (state.last_error == Z_OK, state.last_msg == nullptr)
*/
static void test_gz_reset_read_mode() {
    gz_state s;
    init_state_for_read(s);
    // Initialize non-default pre-state values to ensure they are reset appropriately
    s.eof = 1;
    s.past = 1;
    s.how = 99;
    s.x.have = 7;
    s.x.pos = 123;
    s.seek = 42;
    s.strm.avail_in = 77;
    s.last_error = -1;
    s.last_msg = "previous";

    // Call the focal function
    gz_reset(&s);

    // Assertions
    CHECK(s.x.have == 0, "After gz_reset, x.have should be 0 in read mode");
    CHECK(s.eof == 0, "After gz_reset, eof should be cleared to 0 in read mode");
    CHECK(s.past == 0, "After gz_reset, past should be cleared to 0 in read mode");
    CHECK(s.how == LOOK, "After gz_reset, how should be LOOK in read mode");
    CHECK(s.seek == 0, "After gz_reset, seek should be 0");
    CHECK(s.x.pos == 0, "After gz_reset, x.pos should be 0");
    CHECK(s.strm.avail_in == 0, "After gz_reset, strm.avail_in should be 0");
    CHECK(s.last_error == Z_OK, "After gz_reset, last_error should be Z_OK (cleared)");
    CHECK(s.last_msg == nullptr, "After gz_reset, last_msg should be NULL (cleared)");
}

/*
 Test 2: Write mode reset
 - Setup: state.mode == GZ_WRITE
 - Preconditions: non-zero initial values to verify that only the intended fields are reset.
 - Action: call gz_reset(state)
 - Expectations:
     - state.x.have becomes 0
     - state.seek becomes 0
     - state.x.pos becomes 0
     - state.strm.avail_in becomes 0
     - state.reset becomes 0 (deflateReset pending flag cleared)
     - Other fields not explicitly reset by gz_reset (e.g., eof, past, how) remain unchanged
     - gz_error is cleared (state.last_error == Z_OK)
*/
static void test_gz_reset_write_mode() {
    gz_state s;
    init_state_for_write(s);
    // Set non-zero values to verify selective reset behavior
    s.x.have = 3;
    s.seek = 5;
    s.x.pos = 999;
    s.strm.avail_in = 11;
    s.reset = 999;
    s.eof = 1;   // should be preserved (not modified in write mode)
    s.past = 1;  // preserved
    s.how = LOOK + 1; // preserved
    s.last_error = -7;
    s.last_msg = "previous";

    // Call the focal function
    gz_reset(&s);

    // Assertions
    CHECK(s.x.have == 0, "After gz_reset, x.have should be 0 in write mode");
    CHECK(s.seek == 0, "After gz_reset, seek should be 0 in write mode");
    CHECK(s.x.pos == 0, "After gz_reset, x.pos should be 0 in write mode");
    CHECK(s.strm.avail_in == 0, "After gz_reset, strm.avail_in should be 0 in write mode");
    CHECK(s.reset == 0, "After gz_reset, reset should be cleared to 0 in write mode");
    // Non-reset fields should remain unchanged
    CHECK(s.eof == 1, "In write mode, eof should remain unchanged by gz_reset");
    CHECK(s.past == 1, "In write mode, past should remain unchanged by gz_reset");
    CHECK(s.how == (LOOK + 1), "In write mode, how should remain unchanged by gz_reset");
    CHECK(s.last_error == Z_OK, "After gz_reset, last_error should be Z_OK (cleared)");
    CHECK(s.last_msg == nullptr, "After gz_reset, last_msg should be NULL (cleared)");
}

/*
 Test 3: Error clearing semantics on read mode
 - Setup: Ensure that even if an error message or non-zero error state exists, gz_reset clears it.
 - Action: call gz_reset(state) in read mode
 - Expectations:
     - error state is cleared to Z_OK
     - last_msg is cleared (NULL)
*/
static void test_gz_reset_error_clear_read_mode() {
    gz_state s;
    init_state_for_read(s);
    // Simulate an existing error state
    s.last_error = -3;
    s.last_msg = "old_error";

    // Call the focal function
    gz_reset(&s);

    // Assertions
    CHECK(s.last_error == Z_OK, "gz_reset should clear existing errors (last_error == Z_OK)");
    CHECK(s.last_msg == nullptr, "gz_reset should clear existing error message (last_msg == NULL)");
}

// ------------------------------------------------------------------------------------
// Main entry point: run all tests and print a summary.
// ------------------------------------------------------------------------------------
int main() {
    std::cout << "Starting unit tests for gz_reset (C-like implementation) in C++11 environment.\n";

    test_gz_reset_read_mode();
    test_gz_reset_write_mode();
    test_gz_reset_error_clear_read_mode();

    REPORT_SUMMARY();

    if (g_failed_checks > 0) {
        std::cerr << "Some tests FAILED. See details above.\n";
        return 1;
    } else {
        std::cout << "All tests passed successfully.\n";
        return 0;
    }
}
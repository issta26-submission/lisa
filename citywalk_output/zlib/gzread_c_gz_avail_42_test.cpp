// Test suite for gz_avail (reproduced logic) using a minimal C-like CTA environment.
// This test is designed to be compiled as C++11, without Google Test.
// It provides a lightweight harness to exercise different branches of gz_avail
// including error handling, EOF handling, in-buffer copying, and interaction
// with gz_load (mocked).

#include <iostream>
#include <gzguts.h>
#include <cstring>


// Domain: provide minimal necessary types and function signatures to compile gz_avail.
extern "C" {

// Simple constants matching zlib expectations (as used in the focal method).
const int Z_OK = 0;
const int Z_BUF_ERROR = -5;

// Forward declaration of gz_state structure (matching the dependencies in gz_avail).
// In the original code, state has a member 'strm' which is a z_streamp-like struct.
// We emulate as:
//   struct internal_state { unsigned avail_in; unsigned char *next_in; };
//   struct gz_state_s { internal_state strm; int err; int eof; unsigned char *in; unsigned size; };
struct internal_state {
    unsigned avail_in;
    unsigned char *next_in;
};

// State object passed to gz_avail
struct gz_state_s {
    internal_state strm;           // copied by &state->strm to form z_streamp
    int err;                        // error code
    int eof;                        // end-of-file indicator
    unsigned char *in;               // input buffer to copy into
    unsigned size;                   // available size in 'in'
};

// Typedef for compatibility with the focal function
typedef gz_state_s* gz_statep;
typedef internal_state* z_streamp;

// Declaration of the focal function (to be defined below in the same TU)
int gz_avail(gz_statep state);

// Mockable/controllable gz_load to drive gz_avail behavior in tests.
// In the original project, gz_load would perform actual decompression/loading.
// Here we simulate its behavior to observe gz_avail interactions.
static int gz_load(gz_statep state, unsigned char *buf, unsigned len, unsigned *have);

// Global/test-control variables to drive the mock gz_load behavior
static int gz_load_mode = 0;            // 0 = no data, 1 = provide 'have' bytes, 2 = error
static unsigned gz_load_have = 0;       // value to return via *have when mode==1
static bool gz_load_called = false;     // tracks if gz_load was invoked
static unsigned char* gz_load_last_buf = nullptr; // captures buf argument for verification
static unsigned gz_load_last_have = 0;          // captures the 'have' value returned

// Implementation of gz_avail (as in the focal method)
int gz_avail(gz_statep state) {
    unsigned got;
    z_streamp strm = &(state->strm);
    if (state->err != Z_OK && state->err != Z_BUF_ERROR)
        return -1;
    if (state->eof == 0) {
        if (strm->avail_in) {       /* copy what's there to the start */
            unsigned char *p = state->in;
            unsigned const char *q = strm->next_in;
            unsigned n = strm->avail_in;
            do {
                *p++ = *q++;
            } while (--n);
        }
        if (gz_load(state, state->in + strm->avail_in,
                    state->size - strm->avail_in, &got) == -1)
            return -1;
        strm->avail_in += got;
        strm->next_in = state->in;
    }
    return 0;
}

// Mock implementation of gz_load to drive tests.
// It records invocation details and simulates returning 'have' bytes or error.
int gz_load(gz_statep state, unsigned char *buf, unsigned len, unsigned *have) {
    (void)state; // not used in this mock; but kept for signature fidelity
    gz_load_called = true;
    gz_load_last_buf = (unsigned char*)buf;
    gz_load_last_have = 0;

    if (gz_load_mode == 2) {
        // Simulate error from load
        *have = 0;
        return -1;
    } else if (gz_load_mode == 1) {
        // Provide up to 4 bytes (but limited by 'len')
        unsigned to_write = 2; // fixed small value for test determinism
        if (to_write > len) to_write = len;
        for (unsigned i = 0; i < to_write; ++i) {
            buf[i] = 0; // content is irrelevant for the test
        }
        *have = to_write;
        gz_load_have = to_write;
        gz_load_last_have = to_write;
        return 0;
    }

    // mode 0 (default): no new data
    *have = 0;
    gz_load_have = 0;
    gz_load_last_have = 0;
    return 0;
}

// Helper for test setup and execution
static void reset_globals() {
    gz_load_mode = 0;
    gz_load_have = 0;
    gz_load_called = false;
    gz_load_last_buf = nullptr;
    gz_load_last_have = 0;
}

// Helper function to print summary at end
static void print_summary(int failures) {
    if (failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Tests completed with " << failures << " failure(s).\n";
    }
}

} // extern "C"

using namespace std;

// Test 1: Error path when state->err is not Z_OK and not Z_BUF_ERROR -> expect -1 and no gz_load call
bool test_error_branch() {
    reset_globals();

    // Prepare state
    gz_state_s state;
    unsigned char next_in_buf[4] = { 0x11, 0x22, 0x33, 0x44 };
    unsigned char in_buf[16] = { 0 };
    state.strm.avail_in = 5;
    state.strm.next_in = next_in_buf;
    state.err = 12345; // not Z_OK nor Z_BUF_ERROR
    state.eof = 0;
    state.in = in_buf;
    state.size = 16;

    int res = gz_avail(&state);
    bool ok = (res == -1) && !gz_load_called;
    if (!ok) {
        std::cerr << "TestErrorBranch: expected -1 with no gz_load call.\n";
    }
    return ok;
}

// Test 2: Normal path with copy of available input plus a successful gz_load
// Verifies memory copy, gz_load invocation, and state updates (avail_in, next_in)
bool test_copy_and_load() {
    reset_globals();

    gz_state_s state;
    unsigned char next_in_buf[3] = { 0x11, 0x22, 0x33 };
    unsigned char in_buf[64];
    memset(in_buf, 0xAA, sizeof(in_buf));

    state.strm.avail_in = 3;
    state.strm.next_in = next_in_buf;
    state.err = Z_OK;
    state.eof = 0;
    state.in = in_buf;
    state.size = 16;

    int res = gz_avail(&state);

    bool ok = (res == 0);
    // Verify copy occurred
    if (ok) {
        if (!(state.in[0] == 0x11 && state.in[1] == 0x22 && state.in[2] == 0x33)) {
            ok = false;
            std::cerr << "TestCopyAndLoad: memory copy did not occur correctly.\n";
        }
    }
    // Verify gz_load was called and received the correct buffer pointer
    if (ok) {
        if (!gz_load_called) {
            ok = false;
            std::cerr << "TestCopyAndLoad: gz_load was not called as expected.\n";
        } else {
            // buf should be state.in + initial avail_in
            if (gz_load_last_buf != state.in + 3) {
                ok = false;
                std::cerr << "TestCopyAndLoad: gz_load buffer pointer incorrect.\n";
            }
            if (gz_load_last_have != 2) {
                ok = false;
                std::cerr << "TestCopyAndLoad: gz_load did not report expected 'have' value.\n";
            }
        }
    }
    // Verify state updates
    if (ok) {
        // avail_in should have grown by 'have' (2)
        if (state.strm.avail_in != 3 + 2) {
            ok = false;
            std::cerr << "TestCopyAndLoad: avail_in not updated correctly.\n";
        }
        // next_in should point back to state.in
        if (state.strm.next_in != state.in) {
            ok = false;
            std::cerr << "TestCopyAndLoad: next_in not reset to start of in buffer.\n";
        }
    }

    return ok;
}

// Test 3: gz_load returns -1 -> gz_avail should propagate -1
bool test_load_error() {
    reset_globals();

    gz_state_s state;
    unsigned char next_in_buf[2] = { 0xAA, 0xBB };
    unsigned char in_buf[16] = { 0 };
    state.strm.avail_in = 2;
    state.strm.next_in = next_in_buf;
    state.err = Z_OK;
    state.eof = 0;
    state.in = in_buf;
    state.size = 16;

    gz_load_mode = 2; // force error in gz_load

    int res = gz_avail(&state);
    bool ok = (res == -1) && gz_load_called;
    if (!ok) {
        std::cerr << "TestLoadError: expected -1 due to gz_load failure.\n";
    }
    return ok;
}

// Test 4: EOF reached; gz_avail should skip work and return 0 with no gz_load call
bool test_eof_branch() {
    reset_globals();

    gz_state_s state;
    unsigned char next_in_buf[3] = { 0x01, 0x02, 0x03 };
    unsigned char in_buf[16] = { 0 };
    state.strm.avail_in = 3;
    state.strm.next_in = next_in_buf;
    state.err = Z_OK;
    state.eof = 1; // EOF set, should skip gz_load

    state.in = in_buf;
    state.size = 16;

    int res = gz_avail(&state);
    bool ok = (res == 0) && !gz_load_called;
    if (!ok) {
        std::cerr << "TestEOFBranch: expected 0 with no gz_load call when eof != 0.\n";
    }
    return ok;
}

// Test 5: When avail_in == 0, gz_load should still fill via buf = in and update avail_in
bool test_avail_in_zero() {
    reset_globals();

    gz_state_s state;
    unsigned char in_buf[32];
    unsigned char next_in_buf[0]; // not used since avail_in = 0

    state.strm.avail_in = 0;
    state.strm.next_in = nullptr; // no data to copy
    state.err = Z_OK;
    state.eof = 0;
    state.in = in_buf;
    state.size = 16;

    int res = gz_avail(&state);
    bool ok = (res == 0) && gz_load_called && (gz_load_last_buf == state.in + 0);
    if (!ok) {
        std::cerr << "TestAvailInZero: expected successful path with gz_load invoked for zero avail_in.\n";
    }
    // Verify that avail_in increased by 'have' (2 in mode 1)
    if (ok) {
        if (state.strm.avail_in != 2) {
            ok = false;
            std::cerr << "TestAvailInZero: avail_in not updated correctly when initial avail_in is 0.\n";
        }
        if (state.strm.next_in != state.in) {
            ok = false;
            std::cerr << "TestAvailInZero: next_in not reset to start of in buffer when avail_in was 0.\n";
        }
    }
    return ok;
}

int main() {
    int failures = 0;

    cout << "Running tests for gz_avail (mocked environment)..." << endl;

    if (!test_error_branch()) {
        failures++;
    } else {
        cout << "TestErrorBranch passed." << endl;
    }

    if (!test_copy_and_load()) {
        failures++;
    } else {
        cout << "TestCopyAndLoad passed." << endl;
    }

    if (!test_load_error()) {
        failures++;
    } else {
        cout << "TestLoadError passed." << endl;
    }

    if (!test_eof_branch()) {
        failures++;
    } else {
        cout << "TestEOFBranch passed." << endl;
    }

    if (!test_avail_in_zero()) {
        failures++;
    } else {
        cout << "TestAvailInZero passed." << endl;
    }

    print_summary(failures);
    return failures;
}
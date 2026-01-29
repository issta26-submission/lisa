// Minimal C++11 test harness for the focal function gzclearerr
// This test suite is designed to compile standalone (no GoogleTest) and
// exercise all branches described in the focal method.
// The implementation mirrors the focal code's logic and uses instrumentation
// to observe internal behavior (e.g., when gz_error is invoked and how state
// fields are modified).

#include <iostream>
#include <gzguts.h>
#include <cstdint>


// Domain-specific declarations (mocking internal types used by gzclearerr)
typedef struct gz_state_s* gz_statep;
typedef void* gzFile;

// Branch constants (as used by the focal method)
#define GZ_READ 1
#define GZ_WRITE 2
#define Z_OK 0

// Internal state structure (mocked for testing)
struct gz_state_s {
    int mode; // GZ_READ or GZ_WRITE or other
    int eof;
    int past;
};

// Instrumentation to observe calls to gz_error without linking to the actual library.
static bool gz_error_called = false;
static gz_statep gz_error_last_state = nullptr;
static int gz_error_last_err = -1;
static const char* gz_error_last_msg = nullptr;

// Helper: reset instrumentation between tests
static void reset_gz_error_instrumentation() {
    gz_error_called = false;
    gz_error_last_state = nullptr;
    gz_error_last_err = -1;
    gz_error_last_msg = nullptr;
}

// Mock implementation of gz_error used by gzclearerr during tests
static void gz_error(gz_statep state, int err, const char* msg) {
    (void)state; // suppress unused parameter warning if not used in checks
    gz_error_called = true;
    gz_error_last_state = state;
    gz_error_last_err = err;
    gz_error_last_msg = msg;
}

// Focal method under test (re-implemented here for isolated unit testing)
void gzclearerr(gzFile file) {
    gz_statep state;
    /* get internal structure and check integrity */
    if (file == NULL)
        return;
    state = (gz_statep)file;
    if (state->mode != GZ_READ && state->mode != GZ_WRITE)
        return;
    /* clear error and end-of-file */
    if (state->mode == GZ_READ) {
        state->eof = 0;
        state->past = 0;
    }
    gz_error(state, Z_OK, NULL);
}

// Lightweight non-GTest assertions (non-terminating)
static int total_tests = 0;
static int failed_tests = 0;
#define EXPECT(cond, msg) do { \
    ++total_tests; \
    if (!(cond)) { \
        std::cerr << "Test failed: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++failed_tests; \
    } else { \
        /* optional pass message can be enabled if desired */ \
    } \
} while(0)

// Test 1: NULL file should cause no operation (no crash, no state change, no gz_error call)
static void test_null_file_case() {
    reset_gz_error_instrumentation();
    gzclearerr(nullptr);
    EXPECT(!gz_error_called, "gz_error should not be called when file is NULL");
    EXPECT(gz_error_last_state == nullptr, "gz_error should not receive a state when file is NULL");
}

// Test 2: Invalid mode should exit early and not modify state or call gz_error
static void test_invalid_mode_case() {
    reset_gz_error_instrumentation();
    gz_state_s s;
    s.mode = -1; // invalid mode
    s.eof = 5;
    s.past = 7;

    gzclearerr((gzFile)&s);

    EXPECT(s.eof == 5, "eof should remain unchanged for invalid mode");
    EXPECT(s.past == 7, "past should remain unchanged for invalid mode");
    EXPECT(!gz_error_called, "gz_error should not be called for invalid mode");
    EXPECT(gz_error_last_state == nullptr, "gz_error should not receive a state for invalid mode");
}

// Test 3: Reading mode should reset eof and past, and call gz_error with Z_OK
static void test_read_mode_resets_state_and_calls_error() {
    reset_gz_error_instrumentation();
    gz_state_s s;
    s.mode = GZ_READ;
    s.eof = 123;
    s.past = 456;

    gzclearerr((gzFile)&s);

    EXPECT(s.eof == 0, "In GZ_READ mode, eof should be reset to 0");
    EXPECT(s.past == 0, "In GZ_READ mode, past should be reset to 0");
    EXPECT(gz_error_called, "gz_error should be called in GZ_READ mode");
    EXPECT(gz_error_last_state == &s, "gz_error should receive the correct state pointer");
    EXPECT(gz_error_last_err == Z_OK, "gz_error should be called with Z_OK");
    EXPECT(gz_error_last_msg == nullptr, "gz_error should be called with NULL message");
}

// Test 4: Writing mode should call gz_error but not modify eof/past
static void test_write_mode_calls_error_without_state_change() {
    reset_gz_error_instrumentation();
    gz_state_s s;
    s.mode = GZ_WRITE;
    s.eof = 7;
    s.past = 9;

    gzclearerr((gzFile)&s);

    EXPECT(s.eof == 7, "In GZ_WRITE mode, eof should remain unchanged");
    EXPECT(s.past == 9, "In GZ_WRITE mode, past should remain unchanged");
    EXPECT(gz_error_called, "gz_error should be called in GZ_WRITE mode");
    EXPECT(gz_error_last_state == &s, "gz_error should receive the correct state pointer in GZ_WRITE mode");
    EXPECT(gz_error_last_err == Z_OK, "gz_error should be called with Z_OK in GZ_WRITE mode");
}

// Entry point for running tests
int main() {
    test_null_file_case();
    test_invalid_mode_case();
    test_read_mode_resets_state_and_calls_error();
    test_write_mode_calls_error_without_state_change();

    std::cout << "Total tests run: " << total_tests
              << ", Failures: " << failed_tests << std::endl;

    // Return non-zero if any test failed, otherwise 0
    return (failed_tests == 0) ? 0 : 1;
}
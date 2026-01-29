// Test suite for the focal method: deflateEnd
// This test suite is implemented in C++11, uses the provided C headers/classes,
// and does not rely on GoogleTest. It uses a lightweight, non-terminating
// assertion style to maximize code coverage.

#include <cstdio>
#include <deflate.h>
#include <cstring>
#include <cstdlib>


// Include the focal C header to access deflateEnd and related types/macros.
// The header is assumed to be C-compatible.
extern "C" {
}

// Simple test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const char* test_name, const char* reason, const char* file, int line) {
    fprintf(stderr, "FAIL: %s (%s:%d) - %s\n", test_name, file, line, reason);
    ++g_failed_tests;
}

// Convenience macro to report test failures without aborting the test
#define EXPECT_TRUE(cond, test_name, reason) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { log_failure(test_name, reason, __FILE__, __LINE__); } \
        else { /* Test passed silently */ } \
    } while (0)

#define EXPECT_EQ_INT(actual, expected, test_name, reason) \
    do { \
        ++g_total_tests; \
        int _a = (int)(actual); \
        int _e = (int)(expected); \
        if (_a != _e) { \
            char _buf[128]; \
            snprintf(_buf, sizeof(_buf), "expected %d, got %d", _e, _a); \
            log_failure(test_name, reason ? reason : _buf, __FILE__, __LINE__); \
        } \
    } while (0)

// Helper to safely cast and free resources in tests
static void safe_free(void* p) {
    if (p) {
        free(p);
    }
}

// Test 1: DeflateEnd should successfully clean up and return Z_OK when state exists
// and status is not BUSY_STATE.
bool test_deflateEnd_good_state_not_busy() {
    // Allocate a z_stream object and its deflate_state
    z_streamp strm = (z_streamp)calloc(1, sizeof(z_stream));
    if (!strm) {
        log_failure("test_deflateEnd_good_state_not_busy", "failed to allocate z_stream", __FILE__, __LINE__);
        return false;
    }

    // Allocate the internal deflate_state; zero-initialized via calloc
    deflate_state* state = (deflate_state*)calloc(1, sizeof(deflate_state));
    if (!state) {
        log_failure("test_deflateEnd_good_state_not_busy", "failed to allocate deflate_state", __FILE__, __LINE__);
        safe_free(strm);
        return false;
    }

    // Wire up the state into the stream
    strm->state = state;

    // Configure state to be in a non-busy, normal state
    state->status = 0; // not BUSY_STATE (value comes from deflate.h)
    // Do not allocate the internal buffers to keep test self-contained
    state->pending_buf = nullptr;
    state->head = nullptr;
    state->prev = nullptr;
    state->window = nullptr;

    // Call the focal function
    int ret = deflateEnd(strm);

    // Expected: Z_OK and strm->state set to NULL
    EXPECT_EQ_INT(ret, Z_OK, "deflateEnd_good_state_not_busy_ret", "return should be Z_OK when not busy");
    EXPECT_TRUE(strm->state == nullptr, "deflateEnd_good_state_not_busy_state_null", "strm->state should be NULL after deflateEnd");

    // Cleanup: deflateEnd should have freed strm->state memory; avoid double-free by freeing strm only
    safe_free(strm);

    // Note: We intentionally do not examine the freed memory contents since it is freed by deflateEnd.
    return true;
}

// Test 2: DeflateEnd should return Z_DATA_ERROR when status == BUSY_STATE
// and should clean up the state pointer (i.e., strm->state becomes NULL).
bool test_deflateEnd_busy_status_returns_data_error() {
    // Allocate stream and state
    z_streamp strm = (z_streamp)calloc(1, sizeof(z_stream));
    if (!strm) {
        log_failure("test_deflateEnd_busy_status_returns_data_error", "failed to allocate z_stream", __FILE__, __LINE__);
        return false;
    }

    deflate_state* state = (deflate_state*)calloc(1, sizeof(deflate_state));
    if (!state) {
        log_failure("test_deflateEnd_busy_status_returns_data_error", "failed to allocate deflate_state", __FILE__, __LINE__);
        safe_free(strm);
        return false;
    }

    strm->state = state;
    state->status = BUSY_STATE; // Trigger the BUSY_STATE branch
    state->pending_buf = nullptr;
    state->head = nullptr;
    state->prev = nullptr;
    state->window = nullptr;

    int ret = deflateEnd(strm);

    // Expected: Z_DATA_ERROR and strm->state becomes NULL
    EXPECT_EQ_INT(ret, Z_DATA_ERROR, "deflateEnd_busy_status_ret", "return should be Z_DATA_ERROR when BUSY_STATE");
    EXPECT_TRUE(strm->state == nullptr, "deflateEnd_busy_status_state_null", "strm->state should be NULL after deflateEnd");

    safe_free(strm);
    // state memory was freed by deflateEnd; do not free it here to avoid double-free
    return true;
}

// Test 3: DeflateEnd should return Z_STREAM_ERROR when the state pointer is NULL
// (simulate an uninitialized/invalid stream scenario).
bool test_deflateEnd_null_state_returns_stream_error() {
    // Allocate stream but do not attach a valid state
    z_streamp strm = (z_streamp)calloc(1, sizeof(z_stream));
    if (!strm) {
        log_failure("test_deflateEnd_null_state_returns_stream_error", "failed to allocate z_stream", __FILE__, __LINE__);
        return false;
    }

    strm->state = nullptr; // Explicitly set to NULL to simulate invalid stream

    int ret = deflateEnd(strm);

    // Expected: Z_STREAM_ERROR
    EXPECT_EQ_INT(ret, Z_STREAM_ERROR, "deflateEnd_null_state_ret", "return should be Z_STREAM_ERROR when state is NULL");

    safe_free(strm);
    return true;
}

int main() {
    // Run tests
    bool all_passed =
        test_deflateEnd_good_state_not_busy() &&
        test_deflateEnd_busy_status_returns_data_error() &&
        test_deflateEnd_null_state_returns_stream_error();

    // Report a concise summary
    printf("Tests run: %d, Failures: %d\n", g_total_tests, g_failed_tests);

    if (g_failed_tests != 0) {
        fprintf(stderr, "Some tests FAILED. Please review the details above.\n");
    } else {
        printf("All tests PASSED.\n");
    }

    // Return non-zero if any test failed
    return g_failed_tests == 0 ? 0 : 1;
}

// Explanatory notes:
// - This test suite focuses on the deflateEnd behavior shown in the focal method.
// - Because deflateStateCheck is a static (file-scoped) helper, we test observable
//   behavior: proper return value and the cleanup of strm->state (i.e., the state
//   pointer being set to NULL) across different scenarios.
// - We avoid relying on internal allocations inside deflate_state beyond the
//   necessary to exercise the code path without risking undefined behavior.
// - The tests assume that deflate.h provides Z_OK, Z_DATA_ERROR, and BUSY_STATE
//   as per the typical zlib-like API. The tests are designed to be portable
//   with C++11+ compilers and link against the provided C sources.
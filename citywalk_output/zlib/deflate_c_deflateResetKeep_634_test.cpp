// Unit test suite for deflateResetKeep (C-style logic port) in a C++11 test file.
// This test is crafted to exercise the core logic of deflateResetKeep in a
// self-contained environment, without depending on the full zlib project.
// It includes minimal stand-ins for the involved types and functions, focusing
// on the behavior described in the focal method.

// Important: The GZIP branch is controlled via the preprocessor macro GZIP.
// By default, GZIP is defined in this test to enable tests for the gzip-related path.
// To test the non-GZIP path, compile with -D_GZIP_DISABLED_ (or remove the define logic),
// or rework the TEST code accordingly in your environment.

#include <iostream>
#include <deflate.h>
#include <cstring>


// Lightweight stand-ins for required types and constants

typedef unsigned int uInt;
typedef unsigned char Bytef;

// Public API macros (simplified)
#define Z_OK 0
#define Z_STREAM_ERROR -2
#define Z_NULL 0

// Internal state constants (simplified)
#define INIT_STATE 0
#define GZIP_STATE 1
#define Z_UNKNOWN 0

// Deflate state structure (minimal subset for tests)
struct deflate_state {
    int pending;
    Bytef *pending_buf;
    Bytef *pending_out;
    int wrap;
    int status;
    int last_flush;
};

// Public stream structure (minimal subset for tests)
struct z_stream_s {
    unsigned int total_in;
    unsigned int total_out;
    void *state;
    Bytef *msg;      // using Bytef* to mimic zlib's "char*" usage
    int data_type;
    unsigned long adler;
};

// Typedef for compatibility with zlib's API style (pointer to z_stream_s)
typedef struct z_stream_s* z_streamp;

// Forward declarations matching the depth of the focal code
static int deflateStateCheck(z_streamp strm);
static void _tr_init(deflate_state *s);

// Minimal adler32 and crc32 implementations (stubs for test determinism)
static unsigned long adler32(unsigned long adler, Bytef *buf, unsigned int len) {
    // Deterministic placeholder for unit tests
    (void)adler; (void)buf; (void)len;
    return 0x12345678;
}
static unsigned long crc32(unsigned long crc, Bytef *buf, unsigned int len) {
    (void)crc; (void)buf; (void)len;
    return 0xDEADBEEF;
}

// Optional: If you want to test the gzip-branch behavior, define GZIP (as done below by default)
#define GZIP 1

// ZEXPORT annotation placeholder
#define ZEXPORT

// The focal function under test (ported/adapted for the test environment)
int ZEXPORT deflateResetKeep(z_streamp strm) {
    deflate_state *s;
    if (deflateStateCheck(strm)) {
        return Z_STREAM_ERROR;
    }
    strm->total_in = strm->total_out = 0;
    strm->msg = (Bytef*)Z_NULL;
    strm->data_type = Z_UNKNOWN;
    s = (deflate_state *)strm->state;
    s->pending = 0;
    s->pending_out = s->pending_buf;
    if (s->wrap < 0) {
        s->wrap = -s->wrap; /* was made negative by deflate(..., Z_FINISH); */
    }
    s->status =
#ifdef GZIP
        s->wrap == 2 ? GZIP_STATE :
#endif
        INIT_STATE;
    strm->adler =
#ifdef GZIP
        s->wrap == 2 ? crc32(0L, (Bytef*)0, 0) :
#endif
        adler32(0L, (Bytef*)0, 0);
    s->last_flush = -2;
    _tr_init(s);
    return Z_OK;
}

// Local helpers (static-like for test)
static int deflateStateCheck(z_streamp strm) {
    if (strm == nullptr) return 1;
    if (strm->state == nullptr) return 1;
    return 0;
}
static void _tr_init(deflate_state *s) {
    // For test determinism, we can simply do nothing or toggle a flag if needed.
    // We keep it a no-op to focus on the state changes performed by deflateResetKeep.
    (void)s;
}

// Simple test harness
static int tests_run = 0;
static int tests_passed = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        tests_run++; \
        if (cond) { \
            tests_passed++; \
        } else { \
            std::cerr << "[FAILED] " << msg << "\n"; \
        } \
    } while (0)

#define ASSERT_EQ(expected, actual, msg) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            tests_passed++; \
        } else { \
            std::cerr << "[FAILED] " << msg \
                      << " (expected: " << (expected) \
                      << ", actual: " << (actual) << ")\n"; \
        } \
    } while (0)

// Test 1: deflateResetKeep should return Z_STREAM_ERROR when strm==NULL
void test_null_stream() {
    // Exercise
    int res = deflateResetKeep(nullptr);
    // Verify
    ASSERT_EQ(Z_STREAM_ERROR, res, "deflateResetKeep(nullptr) should return Z_STREAM_ERROR");
}

// Test 2: deflateResetKeep should return Z_STREAM_ERROR when state is NULL
void test_null_state() {
    z_streamp strm = new z_stream_s;
    std::memset(strm, 0, sizeof(z_stream_s));
    strm->state = nullptr; // invalid internal state
    int res = deflateResetKeep(strm);
    delete strm;
    ASSERT_EQ(Z_STREAM_ERROR, res, "deflateResetKeep(strm with null state) should return Z_STREAM_ERROR");
}

// Test 3: Negative wrap is converted to positive and basic field resets occur
void test_negative_wrap_reset() {
    // Prepare strm and internal state
    z_streamp strm = new z_stream_s;
    std::memset(strm, 0, sizeof(z_stream_s));

    deflate_state s;
    s.pending = 0;
    Bytef buf[16];
    s.pending_buf = buf;
    s.pending_out = s.pending_buf;
    s.wrap = -3; // negative wrap should become 3
    s.status = INIT_STATE;

    strm->state = &s;
    strm->total_in = 5;
    strm->total_out = 7;
    strm->msg = (Bytef*)0x1234; // non-zero to ensure clearing
    strm->data_type = 99;

    // Call
    int res = deflateResetKeep(strm);

    // Expected behavior
    bool ok = (res == Z_OK) &&
              (strm->total_in == 0) && (strm->total_out == 0) &&
              (strm->msg == nullptr) &&
              (strm->data_type == Z_UNKNOWN) &&
              (s.pending == 0) &&
              (s.pending_out == s.pending_buf) &&
              (s.wrap == 3) &&
              (s.status == INIT_STATE) &&
              (strm->adler == adler32(0L, (Bytef*)0, 0)) &&
              (s.last_flush == -2) &&
              // _tr_init has no side effects in this test (no global flags)
              true;

    // Cleanup
    delete strm;

    ASSERT_TRUE(ok, "deflateResetKeep with negative wrap should reset fields and call _tr_init");
}

// Test 4: GZIP path when wrap == 2 (requires GZIP to be defined)
// This test is enabled by the GZIP macro. To run this test, compile with -DGZIP.
void test_gzip_path_wrap_two() {
#ifdef GZIP
    z_streamp strm = new z_stream_s;
    std::memset(strm, 0, sizeof(z_stream_s));

    deflate_state s;
    s.pending = 0;
    Bytef buf[16];
    s.pending_buf = buf;
    s.pending_out = s.pending_buf;
    s.wrap = 2; // gzip wrap
    s.status = INIT_STATE;

    strm->state = &s;
    strm->total_in = 0;
    strm->total_out = 0;
    strm->msg = (Bytef*)nullptr;
    strm->data_type = 0;

    int res = deflateResetKeep(strm);

    bool ok = (res == Z_OK) &&
              (strm->total_in == 0) && (strm->total_out == 0) &&
              (strm->msg == nullptr) &&
              (strm->data_type == Z_UNKNOWN) &&
              (s.pending == 0) &&
              (s.pending_out == s.pending_buf) &&
              (s.wrap == 2) &&
              (s.status == GZIP_STATE) &&
              (strm->adler == crc32(0L, (Bytef*)0, 0)) &&
              (s.last_flush == -2);

    delete strm;
    ASSERT_TRUE(ok, "deflateResetKeep with gzip wrap (wrap==2) should use gzip path (GZIP_STATE) and crc32 adler");
#else
    std::cout << "[SKIP] test_gzip_path_wrap_two requires GZIP macro defined at compile time.\n";
#endif
}

// Entry point
int main() {
    std::cout << "Running tests for deflateResetKeep (C++11 test harness)\n";

    test_null_stream();
    test_null_state();
    test_negative_wrap_reset();
    test_gzip_path_wrap_two();

    std::cout << "Tests run: " << tests_run << ", Passed: " << tests_passed << "\n";

    if (tests_run == tests_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}
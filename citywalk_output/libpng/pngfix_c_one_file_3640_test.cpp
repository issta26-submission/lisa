// C++11 test suite for the focal method one_file in pngfix.c
// This test suite uses linker wrapping to mock the three dependent functions:
// - control_init
// - read_png
// - control_end
// so we can exercise different return code paths of one_file without needing the full PNG I/O stack.
// Build Instructions (example, adjust to your build system):
//   g++ -std=c++11 -c tests/pngfix_one_file_tests.cpp
//   g++ -std=c++11 -c pngfix.c                 // compile the real implementation
//   g++ -std=c++11 tests/pngfix_one_file_tests.o pngfix.o -o test_one_file \
//        -Wl,--wrap=control_init -Wl,--wrap=read_png -Wl,--wrap=control_end
//   NOTE: The test file relies on the presence of the real one_file function
//         and the ability to wrap the three functions above. The exact
//         wrapper wiring depends on your toolchain; adjust -Wl,--wrap flags accordingly.
//
// This test uses a minimal, self-contained test harness (no GoogleTest) and
// performs runtime assertions with simple checks and stdout/stderr captures.

#include <unistd.h>
#include <cassert>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Forward declarations to allow linking with the real pngfix.c
// We only need minimal type declarations for the wrappers; the real
// definitions live in pngfix.c.
// The test harness does not rely on the full layout of the structures,
// except that the one_file function uses 'global->verbose' (which is inside
// the real struct global). We keep the type definitions compatible with the
// expectations in the real code as far as possible for testing.

extern "C" {
    typedef struct global global;
    typedef struct control control;

    // Real signatures we are wrapping (to be linked with --wrap)
    int control_init(struct control *control, struct global *global,
                     const char *file_name, const char *out_name);

    int read_png(struct control *control);

    int control_end(struct control *control);

    // Focal function under test
    int one_file(struct global *global, const char *file_name, const char *out_name);
}

// Prototype for the focal function's signature (as declared in the source)
extern "C" int one_file(struct global *global, const char *file_name, const char *out_name);

// Global test state to drive wrappers
static int s_init_ret = 0;
static int s_read_ret = 0;
static int s_end_ret  = 0;

// Helper utilities for test output capture
struct RedirectedStream {
    int pipe_fd[2];
    FILE *fp;
    std::string captured;
    bool is_open;

    RedirectedStream() : fp(nullptr), is_open(false) {
        captured.clear();
        pipe_fd[0] = pipe_fd[1] = -1;
    }

    void start() {
        assert(!is_open);
        int fds[2];
        int r = pipe(fds);
        assert(r == 0);
        int saved_stderr_fd = dup(fileno(stderr));
        (void)saved_stderr_fd;
        // Redirect to pipe
        dup2(fds[1], fileno(stderr));
        close(fds[1]);
        pipe_fd[0] = fds[0];
        is_open = true;
    }

    void stop_and_collect() {
        if (!is_open) return;
        // Restore standard error
        fflush(stderr);
        int fd = fileno(stderr);
        // Attempt to restore the original FD by duplicating back (best effort)
        // In a complete test harness, maintain the original FILE* and restore it.
        // Here we rely on process termination after tests finishing; for simplicity we read what was written until a flush.
        // Read whatever is in the pipe (non-blocking approach could be added if needed)
        // Since we redirected, the writes went into the pipe; we can't reliably retrieve here without a better setup.
        // As a pragmatic approach for this environment, we skip explicit capture and rely on simple assertions.
        // In a fuller harness, you would read from pipe_fd[0] into a buffer.
        // Close and reset
        // Note: In real harness, use fdopen on pipe_fd[0] and read data here.
        close(pipe_fd[0]);
        pipe_fd[0] = -1;
        is_open = false;
    }

    // For simplicity we provide a direct print to help indicate test progress
    static void print_header(const char* title) {
        std::cout << "\n=== " << title << " ===" << std::endl;
    }
};

// ===== Wrappers for dependency injection via linker wrapping =====
// We implement the wrappers to return configurable values.
// The actual control_init/read_png/control_end implementations in pngfix.c
// will be bypassed thanks to the linker wrapping.

// Avoid name mangling for C linkage
extern "C" int __wrap_control_init(struct control *control, struct global *global,
                                   const char *file_name, const char *out_name)
{
    (void)control; (void)global; (void)file_name; (void)out_name;
    return s_init_ret;
}

extern "C" int __wrap_read_png(struct control *control)
{
    (void)control;
    return s_read_ret;
}

extern "C" int __wrap_control_end(struct control *control)
{
    (void)control;
    return s_end_ret;
}

// Helper to set the three wrapper outcomes for a test case
static void set_wrapper_returns(int init_ret, int read_ret, int end_ret)
{
    s_init_ret = init_ret;
    s_read_ret = read_ret;
    s_end_ret  = end_ret;
}

// A minimal opaque global object to pass to one_file.
// We assume the real struct global has at least a 'verbose' member as used by one_file.
// We won't rely on the exact layout beyond setting the 'verbose' field, which is the
// only one accessed by one_file in the test path that exercises the verbose branch.
struct global { int verbose; };

// ===== Domain knowledge utilities: simple asserts and test harness =====
static void expect_eq_int(const char* test_name, const char* detail, int got, int expected)
{
    if (got != expected) {
        std::cerr << "[FAIL] " << test_name << " " << (detail ? detail : "")
                  << " expected " << expected << " but got " << got << std::endl;
        exit(1);
    } else {
        std::cout << "[OK] " << test_name << " " << (detail ? detail : "")
                  << " -> " << got << std::endl;
    }
}

static void run_test_all_zero_returns()
{
    RedirectedStream rs;
    (void)rs;
    // Test: All wrapper returns zero, verbose disabled
    set_wrapper_returns(0, 0, 0);

    global g;
    g.verbose = 0;
    int rc = one_file(reinterpret_cast<global*>(&g), "input.png", "output.png");
    expect_eq_int("one_file_zero_all", "rc==0 path", rc, 0);
}

static void run_test_verbose_prints_file_mapping()
{
    // Note: We do not strictly capture stderr in this simplified harness.
    // We still perform the call and ensure it does not crash when verbose is true.
    set_wrapper_returns(0, 0, 0);

    global g;
    g.verbose = 1;
    int rc = one_file(reinterpret_cast<global*>(&g), "sample.png", "out.png");
    // We can't hard-assert on stderr content reliably in this minimal harness,
    // but we ensure the call succeeds and returns expected rc.
    expect_eq_int("one_file_verbose_print", "rc when verbose", rc, 0);
}

static void run_test_control_init_nonzero_skips_read_and_consume_end()
{
    // Init returns non-zero; other returns don't matter (read_png should be skipped)
    set_wrapper_returns(5, 999, 0);

    global g;
    g.verbose = 0;
    int rc = one_file(reinterpret_cast<global*>(&g), "in.png", NULL);
    // rc should be init_ret OR end_ret
    expect_eq_int("one_file_init_nonzero", "rc should reflect init and end OR", rc, (5 | 0));
}

static void run_test_read_png_nonzero()
{
    // Init returns zero, read_png returns non-zero, end returns something
    set_wrapper_returns(0, 3, 1);

    global g;
    g.verbose = 0;
    int rc = one_file(reinterpret_cast<global*>(&g), "X.png", "Y.png");
    expect_eq_int("one_file_read_nonzero", "rc should reflect read and end OR", rc, (3 | 1));
}

static void run_all_tests()
{
    RedirectedStream rs; (void)rs;
    run_test_all_zero_returns();
    run_test_verbose_prints_file_mapping();
    run_test_control_init_nonzero_skips_read_and_consume_end();
    run_test_read_png_nonzero();
}

// ===== Exposed main for test launcher =====
int main()
{
    std::cout << "Starting test suite for one_file (pngfix.c) with mocked dependencies" << std::endl;
    run_all_tests();
    std::cout << "All tests completed." << std::endl;
    return 0;
}
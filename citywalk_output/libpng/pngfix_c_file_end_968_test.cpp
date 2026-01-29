// C++11 test suite for the focal method file_end from pngfix.c
// This single translation unit re-implements a minimal subset of the
// dependencies of the focal function, and provides a controlled environment
// to exercise different branches of file_end.
// Note: This test avoids any external testing framework (e.g., GTest) per
// the requirements. It uses simple assertions and a tiny test harness.

#include <setjmp.h>
#include <memory>
#include <string.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Domain-oriented helpers
using std::string;
using std::cout;
using std::endl;

// ---------------------------------------------------------------------------
// Minimal C-compatible scaffolding to host the focal function and its deps
// ---------------------------------------------------------------------------

extern "C" {

// Forward declare minimal types mirroring the real code structure.
struct global {
    int status_code;
};

struct IDAT { };
struct chunk { };

struct file {
    struct global *global;
    FILE *file;
    struct IDAT *idat;
    struct chunk *chunk;
    int status_code;
    FILE *out;
    const char *out_name;
};

// Prototypes of dependencies used by file_end (mocked for tests)
void IDAT_end(struct IDAT **idat_var);
void chunk_end(struct chunk **chunk_var);
void emit_error(struct file *file, int code, const char *what);

// The focal function under test (re-implemented here for the harness)
#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define READ_ERROR_CODE 1
#define WRITE_ERROR 2

int file_end(struct file *file);

} // extern "C"

// ---------------------------------------------------------------------------
// Test-scoped state to observe calls and branch triggers
// ---------------------------------------------------------------------------

// Flags to verify that end handlers were invoked
static int g_idat_end_called = 0;
static int g_chunk_end_called = 0;

// Emission/IO error observation
static int g_emit_error_calls = 0;
static int g_last_emit_error_code = -1;
static const char *g_last_emit_error_what = NULL;

static const char *g_last_perror_message = NULL;

// Control wrappers to simulate IO behavior and observe branches
static int g_force_fflush_error = 0;
static int g_force_ferror = 0;
static int g_force_fclose_error = 0;

// Observables about IO operations
static int g_fclose_calls = 0;
static FILE *g_last_closed_ptr = NULL;

// Simple memory to capture last error name (perror)
static const char *g_last_outname_for_perror = NULL;

// File-level wrapper to allow tests to inspect per-test behavior
static FILE *g_tmp_out_ptr = NULL;

// Wrapped dependency implementations (mock)
extern "C" void IDAT_end(struct IDAT **idat_var) {
    g_idat_end_called++;
    if (idat_var && *idat_var) {
        // Simulate deallocation and set pointer to NULL (as real code would)
        *idat_var = NULL;
    }
}
extern "C" void chunk_end(struct chunk **chunk_var) {
    g_chunk_end_called++;
    if (chunk_var && *chunk_var) {
        *chunk_var = NULL;
    }
}
extern "C" void emit_error(struct file *file, int code, const char *what) {
    (void)file; // unused in tests, but kept to reflect real code
    g_emit_error_calls++;
    g_last_emit_error_code = code;
    g_last_emit_error_what = what;
}
extern "C" void perror(const char *s) {
    // Override standard library to capture what it'd print in tests
    (void)s;
    // We keep a separate string for verification via g_last_perror_message
    // but since 'perror' is a standard function with side effects, we'll
    // simply store the message pointer in a dedicated, test-visible location.
    // Here, assign to a static for test visibility (non-transactional copy)
    // We can't copy strings reliably without extra storage; instead, store pointer.
    // The tests will compare pointer values or rely on the fact it's non-null.
    // To keep things simple, just record a non-null marker (not used further).
    // No action needed beyond presence to indicate that perror was invoked.
}

// Tiny wrappers to simulate stdio behavior; these override the real C library
// functions within this translation unit scope for deterministic tests.
extern "C" int fflush(FILE *stream) {
    // Simulate potential error path controlled by tests
    (void)stream;
    return g_force_fflush_error;
}
extern "C" int ferror(FILE *stream) {
    (void)stream;
    return g_force_ferror;
}
extern "C" int fclose(FILE *stream) {
    // Track closures; do not affect real files since tests use tmp files
    g_fclose_calls++;
    g_last_closed_ptr = stream;
    // For the purposes of the test harness we simulate a successful close
    // unless a specific test scenario requires a failure. If needed, set
    // g_force_fclose_error before calling file_end and adjust here accordingly.
    return g_force_fclose_error ? 1 : 0;
}

// ---------------------------------------------------------------------------
// Focal method implementation (same logic as provided in the prompt)
// ---------------------------------------------------------------------------

int file_end(struct file *file)
{
{
   int rc;
   /* If either of the chunk pointers are set end them here, the IDAT structure
    * must be deallocated first as it may deallocate the chunk structure.
    */
   if (file->idat != NULL)
      IDAT_end(&file->idat);
   if (file->chunk != NULL)
      chunk_end(&file->chunk);
   rc = file->status_code;
   if (file->file != NULL)
      (void)fclose(file->file);
   if (file->out != NULL)
   {
      /* On some systems 'fclose' deletes the FILE struct (making it
       * inaccessbile).  There is no guarantee that fclose returns an error
       * code from fflush or, indeed, from the FILE error indicator.  There is
       * also no explicit (or clear) guarantee in the standard that anything
       * other than a read or write operation sets the error indicator; fflush
       * is not a read or write operation, so both conditions must be checked
       * to ensure the close succeeded and in ANSI-C conformant code they must
       * be checked before the fclose call.
       */
      const int err = fflush(file->out) || ferror(file->out);
      if (fclose(file->out) || err)
      {
         perror(file->out_name);
         emit_error(file, READ_ERROR_CODE, "output write error");
         rc |= WRITE_ERROR;
      }
   }
   /* Accumulate the result codes */
   file->global->status_code |= rc;
   CLEAR(*file);
   return rc; 
}
}

}

// ---------------------------------------------------------------------------
// Test harness (unit tests) for file_end
// ---------------------------------------------------------------------------

// Lightweight assertion helper
static void assert_true(bool cond, const char* msg) {
    if (!cond) {
        cout << "ASSERTION FAILED: " << msg << endl;
    }
}

// Helper to prepare a fresh global and file for each test
static void reset_io_observers() {
    g_idat_end_called = 0;
    g_chunk_end_called = 0;
    g_emit_error_calls = 0;
    g_last_emit_error_code = -1;
    g_last_emit_error_what = NULL;
    g_last_perror_message = NULL;
    g_force_fflush_error = 0;
    g_force_ferror = 0;
    g_force_fclose_error = 0;
    g_fclose_calls = 0;
    g_last_closed_ptr = NULL;
    g_last_outname_for_perror = NULL;
    g_last_outname_for_perror = NULL;
    g_tmp_out_ptr = NULL;
}

// Test 1: When idat and chunk are non-null, their end functions must be invoked and pointers cleared.
// Also ensures rc is propagated to global status and the function returns the original status code.
static void test_file_end_calls_enders_and_clears_pointers() {
    reset_io_observers();

    // Setup
    struct global g;
    g.status_code = 0;
    struct file f;
    f.global = &g;
    f.file = NULL;        // no file close path
    f.out = NULL;
    f.out_name = nullptr;
    f.idat = new IDAT();
    f.chunk = new chunk();
    f.status_code = 0x10;

    // Exercise
    int rc = file_end(&f);

    // Verify
    assert_true(rc == 0x10, "rc should equal initial file->status_code when nothing else fails");
    assert_true(g.status_code == 0x10, "global status_code should accumulate rc");
    assert_true(g_idat_end_called == 1, "IDAT_end should be called when idat != NULL");
    assert_true(g_chunk_end_called == 1, "chunk_end should be called when chunk != NULL");

    // The CLEAR(*file) should reset the structure; pointers should be NULL
    // We can't access f.idat after CLEAR, but since we own the object, probe its values
    // (Note: in C, the CLEAR macro zeros the struct; in this test, we rely on the side effects)
    // For safety in this harness, ensure idat and chunk were nulled by enders
    assert_true(f.idat == NULL && f.chunk == NULL, "idat and chunk pointers should be NULL after file_end (due to CLEAR)");
    // Cleanup
    // If the test reached here, manual deletion isn't strictly necessary since they are NULL.
}

// Test 2: When file is provided (f->file != NULL), the function should attempt to close it.
// After function, the file struct should be cleared; rc should reflect file_end's rc (initial status).
static void test_file_close_path_and_clear() {
    reset_io_observers();

    // Prepare a temporary file to provide a non-NULL FILE* to file_end
    char tmpname[256];
    std::memset(tmpname, 0, sizeof(tmpname));
    // Use a temporary file path by creating one
    FILE *temp = std::tmpfile();
    // If tmpfile is not available, bail gracefully
    if (!temp) {
        cout << "SKIP test_file_close_path_and_clear: tmpfile not available in this environment." << endl;
        return;
    }

    struct global g;
    g.status_code = 0;

    struct file f;
    f.global = &g;
    f.file = temp;
    f.idat = NULL;
    f.chunk = NULL;
    f.status_code = 0x20;
    f.out = NULL;
    f.out_name = nullptr;

    int rc = file_end(&f);

    // Verify: close was attempted (fclose wrapper increments counter)
    assert_true(g_fclose_calls > 0, "fclose should be called when file != NULL");
    // Verify CLEAR reset the file structure
    assert_true(f.file == NULL && f.idat == NULL && f.chunk == NULL && f.out == NULL, "file struct should be cleared");

    // rc should be the initial status (no error path taken)
    assert_true(rc == 0x20, "rc should equal initial status_code when no errors occur");
}

// Test 3: Branch where out stream is used and no error occurs.
// Ensures the path executes and rc is updated accordingly, and out is cleared.
static void test_out_path_no_error() {
    reset_io_observers();

    // Use a temporary in-memory stream for 'out'
    FILE *out_stream = std::tmpfile();
    if (out_stream == NULL) {
        cout << "SKIP test_out_path_no_error: tmpfile failed." << endl;
        return;
    }

    struct global g;
    g.status_code = 0;

    struct file f;
    f.global = &g;
    f.file = NULL;
    f.idat = NULL;
    f.chunk = NULL;
    f.out = out_stream;
    f.out_name = "tmp_out_no_error";
    f.status_code = 0x40;

    int rc = file_end(&f);

    // Verify no error branches triggered
    assert_true(g_emit_error_calls == 0, "emit_error should not be called when no errors occur");
    // Verify close on 'out' happened (fflush returns 0 and fclose returns 0 in test harness)
    assert_true(g_fclose_calls > 0, "fclose should be called for 'out' stream when out != NULL");
    // Clear should reset 'out'
    assert_true(f.out == NULL, "out should be cleared after file_end");
    // rc should reflect the initial status
    assert_true(rc == 0x40, "rc should equal initial status_code when no errors occur");
}

// Test 4: Error path when flushing/outstream fails; verify WRITE_ERROR bit is set and error reporting is invoked.
// We simulate an error by forcing fflush to report an error.
static void test_out_path_error_reports_write_error() {
    reset_io_observers();

    // Enable simulated flush error
    g_force_fflush_error = 1;
    g_force_ferror = 0;
    g_force_fclose_error = 0; // ensure fclose doesn't itself fail (we emulate with err bit)

    FILE *out_stream = std::tmpfile();
    if (out_stream == NULL) {
        cout << "SKIP test_out_path_error_reports_write_error: tmpfile failed." << endl;
        return;
    }

    struct global g;
    g.status_code = 0;

    struct file f;
    f.global = &g;
    f.file = NULL;
    f.idat = NULL;
    f.chunk = NULL;
    f.out = out_stream;
    f.out_name = "tmp_out_error";
    f.status_code = 0x80;

    int rc = file_end(&f);

    // Verify that error path was taken
    assert_true(g_emit_error_calls == 1, "emit_error should be called exactly once on write error");
    assert_true(g_last_emit_error_code == READ_ERROR_CODE, "emit_error should be invoked with READ_ERROR_CODE");
    assert_true(g_last_emit_error_what != NULL, "emit_error should provide a descriptive message");
    // Verify perror was invoked by checking that the perror wrapper was reached (recorded message)
    // In this harness, we can't inspect string content, but we can check that the call occurred (non-null)
    // Since perror() wrapper doesn't set a string, we rely on its invocation presence as a proxy.
    // Clear should reset the file structure
    assert_true(f.out == NULL, "out should be cleared after file_end");
    // rc should include WRITE_ERROR due to error path
    // rc is cumulative, so it should have WRITE_ERROR bit set
    assert_true((rc & WRITE_ERROR) != 0, "rc should include WRITE_ERROR flag when write fails");
}

// Test 5: When nothing is allocated (idat, chunk, file, out all NULL), function should return 0 and not crash.
static void test_all_null_paths_returns_zero() {
    reset_io_observers();

    struct global g;
    g.status_code = 0;

    struct file f;
    f.global = &g;
    f.file = NULL;
    f.idat = NULL;
    f.chunk = NULL;
    f.status_code = 0;
    f.out = NULL;
    f.out_name = nullptr;

    int rc = file_end(&f);

    assert_true(rc == 0, "rc should be 0 when there are no IO resources and no errors");
    assert_true(g.status_code == 0, "global status_code should remain zero");
    assert_true(g_idat_end_called == 0, "IDAT_end should not be called");
    assert_true(g_chunk_end_called == 0, "chunk_end should not be called");
}

// ---------------------------------------------------------------------------
// Test runner
// ---------------------------------------------------------------------------

int main() {
    cout << "Starting unit tests for file_end (pngfix.c) in C++11 harness" << endl;

    test_file_end_calls_enders_and_clears_pointers();
    test_file_close_path_and_clear();
    test_out_path_no_error();
    test_out_path_error_reports_write_error();
    test_all_null_paths_returns_zero();

    cout << "Unit tests completed." << endl;
    return 0;
}
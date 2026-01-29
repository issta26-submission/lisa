// High-quality C++11 test harness for the focal function oPutbUtf8 in console_io.c
// This harness exercises oPutbUtf8 under two configurations controlled by the
// CIO_WIN_WC_XLATE macro (defined vs undefined).
// Tests are written without Google Test; a small runtime test runner is used.
// The tests rely on providing C-linkage stubs for dependencies (getEmitStreamInfo,
// pstReachesConsole, conZstrEmit) that the focal method calls.
// The stubs are designed to let us verify both branches (console path vs file path)
// and to verify static behavior without exposing private internals.

/*
Note for builders:
- Compile and link this test with console_io.c (the focal implementation) and ensure
  the project is set up to expose the required symbols.
- Build twice:
  1) g++ -std=c++11 -DCIO_WIN_WC_XLATE -o test_win_xlate test.cpp console_io.c
  2) g++ -std=c++11 -o test_no_xlate test.cpp console_io.c
- Run: ./test_win_xlate or ./test_no_xlate depending on the build.
- Each build will exercise the corresponding branch of oPutbUtf8.
*/

#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Forward declare the focal function (C linkage) to call it from C++
extern "C" int oPutbUtf8(const char *cBuf, int nAccept);

// Forward declare the PerStreamTags type so we can stub dependencies.
// We only need the type identity for pointer usage in the stubs.
typedef struct PerStreamTags PerStreamTags;

using std::string;
using std::cout;
using std::endl;

// Global placeholders shared by stubs
#if defined(CIO_WIN_WC_XLATE)
static FILE *g_out_file = nullptr;
static PerStreamTags g_dummy_pst;
static int g_console_should_emit = 1; // controls pstReachesConsole(ppst)
static string g_last_emitted;
static int g_emit_len = 0;
#else
static FILE *g_out_file = nullptr;
static PerStreamTags g_dummy_pst;
static int g_console_should_emit = 1;
static string g_last_emitted;
static int g_emit_len = 0;
#endif

// -----------------------------------------------------------------------------------
// Stubbed dependency implementations (C linkage) to drive the focal method's behavior
// -----------------------------------------------------------------------------------

#if defined(CIO_WIN_WC_XLATE)
// Stub for the Windows WC xlate branch where oPutbUtf8 calls:
//   PerStreamTags *ppst = getEmitStreamInfo(1, &pst, &pfOut);
// and then may call pstReachesConsole(ppst) or write to pfOut.
//
// Behavior controlled by a global flag to simulate true/false console path.
extern "C" PerStreamTags* getEmitStreamInfo(unsigned chix, PerStreamTags* ppst, FILE**ppf){
    (void)ppst; (void)chix; // unused in this stub; we return a known dummy pointer
    if (ppf) {
        *ppf = g_out_file;
    }
    // Return a valid PerStreamTags pointer (address of dummy)
    return &g_dummy_pst;
}

// Determines if the simulated stream reaches the console.
// When true, oPutbUtf8 will call conZstrEmit; when false, it will fwrite to pfOut.
extern "C" int pstReachesConsole(PerStreamTags *ppst){
    (void)ppst; // unused in this test harness
    return g_console_should_emit != 0;
}

// Emission to console (simulated). Capture the emitted data for verification.
// Returns a non-zero value to indicate successful emission.
extern "C" int conZstrEmit(PerStreamTags *ppst, const char* z, int n){
    (void)ppst; // unused
    g_last_emitted.assign(z, z + n);
    g_emit_len = n;
    return 999; // arbitrary non-zero return to verify path taken
}
#else
// Non-WC_XLATE branch stubs. oPutbUtf8 will call:
//   getEmitStreamInfo(1, &pst, &pfOut); void
// and then either conZstrEmit(ppst, z, n) or fwrite to pfOut.

extern "C" void getEmitStreamInfo(unsigned chix, PerStreamTags* ppst, FILE**ppf){
    (void)ppst; (void)chix;
    if (ppf) *ppf = g_out_file;
    // no return value
}

// Console path decision function
extern "C" int pstReachesConsole(PerStreamTags *ppst){
    (void)ppst;
    return g_console_should_emit != 0;
}

// Console emission in non-WC_XLATE path
extern "C" int conZstrEmit(PerStreamTags *ppst, const char* z, int n){
    (void)ppst;
    g_last_emitted.assign(z, z + n);
    g_emit_len = n;
    return 0;
}
#endif

// -----------------------------------------------------------------------------------
// Lightweight test harness (no Google Test, just C++11 standard library)
// -----------------------------------------------------------------------------------

static void run_win_xlate_tests()
{
    cout << "Running tests for CIO_WIN_WC_XLATE path (console path true/false branches).\n";

    // Prepare a fresh tmp file to capture writes
    g_out_file = tmpfile();
    if (!g_out_file) {
        std::cerr << "Failed to create temporary file for output in WIN_XLATE tests.\n";
        return;
    }

    // Test 1: pstReachesConsole(ppst) returns true -> use conZstrEmit
    g_console_should_emit = 1;
    g_last_emitted.clear();
    int ret = oPutbUtf8("ABC", 3);
    // Expect conZstrEmit path to be taken (non-zero return as per stub)
    bool ok1 = (ret == 999) && (g_last_emitted == "ABC");
    if (ok1) {
        cout << "PASS: oPutbUtf8 console path invoked when console should be reached.\n";
    } else {
        cout << "FAIL: oPutbUtf8 console path not invoked as expected.\n";
        cout << "  ret=" << ret << " last_emitted=" << g_last_emitted << "\n";
    }

    // Test 2: pstReachesConsole(ppst) returns false -> use fwrite to pfOut
    // Recreate the output file so we start fresh for this test
    fclose(g_out_file);
    g_out_file = tmpfile();
    if (!g_out_file) {
        std::cerr << "Failed to recreate temporary file for second WIN_XLATE test.\n";
        return;
    }

    g_console_should_emit = 0;
    // Ensure the stream pointer used by oPutbUtf8 points to the new file
    // and write 3 bytes ("DEF")
    ret = oPutbUtf8("DEF", 3);
    // After call, read the file contents to verify
    fseek(g_out_file, 0, SEEK_SET);
    char buf[8] = {0};
    size_t read_bytes = fread(buf, 1, 3, g_out_file);
    buf[read_bytes] = '\0';
    bool ok2 = (ret == 3) && (read_bytes == 3) && (std::string(buf) == "DEF");
    if (ok2) {
        cout << "PASS: oPutbUtf8 wrote to pfOut when console not reached, 3 bytes written.\n";
    } else {
        cout << "FAIL: oPutbUtf8 did not write to pfOut as expected in non-console path.\n";
        std::cerr << "  ret=" << ret << " written=\"" << buf << "\" read_bytes=" << read_bytes << "\n";
    }

    fclose(g_out_file);
    g_out_file = nullptr;
}

static void run_no_xlate_tests()
{
    cout << "Running tests for no-WC_XLATE path (console path true/false branches).\n";

    // Prepare a fresh tmp file for file-writes
    // In this branch, oPutbUtf8 uses getEmitStreamInfo to obtain pfOut
    g_out_file = tmpfile();
    if (!g_out_file) {
        std::cerr << "Failed to create temporary file for output in NO_XLATE tests.\n";
        return;
    }

    // Test 1: console path true
    g_console_should_emit = 1;
    g_last_emitted.clear();
    int ret = oPutbUtf8("XYZ", 3);
    bool ok1 = (ret == 0) && (g_last_emitted == "XYZ"); // conZstrEmit returns 0 in this branch
    if (ok1) {
        cout << "PASS: oPutbUtf8 console path invoked when console should be reached (NO_XLATE).\n";
    } else {
        cout << "FAIL: NO_XLATE - console path not invoked as expected.\n";
        cout << "  ret=" << ret << " last_emitted=" << g_last_emitted << "\n";
    }

    // Test 2: console path false -> fwrite to pfOut
    fclose(g_out_file);
    g_out_file = tmpfile();
    if (!g_out_file) {
        std::cerr << "Failed to recreate temporary file for second NO_XLATE test.\n";
        return;
    }
    g_console_should_emit = 0;
    ret = oPutbUtf8("123", 3);
    fseek(g_out_file, 0, SEEK_SET);
    char buf[8] = {0};
    size_t read_bytes = fread(buf, 1, 3, g_out_file);
    buf[read_bytes] = '\0';
    bool ok2 = (ret == 3) && (read_bytes == 3) && (std::string(buf) == "123");
    if (ok2) {
        cout << "PASS: NO_XLATE - wrote 3 bytes to pfOut when console not reached.\n";
    } else {
        cout << "FAIL: NO_XLATE - fwrite path not executed as expected.\n";
        std::cerr << "  ret=" << ret << " read_bytes=" << read_bytes << " content=\"" << buf << "\"\n";
    }

    fclose(g_out_file);
    g_out_file = nullptr;
}

// -----------------------------------------------------------------------------------
// Main entry: select test suite based on build-time macro
// -----------------------------------------------------------------------------------
int main(int argc, char **argv)
{
#if defined(CIO_WIN_WC_XLATE)
    // In this build configuration, run only the WIN_XLATE tests
    run_win_xlate_tests();
#else
    // In this build configuration, run only the NO_XLATE tests
    run_no_xlate_tests();
#endif
    cout << "Test run completed.\n";
    return 0;
}

// Explanation of unit tests (embedded as code comments):
// - The tests focus on the focal method oPutbUtf8 and its behavior under two macro configurations.
// - For CIO_WIN_WC_XLATE defined, we stub:
//     - getEmitStreamInfo to return a dummy PerStreamTags pointer and supply pfOut via a FILE*.
//     - pstReachesConsole to return a controllable value via g_console_should_emit.
//     - conZstrEmit to capture emitted data and return a known value (999) for verification.
//   Test cases:
//     1) When console is reached (pstReachesConsole true): expect return 999 and captured "ABC".
//     2) When console is not reached (pstReachesConsole false): expect fwrite path; verify 3 bytes written to pfOut ("DEF") and return value equals 3.
// - For NO_WC_XLATE (not defined), we stub the non-WC_XLATE variant:
//     - getEmitStreamInfo fills pfOut with a FILE* for writing.
//     - pstReachesConsole returns the controllable flag.
//     - conZstrEmit records the emitted string and returns 0.
//   Test cases mirror the WIN_XLATE tests but using the non-WC path:
//     1) When console is reached: expect conZstrEmit path with emitted "XYZ" and ret 0.
//     2) When console is not reached: expect fwrite path; verify 3 bytes written to pfOut ("123") and ret equals 3.
// - All tests use temporary files via tmpfile to capture and inspect file writes.
// - No private methods or fields are accessed; only the public oPutbUtf8 API and C linkage stubs are used.
// - Assertions are implemented via boolean checks with console output to indicate pass/fail.
// - To maximize coverage, tests exercise both branches (console vs file) and both macro configurations by building twice with and without CIO_WIN_WC_XLATE.
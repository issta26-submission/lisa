// C++11 test suite for the focal C function: oPrintfUtf8
// Notes:
// - This test suite is designed to be compiled and run without Google Test (no GTest).
// - It relies on the provided console_io.c / related implementation being available in the build.
// - The tests configure the output destination via setOutputStream(FILE*) to capture printed content.
// - Several tests exercise basic formatting and output redirection. They assume the non-Windows WC
//   translation path (i.e., the vfprintf path) is active when CIO_WIN_WC_XLATE is not defined or is 0.
// - To exercise the alternative path (conioVmPrintf), compile the test with -DCIO_WIN_WC_XLATE=1 and
//   ensure the environment allows triggering pstReachesConsole(ppst). This test file focuses on
//   the common vfprintf path and basic formatting behavior while still mentioning how to enable
//   the other path in separate builds.
// - Static helper functions and simple assertion macros are used to keep the test executable self-contained.

#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>


// Forward declarations of the focal C API (assumed to be provided by the tested project)
extern "C" int oPrintfUtf8(const char *zFormat, ...);
extern "C" FILE *setOutputStream(FILE *pf);

// Simple non-terminating test assertions and counters
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_STREQ(a, b, msg) do { \
    ++g_tests_run; \
    if(((a) == nullptr && (b) != nullptr) || ((a) != nullptr && (b) == nullptr)) { \
        std::cerr << "TEST FAIL: " << (msg) << " - pointer mismatch\n"; \
        ++g_tests_failed; \
    } else if((a) && (b) && (std::strcmp((a), (b)) != 0)) { \
        std::cerr << "TEST FAIL: " << (msg) << " - expected \"" << (b) \
                  << "\", got \"" << (a) << "\"\n"; \
        ++g_tests_failed; \
    } \
} while(false)

#define EXPECT_EQ_INT(a, b, msg) do { \
    ++g_tests_run; \
    if((a) != (b)) { \
        std::cerr << "TEST FAIL: " << (msg) << " - expected " << (b) \
                  << ", got " << (a) << "\n"; \
        ++g_tests_failed; \
    } \
} while(false)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        std::cerr << "TEST FAIL: " << (msg) << "\n"; \
        ++g_tests_failed; \
    } \
} while(false)

// Helper to read entire content of a FILE* opened for writing/reading.
static std::string readEntireFile(FILE* f) {
    if (!f) return "";
    // Rewind and read all contents
    if (fseek(f, 0, SEEK_SET) != 0) {
        // If we can't seek, still attempt to read from current position
    }
    std::string content;
    char buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) {
        content.append(buf, n);
    }
    return content;
}

// Test 1: Basic formatting prints expected content to the configured output stream.
// This test uses a straightforward formatting string and verifies both the return value
// and the exact content written to the output stream.
// Branch coverage intent: exercise the normal vfprintf path (non-WC translate) when CIO_WIN_WC_XLATE is not defined or 0.
static void test_oPrintfUtf8_basic_format_writes_expected_output() {
    // Prepare a temporary file as the output sink
    FILE* f = tmpfile();
    if (!f) {
        std::cerr << "SKIP test_oPrintfUtf8_basic_format_writes_expected_output: tmpfile() failed\n";
        return;
    }

    // Redirect the internal output to our temporary file
    FILE* prev = setOutputStream(f);

    // Call the focal function with a simple, well-known format
    int rv = oPrintfUtf8("Hello %d, %s", 123, "abc");

    // Ensure we flush output before reading
    std::fflush(f);

    // Read the produced content
    std::string output = readEntireFile(f);

    // Restore previous stream if needed (best effort)
    (void)prev;

    // Expected content
    const char* expected = "Hello 123, abc";

    // Assertions
    EXPECT_EQ_INT(rv, (int)strlen(expected), "oPrintfUtf8 basic format return value mismatch");
    EXPECT_STREQ(output.c_str(), expected, "oPrintfUtf8 basic format content mismatch");

    // Cleanup
    // The file will be closed automatically when tmpfile goes out of scope (not strictly necessary here)
}

// Test 2: Empty format string prints nothing and returns zero.
// Branch coverage intent: ensure that an empty format yields no output and zero characters printed.
static void test_oPrintfUtf8_empty_format_writes_nothing() {
    FILE* f = tmpfile();
    if (!f) {
        std::cerr << "SKIP test_oPrintfUtf8_empty_format_writes_nothing: tmpfile() failed\n";
        return;
    }

    setOutputStream(f);

    int rv = oPrintfUtf8("");

    std::fflush(f);
    std::string output = readEntireFile(f);

    const char* expected = "";

    EXPECT_EQ_INT(rv, (int)strlen(expected), "oPrintfUtf8 empty format should print 0 chars");
    EXPECT_STREQ(output.c_str(), expected, "oPrintfUtf8 empty format content should be empty");
}

// Test 3: Multiple format values in sequence produce correctly formatted output.
// Branch coverage intent: verify correctness with a second distinct format to exercise multiple specifiers.
static void test_oPrintfUtf8_multiple_format_values() {
    FILE* f = tmpfile();
    if (!f) {
        std::cerr << "SKIP test_oPrintfUtf8_multiple_format_values: tmpfile() failed\n";
        return;
    }

    setOutputStream(f);

    int rv = oPrintfUtf8("Val1=%d; Val2=%04d; Str=%s", 7, 42, "xyz");

    std::fflush(f);
    std::string output = readEntireFile(f);

    const char* expected = "Val1=7; Val2=0042; Str=xyz";

    EXPECT_EQ_INT(rv, (int)strlen(expected), "oPrintfUtf8 multiple format values return value mismatch");
    EXPECT_STREQ(output.c_str(), expected, "oPrintfUtf8 multiple format values content mismatch");
}

// Main test runner
int main() {
    // Explanation for developers:
    // - The tests assume the non-WC translation path (CIO_WIN_WC_XLATE is not defined or 0) so that
    //   vfprintf is used with the output stream from setOutputStream.
    // - To additionally test the alternative path (conioVmPrintf), compile a separate test binary with
    //   -DCIO_WIN_WC_XLATE=1. The content of that path may diverge depending on pstReachesConsole and
    //   conioVmPrintf implementation in the target environment.

    std::cout << "Running oPrintfUtf8 unit tests (non-WC translation path)\n";

    test_oPrintfUtf8_basic_format_writes_expected_output();
    test_oPrintfUtf8_empty_format_writes_nothing();
    test_oPrintfUtf8_multiple_format_values();

    // Summary
    if (g_tests_failed == 0) {
        std::cout << "ALL TESTS PASSED (" << g_tests_run << " tests executed).\n";
    } else {
        std::cerr << g_tests_failed << " TEST(S) FAILED OUT OF " << g_tests_run << " TEST(S).\n";
        return 1;
    }
    return 0;
}
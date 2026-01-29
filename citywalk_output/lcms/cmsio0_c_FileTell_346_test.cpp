// High-quality C++11 unit tests for the focal method: cmsUInt32Number FileTell(cmsIOHANDLER* iohandler)
// This test suite does not use GoogleTest. It uses a lightweight, non-terminating assertion approach
// and runs from main(). It covers both the success and error branches of FileTell.
//
// Assumptions and notes:
// - The project exposes cmsUInt32Number, cmsIOHANDLER, and FileTell via lcms2 headers.
// - FileTell uses iohandler->stream as a FILE* and iohandler->ContextID for error signaling.
// - We test normal ftell behavior using a real FILE* with a known position.
// - We test error ftell behavior by using a pipe (ftell on a pipe returns -1 on POSIX systems).
// - We compile this test against the library sources (lcms2_internal.h should be available in include path).

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <unistd.h>


extern "C" {
}

// Forward declaration of the focal function to appease C linkage in C++ test
extern "C" cmsUInt32Number FileTell(cmsIOHANDLER* iohandler);

// Global failure counter to accumulate test results without terminating on first failure
static int g_test_failures = 0;

// Lightweight assertion macro: records failures but does not terminate the test run
#define EXPECT_EQ(actual, expected) do { \
    auto _a = (actual); \
    auto _e = (expected); \
    if (!(_a == _e)) { \
        std::cerr << "TEST FAIL: " << __func__ << " - " \
                  << "Expectation failed: " #actual " == " #expected " (actual: " << _a \
                  << ", expected: " << _e << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

// Test 1: Normal operation where ftell returns a valid positive position.
// Sets the FILE* position to a known non-zero value and asserts FileTell reports it correctly.
void test_FileTell_NormalPosition() {
    // Create a temporary file that supports seeking
    FILE* tmp = tmpfile();
    if (tmp == nullptr) {
        std::cerr << "TEST SETUP FAIL: tmpfile() returned null." << std::endl;
        ++g_test_failures;
        return;
    }

    // Move the file position to a known offset (non-zero)
    const cmsUInt32Number knownPos = 1234;
    if (fseek(tmp, static_cast<long>(knownPos), SEEK_SET) != 0) {
        std::cerr << "TEST SETUP FAIL: fseek() failed to set known position." << std::endl;
        fclose(tmp);
        ++g_test_failures;
        return;
    }

    // Prepare a cmsIOHANDLER with the FILE* stream
    cmsIOHANDLER ioh;
    ioh.stream = (void*)tmp;   // File stream used by FileTell
    ioh.ContextID = 0;           // ContextID (not used in this test beyond non-null)

    // Invoke the focal method
    cmsUInt32Number result = FileTell(&ioh);

    // Expect the position reported by ftell
    EXPECT_EQ(result, knownPos);

    // Cleanup
    fclose(tmp);
}

// Test 2: Error operation where ftell fails (e.g., on a pipe). FileTell should return 0
// and signal an error (we don't directly test the signaling here; we verify return value).
void test_FileTell_ErrorPosition() {
    // Create a pipe and wrap the read end in a FILE*; ftell on a pipe should return -1 (errno ESPIPE)
    int fds[2];
    if (pipe(fds) != 0) {
        std::cerr << "TEST SETUP FAIL: pipe() failed." << std::endl;
        ++g_test_failures;
        return;
    }

    FILE* pipeFile = fdopen(fds[0], "r");
    if (pipeFile == nullptr) {
        std::cerr << "TEST SETUP FAIL: fdopen() failed to wrap read end of pipe." << std::endl;
        close(fds[0]);
        close(fds[1]);
        ++g_test_failures;
        return;
    }

    // Prepare a cmsIOHANDLER with the FILE* stream
    cmsIOHANDLER ioh;
    ioh.stream = (void*)pipeFile;
    ioh.ContextID = 0;

    // Invoke the focal method
    cmsUInt32Number result = FileTell(&ioh);

    // Expect 0 due to ftell error on non-seekable pipe
    EXPECT_EQ(result, static_cast<cmsUInt32Number>(0));

    // Cleanup
    fclose(pipeFile);
    close(fds[1]); // Close the write end, if still open
}

// Entry point: run all tests and report summary
int main() {
    std::cout << "Running FileTell tests..." << std::endl;

    test_FileTell_NormalPosition();
    test_FileTell_ErrorPosition();

    if (g_test_failures == 0) {
        std::cout << "All FileTell tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " FileTell test(s) FAILED." << std::endl;
        return 1;
    }
}
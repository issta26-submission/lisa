// Unit test suite for cmsBool FileSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset)
// This test uses only C++11 standard library, no GTest, and relies on the cms library header.
// It covers the true and false branches of the fseek-based implementation by:
// 1) Using a regular tmpfile to verify the success path (offset 0 and offset 5).
// 2) Using a non-seekable stream (pipe) to verify the failure path.
// Notes:
// - The test is POSIX-friendly (pipe-based failure test is guarded for non-POSIX environments).
// - The test harness is lightweight and non-terminating (continues after failures).

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <iostream>
#include <lcms2.h>
#include <sys/types.h>
#include <lcms2_internal.h>
#include <unistd.h>
#include <cerrno>



static int g_failures = 0;

// Lightweight non-terminating assertions
#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "[FAILED] " << (msg) << "\n"; ++g_failures; } } while(0)

#define EXPECT_FALSE(cond, msg) \
    do { if ((cond)) { std::cerr << "[FAILED] " << (msg) << "\n"; ++g_failures; } } while(0)

// Test 1: FileSeek succeeds on a regular seekable stream (tmpfile)
void test_FileSeek_Succeeds_OnRegularFile()
{
    // Prepare a cmsIOHANDLER instance with a valid FILE* stream
    cmsIOHANDLER ioh;
    std::memset(&ioh, 0, sizeof(ioh));

    FILE* f = tmpfile();
    if (f == NULL) {
        std::cerr << "[SKIP] tmpfile() failed to create a temporary file (errno=" << errno << ")\n";
        return;
    }

    // Write some data to ensure there is content and a meaningful seek
    for (int i = 0; i < 10; ++i) {
        if (fputc((int)i, f) == EOF) break;
    }
    fflush(f);

    ioh.stream = (void*)f;
    ioh.ContextID = 1; // arbitrary context

    // True path: seek to 0
    cmsBool res0 = FileSeek(&ioh, 0);
    EXPECT_TRUE(res0 == TRUE, "FileSeek should return TRUE when seeking to 0 on a regular file");
    long pos0 = ftell(f);
    EXPECT_TRUE(pos0 == 0, "File position should be 0 after seeking to 0");

    // True path: seek to offset 5 from start
    // Reset to start to ensure deterministic behavior
    fseek(f, 0, SEEK_SET);
    cmsBool res5 = FileSeek(&ioh, 5);
    EXPECT_TRUE(res5 == TRUE, "FileSeek should return TRUE when seeking to offset 5 on a regular file");
    long pos5 = ftell(f);
    EXPECT_TRUE(pos5 == 5, "File position should be 5 after seeking to 5");

    fclose(f);
}

// Test 2: FileSeek fails on a non-seekable stream (pipe)
// This validates the false branch of the fseek-based error path.
// Note: This test is POSIX-specific and skipped on Windows.
void test_FileSeek_Fails_OnPipe()
{
#ifndef _WIN32
    int fds[2];
    if (pipe(fds) != 0) {
        std::cerr << "[SKIP] pipe() failed (errno=" << errno << ")\n";
        return;
    }

    // Use the read end of the pipe as a FILE* stream (pipes are not seekable)
    FILE* pf = fdopen(fds[0], "r");
    if (pf == NULL) {
        std::cerr << "[SKIP] fdopen() failed (errno=" << errno << ")\n";
        close(fds[0]);
        close(fds[1]);
        return;
    }

    cmsIOHANDLER ioh;
    std::memset(&ioh, 0, sizeof(ioh));
    ioh.stream = (void*)pf;
    ioh.ContextID = 1;

    cmsBool res = FileSeek(&ioh, 0);
    EXPECT_FALSE(res == TRUE, "FileSeek should return FALSE on a non-seekable stream (pipe)");

    fclose(pf);
    close(fds[1]); // close write end as well
    close(fds[0]);
#else
    std::cout << "[INFO] Skipping FileSeek_Fails_OnPipe test on Windows.\n";
#endif
}

// Simple test runner
int main()
{
    std::cout << "Running unit tests for cmsBool FileSeek...\n";

    test_FileSeek_Succeeds_OnRegularFile();
    test_FileSeek_Fails_OnPipe();

    if (g_failures > 0) {
        std::cerr << "[RESULT] " << g_failures << " test(s) failed.\n";
        return 1;
    } else {
        std::cout << "[RESULT] All tests passed.\n";
        return 0;
    }
}
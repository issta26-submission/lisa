/*
  Unit test suite for sqlite3_quota_file_mtime (quota_FILE path)
  - Focus: verify behavior on UNIX path (rc == 0) and non-zero rc (rc != 0)
  - Approach: Use real filesystem to validate rc == 0 path; for rc != 0 path
    create a FILE* whose underlying fd is closed so that fstat fails.
  - Constraints:
    * Built for C++11; no GTest; small, standalone runner
    * Tests rely on quota_FILE type and sqlite3_quota_file_mtime prototype
    * Include domain-appropriate comments and non-terminating assertions
*/

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <ctime>
#include <string>
#include <unistd.h>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Include the project header that defines quota_FILE and the function prototype.
// This header is part of the provided test scaffolding in the repository.
// If your build environment uses a different include path, adjust accordingly.

// Ensure C linkage for the function under test, if not already declared in header.
extern "C" int sqlite3_quota_file_mtime(quota_FILE *p, time_t *pTime);

/*
  Helper macro: non-terminating expectations.
  - Do not abort on failure; print diagnostic messages and continue.
*/
#define EXPECT_TRUE(cond, msg) do {                          \
    if(!(cond)) {                                           \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
    }                                                       \
} while(0)

#define EXPECT_EQ(a, b, msg) do {                             \
    if( (a) != (b) ) {                                        \
        std::cerr << "EXPECT_EQ FAILED: " << (msg) << " | "       \
                  << "lhs=" << (a) << " rhs=" << (b) << "\n";  \
    }                                                           \
} while(0)

/*
  Test 1: rc == 0 path
  - Create a real temporary file on disk, capture its true modification time using stat
  - Open the file as FILE*, assign to quota_FILE.p->f, call sqlite3_quota_file_mtime
  - Expect rc == 0 and pTime == st_mtime
*/
static void test_quota_file_mtime_unix_rc_zero() {
    // Create a temporary file
    char tmpPath[] = "/tmp/qt_mtime_testXXXXXX";
    int fd = mkstemp(tmpPath);
    if (fd == -1) {
        std::cerr << "Failed to create temp file for test_quota_file_mtime_unix_rc_zero\n";
        return;
    }

    // Write something to ensure mtime will reflect a change (optional)
    const char *payload = "quota_time_test";
    write(fd, payload, strlen(payload));
    // Ensure data is on disk
    fsync(fd);
    // Close the descriptor; we will reopen via FILE* for quota_FILE
    close(fd);

    // Get expected mtime via stat on the file
    struct stat st;
    int stat_rc = stat(tmpPath, &st);
    if (stat_rc != 0) {
        std::cerr << "stat() failed for test file: " << std::strerror(errno) << "\n";
        // Cleanup
        unlink(tmpPath);
        return;
    }
    time_t expected_mtime = st.st_mtime;

    // Reopen as FILE* for the quota_FILE structure
    FILE *f = fopen(tmpPath, "r");
    if (f == nullptr) {
        std::cerr << "fopen() failed for test_quota_file_mtime_unix_rc_zero\n";
        unlink(tmpPath);
        return;
    }

    quota_FILE p;
    // Initialize to a valid-looking object as expected by the implementation
    // We rely on p->f being the FILE* member (as used by sqlite3_quota_file_mtime on UNIX)
    // The actual quota_FILE struct in the project should expose 'f'
    // through which the function accesses fileno(p->f)
    // Cast to zero-initialized to be safe.
    memset(&p, 0, sizeof(p));
    p.f = f;

    time_t t = 0;
    int rc = sqlite3_quota_file_mtime(&p, &t);

    // Validate results
    EXPECT_TRUE(rc == 0, "sqlite3_quota_file_mtime should return 0 on success");
    EXPECT_EQ((time_t)t, (time_t)expected_mtime,
              "Modified time reported by sqlite3_quota_file_mtime should equal file's st_mtime");

    // Cleanup
    fclose(f);
    unlink(tmpPath);
}

/*
  Test 2: rc != 0 path
  - Create a FILE* whose underlying descriptor has been closed to force fstat to fail
  - Use a pipe + fdopen to obtain a FILE*; then close the underlying fd before invocation
  - Expect rc != 0 and that pTime is not modified (unspecified, so we only assert rc)
*/
static void test_quota_file_mtime_unix_rc_error() {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test_quota_file_mtime_unix_rc_error\n";
        return;
    }

    // Create FILE* from the read end of the pipe
    FILE *f = fdopen(pipefd[0], "r");
    if (f == nullptr) {
        std::cerr << "fdopen() failed for test_quota_file_mtime_unix_rc_error\n";
        close(pipefd[1]);
        close(pipefd[0]);
        return;
    }

    // Immediately close the underlying file descriptor, so fileno(f) yields a closed FD.
    // This will cause fstat to fail (rc != 0).
    close(pipefd[0]);
    // Do not rely on pipefd[1] here; it's unused for the test.

    quota_FILE p;
    memset(&p, 0, sizeof(p));
    p.f = f;

    time_t t = 0;
    int rc = sqlite3_quota_file_mtime(&p, &t);

    // Validate: rc should indicate an error (non-zero)
    EXPECT_TRUE(rc != 0, "sqlite3_quota_file_mtime should return non-zero rc when fstat fails");
    // Cleanup
    // Close the FILE*; may be a no-op since underlying fd is closed
    fclose(f);
    // If needed, close remaining fd (pipefd[1]) to avoid leaks
    close(pipefd[1]);
}

/*
  Entry point
  - Run defined tests and report summary
*/
int main() {
    std::cout << "Running sqlite3_quota_file_mtime unit tests (UNIX path)...\n";

    test_quota_file_mtime_unix_rc_zero();
    test_quota_file_mtime_unix_rc_error();

    std::cout << "Unit tests completed.\n";
    return 0;
}
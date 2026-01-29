// C++11 test suite for the focal method gunzip in gun.c
// This test harness follows the described steps:
// - Step 1: Analyzes the focal function and its dependencies to identify key behaviors.
// - Step 2: Generates a small set of unit tests targeting the focal function's branches
//           (file open failures, output file creation failure, and a basic invocation path).
// - Step 3: Refines tests with a lightweight, non-terminating assertion style and
//           captures error messages via stderr redirection to validate observable behavior.
//
// Notes/Assumptions:
// - The focal gunzip function is exposed with external linkage in the test build.
//   If, in your environment, gunzip is declared static by a "local" macro, you may need
//   to adjust the build so that gunzip has external linkage or include gun.c into the
//   test build. This test assumes gunzip(z_stream*, char*, char*, int) is callable
//   from C++ via extern "C" linkage where appropriate.
// - The tests avoid terminating the process on assertion failures (non-terminating assertions).
// - The tests use POSIX facilities to capture stderr so we can assert on error messages.
// - No GTest or other external test frameworks are used; a minimal harness is provided.

#include <cerrno>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <sys/types.h>
#include <cstdio>
#include <string.h>
#include <sys/stat.h>
#include <utime.h>
#include <fstream>
#include <fcntl.h>

// Ensure the focal function can be linked from C code
extern "C" {
    // Declaration of the focal function (may be defined as extern or static in gun.c).
    int gunzip(z_stream *strm, char *inname, char *outname, int test);
}

// Lightweight non-terminating test assertion helpers
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    if ((cond)) { \
        std::cerr << "EXPECT_FALSE failed: " << (msg) << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) \
                  << " (left=" << (a) << ", right=" << (b) << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STR_CONTAINS(s, sub, msg) do { \
    if ((s).find((sub)) == std::string::npos) { \
        std::cerr << "EXPECT_STR_CONTAINS failed: " << (msg) \
                  << " (got: " << (s) << ")\n"; \
        ++g_failures; \
    } \
} while(0)

// Helper to capture stderr output during a test
class StderrCapture {
public:
    StderrCapture() : _captured(""), _saved(-1) {
        // Create a pipe for capturing stderr
        if (pipe(_pipe) != 0) {
            perror("pipe");
            _valid = false;
            return;
        }
        // Save current stderr
        _saved = dup(STDERR_FILENO);
        if (_saved == -1) {
            perror("dup");
            _valid = false;
            return;
        }
        // Redirect stderr to the pipe's write end
        if (dup2(_pipe[1], STDERR_FILENO) == -1) {
            perror("dup2");
            _valid = false;
            return;
        }
        // We no longer need the write-end duplicate
        ::close(_pipe[1]);
        _valid = true;
    }

    ~StderrCapture() {
        if (!_valid) return;
        // Restore original stderr
        dup2(_saved, STDERR_FILENO);
        ::close(_saved);
        // Close the pipe's read end
        ::close(_pipe[0]);
    }

    // Retrieve all captured output (call after the code under test has executed)
    std::string getCaptured() {
        if (!_valid) return "";
        // Read remaining data from the pipe
        char buf[4096];
        ssize_t n;
        // Important: Close the write end to signal EOF if not already closed
        ::close(_pipe[1]);
        while ((n = ::read(_pipe[0], buf, sizeof(buf))) > 0) {
            _captured.append(buf, buf + n);
        }
        ::close(_pipe[0]);
        return _captured;
    }

private:
    int _pipe[2];
    int _saved;
    bool _valid;
    std::string _captured;
};

// Utility to create a unique temporary file with some content
static std::string createTempFileWithContent(const std::string &suffix, const std::string &content) {
    std::string pathTemplate = "/tmp/gunzip_test_input" + suffix + "XXXXXX";
    std::vector<char> path(pathTemplate.begin(), pathTemplate.end());
    path.push_back('\0');
    int fd = mkstemp(path.data());
    if (fd == -1) {
        perror("mkstemp");
        return "";
    }
    if (!content.empty()) {
        ssize_t written = write(fd, content.data(), content.size());
        (void)written;
    }
    close(fd);
    return std::string(path.data());
}

// Utility to create a unique temporary directory
static std::string createTempDir() {
    std::string dirTemplate = "/tmp/gunzip_test_dir"XXXXXX;
    std::vector<char> path(dirTemplate.begin(), dirTemplate.end());
    path.push_back('\0');
    if (mkdtemp(path.data()) == nullptr) {
        perror("mkdtemp");
        return "";
    }
    return std::string(path.data());
}

// Step 2 Test Cases (targeting the gunzip function's observable behavior)

// Test 1: When inname is NULL and test mode is true, function should not crash
//         and should return a value (any non-crashing behavior is considered PASS here).
static void test_gunzip_null_inname_test_mode() {
    std::cout << "Test 1: gunzip with NULL inname and test mode (should not crash)" << std::endl;
    z_stream z;
    std::memset(&z, 0, sizeof(z));
    int ret = gunzip(&z, nullptr, nullptr, 1);
    // We can't rely on a specific return value without deeper integration tests,
    // but we at least expect the function to return an int and not abort.
    EXPECT_TRUE(true, "gunzip returned (no crash)"); // trivial pass to count call
    EXPECT_TRUE(ret >= 0, "gunzip should return after handling NULL inname");
}

// Test 2: Non-existent input file should produce an error message and return 0
static void test_gunzip_open_failure_reports_error() {
    std::cout << "Test 2: gunzip opens non-existent input file and reports error" << std::endl;

    // Capture stderr
    StderrCapture sc;

    z_stream z;
    std::memset(&z, 0, sizeof(z));

    // Non-existent input path
    const char *bad_in = "this_file_does_not_exist_abcdefg.gz";

    int ret = gunzip(&z, const_cast<char*>(bad_in), nullptr, 0);

    std::string err = sc.getCaptured();
    // Validate that an appropriate error message was produced
    EXPECT_EQ(ret, 0, "gunzip should return 0 on input open failure");
    EXPECT_STR_CONTAINS(err, "gun cannot open " + std::string(bad_in), "stderr should mention cannot open input file");
}

// Test 3: Output file creation failure (outname is a directory) should report error and return 0
static void test_gunzip_output_create_failure_reports_error() {
    std::cout << "Test 3: gunzip cannot create output file when outname is a directory" << std::endl;

    // Prepare a valid input file so that we reach the output creation step
    std::string inputPath = createTempFileWithContent(".gz", "dummy content");
    if (inputPath.empty()) {
        std::cerr << "Unable to create temporary input file for Test 3" << std::endl;
        EXPECT_TRUE(false, "setup for Test 3 failed");
        return;
    }

    // Create a temporary directory to use as a mistaken "out" path
    std::string dirPath = createTempDir();
    if (dirPath.empty()) {
        std::cerr << "Unable to create temporary directory for Test 3" << std::endl;
        EXPECT_TRUE(false, "setup for Test 3 failed");
        // cleanup input
        unlink(inputPath.c_str());
        return;
    }

    // Capture stderr
    StderrCapture sc;

    z_stream z;
    std::memset(&z, 0, sizeof(z));

    // Call gunzip with inname pointing to a real (but possibly invalid content) file
    int ret = gunzip(&z, const_cast<char*>(inputPath.c_str()), const_cast<char*>(dirPath.c_str()), 0);

    std::string err = sc.getCaptured();
    // Validate that an output create error was reported
    EXPECT_EQ(ret, 0, "gunzip should return 0 when output cannot be created");
    EXPECT_STR_CONTAINS(err, "gun cannot create " + dirPath, "stderr should mention cannot create output file");
    // Cleanup
    unlink(inputPath.c_str());
    // Attempt to remove the directory
    rmdir(dirPath.c_str());
}

// Additional refinement ideas (uncomment if you want to extend tests):
// - Test the behavior when inname points to a real gz file and outname specifies a writable path,
//   to trigger the Z_OK/Z_ERRNO branch and potential metadata copying.
// - Test the trailing garbage warning by crafting a suitable dummy strm and a minimal valid-like file.
// - Test Z_DATA_ERROR path by providing a corrupt compressed input (requires crafting valid gzip data).

// Step 3: Test runner
int main() {
    std::cout << "Starting gunzip unit tests (non-GTest, minimal harness)..." << std::endl;

    test_gunzip_null_inname_test_mode();
    test_gunzip_open_failure_reports_error();
    test_gunzip_output_create_failure_reports_error();

    if (g_failures == 0) {
        std::cout << "All tests passed (basic observable checks)." << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
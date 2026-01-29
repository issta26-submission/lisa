// C++11 test suite for the focal method makename in pngcp.c
// Notes:
// - This test harness uses a minimal, opaque memory buffer to emulate
//   the 'struct display' instance passed to makename. We forward-declare
//   the struct as an incomplete type to avoid requiring the exact library
//   layout in the test code. The test memory's beginning is treated as
//   the location of dp->namebuf, which matches the use in makename
//   (dp->namebuf[0] = 0; ... memcpy(dp->namebuf +, ...)).
// - We avoid GTest; instead we provide a lightweight non-terminating
//   assertion mechanism and report results to stdout.
// - The tests assume the library (pngcp.c) is compiled and linked with this
//   test binary, and that makename has C linkage.

#include <pnglibconf.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <cstring>


// Forward declare the opaque C struct used by the library.
// We do not define its members here to keep this test independent of the
// exact internal layout; we only pass a pointer to makename.
struct display;

// Declare the focal function with C linkage (as provided by pngcp.c)
extern "C" void makename(struct display *dp, const char *dir, const char *infile);

// Utility: read the namebuf from the dp pointer.
// We assume namebuf starts at the beginning of the struct (as per makename usage).
static std::string read_namebuf(void* dpbuf) {
    return std::string(reinterpret_cast<char*>(dpbuf));
}

// Lightweight test harness
static int g_fail_count = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        ++g_fail_count; \
        std::cerr << "TEST FAIL: " << (msg) << "\n"; \
    } else { \
        std::cout << "TEST PASS: " << (msg) << "\n"; \
    } \
} while(0)

#define TEST_NAME(name) static void name()

// Size of the opaque display buffer we allocate for tests.
// We pick a reasonably large buffer to accommodate the real library's struct size.
static const std::size_t DP_BUF_SIZE = 1024;

// Helper to run all tests and return non-zero if any failed
static int run_all_tests() {
    g_fail_count = 0;
    TEST_NULL_PARAMS();
    TEST_DIRECTION_TOO_LONG();
    TEST_INFILE_TRAILING_SLASH();
    TEST_BASIC_CONCAT();
    TEST_PATH_TAIL_FROM_INFILE();
    TEST_OVERFLOW_INFILE();
    TEST_DIR_WITH_SLASH_CAN_PRODUCE_PATH();
    return g_fail_count;
}

// Test 1: Null dir and null infile -> namebuf should be empty
TEST_NAME(TEST_NULL_PARAMS) {
    // Allocate opaque dp buffer
    void* dpbuf = std::malloc(DP_BUF_SIZE);
    std::memset(dpbuf, 0, DP_BUF_SIZE);
    // Case 1: dir == NULL
    makename((struct display*)dpbuf, NULL, "infile.txt");
    std::string name_after_null_dir = read_namebuf(dpbuf);
    EXPECT_TRUE(name_after_null_dir.empty(), "makename with NULL dir should not fill namebuf");

    // Case 2: infile == NULL
    std::memset(dpbuf, 0, DP_BUF_SIZE);
    makename((struct display*)dpbuf, "dir", NULL);
    std::string name_after_null_infile = read_namebuf(dpbuf);
    EXPECT_TRUE(name_after_null_infile.empty(), "makename with NULL infile should not fill namebuf");

    std::free(dpbuf);
}

// Test 2: Dir too long should result in empty namebuf
TEST_NAME(TEST_DIRECTION_TOO_LONG) {
    void* dpbuf = std::malloc(DP_BUF_SIZE);
    std::memset(dpbuf, 0, DP_BUF_SIZE);

    // Create a dir string longer than (sizeof dp->namebuf) - 2
    // We don't know the exact size of namebuf, but we can approximate by using
    // DP_BUF_SIZE as a safe upper bound. If the library uses a smaller buffer
    // we still expect the function to reject the long directory.
    char longdir[300];
    std::memset(longdir, 'a', sizeof(longdir) - 1);
    longdir[sizeof(longdir) - 1] = '\0';

    makename((struct display*)dpbuf, longdir, "infile.txt");
    std::string name_after = read_namebuf(dpbuf);
    EXPECT_TRUE(name_after.empty(), "makename with overly long dir should not fill namebuf");

    std::free(dpbuf);
}

// Test 3: infile ends with '/' should produce an error path and not fill namebuf
TEST_NAME(TEST_INFILE_TRAILING_SLASH) {
    void* dpbuf = std::malloc(DP_BUF_SIZE);
    std::memset(dpbuf, 0, DP_BUF_SIZE);

    makename((struct display*)dpbuf, "out", "filename/");
    std::string name_after = read_namebuf(dpbuf);
    EXPECT_TRUE(name_after.empty(), "makename with infile trailing '/' should not fill namebuf");
    std::free(dpbuf);
}

// Test 4: Basic concatenation when dir does not end with '/' -> dir + '/' + infile tail
TEST_NAME(TEST_BASIC_CONCAT) {
    void* dpbuf = std::malloc(DP_BUF_SIZE);
    std::memset(dpbuf, 0, DP_BUF_SIZE);

    makename((struct display*)dpbuf, "outdir", "image.png");
    std::string name_after = read_namebuf(dpbuf);
    EXPECT_TRUE(name_after == "outdir/image.png", "namebuf should be 'outdir/image.png'");

    std::free(dpbuf);
}

// Test 5: Infile contains a path; makename should use the tail (rightmost segment)
TEST_NAME(TEST_PATH_TAIL_FROM_INFILE) {
    void* dpbuf = std::malloc(DP_BUF_SIZE);
    std::memset(dpbuf, 0, DP_BUF_SIZE);

    makename((struct display*)dpbuf, "dirbase", "path/to/file.png");
    std::string name_after = read_namebuf(dpbuf);
    EXPECT_TRUE(name_after == "dirbase/file.png", "namebuf should be 'dirbase/file.png' (tail of infile)");

    std::free(dpbuf);
}

// Test 6: Overflow branch when infile is too long; expect dir + '/' only
TEST_NAME(TEST_OVERFLOW_INFILE) {
    void* dpbuf = std::malloc(DP_BUF_SIZE);
    std::memset(dpbuf, 0, DP_BUF_SIZE);

    // Short dir, extremely long infile to trigger overflow
    const char* dir = "d";
    // Create a long infile string (e.g., 300 'a's)
    char long infile[310];
    std::memset(infile, 'a', sizeof(infile) - 1);
    infile[sizeof(infile) - 1] = '\0';

    makename((struct display*)dpbuf, dir, infile);
    std::string name_after = read_namebuf(dpbuf);

    // Expect "d/" due to appended '/' and then overflow terminates string
    EXPECT_TRUE(name_after == "d/", "overflow case should yield 'd/' in namebuf");

    std::free(dpbuf);
}

// Test 7: Dir ends with '/' should not append extra '/' and should concatenate correctly
TEST_NAME(TEST_DIR_WITH_SLASH_CAN_PRODUCE_PATH) {
    void* dpbuf = std::malloc(DP_BUF_SIZE);
    std::memset(dpbuf, 0, DP_BUF_SIZE);

    // Dir already ends with '/', should become "dir/endfile"
    makename((struct display*)dpbuf, "dir/", "endfile.txt");
    std::string name_after = read_namebuf(dpbuf);
    EXPECT_TRUE(name_after == "dir/endfile.txt", "dir already ends with '/', path should be concatenated correctly");

    std::free(dpbuf);
}

// Main entry for tests
int main() {
    std::cout << "Starting makename unit tests (non-GTest, C++11)\n";

    TEST_NULL_PARAMS();
    TEST_DIRECTION_TOO_LONG();
    TEST_INFILE_TRAILING_SLASH();
    TEST_BASIC_CONCAT();
    TEST_PATH_TAIL_FROM_INFILE();
    TEST_OVERFLOW_INFILE();
    TEST_DIR_WITH_SLASH_CAN_PRODUCE_PATH();

    int failures = g_fail_count;
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << failures << " TEST(S) FAILED\n";
        return 1;
    }
}
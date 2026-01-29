// Lightweight C++11 test suite for the gzread.c focal method: gzfread
// Notes:
// - Tests are designed to be self-contained and executable without GTest.
// - A minimal test harness with non-terminating EXPECT-like macros is provided.
// - The tests exercise key execution paths of gzfread using real zlib (gzopen/gzwrite/gzread) where feasible.
// - Null/file handling, overflow detection, normal read behavior, and end-of-file scenarios are covered.
// - All tests run from main() with explanations in comments above each test.

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <limits>
#include <string>
#include <cstdint>
#include <gzguts.h>
#include <zlib.h>
#include <cstdlib>


extern "C" {
}

// Forward declaration of the focal function under test.
// This is a C function; ensure C linkage when calling from C++.
extern "C" z_size_t gzfread(void* buf, z_size_t size, z_size_t nitems, gzFile file);

// Lightweight, non-terminating test assertions (do not abort on failure)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            fprintf(stderr, "TEST FAIL: %s (line %d)\n", (msg), __LINE__); \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_EQ_UINT(a, b, msg) \
    do { \
        ++g_tests_run; \
        if (static_cast<uint64_t>(a) != static_cast<uint64_t>(b)) { \
            fprintf(stderr, "TEST FAIL: %s (line %d): %llu != %llu\n", (msg), __LINE__, (unsigned long long)(a), (unsigned long long)(b)); \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_MEM_EQ(a, b, len, msg) \
    do { \
        ++g_tests_run; \
        if (std::memcmp((a), (b), (len)) != 0) { \
            fprintf(stderr, "TEST FAIL: %s (line %d): memory content differs\n", (msg), __LINE__); \
            ++g_tests_failed; \
        } \
    } while (0)

// Helper: Create a tiny gzip file containing given content using zlib's gzwrite
static bool create_gzip_file_with_content(const std::string& path, const char* data, std::size_t len) {
    gzFile f = gzopen(path.c_str(), "wb");
    if (f == nullptr) {
        fprintf(stderr, "Failed to create gzip file: %s\n", path.c_str());
        return false;
    }
    int written = static_cast<int>(gzwrite(f, const_cast<char*>(data), static_cast<unsigned int>(len)));
    int close_err = gzclose(f);
    if (written != static_cast<int>(len) || close_err != Z_OK) {
        fprintf(stderr, "Failed to write gzip content to: %s\n", path.c_str());
        return false;
    }
    return true;
}

// Test 1: Null file should return 0 immediately (edge input handling)
static void test_null_file_returns_zero() {
    // Expect 0 when file is NULL
    const int dummy = 0;
    z_size_t result = gzfread((void*)nullptr, 1, 10, (gzFile)nullptr);
    EXPECT_TRUE(result == 0, "gzfread should return 0 when gzFile is NULL");
}

// Test 2: Normal read from a real gzip file should return the exact number of items requested
// We create a tiny gzip file containing "abcdef" and attempt to read 6 items of size 1.
static void test_read_whole_gzip_content() {
    // Prepare gzip file with known content
    char tmpname[L_tmpnam];
    if (tmpnam(tmpname) == nullptr) {
        fprintf(stderr, "Unable to generate temporary filename\n");
        EXPECT_TRUE(false, "tmpnam failed");
        return;
    }
    const char* content = "abcdef";
    if (!create_gzip_file_with_content(tmpname, content, 6)) {
        EXPECT_TRUE(false, "Failed to create gzip content file");
        return;
    }

    // Open for reading using gzfread
    gzFile gzf = gzopen(tmpname, "rb");
    EXPECT_TRUE(gzf != NULL, "gzopen for reading should succeed");

    char buf[6] = {0};
    z_size_t read_items = gzfread(buf, 1, 6, gzf);
    gzclose(gzf);

    EXPECT_EQ_UINT(read_items, 6, "gzfread should read 6 items of size 1");
    EXPECT_MEM_EQ(buf, content, 6, "Buffer content should match original data");

    // Cleanup
    std::remove(tmpname);
}

// Test 3: Overflow detection path: request does fit in size_t overflow; function should return 0
// We trigger the overflow by passing a huge size value paired with a valid gz file.
static void test_overflow_detection_returns_zero() {
    // Prepare gzip file with known content
    char tmpname[L_tmpnam];
    if (tmpnam(tmpname) == nullptr) {
        fprintf(stderr, "Unable to generate temporary filename\n");
        EXPECT_TRUE(false, "tmpnam failed");
        return;
    }
    const char* content = "abcdef";
    if (!create_gzip_file_with_content(tmpname, content, 6)) {
        EXPECT_TRUE(false, "Failed to create gzip content file");
        return;
    }

    gzFile gzf = gzopen(tmpname, "rb");
    EXPECT_TRUE(gzf != NULL, "gzopen for reading should succeed");

    // Force overflow: size != 0 and len/size != nitems after multiplication
    const z_size_t big = static_cast<z_size_t>(SIZE_MAX);
    const z_size_t nitems = 2;
    char dummy[1] = {0};
    z_size_t result = gzfread(dummy, big, nitems, gzf);
    gzclose(gzf);

    EXPECT_TRUE(result == 0, "gzfread should return 0 on overflow path");
    std::remove(tmpname);
}

// Test 4: Reading with size = 0 should immediately return 0 without consuming data
static void test_size_zero_returns_zero() {
    char tmpname[L_tmpnam];
    if (tmpnam(tmpname) == nullptr) {
        fprintf(stderr, "Unable to generate temporary filename\n");
        EXPECT_TRUE(false, "tmpnam failed");
        return;
    }
    const char* content = "abcdef";
    if (!create_gzip_file_with_content(tmpname, content, 6)) {
        EXPECT_TRUE(false, "Failed to create gzip content file");
        return;
    }

    gzFile gzf = gzopen(tmpname, "rb");
    EXPECT_TRUE(gzf != NULL, "gzopen for reading should succeed");

    char buf[6];
    z_size_t read_items = gzfread(buf, 0, 6, gzf);
    gzclose(gzf);
    std::memset(buf, 0, 6);

    EXPECT_TRUE(read_items == 0, "gzfread with size=0 should return 0");
    std::remove(tmpname);
}

// Test 5: End-of-file scenario: after reading all data, subsequent reads should return 0
static void test_end_of_file_returns_zero_on_second_read() {
    // Prepare gzip file with known content
    char tmpname[L_tmpnam];
    if (tmpnam(tmpname) == nullptr) {
        fprintf(stderr, "Unable to generate temporary filename\n");
        EXPECT_TRUE(false, "tmpnam failed");
        return;
    }
    const char* content = "abcdef";
    if (!create_gzip_file_with_content(tmpname, content, 6)) {
        EXPECT_TRUE(false, "Failed to create gzip content file");
        return;
    }

    gzFile gzf = gzopen(tmpname, "rb");
    EXPECT_TRUE(gzf != NULL, "gzopen for reading should succeed");

    char buf[6];
    z_size_t read_items = gzfread(buf, 1, 6, gzf);
    EXPECT_EQ_UINT(read_items, 6, "First read should return 6 items");
    // Second read beyond EOF
    z_size_t second_read = gzfread(buf, 1, 4, gzf);
    gzclose(gzf);

    EXPECT_TRUE(second_read == 0, "Second read after EOF should return 0");
    std::remove(tmpname);
}

// Run all tests and report final summary
static void run_all_tests() {
    printf("Starting gzfread unit test suite...\n");

    test_null_file_returns_zero();
    test_read_whole_gzip_content();
    test_overflow_detection_returns_zero();
    test_size_zero_returns_zero();
    test_end_of_file_returns_zero_on_second_read();

    printf("Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    if (g_tests_failed != 0) {
        printf("Some tests FAILED. Review the messages above for details.\n");
    } else {
        printf("All tests PASSED.\n");
    }
}

// Entry point
int main() {
    run_all_tests();
    return g_tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
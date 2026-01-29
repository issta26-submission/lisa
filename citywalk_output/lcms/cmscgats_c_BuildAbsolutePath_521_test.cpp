// Unit test suite for BuildAbsolutePath from cmscgats.c
// Target: C++11 test harness (no GTest) that exercises true/false branches.
// This test calls the external C function BuildAbsolutePath and verifies
// expected behavior for various scenarios.
// The tests are designed to compile in a typical POSIX environment where
// DIR_CHAR is '/' and absolute paths start with '/'. We rely on the actual
// implementation of isabsolutepath and DIR_CHAR from cmscgats.c/linkage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Forward declaration of the C function under test.
// We assume cmsBool is compatible with int in the linking unit (common in C projects).
extern "C" int BuildAbsolutePath(const char *relPath, const char *basePath, char *buffer, unsigned int MaxLen);

// Lightweight test framework (non-terminating assertions)
static int gTotalTests = 0;
static int gFailedTests = 0;

// Simple assertion macros that report failures but do not abort the test run.
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (Line " << __LINE__ << ")\n"; \
        ++gFailedTests; \
    } \
    ++gTotalTests; \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "[FAIL] " << (msg) << " (Line " << __LINE__ << "): " \
                  << "expected " << (b) << ", got " << (a) << "\n"; \
        ++gFailedTests; \
    } \
    ++gTotalTests; \
} while (0)

// Helper to reset test counters (optional)
static void ResetTestCounters() {
    gTotalTests = 0;
    gFailedTests = 0;
}

// Test 1: Absolute path should be copied into buffer unchanged and return TRUE (1)
bool test_absolute_path_copies_input()
{
    const char* absPath = "/home/user/project/file.txt";
    const char* basePath = "/should/not/be/useful";
    char buffer[256];
    unsigned int MaxLen = 256;

    int ret = BuildAbsolutePath(absPath, basePath, buffer, MaxLen);
    EXPECT_EQ(ret, 1, "Absolute path should cause TRUE return");
    EXPECT_EQ(std::strcmp(buffer, absPath), 0, "Buffer should contain the exact absolute path");
    return true;
}

// Test 2: Relative path should be appended to the directory portion of basePath
// Result should be basePath + '/' + relPath (assuming DIR_CHAR == '/')
bool test_relative_path_builds_from_base()
{
    const char* relPath = "subdir/file.txt";
    const char* basePath = "/base/dir";
    char buffer[256];
    unsigned int MaxLen = 256;

    int ret = BuildAbsolutePath(relPath, basePath, buffer, MaxLen);
    EXPECT_EQ(ret, 1, "Relative path should be resolved to absolute path and return TRUE");
    const char* expected = "/base/dir/subdir/file.txt";
    EXPECT_EQ(std::strcmp(buffer, expected), 0, "Buffer should contain basePath + '/' + relPath");
    return true;
}

// Test 3: No separator in basePath should cause failure (tail == NULL)
bool test_no_separator_in_basepath_returns_false()
{
    const char* relPath = "dir/file.txt";
    const char* basePath = "basepathwithoutsep"; // no DIR_CHAR
    char buffer[256];
    unsigned int MaxLen = 256;

    int ret = BuildAbsolutePath(relPath, basePath, buffer, MaxLen);
    EXPECT_EQ(ret, 0, "No separator in basePath should result in FALSE return");
    // Buffer content is unspecified on failure; we avoid asserting it.
    return true;
}

// Test 4: len >= MaxLen should cause failure (not enough room to store path portion)
bool test_len_exceeds_maxlen_causes_false()
{
    // A long base path that ensures tail - buffer >= MaxLen
    const char* relPath = "file.txt";
    const char* basePath = "/this/is/a/very/long/path/that/definitely/exceeds/maxlen";
    char buffer[64];
    unsigned int MaxLen = 16; // intentionally small to trigger length check

    int ret = BuildAbsolutePath(relPath, basePath, buffer, MaxLen);
    EXPECT_EQ(ret, 0, "len(tail - buffer) >= MaxLen should cause FALSE return");
    return true;
}

// Test 5: Absolute path truncated when MaxLen is smaller than path length
bool test_absolute_path_truncated_when_maxlen_small()
{
    // Very long absolute path to force truncation
    const char* absPathLong = "/abcdefghijklmnopqrstuvwxyz0123456789";
    const char* basePath = "/base/path"; // irrelevant for absolute path case
    char buffer[8]; // Intentionally small to force truncation
    unsigned int MaxLen = sizeof(buffer); // 8

    int ret = BuildAbsolutePath(absPathLong, basePath, buffer, MaxLen);
    // For absolute path: function copies MaxLen bytes and sets last to 0.
    // Therefore the buffer should contain the first MaxLen-1 characters of absPathLong.
    const char* expected = absPathLong; // compare only first MaxLen-1 chars
    // Build expected string: first (MaxLen-1) chars of absPathLong
    char expectedTrunc[8];
    std::size_t take = MaxLen - 1;
    if (take > std::strlen(absPathLong)) take = std::strlen(absPathLong);
    std::memcpy(expectedTrunc, absPathLong, take);
    expectedTrunc[take] = '\0';

    EXPECT_EQ(ret, 1, "Absolute path should still return TRUE even when truncated");
    EXPECT_EQ(std::strcmp(buffer, expectedTrunc), 0, "Buffer should contain first MaxLen-1 chars of absolute path");
    return true;
}

int main()
{
    ResetTestCounters();

    // Run tests with explanatory comments
    // Test 1: Absolute path handling
    test_absolute_path_copies_input();

    // Test 2: Relative path resolution against basePath
    test_relative_path_builds_from_base();

    // Test 3: No separator in basePath leads to FALSE
    test_no_separator_in_basepath_returns_false();

    // Test 4: Length check leads to FALSE when MaxLen is too small
    test_len_exceeds_maxlen_causes_false();

    // Test 5: Absolute path truncation when buffer is too small
    test_absolute_path_truncated_when_maxlen_small();

    // Summary
    std::cout << "\nTest Summary: " << gTotalTests << " tests run, "
              << gFailedTests << " failures.\n";

    return (gFailedTests == 0) ? 0 : 1;
}
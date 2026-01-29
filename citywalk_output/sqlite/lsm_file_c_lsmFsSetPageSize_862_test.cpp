// Unit test suite for focal method: lsmFsSetPageSize
// Follows instructions to generate high-quality C++ tests without using GTest.
// The tests are written to be non-terminating (continue on failures) and print
// informative messages for debugging.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdint>


// Candidate Keywords extracted from the focal method and its dependencies:
// - FileSystem: the structure containing nPagesize and nCacheMax
// - lsmFsSetPageSize: the function under test
// - nPagesize: the page size in bytes to be set
// - nCacheMax: the maximum cache size derived as (2048*1024) / nPagesize
// - 2048*1024: a constant used to cap/cache arithmetic
// - integer division semantics for computing nCacheMax
// The following tests implement scenarios ensuring correct field updates for
// various inputs, including boundary-like values and a sequence of updates.

// Minimal FileSystem structure match for the focal function.
// In real project, this would come from the actual header (lsm_file.c/h).
struct FileSystem {
    int nPagesize;
    int nCacheMax;
};

// Declaration of the focal function under test with C linkage to match the
// actual implementation signature in lsm_file.c.
extern "C" void lsmFsSetPageSize(FileSystem *pFS, int nPgsz);

// Simple non-terminating test harness (no GTest/GMock usage).
static int g_failCount = 0;

// Lightweight assertion macro that prints a descriptive message on failure
// but does not terminate the test run.
#define EXPECT_EQ(a, b) do {                                  \
    auto _a = (a);                                              \
    auto _b = (b);                                              \
    if (!(_a == _b)) {                                         \
        fprintf(stderr, "Test failure at %s:%d - EXPECT_EQ failed: " \
                #a " (%lld) != " #b " (%lld)\n",               \
                __FILE__, __LINE__, (long long)_a, (long long)_b); \
        ++g_failCount;                                         \
    }                                                           \
} while(0)

// Test 1: Basic positive values
// - Set page size to 4096 bytes
// - Expect nPagesize updated to 4096
// - Expect nCacheMax = (2048*1024) / 4096
static void test_lsmFsSetPageSize_basic_values() {
    // Arrange
    FileSystem fs;
    fs.nPagesize = 1024; // initial value
    fs.nCacheMax = 0;

    // Act
    lsmFsSetPageSize(&fs, 4096);

    // Assert
    EXPECT_EQ(fs.nPagesize, 4096);
    const int kTotalCacheBytes = 2048 * 1024; // 2,097,152
    EXPECT_EQ(fs.nCacheMax, kTotalCacheBytes / 4096); // expected 512
}

// Test 2: Small page size
// - Set page size to 1 byte
// - Ensure correct nPagesize and computation of nCacheMax
static void test_lsmFsSetPageSize_small_pages() {
    // Arrange
    FileSystem fs;
    fs.nPagesize = 512;
    fs.nCacheMax = 0;

    // Act
    lsmFsSetPageSize(&fs, 1);

    // Assert
    EXPECT_EQ(fs.nPagesize, 1);
    const int kTotalCacheBytes = 2048 * 1024;
    EXPECT_EQ(fs.nCacheMax, kTotalCacheBytes / 1); // 2,097,152
}

// Test 3: Large page size
// - Set page size to a large value (65,536 bytes)
// - Validate computation of nCacheMax
static void test_lsmFsSetPageSize_large_pages() {
    // Arrange
    FileSystem fs;
    fs.nPagesize = 4096;
    fs.nCacheMax = 0;

    // Act
    lsmFsSetPageSize(&fs, 65536);

    // Assert
    EXPECT_EQ(fs.nPagesize, 65536);
    const int kTotalCacheBytes = 2048 * 1024;
    EXPECT_EQ(fs.nCacheMax, kTotalCacheBytes / 65536); // 32
}

// Test 4: Update sequence
// - Change page size multiple times and verify updated values after each call
static void test_lsmFsSetPageSize_update_sequence() {
    // Arrange
    FileSystem fs;
    fs.nPagesize = 4096;
    fs.nCacheMax = 0;

    // Act & Assert: first update
    lsmFsSetPageSize(&fs, 1024);
    EXPECT_EQ(fs.nPagesize, 1024);
    EXPECT_EQ(fs.nCacheMax, (2048*1024) / 1024); // 2048

    // Act & Assert: second update
    lsmFsSetPageSize(&fs, 256);
    EXPECT_EQ(fs.nPagesize, 256);
    EXPECT_EQ(fs.nCacheMax, (2048*1024) / 256); // 8192
}

// Helper to run all tests and report results
static void run_all_tests() {
    test_lsmFsSetPageSize_basic_values();
    test_lsmFsSetPageSize_small_pages();
    test_lsmFsSetPageSize_large_pages();
    test_lsmFsSetPageSize_update_sequence();

    if (g_failCount == 0) {
        printf("All tests passed.\n");
    } else {
        printf("%d test(s) failed.\n", g_failCount);
    }
}

// Main function: executes test suite as per domain knowledge guidance
int main() {
    // Run tests in a single process (no forking or external test runners)
    run_all_tests();
    return g_failCount;
}
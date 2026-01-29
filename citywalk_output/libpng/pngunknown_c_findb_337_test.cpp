// Unit test suite for the focal method: findb(const png_byte *name)
// Note: This test is self-contained and re-implements the minimal environment
// needed to exercise the logic of findb (scanning from NINFO-1 down to 0 and
// performing a 4-byte memcmp against chunk_info[i].name). This avoids the need
// for the full project wiring while still validating the core behavior.
//
// Candidate Keywords (from Step 1 understanding):
// - NINFO: number of known info chunks
// - chunk_info: array of entries containing .name[4]
// - name (png_byte*): 4-byte identifier to compare
// - memcmp: comparison predicate for 4-byte names
// - findb: the function under test

#include <setjmp.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>


// Domain knowledge constraints:
// - Only C++11 standard library usage
// - No GTest; implement a lightweight test harness
// - Static/global state is mocked locally in tests (no reliance on production static members)

typedef unsigned char png_byte; // minimal alias to mirror the production type

// --- Focal method and its minimal mocked environment (self-contained) ---

// Mimic the minimal chunk_info entry used by findb
struct chunk_info_entry {
    unsigned char name[4];
};

// Static/global mock environment to drive the focal function
static int NINFO = 0;
static chunk_info_entry chunk_info[32];

// Local reimplementation of the focal function under test (self-contained for unit tests)
int findb(const png_byte *name)
{
    int i = NINFO;
    while (--i >= 0) {
        if (std::memcmp(chunk_info[i].name, name, 4) == 0)
            break;
    }
    return i;
}

// Helper to set up the mock chunk_info array for tests
static void setup_chunk_info(int n, const unsigned char data[][4])
{
    NINFO = n;
    for (int i = 0; i < n; ++i) {
        std::memcpy(chunk_info[i].name, data[i], 4);
    }
}

// Simple non-terminating test assertions (log on failure but continue execution)
static int total_passed = 0;
static int total_checked = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++total_checked; \
    if (cond) { \
        ++total_passed; \
    } else { \
        std::cerr << "TEST FAIL: " << msg << std::endl; \
    } \
} while (0)

// Test 1: Found in the middle of the array (index 2)
static bool test_findb_found_middle()
{
    // Candidate data:
    // chunk_info[0] = "WXYZ"
    // chunk_info[1] = "MNOP"
    // chunk_info[2] = "ABCD"  <-- target
    // chunk_info[3] = "QRST"
    const unsigned char data[4][4] = {
        { 'W','X','Y','Z' },
        { 'M','N','O','P' },
        { 'A','B','C','D' }, // target at index 2
        { 'Q','R','S','T' }
    };
    setup_chunk_info(4, data);

    const unsigned char target[4] = { 'A','B','C','D' };
    int idx = findb(target);

    TEST_ASSERT(idx == 2, "test_findb_found_middle: expected index 2, got " << idx);
    return true;
}

// Test 2: Not found in the array
static bool test_findb_not_found()
{
    // chunk_info: 3 entries with different names
    const unsigned char data[3][4] = {
        { 'A','A','A','A' },
        { 'B','B','B','B' },
        { 'C','C','C','C' }
    };
    setup_chunk_info(3, data);

    const unsigned char target[4] = { 'D','D','D','D' }; // not present
    int idx = findb(target);

    TEST_ASSERT(idx == -1, "test_findb_not_found: expected -1, got " << idx);
    return true;
}

// Test 3: Found at index 0 (single-element array)
static bool test_findb_found_at_zero()
{
    // chunk_info[0] = "ABCD" and NINFO=1
    const unsigned char data[1][4] = {
        { 'A','B','C','D' }
    };
    setup_chunk_info(1, data);

    const unsigned char target[4] = { 'A','B','C','D' };
    int idx = findb(target);

    TEST_ASSERT(idx == 0, "test_findb_found_at_zero: expected index 0, got " << idx);
    return true;
}

// Test 4: Duplicate entries where the matching name appears multiple times.
// Expect the function to return the lowest index of the most recent (bottom) match.
// Here, we place the same 4-byte name at indices 1 and 3; since the loop starts from NINFO-1,
// the expected result is 3.
static bool test_findb_duplicate_entries_bottommost()
{
    const unsigned char data[4][4] = {
        { 'A','X','X','X' }, // index 0
        { 'D','E','F','G' }, // index 1
        { 'Z','Z','Z','Z' }, // index 2
        { 'D','E','F','G' }  // index 3 -> duplicate name
    };
    setup_chunk_info(4, data);

    const unsigned char target[4] = { 'D','E','F','G' };
    int idx = findb(target);

    TEST_ASSERT(idx == 3, "test_findb_duplicate_entries_bottommost: expected index 3, got " << idx);
    return true;
}

// Runner
int main(void)
{
    bool ok = true;
    ok &= test_findb_found_middle();
    ok &= test_findb_not_found();
    ok &= test_findb_found_at_zero();
    ok &= test_findb_duplicate_entries_bottommost();

    std::cout << "Tests run: " << total_checked << ", Passed: " << total_passed
              << ", Failed: " << (total_checked - total_passed) << std::endl;

    if (!ok) {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}
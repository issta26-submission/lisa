// Unit test suite for the focal method: zip64local_TmzDateToDosDate
// Approach:
// - Expose the static function by redefining the macro 'local' to nothing before including the implementation zip.c.
// - Then call zip64local_TmzDateToDosDate with crafted tm_zip inputs to validate the resulting DOS-date value.
// - Implement a small non-terminating assertion mechanism (not using GTest) to accumulate failures and print a summary.

#define local
#include <cstdio>
#include <cstring>
#include <string.h>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <zip.c>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>



// Minimal non-terminating test harness
static int g_failures = 0;
static int g_test_count = 0;

#define ASSERT_EQ(expected, actual, test_desc) do {                                      \
    ++g_test_count;                                                                         \
    if (static_cast<uint64_t>(static_cast<uint64_t>(actual)) != (uint64_t)(expected)) {  \
        g_failures++;                                                                      \
        printf("FAIL: %s | Expected: %llu, Actual: %llu\n", (test_desc),                   \
               (unsigned long long)(expected), (unsigned long long)(actual));              \
    } else {                                                                               \
        printf("PASS: %s\n", (test_desc));                                                 \
    }                                                                                        \
} while(0)

int main() {
    // Test 1: Year >= 1980 path (first branch taken)
    // Input: year = 1995, Jan 1 00:00:00
    // Expected: year reduced to 15; date_part = day + 32*(mon+1) + 512*year = 1 + 32*(1) + 512*15 = 7713
    // High part: 7713 << 16 = 505479168; Time part = 0
    {
        tm_zip ptm;
        std::memset(&ptm, 0, sizeof(ptm));
        ptm.tm_year = 1995;
        ptm.tm_mon  = 0;   // January
        ptm.tm_mday = 1;
        ptm.tm_hour = 0;
        ptm.tm_min  = 0;
        ptm.tm_sec  = 0;

        uLong result = zip64local_TmzDateToDosDate(&ptm);
        uint64_t actual = static_cast<uint64_t>(result);
        uint64_t expected = 505479168ULL;
        ASSERT_EQ(expected, actual, "Test1: 1995-01-01 00:00:00 (year>=1980 branch)");
    }

    // Test 2: Year >= 1980 path (first branch taken)
    // Input: year = 1995, Jan 2 12:34:56
    // Expected: date_part = 2 + 32*(1) + 512*15 = 7714 -> high = 7714 << 16 = 505544704
    // Time part: sec/2 = 28; min part = 32*34 = 1088; hour part = 2048*12 = 24576; time = 28+1088+24576 = 25692;
    // Result = 505544704 + 25692 = 505570396
    {
        tm_zip ptm;
        std::memset(&ptm, 0, sizeof(ptm));
        ptm.tm_year = 1995;
        ptm.tm_mon  = 0;   // January
        ptm.tm_mday = 2;
        ptm.tm_hour = 12;
        ptm.tm_min  = 34;
        ptm.tm_sec  = 56;

        uLong result = zip64local_TmzDateToDosDate(&ptm);
        uint64_t actual = static_cast<uint64_t>(result);
        uint64_t expected = 505570396ULL;
        ASSERT_EQ(expected, actual, "Test2: 1995-01-02 12:34:56 (year>=1980 branch)");
    }

    // Test 3: Two-digit year path (else-if branch) -> year >= 80 path triggers subtraction by 80
    // Input: year = 85 (interpreted as 1985), Jan 1 00:00:00
    // year -> 85 - 80 = 5
    // date_part = 1 + 32*(1) + 512*5 = 1 + 32 + 2560 = 2593
    // high = 2593 << 16 = 169,934,848; time = 0
    {
        tm_zip ptm;
        std::memset(&ptm, 0, sizeof(ptm));
        ptm.tm_year = 85;
        ptm.tm_mon  = 0;
        ptm.tm_mday = 1;
        ptm.tm_hour = 0;
        ptm.tm_min  = 0;
        ptm.tm_sec  = 0;

        uLong result = zip64local_TmzDateToDosDate(&ptm);
        uint64_t actual = static_cast<uint64_t>(result);
        uint64_t expected = 169934848ULL;
        ASSERT_EQ(expected, actual, "Test3: 85 => 1985-01-01 00:00:00 (two-digit year path)");
    }

    // Test 4: Year path where year yields 79 (neither branch taken, i.e., year < 80)
    // Input: year = 79, Jan 1 00:00:00
    // year remains 79; date_part = 1 + 32*(1) + 512*79 = 40481
    // high = 40481 << 16 = 2,652,962,816; time = 0
    {
        tm_zip ptm;
        std::memset(&ptm, 0, sizeof(ptm));
        ptm.tm_year = 79;
        ptm.tm_mon  = 0;
        ptm.tm_mday = 1;
        ptm.tm_hour = 0;
        ptm.tm_min  = 0;
        ptm.tm_sec  = 0;

        uLong result = zip64local_TmzDateToDosDate(&ptm);
        uint64_t actual = static_cast<uint64_t>(result);
        uint64_t expected = 2652962816ULL;
        ASSERT_EQ(expected, actual, "Test4: year<80 and <1980 path (year stays 79)");
    }

    // Test 5: Boundary case for year exactly 1980
    // Input: year = 1980, Jan 1 00:00:00
    // year -> 0; date_part = 1 + 32*(1) + 512*0 = 33
    // high = 33 << 16 = 2,162,688; time = 0
    {
        tm_zip ptm;
        std::memset(&ptm, 0, sizeof(ptm));
        ptm.tm_year = 1980;
        ptm.tm_mon  = 0;
        ptm.tm_mday = 1;
        ptm.tm_hour = 0;
        ptm.tm_min  = 0;
        ptm.tm_sec  = 0;

        uLong result = zip64local_TmzDateToDosDate(&ptm);
        uint64_t actual = static_cast<uint64_t>(result);
        uint64_t expected = 2162688ULL;
        ASSERT_EQ(expected, actual, "Test5: 1980-01-01 00:00:00 (year==1980 boundary)");
    }

    // Summary
    if (g_failures == 0) {
        printf("All tests PASSED (%d/%d).\n", g_test_count - g_failures, g_test_count);
        return 0;
    } else {
        printf("Total failures: %d out of %d tests.\n", g_failures, g_test_count);
        return 1;
    }
}
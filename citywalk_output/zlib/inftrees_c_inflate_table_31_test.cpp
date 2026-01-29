/*
Unit test suite for inflate_table inftrees.c
- Uses a lightweight self-contained test harness (no GoogleTest).
- Exercises multiple branches of inflate_table:
  1) max == 0 path (test_max_zero)
  2) successful path for a minimal LENS table (test_successful_lens)
  3) error path when a non-trivial length distribution would be over-subscribed (test_error_over_subscribed)
- Relies on project headers (inftrees.h, zutil.h) to obtain exact types and constants.
- Does not rely on private state; uses only public function interface.
- Outputs test results to std::cerr and returns non-zero on failure.
*/

#include <cstring>
#include <cstdint>
#include <iostream>
#include <inftrees.h>
#include <zutil.h>


// Forward-declare C APIs (provided by the project)
extern "C" {
}

// Lightweight test assertion helpers (non-terminating)
static int g_test_failures = 0;

#define TEST_EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

#define TEST_EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " (expected " << (b) << ", got " << (a) \
                  << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

// Helper: zero-initialize a buffer of 'count' elements
template <typename T>
static void zero_buffer(T* buf, size_t count) {
    std::memset(buf, 0, count * sizeof(T));
}

/*
Test 1: max == 0 path
- Provide codes > 0 with all lens values == 0.
- inflate_table should place two invalid-code markers and return 0.
- Verifies: return value, bits == 1, and two table entries with op=64, bits=1, val=0.
*/
static void test_inflate_table_max_zero_path() {
    const unsigned codes = 3;
    unsigned short lens[codes];
    zero_buffer(lens, codes);

    // Prepare a table buffer and a pointer-to-table (as expected by inflate_table)
    const size_t TABLE_SIZE = 4;
    code table_buf[TABLE_SIZE];
    zero_buffer(table_buf, TABLE_SIZE);

    code* table_start = table_buf;
    code* table_ptr = table_start;
    code** table_pp = &table_ptr; // code FAR * FAR *table

    unsigned bits = 0;
    unsigned short work[32]; // workspace used by inflate_table

    // Call: max==0 branch should trigger irrespective of type (we pass CODES)
    int ret = inflate_table(CODES, lens, codes, table_pp, &bits, work);

    TEST_EXPECT_EQ(ret, 0, "inflate_table should return 0 when max==0");
    TEST_EXPECT_EQ(bits, 1u, "bits should be set to 1 in max==0 path");

    // The function should have consumed 2 table entries (two identical 'here' markers)
    TEST_EXPECT_EQ(static_cast<size_t>(table_ptr - table_start), 2u,
                   "table pointer should advance by 2 after max==0 path");

    // Validate the two markers
    TEST_EXPECT_TRUE(table_start[0].op == 64u && table_start[0].bits == 1u && table_start[0].val == 0u,
                     "First marker should be invalid-code marker (op=64, bits=1, val=0)");
    TEST_EXPECT_TRUE(table_start[1].op == 64u && table_start[1].bits == 1u && table_start[1].val == 0u,
                     "Second marker should be invalid-code marker (op=64, bits=1, val=0)");
}

/*
Test 2: Successful path for minimal LENS table
- codes = 1; lens[0] = 1
- Expect a successful decoding table build for a single symbol.
- Verifies: ret == 0, bits == 1 (root=1), and first table entry matches expected 'here' for a simple case.
*/
static void test_inflate_table_successful_lens_minimal() {
    const unsigned codes = 1;
    unsigned short lens[codes];
    lens[0] = 1; // minimal non-zero length

    // Table and workspace
    const size_t TABLE_SIZE = 16;
    code table_buf[TABLE_SIZE];
    zero_buffer(table_buf, TABLE_SIZE);

    code* table_start = table_buf;
    code* table_ptr = table_start;
    code** table_pp = &table_ptr;

    unsigned bits = 0; // root will be adjusted by algorithm
    unsigned short work[64];
    int ret = inflate_table(LENS, lens, codes, table_pp, &bits, work);

    TEST_EXPECT_EQ(ret, 0, "inflate_table should return 0 on successful lens table generation");
    TEST_EXPECT_EQ(bits, 1u, "root bits should equal 1 for this minimal case");

    // The root table should have been populated; two codes entries should be used (2 entries consumed)
    TEST_EXPECT_EQ(static_cast<size_t>(table_ptr - table_start), 2u,
                   "table pointer should advance by 2 for root table of size 1 (root=1)");

    // Validate the first entry (the 'here' entry for the single symbol)
    TEST_EXPECT_TRUE(table_start[0].op == 0u && table_start[0].bits == 1u && table_start[0].val == 0u,
                     "First decoding entry should represent a direct symbol with op=0, bits=1, val=0");
}

/*
Test 3: Over-subscribed path (error condition)
- codes = 1; lens[0] = 2 creates a non-trivial distribution with max=2
- Expect the function to return -1 due to an incomplete/over-subscribed code length set.
*/
static void test_inflate_table_error_over_subscribed() {
    const unsigned codes = 1;
    unsigned short lens[codes];
    lens[0] = 2; // max length greater than 1 to trigger over-subscription path

    // Prepare buffers
    const size_t TABLE_SIZE = 64;
    code table_buf[TABLE_SIZE];
    zero_buffer(table_buf, TABLE_SIZE);

    code* table_start = table_buf;
    code* table_ptr = table_start;
    code** table_pp = &table_ptr;

    unsigned bits = 0;
    unsigned short work[32];
    int ret = inflate_table(LENS, lens, codes, table_pp, &bits, work);

    TEST_EXPECT_EQ(ret, -1, "inflate_table should return -1 for over-subscribed/invalid lengths");
}

int main() {
    // Run tests
    test_inflate_table_max_zero_path();
    test_inflate_table_successful_lens_minimal();
    test_inflate_table_error_over_subscribed();

    if (g_test_failures != 0) {
        std::cerr << "TOTAL TEST FAILURES: " << g_test_failures << std::endl;
        return 1;
    } else {
        std::cerr << "ALL TESTS PASSED" << std::endl;
        return 0;
    }
}
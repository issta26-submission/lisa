// Unit test suite for get_crc_table from crc32.c
// This test suite is designed to be compiled alongside crc32.c (C source).
// It uses a lightweight, non-terminating test harness (assertions do not abort on first failure) and prints a summary.
// The tests assume get_crc_table returns a pointer to the 256-entry CRC32 table (32-bit values).
// The test is written in C++11, without GoogleTest, as requested.

#include <stdatomic.h>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <zutil.h>
#include <iomanip>


// -----------------------------------------------------------------------------------
// Candidate Keywords (Step 1: Program Understanding)
// The following keywords represent the core dependencies and relevant components
// that influence get_crc_table behavior in crc32.c:
//
// - get_crc_table: the focal method under test
// - DYNAMIC_CRC_TABLE: when defined, table is generated lazily via make_crc_table
// - crc_table: the internal 256-entry CRC table
// - make_crc_table: function that populates crc_table when DYNAMIC_CRC_TABLE is defined
// - once/made: synchronization primitive used to ensure make_crc_table runs once
// - z_crc_t: CRC type used for table entries (32-bit typically)
// - FAR: pointer qualifier macro (often defined as empty in many builds)
// - ZEXPORT: calling convention macro (often defined as empty in most builds)
// - POLY (0xedb88320): CRC-32 polynomial used for table generation
// - crc32, crc32_z: CRC computation routines (not directly tested here)
// -----------------------------------------------------------------------------------

// Step 2: Declarative bridge to the focal C function
// We declare the function as extern "C" to match C linkage from the crc32.c file.
// We assume z_crc_t corresponds to 32-bit CRC values; thus, we map the return type
// to a pointer to uint32_t for the test. If the actual type differs, this test
// may need adjustment, but 32-bit CRC tables are standard for CRC-32.
extern "C" const uint32_t* get_crc_table(void);

// Lightweight test harness (non-terminating assertions)
static int g_passed = 0;
static int g_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (cond) { \
        ++g_passed; \
    } else { \
        ++g_failed; \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")\n"; \
    } \
} while (0)

static void test_pointer_not_null()
{
    // Existence and non-nullness of the CRC table pointer
    const uint32_t* tbl = get_crc_table();
    TEST_ASSERT(tbl != nullptr, "get_crc_table() should return a non-null pointer");
}

static void test_pointer_identity_across_calls()
{
    // The CRC table should be cached and consistently returned across calls
    const uint32_t* tbl1 = get_crc_table();
    const uint32_t* tbl2 = get_crc_table();
    TEST_ASSERT(tbl1 != nullptr && tbl2 != nullptr, "get_crc_table() must return non-null pointers on both calls");
    TEST_ASSERT(tbl1 == tbl2, "get_crc_table() should return the same pointer on consecutive calls");
}

static void test_known_table_values()
{
    // Validate a subset of known CRC32 table values.
    // The standard CRC-32 (IEEE 802.3) table begins with expected values:
    // tbl[0] = 0x00000000, tbl[1] = 0x77073096, tbl[2] = 0xEE0E612C, tbl[3] = 0x990951BA, tbl[4] = 0x076DC419
    // These are stable across environments that implement the standard table generation.
    const uint32_t* tbl = get_crc_table();
    TEST_ASSERT(tbl != nullptr, "get_crc_table() should return a valid table pointer before value checks");

    // Check the first few standard values
    TEST_ASSERT(tbl[0] == 0x00000000u, "tbl[0] should be 0x00000000");
    TEST_ASSERT(tbl[1] == 0x77073096u, "tbl[1] should be 0x77073096");
    TEST_ASSERT(tbl[2] == 0xEE0E612Cu, "tbl[2] should be 0xEE0E612C");
    TEST_ASSERT(tbl[3] == 0x990951BAu, "tbl[3] should be 0x990951BA");
    TEST_ASSERT(tbl[4] == 0x076DC419u, "tbl[4] should be 0x076DC419");
}

// Step 3: Test case refinement
// The following main function drives the tests and reports a summary.
// It is written to maximize code coverage and to avoid terminating on first failure.
// It relies only on standard library facilities.
int main()
{
    // Run individual tests
    test_pointer_not_null();
    test_pointer_identity_across_calls();
    test_known_table_values();

    // Summary
    std::cout << "\nCRC_TABLE GET_TEST SUMMARY:\n";
    std::cout << "Passed: " << g_passed << "\n";
    std::cout << "Failed: " << g_failed << "\n";

    // Return non-zero if any test failed
    return (g_failed > 0) ? 1 : 0;
}
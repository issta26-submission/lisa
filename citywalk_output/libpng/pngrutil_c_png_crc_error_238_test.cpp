// Self-contained unit test suite for the focal method png_crc_error.
// This test suite is designed to be compiled with a C++11 compiler without
// using GTest. It provides minimal stubs and a small test harness to exercise
// the different branches of the function's logic.
// Note: This is a self-contained reproduction of the relevant logic to enable
// isolated unit testing without depending on the full libpng build system.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// -----------------------------------------------------------------------------
// Minimal type and macro definitions to mimic the internal environment used by
// png_crc_error (as observed in the provided source).
// These are stand-ins for the private internal libpng types/macros used by the
// focal method and are tailored to drive unit test scenarios.
// -----------------------------------------------------------------------------

typedef unsigned char  png_byte;
typedef unsigned int   png_uint_32;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;
typedef struct png_struct* png_structrp;

// Forward declaration for the fake I/O state macros (enabled in this test).
#define PNG_IO_STATE_SUPPORTED 1
#define PNG_IO_READING 1
#define PNG_IO_CHUNK_CRC 2

// Flags and masks (subset necessary for the test cases)
#define PNG_FLAG_CRC_ANCILLARY_USE        1
#define PNG_FLAG_CRC_ANCILLARY_NOWARN     2
#define PNG_FLAG_CRC_ANCILLARY_MASK       (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN)
#define PNG_FLAG_CRC_CRITICAL_IGNORE      4

// Ancillary chunk predicate: treat chunk_name bit 0x20 as ancillary indicator.
#define PNG_CHUNK_ANCILLARY(name) (((name) & 0x20) != 0)

// -----------------------------------------------------------------------------
// Global state and function stubs to emulate the environment for png_crc_error.
// The test harness provides controlled behavior for reading CRC bytes and
// interpreting them as a 32-bit unsigned integer in network byte order.
// -----------------------------------------------------------------------------

// A simple per-struct storage for the next 4 CRC bytes to be "read" by the test's
// fake png_read_data(). This allows us to prescribe the CRC value used by
// png_crc_error without implementing real I/O.
struct PngStruct {
    uint32_t chunk_name;
    uint32_t flags;
    uint32_t crc;
    int io_state;
    uint8_t next_crc_bytes[4]; // CRC bytes to feed into png_read_data
};

// The function used by png_crc_error to read data. It is a C-linkage function
// so that the focal function can call it as if it were the library's function.
// It simply copies the prescribed 4 CRC bytes from the PngStruct into the
// provided buffer.
extern "C" void png_read_data(png_structrp png_ptr, png_bytep data, uint32_t length) {
    (void)length; // length is expected to be 4 in our usage
    if (png_ptr && data) {
        memcpy(data, static_cast<void*>(((PngStruct*)png_ptr)->next_crc_bytes),
               4);
    } else {
        // Defensive: if null pointers are passed, fill with zeros
        std::memset(data, 0, 4);
    }
}

// Helper to convert a 4-byte big-endian buffer to a 32-bit unsigned integer.
extern "C" png_uint_32 png_get_uint_32(png_const_bytep buf) {
    // Network order: big-endian
    return (static_cast<png_uint_32>(buf[0]) << 24) |
           (static_cast<png_uint_32>(buf[1]) << 16) |
           (static_cast<png_uint_32>(buf[2]) << 8)  |
           (static_cast<png_uint_32>(buf[3]));
}

// Forward declaration of the focal function under test.
// In this self-contained test, we implement a faithful replica of the logic
// found in the provided snippet to ensure the unit tests are self-sufficient.
// The real project would link against libpng's png_crc_error; here we emulate
// behavior directly to exercise the decision branches.
int png_crc_error(png_structrp png_ptr, int handle_as_ancillary);

// Minimal implementation of the focal method (self-contained replica)
int png_crc_error(png_structrp png_ptr, int handle_as_ancillary)
{
    png_byte crc_bytes[4];
    png_uint_32 crc;
    int need_crc = 1;

    if (handle_as_ancillary || PNG_CHUNK_ANCILLARY(png_ptr->chunk_name) != 0)
    {
        if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_MASK) ==
            (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN))
            need_crc = 0;
    }
    else /* critical */
    {
        if ((png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE) != 0)
            need_crc = 0;
    }
#ifdef PNG_IO_STATE_SUPPORTED
    png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_CRC;
#endif
    png_read_data(png_ptr, crc_bytes, 4);
    if (need_crc != 0)
    {
        crc = png_get_uint_32(crc_bytes);
        return crc != png_ptr->crc;
    }
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Lightweight test harness
// -----------------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_failed = 0;

// Simple non-terminating assertion helper
#define TST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "TEST FAIL: " << msg << std::endl; \
    } else { \
        std::cout << "TEST PASS: " << msg << std::endl; \
    } \
} while(0)

// Helper: run a test function and track its result
#define RUN_TEST(func) do { \
    bool ok = (func)(); \
    if (!ok) { g_tests_failed++; } \
} while(0)

// Utility to construct a PngStruct instance with provided fields.
static PngStruct make_ptr(uint32_t chunk_name, uint32_t flags, uint32_t crc,
                          const uint8_t next_bytes[4])
{
    PngStruct p;
    p.chunk_name = chunk_name;
    p.flags = flags;
    p.crc = crc;
    p.io_state = 0;
    std::memcpy(p.next_crc_bytes, next_bytes, 4);
    return p;
}

// Test 1: Ancillary path triggered by handle_as_ancillary with exact ancillary mask -> need_crc == 0
static bool test_case_ancillary_handle_true_need_crc0() {
    // ancillary chunk (bit 0x20 set), flags mask matches USE|NOWARN, and handle_as_ancillary = true
    uint8_t next[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    PngStruct ptr = make_ptr(0x20, // ancillary chunk (bit 0x20)
                             PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN,
                             0xDEADBEEF,
                             next);
    int res = png_crc_error(&ptr, 1); // handle_as_ancillary = 1
    // Expect no CRC check to be performed, so return should be 0
    return (res == 0);
}

// Test 2a: Ancillary path (chunk ancillary) but mask not exactly USE|NOWARN -> need_crc stays 1
static bool test_case_ancillary_mask_not_equal_crc_check_matches() {
    // ancillary chunk, mask not equal to USE|NOWARN, next CRC matches ptr.crc
    uint8_t next[4] = {0x01, 0x02, 0x03, 0x04};
    PngStruct ptr = make_ptr(0x20,     // ancillary chunk
                             0x0,        // no ancillary mask set
                             0x01020304, // pretend current CRC
                             next);
    int res = png_crc_error(&ptr, 0); // handle_as_ancillary = 0 but chunk ancillary
    // Expect CRCs to match -> no error (false)
    return (res == 0);
}

// Test 2b: Ancillary path (chunk ancillary) with mask not equal and CRC mismatch
static bool test_case_ancillary_mask_not_equal_crc_mismatch() {
    // ancillary chunk, mask not equal, next CRC different from ptr.crc
    uint8_t next[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    PngStruct ptr = make_ptr(0x20,     // ancillary chunk
                             0x0,        // no ancillary mask set
                             0x01020304, // pretend current CRC
                             next);
    int res = png_crc_error(&ptr, 0); // handle_as_ancillary = 0
    // Expect mismatch -> return true (1)
    return (res == 1);
}

// Test 3: Critical path with IGNORE flag set -> need_crc should be 0 regardless of CRC read
static bool test_case_critical_ignore() {
    // non-ancillary chunk, CRC ignored
    uint8_t next[4] = {0x12, 0x34, 0x56, 0x78};
    PngStruct ptr = make_ptr(0x00,                 // non-ancillary
                             PNG_FLAG_CRC_CRITICAL_IGNORE,
                             0xDEADBEEF,
                             next);
    int res = png_crc_error(&ptr, 0); // critical ignore
    // Expect 0 because we ignore CRC
    return (res == 0);
}

// Test 4a: Critical default path with exact CRC match (no ignore)
static bool test_case_critical_default_match() {
    // non-ancillary chunk, no ignore
    uint8_t next[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    PngStruct ptr = make_ptr(0x00,        // non-ancillary
                             0x0,         // no special flags
                             0xDEADBEEF,  // current CRC
                             next);
    int res = png_crc_error(&ptr, 0);
    // Expect match -> return 0 (no error)
    return (res == 0);
}

// Test 4b: Critical default path with mismatch CRC
static bool test_case_critical_default_mismatch() {
    // non-ancillary chunk, no ignore, mismatch CRC
    uint8_t next[4] = {0x01, 0x02, 0x03, 0x04};
    PngStruct ptr = make_ptr(0x00,        // non-ancillary
                             0x0,         // no special flags
                             0xAABBCCDD,  // current CRC
                             next);
    int res = png_crc_error(&ptr, 0);
    // Expect mismatch -> return 1
    return (res == 1);
}

// -----------------------------------------------------------------------------
// Main function: execute all tests and print a summary.
// -----------------------------------------------------------------------------

int main() {
    std::cout << "Starting unit tests for png_crc_error (self-contained)\n";

    // Run each test and accumulate results
    RUN_TEST(test_case_ancillary_handle_true_need_crc0);
    RUN_TEST(test_case_ancillary_mask_not_equal_crc_check_matches);
    RUN_TEST(test_case_ancillary_mask_not_equal_crc_mismatch);
    RUN_TEST(test_case_critical_ignore);
    RUN_TEST(test_case_critical_default_match);
    RUN_TEST(test_case_critical_default_mismatch);

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed << "\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
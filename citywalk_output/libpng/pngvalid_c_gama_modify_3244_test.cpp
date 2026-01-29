/*
   C++11 test suite for the focal C function:
   int gama_modify(png_modifier *pm, png_modification *me, int add)

   This test does not depend on GoogleTest. It uses a small, self-contained
   harness with non-terminating assertions to maximize code coverage.

   Notes:
   - The tests rely on libpng-like types (png_modifier, png_modification, CHUNK_gAMA)
     and on the gamma modification initialization function:
       gama_modification_init(gama_modification *me, png_modifier *pm, double gammad)
   - We verify:
     1) Normal behavior with gamma = 2.2 (expected gamma encoding = 220000).
     2) That the add parameter is ignored (UNUSED(add)).
     3) Behavior for a different gamma value (1.0 -> 100000).
   - The gAMA chunk bytes are written as a 4-byte big-endian integer after a 4-byte length
     prefix and the 4-byte chunk type, consistent with PNG chunk encoding.

   Build note:
   - Compile with a C/C++ linker setup that includes the implementation file containing
     gamma_modify and gamma_modification_init (e.g., pngvalid.c or the corresponding
     translation unit in your project) and the libpng headers.
   - This test uses only the C API exposed by the project (no GTest), and uses
     std::printf/std::cout for reporting.
*/

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Attempt to include the C headers used by the focal code.
// If your environment places png.h in a different path, adjust accordingly.
extern "C" {

// Prototypes for the focal API (to ensure correct linkage in C++)
int gama_modify(png_modifier *pm, png_modification *me, int add);
void gama_modification_init(struct _gama_modification *me, png_modifier *pm, double gammad);
// If the exact type name differs (e.g., gama_modification vs _gama_modification),
// adjust accordingly in your environment. The tests below use the types as defined
// by your png.h and the existing initialization function names.
}

// Local test harness state
static int g_failures = 0;
static int g_tests_run = 0;

// Simple non-terminating assertion helper
#define ASSERT_EQ_INT(expected, actual, msg) do { \
    ++g_tests_run; \
    if ((expected) != (actual)) { \
        std::printf("ASSERT_FAIL: %s (expected %d, got %d)\n", (msg), (int)(expected), (int)(actual)); \
        ++g_failures; \
    } else { \
        std::printf("ASSERT_PASS: %s\n", (msg)); \
    } \
} while(0)

#define ASSERT_EQ_UINT32(expected, actual, msg) do { \
    ++g_tests_run; \
    if ((uint32_t)(expected) != (uint32_t)(actual)) { \
        std::printf("ASSERT_FAIL: %s (expected 0x%08x, got 0x%08x)\n", (msg), (uint32_t)(expected), (uint32_t)(actual)); \
        ++g_failures; \
    } else { \
        std::printf("ASSERT_PASS: %s\n", (msg)); \
    } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::printf("ASSERT_FAIL: %s (condition is false)\n", (msg)); \
        ++g_failures; \
    } else { \
        std::printf("ASSERT_PASS: %s\n", (msg)); \
    } \
} while(0)

// Helper to read a 32-bit unsigned int from a 4-byte big-endian array
static uint32_t read_be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

// Test 1: Basic gamma_modify with gamma = 2.2 should write gAMA chunk with 220000
static void test_gama_modify_basic_two_point_two() {
    // Prepare a small buffer to hold the PNG chunk data written by gama_modify
    // The function writes 12 bytes: length (4) + chunk type (4) + gamma (4)
    unsigned char buffer[12];
    std::memset(buffer, 0, sizeof(buffer));

    // Prepare png_modifier with the buffer
    png_modifier pm;
    std::memset(&pm, 0, sizeof(pm));
    // The actual field name is likely 'buffer' or similar; we rely on the project header.
    // If your environment uses a different member name, adjust accordingly.
    pm.buffer = buffer;

    // Prepare gamma modification: use the helper to initialize to 2.2 (which should map to 220000)
    gama_modification gm;
    std::memset(&gm, 0, sizeof(gm)); // ensure deterministic initialization
    gama_modification_init(&gm, &pm, 2.2);

    // Call the function under test
    int ret = gama_modify(&pm, (png_modification *)&gm, 0);

    // Validate return value
    ASSERT_EQ_INT(1, ret, "gama_modify should return 1 on success (gamma 2.2)");

    // Validate buffer content
    // Expect 12 bytes: [0..3] length = 4, [4..7] chunk type = CHUNK_gAMA, [8..11] gamma value
    uint32_t length = read_be32(buffer);
    ASSERT_EQ_UINT32(4, length, "Buffer length should be 4 for the gAMA chunk payload");

    uint32_t type = read_be32(buffer + 4);
    ASSERT_EQ_UINT32(CHUNK_gAMA, type, "Chunk type should be CHUNK_gAMA");

    uint32_t gamma_written = read_be32(buffer + 8);
    // gamma_written should equal gm.gamma (likely 220000 for gamma 2.2)
    ASSERT_EQ_UINT32(gm.gamma, gamma_written, "Gamma value written to buffer should match gm.gamma (2.2 -> 220000)");
}

// Test 2: add parameter is unused (UNUSED(add)); ensure buffer remains the same
static void test_gama_modify_ignores_add_flag() {
    unsigned char buffer[12];
    std::memset(buffer, 0, sizeof(buffer));

    png_modifier pm;
    std::memset(&pm, 0, sizeof(pm));
    pm.buffer = buffer;

    gama_modification gm;
    std::memset(&gm, 0, sizeof(gm));
    gama_modification_init(&gm, &pm, 2.2); // same as Test 1 to keep consistency

    // Call with a non-zero add value
    int ret = gama_modify(&pm, (png_modification *)&gm, 12345);

    // Validate return value
    ASSERT_EQ_INT(1, ret, "gama_modify should return 1 even when add != 0 (unused parameter)");

    // Validate that content is identical to Test 1
    uint32_t length = read_be32(buffer);
    ASSERT_EQ_UINT32(4, length, "Buffer length should be 4 for the gAMA chunk payload (IGNORE add)");

    uint32_t type = read_be32(buffer + 4);
    ASSERT_EQ_UINT32(CHUNK_gAMA, type, "Chunk type should be CHUNK_gAMA (IGNORES add)");

    uint32_t gamma_written = read_be32(buffer + 8);
    ASSERT_EQ_UINT32(gm.gamma, gamma_written, "Gamma value should remain gm.gamma when add is non-zero");
}

// Test 3: Basic gamma_modify with gamma = 1.0 should write gAMA chunk with 100000
static void test_gama_modify_basic_one_point_zero() {
    unsigned char buffer[12];
    std::memset(buffer, 0, sizeof(buffer));

    png_modifier pm;
    std::memset(&pm, 0, sizeof(pm));
    pm.buffer = buffer;

    gama_modification gm;
    std::memset(&gm, 0, sizeof(gm));
    gama_modification_init(&gm, &pm, 1.0); // gamma = 1.0 -> 100000

    int ret = gama_modify(&pm, (png_modification *)&gm, 0);

    // Validate
    ASSERT_EQ_INT(1, ret, "gama_modify should return 1 for gamma 1.0");

    uint32_t length = read_be32(buffer);
    ASSERT_EQ_UINT32(4, length, "Buffer length should be 4 for the gAMA chunk payload (gamma 1.0)");

    uint32_t type = read_be32(buffer + 4);
    ASSERT_EQ_UINT32(CHUNK_gAMA, type, "Chunk type should be CHUNK_gAMA for gamma 1.0");

    uint32_t gamma_written = read_be32(buffer + 8);
    ASSERT_EQ_UINT32(gm.gamma, gamma_written, "Gamma value should be 100000 for gamma 1.0");
}

// Main function to execute tests
int main(void) {
    std::printf("Starting test suite for gama_modify...\n");

    test_gama_modify_basic_two_point_two();
    test_gama_modify_ignores_add_flag();
    test_gama_modify_basic_one_point_zero();

    std::printf("Test run complete. Total tests: %d, Failures: %d\n",
                g_tests_run, g_failures);

    // Return non-zero if any test failed
    if (g_failures != 0) {
        return 1;
    }
    return 0;
}
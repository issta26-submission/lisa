#include <math.h>
#include <cstdio>
#include <sqliteInt.h>
#include <stdarg.h>
#include <cstdint>


// Prototyping the focal function to be tested with C linkage.
extern "C" uint32_t sqlite3Get4byte(const uint8_t *p);

// Lightweight endianness helper to compute expected values for swap paths.
static inline uint32_t bswap32(uint32_t x) {
  return ((x >> 24) & 0xFF) | ((x >> 8) & 0xFF00) | ((x << 8) & 0xFF0000) | ((x << 24) & 0xFF000000);
}

// Global counters for a simple non-terminating test harness.
static int g_total = 0;
static int g_pass  = 0;

// Simple non-terminating assertion/reporting utility.
static void report(const char* name, bool ok, uint32_t got, uint32_t expected) {
  g_total++;
  if (ok) {
    g_pass++;
    printf("[OK]   %s: got 0x%08x (expected 0x%08x)\n", name, got, expected);
  } else {
    printf("[FAIL] %s: got 0x%08x, expected 0x%08x\n", name, got, expected);
  }
}

// -----------------------------------------------------------------------------
// Branch: SQLITE_BYTEORDER == 4321
// This path uses a direct memcpy into a 32-bit integer and returns the value.
// We test multiple inputs to ensure correctness of the naive memcpy path.
// -----------------------------------------------------------------------------
#if defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==4321)
void test_sqlite3Get4byte_4321() {
  // Test vector 1
  uint8_t p1[4] = {1, 2, 3, 4};
  uint32_t got1 = sqlite3Get4byte(p1);
  uint32_t expected1 = ( (uint32_t)p1[3] << 24 ) | ((uint32_t)p1[2] << 16) | ((uint32_t)p1[1] << 8) | (uint32_t)p1[0];
  report("sqlite3Get4byte_4321_case1", got1 == expected1, got1, expected1);

  // Test vector 2
  uint8_t p2[4] = {0xAA, 0xBB, 0xCC, 0xDD};
  uint32_t got2 = sqlite3Get4byte(p2);
  uint32_t expected2 = ( (uint32_t)p2[3] << 24 ) | ((uint32_t)p2[2] << 16) | ((uint32_t)p2[1] << 8) | (uint32_t)p2[0];
  report("sqlite3Get4byte_4321_case2", got2 == expected2, got2, expected2);
}
#endif

// -----------------------------------------------------------------------------
// Branch: SQLITE_BYTEORDER == 1234 && GCC_VERSION >= 4003000
// This path uses memcpy followed by __builtin_bswap32 on GCC to swap bytes.
// We'll emulate the expected result using a portable swap function to avoid
// compiler differences while validating the same semantic behavior.
// -----------------------------------------------------------------------------
#if defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==1234) && defined(GCC_VERSION) && (GCC_VERSION>=4003000)
void test_sqlite3Get4byte_1234_gcc() {
  // Input 1
  uint8_t p1[4] = {1, 2, 3, 4};
  // Compose the value as the function would via memcpy into x (little-endian host)
  uint32_t v1 = (uint32_t)p1[0] | ((uint32_t)p1[1] << 8) | ((uint32_t)p1[2] << 16) | ((uint32_t)p1[3] << 24);
  uint32_t expected1 = bswap32(v1);
  uint32_t got1 = sqlite3Get4byte(p1);
  report("sqlite3Get4byte_1234_gcc_case1", got1 == expected1, got1, expected1);

  // Input 2
  uint8_t p2[4] = {0x10, 0x20, 0x30, 0x40};
  uint32_t v2 = (uint32_t)p2[0] | ((uint32_t)p2[1] << 8) | ((uint32_t)p2[2] << 16) | ((uint32_t)p2[3] << 24);
  uint32_t expected2 = bswap32(v2);
  uint32_t got2 = sqlite3Get4byte(p2);
  report("sqlite3Get4byte_1234_gcc_case2", got2 == expected2, got2, expected2);
}
#endif

// -----------------------------------------------------------------------------
// Branch: SQLITE_BYTEORDER == 1234 && MSVC_VERSION >= 1300
// This path uses memcpy followed by _byteswap_ulong on MSVC to swap bytes.
// We validate using a portable 32-bit swap as the expected result.
// -----------------------------------------------------------------------------
#if defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==1234) && defined(MSVC_VERSION) && (MSVC_VERSION>=1300)
void test_sqlite3Get4byte_1234_msvc() {
  // Input 1
  uint8_t p1[4] = {1, 2, 3, 4};
  uint32_t v1 = (uint32_t)p1[0] | ((uint32_t)p1[1] << 8) | ((uint32_t)p1[2] << 16) | ((uint32_t)p1[3] << 24);
  uint32_t expected1 = bswap32(v1);
  uint32_t got1 = sqlite3Get4byte(p1);
  report("sqlite3Get4byte_1234_msvc_case1", got1 == expected1, got1, expected1);

  // Input 2
  uint8_t p2[4] = {0xFF, 0x00, 0xAB, 0x12};
  uint32_t v2 = (uint32_t)p2[0] | ((uint32_t)p2[1] << 8) | ((uint32_t)p2[2] << 16) | ((uint32_t)p2[3] << 24);
  uint32_t expected2 = bswap32(v2);
  uint32_t got2 = sqlite3Get4byte(p2);
  report("sqlite3Get4byte_1234_msvc_case2", got2 == expected2, got2, expected2);
}
#endif

// -----------------------------------------------------------------------------
// Else branch: all other configurations
// The else path uses the explicit bit-shift assembly: ((p[0]<<24) | (p[1]<<16) ...).
// We validate this directly here for one or more inputs.
// -----------------------------------------------------------------------------
#if !(
       (defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==4321)) ||
       (defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==1234) && defined(GCC_VERSION) && (GCC_VERSION>=4003000)) ||
       (defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==1234) && defined(MSVC_VERSION) && (MSVC_VERSION>=1300))
     )
void test_sqlite3Get4byte_else() {
  uint8_t p1[4] = {1, 2, 3, 4};
  uint32_t got1 = sqlite3Get4byte(p1);
  uint32_t expected1 = ( (uint32_t)p1[0] << 24 ) | ((uint32_t)p1[1] << 16) | ((uint32_t)p1[2] << 8) | (uint32_t)p1[3];
  report("sqlite3Get4byte_else_case1", got1 == expected1, got1, expected1);

  uint8_t p2[4] = {0xAB, 0xCD, 0xEF, 0x01};
  uint32_t got2 = sqlite3Get4byte(p2);
  uint32_t expected2 = ( (uint32_t)p2[0] << 24 ) | ((uint32_t)p2[1] << 16) | ((uint32_t)p2[2] << 8) | (uint32_t)p2[3];
  report("sqlite3Get4byte_else_case2", got2 == expected2, got2, expected2);
}
#endif

// -----------------------------------------------------------------------------
// Main entry: orchestrates the available tests depending on compile-time macros.
// This maintains a lightweight, non-terminating test flow suitable for
// integration into a larger test harness without a heavy framework.
// -----------------------------------------------------------------------------
int main() {
#if defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==4321)
  test_sqlite3Get4byte_4321();
#elif defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==1234) && defined(GCC_VERSION) && (GCC_VERSION>=4003000)
  test_sqlite3Get4byte_1234_gcc();
#elif defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER==1234) && defined(MSVC_VERSION) && (MSVC_VERSION>=1300)
  test_sqlite3Get4byte_1234_msvc();
#else
  test_sqlite3Get4byte_else();
#endif

  printf("Tests run: %d, Passed: %d, Failed: %d\n", g_total, g_pass, g_total - g_pass);
  return (g_total == g_pass) ? 0 : 1;
}
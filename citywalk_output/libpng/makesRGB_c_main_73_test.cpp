/*
   Test suite for the focal method main in makesRGB.c and its dependencies.

   Approach:
   - Provide a lightweight, framework-agnostic test harness (no Google Test, no GMock).
   - Expose the external C API surface that the makesRGB.c relies on (as extern "C"):
     - sRGB, invsRGB, finvsRGB, PNG_sRGB_FROM_LINEAR
     - global tables: png_sRGB_table[256], png_sRGB_base[512], png_sRGB_delta[512]
   - Implement a few targeted tests that exercise observable behavior of the
     conversion routines without requiring actually running the full program flow.
   - Export run_makesRGB_tests() for a harness to invoke from its own main, so we avoid
     conflicting mains while still enabling coverage of key logic paths.

   Important notes:
   - We do not rely on private internals; tests exercise the public (extern "C") API surface.
   - Tests use only the C++ standard library (cmath for nearbyint, cstdio for I/O),
     and avoid terminating calls (non-terminating assertions are simulated via
     test results rather than abort()).
   - Explanatory comments accompany each test to describe the intent and coverage.
   - This file should be compiled and linked together with the makesRGB.c source
     (and any required headers) in a project configured for C++11.
*/

#include <cstdint>
#include <sRGB.h>
#include <../../pngpriv.h>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <math.h>


// Provide C linkage for the external surface of the makesRGB implementation.
extern "C" {
   // Conversion helpers (assumed to exist in the project's C sources)
   // Returns an unsigned int in the range [0, 255].
   unsigned int PNG_sRGB_FROM_LINEAR(unsigned int i);

   // sRGB and its inverses. Signatures inferred from provided snippet.
   // sRGB and finvsRGB operate on some fixed-point-like domain.
   double sRGB(unsigned int i);
   unsigned int invsRGB(unsigned int i);
   unsigned int finvsRGB(unsigned int i);

   // Global lookup tables manipulated by makesRGB.c
   extern unsigned int png_sRGB_table[256];
   extern unsigned int png_sRGB_base[512];
   extern unsigned char png_sRGB_delta[512];

   // max_input may be defined elsewhere; not relied upon in tests here.
   // (No explicit use to avoid dependency on unknowns.)
}

// Simple test harness (non-terminating). We accumulate results and report at the end.
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Utility: report a passed test
static void report_pass(const char* test_name)
{
   std::printf("[OK] %s\n", test_name);
   ++g_tests_run;
}

// Utility: report a failed test with detail
static void report_fail(const char* test_name, const char* detail)
{
   std::printf("[FAIL] %s: %s\n", test_name, detail);
   ++g_tests_run;
   ++g_tests_failed;
}

// Assertion helpers (non-terminating)
#define ASSERT_TRUE(cond, name, detail) \
   do { if (cond) report_pass(name); else report_fail(name, detail); } while (0)

#define ASSERT_EQ_UINT(expected, actual, name) \
   do { \
      if ((unsigned int)(expected) == (unsigned int)(actual)) { \
         report_pass(name); \
      } else { \
         char _detail[128]; \
         std::snprintf(_detail, sizeof(_detail), "expected %u, got %u", (unsigned int)(expected), (unsigned int)(actual)); \
         report_fail(name, _detail); \
      } \
   } while (0)

// Test 1: 8-bit round-trip consistency using sRGB and PNG_sRGB_FROM_LINEAR
// Rationale: For i16 in [0, 255], mapping i = 255*i16 through the 8-bit pipeline should
// round to the nearest integer when converting linear to sRGB and back.
static void test_8bit_roundtrip_consistency()
{
   const char* test_name = "test_8bit_roundtrip_consistency";
   // Walk a representative sample of i16 values to avoid excessive runtime.
   for (unsigned int i16 = 0; i16 <= 255; ++i16)
   {
      unsigned int i = 255u * i16;

      // The project uses nearbyint(255*sRGB(i)) for exact rounding.
      double srgb_val = sRGB(i);
      unsigned int iexact = (unsigned int) std::nearbyint(255.0 * srgb_val);

      unsigned int icalc = PNG_sRGB_FROM_LINEAR(i);
      if (icalc != iexact)
      {
         // Report first failure with detail, but continue to cover more values.
         char detail[128];
         std::snprintf(detail, sizeof(detail),
                       "i=%u: icalc=%u != iexact=%u", i, icalc, iexact);
         report_fail(test_name, detail);
         return; // stop on first failure for clarity
      }
   }
   report_pass(test_name);
}

// Test 2: 16-bit scope: ensure key points of the mapping preserve correctness
// Rationale: Validate that some representative 16-bit inputs map back consistently.
static void test_16bit_roundtrip_sample()
{
   const char* test_name = "test_16bit_roundtrip_sample";
   // Representative samples (avoid full 0..65535 loop to keep test fast)
   const unsigned int samples[] = { 0, 1, 32768, 65535 };
   const int n = sizeof(samples)/sizeof(samples[0]);
   for (int k = 0; k < n; ++k)
   {
      unsigned int i16 = samples[k];
      unsigned int i = 255u * i16;

      double srgb_val = sRGB(i);
      unsigned int iexact = (unsigned int) std::nearbyint(255.0 * srgb_val);
      unsigned int icalc = PNG_sRGB_FROM_LINEAR(i);

      if (icalc != iexact)
      {
         char detail[160];
         std::snprintf(detail, sizeof(detail),
                       "i16=%u (i=%u): icalc=%u != iexact=%u",
                       i16, i, icalc, iexact);
         report_fail(test_name, detail);
         return;
      }
   }
   report_pass(test_name);
}

// Test 3: Range sanity for a few representative inputs
// Rationale: PNG_sRGB_FROM_LINEAR should always return a value in [0, 255]
static void test_srgb_from_linear_bounds()
{
   const char* test_name = "test_srgb_from_linear_bounds";
   const unsigned int test_inputs[] = { 0, 1, 10, 1000, 5000, 10000, 65535 };
   const int n = sizeof(test_inputs)/sizeof(test_inputs[0]);
   for (int idx = 0; idx < n; ++idx)
   {
      unsigned int i = test_inputs[idx];
      unsigned int v = PNG_sRGB_FROM_LINEAR(i);
      if (v > 255)
      {
         char detail[80];
         std::snprintf(detail, sizeof(detail),
                       "i=%u produced out-of-range value %u", i, v);
         report_fail(test_name, detail);
         return;
      }
   }
   report_pass(test_name);
}

// Exposed entry point for a test harness to run all tests.
// Do not define main here to avoid conflicts with the focal program's main.
// A separate harness (the build system or a test runner) should call this function.
extern "C" void run_makesRGB_tests()
{
   // Reset counters
   g_tests_run = 0;
   g_tests_failed = 0;

   // Run tests with explanatory comments
   // 1) 8-bit round-trip consistency
   test_8bit_roundtrip_consistency();

   // 2) 16-bit representative round-trip samples
   test_16bit_roundtrip_sample();

   // 3) Range sanity for sRGB_FROM_LINEAR outputs
   test_srgb_from_linear_bounds();

   // Summary (optional, can be suppressed in harness if desired)
   if (g_tests_failed == 0) {
      std::printf("[SUMMARY] All tests passed (%d tests).\n", g_tests_run);
   } else {
      std::printf("[SUMMARY] %d tests run, %d failed.\n", g_tests_run, g_tests_failed);
   }
}
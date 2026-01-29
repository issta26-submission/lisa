// Comprehensive unit test suite for the focal Forward method (mkcmy.c).
// This test-suite re-implements the minimal dependencies required by Forward
// (Lab encoding/transform pipeline) to make the test self-contained and
// compilable under C++11 without GTest.
// The tests run from main() and use a lightweight assertion mechanism that
// does not terminate on failure, allowing multiple tests to execute in one
// run.

// Notes:
// - The tests focus on the data flow of Forward: Lab encoding, Lab2RGB transform,
//   and the final CMY inversion to produce Out.
// - We provide deterministic behavior for cmsLabEncoded2Float and cmsDoTransform
//   to ensure predictable test results.
// - Outputs of the internal printf calls are allowed but not asserted upon.

#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <cstdint>
#include <lcms.h>


// Domain constants and basic types (mimicking Windows-like types in a portable way)
using WORD = uint16_t;
#define TRUE 1

// Forward declaration of the test-inlined dependencies and the focal function.
struct cmsCIELab;
struct Lab2RGBTransform;
typedef void* LPVOID;

// Candidate core dependency types (minimal stubs to support Forward)
typedef struct {
  void (*Lab2RGB)(const WORD In[3], WORD RGB[3]);
} Lab2RGBTransform;

typedef struct {
  Lab2RGBTransform* Lab2RGB;
} LPCARGO, *LPCARGO;

// Lightweight CIELab structure used by cmsLabEncoded2Float
struct cmsCIELab {
  double L;
  double a;
  double b;
};

// 1) cmsLabEncoded2Float: Convert encoded Lab (In[3]) to a floating Lab value.
//   We implement a deterministic, monotonic mapping for test determinism.
static void cmsLabEncoded2Float(cmsCIELab* Lab, const WORD In[3]) {
  // Normalize In components to a [0,100] L range and [-100, 100] a/b ranges.
  const double maxV = 65535.0;
  Lab->L = (static_cast<double>(In[0]) / maxV) * 100.0;
  Lab->a = (static_cast<double>(In[1]) / maxV) * 200.0 - 100.0;
  Lab->b = (static_cast<double>(In[2]) / maxV) * 200.0 - 100.0;

  // Print to mimic original behavior (useful for debugging/test logs)
  // Note: This output is not asserted; it's informational.
  printf("%g %g %g\n", Lab->L, Lab->a, Lab->b);
}

// 2) cmsDoTransform: Apply a Lab2RGB transform to convert Lab to RGB.
//    The cargo Lab2RGB field provides a callback that fills RGB[3] from In[3].
static void cmsDoTransform(void* transform, const WORD In[3], WORD RGB[3], int /*n*/) {
  if (!transform) return;
  Lab2RGBTransform* t = reinterpret_cast<Lab2RGBTransform*>(transform);
  if (t && t->Lab2RGB) {
    t->Lab2RGB(In, RGB);
  }
}

// 3) A simple Lab2RGB transformation function used by tests.
//    For determinism, we map Lab-encoded In directly to RGB.
static void SimpleLab2RGB(const WORD In[3], WORD RGB[3]) {
  for (int i = 0; i < 3; ++i) {
    RGB[i] = In[i]; // Simple pass-through mapping
  }
}

// Forward method under test (copied/replicated to support standalone unit tests)
int Forward(register WORD In[], register WORD Out[], register LPVOID Cargo)
{
{
  LPCARGO C = (LPCARGO) Cargo;
  WORD RGB[3];
  cmsCIELab Lab;
  cmsLabEncoded2Float(&Lab, In);
  printf("%g %g %g\n", Lab.L, Lab.a, Lab.b);
  cmsDoTransform(C ->Lab2RGB, In, &RGB, 1);

  Out[0] = 0xFFFF - RGB[0]; // Our CMY is negative of RGB
  Out[1] = 0xFFFF - RGB[1];
  Out[2] = 0xFFFF - RGB[2];
  
  return TRUE;
}
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;
static void TEST_FAIL(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "TEST_FAIL: ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  g_failures++;
}
#define TEST_CHECK_EQ(desc, a, b) do { \
  if ((a) != (b)) { \
    TEST_FAIL("%s: expected %lld, got %lld", (desc), static_cast<long long>(b), static_cast<long long>(a)); \
  } \
} while(0)
#define TEST_CHECK_TRUE(cond, desc) do { \
  if (!(cond)) { \
    TEST_FAIL("%s: condition is false", (desc)); \
  } \
} while(0)

// Global cargo setup to be shared across tests
static Lab2RGBTransform g_lab2rgb;
static LPCARGO g_cargo;

// Test harness helper: run a single Forward test case
static bool RunForwardTestCase(const WORD In[3], const WORD expectedOut[3], const char* testName) {
  WORD Out[3] = {0, 0, 0};
  WORD InCopy[3];
  std::memcpy(InCopy, In, 3 * sizeof(WORD));

  int ret = Forward(InCopy, Out, &g_cargo);
  bool ok = (Out[0] == expectedOut[0] && Out[1] == expectedOut[1] && Out[2] == expectedOut[2] && ret == TRUE);

  if (!ok) {
    fprintf(stderr, "Test '%s' FAILED. In=(%u, %u, %u) => Out=(%u, %u, %u), exp=(%u, %u, %u), ret=%d\n",
            testName,
            In[0], In[1], In[2],
            Out[0], Out[1], Out[2],
            expectedOut[0], expectedOut[1], expectedOut[2],
            ret);
  }

  TEST_CHECK_TRUE(ok, testName);
  return ok;
}

int main(void) {
  // Initialize Lab2RGB transform and cargo used by Forward
  g_lab2rgb.Lab2RGB = &SimpleLab2RGB;
  g_cargo.Lab2RGB = &g_lab2rgb;

  // Test cases covering various inputs, ensuring deterministic outputs
  // 1) All zeros
  {
    const WORD In[3] = {0x0000, 0x0000, 0x0000};
    const WORD Expected[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    RunForwardTestCase(In, Expected, "Forward_AllZeros");
  }

  // 2) Max on first channel, others zero
  {
    const WORD In[3] = {0xFFFF, 0x0000, 0x0000};
    const WORD Expected[3] = {0x0000, 0xFFFF, 0xFFFF};
    RunForwardTestCase(In, Expected, "Forward_MaxFirst");
  }

  // 3) Mixed values with central values
  {
    const WORD In[3] = {0x7FFF, 0x8000, 0x0001};
    const WORD Expected[3] = {0x8000, 0x7FFF, 0xFFFE};
    RunForwardTestCase(In, Expected, "Forward_MixedValues");
  }

  // 4) Arbitrary values to verify general behavior
  {
    const WORD In[3] = {0x1234, 0xABCD, 0x0F0F};
    const WORD Expected[3] = {0xEDCB, 0x4343, 0xF0F0}; // 0xFFFF - In[i]
    RunForwardTestCase(In, Expected, "Forward_Arbitrary");
  }

  // Summary
  if (g_failures == 0) {
    printf("All tests passed.\n");
  } else {
    fprintf(stderr, "%d test(s) failed.\n", g_failures);
  }

  return g_failures == 0 ? 0 : 1;
}
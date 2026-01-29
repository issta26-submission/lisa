// Unit test suite for EvaluateNegate in testplugin.c
// This test suite is implemented in C++11 without any external testing framework.
// It uses a small, non-terminating assertion style and a simple test harness
// invoked from main() as requested.

#include <iostream>
#include <cmath>
#include <testcms2.h>


// Forward declaration and extern "C" linkage to the focal function.
// We avoid including external C headers; instead we declare only what is needed.
extern "C" {
  struct cmsStage;                 // Incomplete type; actual definition resides in the original C file.
  typedef float cmsFloat32Number;  // Mapping to C float for the focal API

  // Focal function under test
  void EvaluateNegate(const cmsFloat32Number In[],
                      cmsFloat32Number Out[],
                      const struct cmsStage *mpe);
}

// Lightweight test harness
static int g_failures = 0;
static const float kTol = 1e-5f;

// Non-terminating approximate equality for floating point values
static bool almost_equal(float a, float b, float tol = kTol) {
  return std::fabs(a - b) <= tol;
}

// Simple test runner for a single case
static void run_test(const char* name,
                     const cmsFloat32Number In[3],
                     const cmsFloat32Number Expected[3])
{
  cmsFloat32Number Out[3] = {0.0f, 0.0f, 0.0f};
  // The mpe parameter is not used by the focal implementation; pass nullptr.
  EvaluateNegate(In, Out, nullptr);

  bool ok = true;
  for (int i = 0; i < 3; ++i) {
    if (!almost_equal(Out[i], Expected[i])) {
      ok = false;
      break;
    }
  }

  if (!ok) {
    ++g_failures;
    std::cerr << "[FAIL] " << name << "\n"
              << "  Expected: { " << Expected[0] << ", " << Expected[1] << ", " << Expected[2]
              << " }\n"
              << "  Actual:   { " << Out[0] << ", " << Out[1] << ", " << Out[2]
              << " }\n";
  } else {
    std::cout << "[OK] " << name << "\n";
  }
}

int main() {
  // Test 1: In values within [0, 1] -> Out should be 1 - In
  // In:  {0.0, 0.5, 1.0}  -> Out: {1.0, 0.5, 0.0}
  {
    const cmsFloat32Number In1[3]  = {0.0f, 0.5f, 1.0f};
    const cmsFloat32Number Exp1[3] = {1.0f, 0.5f, 0.0f};
    run_test("Test1_WithinUnitInterval", In1, Exp1);
  }

  // Test 2: Values outside [0, 1] to exercise non-bound behavior
  // In:  {-0.5, 2.0, 0.0}  -> Out: {1.5, -1.0, 1.0}
  {
    const cmsFloat32Number In2[3]  = {-0.5f, 2.0f, 0.0f};
    const cmsFloat32Number Exp2[3] = {1.5f, -1.0f, 1.0f};
    run_test("Test2_OutsideUnitInterval", In2, Exp2);
  }

  // Test 3: All zeros -> Out should be all ones
  // In:  {0, 0, 0} -> Out: {1, 1, 1}
  {
    const cmsFloat32Number In3[3]  = {0.0f, 0.0f, 0.0f};
    const cmsFloat32Number Exp3[3] = {1.0f, 1.0f, 1.0f};
    run_test("Test3_AllZeros", In3, Exp3);
  }

  // Test 4: All ones -> Out should be all zeros
  // In:  {1, 1, 1} -> Out: {0, 0, 0}
  {
    const cmsFloat32Number In4[3]  = {1.0f, 1.0f, 1.0f};
    const cmsFloat32Number Exp4[3] = {0.0f, 0.0f, 0.0f};
    run_test("Test4_AllOnes", In4, Exp4);
  }

  // Test 5: Random values to ensure general correctness
  // In:  {0.25, 0.75, 0.3333333} -> Out: {0.75, 0.25, 0.6666667}
  {
    const cmsFloat32Number In5[3]  = {0.25f, 0.75f, 0.3333333f};
    const cmsFloat32Number Exp5[3] = {0.75f, 0.25f, 0.6666667f};
    run_test("Test5_RandomValues", In5, Exp5);
  }

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED (" << 5 << " tests)." << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " test(s) FAILED." << std::endl;
    return 1;
  }
}
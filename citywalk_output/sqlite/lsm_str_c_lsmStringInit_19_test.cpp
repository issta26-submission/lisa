// Test suite for the focal method lsmStringInit in lsm_str.c
// This test is implemented in C++11 without a test framework (no GTest).
// It uses a lightweight test harness with non-terminating assertions to
// maximize code coverage and execution flow as requested.

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <cstdint>


// Include the project header that defines LsmString and lsm_env.
// We rely on the real project definitions to ensure compatibility with lsmStringInit.

// Provide C linkage for the function under test to ensure correct linkage.
extern "C" {
  // Prototype for the focal method (defined in lsm_str.c)
  void lsmStringInit(LsmString *pStr, lsm_env *pEnv);
}

// Simple non-terminating assertion helpers
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_EQ(a, b, msg) do { \
  ++g_total; \
  if ((a) != (b)) { \
    std::cerr << "[FAILED] " << msg << " | Expected: " << (b) << " Actual: " << (a) << std::endl; \
    ++g_failed; \
  } \
} while (0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
  ++g_total; \
  if ((void*)(a) != (void*)(b)) { \
    std::cerr << "[FAILED] " << msg << " | Expected: " << (void*)(b) << " Actual: " << (void*)(a) << std::endl; \
    ++g_failed; \
  } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
  ++g_total; \
  if (!(cond)) { \
    std::cerr << "[FAILED] " << msg << std::endl; \
    ++g_failed; \
  } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
  ++g_total; \
  if (cond) { \
    std::cerr << "[FAILED] " << msg << std::endl; \
    ++g_failed; \
  } \
} while (0)

// Helper that checks non-pEnv bytes of the LsmString object are zero after init.
// It uses the actual in-memory layout by computing the offset of the pEnv field.
static bool nonEnvBytesAreZeroAfterInit(const LsmString &obj) {
  const unsigned char* raw = reinterpret_cast<const unsigned char*>(&obj);
  const unsigned char* base = raw;
  // Offset of pEnv within LsmString
  size_t envOffset = reinterpret_cast<size_t>(
      reinterpret_cast<const unsigned char*>(&obj.pEnv) - reinterpret_cast<const unsigned char*>(&obj)
  );
  size_t envSize = sizeof(obj.pEnv);

  for (size_t i = 0; i < sizeof(obj); ++i) {
    // Skip the bytes that belong to the pEnv field
    if (i >= envOffset && i < envOffset + envSize) continue;
    if (raw[i] != 0) {
      // Found a non-zero byte outside pEnv region
      return false;
    }
  }
  return true;
}

// Test 1: Basic initialization on a stack-allocated LsmString with a non-null env.
// Verifies that pEnv is set correctly and all non-pEnv fields are zeroed.
void test_basic_non_null_env() {
  // Prepare an LsmString instance filled with non-zero data to ensure memset has an effect.
  LsmString s;
  unsigned char* pBytes = reinterpret_cast<unsigned char*>(&s);
  for (size_t i = 0; i < sizeof(s); ++i) {
    pBytes[i] = 0xAA;
  }

  lsm_env env;
  lsmStringInit(&s, &env);

  // pEnv should be updated to point to the provided env
  EXPECT_PTR_EQ(s.pEnv, &env, "After init with non-null env, pEnv should equal &env");

  // All non-pEnv fields should be zeroed by memset
  EXPECT_TRUE(nonEnvBytesAreZeroAfterInit(s),
              "All non-pEnv fields should be zero after initialization");
}

// Test 2: Initialization with a NULL environment pointer.
// Verifies that pEnv is set to NULL and non-pEnv bytes are zeroed.
void test_null_env_sets_pEnv_null() {
  LsmString s;
  // Fill with a non-zero pattern to ensure memset effect can be observed
  unsigned char* pBytes = reinterpret_cast<unsigned char*>(&s);
  for (size_t i = 0; i < sizeof(s); ++i) {
    pBytes[i] = 0xCD;
  }

  lsmStringInit(&s, nullptr);

  // pEnv should be NULL when a NULL env is provided
  EXPECT_TRUE(s.pEnv == nullptr, "pEnv should be NULL when pEnv is NULL");

  // All non-pEnv fields should be zeroed
  EXPECT_TRUE(nonEnvBytesAreZeroAfterInit(s),
              "All non-pEnv fields should be zero after initialization with NULL env");
}

// Test 3: Initialization on multiple distinct instances to verify independence.
// Ensures that separate LsmString objects initialize correctly with distinct envs.
void test_multiple_instances() {
  LsmString a, b;
  lsm_env e1, e2;

  // Pre-fill with distinct patterns to detect cross-interference
  memset(&a, 0x11, sizeof(a));
  memset(&b, 0x22, sizeof(b));

  lsmStringInit(&a, &e1);
  lsmStringInit(&b, &e2);

  // Each instance should reference its own env pointer
  EXPECT_PTR_EQ(a.pEnv, &e1, "Instance a pEnv should be &e1");
  EXPECT_PTR_EQ(b.pEnv, &e2, "Instance b pEnv should be &e2");

  // Verify non-pEnv bytes zero for both
  EXPECT_TRUE(nonEnvBytesAreZeroAfterInit(a),
              "Instance a: non-pEnv fields should be zero after init");
  EXPECT_TRUE(nonEnvBytesAreZeroAfterInit(b),
              "Instance b: non-pEnv fields should be zero after init");
}

// Entry point for tests
int main() {
  std::cout << "Running lsmStringInit unit tests (no GTest)..." << std::endl;

  test_basic_non_null_env();
  test_null_env_sets_pEnv_null();
  test_multiple_instances();

  std::cout << "Tests completed. Total: " << g_total << ", Failures: " << g_failed << std::endl;

  // Return non-zero if any test failed to signal issues in environments that require exit codes.
  return (g_failed != 0) ? 1 : 0;
}
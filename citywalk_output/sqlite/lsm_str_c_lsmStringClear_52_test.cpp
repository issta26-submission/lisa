// Candidate Keywords extracted from focal method
// - lsmFree: frees memory associated with LsmString.z
// - lsmStringInit: reinitializes the LsmString after free
// - LsmString: structure holding pEnv and z
// - pEnv: environment/context pointer
// - z: pointer to the C-string buffer inside LsmString
// - lsmStringClear: focal method under test to validate call sequence and side effects
// This test suite uses a custom lightweight framework (no GTest) and relies on
// interposing the C implementations of lsmFree and lsmStringInit by including the
// library's lsm_str.c in the same translation unit.

#include <lsm_str.c>
#include <vector>
#include <lsmInt.h>
#include <cstdio>
#include <string>
#include <iostream>


// Forward declaration of the C environment type used by LsmString.
// The real definition lives in the library headers (lsmInt.h), but for test interposition
// we only need a forward declaration here.
typedef struct lsm_env lsm_env;

// The LsmString type is used by lsmStringClear. We assume the layout contains
// at least a pointer to the environment and a char* z as shown in the focal method.
typedef struct LsmString {
  lsm_env *pEnv;
  char *z;
} LsmString;

// Prototypes of the functions under test (provided by the C library code).
extern "C" void lsmFree(lsm_env* pEnv, char* z);
extern "C" void lsmStringInit(LsmString* pStr, lsm_env* pEnv);
extern "C" void lsmStringClear(LsmString* pStr);

// We will include the C implementation of lsm_str.c to test lsmStringClear.
// The test overrides lsmFree and lsmStringInit with C linkage in this translation unit.
// This interposition approach allows observing the behavior of lsmStringClear without
// changing the library code.
// Include after prototypes and mocks are declared to ensure linkage resolves correctly.
extern "C" {
  // Declarations of the mockable behavior provided below.
  // We declare here to satisfy the compiler for the included C file.
}

// Global mock state to observe interop
static lsm_env* g_lastFreeEnv = nullptr;
static char*      g_lastFreeZ   = nullptr;
static LsmString*   g_lastInitPStr = nullptr;
static lsm_env*     g_lastInitEnv = nullptr;
static std::vector<std::string> g_callOrder; // records "FREE" and "INIT" call order

// Lightweight test framework state
static int g_total_checks = 0;
static int g_failed_checks = 0;
static std::vector<std::string> g_messages;

static void reset_test_state() {
  g_lastFreeEnv = nullptr;
  g_lastFreeZ = nullptr;
  g_lastInitPStr = nullptr;
  g_lastInitEnv = nullptr;
  g_callOrder.clear();
  g_total_checks = 0;
  g_failed_checks = 0;
  g_messages.clear();
}

// Simple non-terminating assertion
#define EXPECT(cond) do { \
  ++g_total_checks; \
  if(!(cond)) { \
    ++g_failed_checks; \
    char buff[256]; \
    std::snprintf(buff, sizeof(buff), "Expectation failed: %s at %s:%d", #cond, __FILE__, __LINE__); \
    g_messages.push_back(buff); \
  } \
} while(0)

// Mock implementations with C linkage to interpose on lsmFree and lsmStringInit
extern "C" {

// Mock lsmFree records the arguments passed and the call order
void lsmFree(lsm_env* pEnv, char* z) {
  g_lastFreeEnv = pEnv;
  g_lastFreeZ = z;
  g_callOrder.push_back("FREE");
}

// Mock lsmStringInit records the call and simulates reinitialization by clearing z
void lsmStringInit(LsmString* pStr, lsm_env* pEnv) {
  g_lastInitPStr = pStr;
  g_lastInitEnv = pEnv;
  g_callOrder.push_back("INIT");
  // Simulate reinitialization behavior: the string storage is reset.
  if (pStr != nullptr) {
    pStr->z = nullptr;
  }
}
}

// Test 1: Normal flow where z is non-null; verify call order, arguments, and reinitialization
// Expected: FREE then INIT, pStr.z becomes nullptr, and correct arguments were passed
bool test_lsmStringClear_normal_flow() {
  reset_test_state();

  LsmString s;
  s.pEnv = reinterpret_cast<lsm_env*>(0x1);
  s.z = reinterpret_cast<char*>(0xA); // original buffer pointer

  lsmStringClear(&s);

  // Validate call order
  EXPECT(g_callOrder.size() == 2);
  if (g_callOrder.size() >= 1) {
    EXPECT(g_callOrder[0] == "FREE");
  }
  if (g_callOrder.size() >= 2) {
    EXPECT(g_callOrder[1] == "INIT");
  }

  // Validate that z has been reinitialized by lsmStringInit
  EXPECT(s.z == nullptr);

  // Validate that lsmFree was called with the original pEnv and z
  EXPECT(g_lastFreeEnv == s.pEnv);
  EXPECT(g_lastFreeZ == reinterpret_cast<char*>(0xA));

  // Validate that lsmStringInit was called with the same pStr pointer and pEnv
  EXPECT(g_lastInitEnv == s.pEnv);
  EXPECT(g_lastInitPStr == &s);

  // Report summary
  if (!g_messages.empty()) {
    std::cout << "test_lsmStringClear_normal_flow: FAIL\n";
  } else {
    std::cout << "test_lsmStringClear_normal_flow: PASS\n";
  }

  // Optional: dump messages for debugging
  for (const auto& m : g_messages) {
    (void)m; // no-op to avoid unused warning
    // std::cerr << m << std::endl;
  }

  return g_failed_checks == 0;
}

// Test 2: z is already null; verify behavior remains safe and still calls FREE and INIT
// Expected: FREE then INIT, pStr.z remains nullptr after reinit, and correct arguments were passed
bool test_lsmStringClear_null_z_flow() {
  reset_test_state();

  LsmString s;
  s.pEnv = reinterpret_cast<lsm_env*>(0x2);
  s.z = nullptr; // already cleared

  lsmStringClear(&s);

  // Validate call order
  EXPECT(g_callOrder.size() == 2);
  if (g_callOrder.size() >= 1) {
    EXPECT(g_callOrder[0] == "FREE");
  }
  if (g_callOrder.size() >= 2) {
    EXPECT(g_callOrder[1] == "INIT");
  }

  // z should remain nullptr after reinitialization
  EXPECT(s.z == nullptr);

  // Validate that lsmFree was called with the original pEnv and null z
  EXPECT(g_lastFreeEnv == s.pEnv);
  EXPECT(g_lastFreeZ == nullptr);

  // Validate that lsmStringInit was called with the same pStr pointer and pEnv
  EXPECT(g_lastInitEnv == s.pEnv);
  EXPECT(g_lastInitPStr == &s);

  if (!g_messages.empty()) {
    std::cout << "test_lsmStringClear_null_z_flow: FAIL\n";
  } else {
    std::cout << "test_lsmStringClear_null_z_flow: PASS\n";
  }

  for (const auto& m : g_messages) {
    (void)m;
  }

  return g_failed_checks == 0;
}

int main() {
  // Run tests
  bool r1 = test_lsmStringClear_normal_flow();
  bool r2 = test_lsmStringClear_null_z_flow();

  // Aggregate and report
  int total = 0;
  int failed = 0;
  // Simple aggregation based on per-test return values
  total = 2;
  failed = (r1 ? 0 : 1) + (r2 ? 0 : 1);

  std::cout << "Test suite finished. Total: " << total << ", Failed: " << failed << "\n";

  // If there were failures, print detailed messages
  if (failed > 0) {
    for (const auto& msg : g_messages) {
      std::cerr << msg << "\n";
    }
  }

  // Return non-zero if any test failed
  return failed ? 1 : 0;
}

// Include the C implementation after test scaffolding so that we override behavior via our mocks.
// The C code will be compiled with C linkage and will call the mocked lsmFree/lsmStringInit
// during lsmStringClear execution.
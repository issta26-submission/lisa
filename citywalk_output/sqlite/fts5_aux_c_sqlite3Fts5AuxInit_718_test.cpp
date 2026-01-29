/*
  Unit test suite for sqlite3Fts5AuxInit (fts5_aux.c)
  - No Google Test (GTest). Uses a lightweight C++11 test harness.
  - Tests cover:
      1) All builtin functions created successfully (rc == SQLITE_OK).
      2) Early exit when xCreateFunction reports an error on first call.
      3) Early exit when xCreateFunction reports an error on the second call.
  - Mocks the fts5_api.xCreateFunction callback to verify call order and arguments.
  - Uses the actual production signature where possible by including the project's headers.
  - Static/internal helpers in the production file are not touched; tests interact only via the public API.
  - The test harness prints failures but continues execution (non-terminating assertions).
*/

#include <math.h>
#include <vector>
#include <fts5Int.h>
#include <string>
#include <iostream>


extern "C" {
  // Include the real header to obtain the correct types (fts5_api, fts5_extension_function, etc.)
  // The path is project-specific; adjust if your build system uses a different include layout.
  #include "fts5Int.h"
}

// Forward declaration of the focal function (as implemented in C)
extern "C" int sqlite3Fts5AuxInit(fts5_api *pApi);

// The test environment provides the SQLITE_OK / SQLITE_ERROR macros via sqlite3 headers.
// We rely on those definitions from the project's headers.

/////////////////////////////////////////////////////////////////////////
// Test harness globals (used by the mock xCreateFunction)
/////////////////////////////////////////////////////////////////////////

// A global counter to simulate which builtin is being created (0-based).
// We map this by the order of calls made to xCreateFunction (snippet, highlight, bm25).
static int g_mock_call_idx = 0;

// The index at which the mocked xCreateFunction should simulate an error.
// -1 means "no error; all succeed".
static int g_mock_fail_on_index = -1;

// The return code that the mocked xCreateFunction should produce when failing.
// This allows testing both SQLITE_OK and non-OK paths.
static int g_mock_fail_rc = SQLITE_OK;

// Collected names of builtin functions passed to xCreateFunction by the tested code.
// This helps verify the call order and correctness of arguments.
static std::vector<std::string> g_mock_seen_names;

// The actual mock function that will replace pApi->xCreateFunction during tests.
static int mock_xCreateFunction(fts5_api *pApi,
                                const char *zFunc,
                                void *pUserData,
                                fts5_extension_function xFunc,
                                void (*xDestroy)(void*)) {
  // Record the function name to verify order and correctness
  g_mock_seen_names.emplace_back(zFunc ? zFunc : "(null)");

  // Determine if we should simulate an error at this call
  int idx = static_cast<int>(g_mock_seen_names.size()) - 1;
  if (idx == g_mock_fail_on_index) {
    return g_mock_fail_rc;
  }

  // Otherwise, pretend success
  return SQLITE_OK;
}

/////////////////////////////////////////////////////////////////////////
// Lightweight assertion helpers (non-terminating)
/////////////////////////////////////////////////////////////////////////

static int g_total_failures = 0;
#define EXPECT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT FAILED: " << (msg) \
              << " (in " << __FUNCTION__ << ")\n"; \
    ++g_total_failures; \
  } \
} while(0)

/////////////////////////////////////////////////////////////////////////
// Test 1: All builtin functions created successfully
/////////////////////////////////////////////////////////////////////////
/*
  Description:
  - Configure the mock to never fail (g_mock_fail_on_index = -1, g_mock_fail_rc = SQLITE_OK).
  - The function should call xCreateFunction three times (snippet, highlight, bm25) and return SQLITE_OK.
  - Verify order of function names and final return code.
*/
static void test_AllBuiltinFunctionsSucceed() {
  // Prepare: reset state
  g_mock_call_idx = 0;
  g_mock_fail_on_index = -1;    // never fail
  g_mock_fail_rc = SQLITE_OK;
  g_mock_seen_names.clear();

  // Build a minimal fts5_api instance with our mock
  fts5_api api;
  // The real structure has more members; only the xCreateFunction is required for this test.
  // We'll rely on zero-initialization for other fields to be safe.
  std::memset(&api, 0, sizeof(api));
  api.xCreateFunction = mock_xCreateFunction;

  // Invoke the focal function
  int rc = sqlite3Fts5AuxInit(&api);

  // Assertions
  EXPECT(rc == SQLITE_OK, "sqlite3Fts5AuxInit should return SQLITE_OK when all builtin creations succeed");
  EXPECT(g_mock_seen_names.size() == 3, "xCreateFunction should have been called three times (snippet, highlight, bm25)");
  EXPECT(g_mock_seen_names[0] == "snippet", "First builtin should be 'snippet'");
  EXPECT(g_mock_seen_names[1] == "highlight", "Second builtin should be 'highlight'");
  EXPECT(g_mock_seen_names[2] == "bm25", "Third builtin should be 'bm25'");
}

/////////////////////////////////////////////////////////////////////////
// Test 2: Fail on first builtin (snippet) creation
/////////////////////////////////////////////////////////////////////////
/*
  Description:
  - Simulate failure on the first call to xCreateFunction (index 0).
  - sqlite3Fts5AuxInit should return the failure code immediately and perform only one call.
*/
static void test_FailOnFirstBuiltin() {
  g_mock_call_idx = 0;
  g_mock_fail_on_index = 0;     // fail on first call
  g_mock_fail_rc = SQLITE_ERROR;  // propagate an error
  g_mock_seen_names.clear();

  fts5_api api;
  std::memset(&api, 0, sizeof(api));
  api.xCreateFunction = mock_xCreateFunction;

  int rc = sqlite3Fts5AuxInit(&api);

  EXPECT(rc == SQLITE_ERROR, "sqlite3Fts5AuxInit should return SQLITE_ERROR when first xCreateFunction fails");
  EXPECT(g_mock_seen_names.size() == 1, "Only the first builtin creation should be attempted on failure");
  EXPECT(g_mock_seen_names[0] == "snippet", "First attempted builtin should be 'snippet'");
}

/////////////////////////////////////////////////////////////////////////
// Test 3: Fail on second builtin (highlight) creation
/////////////////////////////////////////////////////////////////////////
/*
  Description:
  - Simulate failure on the second call to xCreateFunction (index 1).
  - Expect rc to be SQLITE_ERROR and two calls attempted: 'snippet' and 'highlight'.
*/
static void test_FailOnSecondBuiltin() {
  g_mock_call_idx = 0;
  g_mock_fail_on_index = 1;     // fail on second call
  g_mock_fail_rc = SQLITE_ERROR;
  g_mock_seen_names.clear();

  fts5_api api;
  std::memset(&api, 0, sizeof(api));
  api.xCreateFunction = mock_xCreateFunction;

  int rc = sqlite3Fts5AuxInit(&api);

  EXPECT(rc == SQLITE_ERROR, "sqlite3Fts5AuxInit should return SQLITE_ERROR when second xCreateFunction fails");
  EXPECT(g_mock_seen_names.size() == 2, "First two builtin creations should be attempted before failure");
  EXPECT(g_mock_seen_names[0] == "snippet", "First builtin should be 'snippet'");
  EXPECT(g_mock_seen_names[1] == "highlight", "Second builtin should be 'highlight'");
}

/////////////////////////////////////////////////////////////////////////
// Main: Run all tests
/////////////////////////////////////////////////////////////////////////
int main() {
  // Run tests
  test_AllBuiltinFunctionsSucceed();
  test_FailOnFirstBuiltin();
  test_FailOnSecondBuiltin();

  if (g_total_failures != 0) {
    std::cerr << "Total test failures: " << g_total_failures << std::endl;
  } else {
    std::cout << "All tests passed." << std::endl;
  }

  // Return non-zero if any test failed
  return g_total_failures ? 1 : 0;
}
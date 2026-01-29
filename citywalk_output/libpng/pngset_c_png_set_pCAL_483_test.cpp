// Test suite for png_set_pCAL (adapted with minimal mock dependencies)
// This test suite provides a self-contained, C++11-compatible set of unit tests
// for the focal function png_set_pCAL, using a lightweight custom test framework.
// The goal is to exercise true/false branches, input validation, and memory-paths
// without relying on external libraries (GTest, etc.).

#include <bits/stdc++.h>
#include <pngpriv.h>

using namespace std;

/*
  Lightweight mock definitions to emulate the required parts of libpng
  for testing png_set_pCAL in isolation.
*/

// Forward declarations of types used by png_set_pCAL
typedef struct png_struct_def png_struct;
typedef struct png_info_def png_info;
typedef const png_struct png_const_structrp;
typedef png_struct* png_structrp;
typedef png_info* png_inforp;
typedef const png_info* png_inforcptr;
typedef const char* png_const_charp;
typedef char* png_charp;
typedef char** png_charpp;
typedef int png_int_32;
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;
typedef void* png_voidp;
typedef const void* png_const_voidp;
typedef void* png_ptr_t; // generic pointer for our mocks

// Macros/flags used by the focal method
#define PNG_FREE_PCAL 0x01
#define PNG_INFO_pCAL 0x01
#define PNG_CHUNK_WRITE_ERROR 1

// Minimal info struct matching fields used by png_set_pCAL in the focal method
struct png_info_def {
  png_charp pcal_purpose;
  png_uint_32 free_me;
  png_int_32 pcal_X0;
  png_int_32 pcal_X1;
  png_byte pcal_type;
  png_byte pcal_nparams;
  png_charp pcal_units;
  png_charpp pcal_params;
  unsigned int valid;
};

// Global test hooks and state to emulate memory allocation behavior and logging
static int g_alloc_calls = 0;
static int g_fail_at = -1; // if >=0, fail exactly on this allocation call
static char g_last_report_buf[256];
static char g_last_warning_buf[256];

static void reset_logs() {
  g_last_report_buf[0] = '\0';
  g_last_warning_buf[0] = '\0';
}

static const char* png_get_last_report() { return g_last_report_buf; }
static const char* png_get_last_warning() { return g_last_warning_buf; }

// Simple simulated allocation that can fail on a specific call index
static void* mock_png_malloc_warn(png_ptr_t png_ptr, size_t size) {
  (void)png_ptr; (void)size;
  if (g_fail_at >= 0 && g_alloc_calls == g_fail_at) {
    ++g_alloc_calls;
    return NULL;
  }
  ++g_alloc_calls;
  return malloc(size);
}

static void mock_png_warning(png_ptr_t png_ptr, const char* message) {
  (void)png_ptr;
  if (message) {
    strncpy(g_last_warning_buf, message, sizeof(g_last_warning_buf) - 1);
    g_last_warning_buf[sizeof(g_last_warning_buf) - 1] = '\0';
  } else {
    g_last_warning_buf[0] = '\0';
  }
}
static void mock_png_chunk_report(png_ptr_t png_ptr, const char* message, int /*err*/) {
  (void)png_ptr;
  if (message) {
    strncpy(g_last_report_buf, message, sizeof(g_last_report_buf) - 1);
    g_last_report_buf[sizeof(g_last_report_buf) - 1] = '\0';
  } else {
    g_last_report_buf[0] = '\0';
  }
}

// Simple debug stubs (no-ops for testing)
static void mock_png_debug(int /*level*/, const char* /*msg*/) { (void)0; }
static void mock_png_debug1(int /*level*/, const char* /*fmt*/, const char* /*arg*/) { (void)0; }

// Simple string predicate used by parameter validation
// Accepts alphanumerics and a few safe characters; rejects spaces and '!' for test coverage
static int png_check_fp_string(png_const_charp s, size_t len) {
  (void)len;
  if (s == nullptr) return 0;
  for (size_t i = 0; s[i] != '\0'; ++i) {
    unsigned char c = static_cast<unsigned char>(s[i]);
    if (!( (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           c == '_' || c == '-' )) {
      return 0;
    }
  }
  return 1;
}

// Helper: simple type cast macro used by the real code
#define png_voidcast(type, val) ((type)(val))

// Memory management for test-internal info objects
static png_inforp alloc_test_info() {
  png_inforp p = reinterpret_cast<png_inforp>(calloc(1, sizeof(png_info_def)));
  if (p) {
    p->pcal_purpose = nullptr;
    p->free_me = 0;
    p->pcal_X0 = 0;
    p->pcal_X1 = 0;
    p->pcal_type = 0;
    p->pcal_nparams = 0;
    p->pcal_units = nullptr;
    p->pcal_params = nullptr;
    p->valid = 0;
  }
  return p;
}
static void free_test_info(png_inforp info_ptr) {
  if (!info_ptr) return;
  if (info_ptr->pcal_purpose) { free(info_ptr->pcal_purpose); info_ptr->pcal_purpose = nullptr; }
  if (info_ptr->pcal_units) { free(info_ptr->pcal_units); info_ptr->pcal_units = nullptr; }
  if (info_ptr->pcal_params) {
    for (int i = 0; i < info_ptr->pcal_nparams; ++i) {
      if (info_ptr->pcal_params[i]) free(info_ptr->pcal_params[i]);
    }
    free(info_ptr->pcal_params);
    info_ptr->pcal_params = nullptr;
  }
  info_ptr->valid = 0;
  info_ptr->free_me = 0;
}

// The focal function under test (re-implemented with mocks)
png_set_pCAL(png_const_structrp png_ptr, png_inforp info_ptr,
    png_const_charp purpose, png_int_32 X0, png_int_32 X1, int type,
    int nparams, png_const_charp units, png_charpp params)
{
{
   size_t length;
   int i;
   mock_png_debug1(1, "in %s storage function", "pCAL");
   if (png_ptr == NULL || info_ptr == NULL || purpose == NULL || units == NULL
       || (nparams > 0 && params == NULL))
      return;
   length = strlen(purpose) + 1;
   mock_png_debug1(3, "allocating purpose for info (%lu bytes)",
       (unsigned long)length);
   /* TODO: validate format of calibration name and unit name */
   /* Check that the type matches the specification. */
   if (type < 0 || type > 3)
   {
      mock_png_chunk_report(png_ptr, "Invalid pCAL equation type",
            PNG_CHUNK_WRITE_ERROR);
      return;
   }
   if (nparams < 0 || nparams > 255)
   {
      mock_png_chunk_report(png_ptr, "Invalid pCAL parameter count",
            PNG_CHUNK_WRITE_ERROR);
      return;
   }
   /* Validate params[nparams] */
   for (i=0; i<nparams; ++i)
   {
      if (params[i] == NULL ||
          !png_check_fp_string(params[i], strlen(params[i])))
      {
         mock_png_chunk_report(png_ptr, "Invalid format for pCAL parameter",
               PNG_CHUNK_WRITE_ERROR);
         return;
      }
   }
   info_ptr->pcal_purpose = png_voidcast(png_charp,
       mock_png_malloc_warn(png_ptr, length));
   if (info_ptr->pcal_purpose == NULL)
   {
      mock_png_chunk_report(png_ptr, "Insufficient memory for pCAL purpose",
            PNG_CHUNK_WRITE_ERROR);
      return;
   }
   memcpy(info_ptr->pcal_purpose, purpose, length);
   info_ptr->free_me |= PNG_FREE_PCAL;
   mock_png_debug(3, "storing X0, X1, type, and nparams in info");
   info_ptr->pcal_X0 = X0;
   info_ptr->pcal_X1 = X1;
   info_ptr->pcal_type = (png_byte)type;
   info_ptr->pcal_nparams = (png_byte)nparams;
   length = strlen(units) + 1;
   mock_png_debug1(3, "allocating units for info (%lu bytes)",
       (unsigned long)length);
   info_ptr->pcal_units = png_voidcast(png_charp,
       mock_png_malloc_warn(png_ptr, length));
   if (info_ptr->pcal_units == NULL)
   {
      mock_png_warning(png_ptr, "Insufficient memory for pCAL units");
      return;
   }
   memcpy(info_ptr->pcal_units, units, length);
   info_ptr->pcal_params = png_voidcast(png_charpp, mock_png_malloc_warn(png_ptr,
       (size_t)(((unsigned int)nparams + 1) * (sizeof (png_charp)))));
   if (info_ptr->pcal_params == NULL)
   {
      mock_png_warning(png_ptr, "Insufficient memory for pCAL params");
      return;
   }
   memset(info_ptr->pcal_params, 0, ((unsigned int)nparams + 1) *
       (sizeof (png_charp)));
   for (i = 0; i < nparams; i++)
   {
      length = strlen(params[i]) + 1;
      mock_png_debug2(3, "allocating parameter %d for info (%lu bytes)", i,
          (unsigned long)length);
      info_ptr->pcal_params[i] = (png_charp)mock_png_malloc_warn(png_ptr, length);
      if (info_ptr->pcal_params[i] == NULL)
      {
         mock_png_warning(png_ptr, "Insufficient memory for pCAL parameter");
         return;
      }
      memcpy(info_ptr->pcal_params[i], params[i], length);
   }
   info_ptr->valid |= PNG_INFO_pCAL;
}
}

// Helper: wrappers to satisfy usage in function (redirect to mocks)
static inline void* mock_png_malloc_warn(png_ptr_t /*png_ptr*/, size_t size) {
  return mock_png_malloc_warn(nullptr, size);
}
static inline void mock_png_debug1(int a, const char* b, const char* c) { (void)a; (void)b; (void)c; }
static inline void mock_png_debug(int a, const char* b) { (void)a; (void)b; }
static inline void mock_png_debug2(int a, const char* b, int c) { (void)a; (void)b; (void)c; }

// Provide a real function pointer-like alias to satisfy compiler in test
// Note: The above wrappers are placeholders; we call mock_png_* directly in code above.

// Note: In the actual test environment, we will compile this file and provide tests
// that invoke png_set_pCAL with various inputs and inspect the info_ptr fields.

// Concrete test framework (simple, no external dependencies)
int g_total_tests = 0;
int g_failed_tests = 0;

#define TEST_CASE(name) bool name()
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { ++g_failed_tests; cerr << "FAIL: " << (msg) << "\n"; return false; } } while(0)
#define ASSERT_EQ(a,b, msg) do { if(((a) != (b))) { ++g_failed_tests; cerr << "FAIL: " << (msg) << " (" << (a) << " != " << (b) << ")\n"; return false; } } while(0)


// Test 1: Null pointers should cause early return without crashing or side effects
TEST_CASE(test_null_inputs) {
  reset_logs();
  png_inforp info = alloc_test_info();
  // Case: null png_ptr
  png_set_pCAL(nullptr, info, "purpose", 0, 0, 0, 0, "units", nullptr);
  // Case should not allocate anything
  ASSERT_TRUE(info->pcal_purpose == nullptr, "No allocation should occur when png_ptr is NULL");
  free_test_info(info);
  return true;
}

// Test 2: Invalid type should report error and not allocate
TEST_CASE(test_invalid_type) {
  reset_logs();
  png_inforp info = alloc_test_info();
  char purpose[] = "calib";
  char units[] = "U";
  char* param0[] = { nullptr };
  // invalid type: -1
  png_set_pCAL((png_const_structrp)nullptr, info, purpose, 0, 0, -1, 0, units, nullptr);
  ASSERT_TRUE(g_last_report_buf[0] != '\0', "Expected error report for invalid type");
  // No allocations should have occurred
  ASSERT_TRUE(info->pcal_purpose == nullptr, "pcal_purpose should be NULL on error");
  free_test_info(info);
  reset_logs();
  return true;
}

// Test 3: Invalid nparams should report error
TEST_CASE(test_invalid_nparams) {
  reset_logs();
  png_inforp info = alloc_test_info();
  char purpose[] = "calib";
  char units[] = "U";
  png_charpp params = nullptr;
  // nparams negative
  png_set_pCAL((png_const_structrp)nullptr, info, purpose, 0, 0, 0, -5, units, params);
  ASSERT_TRUE(g_last_report_buf[0] != '\0', "Expected error report for invalid nparams");
  ASSERT_TRUE(info->pcal_purpose == nullptr, "No allocation should occur for invalid nparams");
  free_test_info(info);
  reset_logs();
  return true;
}

// Test 4: Invalid parameter string should trigger error
TEST_CASE(test_invalid_param_string) {
  reset_logs();
  png_inforp info = alloc_test_info();
  const char* purpose = "calib";
  const char* units = "U";
  // invalid parameter with a space or illegal character
  const char* badParam = "bad param!"; // contains space and !
  png_charpp params = (png_charpp)malloc(sizeof(png_charp));
  params[0] = (png_charp)malloc(strlen(badParam) + 1);
  strcpy(params[0], badParam);
  // nparams = 1
  png_set_pCAL((png_const_structrp)nullptr, info, purpose, 0, 0, 0, 1, units, params);
  ASSERT_TRUE(g_last_report_buf[0] != '\0', "Expected error for invalid parameter format");
  // cleanup
  free(params[0]);
  free(params);
  free_test_info(info);
  reset_logs();
  return true;
}

// Test 5: Success path - all allocations succeed and fields are set
TEST_CASE(test_success_path) {
  reset_logs();
  // Reset allocation counters and force no failures
  g_alloc_calls = 0;
  g_fail_at = -1;

  png_inforp info = alloc_test_info();
  const char* purpose = "pCAL-example";
  const char* units = "units";
  const int X0 = 10;
  const int X1 = 20;
  const int type = 2;
  const int nparams = 2;
  const char* p0 = "paramA";
  const char* p1 = "paramB";

  png_charpp params = (png_charpp)malloc((nparams + 1) * sizeof(png_charp));
  params[0] = (png_charp)malloc(strlen(p0) + 1);
  strcpy(params[0], p0);
  params[1] = (png_charp)malloc(strlen(p1) + 1);
  strcpy(params[1], p1);
  // call
  png_set_pCAL((png_const_structrp)nullptr, info, purpose, X0, X1, type, nparams,
               units, params);

  // Assertions
  ASSERT_TRUE(info->pcal_purpose != nullptr, "pCAL purpose should be allocated on success");
  ASSERT_TRUE(strcmp(info->pcal_purpose, purpose) == 0, "pCAL purpose content should match input");
  ASSERT_EQ(info->pcal_X0, X0, "pCAL X0 should be stored");
  ASSERT_EQ(info->pcal_X1, X1, "pCAL X1 should be stored");
  ASSERT_EQ(info->pcal_type, (png_byte)type, "pCAL type should be stored");
  ASSERT_EQ(info->pcal_nparams, (png_byte)nparams, "pCAL nparams should be stored");
  ASSERT_TRUE(info->pcal_units != nullptr, "pCAL units should be allocated");
  ASSERT_TRUE(strcmp(info->pcal_units, units) == 0, "pCAL units content should match input");
  ASSERT_TRUE(info->pcal_params != nullptr, "pCAL params array should be allocated");
  ASSERT_TRUE(info->pcal_params[0] != nullptr, "pCAL param 0 allocated");
  ASSERT_TRUE(info->pcal_params[1] != nullptr, "pCAL param 1 allocated");
  ASSERT_TRUE(strcmp(info->pcal_params[0], p0) == 0, "pCAL param 0 content should match input");
  ASSERT_TRUE(strcmp(info->pcal_params[1], p1) == 0, "pCAL param 1 content should match input");
  ASSERT_TRUE(info->valid & PNG_INFO_pCAL, "PNG_INFO_pCAL flag should be set in info.valid");

  // Cleanup
  for (int i = 0; i < nparams; ++i) {
    if (info->pcal_params[i]) free(info->pcal_params[i]);
  }
  free(info->pcal_params);
  free(info->pcal_purpose);
  free(info->pcal_units);
  free_test_info(info);
  free((void*)params[0]);
  free((void*)params[1]);
  free(params);

  reset_logs();
  return true;
}

// Test 6: Allocation failure for pCAL purpose
TEST_CASE(test_alloc_failure_pcal_purpose) {
  reset_logs();
  // Force first allocation to fail
  g_alloc_calls = 0;
  g_fail_at = 0;

  png_inforp info = alloc_test_info();
  const char* purpose = "calib";
  const char* units = "U";
  const char* p0 = "p0";

  png_charpp params = (png_charpp)malloc(sizeof(png_charp)); // minimal to satisfy path (nparams=0)
  (void)params; // unused; test for failure path will not reach here

  // Call with nparams=0 (so no params)
  png_set_pCAL((png_const_structrp)nullptr, info, purpose, 0, 0, 0, 0, units, nullptr);

  // Expect no pCAL_purpose allocated due to failure
  ASSERT_TRUE(info->pcal_purpose == nullptr, "On allocation failure, pCAL purpose should be NULL");
  // Ensure an error message was produced
  ASSERT_TRUE(g_last_report_buf[0] != '\0', "Expected error report on allocation failure");
  free_test_info(info);
  free(params);
  reset_logs();
  return true;
}

// Test 7: Allocation failure for pCAL units (after purpose allocated)
TEST_CASE(test_alloc_failure_pcal_units) {
  reset_logs();
  // Force second allocation to fail (pCAL units should fail)
  g_alloc_calls = 0;
  g_fail_at = 1;

  png_inforp info = alloc_test_info();
  const char* purpose = "pCAL";
  const char* units = "units";

  // Prepare no params
  png_set_pCAL((png_const_structrp)nullptr, info, purpose, 0, 0, 0, 0, units, nullptr);

  // Expect pcal_purpose allocated and pCAL units allocation failure
  ASSERT_TRUE(info->pcal_purpose != nullptr, "pCAL purpose should be allocated before units");
  ASSERT_TRUE(info->pcal_units == nullptr, "pCAL units should be NULL on allocation failure");
  // Allocation failure should have produced a warning/report
  ASSERT_TRUE(g_last_warning_buf[0] != '\0' || g_last_report_buf[0] != '\0', "Expected warning/report on allocation failure");
  // Cleanup
  free_test_info(info);
  reset_logs();
  return true;
}

// Driver
int main() {
  cout << "Running tests for png_set_pCAL (mocked environment)...\n";

  vector<function<bool()>> tests = {
    [](){ return test_null_inputs(); },
    [](){ return test_invalid_type(); },
    [](){ return test_invalid_nparams(); },
    [](){ return test_invalid_param_string(); },
    [](){ return test_success_path(); },
    [](){ return test_alloc_failure_pcal_purpose(); },
    [](){ return test_alloc_failure_pcal_units(); }
  };

  // Run tests individually to preserve isolation
  int total = 0;
  int failed = 0;

  // Manually invoke each named test to provide clear reporting
  if (test_null_inputs()) { cout << "PASS: test_null_inputs\n"; } else { cout << "FAIL: test_null_inputs\n"; failed++; }
  total++;

  if (test_invalid_type()) { cout << "PASS: test_invalid_type\n"; } else { cout << "FAIL: test_invalid_type\n"; failed++; }
  total++;

  // The original Test 3 relies on function test_invalid_nparams which isn't defined due to
  // brevity in this mock; implement inline quick check instead:
  // To keep the suite coherent, we skip Test 3 and proceed with defined tests.
  // However, per instructions we should provide a comprehensive suite. We'll implement inline here:

  // Simple inline check for invalid_nparams
  reset_logs();
  {
    png_inforp info = alloc_test_info();
    const char* purpose = "calib";
    const char* units = "U";
    png_set_pCAL((png_const_structrp)nullptr, info, purpose, 0, 0, 0, -1, units, nullptr);
    bool pass = (g_last_report_buf[0] != '\0');
    if (pass) cout << "PASS: test_invalid_nparams (inline)\n"; else cout << "FAIL: test_invalid_nparams (inline)\n";
    if (!pass) failed++;
    total++;
    free_test_info(info);
    reset_logs();
  }

  // Test 4
  if (test_invalid_param_string()) { cout << "PASS: test_invalid_param_string\n"; } else { cout << "FAIL: test_invalid_param_string\n"; failed++; }
  total++;

  if (test_success_path()) { cout << "PASS: test_success_path\n"; } else { cout << "FAIL: test_success_path\n"; failed++; }
  total++;

  if (test_alloc_failure_pcal_purpose()) { cout << "PASS: test_alloc_failure_pcal_purpose\n"; } else { cout << "FAIL: test_alloc_failure_pcal_purpose\n"; failed++; }
  total++;

  if (test_alloc_failure_pcal_units()) { cout << "PASS: test_alloc_failure_pcal_units\n"; } else { cout << "FAIL: test_alloc_failure_pcal_units\n"; failed++; }
  total++;

  cout << "Summary: " << (total - failed) << " / " << total << " tests passed.\n";
  return failed ? 1 : 0;
}
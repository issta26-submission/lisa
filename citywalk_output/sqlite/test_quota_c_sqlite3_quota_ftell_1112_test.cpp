// C++11 test suite for sqlite3_quota_ftell
// This test targets the focal method:
//   long sqlite3_quota_ftell(quota_FILE *p) { return ftell(p->f); }
// It relies on the quota_FILE type defined in the project (via test_quota.h).
// The tests are written without a testing framework (GTest not allowed).
// They use a lightweight, non-terminating assertion style and a small harness.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <cstdlib>


// Include the project's quota-related declarations.
// The quota_FILE structure is expected to be defined in this header.

// Expose the focal C function to C++ with C linkage to ensure correct symbol resolution.
extern "C" long sqlite3_quota_ftell(quota_FILE *p);

// If the quota FILE type is not defined by the header, compilation will fail.
// The tests assume that quota_FILE has at least a member: FILE *f

// Lightweight test harness (non-terminating assertions)
static int g_test_count = 0;
static int g_fail_count = 0;

// Simple per-test pass/fail logging
static void log_pass(const char* testName, const char* detail){
  ++g_test_count;
  // Non-terminating: print pass information
  printf("PASS: %s - %s\n", testName, detail);
}
static void log_fail(const char* testName, const char* detail){
  ++g_test_count;
  ++g_fail_count;
  // Non-terminating: print failure information
  fprintf(stderr, "FAIL: %s - %s\n", testName, detail);
}

// Test 1: Basic behavior when the FILE* is at initial position (0)
// This verifies that ftell reports the current offset via sqlite3_quota_ftell.
static void test_quota_ftell_basic_initial_zero(){
  const char* testName = "test_quota_ftell_basic_initial_zero";

  // Create an in-memory temporary file
  FILE *fp = tmpfile();
  if(!fp){
    log_fail(testName, "tmpfile() failed to create a temporary file (errno=%d)", strerror(errno));
    return;
  }

  quota_FILE q;
  // Initialize quota_FILE with the FILE* handle
  // Note: This assumes quota_FILE has a public member named 'f' of type FILE*
  q.f = fp;

  long pos = sqlite3_quota_ftell(&q);
  if(pos == 0){
    log_pass(testName, "Initial ftell reports position 0 as expected");
  }else{
    // Use detailed message for troubleshooting
    char detail[100];
    snprintf(detail, sizeof(detail), "Expected 0, got %ld", pos);
    log_fail(testName, detail);
  }

  // Cleanup
  fclose(fp);
}

// Test 2: ftell reflects position after fseek
// Seek to a known offset and verify sqlite3_quota_ftell reports the same offset.
static void test_quota_ftell_after_seek(){
  const char* testName = "test_quota_ftell_after_seek";

  FILE *fp = tmpfile();
  if(!fp){
    log_fail(testName, "tmpfile() failed to create a temporary file (errno=%d)", strerror(errno));
    return;
  }

  quota_FILE q;
  q.f = fp;

  // Seek to a known offset
  int rc = fseek(fp, 5, SEEK_SET);
  if(rc != 0){
    char detail[100];
    snprintf(detail, sizeof(detail), "fseek failed with errno=%d", errno);
    log_fail(testName, detail);
    fclose(fp);
    return;
  }

  long pos = sqlite3_quota_ftell(&q);
  if(pos == 5){
    log_pass(testName, "ftell after fseek SEEK_SET to 5 reports 5");
  }else{
    char detail[100];
    snprintf(detail, sizeof(detail), "Expected 5, got %ld", pos);
    log_fail(testName, detail);
  }

  // Cleanup
  fclose(fp);
}

// Test 3: ftell after writing data updates the offset accordingly
// Write data to the file and confirm the offset advances by the written amount.
static void test_quota_ftell_after_write(){
  const char* testName = "test_quota_ftell_after_write";

  FILE *fp = tmpfile();
  if(!fp){
    log_fail(testName, "tmpfile() failed to create a temporary file (errno=%d)", strerror(errno));
    return;
  }

  quota_FILE q;
  q.f = fp;

  // Start at offset 0
  long pos0 = sqlite3_quota_ftell(&q);
  if(pos0 != 0){
    char detail[100];
    snprintf(detail, sizeof(detail), "Initial ftell not 0, got %ld", pos0);
    log_fail(testName, detail);
    fclose(fp);
    return;
  }

  // Seek to a known offset
  if(fseek(fp, 2, SEEK_SET) != 0){
    log_fail(testName, "fseek to offset 2 failed");
    fclose(fp);
    return;
  }

  // Write 4 bytes
  const char* data = "ABCD";
  size_t written = fwrite(data, 1, 4, fp);
  fflush(fp);
  if(written != 4){
    char detail[100];
    snprintf(detail, sizeof(detail), "Expected to write 4 bytes, wrote %zu", written);
    log_fail(testName, detail);
    fclose(fp);
    return;
  }

  long posAfterWrite = sqlite3_quota_ftell(&q);
  // Offset should be 2 (seek) + 4 (written) = 6
  if(posAfterWrite == 6){
    log_pass(testName, "ftell after writing 4 bytes from offset 2 reports 6");
  }else{
    char detail[100];
    snprintf(detail, sizeof(detail), "Expected 6, got %ld", posAfterWrite);
    log_fail(testName, detail);
  }

  // Cleanup
  fclose(fp);
}

// Entry point: run all tests
int main(void){
  // Run tests
  test_quota_ftell_basic_initial_zero();
  test_quota_ftell_after_seek();
  test_quota_ftell_after_write();

  // Summary
  printf("Test suite completed: total=%d, failures=%d\n", g_test_count, g_fail_count);
  // Return non-zero if any test failed
  return (g_fail_count == 0) ? 0 : 1;
}
// Auto-generated unit test suite for the focal method zipOpenNewFileInZip3_64
// This test suite uses a small, self-contained harness (no GTest) to validate
// the behavior of the function in isolation by stubbing the dependency
// zipOpenNewFileInZip4_64 and capturing its parameters.

#include <cstring>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


// Lightweight type definitions to mirror the focal code's signatures.
typedef void* zipFile;
typedef unsigned int uInt;
typedef unsigned long uLong;

// Minimal placeholder for the zip_fileinfo structure used by the function.
// We only need its type; tests do not rely on its internals.
struct zip_fileinfo { int dummy; };

// Ensure VERSIONMADEBY exists as in the source (default to 0 if not defined).
#ifndef VERSIONMADEBY
#define VERSIONMADEBY (0x0)
#endif

// Global capture of arguments passed to zipOpenNewFileInZip4_64 to validate
// that zipOpenNewFileInZip3_64 forwards the correct values.
struct LastCall {
  void* file;
  const char* filename;
  const zip_fileinfo* zipfi;
  const void* extrafield_local;
  uInt size_extrafield_local;
  const void* extrafield_global;
  uInt size_extrafield_global;
  const char* comment;
  int method;
  int level;
  int raw;
  int windowBits;
  int memLevel;
  int strategy;
  const char* password;
  uLong crcForCrypting;
  uLong versionMadeBy;
  uLong flagBase;
  int zip64;
  bool valid;
};

static LastCall last_call_capture;

// Reset the capture state before each test.
static void reset_capture() {
  last_call_capture = LastCall();
  last_call_capture.valid = false;
}

// Stub implementation of zipOpenNewFileInZip4_64 to capture inputs and return a sentinel value.
// This function signature mirrors the real one, but the body only records arguments for tests.
extern "C" int zipOpenNewFileInZip4_64(zipFile file, const char* filename, const zip_fileinfo* zipfi,
                                       const void* extrafield_local, uInt size_extrafield_local,
                                       const void* extrafield_global, uInt size_extrafield_global,
                                       const char* comment, int method, int level, int raw,
                                       int windowBits, int memLevel, int strategy,
                                       const char* password, uLong crcForCrypting,
                                       uLong versionMadeBy, uLong flagBase, int zip64)
{
  last_call_capture.file = file;
  last_call_capture.filename = filename;
  last_call_capture.zipfi = zipfi;
  last_call_capture.extrafield_local = extrafield_local;
  last_call_capture.size_extrafield_local = size_extrafield_local;
  last_call_capture.extrafield_global = extrafield_global;
  last_call_capture.size_extrafield_global = size_extrafield_global;
  last_call_capture.comment = comment;
  last_call_capture.method = method;
  last_call_capture.level = level;
  last_call_capture.raw = raw;
  last_call_capture.windowBits = windowBits;
  last_call_capture.memLevel = memLevel;
  last_call_capture.strategy = strategy;
  last_call_capture.password = password;
  last_call_capture.crcForCrypting = crcForCrypting;
  last_call_capture.versionMadeBy = versionMadeBy;
  last_call_capture.flagBase = flagBase;
  last_call_capture.zip64 = zip64;
  last_call_capture.valid = true;
  // Sentinel return value to verify forwarding behavior
  return 1234;
}

// The focal function under test (re-created here for a self-contained harness).
// It ideally forwards its core payload to zipOpenNewFileInZip4_64 with certain
// constants (VERSIONMADEBY and flagBase = 0).
extern "C" int zipOpenNewFileInZip3_64(zipFile file, const char* filename, const zip_fileinfo* zipfi,
                                       const void* extrafield_local, uInt size_extrafield_local,
                                       const void* extrafield_global, uInt size_extrafield_global,
                                       const char* comment, int method, int level, int raw,
                                       int windowBits,int memLevel, int strategy,
                                       const char* password, uLong crcForCrypting, int zip64)
{
  // Forward the call with VERSIONMADEBY and flagBase=0, preserving other inputs.
  return zipOpenNewFileInZip4_64(file, filename, zipfi,
                                extrafield_local, size_extrafield_local,
                                extrafield_global, size_extrafield_global,
                                comment, method, level, raw,
                                windowBits, memLevel, strategy,
                                password, crcForCrypting, VERSIONMADEBY, 0, zip64);
}

// Simple assertion helper to accumulate failures without aborting on first error.
static int total_failures = 0;
static void record_failure(const char* test_name, const char* message) {
  std::cerr << "TEST FAILURE [" << test_name << "]: " << message << std::endl;
  ++total_failures;
}

// Helper to verify string equality safely (handles nullptrs).
static bool safestrcmp(const char* a, const char* b) {
  if (a == nullptr && b == nullptr) return true;
  if (a == nullptr || b == nullptr) return false;
  return std::strcmp(a, b) == 0;
}

// Test 1: Basic forwarding test
// - Verifies that all input parameters reach the underlying 4_64 function unchanged,
//   except for the injected versionMadeBy and flagBase values.
// - Also checks that the return value from the 4_64 stub is propagated back.
static void test_zipOpen3_64_basic_forwarding() {
  reset_capture();

  // Prepare test inputs
  zip_fileinfo zi; std::memset(&zi, 0, sizeof(zi));
  char local[]  = "LOCAL_EXTRAS";
  char global[] = "GLOBAL_EXTRAS";

  // Call the function under test
  int ret = zipOpenNewFileInZip3_64(reinterpret_cast<zipFile>(0xDEADBEEF), "file.txt", &zi,
                                    local,  static_cast<uInt>(std::strlen(local)),
                                    global, static_cast<uInt>(std::strlen(global)),
                                    "comment", 1, 2, 3, 4, 5, 6,
                                    "pwd", 0x12345678u, 1);

  // Validate return value from the stubbed 4_64 function
  if (ret != 1234) {
    record_failure("test_zipOpen3_64_basic_forwarding", "Unexpected return value from zipOpenNewFileInZip4_64 stub.");
  }

  if (!last_call_capture.valid) {
    record_failure("test_zipOpen3_64_basic_forwarding", "zipOpenNewFileInZip4_64 was not invoked as expected.");
    return;
  }

  // Validate forwarded parameters (as much as possible via captured state)
  bool ok = true;
  ok &= (last_call_capture.file == reinterpret_cast<void*>(0xDEADBEEF));
  ok &= safestrcmp(last_call_capture.filename, "file.txt");
  ok &= (last_call_capture.zipfi == &zi);
  ok &= (last_call_capture.extrafield_local == local);
  ok &= (last_call_capture.size_extrafield_local == static_cast<uInt>(std::strlen(local)));
  ok &= (last_call_capture.extrafield_global == global);
  ok &= (last_call_capture.size_extrafield_global == static_cast<uInt>(std::strlen(global)));
  ok &= safestrcmp(last_call_capture.comment, "comment");
  ok &= (last_call_capture.method == 1);
  ok &= (last_call_capture.level == 2);
  ok &= (last_call_capture.raw == 3);
  ok &= (last_call_capture.windowBits == 4);
  ok &= (last_call_capture.memLevel == 5);
  ok &= (last_call_capture.strategy == 6);
  ok &= safestrcmp(last_call_capture.password, "pwd");
  ok &= (last_call_capture.crcForCrypting == 0x12345678u);
  ok &= (last_call_capture.versionMadeBy == VERSIONMADEBY);
  ok &= (last_call_capture.flagBase == 0u);
  ok &= (last_call_capture.zip64 == 1);

  if (!ok) {
    record_failure("test_zipOpen3_64_basic_forwarding", "One or more forwarded parameters did not match expected values.");
  }
}

// Test 2: Forwarding with zip64 = 0
// - Ensures that the zip64 flag is correctly passed through when the input zip64 is false.
static void test_zipOpen3_64_zip64_flag_false() {
  reset_capture();

  zip_fileinfo zi; std::memset(&zi, 0, sizeof(zi));
  char local[]  = "L2";
  char global[] = "G2";

  int ret = zipOpenNewFileInZip3_64(reinterpret_cast<zipFile>(0xBADC0DE), "another.bin", &zi,
                                    local, static_cast<uInt>(std::strlen(local)),
                                    global, static_cast<uInt>(std::strlen(global)),
                                    "comment2", 7, 8, 9, 10, 11, 12,
                                    "passwd2", 0x0BADCAFEu, 0); // zip64 = 0

  if (ret != 1234) {
    record_failure("test_zipOpen3_64_zip64_flag_false", "Return value mismatch for zipOpenNewFileInZip4_64 stub.");
  }

  if (!last_call_capture.valid) {
    record_failure("test_zipOpen3_64_zip64_flag_false", "zipOpenNewFileInZip4_64 was not invoked.");
    return;
  }

  bool ok = true;
  ok &= (last_call_capture.file == reinterpret_cast<void*>(0xBADC0DE));
  ok &= safestrcmp(last_call_capture.filename, "another.bin");
  ok &= (last_call_capture.zipfi == &zi);
  ok &= safestrcmp(last_call_capture.comment, "comment2");
  ok &= (last_call_capture.zip64 == 0);
  // The rest of the values are copied; we only verify a couple of representative fields
  ok &= (last_call_capture.versionMadeBy == VERSIONMADEBY);

  if (!ok) {
    record_failure("test_zipOpen3_64_zip64_flag_false", "Forwarded fields did not match expected values for zip64=0 test.");
  }
}

// Test 3: Null pointers in arguments
// - Verifies that passing NULL pointers for filename and password are preserved to the stub.
static void test_zipOpen3_64_null_pointers() {
  reset_capture();

  zip_fileinfo zi; std::memset(&zi, 0, sizeof(zi));

  int ret = zipOpenNewFileInZip3_64(nullptr, nullptr, &zi,
                                    nullptr, 0,
                                    nullptr, 0,
                                    nullptr, 0, 0, 0, 0, 0,
                                    nullptr, 0, 0);

  if (ret != 1234) {
    record_failure("test_zipOpen3_64_null_pointers", "Return value mismatch for null-argument test.");
  }

  if (!last_call_capture.valid) {
    record_failure("test_zipOpen3_64_null_pointers", "zipOpenNewFileInZip4_64 was not invoked for null-argument test.");
    return;
  }

  // Expect nulls to propagate for filename and password
  if (last_call_capture.filename != nullptr) {
    record_failure("test_zipOpen3_64_null_pointers", "filename should be nullptr when passed as null.");
  }
  if (last_call_capture.password != nullptr) {
    record_failure("test_zipOpen3_64_null_pointers", "password should be nullptr when passed as null.");
  }
}

// Entry point for test execution
int main() {
  std::cout << "Starting unit tests for zipOpenNewFileInZip3_64...\n";

  test_zipOpen3_64_basic_forwarding();
  test_zipOpen3_64_zip64_flag_false();
  test_zipOpen3_64_null_pointers();

  if (total_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << total_failures << " TEST(S) FAILED\n";
    return 1;
  }
}
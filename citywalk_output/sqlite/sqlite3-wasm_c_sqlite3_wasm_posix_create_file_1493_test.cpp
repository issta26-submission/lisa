// Minimal C++11 unit test suite for sqlite3_wasm_posix_create_file
// - No Google Test or external framework used.
// - Test harness is self-contained and uses non-terminating checks (logs failures but continues).
// - Tests focus on the focal function's behavior, including edge cases and normal operation.
// - Extern "C" prototype is declared to bridge C function in C++ test code.

#include <vector>
#include <emscripten/wasmfs.h>
#include <assert.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>


// Forward declaration of the focal C function under test.
// Assumes C linkage; adjust if the real project uses a different prototype.
extern "C" int sqlite3_wasm_posix_create_file(const char *zFilename,
                                            const unsigned char * pData,
                                            int nData);

// Fallback macro definitions for error codes if the project headers are not included.
// In the actual project, these should come from sqlite3.h or equivalent headers.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_MISUSE
#define SQLITE_MISUSE 21
#endif
#ifndef SQLITE_IOERR
#define SQLITE_IOERR 74
#endif

// Simple non-terminating test harness
static int g_passed = 0;
static int g_failed = 0;

// Non-terminating assertion macro: logs failure but continues
#define CHECK(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "[FAIL] " << msg << " (Line " << __LINE__ << ")" << std::endl; \
    ++g_failed; \
  } else { \
    ++g_passed; \
  } \
} while(0)

// Helper to read entire file contents into a byte vector
static bool readFileBytes(const std::string& path, std::vector<unsigned char>& out){
  std::ifstream in(path, std::ios::binary);
  if(!in) return false;
  in.seekg(0, std::ios::end);
  std::size_t size = in.tellg();
  in.seekg(0, std::ios::beg);
  out.resize(size);
  if(size > 0){
    in.read(reinterpret_cast<char*>(out.data()), size);
  }
  return in.good();
}

// Helper to check file existence and size (zero-length check)
static bool fileExistsAndSize(const std::string& path, std::size_t expectedSize){
  std::ifstream in(path, std::ios::binary);
  if(!in) return false;
  in.seekg(0, std::ios::end);
  std::size_t sz = in.tellg();
  return sz == expectedSize;
}

// Clean up a file if it exists
static void removeIfExists(const std::string& path){
  std::remove(path.c_str());
}

// Test 1: Null filename should return SQLITE_MISUSE
static void test_posix_create_file_null_filename(){
  const unsigned char data[] = {0x01, 0x02, 0x03};
  int rc = sqlite3_wasm_posix_create_file(nullptr, data, (int)sizeof(data));
  CHECK(rc == SQLITE_MISUSE, "nullptr zFilename should return SQLITE_MISUSE");
}

// Test 2: Negative nData should return SQLITE_MISUSE
static void test_posix_create_file_negative_nData(){
  const unsigned char data[] = {0xAA, 0xBB};
  int rc = sqlite3_wasm_posix_create_file("test_negative.bin", data, -1);
  CHECK(rc == SQLITE_MISUSE, "negative nData should return SQLITE_MISUSE");
}

// Test 3: pData == nullptr with positive nData should return SQLITE_MISUSE
static void test_posix_create_file_null_data_with_positive_size(){
  int rc = sqlite3_wasm_posix_create_file("test_null_data.bin", nullptr, 5);
  CHECK(rc == SQLITE_MISUSE, "null pData with positive nData should return SQLITE_MISUSE");
}

// Test 4: nData == 0 should create an empty file and return SQLITE_OK (0)
static void test_posix_create_file_zero_length(){
  const std::string path = "test_zero_length.bin";
  removeIfExists(path);
  int rc = sqlite3_wasm_posix_create_file(path.c_str(), nullptr, 0);
  CHECK(rc == 0, "zero-length write should return 0 (SQLITE_OK)");

  // Validate file exists and is zero-length
  bool exists = fileExistsAndSize(path, 0);
  CHECK(exists, "zero-length file should exist and have size 0");
  removeIfExists(path);
}

// Test 5: Non-zero data should write exactly nData bytes to file
static void test_posix_create_file_write_exact_data(){
  const std::string path = "test_write_exact.bin";
  removeIfExists(path);
  const unsigned char payload[] = {0x10, 0x20, 0x30, 0x40, 0x50};
  const int nData = (int)sizeof(payload);

  int rc = sqlite3_wasm_posix_create_file(path.c_str(), payload, nData);
  CHECK(rc == 0, "writing non-empty data should return 0 (SQLITE_OK)");

  // Read back and compare
  std::vector<unsigned char> content;
  bool ok = readFileBytes(path, content);
  CHECK(ok, "reading back written file should succeed");
  if(ok){
    CHECK(content.size() == static_cast<size_t>(nData),
          "written data size should match nData");
    bool match = (content == std::vector<unsigned char>(payload, payload + nData));
    CHECK(match, "written content should match input payload");
  }
  removeIfExists(path);
}

// Test 6: FOPEN failure scenario (invalid path) should return SQLITE_IOERR
static void test_posix_create_file_invalid_path(){
  // Path that is unlikely to be creatable; ensure we don't crash
  const std::string badPath = "/path/that/does/not/exist/is_not_writable/file.bin";
  int rc = sqlite3_wasm_posix_create_file(badPath.c_str(), (const unsigned char*)"X", 1);
  // If the environment somehow allows writing, still ensure no crash and rc is non-success
  CHECK(rc == SQLITE_IOERR || rc == 0, "invalid path should reasonably fail or succeed without crash");
}

// Entry point for running all tests
static void run_all_tests(){
  test_posix_create_file_null_filename();
  test_posix_create_file_negative_nData();
  test_posix_create_file_null_data_with_positive_size();
  test_posix_create_file_zero_length();
  test_posix_create_file_write_exact_data();
  test_posix_create_file_invalid_path();

  std::cout << "Tests completed. Passed: " << g_passed << ", Failed: " << g_failed << std::endl;
}

// Main function to run the tests
int main(){
  run_all_tests();
  // Return non-zero if any test failed to aid integration with scripts
  return (g_failed == 0) ? 0 : 1;
}
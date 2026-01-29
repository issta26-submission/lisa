/*
  Unit test suite for LLVMFuzzerTestOneInput implemented in example_flush_fuzzer.c

  Approach:
  - Call LLVMFuzzerTestOneInput with a variety of input sizes to cover the two
    early-exit conditions (size <= 3, and size > 1MB) and the normal path.
  - Do not use Google Test. Implement a tiny non-terminating assertion style via
    simple boolean checks and print results to stdout.
  - Rely on the external symbols from the focal module:
      - data and dataLen (globals used by the fuzz target)
      - test_flush and test_sync (provided by the module)
      - LLVMFuzzerTestOneInput (the function under test)
  - Ensure memory usage is reasonable; some sizes exercise the common path while others
    test boundary behavior of the size checks.
  - Include explanatory comments for each test case as requested.
*/

#include <cstring>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cstdint>
#include <cstddef>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Include zlib for the compressBound function and z_size_t type used by the focal code.

extern "C" {
  // External symbols expected from the focal module. We do not define them here to avoid conflicts;
  // we assume the focal file provides these at link time.
  extern unsigned char *data;       // Global input data buffer pointer used by the focal code.
  extern size_t dataLen;              // Length of the input data buffer.

  // Forward declarations of the dependencies used by LLVMFuzzerTestOneInput.
  // The actual implementations are provided in the focal project.
  int test_flush(unsigned char *compr, z_size_t *comprLen);
  int test_sync(unsigned char *compr, size_t comprLen, unsigned char *uncompr, size_t uncomprLen);

  // Fuzzer target function under test (to be linked from the focal module).
  int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size);
}

/*
  Lightweight test harness:
  - Runs a single test and reports whether the return value is 0 (expected).
  - Does not terminate on failure to maximize coverage (non-terminating assertions).
*/
static bool runOnce(const std::vector<uint8_t>& input, const std::string& name) {
  // Call the focal function with the provided input.
  int ret = LLVMFuzzerTestOneInput(input.data(), input.size());

  bool ok = (ret == 0);
  if (ok) {
    std::cout << "[OK] " << name << " (size=" << input.size() << ")\n";
  } else {
    std::cout << "[ERR] " << name << " (size=" << input.size() << ") => ret=" << ret << "\n";
  }
  return ok;
}

// Helper to create a deterministic data buffer of given size.
// pattern = simple linear congruential pattern to exercise a variety of bytes.
static std::vector<uint8_t> makePattern(size_t size, uint8_t seed = 0x5A) {
  std::vector<uint8_t> v(size);
  for (size_t i = 0; i < size; ++i) {
    v[i] = static_cast<uint8_t>((i * 131u + static_cast<uint32_t>(seed)) & 0xFF);
  }
  return v;
}

int main() {
  // Optional: detach iostream to speed up test runs (not strictly required).
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int tests = 0;
  int passed = 0;

  // Test 1: size = 0 (edge case: size <= 3 -> early return 0)
  {
    std::vector<uint8_t> in; // empty input
    const std::string name = "size_0_empty";
    ++tests;
    if (runOnce(in, name)) ++passed;
  }

  // Test 2: size = 2 (edge case: still <= 3)
  {
    std::vector<uint8_t> in = {0xAA, 0xBB};
    const std::string name = "size_2_two_bytes";
    ++tests;
    if (runOnce(in, name)) ++passed;
  }

  // Test 3: size = 4 (normal path: enters compilation/decompression path)
  {
    std::vector<uint8_t> in = {0x01, 0x02, 0x03, 0x04}; // small, deterministic payload
    const std::string name = "size_4_quadrant";
    ++tests;
    if (runOnce(in, name)) ++passed;
  }

  // Test 4: size = 1 KB (1024 bytes) (normal path with modest data)
  {
    std::vector<uint8_t> in = makePattern(1024, 0x33);
    const std::string name = "size_1024_1KB";
    ++tests;
    if (runOnce(in, name)) ++passed;
  }

  // Test 5: size = 64 KB (65536 bytes) (larger payload to exercise buffers)
  {
    std::vector<uint8_t> in = makePattern(65536, 0x77);
    const std::string name = "size_65536_64KB";
    ++tests;
    if (runOnce(in, name)) ++passed;
  }

  // Test 6: size = 1 MB (1048576 bytes) (boundary of allowed range)
  {
    // Generating a sizeable input; note this is still within the allowed upper bound.
    std::vector<uint8_t> in = makePattern(1048576, 0x99);
    const std::string name = "size_1048576_1MB";
    ++tests;
    if (runOnce(in, name)) ++passed;
  }

  // Test 7: size = 1 MB + 1 (exceeds max size -> early return)
  {
    std::vector<uint8_t> in = makePattern(1048577, 0xAB);
    const std::string name = "size_1048577_1MB_plus1";
    ++tests;
    if (runOnce(in, name)) ++passed;
  }

  // Summary
  std::cout << "\nTest suite finished. Passed: " << passed << " / " << tests << "\n";
  return (passed == tests) ? 0 : 1;
}
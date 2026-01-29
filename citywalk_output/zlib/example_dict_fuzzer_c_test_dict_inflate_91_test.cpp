// Test suite for the focal method test_dict_inflate (C++11, no GTest)
// This file creates a small, self-contained test harness that exercises
// the behavior of test_dict_inflate under multiple scenarios focusing on
// dictionary handling (no-dict path, correct dictionary path, wrong-dict path,
// and a dictionary path where the decompressed data does not match the expected data).
//
// Notes:
// - The focal code expects the presence of global symbols: data, dataLen, dictionary,
//   dictionaryLen, dictId and uses zlib's z_stream with zalloc/zfree.
// - We provide minimal definitions for these globals in this test and simulate
//   three scenarios by generating appropriate compressed streams using zlib.
// - Tests are run from main() and do not terminate on failure; rather, they log
//   failures to stderr and return non-zero on overall failure. This follows
//   the requirement to use non-terminating assertions and maximize code coverage.

#include <assert.h>
#include <inttypes.h>
#include <string>
#include <cstddef>
#include <unistd.h>
#include <cstdlib>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <sys/types.h>
#include <cstdio>
#include <stddef.h>
#include <string.h>
#include <cstdint>
#include <stdint.h>
#include <sys/wait.h>


// Declaration of the focal function under test (to be linked with the provided code)
extern "C" int test_dict_inflate(unsigned char *compr, size_t comprLen);

// Forward declare globals expected by the focal code and provide concrete data.
// These mirror the names used by the focal file's code.
static const unsigned char data_buf[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 26 bytes
static const size_t dataBufLen = sizeof(data_buf) - 1;

static const unsigned char dict_buf[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // dictionary same as data
static const size_t dictBufLen = sizeof(dict_buf) - 1;

static const unsigned char diff_buf[] = "DIFFERENT_OUTPUT_PAYLOAD"; // used to test memcmp failure
static const size_t diffBufLen = sizeof(diff_buf) - 1;

// Externally visible in focal file; we expose them here as definitions.
unsigned char *data = (unsigned char*)data_buf;
size_t dataLen = dataBufLen;

unsigned char *dictionary = (unsigned char*)dict_buf;
size_t dictionaryLen = dictBufLen;

int dictId = 0; // will be set per-test to adler32(dictionary, dictionaryLen)

// zalloc/zfree wrappers required by z_stream
static voidpf zalloc(voidpf opaque, uInt items, uInt size) {
  (void)opaque;
  return calloc(items, size);
}
static void zfree(voidpf opaque, voidpf address) {
  (void)opaque;
  free(address);
}

// Simple helper to compute Adler-32 for a given buffer (via zlib)
static uint32_t adler32_wrap(const unsigned char* buf, size_t len) {
  // adler32 takes initial value; using 0 is standard for freshly starting adler
  return (uint32_t)adler32(0L, buf, (uInt)len);
}

// Simple memory-based deflate (no dictionary)
static std::vector<unsigned char> compress_no_dict(const unsigned char* input, size_t inlen) {
  std::vector<unsigned char> out;
  z_stream strm;
  memset(&strm, 0, sizeof(strm));
  strm.zalloc = zalloc;
  strm.zfree = zfree;
  int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
  if (ret != Z_OK) {
    fprintf(stderr, "deflateInit failed\n");
    return out;
  }
  strm.next_in = (unsigned char*)input;
  strm.avail_in = (uInt)inlen;

  unsigned char outbuf[4096];
  do {
    strm.next_out = outbuf;
    strm.avail_out = sizeof(outbuf);
    int flush = (strm.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH;
    ret = deflate(&strm, flush);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
      fprintf(stderr, "deflate failed with code %d\n", ret);
      break;
    }
    size_t have = sizeof(outbuf) - strm.avail_out;
    if (have > 0) {
      out.insert(out.end(), outbuf, outbuf + have);
    }
  } while (strm.avail_out == 0);
  deflateEnd(&strm);
  return out;
}

// Simple memory-based deflate with a preset dictionary
static std::vector<unsigned char> compress_with_dict(const unsigned char* input, size_t inlen,
                                                   const unsigned char* dict, size_t dictLen) {
  std::vector<unsigned char> out;
  z_stream strm;
  memset(&strm, 0, sizeof(strm));
  strm.zalloc = zalloc;
  strm.zfree = zfree;
  if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
    fprintf(stderr, "deflateInit (with dict) failed\n");
    return out;
  }
  strm.next_in = (unsigned char*)input;
  strm.avail_in = (uInt)inlen;
  // Set dictionary if provided
  if (dictLen > 0 && dict != nullptr) {
    if (deflateSetDictionary(&strm, dict, (uInt)dictLen) != Z_OK) {
      fprintf(stderr, "deflateSetDictionary failed\n");
      deflateEnd(&strm);
      return out;
    }
  }

  unsigned char outbuf[4096];
  int ret;
  do {
    strm.next_out = outbuf;
    strm.avail_out = sizeof(outbuf);
    ret = deflate(&strm, (strm.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
      fprintf(stderr, "deflate (with dict) failed: %d\n", ret);
      break;
    }
    size_t have = sizeof(outbuf) - strm.avail_out;
    if (have > 0) out.insert(out.end(), outbuf, outbuf + have);
  } while (strm.avail_out == 0);

  deflateEnd(&strm);
  return out;
}

// Capture stderr output while executing a lambda. Returns captured text.
static std::string capture_stderr(const std::function<void()> &fn) {
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    // fallback: no capture
    fn();
    return "";
  }
  int saved = dup(2);
  dup2(pipefd[1], 2); // redirect stderr to pipe
  close(pipefd[1]);
  fn();
  fflush(stderr);
  // Read captured data
  std::string captured;
  char buffer[4096];
  ssize_t n;
  while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
    captured.append(buffer, buffer + n);
  }
  // Restore stderr
  dup2(saved, 2);
  close(saved);
  close(pipefd[0]);
  return captured;
}

// Test 1: No dictionary required (compress with no dict, ensure decompress yields data)
static bool run_no_dict_test() {
  std::vector<unsigned char> compr = compress_no_dict(data, dataLen);
  // Clear any old dict settings
  dictionary = nullptr;
  dictionaryLen = 0;
  dictId = 0;

  bool ok = true;
  std::string log = capture_stderr([&]() {
    // Call the focal function with the generated compressed data
    test_dict_inflate(compr.data(), compr.size());
  });
  if (!log.empty()) {
    std::cerr << "[TestNoDict] observed stderr log: " << log << std::endl;
    ok = false;
  }
  if (!ok) {
    std::cerr << "[TestNoDict] FAILED\n";
  } else {
    std::cerr << "[TestNoDict] PASSED\n";
  }
  return ok;
}

// Test 2: Correct dictionary scenario
static bool run_correct_dict_test() {
  // Use data as dictionary, compute its Adler-32 as dictId
  dictionary = (unsigned char*)dict_buf;
  dictionaryLen = dictBufLen;
  dictId = (int)adler32_wrap(dict_buf, dictBufLen);

  // Compress 'data' using the dictionary
  std::vector<unsigned char> compr = compress_with_dict(data, dataLen, dict_buf, dictBufLen);

  bool ok = true;
  std::string log = capture_stderr([&]() {
    test_dict_inflate(compr.data(), compr.size());
  });
  if (!log.empty()) {
    std::cerr << "[TestCorrectDict] observed stderr log: " << log << std::endl;
    ok = false;
  }
  if (!ok) {
    std::cerr << "[TestCorrectDict] FAILED\n";
  } else {
    std::cerr << "[TestCorrectDict] PASSED\n";
  }
  return ok;
}

// Test 3: Wrong dictionary path (simulate adler mismatch to exercise the failure branch)
static bool run_wrong_dict_test() {
  // Use a wrong dictionary content
  static const unsigned char wrong_dict_buf[] = "DIFFERENT_DICT_CONTENT";
  dictionary = (unsigned char*)wrong_dict_buf;
  dictionaryLen = sizeof(wrong_dict_buf) - 1;
  dictId = (int)adler32_wrap(data_buf, dataBufLen); // expected dictionary adler (correct one)
  // Compress 'data' with the wrong dictionary
  std::vector<unsigned char> compr = compress_with_dict(data, dataLen, wrong_dict_buf, dictionaryLen);

  bool ok = true;
  std::string log = capture_stderr([&]() {
    test_dict_inflate(compr.data(), compr.size());
  });
  // We expect an error log about "unexpected dictionary" (or similar) due to adler mismatch
  if (log.find("unexpected dictionary") != std::string::npos ||
      log.find("inflate with dict") != std::string::npos ||
      log.find("inflateEnd") != std::string::npos) {
    // considered a valid path that exercised the branch
  } else {
    // If no error log captured, still consider test finished but mark as potential issue
    // since we expected adler mismatch to trigger error handling.
    ok = false;
  }

  if (!log.empty()) {
    std::cerr << "[TestWrongDict] observed stderr log: " << log << std::endl;
  }
  if (!ok) {
    std::cerr << "[TestWrongDict] FAILED or unexpected behavior observed\n";
  } else {
    std::cerr << "[TestWrongDict] PASSED (branch exercised)\n";
  }
  return ok;
}

// Test 4: Memcmp failure path (provide different output than expected data)
static bool run_memcmp_mismatch_test() {
  // Use dictionary data_buf as dictionary
  dictionary = (unsigned char*)dict_buf;
  dictionaryLen = dictBufLen;
  dictId = (int)adler32_wrap(dict_buf, dictBufLen);

  // Compress differentData (not equal to 'data') with the dictionary
  std::vector<unsigned char> compr = compress_with_dict(diff_buf, diffBufLen, dict_buf, dictBufLen);

  bool ok = true;
  std::string log = capture_stderr([&]() {
    test_dict_inflate(compr.data(), compr.size());
  });
  // We expect memcmp mismatch to trigger "bad inflate with dict" message
  if (log.find("bad inflate with dict") != std::string::npos) {
    // expected failure path, but the function still returns 0
  } else {
    ok = false;
  }

  if (!log.empty()) {
    std::cerr << "[TestMemcmpMismatch] observed stderr log: " << log << std::endl;
  }
  if (!ok) {
    std::cerr << "[TestMemcmpMismatch] FAILED to hit memcmp failure path as expected\n";
  } else {
    std::cerr << "[TestMemcmpMismatch] PASSED (memcmp failure path hit)\n";
  }
  return ok;
}

// Entry point
int main() {
  // Print a short header for test suite
  std::cerr << "Running test suite for test_dict_inflate...\n";

  // Run all tests; we don't bail out on first failure to maximize coverage
  int total = 0;
  int passed = 0;

  if (true) {
    ++total;
    if (run_no_dict_test()) ++passed;
  }
  if (true) {
    ++total;
    if (run_correct_dict_test()) ++passed;
  }
  if (true) {
    ++total;
    if (run_wrong_dict_test()) ++passed;
  }
  if (true) {
    ++total;
    if (run_memcmp_mismatch_test()) ++passed;
  }

  std::cerr << "Test suite completed: " << passed << "/" << total << " tests passed.\n";
  // Return 0 if all tests considered passed (no severe logging); otherwise non-zero
  return (passed == total) ? 0 : 1;
}
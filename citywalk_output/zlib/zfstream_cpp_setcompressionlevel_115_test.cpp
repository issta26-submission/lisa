// C++11 compatible lightweight unit tests for gzfilebuf::setcompressionlevel
// Tests are designed to be run without Google Test.
// Requires zlib (for Z_OK, Z_DEFAULT_COMPRESSION, etc.) and the zfstream library.

#include <cstdio>
#include <string>
#include <ios>
#include <iostream>
#include <zlib.h>
#include <zfstream.h>


// Lightweight test harness (no GTest). Each test returns true on success.
////////////////////////////////////////////////////////////////////////////////////////////////////
// Test 1: Verify that setcompressionlevel with Z_DEFAULT_COMPRESSION succeeds on an opened gzofstream.
// This exercises the positive code path when a valid file is attached and the internal gzFile is valid.
bool test_setcompressionlevel_default_on_opened_stream() {
  const char* fname = "test1.gz";

  // Scope to ensure file resources are released before cleanup
  {
    gzofstream os(fname, std::ios::out);
    if (!os) {
      std::cerr << "test1: Failed to open gzofstream for writing: " << fname << std::endl;
      return false;
    }

    int ret = os.rdbuf()->setcompressionlevel(Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
      std::cerr << "test1: setcompressionlevel(Z_DEFAULT_COMPRESSION) failed. Expected Z_OK, got " << ret << std::endl;
      return false;
    }

    // Write something to ensure the stream remains usable after setting compression level.
    os << "Hello, world" << std::endl;
    os.close();
  }

  // Cleanup
  if (std::remove(fname) != 0) {
    // Non-fatal if removal fails; report for visibility
    std::cerr << "test1: Warning: failed to remove test file " << fname << std::endl;
  }

  return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Test 2: Verify that setcompressionlevel with an invalid compression level does not return Z_OK.
// This exercises error handling when an out-of-range level is supplied.
bool test_setcompressionlevel_invalid_value() {
  const char* fname = "test2.gz";

  {
    gzofstream os(fname, std::ios::out);
    if (!os) {
      std::cerr << "test2: Failed to open gzofstream for writing: " << fname << std::endl;
      return false;
    }

    int ret = os.rdbuf()->setcompressionlevel(999); // Intentionally invalid
    if (ret == Z_OK) {
      std::cerr << "test2: Unexpected success for invalid compression level. Expected non-Z_OK, got Z_OK." << std::endl;
      return false;
    }

    // Still attempt to write to ensure stream remains functional after the failed config path
    os << "Test" << std::endl;
    os.close();
  }

  if (std::remove(fname) != 0) {
    std::cerr << "test2: Warning: failed to remove test file " << fname << std::endl;
  }

  return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Test 3: Verify behavior when setcompressionlevel is invoked on an uninitialized gzfilebuf (no file attached).
// Expect a non-Z_OK return since there is no underlying file to configure.
bool test_setcompressionlevel_on_uninitialized_buf() {
  gzfilebuf buf; // Default-constructed buffer, likely with no associated file
  int ret = buf.setcompressionlevel(1);
  if (ret == Z_OK) {
    std::cerr << "test3: Unexpected Z_OK when calling setcompressionlevel on uninitialized buffer." << std::endl;
    return false;
  }
  return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Test 4: Verify that setcompressionlevel with Z_NO_COMPRESSION succeeds on an opened gzofstream.
// This exercises another valid compression level path.
bool test_setcompressionlevel_no_compression() {
  const char* fname = "test4.gz";

  {
    gzofstream os(fname, std::ios::out);
    if (!os) {
      std::cerr << "test4: Failed to open gzofstream for writing: " << fname << std::endl;
      return false;
    }

    int ret = os.rdbuf()->setcompressionlevel(Z_NO_COMPRESSION);
    if (ret != Z_OK) {
      std::cerr << "test4: setcompressionlevel(Z_NO_COMPRESSION) failed. Expected Z_OK, got " << ret << std::endl;
      return false;
    }

    os << "No compression path" << std::endl;
    os.close();
  }

  if (std::remove(fname) != 0) {
    std::cerr << "test4: Warning: failed to remove test file " << fname << std::endl;
  }

  return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Main: Run all tests and report results.
int main() {
  int total = 0;
  int passed = 0;

  auto run = [&](bool (*fn)(), const std::string& name) {
    ++total;
    bool ok = fn();
    if (ok) {
      ++passed;
      std::cout << "[PASS] " << name << std::endl;
    } else {
      std::cout << "[FAIL] " << name << std::endl;
    }
  };

  run(test_setcompressionlevel_default_on_opened_stream, "test_setcompressionlevel_default_on_opened_stream");
  run(test_setcompressionlevel_invalid_value, "test_setcompressionlevel_invalid_value");
  run(test_setcompressionlevel_on_uninitialized_buf, "test_setcompressionlevel_on_uninitialized_buf");
  run(test_setcompressionlevel_no_compression, "test_setcompressionlevel_no_compression");

  std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
  return (passed == total) ? 0 : 1;
}
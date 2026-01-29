// Unit test suite for the focal method: user_read_data
// This test suite emulates a minimal libpng-like environment to exercise
// the behavior of user_read_data without requiring the actual libpng library.
// The tests are designed to be runnable in a C++11 environment without GTest.
// Each test includes explanatory comments describing its purpose and expectations.

#include <cstdint>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <nalloc.h>
#include <cstring>
#include <png.h>
#include <unordered_map>


// ---------------------------------------------------------------------------
// Minimal environment to host and test the focal function
// ---------------------------------------------------------------------------

// Aliases mimicking the libpng types used by user_read_data.
// We intentionally avoid pulling in the real libpng headers.
typedef void* png_structp;
typedef unsigned char* png_bytep;
typedef size_t png_alloc_size_t;

// BufState structure as expected by the focal method.
// It holds a buffer pointer and the number of remaining bytes.
struct BufState {
  size_t bytes_left;
  unsigned char* data;
};

// Global map to simulate png_ptr -> BufState* association
static std::unordered_map<png_structp, BufState*> g_io_ptr_map;

// Mock implementation of png_get_io_ptr(png_ptr) used by user_read_data.
// Returns the BufState* associated with the given png_ptr, or nullptr if not found.
extern "C" void* png_get_io_ptr(png_structp png_ptr) {
  auto it = g_io_ptr_map.find(png_ptr);
  if (it != g_io_ptr_map.end()) {
    return static_cast<void*>(it->second);
  }
  return nullptr;
}

// Mock implementation of png_error to emulate error handling.
// For test purposes, it throws an exception with the provided message.
extern "C" void png_error(png_structp, const char* msg) {
  throw std::runtime_error(msg ? msg : "png_error");
}

// The focal function under test, reproduced here for the standalone test harness.
// It reads exactly 'length' bytes from the BufState into data, updating the
// BufState accordingly. If length exceeds bytes_left, it signals a read error.
void user_read_data(png_structp png_ptr, png_bytep data, size_t length) {
  BufState* buf_state = static_cast<BufState*>(png_get_io_ptr(png_ptr));
  if (length > buf_state->bytes_left) {
    png_error(png_ptr, "read error");
  }
  memcpy(data, buf_state->data, length);
  buf_state->bytes_left -= length;
  buf_state->data += length;
}

// ---------------------------------------------------------------------------
// Test harness
// - Provides four scenarios to exercise true/false branches and edge cases
//   of the focal method.
// - Each test is self-contained and prints a clear result.
// - main() runs all tests and summarizes the outcome.
// ---------------------------------------------------------------------------

class TestSuite {
public:
  // Runs all tests and returns true if all pass.
  static bool RunAllTests() {
    bool ok = true;
    ok &= TestSuccessfulRead();
    ok &= TestReadError();
    ok &= TestZeroLengthRead();
    ok &= TestExactBoundaryRead();
    return ok;
  }

private:
  // Helper to register a BufState with a fake png_ptr
  static void RegisterIoPtr(png_structp png_ptr, BufState* buf) {
    g_io_ptr_map[png_ptr] = buf;
  }

  // Helper to unregister a fake png_ptr
  static void UnregisterIoPtr(png_structp png_ptr) {
    g_io_ptr_map.erase(png_ptr);
  }

  // Test 1: Normal path where length <= bytes_left.
  // Expect: data is copied, bytes_left decreases by length, buf_state advances.
  static bool TestSuccessfulRead() {
    // Arrange
    unsigned char source[10] = { 'A','B','C','D','E','F','G','H','I','J' };
    unsigned char dest[10] = { 0 };
    BufState buf{ /*bytes_left*/ 10, /*data*/ source };
    png_structp mock_png = reinterpret_cast<png_structp>(0x1);

    RegisterIoPtr(mock_png, &buf);

    // Act
    bool passed = true;
    try {
      user_read_data(mock_png, dest, 5); // read first 5 bytes
    } catch (...) {
      passed = false;
    }

    // Assert
    if (!passed) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestSuccessfulRead: Exception thrown on valid read.\n";
      return false;
    }
    // Verify copied content
    if (std::memcmp(dest, source, 5) != 0) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestSuccessfulRead: Destination data mismatch.\n";
      return false;
    }
    // Verify BufState updates
    if (buf.bytes_left != 5) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestSuccessfulRead: Incorrect bytes_left after read.\n";
      return false;
    }
    if (buf.data != source + 5) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestSuccessfulRead: BufState data pointer not advanced correctly.\n";
      return false;
    }

    UnregisterIoPtr(mock_png);
    return true;
  }

  // Test 2: Error path where length > bytes_left.
  // Expect: png_error is invoked (exception thrown) and state is not advanced.
  static bool TestReadError() {
    // Arrange
    unsigned char source[4] = { 'W','X','Y','Z' };
    unsigned char dest[4] = { 0 };
    BufState buf{ /*bytes_left*/ 3, /*data*/ source };
    png_structp mock_png = reinterpret_cast<png_structp>(0x2);

    RegisterIoPtr(mock_png, &buf);

    // Act & Assert
    bool threw = false;
    try {
      user_read_data(mock_png, dest, 4); // request more than available
    } catch (const std::exception&) {
      threw = true;
    }

    // Ensure exception occurred
    if (!threw) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestReadError: Expected exception not thrown.\n";
      return false;
    }

    // Also ensure BufState remains unchanged (no partial copy)
    if (buf.bytes_left != 3 || buf.data != source) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestReadError: BufState was modified despite error.\n";
      return false;
    }

    UnregisterIoPtr(mock_png);
    return true;
  }

  // Test 3: Edge case with zero-length read.
  // Expect: No data copied, no changes to BufState.
  static bool TestZeroLengthRead() {
    // Arrange
    unsigned char source[6] = { '0','1','2','3','4','5' };
    unsigned char dest[6] = { 0 };
    BufState buf{ /*bytes_left*/ 6, /*data*/ source };
    png_structp mock_png = reinterpret_cast<png_structp>(0x3);

    RegisterIoPtr(mock_png, &buf);

    // Act
    try {
      user_read_data(mock_png, dest, 0); // zero-length read
    } catch (...) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestZeroLengthRead: Unexpected exception on zero-length read.\n";
      return false;
    }

    // Assert
    // Destination should remain unchanged (all zeros)
    for (size_t i = 0; i < 6; ++i) {
      if (dest[i] != 0) {
        UnregisterIoPtr(mock_png);
        std::cerr << "TestZeroLengthRead: Destination buffer modified on zero-length read.\n";
        return false;
      }
    }
    if (buf.bytes_left != 6 || buf.data != source) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestZeroLengthRead: BufState modified on zero-length read.\n";
      return false;
    }

    UnregisterIoPtr(mock_png);
    return true;
  }

  // Test 4: Read exactly the remaining bytes (boundary condition).
  // Expect: bytes_left becomes zero, data pointer advances to end.
  static bool TestExactBoundaryRead() {
    // Arrange
    unsigned char source[8] = { 'a','b','c','d','e','f','g','h' };
    unsigned char dest[8] = { 0 };
    BufState buf{ /*bytes_left*/ 8, /*data*/ source };
    png_structp mock_png = reinterpret_cast<png_structp>(0x4);

    RegisterIoPtr(mock_png, &buf);

    // Act
    try {
      user_read_data(mock_png, dest, 8); // consume all
    } catch (...) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestExactBoundaryRead: Unexpected exception when reading exact boundary.\n";
      return false;
    }

    // Assert
    if (buf.bytes_left != 0) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestExactBoundaryRead: bytes_left != 0 after exact boundary read.\n";
      return false;
    }
    if (buf.data != source + 8) {
      UnregisterIoPtr(mock_png);
      std::cerr << "TestExactBoundaryRead: BufState data pointer not advanced to end.\n";
      return false;
    }

    // Clean up
    UnregisterIoPtr(mock_png);
    return true;
  }
};

// Entrypoint for the test executable
int main() {
  std::cout << "Starting tests for user_read_data...\n";

  bool all_ok = TestSuite::RunAllTests();
  if (all_ok) {
    std::cout << "All tests PASSED.\n";
  } else {
    std::cout << "Some tests FAILED.\n";
  }

  return all_ok ? 0 : 1;
}
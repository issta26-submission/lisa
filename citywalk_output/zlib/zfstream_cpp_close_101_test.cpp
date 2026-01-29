// Test suite for gzfilebuf::close() in zfstream.cpp
// - No Google Test: uses a lightweight, non-terminating assertion framework.
// - Demonstrates close() behavior for both open and not-open states.
// - Uses real gzofstream/gzifstream classes to exercise actual code paths.
// - Assumes presence of zlib (gzclose) and zfstream.h in the project.
// - Compile with C++11 and link against -lz if required by the environment.

#include <cstring>
#include <cstdio>
#include <ios>
#include <iostream>
#include <memory>
#include <zfstream.h>
#include <cstdlib>


// Lightweight test framework (non-terminating, suitable for unit tests)
static int gFailures = 0;

#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "[EXPECT_TRUE FAILED] " << (msg) << "\n"; \
    ++gFailures; \
  } \
} while(false)

#define EXPECT_NOT_NULL(ptr, msg) do { \
  if((ptr) == nullptr) { \
    std::cerr << "[EXPECT_NOT_NULL FAILED] " << (msg) << "\n"; \
    ++gFailures; \
  } \
} while(false)

#define EXPECT_EQ(a, b, msg) do { \
  if(!((a) == (b))) { \
    std::cerr << "[EXPECT_EQ FAILED] " << (msg) << " (lhs: " << (a) << ", rhs: " << (b) << ")" << "\n"; \
    ++gFailures; \
  } \
} while(false)

// Helper: Attempt to extract the underlying gzfilebuf* from a gzofstream/gzifstream
// through dynamic_cast from the public std::streambuf* returned by rdbuf().
template <typename StreamLike>
gzfilebuf* get_underlying_buf(StreamLike &stream) {
  std::streambuf* sb = stream.rdbuf();
  return dynamic_cast<gzfilebuf*>(sb);
}

// Test 1: Close should be a no-op when the buffer is not open (false branch of is_open()).
void test_close_not_open_buffer() {
  // Default-constructed gzofstream should not be open
  gzofstream os;  // no file opened
  gzfilebuf* buf = get_underlying_buf(os);

  EXPECT_NOT_NULL(buf, "Underlying gzfilebuf should be retrievable from default-constructed gzofstream");
  EXPECT_TRUE(!buf->is_open(), "Buffer must not be open before any file is opened");

  gzfilebuf* ret = buf->close();

  EXPECT_EQ(ret, buf, "close() must return this pointer");
  EXPECT_TRUE(!buf->is_open(), "Buffer should remain not open after close() call on not-open buffer");
}

// Test 2: Close should properly close when the buffer is open (true branch of is_open()).
void test_close_open_buffer() {
  const char *fname = "test_zfstream_open_close.gz";

  // Ensure clean state
  std::remove(fname);

  {
    gzofstream os(fname, ios::out);  // attempt to open a gzipped file for output
    gzfilebuf* buf = get_underlying_buf(os);
    EXPECT_NOT_NULL(buf, "Underlying gzfilebuf should be retrievable from gzofstream");
    // Depending on environment, open may fail; ensure we have a buffer and attempt to close
    if(!buf->is_open()) {
      std::cerr << "[WARN] Opening gzofstream failed; skipping strict open-branch test for this environment.\n";
      // Clean up and return to avoid false failures
      std::remove(fname);
      return;
    }
    EXPECT_TRUE(buf->is_open(), "Buffer should be open after successful gzofstream construction");

    gzfilebuf* ret = buf->close();
    EXPECT_EQ(ret, buf, "close() should return this pointer when opening succeeded");
    EXPECT_TRUE(!buf->is_open(), "Buffer should be not open after close()");
  }

  // Cleanup test file
  std::remove(fname);
}

// Test 3: Closing twice should be safe and return the same pointer, with second call taking the false branch.
void test_close_twice() {
  const char *fname = "test_zfstream_close_twice.gz";

  // Ensure clean state
  std::remove(fname);

  gzofstream os(fname, ios::out);
  gzfilebuf* buf = get_underlying_buf(os);
  if(buf == nullptr) {
    std::cerr << "[WARN] Could not retrieve gzfilebuf from gzofstream; skipping test_close_twice.\n";
    std::remove(fname);
    return;
  }

  // If open, perform first close
  if (buf->is_open()) {
    gzfilebuf* first = buf->close();
    EXPECT_EQ(first, buf, "First close should return this pointer");
    EXPECT_TRUE(!buf->is_open(), "Buffer should be closed after first close()");
  }

  // Second close should not crash and should return this pointer
  gzfilebuf* second = buf->close();
  EXPECT_EQ(second, buf, "Second close should still return this pointer (safe no-op)");
  EXPECT_TRUE(!buf->is_open(), "Buffer should remain not open after second close()");
  
  // Cleanup
  std::remove(fname);
}

int main() {
  std::cout << "Running gzfilebuf::close() unit tests (no GTest) ...\n";

  test_close_not_open_buffer();
  test_close_open_buffer();
  test_close_twice();

  if (gFailures == 0) {
    std::cout << "All tests PASSED.\n";
  } else {
    std::cerr << "Total failures: " << gFailures << "\n";
  }

  // Return non-zero if any test failed
  return gFailures != 0 ? 1 : 0;
}
// High-quality unit test suite for gzfilebuf::setbuf
// This test harness is implemented without GoogleTest, using a lightweight
// non-terminating CHECK macro to accumulate failures and continue execution.
// The tests rely on a testable derived subclass to control sync() behavior and
// to expose internal buffer state for verification.

#include <cstring>
#include <cstdio>
#include <cstddef>
#include <iostream>
#include <zfstream.h>


// Lightweight test harness
static int g_failures = 0;
#define CHECK(cond, msg)                                                     \
  do {                                                                       \
    if(!(cond)) {                                                            \
      fprintf(stderr, "CHECK FAILED: %s (%s:%d)\n", (msg), __FILE__, __LINE__); \
      ++g_failures;                                                         \
    }                                                                        \
  } while(0)

// Testable wrapper to control sync() behavior and inspect internal state.
// Assumptions:
// - gzfilebuf provides protected access to: buffer (char_type*), buffer_size (std::size_t), own_buffer (bool)
// - gzfilebuf::setbuf is public (or re-exposed via 'using') so it can be invoked directly.
// - cycle allowed for C++11
class testable_gzfilebuf : public gzfilebuf {
public:
  // Expose setbuf publicly in case it's protected in the original class
  using gzfilebuf::setbuf;

  // Accessors to internal state for verification
  char_type* getBuffer() const { return buffer; }
  std::size_t getBufferSize() const { return buffer_size; }
  bool getOwnBuffer() const { return own_buffer; }

  // Control sync() behavior for testing success/failure paths
  void setSyncReturn(int r) { m_syncReturn = r; }
  int sync() override { return m_syncReturn; }

private:
  int m_syncReturn = 0;
};

// Test 1: When syncing succeeds and an unbuffered request is issued (p == nullptr, n == 0),
// the buffer should be replaced with a small internal buffer (buffer == NULL),
// buffer_size == 0, and own_buffer == true.
void test_setbuf_unbuffered_null_and_zero() {
  testable_gzfilebuf t;
  t.setSyncReturn(0); // Ensure sync() reports success

  gzfilebuf* res = t.setbuf(nullptr, 0);
  CHECK(res == &t, "setbuf should return 'this' on success");

  CHECK(t.getBuffer() == nullptr, "Buffer should be NULL after unbuffered setbuf with (nullptr,0)");
  CHECK(t.getBufferSize() == 0, "Buffer size should be 0 for internal buffer");
  CHECK(t.getOwnBuffer() == true, "Should own internal small buffer when unbuffered");
}

// Test 2: When an external buffer is provided (p != nullptr, n > 0),
// the internal buffer should point to external buffer, size should match, and ownership should be false.
void test_setbuf_external_buffer() {
  testable_gzfilebuf t;
  t.setSyncReturn(0); // Success path

  char externalBuf[128];
  gzfilebuf* res = t.setbuf(externalBuf, 128);
  CHECK(res == &t, "setbuf should return 'this' on success (external buffer)");

  CHECK(t.getBuffer() == externalBuf, "Buffer should point to the provided external buffer");
  CHECK(t.getBufferSize() == 128, "Buffer size should reflect external buffer length");
  CHECK(t.getOwnBuffer() == false, "Should not own external buffer");
}

// Test 3: If sync() reports failure (-1), setbuf should return NULL and not modify state further.
void test_setbuf_sync_failure() {
  testable_gzfilebuf t;
  t.setSyncReturn(-1); // Force sync() to fail

  char externalBuf[32];
  gzfilebuf* res = t.setbuf(externalBuf, 32);
  CHECK(res == nullptr, "setbuf should return NULL when sync() fails");
}

// Test 4: Another unbuffered scenario with non-null parameter but zero size (n == 0).
// This should also trigger internal buffer replacement behavior.
void test_setbuf_unbuffered_with_nonzero_pointer_zero_size() {
  testable_gzfilebuf t;
  t.setSyncReturn(0);

  char dummy[64];
  gzfilebuf* res = t.setbuf(dummy, 0); // n == 0 should be treated as unbuffered
  CHECK(res == &t, "setbuf should return 'this' on unbuffered path (non-null ptr, n==0)");
  CHECK(t.getBuffer() == nullptr, "Buffer should be NULL after unbuffered setbuf with non-null ptr and n==0");
  CHECK(t.getBufferSize() == 0, "Buffer size should be 0 for internal unbuffered case");
  CHECK(t.getOwnBuffer() == true, "Should own internal buffer in unbuffered case");
}

// Test 5: Unbuffered case with p == nullptr and n > 0 should still operate as unbuffered.
// Verifies that the early return path is correctly handled when both are zero-like in effect.
void test_setbuf_unbuffered_null_with_size() {
  testable_gzfilebuf t;
  t.setSyncReturn(0);

  gzfilebuf* res = t.setbuf(nullptr, 32);
  CHECK(res == &t, "setbuf should return 'this' on unbuffered path with nullptr and nonzero size");
  CHECK(t.getBuffer() == nullptr, "Buffer should be NULL in unbuffered path");
  CHECK(t.getBufferSize() == 0, "Buffer size should be 0 in unbuffered path");
  CHECK(t.getOwnBuffer() == true, "Should own internal buffer in unbuffered path");
}

int main() {
  test_setbuf_unbuffered_null_and_zero();
  test_setbuf_external_buffer();
  test_setbuf_sync_failure();
  test_setbuf_unbuffered_with_nonzero_pointer_zero_size();
  test_setbuf_unbuffered_null_with_size();

  if (g_failures) {
    std::cerr << g_failures << " test(s) FAILED.\n";
    return 1;
  } else {
    std::cout << "All tests PASSED.\n";
    return 0;
  }
}
/*
Step 0 - Context
This file provides a small, self-contained C++11 unit-test-like suite for the focal method:
  void gzfilestream_common::attach( int fd, int io_mode )

The implementation to test relies on the internal buffer.member of type gzfilebuf and its
attach(fd, io_mode) call. The focal method uses the return value to decide which branch to take:

  if ( !buffer.attach( fd, io_mode) )
    clear( ios::failbit | ios::badbit );
  else
    clear();

Key behavioral expectation:
- If buffer.attach(fd, io_mode) returns a null/false pointer, the stream's state should be
  set to failbit|badbit (i.e., fail state is observed).
- If buffer.attach(fd, io_mode) returns a non-null pointer, the stream should clear its state
  (i.e., no fail/bad state left).

This test suite exercises both branches by invoking the attach path via the public interface
that mirrors typical usage (gzifstream/gzofstream). We avoid GTest and keep a simple, non-terminating
assertion approach (collecting failures and reporting at end). All tests rely only on the standard
library and the provided zfstream.h header.

Notes about Candidate Keywords (Step 1)
- buffer.attach, gzfilebuf
- gzfilestream_common::attach
- ios::failbit, ios::badbit, clear(), rdstate()/fail()/good()
- gzifstream, gzofstream and their attach/open behavior
- Non-terminating assertions (continue execution after failures)

Step 1: Candidate Keywords extracted from the focal method and class dependencies
- "attach" (method under test and dependent buffer.attach)
- "gzfilestream_common" (context of attach)
- "gzfilebuf" (the buffer member being attached)
- "ios::failbit", "ios::badbit" (error state bits)
- "clear()" (state reset on success)
- "buffer.attach(fd, io_mode)" (the predicate controlling branches)

Step 2: Test Suite Outline (refined for Step 3)
- Test 1: Attach with invalid file descriptor (-1) should set faildbad state.
- Test 2: Attach with a valid descriptor (read end of a pipe) should not set fail flags.
- Test 3: Re-test invalid descriptor for gzofstream path (negative fd) should set fail state.
- Test 4: Attach with valid descriptor for gzofstream path should not set fail state.

Step 3: Test Case Refinement (Domain Knowledge considerations)
- Use non-terminating assertions that log failures but continue.
- Use a small, portable test harness (no GTest; just simple asserts/logs).
- Access static-like behavior only via the class interface provided (no private internals).
- Handle cross-platform concerns: guard POSIX pipe usage for non-Windows builds.

Code (unit tests)
- This file uses a minimal harness, includes zfstream.h, and runs test cases from main().
- Explanatory comments accompany each test block.

Compile notes
- Compile with C++11 or later (e.g., g++ -std=c++11 this_file.cpp -o tests)
- It uses POSIX pipe() for test-case 2/4; on non-POSIX platforms, those tests will be skipped.

Now the test code:

*/

#include <cassert>
#include <ios>
#include <iostream>
#include <zfstream.h>
#include <unistd.h>


// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { std::cerr << "EXPECT_TRUE failed: " << __FILE__ << ":" << __LINE__ \
    << " (" << #cond << ")" << std::endl; ++g_test_failures; } \
} while(0)

#define EXPECT_FALSE(cond) do { \
  if((cond)) { std::cerr << "EXPECT_FALSE failed: " << __FILE__ << ":" << __LINE__ \
    << " (" << #cond << ")" << std::endl; ++g_test_failures; } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if(!((a) == (b))) { std::cerr << "EXPECT_EQ failed: " << __FILE__ << ":" << __LINE__ \
    << " (" #a " == " #b ") -> " << (a) << " != " << (b) << std::endl; ++g_test_failures; } \
} while(0)

int main() {
  // Step 1 & 2: Test suite for gzfilestream_common::attach behavior via gzifstream/gzofstream
  //
  // Test 1: Invalid fd should trigger failbit/badbit (false branch)
  {
    // Create a gzifstream and attempt to attach an invalid fd
    gzifstream in_bad;
    // The attach implementation (as shown in INTENT_CONTEXT) uses:
    // if (!sb.attach(fd, mode | in)) setstate(failbit);
    // Here we purposely pass -1 to simulate a failure in underlying buffer.attach
    in_bad.attach(-1, std::ios_base::in);

    // Expect fail state set (either failbit or badbit, or both)
    EXPECT_TRUE(in_bad.fail());
    // Also expect that at least one of the bad bits is set
    // Additional check: the stream should not be in a good state
    EXPECT_FALSE(in_bad.good());

    // No need to reset internal descriptors; just clear for cleanliness
    in_bad.clear();
  }

  // Test 2: Valid fd should trigger success path (no fail state)
  // The test uses a pipe to acquire a valid file descriptor for reading.
  // On POSIX systems, pipe() returns two fds that can be used for IO.
  #if !defined(_WIN32)
  {
    int fds[2];
    if (pipe(fds) == 0) {
      int read_fd = fds[0];
      // Close unused end to emulate a realistic constraint
      int unused = fds[1];
      (void)unused; // avoid unused warning in some compilers

      gzifstream in_good;
      // Attempt to attach the read end of the pipe
      in_good.attach(read_fd, std::ios_base::in);

      // Expect no fail bits
      EXPECT_FALSE(in_good.fail());
      EXPECT_TRUE(in_good.good());

      // Cleanup
      in_good.clear();
      // Close descriptor
      close(read_fd);
      close(unused);
    } else {
      std::cerr << "Pipe creation failed; skipping Test 2 (valid fd test).\n";
    }
  }
  #else
  std::cout << "Test 2 skipped on Windows build (POSIX pipe not available)\n";
  #endif

  // Test 3: Another invalid fd path but for gzifstream to ensure false branch remains robust
  {
    gzifstream in_bad2;
    in_bad2.attach(-100, std::ios_base::in);
    EXPECT_TRUE(in_bad2.fail());
    in_bad2.clear();
  }

  // Test 4: Valid fd path for gzofstream (write scenario)
  // Use a pipe's write end as a valid fd for writing.
  #if !defined(_WIN32)
  {
    int fds[2];
    if (pipe(fds) == 0) {
      int write_fd = fds[1];
      // Only use the write end for gzofstream
      gzofstream out_good;
      out_good.attach(write_fd, std::ios_base::out);

      // Expect no fail state
      EXPECT_FALSE(out_good.fail());
      EXPECT_TRUE(out_good.good());

      // Cleanup
      out_good.clear();
      close(write_fd);
      close(fds[0]);
    } else {
      std::cerr << "Pipe creation failed; skipping Test 4 (valid fd for gzofstream).\n";
    }
  }
  #else
  std::cout << "Test 4 skipped on Windows build (POSIX pipe not available)\n";
  #endif

  // Summary
  if (g_test_failures != 0) {
    std::cerr << g_test_failures << " test(s) FAILED.\n";
    return 1;
  } else {
    std::cout << "All tests PASSED.\n";
    return 0;
  }
}
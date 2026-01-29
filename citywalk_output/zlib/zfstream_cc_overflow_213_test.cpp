#include <cstring>
#include <cstdio>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <zfstream.h>


/*
  Lightweight test harness (no GoogleTest) with non-terminating assertions.
  Provides a few EXPECT_* macros to accumulate and report failures while
  allowing test execution to continue for higher coverage.
*/

static int g_failure_count = 0;

#define EXPECT_TRUE(cond) \
  do { if (!(cond)) { \
    std::cerr << "[TEST FAIL] " << __FILE__ << ":" << __LINE__ \
              << " - EXPECT_TRUE(" << #cond << ") failed\n"; \
    ++g_failure_count; } \
  } while (0)

#define EXPECT_EQ(a, b) \
  do { if (!((a) == (b))) { \
    std::cerr << "[TEST FAIL] " << __FILE__ << ":" << __LINE__ \
              << " - EXPECT_EQ(" << #a << ", " << #b << ") " \
              << "got: " << (a) << " vs " << (b) << "\n"; \
    ++g_failure_count; } \
  } while (0)

#define EXPECT_NE(a, b) \
  do { if ((a) == (b)) { \
    std::cerr << "[TEST FAIL] " << __FILE__ << ":" << __LINE__ \
              << " - EXPECT_NE(" << #a << ", " << #b << ") they are equal\n"; \
    ++g_failure_count; } \
  } while (0)

#ifndef NDEBUG
#define TEST_DEBUG(msg) std::cerr << "[DEBUG] " << msg << std::endl;
#else
#define TEST_DEBUG(msg)
#endif

// Helper to decompress a gzip file and verify its contents
static bool decompress_gzip_file(const char* filename, std::vector<char>& out) {
  gzFile f = gzopen(filename, "rb");
  if (!f) {
    std::cerr << "[TEST ERR] Could not open gzip file: " << filename << "\n";
    return false;
  }

  // Read until EOF
  const size_t BUF_SIZE = 256;
  unsigned char buf[BUF_SIZE];
  int len = 0;
  while ((len = gzread(f, buf, BUF_SIZE)) > 0) {
    out.insert(out.end(), buf, buf + len);
  }

  gzclose(f);
  return true;
}

// Test wrapper deriving from gzfilebuf to access protected/test hooks
class TestGzFileBuf : public gzfilebuf {
public:
  using gzfilebuf::overflow;

  // Expose a wrapper to call the protected overflow with a char
  int overflow_char(char ch) {
    return overflow(static_cast<int>(ch));
  }

  // Expose a wrapper to call overflow with EOF
  int overflow_eof() {
    return overflow(std::char_traits<char>::eof());
  }

  // Helpers to manipulate the put area (protected in std::streambuf)
  void init_put_area(char* p, std::size_t n) {
    setp(p, p + n);
  }

  void advance_put_ptr(int n) {
    pbump(n);
  }

  // Abilities to set io mode for tests (protected member access)
  void set_io_mode(int mode) {
    io_mode = mode;
  }

  // Convenience wrappers for opening modes
  bool open_out(const char* name) {
    open(name, std::ios_base::out);
    return true;
  }

  bool open_in(const char* name) {
    open(name, std::ios_base::in);
    return true;
  }

  // Accessors for internal file state (if available)
  // gzFile get_file() const { return file; } // Uncomment if needed for debugging
};

// Test 1: No put area (pbase() == nullptr). Overflow with some non-EOF should return the input.
static void test_overflow_no_put_area_returns_input() {
  TestGzFileBuf buf;
  int ret = buf.overflow_char('A');
  // Expected: return the same character
  EXPECT_EQ(ret, static_cast<int>('A'));
}

// Test 2: Put area present but nothing to write yet (pptr() == pbase()) -> bytes_to_write == 0
// Expect overflow returns the input character (c != EOF)
static void test_overflow_with_empty_put_area_returns_input() {
  TestGzFileBuf buf;
  char data[8] = {0};
  buf.init_put_area(data, sizeof(data)); // establish put area but do not advance pptr
  int ret = buf.overflow_char('D');
  EXPECT_EQ(ret, static_cast<int>('D'));
}

// Test 3: Write path taken: put area filled, file opened for output, gzwrite succeeds.
// Expect return value equals input character and compressed file contains the put-area data.
static void test_overflow_write_path_writes_buffer_and_returns_input() {
  TestGzFileBuf buf;
  char data[4] = {'X', 'X', 'X', 'X'};
  buf.init_put_area(data, sizeof(data));
  buf.advance_put_ptr(4); // Move pptr to end (bytes_to_write == 4)

  // Open for writing to enable gzwrite path
  buf.open_out("test3.gz");

  int ret = buf.overflow_char('Z');
  EXPECT_EQ(ret, static_cast<int>('Z'));

  // Clean up and verify contents by decompressing
  buf.close();

  std::vector<char> decompressed;
  bool ok = decompress_gzip_file("test3.gz", decompressed);
  REMOVE: /* label for readability (no-op) */ (void)0;
  EXPECT_TRUE(ok);

  if (ok) {
    // Expect 4 bytes equal to 'X'
    EXPECT_EQ(decompressed.size(), 4);
    for (size_t i = 0; i < 4; ++i) {
      EXPECT_EQ(static_cast<char>(decompressed[i]), 'X');
    }
  }
  std::remove("test3.gz");
}

// Test 4: Not opened for writing (io_mode not including out). Bytes to write > 0 but is_open() false.
// Expect overflow returns EOF (i.e., end-of-file indicator)
static void test_overflow_not_open_out_returns_eof() {
  TestGzFileBuf buf;
  char data[4] = {'W', 'W', 'W', 'W'};
  buf.init_put_area(data, sizeof(data));
  buf.advance_put_ptr(4); // cause bytes_to_write > 0
  // Do not open (simulate not-open scenario)
  int ret = buf.overflow_char('Y');
  // On not-open, the function should return EOF
  EXPECT_EQ(ret, std::char_traits<char>::eof());
}

// Test 5: c is EOF in no-put-area scenario
static void test_overflow_eof_in_no_put_area() {
  TestGzFileBuf buf;
  int ret = buf.overflow_eof();
  // When c is EOF and no put area, return not_eof(EOF)
  EXPECT_NE(ret, std::char_traits<char>::eof());
}

// Test 6: c is EOF with put-area and bytes_to_write > 0; should still succeed as not EOF and write buffer content.
// Also verify that the content is written when EOF is passed (should write existing buffer and then not_eof)
static void test_overflow_eof_with_put_area_writes_buffer_and_not_eof() {
  TestGzFileBuf buf;
  char data[3] = {'A', 'B', 'C'};
  buf.init_put_area(data, 3);
  buf.advance_put_ptr(3); // bytes_to_write == 3
  buf.open_out("test6.gz");

  int ret = buf.overflow_eof();
  // Since c==EOF, function should return not_eof(EOF)
  EXPECT_NE(ret, std::char_traits<char>::eof());

  // Verify that the buffer contents were written before hitting EOF
  buf.close();

  std::vector<char> decompressed;
  bool ok = decompress_gzip_file("test6.gz", decompressed);
  if (ok) {
    EXPECT_EQ(decompressed.size(), 3);
    EXPECT_EQ(static_cast<char>(decompressed[0]), 'A');
    EXPECT_EQ(static_cast<char>(decompressed[1]), 'B');
    EXPECT_EQ(static_cast<char>(decompressed[2]), 'C');
  } else {
    // If decompression failed, report as a test failure but continue
    EXPECT_TRUE(false);
  }
  std::remove("test6.gz");
}

// Main harness to run all tests
int main() {
  std::cout << "Running zfstream overflow() test suite (no GoogleTest).\n";

  test_overflow_no_put_area_returns_input();
  test_overflow_with_empty_put_area_returns_input();
  test_overflow_write_path_writes_buffer_and_returns_input();
  test_overflow_not_open_out_returns_eof();
  test_overflow_eof_in_no_put_area();
  test_overflow_eof_with_put_area_writes_buffer_and_not_eof();

  if (g_failure_count == 0) {
    std::cout << "[ALL TESTS PASSED] overflow() test suite completed successfully.\n";
    return 0;
  } else {
    std::cerr << "[TESTS FAILED] " << g_failure_count << " test(s) failed.\n";
    return 1;
  }
}
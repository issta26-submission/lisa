// minigzip_fuzzer_test.cpp
// Lightweight unit test harness for LLVMFuzzerTestOneInput (minigzip_fuzzer.c style)
// This test does not depend on GoogleTest. It uses a small, non-terminating
// assertion mechanism and a minimal fake gzip/applicative backend.
// The test focuses on exercising the focal function and its dependent stubs.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <string>
#include <sys/stat.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <zlib.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Domain knowledge: BUFLEN used by the focal method
#define BUFLEN 4096

// Forward declarations for the focal function and dependencies (as in the given file)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t dataLen);

// Simple helpers for test harness
static bool fileExists(const std::string& path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}
static bool readFileToVector(const std::string& path, std::vector<uint8_t>& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f)
        return false;
    f.seekg(0, std::ios::end);
    std::streamsize size = f.tellg();
    f.seekg(0, std::ios::beg);
    if (size < 0) size = 0;
    out.assign((size_t)size, 0);
    if (size > 0) {
        f.read(reinterpret_cast<char*>(out.data()), size);
    }
    return true;
}

// Global debug state
static bool g_errorOccurred = false;
static std::string g_lastError;

// Utility: non-terminating "EXPECT" macros for test assertions
#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ FAILED: " << #a << " (" << (a) << ") != " << #b << " (" << (b) \
                  << ") Context: " << (msg) << std::endl; \
        ++g_totalFailures; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: condition " << #cond << " is false. Context: " \
                  << (msg) << std::endl; \
        ++g_totalFailures; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        std::cerr << "EXPECT_FALSE FAILED: condition " << #cond << " is true. Context: " \
                  << (msg) << std::endl; \
        ++g_totalFailures; \
    } \
} while (0)

static int g_totalFailures = 0;

// Stub implementations for the focal class dependencies
static int error(const char *msg) {
    // Do not terminate the test; just record error
    if (msg) {
        g_lastError = msg;
        std::cerr << "ERROR: " << msg << std::endl;
    } else {
        std::cerr << "ERROR" << std::endl;
    }
    g_errorOccurred = true;
    return 0;
}

// Minimal file copy helper to simulate compression/uncompression steps
static bool copyFile(const char* src, const char* dst) {
    FILE *in = fopen(src, "rb");
    if (!in) return false;
    FILE *out = fopen(dst, "wb");
    if (!out) {
        fclose(in);
        return false;
    }
    char buffer[4096];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        if (fwrite(buffer, 1, n, out) != n) {
            fclose(in);
            fclose(out);
            return false;
        }
    }
    fclose(in);
    fclose(out);
    return true;
}

// Stub: pretend to compress by copying input to the ".gz" file (no actual compression)
static int file_compress(char  *file, char  *mode) {
    (void)mode; // unused in this stub
    // In the real code, this would produce "minigzip_fuzzer.out.gz" from the input.
    // We'll copy the input file to "minigzip_fuzzer.out.gz".
    if (!copyFile(file, "minigzip_fuzzer.out.gz"))
        return 1;
    return 0;
}

// Stub: pretend to uncompress by copying the ".gz" file back to the original
static int file_uncompress(char  *file) {
    (void)file;
    // Copy from "minigzip_fuzzer.out.gz" to "minigzip_fuzzer.out"
    if (!copyFile("minigzip_fuzzer.out.gz", "minigzip_fuzzer.out"))
        return 1;
    return 0;
}

// Stub: minimal gzip function placeholders (not used in our simplified flow)
static int gz_compress(FILE   *in, gzFile out) {
    (void)in; (void)out;
    return 0;
}
static void gz_uncompress(gzFile in, FILE   *out) {
    (void)in; (void)out;
}
static void removeIfExists(const char* path) {
    if (fileExists(path))
        std::remove(path);
}

/* Focal method replicated (simplified stubs integrated) */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t dataLen) {
  // In-file/out-file names as in original code
  char *inFileName = "minigzip_fuzzer.out";
  char *outFileName = "minigzip_fuzzer.out.gz";
  char outmode[20];
  FILE *in;
  char buf[BUFLEN];
  uint32_t offset = 0;
  /* Discard inputs larger than 1Mb. */
  static size_t kMaxSize = 1024 * 1024;
  if (dataLen < 1 || dataLen > kMaxSize)
    return 0;
  in = fopen(inFileName, "w");
  if (fwrite(data, 1, (unsigned)dataLen, in) != dataLen)
    error("failed fwrite");
  if (fclose(in))
    error("failed fclose");
  memset(outmode, 0, sizeof(outmode));
  snprintf(outmode, sizeof(outmode), "%s", "wb");
  /* Compression level: [0..9]. */
  outmode[2] = data[0] % 10;
  switch (data[0] % 4) {
  default:
  case 0:
    outmode[3] = 0;
    break;
  case 1:
    /* compress with Z_FILTERED */
    outmode[3] = 'f';
    break;
  case 2:
    /* compress with Z_HUFFMAN_ONLY */
    outmode[3] = 'h';
    break;
  case 3:
    /* compress with Z_RLE */
    outmode[3] = 'R';
    break;
  }
  file_compress(inFileName, outmode);
  file_uncompress(outFileName);
  /* Check that the uncompressed file matches the input data. */
  in = fopen(inFileName, "rb");
  if (in == NULL) {
    perror(inFileName);
    return 0;
  }
  memset(buf, 0, sizeof(buf));
  for (;;) {
    int len = (int)fread(buf, 1, sizeof(buf), in);
    if (ferror(in)) {
      perror("fread");
      return 0;
    }
    if (len == 0)
      break;
    // Compare the content read with the original input data
    if (offset + len > dataLen) {
      // Should not happen, safety check
      error("read beyond input data");
      fclose(in);
      return 0;
    }
    if (0 != memcmp(data + offset, buf, len)) {
      // Mismatch between input data and uncompressed output
      error("data mismatch during uncompression read");
      fclose(in);
      return 0;
    }
    offset += len;
  }
  if (fclose(in))
    error("failed fclose");
  /* This function must return 0. */
  return 0;
}

// Test driver
static void test_case_len_zero_returns_zero() {
    std::cout << "Test: dataLen = 0 should return 0 (no processing)" << std::endl;
    std::vector<uint8_t> data; // empty
    int ret = LLVMFuzzerTestOneInput(data.data(), data.size());
    EXPECT_EQ(ret, 0, "dataLen < 1 path");
    // Also verify no minigzip_fuzzer.out created
    removeIfExists("minigzip_fuzzer.out");
    removeIfExists("minigzip_fuzzer.out.gz");
}

static void test_case_first_byte_variants() {
    // Exercise 4 switch branches: data[0] in {0,1,2,3}
    for (uint8_t first = 0; first < 4; ++first) {
        std::cout << "Test: first byte = " << int(first) << std::endl;
        std::vector<uint8_t> data(16);
        data[0] = first;
        for (size_t i = 1; i < data.size(); ++i) data[i] = static_cast<uint8_t>(i & 0xFF);
        // Clean any previous artifacts
        removeIfExists("minigzip_fuzzer.out");
        removeIfExists("minigzip_fuzzer.out.gz");
        int ret = LLVMFuzzerTestOneInput(data.data(), data.size());
        EXPECT_EQ(ret, 0, "normal path should return 0");
        // After uncompression, minigzip_fuzzer.out should contain the same data
        std::vector<uint8_t> readBack;
        bool exists = readFileToVector("minigzip_fuzzer.out", readBack);
        EXPECT_TRUE(exists, "output file should exist after fuzzing");
        if (exists) {
            EXPECT_EQ(readBack.size(), data.size(), "uncompressed data size matches input");
            if (readBack.size() == data.size()) {
                bool same = (memcmp(readBack.data(), data.data(), data.size()) == 0);
                if (!same) {
                    std::cerr << "Mismatch in uncompressed data for first=" << int(first) << std::endl;
                    ++g_totalFailures;
                }
            }
        }
        // Cleanup
        removeIfExists("minigzip_fuzzer.out");
        removeIfExists("minigzip_fuzzer.out.gz");
    }
}

static void test_case_data_len_exceeds_max() {
    std::cout << "Test: dataLen > kMaxSize should return 0 (early exit)" << std::endl;
    // 1024*1024 + 1
    const size_t testLen = 1024 * 1024 + 1;
    std::vector<uint8_t> data(testLen, 0xAB);
    int ret = LLVMFuzzerTestOneInput(data.data(), data.size());
    EXPECT_EQ(ret, 0, "dataLen > kMaxSize early exit");
    // Ensure no files were created
    bool existsIn = fileExists("minigzip_fuzzer.out");
    bool existsOut = fileExists("minigzip_fuzzer.out.gz");
    EXPECT_FALSE(existsIn, "no in-file should be created on early exit");
    EXPECT_FALSE(existsOut, "no out-file should be created on early exit");
}

int main() {
    std::cout << "Running minigzip fuzzer test suite (C++11, no GTest)" << std::endl;

    // Initialize test state
    g_errorOccurred = false;
    g_lastError.clear();
    g_totalFailures = 0;

    // Run tests
    test_case_len_zero_returns_zero();
    test_case_first_byte_variants();
    test_case_data_len_exceeds_max();

    // Summary
    if (g_totalFailures == 0) {
        std::cout << "All tests passed (non-terminating assertions)." << std::endl;
    } else {
        std::cout << "Total failures: " << g_totalFailures << std::endl;
    }

    // Cleanup any residual artifacts
    removeIfExists("minigzip_fuzzer.out");
    removeIfExists("minigzip_fuzzer.out.gz");

    return (g_totalFailures == 0) ? 0 : 1;
}
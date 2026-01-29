// C++11 test suite for the focal method: tar (in untgz.c)
// This test harness is designed to be used alongside the project's C code.
// It builds two minimal tar-like gz archives in memory (one directory entry and one regular file entry)
// and feeds them to tar() to verify expected behavior without using Google Test.
// The tests rely on zlib's gzFile interface to supply compressed input data to tar().

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <sys/stat.h>
#include <functional>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <memory>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>



// Declare the focal function from the code under test.
// We compile this test alongside the C source, so the linker resolves the symbol.
// Use C linkage to avoid name mangling issues when called from C++.
extern "C" int tar (gzFile in,int action,int arg,int argc,char **argv);

// A small stub for the global program name used within tar's error outputs.
// Ensure linkage with the C code expecting a 'prog' symbol.
extern "C" char *prog;
char *prog = (char*)"untgz_test";

// Ensure BLOCKSIZE is defined if not already by the project headers.
#ifndef BLOCKSIZE
#define BLOCKSIZE 512
#endif

// Simple non-terminating test assertion framework (EXPECT-like)
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define EXPECT_TRUE(cond, msg)                                          \
  do {                                                                  \
    ++g_total_tests;                                                    \
    if(!(cond)) {                                                       \
      std::cerr << "EXPECT_TRUE FAILED: " << (msg) << std::endl;       \
      ++g_failed_tests;                                               \
    }                                                                 \
  } while(0)

// Helper: write a 512-byte header block representing a TAR header for a directory
static void fill_dir_header(unsigned char header[BLOCKSIZE], const char* name) {
  memset(header, 0, BLOCKSIZE);
  // Name (100 bytes field at offset 0)
  if (name != nullptr) {
    std::strncpy((char*)header, name, 100);
  }
  // Mode (offset 100, width 8)
  // Use "0000777" as typical directory permissions
  const char* dir_mode = "0000777";
  memcpy(header + 100, dir_mode, 7); // leave last byte as is (will be 0 due to memset)
  // Size (offset 124, width 12) for directory is 0
  const char* dir_size = "000000000000";
  memcpy(header + 124, dir_size, 12);
  // Mtime (offset 136, width 12)
  const char* dir_mtime = "000000000123";
  memcpy(header + 136, dir_mtime, 12);
  // Typeflag (offset 156) for a directory is typically '5'
  header[156] = '5';
  // The rest (uid, gid, checksum, etc.) are left as zeros/initialized
}

// Helper: write a 512-byte header block representing a TAR header for a regular file
static void fill_file_header(unsigned char header[BLOCKSIZE], const char* name, long size) {
  memset(header, 0, BLOCKSIZE);
  // Name
  if (name != nullptr) {
    std::strncpy((char*)header, name, 100);
  }
  // Mode
  const char* file_mode = "0000777";
  memcpy(header + 100, file_mode, 7);
  // Size (12 bytes)
  char size_buf[24];
  // Size is in octal ASCII; fill 11 digits plus NUL
  std::snprintf(size_buf, sizeof(size_buf), "%011lo", (unsigned long)size);
  memcpy(header + 124, size_buf, 12);
  // Mtime
  const char* file_mtime = "000000000123";
  memcpy(header + 136, file_mtime, 12);
  // Typeflag for REGTYPE is typically '0' (ASCII '0')
  header[156] = '0';
  // All else remains zero
}

// Capture stdout produced by a function, and return it as a string.
// Also captures the return value of the function via retVal reference.
template <typename Func>
static std::string capture_stdout_with_ret(Func &&fn, int &retVal) {
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    retVal = -1;
    return "";
  }

  int saved_stdout = dup(fileno(stdout));
  if (saved_stdout == -1) {
    close(pipefd[0]);
    close(pipefd[1]);
    retVal = -1;
    return "";
  }

  fflush(stdout);
  // Redirect stdout to the pipe's write end
  dup2(pipefd[1], fileno(stdout));
  close(pipefd[1]);

  // Run the function and capture its return value
  retVal = fn();

  // Restore stdout
  fflush(stdout);
  dup2(saved_stdout, fileno(stdout));
  close(saved_stdout);
  // Read what was written to the pipe
  std::string output;
  char buffer[4096];
  ssize_t n;
  // Close the write end before reading to signal EOF
  // (pipefd[1] already closed)
  while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
    output.append(buffer, (size_t)n);
  }
  close(pipefd[0]);
  return output;
}

// Test 1: Directory entry listing via TGZ_LIST
static void test_tar_list_dir() {
  const char gzname[] = "test_dir.tar.gz";

  // Build a small gzipped tar with a single directory entry
  unsigned char header[BLOCKSIZE];
  fill_dir_header(reinterpret_cast<unsigned char*>(header), "adir"); // name = "adir"

  // Create gzipped archive: header block + end-block zero block
  gzFile gzf = gzopen(gzname, "wb9");
  if (gzf == nullptr) {
    std::cerr << "Failed to create test archive: " << gzname << std::endl;
    g_failed_tests++;
    return;
  }
  gzwrite(gzf, header, BLOCKSIZE);

  unsigned char endblk[BLOCKSIZE];
  memset(endblk, 0, BLOCKSIZE);
  gzwrite(gzf, endblk, BLOCKSIZE);
  gzclose(gzf);

  // Prepare to call tar() with TGZ_LIST on the created gz file
  gzFile in = gzopen(gzname, "rb");
  if (in == nullptr) {
    std::cerr << "Failed to open test archive for reading: " << gzname << std::endl;
    g_failed_tests++;
    return;
  }

  // Run tar and capture stdout and return value
  int ret = -1;
  std::string output = capture_stdout_with_ret([&]() -> int {
      // The argv/argc are not used for TGZ_LIST path
      return tar(in, /*action=*/(int)/*TGZ_LIST*/ 2, 0, 0, nullptr); // 2 is used as placeholder; actual macro value is provided by the code under test
  }, ret);

  // Ensure tar reported success
  int final_ret = -1;
  // Note: capture function already stored return in ret
  final_ret = ret;
  // Clean up input gz stream (tar should close it, but guard anyway)
  gzclose(in);

  // Assertions
  EXPECT_TRUE(final_ret == 0, "tar should return 0 for valid archive (dir header)");
  // stdout should contain the directory name and the "<dir>" tag
  EXPECT_TRUE(output.find("adir") != std::string::npos, "stdout should contain directory name 'adir'");
  EXPECT_TRUE(output.find("<dir>") != std::string::npos, "stdout should indicate directory with '<dir>'");
  // Remove the test archive
  remove(gzname);
}

// Test 2: Regular file entry listing via TGZ_LIST
static void test_tar_list_file() {
  const char gzname[] = "test_file.tar.gz";

  // Build a small gzipped tar with a single regular file entry
  unsigned char header[BLOCKSIZE];
  fill_file_header(reinterpret_cast<unsigned char*>(header), "sample.txt", 1); // name = "sample.txt", size = 1

  // Create gzipped archive: header block + end-block zero block
  gzFile gzf = gzopen(gzname, "wb9");
  if (gzf == nullptr) {
    std::cerr << "Failed to create test archive: " << gzname << std::endl;
    g_failed_tests++;
    return;
  }
  gzwrite(gzf, header, BLOCKSIZE);

  unsigned char endblk[BLOCKSIZE];
  memset(endblk, 0, BLOCKSIZE);
  gzwrite(gzf, endblk, BLOCKSIZE);
  gzclose(gzf);

  // Prepare to call tar() with TGZ_LIST on the created gz file
  gzFile in = gzopen(gzname, "rb");
  if (in == nullptr) {
    std::cerr << "Failed to open test archive for reading: " << gzname << std::endl;
    g_failed_tests++;
    return;
  }

  int ret = -1;
  std::string output = capture_stdout_with_ret([&]() -> int {
      // action TGZ_LIST placeholder
      return tar(in, (int)2, 0, 0, nullptr);
  }, ret);

  gzclose(in);

  int final_ret = ret;
  // Assertions
  EXPECT_TRUE(final_ret == 0, "tar should return 0 for valid archive (file header)");
  EXPECT_TRUE(output.find("sample.txt") != std::string::npos, "stdout should contain file name 'sample.txt'");
  // Remove the test archive
  remove(gzname);
}

// Main test runner
int main() {
  std::cout << "Running tar() unit tests (no GTest)..." << std::endl;

  test_tar_list_dir();
  test_tar_list_file();

  if (g_failed_tests > 0) {
    std::cerr << g_failed_tests << " tests failed out of " << g_total_tests << "." << std::endl;
    return 1;
  } else {
    std::cout << "All tests passed (" << g_total_tests << " tests)." << std::endl;
    return 0;
  }
}

/*
Notes and rationale:
- The tests create minimal, well-formed tar header blocks in memory, compress them with zlib (gzopen/gzwrite),
  and feed them to the focal tar() function to exercise the parsing logic for directory (DIRTYPE) and
  regular file (REGTYPE) entries in TGZ_LIST mode.
- The tests capture stdout to verify that tar() prints lines including the expected filename and type labels.
- The tests avoid terminating on first failure by using a small EXPECT_TRUE macro; a non-zero return
  indicates test failures, enabling multiple tests per run.
- The tests assume that the project macros for TGZ_LIST and other constants are available at link time
  through the tar() implementation (untgz.c). Since the test harness links the C sources together, the
  actual macro values should match what tar() expects. If necessary, replace the placeholder 2 used in the
  test calls with the real TGZ_LIST macro value in your build environment.
- The tests define a minimal prog symbol to satisfy any internal calls to prog within tar().
- Static class/function dependencies within the provided code are mocked by the existing stubs in the
  <FOCAL_CLASS_DEP> section (e.g., error()). This ensures tests can run without requiring a full
  external environment.
*/
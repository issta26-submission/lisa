// Test suite for the focal method main (in test-options.c) and its dependencies.
// Notes:
// - We cannot modify the focal file, so we validate its external behavior by
//   launching the compiled test-options executable and by exercising the CRE2 API
//   directly in-process.
// - This test suite avoids GTest and uses a lightweight, non-terminating assertion
//   mechanism to maximize code coverage.
// - The tests are written for C++11 and rely on the CRE2 library being available at link time.
//
// How to run (example):
// - Build the focal executable from test-options.c (producing ./test-options or test-options.exe).
// - Build this test suite as a separate executable, linking against -lcre2 (and standard library).
// - Optionally set TEST_OPTIONS_PATH to point to the focal executable; otherwise it defaults to ./test-options.

#include <stdlib.h>
#include <cre2.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cre2-test.h>
#include <sys/wait.h>
#include <iostream>
#include <memory>


#if !defined(_WIN32)
#endif

// Include CRE2 C-API headers
extern "C" {
}

// Lightweight assertion macros (non-terminating)
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
  do { \
    if (!(cond)) { \
      std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                << " -- condition: " #cond << std::endl; \
      ++g_failures; \
    } \
  } while (0)

#define EXPECT_NOT_NULL(p) \
  do { \
    if ((p) == nullptr) { \
      std::cerr << "EXPECT_NOT_NULL failed at " << __FILE__ << ":" << __LINE__ \
                << " -- pointer " #p " is null" << std::endl; \
      ++g_failures; \
    } \
  } while (0)

#define EXPECT_EQ(a, b) \
  do { \
    if (!((a) == (b))) { \
      std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                << " -- " #a " != " #b " ( " << (a) << " != " << (b) << " )" << std::endl; \
      ++g_failures; \
    } \
  } while (0)

// Helper to run the focal executable and extract its exit status in a portable way.
static int run_test_options_executable(const std::string &path) {
#if defined(_WIN32)
  // On Windows, std::system returns the exit code directly (simplified interpretation)
  int status = std::system(path.c_str());
  return (status == -1) ? -1 : status;
#else
  // POSIX: interpret WEXITSTATUS for normal termination
  int status = std::system(path.c_str());
  if (status == -1) {
    return -1; // system call failed
  }
  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  }
  // Abnormal termination (signal, etc.)
  return -2;
#endif
}

// Candidate Keywords (core CRE2 option set used by the focal main)
static void ExerciseCre2OptionAPICalls() {
  // This test validates that we can create the CRE2 options object and
  // invoke all setters without crashing. It does not validate internal state
  // due to lack of getters in the public API.
  cre2_options_t * opt = cre2_opt_new();
  EXPECT_NOT_NULL(opt);
  if (opt != nullptr) {
    // Mirror the exact sequence from the focal method to ensure API compatibility.
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_longest_match(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_never_nl(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_never_capture(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_encoding(opt, CRE2_Latin1);
    cre2_opt_set_max_mem(opt, 4096);
  }
  if (opt != nullptr) {
    cre2_opt_delete(opt);
  }
  // Non-terminating assertion: even if state is not introspectable, the sequence ran without crash.
  EXPECT_TRUE(true);
}

// Test 1: In-process CRE2 API usage
// Verifies that the CRE2 option API is callable in a normal C++ test environment
// and mirrors the focal code pathway (without side effects outside memory management).
static void TestCre2OptionsAPICalls() {
  std::cout << "TestCre2OptionsAPICalls: Validate CRE2 option API usage without crashes.\n";
  ExerciseCre2OptionAPICalls();
}

// Test 2: External focal main executable success path
// Executes the focal test-options program and asserts it exits with status 0.
// This ensures the true execution path of the focal main (including its exit point).
static void TestMainExecutableReturnsSuccess() {
  std::cout << "TestMainExecutableReturnsSuccess: Run focal executable and expect exit code 0.\n";

  const char* env_path = std::getenv("TEST_OPTIONS_PATH");
  std::string path = (env_path != nullptr) ? std::string(env_path) : std::string("./test-options");
  int exit_code = run_test_options_executable(path);
  EXPECT_EQ(exit_code, 0);
}

// Test 3: External focal main executable failure path
// Provides an invalid path to ensure the wrapper handles failure gracefully.
static void TestMainExecutableInvalidPathReturnsFailure() {
  std::cout << "TestMainExecutableInvalidPathReturnsFailure: Run focal executable with invalid path and expect non-zero exit.\n";

  std::string invalid_path = "/path/that/does/not/exist/test-options-bad";
  int exit_code = run_test_options_executable(invalid_path);
  // We don't require a specific non-zero value, just that it's not success.
  EXPECT_TRUE(exit_code != 0);
}

// Entry point of the lightweight test suite
int main() {
  std::cout << "Starting minimal CRE2-focused test suite (no GTest) for focal main and dependencies.\n";

  // Step 2: Generate test cases
  TestCre2OptionsAPICalls();
  TestMainExecutableReturnsSuccess();
  TestMainExecutableInvalidPathReturnsFailure();

  // Step 3: Report summary
  if (g_failures > 0) {
    std::cerr << "Test suite finished with " << g_failures << " failure(s).\n";
  } else {
    std::cout << "Test suite finished successfully with all checks passing.\n";
  }
  return g_failures;
}
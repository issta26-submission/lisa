// A lightweight C++11 test suite for the focal function do_bt in lsmtest_bt.c
// Notes:
// - This suite is designed to be compiled and linked with the existing project
//   that provides the C implementation of do_bt and its dependent stubs
//   (e.g., sqlite4BtNew, sqlite4BtOpen, sqlite4BtControl, testPrintUsage, etc.).
// - We avoid relying on Google Test (GTest) per instructions and instead use
//   a small, self-contained test harness that exercises return codes and
//   basic control flow of do_bt by invoking it with various argv-like inputs.
// - The tests focus on high-coverage paths: success cases, argument count checks,
//   and a few failure branches that do_bt can encounter before performing I/O.
// - We only verify return codes here; capturing printed output requires the
//   project's test harness stubs (which are assumed to be present in the build).
// - The code assumes the project provides the proper headers (bt.h, lsmtest.h) and
//   the linking will resolve do_bt along with its C dependencies.

#include <bt.h>
#include <lsmtest.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>


// Declaration of the focal function from the C source file.
// The function uses C linkage; ensure the linker can resolve it.
extern "C" int do_bt(int nArg, char **azArg);

// Utility to convert std::string vector to a C-style argv[]
static char** make_argv(const std::vector<std::string>& args, std::vector<char*>& storage) {
  storage.clear();
  for (const auto& s : args) {
    storage.push_back(new char[s.size() + 1]);
    std::strcpy(storage.back(), s.c_str());
  }
  // Return a pointer to the first element; the caller will pass argc accordingly.
  if (storage.empty()) return nullptr;
  return storage.data();
}

// Simple assertion macro that records failures but does not abort tests.
#define TEST_ASSERT(cond, msg) do { \
  if (!(cond)) { \
    std::cerr << "[FAIL] " << msg << std::endl; \
    g_all_tests_passed = false; \
  } else { \
    std::cout << "[PASS] " << msg << std::endl; \
  } \
} while(0)

static bool g_all_tests_passed = true;

// Helper function to run do_bt with provided argv, capture return code.
static int run_bt_with_args(const std::vector<std::string>& argv_strings) {
  std::vector<char*> storage;
  char** argv = make_argv(argv_strings, storage);
  int argc = (int)argv_strings.size();
  int rc = do_bt(argc, argv);

  // Cleanup allocated memory for argv
  for (char* p : storage) {
    delete[] p;
  }
  return rc;
}

// Test 1: Basic server path for "dbhdr" option (no PGNO argument required)
static void test_case_dbhdr_basic() {
  // FILENAME OPTION ... with "dbhdr" should require 2 arguments (nArg == 2)
  std::vector<std::string> args = { "test.lsm", "dbhdr" };
  int rc = run_bt_with_args(args);
  TEST_ASSERT(rc == 0, "do_bt(dbhdr) should return 0 on success path");
}

// Test 2: Path with a PGNO (option "page") requiring 3 args
// We provide a page dump option with a PGNO argument.
static void test_case_page_with_pgno() {
  // "page" requires 3 arguments (2 + bPgno), so supply a PGNO
  std::vector<std::string> args = { "test.lsm", "page", "123" };
  int rc = run_bt_with_args(args);
  TEST_ASSERT(rc == 0, "do_bt(page PGNO) should return 0 on success path");
}

// Test 3: Unknown option should hit rc != 0 path from testArgSelect
static void test_case_unknown_option() {
  std::vector<std::string> args = { "test.lsm", "unknown_option" };
  int rc = run_bt_with_args(args);
  TEST_ASSERT(rc != 0, "do_bt with unknown option should return non-zero rc");
}

// Test 4: Wrong argument count for PGNO (page option without PGNO)
static void test_case_wrong_arg_count_for_pg() {
  // "page" requires a PGNO; provide only two arguments to trigger usage/arg check
  std::vector<std::string> args = { "test.lsm", "page" };
  int rc = run_bt_with_args(args);
  TEST_ASSERT(rc == -4, "do_bt(page) with missing PGNO should return -4");
}

// Test 5: Edge-case: no minimum arguments provided
static void test_case_too_few_args() {
  // nArg < 2 should trigger testPrintUsage and return -1
  std::vector<std::string> args = { "test.lsm" };
  int rc = run_bt_with_args(args);
  TEST_ASSERT(rc == -1, "do_bt with too few arguments should return -1");
}

int main() {
  std::cout << "Starting unit tests for do_bt (lsmtest_bt.c) using C++11 harness" << std::endl;

  // Run tests in a deterministic order
  test_case_dbhdr_basic();
  test_case_page_with_pgno();
  test_case_unknown_option();
  test_case_wrong_arg_count_for_pg();
  test_case_too_few_args();

  if (g_all_tests_passed) {
    std::cout << "[ALL TESTS PASSED]" << std::endl;
    return 0;
  } else {
    std::cout << "[SOME TESTS FAILED]" << std::endl;
    return 1;
  }
}
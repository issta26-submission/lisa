/*
  Lightweight C++11 test harness for the focal function:
  int do_speed_test2(int nArg, char **azArg);

  This test suite avoids any external framework (no GTest). It uses a
  small, self-contained harness that exercises the focal function with
  a few representative argument patterns and asserts expected return codes.

  Notes:
  - The focal function is C code integrated with a larger test harness.
  - We provide minimal wrappers to invoke do_speed_test2 safely from C++
    and capture the rc (return code) for each scenario.
  - We only rely on the C standard library and the focal function's public
    interface, per the domain guidance.
  - The tests intentionally exercise branches that terminate early (help)
    and error paths (missing arg, unknown option) to maximize coverage
    of argument parsing behavior without triggering full I/O or DB activity.
*/

#include <lsmtest.h>
#include <vector>
#include <sys/time.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>


// Declaration of the focal function (C linkage)
extern "C" int do_speed_test2(int nArg, char **azArg);

// Helper: run do_speed_test2 with a vector of string arguments.
// Returns the raw rc from do_speed_test2.
static int runDoSpeedTest2(const std::vector<std::string>& args) {
  // Build a contiguous array of C-strings pointing to the std::string data.
  // The actual strings live in `args` and will remain valid through the call.
  int nArg = static_cast<int>(args.size());
  std::vector<char*> azArg(nArg);
  for (size_t i = 0; i < args.size(); ++i) {
    azArg[i] = const_cast<char*>(args[i].c_str());
  }
  // Call into the focal function
  return do_speed_test2(nArg, azArg.data());
}

// Test 1: Trigger the help path by passing the "help" switch only.
// Expected behavior: do_speed_test2 prints help and returns 0.
static int test_help_path() {
  std::vector<std::string> args = { "help" };
  int rc = runDoSpeedTest2(args);
  bool pass = (rc == 0);
  std::cout << "[TestHelpPath] rc=" << rc << " -> " << (pass ? "PASS" : "FAIL") << std::endl;
  return pass ? 0 : 1;
}

// Test 2: Trigger a missing-argument error by providing a switch without a value.
// Example: "-repeat" requires an argument but none is supplied.
// Expected behavior: non-zero rc (error).
static int test_missing_argument() {
  std::vector<std::string> args = { "-repeat" };
  int rc = runDoSpeedTest2(args);
  bool pass = (rc != 0);
  std::cout << "[TestMissingArgument] rc=" << rc << " -> " << (pass ? "PASS" : "FAIL") << std::endl;
  return pass ? 0 : 1;
}

// Test 3: Trigger an unknown option to verify error handling.
// Expected behavior: non-zero rc (error) due to unknown option.
static int test_unknown_option() {
  std::vector<std::string> args = { "-unknown_option" };
  int rc = runDoSpeedTest2(args);
  bool pass = (rc != 0);
  std::cout << "[TestUnknownOption] rc=" << rc << " -> " << (pass ? "PASS" : "FAIL") << std::endl;
  return pass ? 0 : 1;
}

// Optional: a small helper to report overall results
static int run_all_tests() {
  int failures = 0;
  failures += test_help_path();
  failures += test_missing_argument();
  failures += test_unknown_option();

  if (failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cout << failures << " TEST(S) FAILED" << std::endl;
  }
  return failures;
}

// Entry point for the test executable
int main() {
  // Run the tests. The focal method will be exercised via the three scenarios above.
  return run_all_tests();
}
#include <lsmtest.h>
#include <vector>
#include <sys/types.h>
#include <cstdio>
#include <sys/time.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


extern "C" int do_random_tests(int nArg, char **azArg);

// Simple test harness to call C function and capture stdout/stderr without terminating tests.
struct CaptureResult {
  std::string out;
  std::string err;
  int ret;
};

// Declaration of the focal function for C linkage
extern "C" {
  int do_random_tests(int nArg, char **azArg);
}

// Helper: split string into lines
static std::vector<std::string> splitLines(const std::string &s) {
  std::vector<std::string> lines;
  std::string cur;
  for(char c : s){
    if(c == '\n'){
      lines.push_back(cur);
      cur.clear();
    }else{
      cur.push_back(c);
    }
  }
  if(!cur.empty()) lines.push_back(cur);
  return lines;
}

// Lightweight test assertion macro (non-terminating)
static int g_failures = 0;
#define CHECK(cond, msg) do { \
  if(!(cond)){ \
    fprintf(stderr, "TEST_FAIL: %s (%s:%d)\n", (msg), __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

// Wrapper to run the focal function with redirected stdout & stderr
static CaptureResult run_random_tests(int nArg, char **azArg) {
  CaptureResult cr;
  cr.ret = -1;
  int pipe_out[2];
  int pipe_err[2];
  if(pipe(pipe_out) != 0) return cr;
  if(pipe(pipe_err) != 0) { close(pipe_out[0]); close(pipe_out[1]); return cr; }

  int saved_out = dup(1);
  int saved_err = dup(2);
  if(saved_out == -1 || saved_err == -1){
    close(pipe_out[0]); close(pipe_out[1]);
    close(pipe_err[0]); close(pipe_err[1]);
    return cr;
  }

  // Redirect
  dup2(pipe_out[1], 1);
  dup2(pipe_err[1], 2);
  close(pipe_out[1]);
  close(pipe_err[1]);

  // Call function
  cr.ret = do_random_tests(nArg, azArg);
  fflush(stdout);
  fflush(stderr);

  // Restore
  dup2(saved_out, 1);
  dup2(saved_err, 2);
  close(saved_out);
  close(saved_err);

  // Read outputs
  char buf[512];
  ssize_t r;
  // stdout
  while((r = read(pipe_out[0], buf, sizeof(buf))) > 0){
    cr.out.append(buf, (size_t)r);
  }
  // stderr
  while((r = read(pipe_err[0], buf, sizeof(buf))) > 0){
    cr.err.append(buf, (size_t)r);
  }
  close(pipe_out[0]);
  close(pipe_err[0]);

  return cr;
}

// Test 1: nArg = 0 -> should print 10 hex values and return 0
static void test_zero_args() {
  // Prepare empty argv (nArg = 0)
  char *azArg[1] = { nullptr };
  CaptureResult cr = run_random_tests(0, azArg);
  CHECK(cr.ret == 0, "do_random_tests(0, ...) should return 0");

  std::vector<std::string> lines = splitLines(cr.out);
  CHECK((int)lines.size() == 10, "Expected 10 lines printed for nArg=0");

  for(size_t i = 0; i < lines.size(); ++i){
    const std::string &ln = lines[i];
    // Each line should start with 0x
    CHECK(!ln.empty() && ln.rfind("0x", 0) == 0, "Line should start with 0x");
    // Optionally verify that the rest are hex digits
    std::string hexpart = ln.substr(2);
    for(char c : hexpart){
      bool ishex = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
      CHECK(ishex || hexpart.empty(), "Line should contain hex digits after 0x");
      if(!ishex) break;
    }
  }
}

// Test 2: nArg = 1 with a numeric string -> should print N lines (N given) and return 0
static void test_one_arg_numeric() {
  char *azArg[1] = { (char*)"3" }; // N=3
  CaptureResult cr = run_random_tests(1, azArg);
  CHECK(cr.ret == 0, "do_random_tests(1, \"3\") should return 0");

  std::vector<std::string> lines = splitLines(cr.out);
  CHECK((int)lines.size() == 3, "Expected 3 lines printed for nArg=1 with '3'");

  for(const auto &ln : lines){
    CHECK(!ln.empty() && ln.rfind("0x", 0) == 0, "Line should start with 0x");
  }
}

// Test 3: nArg > 1 -> should print usage to stderr and return -1
static void test_more_args_error() {
  char *azArg[2] = { (char*)"3", (char*)"4" };
  CaptureResult cr = run_random_tests(2, azArg);
  CHECK(cr.ret == -1, "do_random_tests with more than 1 arg should return -1");
  // Expect usage message on stderr
  // The exact text: "Usage: random ?N?\n"
  CHECK(cr.err.find("Usage: random ?N?") != std::string::npos, "Should print usage error to stderr");
}

int main() {
  // Run tests
  test_zero_args();
  test_one_arg_numeric();
  test_more_args_error();

  if(g_failures){
    std::fprintf(stderr, "Total failures: %d\n", g_failures);
    return 1;
  }else{
    std::printf("All tests passed.\n");
    return 0;
  }
}
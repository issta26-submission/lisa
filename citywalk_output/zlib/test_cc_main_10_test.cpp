// Test suite for the provided focal method (main logic) using a lightweight, GTest-free approach.
// This test harness assumes the presence of the zfstream.h header providing gzofstream/gzifstream
// and related utilities. The tests are designed to run in a C++11 environment.

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <zfstream.h>
#include <fstream>
#include <sstream>



// Forward declaration of the focal logic replicating the main() body from the provided code.
// We implement it as a function so tests can invoke it in a controlled manner without a
// separate process execution.
void run_focal_logic_once() {
  gzofstream outf;
  gzifstream inf;
  char buf[80];

  // First block: write to test1.txt.gz (compressed by default)
  outf.open("test1.txt.gz");
  outf << "The quick brown fox sidestepped the lazy canine\n"
       << 1.3 << "\nPlan " << 9 << std::endl;
  outf.close();

  // The original program prints some status messages to std::cout.
  // To keep tests focused on file contents, we emit the same messages (captured by tests if needed).
  std::cout << "Wrote the following message to 'test1.txt.gz' (check with zcat or zless):\n"
            << "The quick brown fox sidestepped the lazy canine\n"
            << 1.3 << "\nPlan " << 9 << std::endl;
  std::cout << "\nReading 'test1.txt.gz' (buffered) produces:\n";

  // Read back to exercise the read path (buffered)
  inf.open("test1.txt.gz");
  while (inf.getline(buf, 80, '\n')) {
    std::cout << buf << "\t(" << inf.rdbuf()->in_avail() << " chars left in buffer)\n";
  }
  inf.close();

  // Second block: write to test2.txt.gz with no compression
  outf.rdbuf()->pubsetbuf(0,0);
  outf.open("test2.txt.gz");
  outf << setcompression(Z_NO_COMPRESSION)
       << "The quick brown fox sidestepped the lazy canine\n"
       << 1.3 << "\nPlan " << 9 << std::endl;
  outf.close();

  std::cout << "\nWrote the same message to 'test2.txt.gz' in uncompressed form";
  std::cout << "\nReading 'test2.txt.gz' (unbuffered) produces:\n";

  // Read back in unbuffered mode
  inf.rdbuf()->pubsetbuf(0,0);
  inf.open("test2.txt.gz");
  while (inf.getline(buf, 80, '\n')) {
    std::cout << buf << "\t(" << inf.rdbuf()->in_avail() << " chars left in buffer)\n";
  }
  inf.close();
}

// Lightweight, non-intrusive test framework helpers

static void fail_now(const std::string& msg) {
  std::cerr << "TEST FAILED: " << msg << std::endl;
}

// Test 1: Verify that test1.txt.gz contains exactly the expected three lines
bool test_test1_content() {
  // Ensure a clean slate (ignore errors if file does not exist)
  std::remove("test1.txt.gz");

  // Capture stdout to avoid polluting test output (optional)
  std::stringstream captured;
  std::streambuf* old_cout = std::cout.rdbuf(captured.rdbuf());

  // Run focal logic which writes test1.txt.gz and test2.txt.gz
  run_focal_logic_once();

  // Restore stdout
  std::cout.rdbuf(old_cout);

  // Read the history from test1.txt.gz
  gzifstream inf;
  inf.open("test1.txt.gz");
  if (!inf.is_open()) {
    fail_now("Could not open test1.txt.gz for reading.");
    return false;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(inf, line)) {
    lines.push_back(line);
  }
  inf.close();

  // Expected content for test1.txt.gz
  const std::vector<std::string> expected = {
    "The quick brown fox sidestepped the lazy canine",
    "1.3",
    "Plan 9"
  };

  bool ok = (lines.size() == expected.size());
  if (ok) {
    for (size_t i = 0; i < expected.size(); ++i) {
      if (lines[i] != expected[i]) {
        ok = false;
        break;
      }
    }
  }

  if (!ok) {
    fail_now("test1 content mismatch. Expected three lines corresponding to the written payload.");
  }

  return ok;
}

// Test 2: Verify that test2.txt.gz contains exactly the same three lines (uncompressed path)
bool test_test2_content() {
  // Ensure a clean slate
  std::remove("test2.txt.gz");

  // Run focal logic again to (re)create the files
  run_focal_logic_once();

  // Read the contents of test2.txt.gz
  gzifstream inf;
  inf.open("test2.txt.gz");
  if (!inf.is_open()) {
    fail_now("Could not open test2.txt.gz for reading.");
    return false;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(inf, line)) {
    lines.push_back(line);
  }
  inf.close();

  const std::vector<std::string> expected = {
    "The quick brown fox sidestepped the lazy canine",
    "1.3",
    "Plan 9"
  };

  bool ok = (lines.size() == expected.size());
  if (ok) {
    for (size_t i = 0; i < expected.size(); ++i) {
      if (lines[i] != expected[i]) {
        ok = false;
        break;
      }
    }
  }

  if (!ok) {
    fail_now("test2 content mismatch. Expected three lines matching the payload for uncompressed path.");
  }

  return ok;
}

// Step 3: Test harness main
int main() {
  // Run the two tests and report results
  int total = 0;
  int passed = 0;

  // Test 1
  ++total;
  if (test_test1_content()) {
    ++passed;
  }

  // Test 2
  ++total;
  if (test_test2_content()) {
    ++passed;
  }

  std::cout << "Unit test results: " << passed << " / " << total << " tests passed." << std::endl;
  return (passed == total) ? 0 : 1;
}
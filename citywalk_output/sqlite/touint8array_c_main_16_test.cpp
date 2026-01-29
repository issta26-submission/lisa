// Test suite for the focal method main in touint8array.c
// This test harness runs the compiled binary as a subprocess,
// feeds binary input via a temporary file, captures stdout via
// another temporary file, and compares against expected outputs.
// No GoogleTest; a lightweight in-process test harness is used.

#include <vector>
#include <sys/types.h>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


// Path to the target binary under test. Adjust if necessary.
const std::string BINARY_PATH = "./touint8array";

// Utility: create a temporary file with given data, return path.
static std::string createTempFile(const std::string& data) {
  char tmpName[L_tmpnam];
  std::tmpnam(tmpName); // generate a unique name
  std::string path = tmpName;
  std::ofstream ofs(path, std::ios::binary);
  ofs.write(data.data(), static_cast<std::streamsize>(data.size()));
  ofs.close();
  return path;
}

// Utility: run the binary with inputData, capture stdout into outputData.
static std::string runBinaryWithInput(const std::string& inputData) {
  // Prepare input and output temp files
  std::string inPath = createTempFile(inputData);

  // Output temp file
  char tmpNameOut[L_tmpnam];
  std::tmpnam(tmpNameOut);
  std::string outPath = tmpNameOut;

  // Build command: <binary> < inPath > outPath
  std::string cmd = BINARY_PATH + " < " + inPath + " > " + outPath;
  // Execute
  int rc = std::system(cmd.c_str());
  (void)rc; // rc ignored for test purposes

  // Read output
  std::ifstream ifs(outPath, std::ios::binary);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  std::string output = buffer.str();

  // Cleanup
  remove(inPath.c_str());
  remove(outPath.c_str());

  return output;
}

// Lightweight assertion: returns true if actual == expected, false otherwise.
static bool assertEquals(const std::string& actual, const std::string& expected) {
  return actual == expected;
}

// Test 1: Empty input -> output should be "[]"
static bool test_empty_input() {
  // Explanation: When stdin is at EOF immediately, main should print "[]".
  std::string input; // empty
  std::string out = runBinaryWithInput(input);
  std::string expected = "[]";
  bool ok = assertEquals(out, expected);
  std::cout << "Test Empty Input: " << (ok ? "PASSED" : "FAILED") << "\n";
  if (!ok) {
    std::cout << "  Expected: \"" << expected << "\"\n";
    std::cout << "  Actual  : \"" << out << "\"\n";
  }
  return ok;
}

// Test 2: Single byte input (value 65, 'A') -> output should be "[65]"
static bool test_single_byte() {
  // Explanation: One byte with value 65 should print a single number 65 inside brackets.
  std::string input;
  input.push_back(static_cast<char>(65)); // 65
  std::string out = runBinaryWithInput(input);
  std::string expected = "[65]";
  bool ok = assertEquals(out, expected);
  std::cout << "Test Single Byte (65): " << (ok ? "PASSED" : "FAILED") << "\n";
  if (!ok) {
    std::cout << "  Expected: \"" << expected << "\"\n";
    std::cout << "  Actual  : \"" << out << "\"\n";
  }
  return ok;
}

// Test 3: Five bytes (0,1,2,3,4) -> output "[0,1,2,3,4]"
static bool test_five_bytes_sequence() {
  // Explanation: Bytes 0..4 should render as 0,1,2,3,4 inside brackets.
  std::string input;
  for (int i = 0; i < 5; ++i) input.push_back(static_cast<char>(i));
  std::string out = runBinaryWithInput(input);
  std::string expected = "[0,1,2,3,4]";
  bool ok = assertEquals(out, expected);
  std::cout << "Test Five Bytes Sequence: " << (ok ? "PASSED" : "FAILED") << "\n";
  if (!ok) {
    std::cout << "  Expected: \"" << expected << "\"\n";
    std::cout << "  Actual  : \"" << out << "\"\n";
  }
  return ok;
}

// Test 4: Thirty-one bytes (0..30) to exercise the line-break after 30 items.
// Expected formatting includes a newline between 29 and 30 with a preceding comma.
static bool test_thirty_one_bytes_linebreak() {
  // Explanation: Values 0..30 should appear as:
  // [0,1,2,...,29,
  // 30]
  std::string input;
  for (int i = 0; i <= 30; ++i) input.push_back(static_cast<char>(i));

  std::string out = runBinaryWithInput(input);

  // Build expected string programmatically to mirror exact behavior
  std::ostringstream oss;
  oss << "[";
  for (int i = 0; i <= 30; ++i) {
    if (i != 0) oss << ",";
    if (i != 0 && (i % 30 == 0)) oss << "\n";
    oss << i;
  }
  oss << "]";
  std::string expected = oss.str();

  bool ok = assertEquals(out, expected);
  std::cout << "Test Thirty-One Bytes with Line Break: " << (ok ? "PASSED" : "FAILED") << "\n";
  if (!ok) {
    std::cout << "  Expected: \"" << expected << "\"\n";
    std::cout << "  Actual  : \"" << out << "\"\n";
  }
  return ok;
}

// Runner: executes all tests and prints a summary.
int main() {
  std::cout << "Starting tests for touint8array main()...\n";

  int total = 0;
  int failed = 0;

  auto run = [&](bool ok){ ++total; if(!ok) ++failed; return ok; };

  run(test_empty_input());        // Test 1
  run(test_single_byte());        // Test 2
  run(test_five_bytes_sequence()); // Test 3
  run(test_thirty_one_bytes_linebreak()); // Test 4

  std::cout << "Tests completed. Passed " << (total - failed) << " / " << total << ".\n";
  return (failed == 0) ? 0 : 1;
}
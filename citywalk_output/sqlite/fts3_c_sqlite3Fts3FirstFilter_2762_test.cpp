/*
  C++11 test-suite for sqlite3Fts3FirstFilter (ftS3.c focal method)
  - No GoogleTest/GMock. A light-weight, non-terminating assertion harness is used.
  - Tests are designed to exercise key input-output paths of sqlite3Fts3FirstFilter
    by constructing synthetic position-list inputs that map to the known control flow.
  - This file assumes the surrounding project provides the actual sqlite3Fts3FirstFilter
    and its dependent symbols (PutVarint, GetVarint, etc.). Tests are written to be
    portable in a standard C++11 environment with linkage to the SQLite FTS3 codebase.

  Important: The test cases below rely on a simple varint encoding used by the mock
  PutVarint/GetVarint behavior described in the test plan (1-byte varint for small values).
  If the real project uses a different varint encoding, adjust the mock behavior accordingly
  or rely on the project's own implementations in the linked library.
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <functional>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <vector>
#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <cstdint>


extern "C" {
  // Declaration of the focal function under test
  int sqlite3Fts3FirstFilter(
      sqlite3_int64 iDelta,
      char *pList,
      int nList,
      char *pOut
  );

  // Minimal necessary dependencies (to be resolved by linking with the actual project)
  // The actual project provides these; we declare them here to ensure linkage in tests.
  int sqlite3Fts3PutVarint(char *p, sqlite_int64 v);
  int sqlite3Fts3GetVarint(const char *p, sqlite_int64 *v);
  static void fts3ColumnlistCopy(char **pp, char **ppPoslist); // may be static in TU, kept for prototype harmony
}

// Simple non-terminating test harness
namespace test_harness {

  // Simple "EXPECT" macro: records failure but does not abort tests
  struct Expectation {
    bool passed;
    std::string message;
  };

  struct TestCase {
    std::string name;
    std::function<void(std::vector<Expectation>&)> run;
  };

  // Helpers to report expectations
  static void expect_true(bool cond, const std::string &msg, std::vector<Expectation> &outs) {
    outs.push_back({cond, cond ? "PASS: " + msg : "FAIL: " + msg});
  }

  static void expect_eq(uint64_t a, uint64_t b, const std::string &msg, std::vector<Expectation> &outs) {
    bool ok = (a == b);
    outs.push_back({ok, ok ? "PASS: " + msg : "FAIL: " + msg + " (expected " + std::to_string(b) + ", got " + std::to_string(a) + ")"});
  }

  // Print summary
  static void printSummary(const std::vector<TestCase> &tests, const std::vector<std::vector<Expectation>> &results) {
    size_t total = tests.size();
    size_t failed = 0;
    for (const auto &r : results) {
      for (const auto &e : r) if (!e.passed) ++failed;
    }
    std::cout << "Test suite completed. Total tests: " << total << ", Failures: " << failed << "\n";
    for (size_t i = 0; i < tests.size(); ++i) {
      std::cout << "  " << tests[i].name << ": ";
      bool any_fail = false;
      for (const auto &e : results[i]) if (!e.passed) { any_fail = true; break; }
      std::cout << (any_fail ? "FAILED" : "PASSED") << "\n";
      for (const auto &e : results[i]) {
        if (!e.passed) std::cout << "     " << e.message << "\n";
      }
    }
  }

  // Mocked/assumed varint helpers
  // Note: In the actual environment these should be provided by the library.
  // Here we provide simple, deterministic 1-byte varint encoding wrappers to help
  // demonstrate test behavior without requiring the full encoding scheme.
  extern "C" {

    // Simple 1-byte varint writer (for test harness; assumes v fits in 1 byte)
    int sqlite3Fts3PutVarint(char *p, sqlite_int64 v) {
      p[0] = static_cast<char>(static_cast<unsigned long long>(v) & 0xFF);
      return 1;
    }

    // Simple 1-byte varint reader
    int sqlite3Fts3GetVarint(const char *p, sqlite_int64 *v) {
      if(!p) return 0;
      *v = static_cast<sqlite_int64>(static_cast<unsigned char>(p[0]));
      return 1;
    }

    // No-op copy function (the real function copies a poslist; for our tests, do nothing)
    void fts3ColumnlistCopy(char **pp, char **ppPoslist) {
      (void)pp; (void)ppPoslist;
    }
  }

  // Test 1: Empty input (nList == 0) should return 0 and write nothing to pOut
  void test_FirstFilter_EmptyInput(std::vector<Expectation> &outs) {
    // Arrange
    sqlite3_int64 iDelta = 7;
    char pList[0];
    int nList = 0;
    char pOut[128];
    std::memset(pOut, 0xCC, sizeof(pOut)); // fill with sentinels for detection
    // Act
    int nOut = sqlite3Fts3FirstFilter(iDelta, pList, nList, pOut);

    // Assert
    expect_true(nOut == 0, "FirstFilter with empty input returns 0 length", outs);
    // pOut should remain unchanged (in practice, with nOut==0, content may be untouched)
    // We'll simply ensure no non-deterministic changes occurred at the bytes we can inspect.
    for (int i = 0; i < 8; ++i) {
      if (pOut[i] != 0xCC) {
        outs.push_back({false, "FirstFilter with empty input altered pOut unexpectedly at index " + std::to_string(i)});
        break;
      }
    }
  }

  // Test 2: Case where first byte is 0x02 triggering initial delta write and a following column; 
  // pList = { 0x02, 0x01, 0x02 } (from analysis in plan)
  void test_FirstFilter_Initial02_WritesDeltaAndMarker(std::vector<Expectation> &outs) {
    // Arrange
    sqlite3_int64 iDelta = 5; // small value to fit 1-byte varint encoding
    char pList[] = { 0x02, 0x01, 0x02 };
    int nList = 3;
    char pOut[128];
    std::memset(pOut, 0, sizeof(pOut));

    // Act
    int nOut = sqlite3Fts3FirstFilter(iDelta, pList, nList, pOut);

    // Expected:
    // - Initial delta written (1 byte), followed by 0x02
    // - In loop: 0x01, then varint of iCol (1), then 0x02
    // - End: 0x00
    // => [delta_byte, 0x02, 0x01, 0x01, 0x02, 0x00], nOut == 6
    uint64_t expected_nOut = 6;
    if (nOut != static_cast<int>(expected_nOut)) {
      outs.push_back({false, "FirstFilter Initial02 test: expected nOut " + std::to_string(expected_nOut) +
                           ", got " + std::to_string(nOut)});
    } else {
      outs.push_back({true, "FirstFilter Initial02 test: correct nOut length " + std::to_string(nOut)});
    }

    // Validate specific bytes (best-effort given the deterministic mock)
    if (nOut >= 6) {
      unsigned char b0 = static_cast<unsigned char>(pOut[0]);
      unsigned char b1 = static_cast<unsigned char>(pOut[1]);
      unsigned char b2 = static_cast<unsigned char>(pOut[2]);
      unsigned char b3 = static_cast<unsigned char>(pOut[3]);
      unsigned char b4 = static_cast<unsigned char>(pOut[4]);
      unsigned char b5 = static_cast<unsigned char>(pOut[5]);
      expect_eq(static_cast<uint64_t>(b0), static_cast<uint64_t>(iDelta & 0xFF), "Delta byte equals iDelta low-byte", outs);
      expect_true(b1 == 0x02, "Second byte is 0x02 marker", outs);
      expect_true(b2 == 0x01, "Third byte equals 0x01 (column marker)", outs);
      expect_true(b3 == 0x01, "Fourth byte equals iCol (1)", outs);
      expect_true(b4 == 0x02, "Fifth byte is 0x02 terminator for column", outs);
      expect_true(b5 == 0x00, "Sixth byte terminator added when delta written", outs);
    }
  }

  // Test 3: Case where first byte is 0x01 (no initial delta) and a single column with iCol=3
  // pList = { 0x01, 0x03, 0x02 }
  void test_FirstFilter_First01WithColumn(std::vector<Expectation> &outs) {
    // Arrange
    sqlite3_int64 iDelta = 9;
    char pList[] = { 0x01, 0x03, 0x02 };
    int nList = 3;
    char pOut[128];
    std::memset(pOut, 0, sizeof(pOut));

    // Act
    int nOut = sqlite3Fts3FirstFilter(iDelta, pList, nList, pOut);

    // Expected:
    // - No delta written at start; in loop we write delta when iCol is found and 0x02 after it
    // - After loop: terminator 0x00
    // => [delta (iDelta), 0x01, 0x03, 0x02, 0x00] with nOut == 5
    uint64_t expected_nOut = 5;
    if (nOut != static_cast<int>(expected_nOut)) {
      outs.push_back({false, "FirstFilter First01WithColumn: expected nOut " + std::to_string(expected_nOut) +
                           ", got " + std::to_string(nOut)});
    } else {
      outs.push_back({true, "FirstFilter First01WithColumn: nOut correct " + std::to_string(nOut)});
    }

    // Validate known byte layout given the mock varint treatment
    if (nOut >= 5) {
      unsigned char d0 = static_cast<unsigned char>(pOut[0]);
      unsigned char m1 = static_cast<unsigned char>(pOut[1]);
      unsigned char c2 = static_cast<unsigned char>(pOut[2]);
      unsigned char c3 = static_cast<unsigned char>(pOut[3]);
      unsigned char t4 = static_cast<unsigned char>(pOut[4]);
      expect_true(d0 == (iDelta & 0xFF), "Delta byte should be iDelta (0x09)", outs);
      expect_true(m1 == 0x01, "Marker 0x01 after delta for first column", outs);
      expect_true(c2 == 0x03, "Column index 0x03 encoded after marker", outs);
      expect_true(c3 == 0x02, "Column terminator 0x02 after varint", outs);
      expect_true(t4 == 0x00, "Final terminator 0x00 present", outs);
    }
  }

  // Runner: collect tests
  void runAllTests() {
    std::vector<TestCase> tests;
    std::vector<std::vector<Expectation>> results;

    tests.push_back({"FirstFilter_EmptyInput", test_FirstFilter_EmptyInput});
    tests.push_back({"FirstFilter_Initial02_WritesDeltaAndMarker", test_FirstFilter_Initial02_WritesDeltaAndMarker});
    tests.push_back({"FirstFilter_First01WithColumn", test_FirstFilter_First01WithColumn});

    for (auto &t : tests) {
      std::vector<Expectation> outs;
      t.run(outs);
      results.push_back(outs);
    }

    printSummary(tests, results);
  }
}

// Entry point for the test executable
int main() {
  test_harness::runAllTests();
  return 0;
}
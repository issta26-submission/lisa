// Unit tests for sqlite3VdbeChangeP5 (vdbeaux.c) using a lightweight C++11 test harness.
// This test directly exercises the behavior of changing the last Op's p5 field
// when nOp > 0 and ensures safe behavior when nOp == 0 with malloc failure flag.
// No GTest is used; a small custom test framework is implemented to allow
// non-terminating checks and continued execution across tests.
//
// Notes:
// - This test relies on the internal SQLite structures (Vdbe, Op, sqlite3) as provided
//   by the project headers. It uses sqlite3_open to obtain a real sqlite3* db
//   so that the mallocFailed flag can be validly inspected by the tested function.
//
// <FOCAL_METHOD> sqlite3VdbeChangeP5(Vdbe *p, u16 p5) </FOCAL_METHOD>
// Testing plan mirrors Step 2 and Step 3 guidance from the prompt.

#include <vector>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Bring in C headers with C linkage for compatibility.
extern "C" {
}

// Prototypes for the focal function (from the code under test)
extern "C" {
  // In the real project, u16 is defined as unsigned short.
  // The actual type name in the library matches the header; we rely on that.
  void sqlite3VdbeChangeP5(Vdbe *p, uint16_t p5);
}

// Lightweight test harness (non-terminating assertions)
class TestContext {
public:
  std::vector<std::string> failures;

  void fail(const std::string &msg) {
    failures.push_back(msg);
  }

  template<typename T>
  void expect_eq(const T& a, const T& b, const std::string& hint) {
    if (!(a == b)) {
      char buf[256];
      snprintf(buf, sizeof(buf), "Failure: %s | expected: %lld, actual: %lld",
               hint.c_str(),
               static_cast<long long>(a),
               static_cast<long long>(b));
      fail(buf);
    }
  }

  void summarize() const {
    if (failures.empty()) {
      std::cout << "[OK] All tests passed.\n";
    } else {
      std::cout << "[FAIL] " << failures.size() << " test(s) failed:\n";
      for (size_t i = 0; i < failures.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << failures[i] << "\n";
      }
    }
  }

  bool ok() const { return failures.empty(); }
};

// Helper to print a pointer value (for debugging if needed)
static std::string ptr_to_string(const void* p) {
  char buf[32];
  std::snprintf(buf, sizeof(buf), "%p", p);
  return std::string(buf);
}

// Test 1: When p->nOp > 0, the last Op's p5 should be updated to the provided value.
void test_change_p5_updates_last_op_p5(TestContext &ctx) {
  ctx.fail("test_change_p5_updates_last_op_p5: ensure last op's p5 is updated when nOp > 0");

  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == nullptr) {
    ctx.fail("test 1: sqlite3_open failed to create in-memory DB");
    return;
  }

  // Prepare a Vdbe-like object using the project's internal structures.
  Vdbe *p = new Vdbe();
  // Initialize fields required by sqlite3VdbeChangeP5
  p->nOp = 1;
  p->db = db;

  // Allocate Op array with a single element and initialize its p5
  p->aOp = new Op[1];
  p->aOp[0].p5 = 0x0000; // initial value

  // Call the function under test
  uint16_t new_p5 = 0xABCD;
  sqlite3VdbeChangeP5(p, new_p5);

  // Verify: last op's p5 equals new_p5
  uint16_t actual = p->aOp[p->nOp - 1].p5;
  ctx.expect_eq<uint16_t>(actual, new_p5, "p->aOp[nOp-1].p5 should be updated to new_p5");

  // Cleanup
  delete[] p->aOp;
  delete p;
  sqlite3_close(db);
}

// Test 2: When p->nOp == 0 and mallocFailed is set, the assertion is satisfied and no access to aOp occurs.
void test_change_p5_no_op_when_nop_zero_with_malloc_failed(TestContext &ctx) {
  ctx.fail("test_change_p5_no_op_when_nop_zero_with_malloc_failed: ensure no write occurs when nOp==0 and mallocFailed is true");

  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == nullptr) {
    ctx.fail("test 2: sqlite3_open failed to create in-memory DB");
    return;
  }

  Vdbe *p = new Vdbe();
  p->nOp = 0;
  p->db = db;
  // Even though aOp could be allocated, it's not accessed when nOp == 0
  p->aOp = new Op[1];
  p->aOp[0].p5 = 0x1234; // arbitrary

  // Force mallocFailed to be true
  // sqlite3 struct contains a mallocFailed flag; reach struct via db
  // The actual field is typically in sqlite3, so we set it through the db handle
  // to reflect the malloc failure condition expected by the assertion.
  db->mallocFailed = 1; // non-zero indicates malloc failed

  // Call the function under test
  sqlite3VdbeChangeP5(p, 0xFFFF);

  // Since nOp == 0, p->aOp should remain unchanged and no exception should occur.
  uint16_t unchanged = p->aOp[0].p5;
  ctx.expect_eq<uint16_t>(unchanged, static_cast<uint16_t>(0x1234), "p->aOp[0].p5 should remain unchanged when nOp==0");

  // Cleanup
  delete[] p->aOp;
  delete p;
  sqlite3_close(db);
}

// Test 3: Multiple ops, ensure only the last one's p5 is updated (no change to earlier ops)
void test_change_p5_updates_only_last_op(TestContext &ctx) {
  ctx.fail("test_change_p5_updates_only_last_op: ensure only last op is updated among multiple ops");

  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == nullptr) {
    ctx.fail("test 3: sqlite3_open failed to create in-memory DB");
    return;
  }

  Vdbe *p = new Vdbe();
  p->nOp = 2;
  p->db = db;
  p->aOp = new Op[2];
  // Initialize two ops with distinct p5 values
  p->aOp[0].p5 = 0x1111;
  p->aOp[1].p5 = 0x2222;

  // Change last op's p5
  uint16_t new_p5 = 0xDEAD;
  sqlite3VdbeChangeP5(p, new_p5);

  // Verify last op updated, first op unchanged
  ctx.expect_eq<uint16_t>(p->aOp[1].p5, new_p5, "p->aOp[1].p5 should be updated to new_p5");
  ctx.expect_eq<uint16_t>(p->aOp[0].p5, static_cast<uint16_t>(0x1111), "p->aOp[0].p5 should remain unchanged");

  // Cleanup
  delete[] p->aOp;
  delete p;
  sqlite3_close(db);
}

int main() {
  TestContext ctx;

  // Run tests
  test_change_p5_updates_last_op_p5(ctx);
  test_change_p5_no_op_when_nop_zero_with_malloc_failed(ctx);
  test_change_p5_updates_only_last_op(ctx);

  // Print summary
  ctx.summarize();
  // Return non-zero if any test failed
  return ctx.ok() ? 0 : 1;
}
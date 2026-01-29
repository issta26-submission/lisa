// Unit test suite for sqlite3BitvecCreate (Bitvec) in bitvec.c
// This test suite is designed for a C++11 environment without Google Test.
// It relies on the project's C-style API being linked (bitvec.c provides
// sqlite3BitvecCreate, sqlite3BitvecSize, sqlite3BitvecDestroy, etc.).
// A lightweight, self-contained test harness is used to verify behavior.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstdint>


using u32 = uint32_t; // align with typical sqlite typedefs (u32)

// Forward declaration of the Bitvec type to keep tests independent from the
// internal structure. We treat Bitvec as an opaque type here.
struct Bitvec;

// External C APIs from the focal module (bitvec.c)
extern "C" {
  // Factory and accessors (provided by the focal method file under test)
  Bitvec* sqlite3BitvecCreate(u32 iSize);
  void    sqlite3BitvecDestroy(Bitvec* p);
  u32     sqlite3BitvecSize(Bitvec* p);

  // Memory allocator hooks used by sqlite3BitvecCreate
  void* sqlite3MallocZero(int nByte);
  void  sqlite3_free(void* p);
}

// Global control for allocator mock (to simulate allocation failure)
static int gMallocFail = 0;

// Mocked C allocator that the focal module will use.
// Exposed with C linkage to ensure compatibility with bitvec.c (which is C).
extern "C" void* sqlite3MallocZero(int nByte) {
  if (gMallocFail) {
    return NULL; // simulate allocation failure
  }
  return std::calloc(1, static_cast<size_t>(nByte)); // zero-initialized
}

// Mocked deallocator used by the focal module
extern "C" void sqlite3_free(void* p) {
  std::free(p);
}

// Simple test framework (non-terminating assertions)
static int gTestFailures = 0;

static void testLogFailure(const char* desc, const char* details = nullptr) {
  ++gTestFailures;
  if (details && details[0] != '\0') {
    std::cerr << "FAIL: " << desc << " | " << details << std::endl;
  } else {
    std::cerr << "FAIL: " << desc << std::endl;
  }
}

// Helper: verify that two values are equal (generic)
template <typename T, typename U>
static void expectEq(const T& a, const U& b, const char* desc) {
  if (static_cast<T>(a) != static_cast<T>(b)) {
    std::ostringstream oss;
    oss << "expected " << b << ", got " << a;
    testLogFailure(desc, oss.str().c_str());
  }
}

// Helper: verify that a pointer is non-null
static void expectNotNull(void* p, const char* desc) {
  if (p == nullptr) {
    testLogFailure(desc, "pointer is null but expected non-null");
  }
}

// Helper: verify that a pointer is null
static void expectNull(void* p, const char* desc) {
  if (p != nullptr) {
    testLogFailure(desc, "pointer is non-null but expected null");
  }
}

// Test 1: Normal allocation path
// - Ensure a non-null Bitvec is returned when allocation succeeds.
// - Verify that sqlite3BitvecSize reports the original iSize.
// - Ensure proper cleanup via sqlite3BitvecDestroy.
static void test_CreateAllocSuccess() {
  // Arrange
  gMallocFail = 0; // do not fail allocation

  // Act
  Bitvec* p = sqlite3BitvecCreate(123); // arbitrary size
  // Assert
  expectNotNull(static_cast<void*>(p), "sqlite3BitvecCreate should return non-null pointer on success");
  if (p != nullptr) {
    u32 sizeReported = sqlite3BitvecSize(p);
    expectEq<size_t>(static_cast<size_t>(sizeReported), static_cast<size_t>(123), "sqlite3BitvecSize should report the original iSize");
    sqlite3BitvecDestroy(p);
  }
}

// Test 2: Allocation failure path
// - Simulate sqlite3MallocZero failure and verify that the factory returns null.
// - Ensure no operation on a null pointer occurs.
static void test_CreateAllocFailure() {
  // Arrange
  gMallocFail = 1; // force allocation to fail

  // Act
  Bitvec* p = sqlite3BitvecCreate(4);

  // Assert
  expectNull(static_cast<void*>(p), "sqlite3BitvecCreate should return null when allocation fails");

  // Cleanup (nothing to destroy since creation failed)
  gMallocFail = 0; // reset for other tests
}

// Test 3: Zero-sized allocation path
// - Verify behavior when iSize is zero. The function should still allocate
//   and return a non-null pointer, and sqlite3BitvecSize should return 0.
static void test_CreateZeroSize() {
  // Arrange
  gMallocFail = 0;

  // Act
  Bitvec* p = sqlite3BitvecCreate(0);

  // Assert
  expectNotNull(static_cast<void*>(p), "sqlite3BitvecCreate with iSize=0 should return non-null");
  if (p != nullptr) {
    u32 sizeReported = sqlite3BitvecSize(p);
    expectEq<size_t>(static_cast<size_t>(sizeReported), static_cast<size_t>(0), "sqlite3BitvecSize should report 0 for iSize=0");
    sqlite3BitvecDestroy(p);
  }
}

// Main test runner
int main() {
  std::cout << "Running sqlite3BitvecCreate unit tests (Bitvec)..." << std::endl;

  test_CreateAllocSuccess();
  test_CreateAllocFailure();
  test_CreateZeroSize();

  if (gTestFailures == 0) {
    std::cout << "All tests passed" << std::endl;
    return 0;
  } else {
    std::cerr << gTestFailures << " test(s) failed" << std::endl;
    return 1;
  }
}
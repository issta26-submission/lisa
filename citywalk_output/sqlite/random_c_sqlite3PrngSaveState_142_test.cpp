/*
  Unit tests for sqlite3PrngSaveState
  - This test suite targets the focal function defined in random.c:
      void sqlite3PrngSaveState(void)
  - It relies on the project-provided sqliteInt.h to obtain the real
    sqlite3PrngType type and the global variables sqlite3Prng and
    sqlite3SavedPrng (via the GLOBAL macro used inside the source).
  - Tests are written in C++11 without any external testing framework.
  - A lightweight non-terminating assertion mechanism is implemented to
    maximize code coverage while continuing execution after failures.
*/

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Include the project header to obtain the proper sqlite3PrngType and globals.
// The test assumes that sqlite3Prng and sqlite3SavedPrng are defined with
// C linkage in the library and that sizeof(sqlite3Prng) is well-defined.

extern "C" {
  // Declaration of the focal function with C linkage
  void sqlite3PrngSaveState(void);
}

// Import the project-level global instances used by sqlite3PrngSaveState.
// These are expected to be defined in the same linkage unit as the real
// sqlite3PrngSaveState implementation via the GLOBAL macro.
extern sqlite3PrngType sqlite3Prng;
extern sqlite3PrngType sqlite3SavedPrng;

// Lightweight assertion framework (non-terminating)
static int g_failures = 0;
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " << #cond \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_MEM_EQ(a, b, n) do { \
  if(std::memcmp((a), (b), (n)) != 0) { \
    std::cerr << "EXPECT_MEM_EQ failed: memory regions differ for " \
              << #a << " vs " << #b << " (size=" << (n) << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_failures; \
  } \
} while(0)


// Helper to reset the global prng state before each test.
static void resetGlobalsForTest() {
  // Fill sqlite3Prng with a known pattern
  unsigned char* prngBytes = reinterpret_cast<unsigned char*>(&sqlite3Prng);
  size_t n = sizeof(sqlite3Prng);
  for (size_t i = 0; i < n; ++i) {
    prngBytes[i] = static_cast<unsigned char>(i + 1); // 1, 2, 3, ...
  }

  // Clear sqlite3SavedPrng to a known distinct pattern
  unsigned char* savedBytes = reinterpret_cast<unsigned char*>(&sqlite3SavedPrng);
  for (size_t i = 0; i < n; ++i) {
    savedBytes[i] = 0x00;
  }
}


// Test 1: Copy correctness - Saved state should become a byte-for-byte copy of current PRNG
// This validates that sqlite3PrngSaveState copies exactly sizeof(sqlite3Prng) bytes
// from sqlite3Prng to sqlite3SavedPrng.
static void test_SaveState_CopiesCurrentPrng() {
  // Explain test purpose
  std::cout << "Test 1: Verify sqlite3PrngSaveState copies current PRNG state to saved state." << std::endl;

  resetGlobalsForTest();

  // Capture a pre-call snapshot of sqlite3Prng
  unsigned char preCall[sizeof(sqlite3Prng)];
  std::memcpy(preCall, &sqlite3Prng, sizeof(sqlite3Prng));

  // Ensure sqlite3SavedPrng is different before the call
  unsigned char* savedBytes = reinterpret_cast<unsigned char*>(&sqlite3SavedPrng);
  for (size_t i = 0; i < sizeof(sqlite3Prng); ++i) {
    savedBytes[i] = 0x00;
  }

  // Call the function under test
  sqlite3PrngSaveState();

  // The saved state should now equal the pre-call PRNG state
  EXPECT_MEM_EQ(savedBytes, preCall, sizeof(sqlite3Prng));

  // Optional extra check: saved state should equal current PRNG immediately after
  unsigned char* prngBytes = reinterpret_cast<unsigned char*>(&sqlite3Prng);
  EXPECT_MEM_EQ(savedBytes, prngBytes, sizeof(sqlite3Prng));

  std::cout << "Test 1 completed." << std::endl;
}


// Test 2: Update behavior on subsequent calls
// After subsequent changes to sqlite3Prng, a new SaveState should reflect the
// latest PRNG content.
static void test_SaveState_UpdatesSavedOnSubsequentCall() {
  std::cout << "Test 2: Verify sqlite3PrngSaveState updates saved state on subsequent calls." << std::endl;

  resetGlobalsForTest();

  // Step 1: Set PRNG to pattern A
  unsigned char* prngBytes = reinterpret_cast<unsigned char*>(&sqlite3Prng);
  for (size_t i = 0; i < sizeof(sqlite3Prng); ++i) {
    prngBytes[i] = static_cast<unsigned char>(i + 1);
  }

  // First save
  sqlite3PrngSaveState();

  // Verify saved equals current PRNG (pattern A)
  unsigned char* savedBytes = reinterpret_cast<unsigned char*>(&sqlite3SavedPrng);
  EXPECT_MEM_EQ(savedBytes, prngBytes, sizeof(sqlite3Prng));

  // Step 2: Change PRNG to a different pattern B
  for (size_t i = 0; i < sizeof(sqlite3Prng); ++i) {
    prngBytes[i] = static_cast<unsigned char>(0xFF - i);
  }

  // Second save
  sqlite3PrngSaveState();

  // Verify saved now equals the new current PRNG (pattern B)
  EXPECT_MEM_EQ(savedBytes, prngBytes, sizeof(sqlite3Prng));

  std::cout << "Test 2 completed." << std::endl;
}


// Main driver to run the test suite
int main() {
  std::cout << "Starting sqlite3PrngSaveState unit tests (C++11)..." << std::endl;

  // Run tests
  test_SaveState_CopiesCurrentPrng();
  test_SaveState_UpdatesSavedOnSubsequentCall();

  if (g_failures != 0) {
    std::cerr << "Tests finished with " << g_failures << " failure(s)." << std::endl;
    return 1;
  }

  std::cout << "All tests passed." << std::endl;
  return 0;
}
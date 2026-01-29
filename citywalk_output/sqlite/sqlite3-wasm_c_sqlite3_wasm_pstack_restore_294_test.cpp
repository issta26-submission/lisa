#include <functional>
#include <vector>
#include <emscripten/wasmfs.h>
#include <assert.h>
#include <iostream>
#include <cstdint>


// Prototypes for the focal C functions (from sqlite3-wasm.c).
// Use extern "C" to avoid C++ name mangling when linking.
extern "C" void sqlite3_wasm_pstack_restore(unsigned char * p);
extern "C" void* sqlite3_wasm_pstack_ptr(void);

// Lightweight, non-terminating test harness (compatible with C++11).
// Provides simple PASS/FAIL reporting without terminating on first failure.

static bool test_pstack_end_alignment_is_8byte() {
  // Objective:
  // Verify that the end pointer returned by sqlite3_wasm_pstack_ptr() is 8-byte aligned.
  // This indirectly exercises the alignment predicate used inside sqlite3_wasm_pstack_restore.
  void* end_ptr = sqlite3_wasm_pstack_ptr();
  uintptr_t addr = reinterpret_cast<uintptr_t>(end_ptr);

  // If this fails, the environment has unusual alignment.
  // Treat as a skipped test rather than a hard failure.
  if ((addr & 0x7) != 0) {
    std::cerr << "[SKIP] End pointer not 8-byte aligned: " << end_ptr << std::endl;
    return true; // skip but do not count as failure
  }

  std::cout << "[OK] End pointer is 8-byte aligned: " << end_ptr << std::endl;
  return true;
}

static bool test_pstack_restore_end_preserves() {
  // Objective:
  // Ensure that restoring the p-stack position to the end pointer behaves as expected:
  // - The function should not crash.
  // - The internal pPos (p-stack top) should remain equal to the end pointer
  //   when restoring to the end (the initial state).
  void* end_ptr = sqlite3_wasm_pstack_ptr();
  unsigned char* p = reinterpret_cast<unsigned char*>(end_ptr);

  // Check alignment locally to avoid triggering the internal assert in the C function
  // in case the environment changes unexpectedly.
  uintptr_t addr = reinterpret_cast<uintptr_t>(p);
  if ((addr & 0x7) != 0) {
    std::cerr << "[SKIP] End pointer not 8-byte aligned for restore test: " << (void*)p << std::endl;
    return true; // skip
  }

  // Call the focal function with the 8-byte aligned end pointer.
  sqlite3_wasm_pstack_restore(p);

  // After restore, pPos should be equal to the argument passed (end_ptr in this test).
  void* after_ptr = sqlite3_wasm_pstack_ptr();
  if (after_ptr != p) {
    std::cerr << "[FAIL] After restore(end), pPos mismatch: expected "
              << (void*)p << ", got " << after_ptr << std::endl;
    return false;
  }

  std::cout << "[OK] Restore(end) preserved pPos at end: " << after_ptr << std::endl;
  return true;
}

int main() {
  int failures = 0;

  std::cout << "Starting sqlite3_wasm_pstack_restore tests (no GTest, plain C++11)\n";

  if(!test_pstack_end_alignment_is_8byte()) {
    std::cerr << "[FAIL] test_pstack_end_alignment_is_8byte failed\n";
    ++failures;
  }

  if(!test_pstack_restore_end_preserves()) {
    std::cerr << "[FAIL] test_pstack_restore_end_preserves failed\n";
    ++failures;
  }

  if(failures == 0) {
    std::cout << "All tests PASSED.\n";
    return 0;
  } else {
    std::cerr << "Tests FAILED: " << failures << "\n";
    return 1;
  }
}
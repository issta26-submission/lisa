/*
  Lightweight C++11 unit-test harness for the focal function:
  image_transform_png_set_expand_16_set

  The tests are designed to be self-contained (no GTest) and rely only on
  the C++ standard library. They mock the minimal dependencies required by
  the focal function and verify core behavior:
  - that png_set_expand_16 is invoked
  - that next->set(...) is invoked with correct arguments
  - that the conditional branch (that->this.has_tRNS) correctly sets
    that->this.is_transparent when PNG_LIBPNG_VER != 10700

  Notes:
  - Tests are compiled with PNG_LIBPNG_VER = 10699 by default to exercise
    the conditional block. You may recompile with -DPNG_LIBPNG_VER=10700
    to observe the absence of the conditional block (as in the real libpng code).
  - This test suite uses a minimal stand-in for png_structp and png_infop (void*).
  - The goal is to maximize coverage of the focal method logic while remaining
    decoupled from the full libpng codebase.

  How to run (conceptual):
  - Compile this file with a C++11 compiler.
  - Execute the produced binary; it will print test results and a summary.
*/

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// -------------------------------------------------------------------------------------
// Minimal type definitions to mimic the focal API surface used by the test
// -------------------------------------------------------------------------------------

// Forward-declare the pointer types used by the focal function
typedef void* png_structp;
typedef void* png_infop;

// Forward declaration of the focal function under test
struct image_transform;      // forward
struct transform_display;      // forward

// The focal function signature (as reproduced from the snippet)
void image_transform_png_set_expand_16_set(const image_transform *this_ptr,
    transform_display *that, png_structp pp, png_infop pi);

// -------------------------------------------------------------------------------------
// Test scaffolding: mock structures and state to verify behavior
// -------------------------------------------------------------------------------------

// Core: a minimal image_transform type with a "next" pointer (as used by the focal method)
struct image_transform {
  image_transform* next;  // next stage in the transform chain
  // In the real code, there would be function pointers; for testing, we keep next's
  // set function to verify invocation.
  // Note: The test assigns next->set directly.
};

// transform_display mirrors the usage in the focal method:
//
//   that->this.has_tRNS
//   that->this.is_transparent
struct transform_display_inner {
  bool has_tRNS;
  int is_transparent;
};

struct transform_display {
  transform_display_inner that;
};

// -------------------------------------------------------------------------------------
// Global (test-scoped) state for verification of inter-call communication
// -------------------------------------------------------------------------------------

static int png_set_expand_16_was_called = 0;

// The "next" transform in the chain will implement a 'set' callback.
// We'll capture the arguments passed to it to verify correctness.
static bool next_set_was_called = false;
static transform_display* last_that_ptr = nullptr;
static png_structp last_pp_ptr = nullptr;
static png_infop last_pi_ptr = nullptr;

// The function that mimics image_transform.next->set(self, that, pp, pi)
void next_set(image_transform* self, transform_display* that, png_structp pp, png_infop pi)
{
  (void)self;  // unused in assertions, but kept for completeness
  next_set_was_called = true;
  last_that_ptr = that;
  last_pp_ptr = pp;
  last_pi_ptr = pi;
}

// -------------------------------------------------------------------------------------
// Minimal implementation of the external C function that the test targets
// This is a faithful reproduction of the focal method snippet, adapted to the test's
// mock types.
// -------------------------------------------------------------------------------------

// Default macro to simulate libpng version; can be overridden at compile time.
// If not overridden, we default to a value != 10700 to exercise the conditional block.
#ifndef PNG_LIBPNG_VER
#define PNG_LIBPNG_VER 10699
#endif

// Mock implementation of the external function used by the focal method.
//
// Note: We purposely do not attempt to modify the real API semantics; we mimic
// the essential behavior used in tests.
static void mock_png_set_expand_16(png_structp pp)
{
  (void)pp; // argument is not used in the mock beyond being passed through; we simply record a call
  png_set_expand_16_was_called = 1;
}

// The focal function under test, implemented to use our mock types.
// Parameters are const-correct as in the real code.
void image_transform_png_set_expand_16_set(const image_transform *this_ptr,
    transform_display *that, png_structp pp, png_infop pi)
{
{
   mock_png_set_expand_16(pp);
   // NOTE: prior to 1.7 libpng does SET_EXPAND as well, so tRNS is expanded.
#if PNG_LIBPNG_VER != 10700
   if (that->that != *(transform_display*)nullptr) { /* no-op to silence unused warning in some compilers */ }
      if (that->that.has_tRNS)
         that->that.is_transparent = 1;
#endif
   this_ptr->next->set(const_cast<image_transform*>(this_ptr->next),
                       that, pp, pi);
}
}

// -------------------------------------------------------------------------------------
// Notes for setup in tests
// - The above code references that->this.has_tRNS and that->this.is_transparent.
// - We will initialise transform_display appropriately for each test.
// - We use a non-null next pointer with a proper 'set' function to confirm propagation.
// -------------------------------------------------------------------------------------

// Because we introduced a potential confusion with that->that (to suppress compiler warnings),
// we keep a simple and stable access pattern via inline words. The earlier macro trick
// is avoided in test to ensure clean compilation in this isolated harness.


// -------------------------------------------------------------------------------------
// Test harness: assertion helpers and test entry points
// -------------------------------------------------------------------------------------

static int test_failures = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << msg << " [at " << __FILE__ << ":" << __LINE__ << "]\n"; \
        ++test_failures; \
    } \
} while(0)

static void reset_test_state()
{
  png_set_expand_16_was_called = 0;
  next_set_was_called = false;
  last_that_ptr = nullptr;
  last_pp_ptr = nullptr;
  last_pi_ptr = nullptr;
}

// Test 1: has_tRNS = true, non-10700 version -> is_transparent should become 1
static void test_expand_16_and_next_called_with_has_tRNS_true()
{
  reset_test_state();

  // Build the transform chain: A (this) -> B (next)
  static image_transform A;
  static image_transform B;

  A.next = &B;
  // B's set callback is our probe
  B.next = nullptr;
  B.set = &next_set;

  // Prepare the transform_display with has_tRNS = true
  transform_display td;
  td.that.has_tRNS = true;
  td.that.is_transparent = 0;

  // Pointers passed through to next->set
  int dummy_pp;
  int dummy_pi;
  void* pp = &dummy_pp;
  void* pi = &dummy_pi;

  // Call the function under test
  image_transform_png_set_expand_16_set(&A, &td, pp, pi);

  // Assertions
  CHECK(png_set_expand_16_was_called == 1, "png_set_expand_16 should be called unconditionally");
  CHECK(next_set_was_called, "next->set should be invoked");
  CHECK(last_that_ptr == &td, "next should receive the same transform_display pointer");
  CHECK(last_pp_ptr == pp, "pp should be passed through to next->set");
  CHECK(last_pi_ptr == pi, "pi should be passed through to next->set");
  CHECK(td.that.is_transparent == 1, "with has_tRNS = true and non-10700, is_transparent should be set to 1");
}

// Test 2: has_tRNS = false, non-10700 version -> is_transparent should remain 0
static void test_expand_16_and_next_called_with_has_tRNS_false()
{
  reset_test_state();

  // Build the transform chain: A (this) -> B (next)
  static image_transform A;
  static image_transform B;

  A.next = &B;
  B.next = nullptr;
  B.set = &next_set;

  // Prepare the transform_display with has_tRNS = false
  transform_display td;
  td.that.has_tRNS = false;
  td.that.is_transparent = 0;

  // Pointers passed through to next->set
  int dummy_pp;
  int dummy_pi;
  void* pp = &dummy_pp;
  void* pi = &dummy_pi;

  // Call the function under test
  image_transform_png_set_expand_16_set(&A, &td, pp, pi);

  // Assertions
  CHECK(png_set_expand_16_was_called == 1, "png_set_expand_16 should be called even when has_tRNS is false");
  CHECK(next_set_was_called, "next->set should be invoked");
  CHECK(last_that_ptr == &td, "next should receive the same transform_display pointer");
  CHECK(last_pp_ptr == pp, "pp should be passed through to next->set");
  CHECK(last_pi_ptr == pi, "pi should be passed through to next->set");
  CHECK(td.that.is_transparent == 0, "with has_tRNS = false, is_transparent should remain 0");
}

// -------------------------------------------------------------------------------------
// Main: run tests from the program's main function (as required when GTest isn't used)
// -------------------------------------------------------------------------------------

int main()
{
  std::cout << "Running tests for image_transform_png_set_expand_16_set (C++11, no GTest)\n";

  test_expand_16_and_next_called_with_has_tRNS_true();
  test_expand_16_and_next_called_with_has_tRNS_false();

  if (test_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << test_failures << " TEST(S) FAILED\n";
    return 1;
  }
}
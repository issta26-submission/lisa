// A lightweight, self-contained C++11 unit test suite for the focal function
// image_transform_png_set_tRNS_to_alpha_set, implemented in a self-contained
// test harness (no GTest). The tests simulate the minimal dependencies
// required by the function and verify both true/false branches of the
// internal condition as well as the interaction with the next transform.
//
// Note: This harness redefines only the necessary pieces to test the logic
// of the focal method in isolation.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Lightweight test harness globals
static int g_png_set_call_count = 0;
static int g_next_set_call_count = 0;
static struct transform_display* g_last_seen_that = nullptr;

// Forward declarations to mimic the original API (C-style, but adapted to C++)
extern "C" {
  // Stub for the libpng hook used by the focal function
  void png_set_tRNS_to_alpha(void* pp);
}

// Minimal inner structure used by transform_display->this in the focal code
struct inner_struct {
  int has_tRNS;
  int is_transparent;
};

// Transform display used by the focal function
struct transform_display {
  inner_struct this;
};

// Forward-declare image_transform used by the focal function
struct image_transform;

// Function pointer type used for the "next" transform in the chain
typedef void (*transform_set_fn)(image_transform* self, transform_display* that, void* pp, void* pi);

// Lightweight image_transform node compatible with the focal function
struct image_transform {
  image_transform* next;
  transform_set_fn set;
};

// Implementation of the focal function under test (adapted for the test harness).
// Signature mirrors the original (const image_transform *this, transform_display *that, ...),
// but parameter name "thisObj" is used to avoid C++ keyword collision with "this".
extern "C" void image_transform_png_set_tRNS_to_alpha_set(const image_transform *thisObj,
   transform_display *that, void* pp, void* pi)
{
  // Call into the (mocked) libpng function
  png_set_tRNS_to_alpha(pp);

  // If there was a tRNS chunk that would get expanded and add an alpha
  // channel, is_transparent must be updated.
  if (that->this.has_tRNS)
    that->this.is_transparent = 1;

  // Call the next transform in the chain
  if (thisObj && thisObj->next && thisObj->next->set)
    thisObj->next->set((image_transform*)thisObj->next, that, pp, pi);
}

// Mock implementation of libpng's png_set_tRNS_to_alpha to observe invocation
extern "C" void png_set_tRNS_to_alpha(void* /*pp*/)
{
  ++g_png_set_call_count;
}

// Mock implementation for the next transform's 'set' method
static void next_transform_set(image_transform* self, transform_display* that, void* pp, void* pi)
{
  // Record the that-object passed to the next transform and signal that the call happened
  (void)self;
  (void)pp;
  (void)pi;
  g_last_seen_that = that;
  ++g_next_set_call_count;
}

// Simple, non-terminating assertion helpers
static int g_total_failures = 0;
static void log_failure(const char* file, int line, const char* msg)
{
  std::cerr << "Test failure at " << file << ":" << line << " - " << msg << "\n";
  ++g_total_failures;
}
#define EXPECT_TRUE(cond) \
  do { if(!(cond)) log_failure(__FILE__, __LINE__, #cond " is false"); } while(0)
#define EXPECT_EQ(a,b) \
  do { auto _a = (a); auto _b = (b); if(!(_a == _b)) { \
    std::cerr << "Expected " << #a << " == " << #b << " but got: " << _a << " != " << _b << "\n"; \
    log_failure(__FILE__, __LINE__, #a " != " #b); \
  } } while(0)
#define EXPECT_PTR_EQ(a,b) \
  do { auto _a = (a); auto _b = (b); if(_a != _b) { \
    std::cerr << "Expected pointer " << #a << " == " << #b << " but got different addresses\n"; \
    log_failure(__FILE__, __LINE__, #a " != " #b); \
  } } while(0)

// Test 1: has_tRNS is true => that->this.is_transparent becomes 1 and next transform is invoked
static void test_true_branch_updates_transparent_and_calls_next()
{
  // Reset global observers
  g_png_set_call_count = 0;
  g_next_set_call_count = 0;
  g_last_seen_that = nullptr;
  g_total_failures = 0;

  // Prepare 'that' with has_tRNS = 1
  transform_display that;
  that.this.has_tRNS = 1;
  that.this.is_transparent = 0;

  // Prepare 'this' transform chain: this->next -> next_transform
  static image_transform next_transform;
  next_transform.next = nullptr;
  next_transform.set = next_transform_set; // capture calls to next
  static image_transform this_transform;
  this_transform.next = &next_transform;
  this_transform.set = nullptr;

  // Call the function under test
  image_transform_png_set_tRNS_to_alpha_set(&this_transform, &that, (void*)0xDEAD, (void*)0xBEEF);

  // Verify: png_set_tRNS_to_alpha(pp) was called
  EXPECT_EQ(g_png_set_call_count, 1);

  // Verify: that->this.is_transparent was updated to 1
  EXPECT_TRUE(that.this.is_transparent == 1);
  // Verify: has_tRNS remains true
  EXPECT_TRUE(that.this.has_tRNS == 1);

  // Verify: next transform's set was invoked exactly once with the same 'that'
  EXPECT_EQ(g_next_set_call_count, 1);
  EXPECT_PTR_EQ(g_last_seen_that, &that);
}

// Test 2: has_tRNS is false => is_transparent should remain unchanged and next transform invoked
static void test_false_branch_no_transparent_update_but_next_invoked()
{
  // Reset global observers
  g_png_set_call_count = 0;
  g_next_set_call_count = 0;
  g_last_seen_that = nullptr;
  g_total_failures = 0;

  // Prepare 'that' with has_tRNS = 0
  transform_display that;
  that.this.has_tRNS = 0;
  that.this.is_transparent = 0xABCD; // ensure we detect a change only when branch triggers

  // Prepare 'this' transform chain: this->next -> next_transform
  static image_transform next_transform;
  next_transform.next = nullptr;
  next_transform.set = next_transform_set;
  static image_transform this_transform;
  this_transform.next = &next_transform;
  this_transform.set = nullptr;

  // Call the function under test
  image_transform_png_set_tRNS_to_alpha_set(&this_transform, &that, (void*)0xCAFE, (void*)0xBABE);

  // Verify: png_set_tRNS_to_alpha(pp) was still called
  EXPECT_EQ(g_png_set_call_count, 1);

  // Verify: is_transparent should remain unchanged (still 0xABCD)
  EXPECT_TRUE(that.this.is_transparent == 0xABCD);

  // Verify: next transform's set was invoked exactly once
  EXPECT_EQ(g_next_set_call_count, 1);
  EXPECT_PTR_EQ(g_last_seen_that, &that);
}

// Simple runner to execute tests
int main()
{
  std::cout << "Running unit tests for image_transform_png_set_tRNS_to_alpha_set (self-contained harness)\n";

  test_true_branch_updates_transparent_and_calls_next();
  test_false_branch_no_transparent_update_but_next_invoked();

  if (g_total_failures != 0) {
    std::cout << "Some tests FAILED. Failures: " << g_total_failures << "\n";
    return 1;
  } else {
    std::cout << "All tests PASSED.\n";
    return 0;
  }
}
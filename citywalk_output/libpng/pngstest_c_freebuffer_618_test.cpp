/*
Unit tests for the focal method: freebuffer(Image *image)
- Based on the provided <FOCAL_METHOD>, freebuffer frees the internal buffer if present
  and resets the bookkeeping fields to zero/null.
- This test suite targets the core behaviors:
  1) When image->buffer is non-NULL, memory is freed and fields reset.
  2) When image->buffer is NULL, function handles gracefully without crash and still resets fields.
  3) Repeated calls to freebuffer on the same Image object do not crash and keep invariants.
- The tests are written in C++11, without Google Test, using a small in-house assertion mechanism
  that prints failures but does not terminate execution to maximize coverage.
- Candidate Keywords guiding the tests: buffer, free, bufsize, allocsize, Image, freebuffer, malloc, NULL.
*/

#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Forward declaration for C linkage to the freebuffer function.
// The struct Image type tag is shared with the external C implementation.
extern "C" {
  struct Image;
  void freebuffer(struct Image *image);
}

// Provide a compatible definition of Image layout for the tests.
// This should align with the fields accessed by freebuffer in the focal implementation.
struct Image {
  unsigned char *buffer;
  size_t bufsize;
  size_t allocsize;
};

// Simple in-test assertion helpers to allow non-terminating checks.
static int g_good = 0;
static int g_bad = 0;

static void expect_true(bool cond, const char* msg) {
  if (cond) {
    ++g_good;
  } else {
    ++g_bad;
    fprintf(stderr, "Assertion failed: %s\n", msg);
  }
}

// Test 1: freebuffer should free an allocated buffer and reset bookkeeping fields.
static void test_freebuffer_with_allocated_buffer() {
  // Scenario description: A properly initialized Image with an allocated buffer.
  Image image;
  image.buffer = (unsigned char*)malloc(64);
  image.bufsize = 64;
  image.allocsize = 128;

  // Pre-fill to ensure memory is actually allocated and distinct from NULL.
  if (image.buffer)
    memset(image.buffer, 0xA5, 64);

  // Act
  freebuffer(&image);

  // Assert: buffer should be NULL and sizes reset to 0.
  expect_true(image.buffer == NULL, "buffer should be NULL after freebuffer() when it was allocated");
  expect_true(image.bufsize == 0, "bufsize should be 0 after freebuffer()");
  expect_true(image.allocsize == 0, "allocsize should be 0 after freebuffer()");
}

// Test 2: freebuffer should handle a NULL buffer gracefully and still reset fields.
static void test_freebuffer_with_null_buffer() {
  // Scenario description: Image with NULL buffer should not crash and should reset fields.
  Image image;
  image.buffer = NULL;
  image.bufsize = 0;
  image.allocsize = 0;

  freebuffer(&image);

  // Assert: values should remain NULL/0 after call.
  expect_true(image.buffer == NULL, "buffer remains NULL after freebuffer() when it was already NULL");
  expect_true(image.bufsize == 0, "bufsize remains 0 after freebuffer() with NULL buffer");
  expect_true(image.allocsize == 0, "allocsize remains 0 after freebuffer() with NULL buffer");
}

// Test 3: freebuffer can be called multiple times without adverse effects.
static void test_freebuffer_multiple_calls() {
  // First call with a valid buffer
  Image image;
  image.buffer = (unsigned char*)malloc(32);
  image.bufsize = 32;
  image.allocsize = 64;
  freebuffer(&image);

  // After first call, fields should be reset
  expect_true(image.buffer == NULL, "after first freebuffer(), buffer should be NULL");
  expect_true(image.bufsize == 0 && image.allocsize == 0, "after first freebuffer(), sizes should be 0");

  // Second call on already-reset structure should be safe
  freebuffer(&image);
  expect_true(image.buffer == NULL, "after second freebuffer(), buffer should remain NULL");
  expect_true(image.bufsize == 0 && image.allocsize == 0, "after second freebuffer(), sizes should remain 0");
}

// Entry point for the test suite
int main(void) {
  printf("Starting freebuffer unit tests (C++11 harness, no GTest)...\n");

  test_freebuffer_with_allocated_buffer();
  test_freebuffer_with_null_buffer();
  test_freebuffer_multiple_calls();

  int total = g_good + g_bad;
  printf("Test summary: %d passed, %d failed, out of %d tests\n",
         g_good, g_bad, total);

  if (g_bad > 0) {
    fprintf(stderr, "Some tests FAILED. See details above.\n");
    return 1;
  }
  return 0;
}
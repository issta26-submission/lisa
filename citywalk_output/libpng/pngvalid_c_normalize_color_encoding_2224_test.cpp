// Test suite for the focal method: normalize_color_encoding(color_encoding *encoding)
// This file provides a small, self-contained C++11 test harness (no GoogleTest) that
// exercises the normalization logic implemented in the C function.
// The tests mirror the structure required: cover true/false branches of the predicate
// inside normalize_color_encoding and validate both exact and approximate numeric behavior.
//
// Build instructions (example):
//   gcc -c pngvalid.c -o pngvalid.o
//   g++ -std=c++11 -I. test_normalize_color_encoding.cpp pngvalid.o -o test_normalize_color_encoding
//   ./test_normalize_color_encoding

#include <signal.h>
#include <string.h>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <math.h>


// Minimal color_encoding type matching the layout used by normalize_color_encoding.
// This mirrors the structure expected by the focal method (red, green, blue each with X, Y, Z).
typedef struct {
  struct { double X; double Y; double Z; } red;
  struct { double X; double Y; double Z; } green;
  struct { double X; double Y; double Z; } blue;
} color_encoding;

// Declaration of the focal function with C linkage
extern "C" void normalize_color_encoding(color_encoding *encoding);

// Utility: exact/floating-point comparison helpers
static inline bool approx(double a, double b, double eps = 1e-9) {
  return std::fabs(a - b) <= eps;
}

// Test 1: When whiteY == 1, the encoding should remain unchanged (false branch of the if)
bool test_no_change_when_whiteY_is_one() {
  color_encoding enc;
  // Initialize with clear values
  enc.red.X = 10.0; enc.red.Y = 1.0; enc.red.Z = 20.0;
  enc.green.X = 5.0; enc.green.Y = 0.0; enc.green.Z = 9.0;
  enc.blue.X = 7.0; enc.blue.Y = 0.0; enc.blue.Z = 3.0;

  // Copy of initial values for later comparison
  const double redX0 = enc.red.X, redY0 = enc.red.Y, redZ0 = enc.red.Z;
  const double greenX0 = enc.green.X, greenY0 = enc.green.Y, greenZ0 = enc.green.Z;
  const double blueX0 = enc.blue.X, blueY0 = enc.blue.Y, blueZ0 = enc.blue.Z;

  // Call the function under test
  normalize_color_encoding(&enc);

  // Verify no changes occurred
  if (!approx(enc.red.X, redX0) || !approx(enc.red.Y, redY0) || !approx(enc.red.Z, redZ0) ||
      !approx(enc.green.X, greenX0) || !approx(enc.green.Y, greenY0) || !approx(enc.green.Z, greenZ0) ||
      !approx(enc.blue.X, blueX0) || !approx(enc.blue.Y, blueY0) || !approx(enc.blue.Z, blueZ0)) {
    printf("test_no_change_when_whiteY_is_one: FAILED - encoding changed when whiteY == 1\n");
    return false;
  }

  return true;
}

// Test 2: When whiteY != 1, all components should be divided by whiteY
bool test_normalization_applies_when_whiteY_not_one() {
  color_encoding enc;
  // Initialize with specific non-trivial values to assert correctness
  enc.red.X = 2.0; enc.red.Y = 0.4; enc.red.Z = 6.0;
  enc.green.X = 3.0; enc.green.Y = 0.2; enc.green.Z = 9.0;
  enc.blue.X = 4.0; enc.blue.Y = 0.2; enc.blue.Z = 12.0;

  // Expected whiteY = 0.4 + 0.2 + 0.2 = 0.8
  const double wY = 0.8;

  // Call the function under test
  normalize_color_encoding(&enc);

  // Validate expected results
  bool ok = true;
  ok &= approx(enc.red.X, 2.0 / wY);
  ok &= approx(enc.red.Y, 0.4 / wY);
  ok &= approx(enc.red.Z, 6.0 / wY);

  ok &= approx(enc.green.X, 3.0 / wY);
  ok &= approx(enc.green.Y, 0.2 / wY);
  ok &= approx(enc.green.Z, 9.0 / wY);

  ok &= approx(enc.blue.X, 4.0 / wY);
  ok &= approx(enc.blue.Y, 0.2 / wY);
  ok &= approx(enc.blue.Z, 12.0 / wY);

  if (!ok) {
    printf("test_normalization_applies_when_whiteY_not_one: FAILED - normalization results incorrect\n");
    // Debug print for pinpointing differences
    printf("Expected red: X=%.9f Y=%.9f Z=%.9f\n", 2.0/wY, 0.4/wY, 6.0/wY);
    printf("Actual   red: X=%.9f Y=%.9f Z=%.9f\n", enc.red.X, enc.red.Y, enc.red.Z);
    printf("Expected green: X=%.9f Y=%.9f Z=%.9f\n", 3.0/wY, 0.2/wY, 9.0/wY);
    printf("Actual   green: X=%.9f Y=%.9f Z=%.9f\n", enc.green.X, enc.green.Y, enc.green.Z);
    printf("Expected blue: X=%.9f Y=%.9f Z=%.9f\n", 4.0/wY, 0.2/wY, 12.0/wY);
    printf("Actual   blue: X=%.9f Y=%.9f Z=%.9f\n", enc.blue.X, enc.blue.Y, enc.blue.Z);
    return false;
  }

  return true;
}

// Main test harness
int main(void) {
  int failures = 0;

  if (!test_no_change_when_whiteY_is_one()) {
    ++failures;
  } else {
    printf("test_no_change_when_whiteY_is_one: PASSED\n");
  }

  if (!test_normalization_applies_when_whiteY_not_one()) {
    ++failures;
  } else {
    printf("test_normalization_applies_when_whiteY_not_one: PASSED\n");
  }

  if (failures == 0) {
    printf("All tests PASSED\n");
    return 0;
  } else {
    printf("%d test(s) FAILED\n", failures);
    return 1;
  }
}
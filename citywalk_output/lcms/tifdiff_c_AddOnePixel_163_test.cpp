// C++11 compatible unit test suite for AddOnePixel (no GTest, self-contained main)
 // Focus: test the behavior of AddOnePixel(LPSTAT st, double dE)
 // under various scenarios to achieve high coverage of the function logic.

#include <limits>
#include <iostream>
#include <utils.h>
#include <cmath>
#include <tiffio.h>


// Forward declaration of the focal function as it would appear in the tested code.
// The function is C linkage, so we declare it as extern "C".
extern "C" void AddOnePixel(void* stVoid, double dE);

// Minimal reproduction of the LPSTAT/STAT structure as expected by AddOnePixel.
// We assume the real project uses a struct with at least these five double fields.
// The exact typedef is not required for linking as long as the memory layout matches
// for the accessed members. We provide a compatible layout here for testing.
typedef struct _STAT {
    double x;
    double x2;
    double n;
    double Peak;
    double Min;
} STAT, *LPSTAT;

// Helper to bridge the opaque LPSTAT type used by the focal function.
// The actual AddOnePixel expects LPSTAT; we cast the opaque pointer accordingly.
static inline void AddOnePixelWrapper(LPSTAT st, double dE) {
    // The real function signature in the focal file is:
    // void AddOnePixel(LPSTAT st, double dE)
    // We pass st as the same pointer type expected by the function.
    AddOnePixel((void*)st, dE);
}

// Global test counters
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertion helpers
static void expect_true(bool cond, const char* desc) {
    ++g_total_tests;
    if (!cond) {
        ++g_failed_tests;
        std::cerr << "FAIL: " << desc << std::endl;
    } else {
        std::cout << "PASS: " << desc << std::endl;
    }
}

static void expect_double_eq(double actual, double expected, double tol, const char* desc) {
    expect_true(std::fabs(actual - expected) <= tol, desc);
}

// Test 1: Basic updates when initial Peak/Min are extreme values
// - Initialize Peak to -inf and Min to +inf to ensure both Peak and Min update on first dE.
static void test_AddOnePixel_basic_updates() {
    // Arrange
    STAT s;
    s.x = 0.0;
    s.x2 = 0.0;
    s.n = 0.0;
    s.Peak = -std::numeric_limits<double>::infinity();
    s.Min  = std::numeric_limits<double>::infinity();
    LPSTAT st = &s;

    // Act
    double dE = 3.0;
    AddOnePixelWrapper(st, dE);

    // Assert
    expect_double_eq(st->x, 3.0, 1e-9, "x should accumulate dE (3.0)");
    expect_double_eq(st->x2, 9.0, 1e-9, "x2 should accumulate dE^2 (9.0)");
    expect_double_eq(st->n, 1.0, 1e-9, "n should increment by 1.0");
    expect_double_eq(st->Peak, 3.0, 1e-9, "Peak should be updated to dE (3.0)");
    expect_double_eq(st->Min, 3.0, 1e-9, "Min should be updated to dE (3.0)");
}

// Test 2: Updates when dE is smaller than current Min and larger than current Peak
// - Reinitialize with specific values to isolate the Min and Peak updates.
static void test_AddOnePixel_min_and_peak_updates() {
    // Arrange
    STAT s;
    s.x = 0.0;
    s.x2 = 0.0;
    s.n = 0.0;
    s.Peak = -100.0;
    s.Min  = 50.0;
    LPSTAT st = &s;

    // Act
    double dE = -20.0;
    AddOnePixelWrapper(st, dE);

    // Assert
    expect_double_eq(st->x, -20.0, 1e-9, "x should be -20.0 after dE = -20.0");
    expect_double_eq(st->x2, 400.0, 1e-9, "x2 should be 400.0 after dE^2");
    expect_double_eq(st->n, 1.0, 1e-9, "n should be 1.0 after first pixel");
    expect_double_eq(st->Peak, -20.0, 1e-9, "Peak should update to -20.0");
    expect_double_eq(st->Min, -20.0, 1e-9, "Min should update to -20.0");
}

// Test 3: No changes to Peak or Min when dE is between current bounds
static void test_AddOnePixel_no_bound_updates() {
    // Arrange
    STAT s;
    s.x = 0.0;
    s.x2 = 0.0;
    s.n = 0.0;
    s.Peak = 0.5;
    s.Min  = -0.5;
    LPSTAT st = &s;

    // Act
    double dE = 0.3;
    AddOnePixelWrapper(st, dE);

    // Assert
    expect_double_eq(st->x, 0.3, 1e-9, "x should be 0.3 after dE = 0.3");
    expect_double_eq(st->x2, 0.09, 1e-9, "x2 should be 0.09 after dE^2");
    expect_double_eq(st->n, 1.0, 1e-9, "n should be 1.0 after first pixel");
    expect_double_eq(st->Peak, 0.5, 1e-9, "Peak should remain 0.5");
    expect_double_eq(st->Min, -0.5, 1e-9, "Min should remain -0.5");
}

// Test 4: dE equal to current Peak/Min does not trigger updates
static void test_AddOnePixel_equal_bound_no_update() {
    // Arrange: set Peak and Min to values, then provide dE exactly equal to Peak
    STAT s;
    s.x = 0.0;
    s.x2 = 0.0;
    s.n = 0.0;
    s.Peak = 2.0;
    s.Min  = -5.0;
    LPSTAT st = &s;

    // Act: dE equals current Peak
    double dE = 2.0;
    AddOnePixelWrapper(st, dE);

    // Assert: Peak should not change (equal), Min should not change
    expect_double_eq(st->x, 2.0, 1e-9, "x should be 2.0 after dE = Peak (2.0)");
    expect_double_eq(st->x2, 4.0, 1e-9, "x2 should be 4.0 after dE^2");
    expect_double_eq(st->n, 1.0, 1e-9, "n should be 1.0 after first pixel");
    expect_double_eq(st->Peak, 2.0, 1e-9, "Peak should remain 2.0 (no update on equal)");
    expect_double_eq(st->Min, -5.0, 1e-9, "Min should remain -5.0 (no update on dE > Min)");
}

// Test 5: Multiple sequential calls accumulate state correctly
static void test_AddOnePixel_multiple_calls_accumulate() {
    // Arrange
    STAT s;
    s.x = 0.0;
    s.x2 = 0.0;
    s.n = 0.0;
    s.Peak = -std::numeric_limits<double>::infinity();
    s.Min  = std::numeric_limits<double>::infinity();
    LPSTAT st = &s;

    // Act: first pixel
    AddOnePixelWrapper(st, 4.0);
    // Assert after first
    expect_double_eq(st->x, 4.0, 1e-9, "After first dE=4.0, x should be 4.0");
    expect_double_eq(st->x2, 16.0, 1e-9, "After first dE=4.0, x2 should be 16.0");
    expect_double_eq(st->n, 1.0, 1e-9, "After first dE=4.0, n should be 1.0");
    expect_double_eq(st->Peak, 4.0, 1e-9, "After first dE=4.0, Peak should be 4.0");
    expect_double_eq(st->Min, 4.0, 1e-9, "After first dE=4.0, Min should be 4.0");

    // Act: second pixel
    AddOnePixelWrapper(st, -1.0);

    // Assert after second
    expect_double_eq(st->x, 3.0, 1e-9, "After second dE=-1.0, x should be 3.0");
    expect_double_eq(st->x2, 17.0, 1e-9, "After second dE=-1.0, x2 should be 17.0");
    expect_double_eq(st->n, 2.0, 1e-9, "After second pixel, n should be 2.0");
    expect_double_eq(st->Peak, 4.0, 1e-9, "Peak should remain 4.0 after second pixel");
    expect_double_eq(st->Min, -1.0, 1e-9, "Min should update to -1.0 after second pixel");
}

// Driver to run all tests and report summary
static void run_all_tests() {
    test_AddOnePixel_basic_updates();
    test_AddOnePixel_min_and_peak_updates();
    test_AddOnePixel_no_bound_updates();
    test_AddOnePixel_equal_bound_no_update();
    test_AddOnePixel_multiple_calls_accumulate();

    std::cout << "Total tests: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
}

// Entry point
int main() {
    // Run the suite
    run_all_tests();

    // Return non-zero if any test failed
    if (g_failed_tests > 0) {
        std::cerr << "Test suite completed with failures." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed successfully." << std::endl;
        return 0;
    }
}
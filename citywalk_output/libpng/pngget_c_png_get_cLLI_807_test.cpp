/*
Unit test suite for the focal function: png_get_cLLI
This test suite is self-contained (no external GTest) and mimics a minimal
libpng-like environment sufficient to exercise the png_get_cLLI logic.

Key points covered:
- True/false branches for input validation predicates.
- Correct computation of output values when maxCLL/maxFALL are provided.
- Handling of NULL pointers for png_ptr and info_ptr.
- Distinguishing whether to write to maxCLL/maxFALL based on non-NULL pointers.

Notes:
- The implementation mirrors the focal method's behavior exactly as given.
- All tests use terminating-free (non-throwing) assertions; failures are collected
  and reported at the end.
*/

#include <string>
#include <cmath>
#include <iostream>
#include <pngpriv.h>
#include <limits>


// Domain knowledge helpers
namespace test_domain {

// Lightweight test assertion helpers (non-terminating)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++test_domain::g_failures; \
        std::cerr << "[FAIL] " << (msg) << " | Condition: " << #cond << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(val, exp, msg) do { \
    if(!((val) == (exp))) { \
        ++test_domain::g_failures; \
        std::cerr << "[FAIL] " << (msg) \
                  << " | Expected: " << (exp) \
                  << ", Got: " << (val) << std::endl; \
    } \
} while(0)

#define EXPECT_EQ_DBL(a, b, tol, msg) do { \
    double diff = std::fabs((a) - (b)); \
    if(!(diff <= (tol))) { \
        ++test_domain::g_failures; \
        std::cerr << "[FAIL] " << (msg) \
                  << " | Expected: " << (b) \
                  << ", Got: " << (a) \
                  << " (diff=" << diff << ")" << std::endl; \
    } \
} while(0)

} // namespace test_domain

// Minimal mock of the C API/types required by the focal function
extern "C" {

// Forward declare minimal structures to resemble libpng types
struct png_struct_def {
    int dummy;
};

// const pointer type for png_ptr
typedef const png_struct_def* png_const_structrp;
typedef png_struct_def*       png_structrp;

// Info structure with fields accessed by png_get_cLLI
struct png_info {
    unsigned int valid;
    unsigned int maxCLL;
    unsigned int maxFALL;
};

// Pointers to info structures (opaque in real libpng)
typedef png_info*       png_inforp;
typedef const png_info* png_const_inforp;

// PNG_INFO_cLLI bit flag (mirrors real library usage)
#define PNG_INFO_cLLI 0x01

// Minimal no-op macro for debug (as used in focal method)
#define png_debug1(level, fmt, arg) do { (void)(level); (void)(fmt); (void)(arg); } while(0)

// The focal method under test (copied/replicated for a self-contained test)
int png_get_cLLI(png_const_structrp png_ptr, png_const_inforp info_ptr,
      double *maxCLL, double *maxFALL)
{
{
   png_debug1(1, "in %s retrieval function", "cLLI(float)");
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_cLLI) != 0)
   {
      if (maxCLL != NULL) *maxCLL = info_ptr->maxCLL * .0001;
      if (maxFALL != NULL) *maxFALL = info_ptr->maxFALL * .0001;
      return PNG_INFO_cLLI;
   }
   return 0;
}
}
} // extern "C"

// Lightweight test harness wrappers
static void run_all_heavy_tests();

int main() {
    using namespace test_domain;

    // Individual unit tests
    // Test 1: NULL png_ptr should yield 0 and not modify output vars
    {
        // Arrange
        png_info info;
        info.valid = PNG_INFO_cLLI;
        info.maxCLL = 12345;
        info.maxFALL = 54321;
        double cll = 0.0, fall = 0.0;
        png_const_structrp null_ptr = nullptr;
        png_const_inforp null_info = reinterpret_cast<png_const_inforp>(&info);

        // Act
        int ret = png_get_cLLI(null_ptr, null_info, &cll, &fall);

        // Assert
        EXPECT_EQ(ret, 0, "png_get_cLLI should return 0 when png_ptr is NULL");
        // Out parameters should remain unchanged
        EXPECT_EQ(cll, 0.0, "maxCLL should remain unchanged when png_ptr is NULL");
        EXPECT_EQ(fall, 0.0, "maxFALL should remain unchanged when png_ptr is NULL");
    }

    // Test 2: NULL info_ptr should yield 0
    {
        // Arrange
        png_struct_def dummy;
        png_structrp ptr = &dummy;
        double cll = 0.0, fall = 0.0;
        int ret;
        // Act
        ret = png_get_cLLI(reinterpret_cast<png_const_structrp>(ptr), nullptr, &cll, &fall);
        // Assert
        EXPECT_EQ(ret, 0, "png_get_cLLI should return 0 when info_ptr is NULL");
        EXPECT_EQ(cll, 0.0, "maxCLL should remain unchanged when info_ptr is NULL");
        EXPECT_EQ(fall, 0.0, "maxFALL should remain unchanged when info_ptr is NULL");
    }

    // Test 3: info_ptr valid flag not set should return 0
    {
        // Arrange
        png_struct_def dummy;
        png_structrp ptr = &dummy;
        png_info info;
        info.valid = 0; // bit not set
        info.maxCLL = 100;
        info.maxFALL = 200;
        double cll = 0.0, fall = 0.0;
        // Act
        int ret = png_get_cLLI(reinterpret_cast<png_const_structrp>(ptr), &info, &cll, &fall);
        // Assert
        EXPECT_EQ(ret, 0, "png_get_cLLI should return 0 when PNG_INFO_cLLI flag is not set");
        // Output values should remain unchanged
        EXPECT_EQ(cll, 0.0, "maxCLL should remain unchanged when flag not set");
        EXPECT_EQ(fall, 0.0, "maxFALL should remain unchanged when flag not set");
    }

    // Test 4: info_ptr valid and PNG_INFO_cLLI set; both outputs provided
    {
        // Arrange
        png_struct_def dummy;
        png_structrp ptr = &dummy;
        png_info info;
        info.valid = PNG_INFO_cLLI;
        info.maxCLL = 10000; // scaled by 0.0001 -> 1.0
        info.maxFALL = 5000;  // scaled by 0.0001 -> 0.5
        double cll = 0.0, fall = 0.0;
        // Act
        int ret = png_get_cLLI(reinterpret_cast<png_const_structrp>(ptr), &info, &cll, &fall);
        // Assert
        EXPECT_EQ(ret, PNG_INFO_cLLI, "png_get_cLLI should return PNG_INFO_cLLI when valid");
        EXPECT_EQ_DBL(cll, 1.0, 1e-12, "maxCLL should be info.maxCLL * 0.0001 (1.0)");
        EXPECT_EQ_DBL(fall, 0.5, 1e-12, "maxFALL should be info.maxFALL * 0.0001 (0.5)");
    }

    // Test 5: info_ptr valid; only maxCLL non-NULL (maxFALL NULL)
    {
        // Arrange
        png_struct_def dummy;
        png_structrp ptr = &dummy;
        png_info info;
        info.valid = PNG_INFO_cLLI;
        info.maxCLL = 12345; // 1.2345
        info.maxFALL = 9999;  // should be ignored
        double cll = 0.0;
        // Act
        int ret = png_get_cLLI(reinterpret_cast<png_const_structrp>(ptr), &info, &cll, nullptr);
        // Assert
        EXPECT_EQ(ret, PNG_INFO_cLLI, "png_get_cLLI should still return PNG_INFO_cLLI when maxFALL is NULL");
        EXPECT_EQ_DBL(cll, 1.2345, 1e-12, "maxCLL should be info.maxCLL * 0.0001 (1.2345)");
    }

    // Test 6: info_ptr valid; only maxFALL non-NULL (maxCLL NULL)
    {
        // Arrange
        png_struct_def dummy;
        png_structrp ptr = &dummy;
        png_info info;
        info.valid = PNG_INFO_cLLI;
        info.maxCLL = 98765;
        info.maxFALL = 43210; // 4.3210
        double fall = 0.0;
        // Act
        int ret = png_get_cLLI(reinterpret_cast<png_const_structrp>(ptr), &info, nullptr, &fall);
        // Assert
        EXPECT_EQ(ret, PNG_INFO_cLLI, "png_get_cLLI should return PNG_INFO_cLLI when maxCLL is NULL and maxFALL non-NULL");
        EXPECT_EQ_DBL(fall, 4.321, 1e-12, "maxFALL should be info.maxFALL * 0.0001 (4.3210)");
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "[OK] All tests passed." << std::endl;
    } else {
        std::cout << "[SUMMARY] Total failures: " << g_failures << std::endl;
    }

    return g_failures;
}
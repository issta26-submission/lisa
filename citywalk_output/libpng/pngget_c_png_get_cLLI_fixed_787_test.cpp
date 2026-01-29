// Minimal C++11 unit test suite for the focal function png_get_cLLI_fixed
// This test suite provides a self-contained mock of the necessary PNG structures
// and tests the exact logic described in the focal method without relying on GTest.

#include <cstdint>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <pngpriv.h>


// Lightweight mock of necessary PNG components to compile and test the focal function.
// These definitions imitate the essential API surface used by png_get_cLLI_fixed.

#define PNG_INFO_cLLI 0x01  // bit flag representing cLLI validity
#define png_debug1(level, fmt, arg) (void)0  // no-op for test environment

// Forward-declared opaque types to resemble libpng's usage in the focal method
struct png_struct;
struct png_info;

// Typedefs to mimic libpng typedef naming in a simplified way
typedef const struct png_struct* png_const_structrp;
typedef struct png_struct*       png_structrp;
typedef struct png_info*           png_inforp;
typedef const struct png_info*     png_const_inforp;
typedef uint32_t png_uint_32;
typedef uint32_t* png_uint_32p;

// Implementations of the minimal structures
struct png_struct {
    // Intentionally empty
};

struct png_info {
    unsigned valid;
    unsigned maxCLL;
    unsigned maxFALL;
};

// png_get_cLLI_fixed prototype (as in the focal method)
/*
png_uint_32 png_get_cLLI_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32p maxCLL,
    png_uint_32p maxFALL)
*/
static unsigned png_get_cLLI_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32p maxCLL, png_uint_32p maxFALL)
{
    // The function under test: returns PNG_INFO_cLLI when valid, else 0
    png_debug1(1, "in %s retrieval function", "cLLI");
    if (png_ptr != NULL && info_ptr != NULL &&
        (info_ptr->valid & PNG_INFO_cLLI) != 0)
    {
        if (maxCLL != NULL) *maxCLL = info_ptr->maxCLL;
        if (maxFALL != NULL) *maxFALL = info_ptr->maxFALL;
        return PNG_INFO_cLLI;
    }
    return 0;
}

// Simple test framework (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Helper to stringify values for assertion messages

template <typename T>
static std::string to_string_t(const T& v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

static void log_failure(const std::string& msg) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
}

// Assertions
#define EXPECT_EQ(a, b) do { ++g_total; if (!((a) == (b))) { ++g_failed; std::cerr << "  Expected: " << to_string_t((a)) << " == " << to_string_t((b))
    << "  [at " << __FILE__ << ":" << __LINE__ << "]\n"; } } while(0)

#define EXPECT_TRUE(cond) do { ++g_total; if (!(cond)) { ++g_failed; std::cerr << "  Expected: condition true but was false"
    << "  [at " << __FILE__ << ":" << __LINE__ << "]\n"; } } while(0)

#define EXPECT_FALSE(cond) do { ++g_total; if ((cond)) { ++g_failed; std::cerr << "  Expected: condition false but was true"
    << "  [at " << __FILE__ << ":" << __LINE__ << "]\n"; } } while(0)

#define RUN_TEST(func) do { std::cout << "RUNNING " << #func << "..." << std::endl; func(); } while(0)

// Test 1: When both png_ptr and info_ptr are non-null and info_ptr->valid has PNG_INFO_cLLI set,
// the function should write maxCLL/maxFALL (when non-null) and return PNG_INFO_cLLI.
static void test_case_successful_read_both_pointers_set() {
    // Arrange
    png_struct *ptr = new png_struct();
    png_const_structrp png_ptr = ptr;

    png_info info;
    info.valid = PNG_INFO_cLLI;
    info.maxCLL = 1234;
    info.maxFALL = 5678;

    uint32_t cLLL = 0;
    uint32_t FALL = 0;

    // Act
    unsigned ret = png_get_cLLI_fixed(png_ptr, &info, &cLLL, &FALL);

    // Assert
    EXPECT_EQ(ret, static_cast<unsigned>(PNG_INFO_cLLI));
    EXPECT_EQ(cLLL, static_cast<uint32_t>(1234));
    EXPECT_EQ(FALL, static_cast<uint32_t>(5678));

    delete ptr;
}

// Test 1a: Ensure that passing a NULL maxCLL still writes maxFALL correctly when the info_ptr is valid.
static void test_case_successful_read_only_maxFALL() {
    // Arrange
    png_struct *ptr = new png_struct();
    png_const_structrp png_ptr = ptr;

    png_info info;
    info.valid = PNG_INFO_cLLI;
    info.maxCLL = 9999;
    info.maxFALL = 42;

    uint32_t cLLL = 0;
    uint32_t FALL = 0;

    // Act
    unsigned ret = png_get_cLLI_fixed(png_ptr, &info, nullptr, &FALL);

    // Assert
    EXPECT_EQ(ret, static_cast<unsigned>(PNG_INFO_cLLI));
    // maxFALL should be written
    EXPECT_EQ(FALL, static_cast<uint32_t>(42));
    // maxCLL was NULL, so cLLL remains unchanged (0)
    EXPECT_EQ(cLLL, static_cast<uint32_t>(0));

    delete ptr;
}

// Test 1b: Ensure that passing a NULL maxFALL writes maxCLL correctly when non-NULL.
static void test_case_successful_read_only_maxCLL() {
    // Arrange
    png_struct *ptr = new png_struct();
    png_const_structrp png_ptr = ptr;

    png_info info;
    info.valid = PNG_INFO_cLLI;
    info.maxCLL = 777;
    info.maxFALL = 8888;

    uint32_t cLLL = 0;
    uint32_t FALL = 0;

    // Act
    unsigned ret = png_get_cLLI_fixed(png_ptr, &info, &cLLL, nullptr);

    // Assert
    EXPECT_EQ(ret, static_cast<unsigned>(PNG_INFO_cLLI));
    EXPECT_EQ(cLLL, static_cast<uint32_t>(777));
    // maxFALL should be untouched since it's NULL
    EXPECT_EQ(FALL, static_cast<uint32_t>(0));

    delete ptr;
}

// Test 2: When the info_ptr valid flag does not include PNG_INFO_cLLI, the function should return 0
// and not modify any output parameters.
static void test_case_invalid_valid_bit_no_write() {
    // Arrange
    png_struct *ptr = new png_struct();
    png_const_structrp png_ptr = ptr;

    png_info info;
    info.valid = 0;          // flag does not include cLLI
    info.maxCLL = 555;
    info.maxFALL = 666;

    uint32_t cLLL = 0xDEADBEEF;
    uint32_t FALL = 0xBAADF00D;

    // Act
    unsigned ret = png_get_cLLI_fixed(png_ptr, &info, &cLLL, &FALL);

    // Assert
    EXPECT_EQ(ret, static_cast<unsigned>(0));
    // Outputs should remain unchanged
    EXPECT_EQ(cLLL, static_cast<uint32_t>(0xDEADBEEF));
    EXPECT_EQ(FALL, static_cast<uint32_t>(0xBAADF00D));

    delete ptr;
}

// Test 3: NULL pointers should yield 0 and no modifications occur
static void test_case_null_ptrs() {
    // Arrange
    png_info info;
    info.valid = PNG_INFO_cLLI;
    info.maxCLL = 123;
    info.maxFALL = 456;

    uint32_t cLLL = 1;
    uint32_t FALL = 2;

    // Act
    unsigned ret = png_get_cLLI_fixed(nullptr, &info, &cLLL, &FALL);

    // Assert
    EXPECT_EQ(ret, static_cast<unsigned>(0));
    // No modification should occur
    EXPECT_EQ(cLLL, static_cast<uint32_t>(1));
    EXPECT_EQ(FALL, static_cast<uint32_t>(2));

    // Also test with null info_ptr
    ret = png_get_cLLI_fixed(reinterpret_cast<png_const_structrp>(0x1), nullptr, &cLLL, &FALL);
    EXPECT_EQ(ret, static_cast<unsigned>(0));
}

// Test 4: Null max parameter pointers do not crash
static void test_case_null_max_params_do_not_crash() {
    // Arrange
    png_struct *ptr = new png_struct();
    png_const_structrp png_ptr = ptr;
    png_info info;
    info.valid = PNG_INFO_cLLI;
    info.maxCLL = 42;
    info.maxFALL = 84;

    // Act
    unsigned ret = png_get_cLLI_fixed(png_ptr, &info, nullptr, nullptr);

    // Assert: should still return PNG_INFO_cLLI since pointers exist but both max params are NULL
    // The original code checks only the pointers; since both pointers are NULL, no writes occur
    EXPECT_EQ(ret, static_cast<unsigned>(PNG_INFO_cLLI));
    delete ptr;
}

// Runner
int main() {
    std::cout << "Starting png_get_cLLI_fixed unit tests (self-contained, no external test framework)" << std::endl;

    RUN_TEST(test_case_successful_read_both_pointers_set);
    RUN_TEST(test_case_successful_read_only_maxFALL);
    RUN_TEST(test_case_successful_read_only_maxCLL);
    RUN_TEST(test_case_invalid_valid_bit_no_write);
    RUN_TEST(test_case_null_ptrs);
    RUN_TEST(test_case_null_max_params_do_not_crash);

    std::cout << "Tests run: " << g_total << ", Failures: " << g_failed << std::endl;
    if (g_failed != 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }
    std::cout << "All tests PASSED." << std::endl;
    return 0;
}
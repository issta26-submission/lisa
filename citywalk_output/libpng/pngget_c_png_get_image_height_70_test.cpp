// Minimal C++11 test suite for the focal function png_get_image_height
// This test does not rely on GoogleTest and uses a lightweight custom test harness
// to exercise true/false branches of the function's conditional logic.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Declaration of the focal function with C linkage to ensure proper symbol resolution.
// We intentionally use a generic pointer signature here (void*) since the actual internal
// types are defined by the library (pngpriv.h). The runtime behavior only depends on
// the non-null pointers and the height member inside info_ptr.
extern "C" unsigned int png_get_image_height(void* png_ptr, void* info_ptr);

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating check macro: reports failures but continues execution
#define CHECK(cond, msg)                                            \
    do {                                                            \
        ++g_total_tests;                                            \
        if (!(cond)) {                                              \
            ++g_failed_tests;                                       \
            std::cerr << "TEST FAILED: " << msg                   \
                      << "  [test=" << g_total_tests              \
                      << ", file=" << __FILE__                     \
                      << ", line=" << __LINE__ << "]" << std::endl; \
        } else {                                                    \
            std::cout << "TEST PASSED: " << msg << std::endl;      \
        }                                                           \
    } while (0)

// Test 1: True branch - both pointers non-null; height is read from info_ptr
// We craft a fake info_ptr memory where the first 4 bytes represent the height value.
// This relies on the internal layout of png_info such that height is the first member.
// This matches common internal layouts used by libpng (height often appears first).
void test_height_read_when_both_ptrs_non_null()
{
    // Arrange
    uint32_t height_value = 0xDEADBEEF; // known height
    void* fake_png_ptr = reinterpret_cast<void*>(0x1); // non-null dummy png_ptr

    // Pass the address of height_value as the info_ptr
    void* fake_info_ptr = static_cast<void*>(&height_value);

    // Act
    unsigned int result = png_get_image_height(fake_png_ptr, fake_info_ptr);

    // Assert: result should equal the height value stored in height_value
    CHECK(result == height_value,
          "png_get_image_height returns info_ptr->height when both pointers are non-null");
}

// Test 2: False branch - png_ptr is NULL -> should return 0 regardless of info_ptr
void test_height_returns_zero_when_png_ptr_null()
{
    // Arrange
    uint32_t height_value = 1234;
    void* info_ptr = static_cast<void*>(&height_value);

    // Act
    unsigned int result = png_get_image_height(nullptr, info_ptr);

    // Assert
    CHECK(result == 0,
          "png_get_image_height returns 0 when png_ptr is NULL");
}

// Test 3: False branch - info_ptr is NULL -> should return 0 regardless of png_ptr
void test_height_returns_zero_when_info_ptr_null()
{
    // Arrange
    void* png_ptr = reinterpret_cast<void*>(0x2);

    // Act
    unsigned int result = png_get_image_height(png_ptr, nullptr);

    // Assert
    CHECK(result == 0,
          "png_get_image_height returns 0 when info_ptr is NULL");
}

// Test 4: Both NULL -> should return 0
void test_height_returns_zero_when_both_null()
{
    // Act
    unsigned int result = png_get_image_height(nullptr, nullptr);

    // Assert
    CHECK(result == 0,
          "png_get_image_height returns 0 when both pointers are NULL");
}

// Optional Test 5: Non-null pointers with zero height value
void test_height_zero_value()
{
    // Arrange
    uint32_t height_value = 0; // explicit zero height
    void* fake_png_ptr = reinterpret_cast<void*>(0x3);
    void* fake_info_ptr = static_cast<void*>(&height_value);

    // Act
    unsigned int result = png_get_image_height(fake_png_ptr, fake_info_ptr);

    // Assert
    CHECK(result == height_value,
          "png_get_image_height correctly returns zero when info_ptr->height is zero");
}

int main()
{
    std::cout << "Starting unit tests for png_get_image_height..." << std::endl;

    // Run tests
    test_height_read_when_both_ptrs_non_null();
    test_height_returns_zero_when_png_ptr_null();
    test_height_returns_zero_when_info_ptr_null();
    test_height_returns_zero_when_both_null();
    test_height_zero_value();

    // Summary
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Total tests run: " << g_total_tests << std::endl;
    std::cout << "Total failures:  " << g_failed_tests << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
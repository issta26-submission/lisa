// Unit test suite for the focal method png_zfree (located in png.c)
// This test suite is written for C++11, without using GTest.
// It mocks the dependency png_free to verify behavior of png_zfree.
// The tests are executed from main() as per domain knowledge guidance.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


// Forward declaration of the function under test.
// We assume the real signature uses "voidpf" which is typically a typedef to void*.
// For testing purposes we declare a compatible C linkage version.
extern "C" void png_zfree(void* png_ptr, void* ptr);

// Mocked dependency: png_free
// We provide a C-linkage mock to intercept calls made by png_zfree.
// The real png_free would deallocate memory; here we only record the arguments for assertions.
static bool g_png_free_called = false;
static void* g_last_png_ptr = nullptr;
static void* g_last_ptr = nullptr;

extern "C" void png_free(void* png_ptr, void* ptr)
{
    g_png_free_called = true;
    g_last_png_ptr = png_ptr;
    g_last_ptr = ptr;
}

// Utility to reset mock state before each test
static void reset_mock_state()
{
    g_png_free_called = false;
    g_last_png_ptr = nullptr;
    g_last_ptr = nullptr;
}

// Test 1: png_zfree should pass through the second argument (ptr) unchanged to png_free
// This ensures the wrapper does not alter the memory being freed.
static void test_png_zfree_pass_through_second_arg()
{
    reset_mock_state();

    void* test_png_ptr = reinterpret_cast<void*>(0x1001);
    void* test_ptr     = reinterpret_cast<void*>(0x2002);

    // Call the function under test
    png_zfree(test_png_ptr, test_ptr);

    // Validate that png_free was invoked
    if (!g_png_free_called) {
        std::cout << "[FAIL] test_png_zfree_pass_through_second_arg: png_free was not called.\n";
        return;
    }

    // Validate that the second argument (ptr) is passed unchanged
    if (g_last_ptr != test_ptr) {
        std::cout << "[FAIL] test_png_zfree_pass_through_second_arg: expected ptr "
                  << test_ptr << " but got " << g_last_ptr << "\n";
        return;
    }

    // We don't assert on the first parameter (png_ptr) because the code casts it
    // via png_voidcast/pointer-compatibility logic; what matters for correctness
    // here is that the memory pointer to free was passed through as-is.
    std::cout << "[PASS] test_png_zfree_pass_through_second_arg\n";
}

// Test 2: png_zfree should handle NULL ptr gracefully (still passes NULL to png_free)
// This simulates freeing a NULL pointer; behavior should be to attempt to free NULL safely.
static void test_png_zfree_null_ptr_handling()
{
    reset_mock_state();

    void* test_png_ptr = reinterpret_cast<void*>(0x3003);
    void* test_ptr     = nullptr;

    png_zfree(test_png_ptr, test_ptr);

    if (!g_png_free_called) {
        std::cout << "[FAIL] test_png_zfree_null_ptr_handling: png_free was not called.\n";
        return;
    }

    if (g_last_ptr != nullptr) {
        std::cout << "[FAIL] test_png_zfree_null_ptr_handling: expected NULL ptr, got "
                  << g_last_ptr << "\n";
        return;
    }

    std::cout << "[PASS] test_png_zfree_null_ptr_handling\n";
}

// Test 3: png_zfree should handle NULL png_ptr (the png_ptr argument is casted internally)
// We verify that the second argument is still passed through unchanged to png_free.
static void test_png_zfree_null_png_ptr()
{
    reset_mock_state();

    void* test_png_ptr = nullptr;
    void* test_ptr     = reinterpret_cast<void*>(0xDEAD);

    png_zfree(test_png_ptr, test_ptr);

    if (!g_png_free_called) {
        std::cout << "[FAIL] test_png_zfree_null_png_ptr: png_free was not called.\n";
        return;
    }

    if (g_last_ptr != test_ptr) {
        std::cout << "[FAIL] test_png_zfree_null_png_ptr: expected ptr " << test_ptr
                  << " but got " << g_last_ptr << "\n";
        return;
    }

    std::cout << "[PASS] test_png_zfree_null_png_ptr\n";
}

// Main function to execute tests
int main()
{
    // Explain what is being tested
    std::cout << "png_zfree unit tests (C++11, without Google Test)\n";
    std::cout << "-----------------------------------------------\n";

    test_png_zfree_pass_through_second_arg();
    test_png_zfree_null_ptr_handling();
    test_png_zfree_null_png_ptr();

    std::cout << "-----------------------------------------------\n";
    std::cout << "All tests executed.\n";
    return 0;
}
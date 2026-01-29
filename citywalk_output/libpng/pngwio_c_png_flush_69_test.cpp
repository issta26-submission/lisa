/*
Step 1. Program Understanding (Candidate Keywords)
- Candidate Keywords: png_ptr, output_flush_fn, png_struct, png_structrp, function pointer call, NULL check, side-effect (callback invocation)
- These guide the tests to verify the behavior when the output_flush_fn is NULL or non-NULL and ensure the callback is invoked with the correct argument.
*/

/*
This test suite provides a self-contained mock environment to exercise the focal behavior of png_flush as described:

png_flush(png_structrp png_ptr)
{
{
   if (png_ptr->output_flush_fn != NULL)
      (*(png_ptr->output_flush_fn))(png_ptr);
}
}

Notes:
- We implement a minimal mock of the necessary libpng-like types and the exact function under test in C++11.
- Tests cover: (a) NULL callback does nothing, (b) non-NULL callback is invoked exactly once with the correct pointer.
- A tiny, self-contained test harness with non-terminating EXPECT_* style checks is provided (no GoogleTest).
*/

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal mock-up of the libpng-like types used by png_flush
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;             // png_structrp behaves as png_struct*
typedef const unsigned char* png_const_bytep;
typedef unsigned char* png_bytep;

// Callback type expected by png_flush
typedef void (*png_flush_ptr)(png_structrp);

// Minimal png_struct with the field used by png_flush
struct png_struct {
    png_flush_ptr output_flush_fn;
    // Other members are omitted as they're not required for this test
};

// Forward declaration of the focal function under test (as in the real code)
extern "C" void png_flush(png_structrp png_ptr);

//
// Focal method implementation (replica for test purposes; mirrors the provided snippet)
// This is the exact behavior described: if non-NULL, call the callback with png_ptr.
//
extern "C" void png_flush(png_structrp png_ptr)
{
    {
       if (png_ptr->output_flush_fn != NULL)
          (*(png_ptr->output_flush_fn))(png_ptr);
    }
}

/*
Test harness utilities
- Lightweight, non-terminating test assertions (EXPECT_* style)
- Tracks total tests and failed tests
- Outputs per-test diagnostics to std::cerr
*/

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Global state to observe callback invocation
static bool g_callback_was_called = false;
static png_structrp g_callback_received_ptr = nullptr;

// A test callback that records invocation and the received pointer
static void test_callback(png_structrp ptr)
{
    g_callback_was_called = true;
    g_callback_received_ptr = ptr;
}

// Assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_tests; \
        if(!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "[FAILED] " << __FUNCTION__ << ": Expected " #cond << std::endl; \
        } \
    } while(0)

#define EXPECT_EQ_PTR(a, b) \
    do { \
        ++g_total_tests; \
        if((void*)(a) != (void*)(b)) { \
            ++g_failed_tests; \
            std::cerr << "[FAILED] " << __FUNCTION__ << ": Pointers differ: " \
                      << static_cast<const void*>(a) << " != " \
                      << static_cast<const void*>(b) << std::endl; \
        } \
    } while(0)

// Test 1: When output_flush_fn is NULL, png_flush should not call any function.
static void test_png_flush_no_callback()
{
    // Arrange
    png_struct s;
    s.output_flush_fn = nullptr;
    g_callback_was_called = false;
    g_callback_received_ptr = nullptr;

    // Act
    png_flush(&s);

    // Assert
    EXPECT_TRUE(g_callback_was_called == false); // callback should not be invoked
    // Pointer should remain untouched
    EXPECT_EQ_PTR(g_callback_received_ptr, (png_structrp)nullptr);
}

// Test 2: When output_flush_fn is non-NULL, png_flush should call it exactly once with the same pointer.
static void test_png_flush_with_callback_invoked_once()
{
    // Arrange
    png_struct s;
    s.output_flush_fn = test_callback;
    g_callback_was_called = false;
    g_callback_received_ptr = nullptr;

    // Act
    png_flush(&s);

    // Assert
    EXPECT_TRUE(g_callback_was_called);                 // callback should be invoked
    EXPECT_EQ_PTR(g_callback_received_ptr, &s);         // callback should receive the original pointer
}

// Test 3 (additional coverage): Ensure subsequent invocations work when switching callbacks
static void test_png_flush_with_different_callbacks()
{
    // Arrange first instance with test_callback
    png_struct s1;
    s1.output_flush_fn = test_callback;
    g_callback_was_called = false;
    g_callback_received_ptr = nullptr;

    // Act first call
    png_flush(&s1);
    EXPECT_TRUE(g_callback_was_called);
    EXPECT_EQ_PTR(g_callback_received_ptr, &s1);

    // Reset and switch to a NULL callback to ensure no further calls occur
    s1.output_flush_fn = nullptr;
    g_callback_was_called = false;
    g_callback_received_ptr = nullptr;
    png_flush(&s1);
    EXPECT_TRUE(g_callback_was_called == false);
    EXPECT_EQ_PTR(g_callback_received_ptr, nullptr);
}

// Runner
static void run_all_tests()
{
    test_png_flush_no_callback();
    test_png_flush_with_callback_invoked_once();
    test_png_flush_with_different_callbacks();

    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
}

int main()
{
    run_all_tests();
    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
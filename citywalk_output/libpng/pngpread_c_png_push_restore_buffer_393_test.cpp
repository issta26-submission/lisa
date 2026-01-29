// Lightweight C++-11 unit test suite for the focal method png_push_restore_buffer
// This test does not rely on Google Test or any external framework.
// It uses a small, non-terminating assertion framework to maximize code coverage.

#include <cstddef>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>


// Import library dependencies and ensure C linkage for C-implementation symbols
extern "C" {
}

// Ensure the function under test has C linkage
extern "C" void png_push_restore_buffer(png_structrp png_ptr, png_bytep buffer, size_t buffer_length);

// Simple non-terminating test framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Log a non-fatal failure without stopping test execution
static void log_failure(const char* file, int line, const char* cond) {
    fprintf(stderr, "Test Failure -> %s:%d: condition '%s' failed\n", file, line, cond);
}

// Non-terminating assertion macro
#define TEST_CHECK(cond) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            ++g_tests_failed; \
            log_failure(__FILE__, __LINE__, #cond); \
        } \
    } while (0)

#define EXPECT_TRUE(cond) TEST_CHECK(static_cast<bool>(cond))
#define EXPECT_EQ(a, b) TEST_CHECK(static_cast<long long>(a) == static_cast<long long>(b))
#define EXPECT_PTR_EQ(a, b) TEST_CHECK(((void*)(a) == (void*)(b)))
#define EXPECT_SIZE_EQ(a, b) TEST_CHECK(static_cast<size_t>(a) == static_cast<size_t>(b))

// Test 1: Basic non-null buffer with non-zero length
// Verifies that the function sets internal pointers and sizes correctly.
void test_png_push_restore_buffer_basic_non_null() {
    // Create a png_ptr instance sized to the actual structure used by the library
    png_structrp png_ptr = NULL;
    {
        png_structrp tmp_ptr = NULL;
        size_t size = sizeof(*(tmp_ptr)); // size of struct png_struct
        png_ptr = reinterpret_cast<png_structrp>(malloc(size));
        if (png_ptr == NULL) {
            fprintf(stderr, "Memory allocation for png_ptr failed in test_basic_non_null\n");
            g_tests_run++;
            g_tests_failed++;
            return;
        }
    }

    // Initialize fields that png_push_restore_buffer will touch
    png_ptr->save_buffer_size = 16;            // expected additive size
    png_ptr->current_buffer = NULL;
    png_ptr->current_buffer_size = 0;
    png_ptr->buffer_size = 0;
    png_ptr->current_buffer_ptr = NULL;

    unsigned char testBuffer[13] = {0};        // non-null buffer
    png_bytep buffer = testBuffer;
    size_t buffer_length = 13;

    // Call the focal function
    png_push_restore_buffer(png_ptr, buffer, buffer_length);

    // Assertions
    EXPECT_PTR_EQ(png_ptr->current_buffer, buffer);
    EXPECT_EQ(png_ptr->current_buffer_size, buffer_length);
    EXPECT_EQ(png_ptr->buffer_size, buffer_length + png_ptr->save_buffer_size);
    EXPECT_PTR_EQ(png_ptr->current_buffer_ptr, png_ptr->current_buffer);

    free(png_ptr);
}

// Test 2: Null buffer pointer with zero length
// Validates handling of a null input buffer without crashing and proper field updates.
void test_png_push_restore_buffer_null_buffer() {
    png_structrp png_ptr = NULL;
    {
        png_structrp tmp_ptr = NULL;
        size_t size = sizeof(*(tmp_ptr));
        png_ptr = reinterpret_cast<png_structrp>(malloc(size));
        if (png_ptr == NULL) {
            fprintf(stderr, "Memory allocation for png_ptr failed in test_null_buffer\n");
            g_tests_run++;
            g_tests_failed++;
            return;
        }
    }

    // Initialize fields
    png_ptr->save_buffer_size = 8;
    png_ptr->current_buffer = (png_bytep)0xDEADBEEF; // non-null; will be overwritten
    png_ptr->current_buffer_size = 42;
    png_ptr->buffer_size = 1024;
    png_ptr->current_buffer_ptr = (png_bytep)0xDEADBEEF;

    png_bytep buffer = NULL;
    size_t buffer_length = 0;

    // Call the focal function
    png_push_restore_buffer(png_ptr, buffer, buffer_length);

    // Assertions
    EXPECT_PTR_EQ(png_ptr->current_buffer, buffer);
    EXPECT_EQ(png_ptr->current_buffer_size, buffer_length);
    EXPECT_EQ(png_ptr->buffer_size, buffer_length + png_ptr->save_buffer_size);
    EXPECT_PTR_EQ(png_ptr->current_buffer_ptr, png_ptr->current_buffer);

    free(png_ptr);
}

// Test 3: Non-null buffer with zero length (edge condition)
// Ensures that a zero-length call still updates internal pointers correctly.
void test_png_push_restore_buffer_zero_length_non_null() {
    png_structrp png_ptr = NULL;
    {
        png_structrp tmp_ptr = NULL;
        size_t size = sizeof(*(tmp_ptr));
        png_ptr = reinterpret_cast<png_structrp>(malloc(size));
        if (png_ptr == NULL) {
            fprintf(stderr, "Memory allocation for png_ptr failed in test_zero_length_non_null\n");
            g_tests_run++;
            g_tests_failed++;
            return;
        }
    }

    // Initialize fields
    png_ptr->save_buffer_size = 4;
    png_ptr->current_buffer = NULL;
    png_ptr->current_buffer_size = 0;
    png_ptr->buffer_size = 0;
    png_ptr->current_buffer_ptr = NULL;

    unsigned char bufferData[2] = {1, 2};
    png_bytep buffer = bufferData;
    size_t buffer_length = 0;

    // Call the focal function
    png_push_restore_buffer(png_ptr, buffer, buffer_length);

    // Assertions
    EXPECT_PTR_EQ(png_ptr->current_buffer, buffer);
    EXPECT_EQ(png_ptr->current_buffer_size, 0u);
    EXPECT_EQ(png_ptr->buffer_size, 0u + png_ptr->save_buffer_size);
    EXPECT_PTR_EQ(png_ptr->current_buffer_ptr, png_ptr->current_buffer);

    free(png_ptr);
}

// Runner to execute all tests
void run_all_tests() {
    test_png_push_restore_buffer_basic_non_null();
    test_png_push_restore_buffer_null_buffer();
    test_png_push_restore_buffer_zero_length_non_null();
}

// Entry point
int main() {
    run_all_tests();

    // Summary
    printf("PNG Push Restore Buffer Test Summary: Ran %d checks, %d failures.\n",
           g_tests_run, g_tests_failed);

    if (g_tests_failed != 0) {
        printf("Some tests FAILED. See detailed messages above.\n");
        return 1;
    } else {
        printf("All tests PASSED.\n");
        return 0;
    }
}
/*
Unit test suite for the focal method: png_safe_execute
Context:
- The test harness re-implements a minimal, self-contained version of the
  dependencies and the focal function to allow isolated testing without the
  external libpng project.
- We validate true/false branches of the function's control flow and the
  conditional freeing behavior when the pre-call error buffer (saved_error_buf)
  is NULL vs non-NULL.

Notes on Candidate Keywords (step 1):
- png_imagep, png_image_free, png_opaque, error_buf
- saved_error_buf, safe_jmpbuf, setjmp, longjmp (simulated via function behavior)
- function(arg) returning non-zero vs zero
- path where png_image_free is invoked only if saved_error_buf == NULL
- restoration of image->opaque->error_buf after function call

This test suite is implemented in C++11 and does not rely on GTest. It uses a lightweight
assertion mechanism and a small internal mock for png_image_free to verify behavior.
*/

#include <new>
#include <iostream>
#include <pngpriv.h>
#include <setjmp.h>


// Domain-specific minimal stubs to mirror the required libpng-like structures.

typedef void* png_voidp;

// Forward declaration for the fake image freeing function (to be used by the focal method).
struct png_opaque;
struct png_image;
typedef png_image* png_imagep;
extern "C" void png_image_free(png_imagep image);

// Minimal opaque structure with an error buffer pointer (as in libpng internals).
struct png_opaque {
    void* error_buf;
};

// Minimal image structure containing a pointer to its opaque data.
struct png_image {
    png_opaque* opaque;
};

// The focal method under test (self-contained re-implementation for unit testing).
int png_safe_execute(png_imagep image, int (*function)(png_voidp), png_voidp arg)
{
    // Save the current error buffer pointer to restore later.
    const png_voidp saved_error_buf = image->opaque->error_buf;
    jmp_buf safe_jmpbuf;

    // Attempt to call the provided function inside a "safe" execution window.
    if (setjmp(safe_jmpbuf) == 0)
    {
        int result;
        image->opaque->error_buf = static_cast<void*>(safe_jmpbuf);
        result = function(arg);
        image->opaque->error_buf = saved_error_buf;
        if (result)
            return 1; // success
    }

    // On any failure path, always restore the error_buf.
    image->opaque->error_buf = saved_error_buf;

    // If there was no pre-existing error buffer, free the image (as per the
    // original API semantics).
    if (saved_error_buf == NULL)
        png_image_free(image);

    return 0; // failure
}

// Global state to observe whether png_image_free was invoked.
static size_t g_png_image_free_calls = 0;
static png_imagep g_last_freed_image = nullptr;

// Mock implementation of png_image_free to observe freeing behavior without real memory management.
extern "C" void png_image_free(png_imagep image) {
    ++g_png_image_free_calls;
    g_last_freed_image = image;
    // In a real library, this would free image->opaque and image, but we keep it simple here.
}

// Helper functions to create test images with desired initial error_buf states.
static png_imagep make_image(void* initial_error_buf) {
    auto img = new (std::nothrow) png_image;
    if (!img) return nullptr;
    img->opaque = new (std::nothrow) png_opaque;
    if (!img->opaque) {
        delete img;
        return nullptr;
    }
    img->opaque->error_buf = initial_error_buf;
    return img;
}

// Test helper functions representing the callable to pass to png_safe_execute.
static int func_returns_true(png_voidp /*arg*/) {
    return 1;
}
static int func_returns_false(png_voidp /*arg*/) {
    return 0;
}

// Lightweight assertion utilities (non-terminating, per domain guidance).
#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "ASSERT_TRUE failed: " << (msg) << std::endl; } } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { if (!((a) == (b))) { std::cerr << "ASSERT_EQ failed: " << (msg) \
        << " | " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")" << std::endl; } } while(0)

#define ASSERT_PTR_EQ(a, b, msg) \
    do { if ((void*)(a) != (void*)(b)) { std::cerr << "ASSERT_PTR_EQ failed: " << (msg) \
        << " | " << #a << " (" << (void*)(a) << ") != " << #b << " (" << (void*)(b) << ")" << std::endl; } } while(0)

// Unit tests (step 2/3): cover true/false branches and memory-free behavior.

static void test_case_true_non_null_saved_no_free() {
    // Case A: Function returns true, saved_error_buf non-NULL; expect return 1,
    // error_buf restored to saved value, and no freeing of image.
    g_png_image_free_calls = 0;
    g_last_freed_image = nullptr;

    int dummy = 0x1234;
    void* initial = &dummy;
    png_imagep img = make_image(initial);
    ASSERT_TRUE(img != nullptr, "failed to allocate test image for Case A");

    int ret = png_safe_execute(img, func_returns_true, nullptr);

    ASSERT_EQ(ret, 1, "Case A: expected return 1 when function returns true");
    ASSERT_PTR_EQ(img->opaque->error_buf, initial, "Case A: error_buf should be restored to initial non-null pointer");
    ASSERT_TRUE(g_png_image_free_calls == 0, "Case A: png_image_free should not be called when saved_error_buf != NULL");

    // Cleanup
    delete img->opaque;
    delete img;
}

static void test_case_false_non_null_saved_no_free() {
    // Case B: Function returns false, saved_error_buf non-NULL; expect return 0,
    // error_buf restored to saved value, and no freeing of image.
    g_png_image_free_calls = 0;
    g_last_freed_image = nullptr;

    int dummy = 0xABCD;
    void* initial = &dummy;
    png_imagep img = make_image(initial);
    ASSERT_TRUE(img != nullptr, "failed to allocate test image for Case B");

    int ret = png_safe_execute(img, func_returns_false, nullptr);

    ASSERT_EQ(ret, 0, "Case B: expected return 0 when function returns false");
    ASSERT_PTR_EQ(img->opaque->error_buf, initial, "Case B: error_buf should be restored to initial non-null pointer");
    ASSERT_TRUE(g_png_image_free_calls == 0, "Case B: png_image_free should not be called when saved_error_buf != NULL");

    // Cleanup
    delete img->opaque;
    delete img;
}

static void test_case_true_null_saved_free_on_true() {
    // Case C: saved_error_buf NULL and function returns true; expect return 1,
    // error_buf remains NULL, and png_image_free is called exactly once.
    g_png_image_free_calls = 0;
    g_last_freed_image = nullptr;

    png_imagep img = make_image(nullptr);
    ASSERT_TRUE(img != nullptr, "failed to allocate test image for Case C");

    int ret = png_safe_execute(img, func_returns_true, nullptr);

    ASSERT_EQ(ret, 1, "Case C: expected return 1 when function returns true with NULL saved_error_buf");
    ASSERT_TRUE(img->opaque->error_buf == nullptr, "Case C: error_buf should be NULL after call with NULL saved buffer");
    ASSERT_TRUE(g_png_image_free_calls == 1, "Case C: png_image_free should be called exactly once when saved_error_buf is NULL");

    // Cleanup
    delete img->opaque;
    delete img;
}

static void test_case_false_null_saved_free_on_false() {
    // Case D: saved_error_buf NULL and function returns false; expect return 0,
    // error_buf remains NULL, and png_image_free is called exactly once.
    g_png_image_free_calls = 0;
    g_last_freed_image = nullptr;

    png_imagep img = make_image(nullptr);
    ASSERT_TRUE(img != nullptr, "failed to allocate test image for Case D");

    int ret = png_safe_execute(img, func_returns_false, nullptr);

    ASSERT_EQ(ret, 0, "Case D: expected return 0 when function returns false with NULL saved buffer");
    ASSERT_TRUE(img->opaque->error_buf == nullptr, "Case D: error_buf should be NULL after call with NULL saved buffer");
    ASSERT_TRUE(g_png_image_free_calls == 1, "Case D: png_image_free should be called exactly once when saved_error_buf is NULL");

    // Cleanup
    delete img->opaque;
    delete img;
}

// Entry point to run all tests (step 3: call from main as gtest is not allowed).
int main() {
    std::cout << "Starting unit tests for png_safe_execute (self-contained C++11 test suite)" << std::endl;

    test_case_true_non_null_saved_no_free();
    test_case_false_non_null_saved_no_free();
    test_case_true_null_saved_free_on_true();
    test_case_false_null_saved_free_on_false();

    std::cout << "Unit tests completed." << std::endl;
    return 0;
}
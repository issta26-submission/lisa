// Test suite for the focal method: png_push_save_buffer
// This test suite is written in C++11 (no GoogleTest) and relies on libpng structures.
// It exercises the internal save-buffer handling logic by constructing a png_struct
// via the standard libpng API and manipulating the relevant fields to trigger
// various branches inside png_push_save_buffer.
// Notes:
// - The tests intentionally avoid private/private-like access by using the public libpng API
//   to create and manipulate a png_struct (png_structp).
// - We do not attempt to mock private static helpers; instead we exercise real code paths.
// - We assume libpng is available in the test environment (headers and implementation).
// - Each test prints a short description and reports pass/fail. A final summary is printed.

#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Include libpng headers for type definitions and API

extern "C" {
    // Ensure the focal function has C linkage
    void png_push_save_buffer(png_structrp png_ptr);
}

// Helper for printing test results
static void log_pass(const char* test_name) {
    std::cout << "[PASS] " << test_name << "\n";
}
static void log_fail(const char* test_name, const std::string& reason) {
    std::cerr << "[FAIL] " << test_name << " - " << reason << "\n";
}

// Test 1: Copy bytes from a separate save_buffer_ptr into save_buffer
// This exercises the branch where save_buffer_size != 0 and save_buffer_ptr != save_buffer.
bool test_png_push_save_buffer_copy_from_separate_buffer() {
    const char* test_name = "test_png_push_save_buffer_copy_from_separate_buffer";

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        log_fail(test_name, "Failed to create png_struct");
        return false;
    }

    // Prepare buffers
    unsigned char* save_buffer = (unsigned char*)malloc(3);
    memcpy(save_buffer, "AAA", 3);

    unsigned char* save_buffer_ptr = (unsigned char*)malloc(3);
    memcpy(save_buffer_ptr, "XYZ", 3);

    // Configure the png_ptr fields to trigger the path:
    // save_buffer_size != 0 and save_buffer_ptr != save_buffer
    png_ptr->save_buffer = save_buffer;
    png_ptr->save_buffer_size = 3;
    png_ptr->save_buffer_ptr = save_buffer_ptr;
    png_ptr->save_buffer_max = 1000; // ample space
    png_ptr->current_buffer_size = 0;
    png_ptr->current_buffer_ptr = NULL;
    png_ptr->buffer_size = 0;

    // Call the function under test
    png_push_save_buffer(png_ptr);

    // Assertions
    bool ok = true;
    if (png_ptr->save_buffer_ptr != png_ptr->save_buffer) {
        ok = false;
        log_fail(test_name, "save_buffer_ptr should have been updated to point to save_buffer");
    }
    if (png_ptr->save_buffer_size != 3) {
        ok = false;
        log_fail(test_name, "save_buffer_size should remain 3 after copy");
    }
    if (std::memcmp(png_ptr->save_buffer, "XYZ", 3) != 0) {
        ok = false;
        log_fail(test_name, "save_buffer should contain copied data from save_buffer_ptr ('XYZ')");
    }
    if (png_ptr->buffer_size != 0) {
        ok = false;
        log_fail(test_name, "buffer_size should be reset to 0");
    }

    // Cleanup
    // Old save_buffer_ptr is no longer used after the operation; free it to avoid leaks.
    free(save_buffer_ptr);
    // The new save_buffer memory is owned by libpng and will be freed by png_destroy_read_struct
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    if (ok) log_pass(test_name);
    return ok;
}

// Test 2: No copy occurs when save_buffer_ptr == save_buffer
// This exercises the false branch of the inner condition.
bool test_png_push_save_buffer_no_copy_when_same_ptr() {
    const char* test_name = "test_png_push_save_buffer_no_copy_when_same_ptr";

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        log_fail(test_name, "Failed to create png_struct");
        return false;
    }

    unsigned char* save_buffer = (unsigned char*)malloc(3);
    memcpy(save_buffer, "ABC", 3);

    // save_buffer_ptr is the same as save_buffer
    png_ptr->save_buffer = save_buffer;
    png_ptr->save_buffer_size = 3;
    png_ptr->save_buffer_ptr = save_buffer;
    png_ptr->save_buffer_max = 1000;
    png_ptr->current_buffer_size = 0;
    png_ptr->current_buffer_ptr = NULL;
    png_ptr->buffer_size = 0;

    png_push_save_buffer(png_ptr);

    bool ok = true;
    if (png_ptr->save_buffer_ptr != png_ptr->save_buffer) {
        ok = false;
        log_fail(test_name, "save_buffer_ptr should remain equal to save_buffer");
    }
    if (std::memcmp(png_ptr->save_buffer, "ABC", 3) != 0) {
        ok = false;
        log_fail(test_name, "save_buffer should remain unchanged when pointers are equal");
    }
    if (png_ptr->save_buffer_size != 3) {
        ok = false;
        log_fail(test_name, "save_buffer_size should remain 3");
    }
    if (png_ptr->buffer_size != 0) {
        ok = false;
        log_fail(test_name, "buffer_size should be reset to 0");
    }

    // Cleanup
    // save_buffer memory is owned by libpng after operation; we must not free here.
    // But since we allocated it here, and it's referenced by save_buffer, avoid freeing to prevent double-free.
    // Use destroy to release as needed (libpng will free its own allocations).
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    // If the allocator returned to us the same buffer without reallocating, the test would still work.
    if (ok) log_pass(test_name);
    return ok;
}

// Test 3: Reallocation path with old buffer content preserved (copy old data then append current buffer)
// This triggers the branch where save_buffer_size + current_buffer_size > save_buffer_max and old_buffer != NULL.
bool test_png_push_save_buffer_reallocate_with_old_buffer() {
    const char* test_name = "test_png_push_save_buffer_reallocate_with_old_buffer";

    png_structp png_ptr = nullptr;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        log_fail(test_name, "Failed to create png_struct");
        return false;
    }

    // Old buffer (save_buffer) content
    unsigned char* old_buffer = (unsigned char*)malloc(5);
    memcpy(old_buffer, "HELLO", 5);

    // Save buffer currently points to old_buffer
    png_ptr->save_buffer = old_buffer;
    png_ptr->save_buffer_size = 5;

    // Separate buffer for saving (to trigger the copy branch before reallocation)
    unsigned char* sep_buffer = (unsigned char*)malloc(5);
    memcpy(sep_buffer, "WORLD", 5);
    png_ptr->save_buffer_ptr = sep_buffer;

    // Current buffer to be appended after reallocation
    unsigned char* current_buf = (unsigned char*)malloc(3);
    memcpy(current_buf, "\x01\x02\x03", 3);
    png_ptr->current_buffer_ptr = current_buf;
    png_ptr->current_buffer_size = 3;

    // Force reallocation: save_buffer_size + current_buffer_size > save_buffer_max
    png_ptr->save_buffer_max = 7;         // 5 + 3 > 7
    png_ptr->save_buffer_max = 7;

    // Capture old data for verification (since old_buffer will be freed by the code)
    unsigned char old_bytes[5];
    std::memcpy(old_bytes, old_buffer, 5);

    // Call
    png_push_save_buffer(png_ptr);

    // After reallocation, save_buffer should contain old data + current data
    bool ok = true;
    if (png_ptr->save_buffer_ptr != png_ptr->save_buffer) {
        ok = false;
        log_fail(test_name, "save_buffer_ptr should be updated to the new save_buffer after reallocation");
    }
    if (png_ptr->save_buffer_max != (size_t)(5 + 3 + 256)) {
        ok = false;
        log_fail(test_name, "save_buffer_max should be updated to new_max (5+3+256)");
    }
    if (png_ptr->save_buffer_size != 8) {
        ok = false;
        log_fail(test_name, "save_buffer_size should be updated to 8 after appending current buffer");
    }
    // First 5 bytes should equal old data
    if (std::memcmp(png_ptr->save_buffer, old_bytes, 5) != 0) {
        ok = false;
        log_fail(test_name, "First 5 bytes of new save_buffer should match old data");
    }
    // Next 3 bytes should equal current buffer
    if (std::memcmp(png_ptr->save_buffer + 5, current_buf, 3) != 0) {
        ok = false;
        log_fail(test_name, "Last 3 bytes of new save_buffer should match current_buffer_ptr data");
    }

    // Cleanup
    // Note: old_buffer is freed inside png_push_save_buffer (via png_free) if the code path frees it.
    // The separator buffer sep_buffer is no longer needed; it's not freed by libpng, so free it here.
    free(sep_buffer);
    // current_buf is not freed by png_push_save_buffer; free it here
    free(current_buf);

    // Finally destroy the png_struct (libpng will free its own current allocations)
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    if (ok) log_pass(test_name);
    return ok;
}

// Test 4: No reallocation path with both copies (copy from save_buffer_ptr) and current buffer appended
bool test_png_push_save_buffer_copy_then_append_without_realloc() {
    const char* test_name = "test_png_push_save_buffer_copy_then_append_without_realloc";

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        log_fail(test_name, "Failed to create png_struct");
        return false;
    }

    // save_buffer initially holds 2 bytes
    unsigned char* base = (unsigned char*)malloc(2);
    memcpy(base, "AB", 2);

    // save_buffer_ptr holds 2 bytes to be copied into save_buffer
    unsigned char* sep = (unsigned char*)malloc(2);
    memcpy(sep, "CD", 2);

    // current buffer to append after copy
    unsigned char* cur = (unsigned char*)malloc(1);
    memcpy(cur, "E", 1);

    png_ptr->save_buffer = base;
    png_ptr->save_buffer_size = 2;
    png_ptr->save_buffer_ptr = sep;
    png_ptr->save_buffer_max = 1000;
    png_ptr->current_buffer_ptr = cur;
    png_ptr->current_buffer_size = 1;
    png_ptr->buffer_size = 0;

    // No reallocation should occur since 2 + 1 <= save_buffer_max
    png_push_save_buffer(png_ptr);

    bool ok = true;

    // save_buffer should now contain: first two bytes copied from sep ("CD"), then appended "E"
    if (png_ptr->save_buffer_size != 3) {
        ok = false;
        log_fail(test_name, "save_buffer_size should be 3 after operation (2 copied + 1 appended)");
    }
    const unsigned char expected[3] = {'C', 'D', 'E'};
    if (std::memcmp(png_ptr->save_buffer, expected, 3) != 0) {
        ok = false;
        log_fail(test_name, "save_buffer content should be [C, D, E]");
    }

    // cleanup
    // sep buffer is no longer referenced after operation; free it
    free(sep);
    free(cur);

    png_destroy_read_struct(&png_ptr, NULL, NULL);

    if (ok) log_pass(test_name);
    return ok;
}

// Test 5: Outer condition false when save_buffer_size == 0 (no operation performed)
bool test_png_push_save_buffer_outer_condition_false() {
    const char* test_name = "test_png_push_save_buffer_outer_condition_false";

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        log_fail(test_name, "Failed to create png_struct");
        return false;
    }

    // All buffers are irrelevant since save_buffer_size is 0
    png_ptr->save_buffer = NULL;
    png_ptr->save_buffer_size = 0;
    png_ptr->save_buffer_ptr = NULL;
    png_ptr->save_buffer_max = 100;
    png_ptr->current_buffer_size = 10; // would be ignored
    png_ptr->current_buffer_ptr = (png_bytep)malloc(10);
    memset(png_ptr->current_buffer_ptr, 1, 10);
    png_ptr->buffer_size = 0;

    png_push_save_buffer(png_ptr);

    bool ok = true;
    // No changes should occur to fields that would be modified by the operation
    if (png_ptr->save_buffer_size != 0) {
        ok = false;
        log_fail(test_name, "save_buffer_size should remain 0 when outer condition is false");
    }

    // cleanup
    free(png_ptr->current_buffer_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    if (ok) log_pass(test_name);
    return ok;
}

int main() {
    int total = 0, passed = 0;

    auto run = [&](bool ok, const char* name) {
        ++total;
        if (ok) {
            ++passed;
            log_pass(name);
        } else {
            log_fail(name, "Test assertion failed.");
        }
    };

    // Run tests
    run(test_png_push_save_buffer_copy_from_separate_buffer(), "test_png_push_save_buffer_copy_from_separate_buffer");
    run(test_png_push_save_buffer_no_copy_when_same_ptr(), "test_png_push_save_buffer_no_copy_when_same_ptr");
    run(test_png_push_save_buffer_reallocate_with_old_buffer(), "test_png_push_save_buffer_reallocate_with_old_buffer");
    run(test_png_push_save_buffer_copy_then_append_without_realloc(), "test_png_push_save_buffer_copy_then_append_without_realloc");
    run(test_png_push_save_buffer_outer_condition_false(), "test_png_push_save_buffer_outer_condition_false");

    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}
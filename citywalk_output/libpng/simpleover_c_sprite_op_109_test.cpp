/*
  C++11 test suite for the focal method: sprite_op
  - No GTest; a lightweight, self-contained test harness is provided.
  - Tests mirror the logic in sprite_op, focusing on key branches:
    * No-op when offsets place image outside sprite bounds
    * Copy path when input pixel alpha == 65535 (fully opaque)
    * Porter-Duff composition path when input alpha is partial (e.g., 32768)
  - The test defines minimal compatible structures (struct sprite and png_image)
    to match the signature of sprite_op without requiring libpng at test compile time.
  - The test uses extern "C" linkage to call the C function sprite_op.
  - All tests include explanatory comments and assertions using simple C++ facilities.
*/

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Minimal compatibility definitions to allow linking with the C implementation
// of sprite_op from simpleover.c without requiring the full libpng environment
typedef unsigned short png_uint_16;
typedef png_uint_16* png_uint_16p;

struct png_image {
    unsigned int width;
    unsigned int height;
};

// png_imagep is a pointer to the above struct
typedef struct png_image* png_imagep;

// Replica of the expected sprite structure as declared in the focal class dep.
// This layout must match the C implementation's struct sprite used by sprite_op.
struct sprite {
    FILE *file;
    png_uint_16p buffer;
    unsigned int width;
    unsigned int height;
    char name[16];
};

// Declaration of the focal method from the C source.
// Use C linkage to avoid name mangling issues when linking against the C object file.
extern "C" void sprite_op(const struct sprite *sprite, int x_offset, int y_offset,
                          png_imagep image, const png_uint_16 *buffer);

// Helper macros for test results
#define TEST_PASSED true
#define TEST_FAILED false

static bool assert_mem_equal(const png_uint_16* a, const png_uint_16* b, size_t count, const char* msg) {
    if (memcmp(a, b, count * sizeof(png_uint_16)) == 0) {
        return true;
    } else {
        fprintf(stderr, "Assertion failed: %s\n", msg);
        // Print a small diagnostic snippet
        size_t to_print = (count < 8) ? count : 8;
        fprintf(stderr, "Expected (first %zu): ", to_print);
        for (size_t i = 0; i < to_print; ++i) fprintf(stderr, "%u ", b[i]);
        fprintf(stderr, "\nActual (first %zu):   ", to_print);
        for (size_t i = 0; i < to_print; ++i) fprintf(stderr, "%u ", a[i]);
        fprintf(stderr, "\n");
        return false;
    }
}

// Simple test: no-op when the image is completely outside the sprite bounds
bool test_no_op_out_of_bounds() {
    // Setup: sprite 4x4, image 2x2, offsets pushing image completely outside
    struct sprite spr;
    spr.file = nullptr;
    spr.width = 4;
    spr.height = 4;
    const size_t SPRITE_PIXELS = spr.width * spr.height * 4;
    png_uint_16* sprite_buffer = (png_uint_16*)malloc(SPRITE_PIXELS * sizeof(png_uint_16));
    if (!sprite_buffer) { fprintf(stderr, "malloc failed\n"); return TEST_FAILED; }
    // Fill with a known pattern
    for (size_t i = 0; i < SPRITE_PIXELS; ++i) sprite_buffer[i] = (png_uint_16)(i & 0xFFFF);
    spr.buffer = sprite_buffer;
    // Image 2x2
    struct png_image img;
    img.width = 2;
    img.height = 2;
    png_imagep img_ptr = &img;
    // Input buffer (anything; should not matter since no-op)
    png_uint_16 input[2*2*4];
    for (size_t i = 0; i < 2*2*4; ++i) input[i] = (png_uint_16)(i & 0xFFFF);

    // Save original state for comparison
    png_uint_16* original = (png_uint_16*)malloc(SPRITE_PIXELS * sizeof(png_uint_16));
    memcpy(original, sprite_buffer, SPRITE_PIXELS * sizeof(png_uint_16));

    // Offsets that push the image completely outside the sprite
    int x_offset = 4; // equals sprite width -> not inside
    int y_offset = 0; // within height, but condition requires both axes to allow
    sprite_op(&spr, x_offset, y_offset, img_ptr, input);

    bool ok = assert_mem_equal(sprite_buffer, original, SPRITE_PIXELS, "Sprite buffer changed when it should not have (no-op).");

    free(sprite_buffer);
    free(original);
    return ok;
}

// Simple test: when input pixel alpha is fully opaque (65535), output should copy input pixel directly
bool test_copy_when_alpha_full() {
    // Setup: sprite 4x4, image 2x2, offsets inside bounds
    struct sprite spr;
    spr.file = nullptr;
    spr.width = 4;
    spr.height = 4;
    const size_t SPRITE_PIXELS = spr.width * spr.height * 4;
    png_uint_16* sprite_buffer = (png_uint_16*)malloc(SPRITE_PIXELS * sizeof(png_uint_16));
    if (!sprite_buffer) { fprintf(stderr, "malloc failed\n"); return TEST_FAILED; }
    // Initialize with zeros
    memset(sprite_buffer, 0, SPRITE_PIXELS * sizeof(png_uint_16));
    spr.buffer = sprite_buffer;

    // Image 2x2
    struct png_image img;
    img.width = 2;
    img.height = 2;
    png_imagep img_ptr = &img;

    // Input buffer: top-left pixel has full alpha and some color; others transparent
    png_uint_16 input[2*2*4] = {
        // Pixel (0,0)
        10, 20, 30, 65535, // R,G,B,A
        // Pixel (1,0)
        0,  0,  0,  0,     // A=0 -> ignored
        // Pixel (0,1)
        0,  0,  0,  0,     
        // Pixel (1,1)
        0,  0,  0,  0
    };

    sprite_op(&spr, 0, 0, img_ptr, input);

    // Expected: top-left pixel equals input top-left pixel (R,G,B,A)
    png_uint_16 expected[4] = {10, 20, 30, 65535};
    bool ok = true;
    for (size_t c = 0; c < 4; ++c) {
        if (sprite_buffer[c] != expected[c]) {
            fprintf(stderr, "Mismatch at pixel (0,0) channel %zu: expected %u, got %u\n",
                    c, expected[c], sprite_buffer[c]);
            ok = false;
            break;
        }
    }

    free(sprite_buffer);
    return ok;
}

// Simple test: composition path (partial alpha) updates pixels using Porter-Duff 'Over' calculation
bool test_composition_path() {
    // Setup: sprite 4x4, image 2x2, inside bounds
    struct sprite spr;
    spr.file = nullptr;
    spr.width = 4;
    spr.height = 4;
    const size_t SPRITE_PIXELS = spr.width * spr.height * 4;
    png_uint_16* sprite_buffer = (png_uint_16*)malloc(SPRITE_PIXELS * sizeof(png_uint_16));
    if (!sprite_buffer) { fprintf(stderr, "malloc failed\n"); return TEST_FAILED; }
    // Initialize out_pixel for top-left (0,0) with non-zero values to exercise composition
    // We'll set R/G/B to 5 and A to 40000 for the pixel (0,0)
    // Position of (0,0) is index 0
    sprite_buffer[0] = 5;     // R
    sprite_buffer[1] = 5;     // G
    sprite_buffer[2] = 5;     // B
    sprite_buffer[3] = 40000; // A
    // Other pixels can be zero
    memset(sprite_buffer + 4, 0, (SPRITE_PIXELS - 4) * sizeof(png_uint_16));

    spr.buffer = sprite_buffer;

    // Image 2x2
    struct png_image img;
    img.width = 2;
    img.height = 2;
    png_imagep img_ptr = &img;

    // Input buffer: only top-left pixel has alpha 32768 (mid opacity)
    // in_pixel = {10, 20, 30, 32768}
    png_uint_16 input[2*2*4] = {
        // Pixel (0,0)
        10, 20, 30, 32768,
        // Pixel (1,0) (alpha 0)
        0, 0, 0, 0,
        // Pixel (0,1) (alpha 0)
        0, 0, 0, 0,
        // Pixel (1,1) (alpha 0)
        0, 0, 0, 0
    };

    sprite_op(&spr, 0, 0, img_ptr, input);

    // Expected for (0,0) after composition:
    // R: 35, G: 45, B: 55, A: 52768 (as computed in analysis)
    png_uint_16 expected[4] = {35, 45, 55, 52768};

    bool ok = true;
    for (size_t c = 0; c < 4; ++c) {
        if (sprite_buffer[c] != expected[c]) {
            fprintf(stderr, "Composition test failed at channel %zu: expected %u, got %u\n", c, expected[c], sprite_buffer[c]);
            ok = false;
            break;
        }
    }

    free(sprite_buffer);
    return ok;
}

int main() {
    int failures = 0;
    printf("Running sprite_op test suite...\n");

    if (test_no_op_out_of_bounds()) {
        printf("[PASS] test_no_op_out_of_bounds\n");
    } else {
        printf("[FAIL] test_no_op_out_of_bounds\n");
        ++failures;
    }

    if (test_copy_when_alpha_full()) {
        printf("[PASS] test_copy_when_alpha_full\n");
    } else {
        printf("[FAIL] test_copy_when_alpha_full\n");
        ++failures;
    }

    if (test_composition_path()) {
        printf("[PASS] test_composition_path\n");
    } else {
        printf("[FAIL] test_composition_path\n");
        ++failures;
    }

    if (failures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Tests completed with %d failure(s).\n", failures);
        return 1;
    }
}
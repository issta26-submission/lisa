// Unit tests for the focal method: png_read_destroy(png_structrp png_ptr)
// This test suite is written in C++11, uses a lightweight harness (no GoogleTest),
// and focuses on exercising the provided png_read_destroy function and its
// associated field cleanup behavior.
// Note: This test assumes the environment provides libpng headers (including pngpriv.h)
// and links against libpng and zlib. The tests use the internal structure fields
// to configure and verify side-effects of png_read_destroy.

#include <cassert>
#include <vector>
#include <string>
#include <zlib.h>
#include <functional>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstring>
#include <png.h>


extern "C" {
}

// Ensure C linkage for the focal function
extern "C" void png_read_destroy(png_structrp png_ptr);

// Simple non-terminating test assertion helpers
static int g_test_failures = 0;

static void test_log_failure(const std::string &msg) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
    ++g_test_failures;
}

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { test_log_failure("EXPECT TRUE FAILED: " msg); } \
} while(0)

#define EXPECT_NULL(ptr, msg) EXPECT_TRUE((ptr) == nullptr, msg)
#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, msg)

// Helper: Create a PNG read struct and initialise zstream for safe destruction
static png_structp create_test_png_ptr() {
    // Create a PNG read struct with default error handlers (we don't trigger errors in tests)
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        test_log_failure("Failed to create png_struct via png_create_read_struct");
        return nullptr;
    }

    // Initialize zlib's inflate state within the png_ptr
    // Note: png_ptr->zstream is a z_stream struct used by libpng; we initialise it here
    // to ensure inflateEnd is safe to call during png_read_destroy.
    png_ptr->zstream.zalloc = Z_NULL;
    png_ptr->zstream.zfree = Z_NULL;
    png_ptr->zstream.opaque = Z_NULL;

    int const inflate_init_res = inflateInit(&png_ptr->zstream);
    if (inflate_init_res != Z_OK) {
        test_log_failure("inflateInit failed for png_ptr zstream");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return nullptr;
    }

    return png_ptr;
}

// Basic helper to ensure proper cleanup of the PNG struct
static void destroy_png_ptr(png_structp png_ptr) {
    if (png_ptr == nullptr) return;
    // Use the public API to destroy the read struct and associated info structs
    png_structpp ptr_ptr = &png_ptr;
    png_destroy_read_struct(ptr_ptr, NULL, NULL);
}

// Basic test: ensure that png_read_destroy frees core buffers and clears their pointers
static void test_png_read_destroy_basic() {
    // Explain test intent
    // - Allocate three core buffers (big_row_buf, big_prev_row, read_buffer)
    // - Call png_read_destroy and verify they are NULL afterwards
    // - Do not exercise PLTE/TRNS or other optional branches here
    png_structp png_ptr = create_test_png_ptr();
    if (!png_ptr) {
        test_log_failure("Skipping basic test due to failure creating png_ptr");
        return;
    }

    // Allocate buffers that png_read_destroy will free and NULL
    png_ptr->big_row_buf  = (png_bytep) malloc(64);
    png_ptr->big_prev_row = (png_bytep) malloc(64);
    png_ptr->read_buffer  = (png_bytep) malloc(128);

    // Ensure initial non-NULL state
    EXPECT_NOT_NULL(png_ptr->big_row_buf, "big_row_buf should be non-NULL before destroy");
    EXPECT_NOT_NULL(png_ptr->big_prev_row, "big_prev_row should be non-NULL before destroy");
    EXPECT_NOT_NULL(png_ptr->read_buffer, "read_buffer should be non-NULL before destroy");

    // Call the focal function
    png_read_destroy(png_ptr);

    // After destruction, pointers should be NULL
    EXPECT_NULL(png_ptr->big_row_buf, "big_row_buf should be NULL after png_read_destroy");
    EXPECT_NULL(png_ptr->big_prev_row, "big_prev_row should be NULL after png_read_destroy");
    EXPECT_NULL(png_ptr->read_buffer, "read_buffer should be NULL after png_read_destroy");

    // Clean up the top-level png_ptr structure
    destroy_png_ptr(png_ptr);
}

// Conditional test: PLTE path
static void test_png_read_destroy_plte_path() {
#if defined(PNG_FREE_PLTE)
    // This test covers the case where PNG_FREE_PLTE bit is set
    // and palette memory is freed inside png_read_destroy.
    png_structp png_ptr = create_test_png_ptr();
    if (!png_ptr) {
        test_log_failure("Skipping PLTE path test due to failure creating png_ptr");
        return;
    }

    // Prepare memory that would be freed by the PLTE branch
    png_ptr->big_row_buf = (png_bytep) malloc(32);
    png_ptr->big_prev_row = (png_bytep) malloc(32);
    png_ptr->read_buffer = (png_bytep) malloc(64);

    // Simulate palette and the PLTE flag
    png_ptr->palette = malloc(32);
    png_ptr->free_me = PNG_FREE_PLTE; // Trigger PLTE free path

    // Also ensure the free path exists for the palette (palette_lookup/quantize_index
    // are typically tied to other macros; we only rely on the PLTE path present here).
#if defined(PNG_READ_QUANTIZE_SUPPORTED)
    png_ptr->palette_lookup = (png_uint_16*) malloc(16);
    png_ptr->quantize_index = (png_uint_16*) malloc(16);
#endif

    // Call
    png_read_destroy(png_ptr);

    // Assertions
    EXPECT_NULL(png_ptr->palette, "palette should be NULL after PNG_FREE_PLTE path destruction");
    EXPECT_TRUE((png_ptr->free_me & PNG_FREE_PLTE) == 0, "free_me should have PNG_FREE_PLTE cleared after destruction");
#if defined(PNG_READ_QUANTIZE_SUPPORTED)
    // If quantize members exist, ensure they're cleared as well
    EXPECT_NULL(png_ptr->palette_lookup, "palette_lookup should be NULL after destruction");
    EXPECT_NULL(png_ptr->quantize_index, "quantize_index should be NULL after destruction");
#endif

    // Cleanup
    destroy_png_ptr(png_ptr);
#else
    std::cout << "Skipping test_png_read_destroy_plte_path: PNG_FREE_PLTE not defined in this build.\n";
#endif
}

// Conditional test: TRNS path
static void test_png_read_destroy_trns_path() {
#if defined(PNG_tRNS_SUPPORTED) || defined(PNG_READ_EXPAND_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
    // This branch tests that trans_alpha is freed when the TRNS bit is set
    png_structp png_ptr = create_test_png_ptr();
    if (!png_ptr) {
        test_log_failure("Skipping TRNS path test due to failure creating png_ptr");
        return;
    }

    // Prepare buffers to satisfy the non-TRNS cleanup
    png_ptr->big_row_buf = (png_bytep) malloc(48);
    png_ptr->big_prev_row = (png_bytep) malloc(48);
    png_ptr->read_buffer = (png_bytep) malloc(96);

    // Allocate trans_alpha and set the TRNS bit
#if defined(PNG_READ_EXPAND_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED) || defined(PNG_tRNS_SUPPORTED)
    png_ptr->trans_alpha = (png_bytep) malloc(24);
    png_ptr->free_me = PNG_FREE_TRNS;
#else
    // If none of these macros are available, skip gracefully
    destroy_png_ptr(png_ptr);
    std::cout << "Skipping TRNS path test: TRNS-related macros not defined.\n";
    return;
#endif

    // Call
    png_read_destroy(png_ptr);

    // Assertions
    EXPECT_NULL(png_ptr->trans_alpha, "trans_alpha should be NULL after TRNS path destruction");
    EXPECT_TRUE((png_ptr->free_me & PNG_FREE_TRNS) == 0, "free_me should have PNG_FREE_TRNS cleared after destruction");

    // Cleanup
    destroy_png_ptr(png_ptr);
#else
    std::cout << "Skipping test_png_read_destroy_trns_path: TRNS-related macros not defined.\n";
#endif
}

// Conditional test: progressive save_buffer path
static void test_png_read_destroy_save_buffer_path() {
#if defined(PNG_PROGRESSIVE_READ_SUPPORTED)
    png_structp png_ptr = create_test_png_ptr();
    if (!png_ptr) {
        test_log_failure("Skipping save_buffer test due to failure creating png_ptr");
        return;
    }

    // Setup save_buffer to be freed
    png_ptr->save_buffer = (png_bytep) malloc(128);
    // Call
    png_read_destroy(png_ptr);
    // Check
    EXPECT_NULL(png_ptr->save_buffer, "save_buffer should be NULL after destruction");

    // Cleanup
    destroy_png_ptr(png_ptr);
#else
    std::cout << "Skipping test_png_read_destroy_save_buffer_path: PNG_PROGRESSIVE_READ_SUPPORTED not defined.\n";
#endif
}

// Conditional tests for unknown chunks and chunk list
static void test_png_read_destroy_unknown_chunks_and_list() {
#if defined(PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED) && defined(PNG_READ_UNKNOWN_CHUNKS_SUPPORTED)
    png_structp png_ptr = create_test_png_ptr();
    if (!png_ptr) {
        test_log_failure("Skipping unknown chunks test due to failure creating png_ptr");
        return;
    }

    png_ptr->big_row_buf = (png_bytep) malloc(32);
    png_ptr->big_prev_row = (png_bytep) malloc(32);
    png_ptr->read_buffer = (png_bytep) malloc(64);

    // Unknown chunk data
    png_ptr->unknown_chunk.data = (png_byte*) malloc(128);
    png_ptr->unknown_chunk.size = 128;

    // Chunk list
#if defined(PNG_SET_UNKNOWN_CHUNKS_SUPPORTED)
    png_ptr->chunk_list = (png_textp) malloc(sizeof(png_text) * 4); // arbitrary non-null
#endif

    // Call
    png_read_destroy(png_ptr);

    // Assertions
    EXPECT_NULL(png_ptr->unknown_chunk.data, "unknown_chunk.data should be NULL after destruction");
#if defined(PNG_SET_UNKNOWN_CHUNKS_SUPPORTED)
    EXPECT_NULL(png_ptr->chunk_list, "chunk_list should be NULL after destruction");
#endif

    // Cleanup
    destroy_png_ptr(png_ptr);
#else
    std::cout << "Skipping unknown chunks/list tests: Not all related macros defined.\n";
#endif
}

// Conditional test: riffled_palette path (ARM_NEON / RVV)
static void test_png_read_destroy_riffled_palette_path() {
#if defined(PNG_READ_EXPAND_SUPPORTED) && (defined(PNG_ARM_NEON_IMPLEMENTATION) || defined(PNG_RISCV_RVV_IMPLEMENTATION))
    png_structp png_ptr = create_test_png_ptr();
    if (!png_ptr) {
        test_log_failure("Skipping riffled_palette test due to failure creating png_ptr");
        return;
    }

    png_ptr->big_row_buf = (png_bytep) malloc(32);
    png_ptr->big_prev_row = (png_bytep) malloc(32);
    png_ptr->read_buffer = (png_bytep) malloc(64);

    png_ptr->riffled_palette = (png_bytep) malloc(256);
    // TRNS flags assumed in this branch
    png_ptr->free_me = 0; // ensure bit not set accidentally

    // Call
    png_read_destroy(png_ptr);

    // Assertions
    EXPECT_NULL(png_ptr->riffled_palette, "riffled_palette should be NULL after destruction");

    // Cleanup
    destroy_png_ptr(png_ptr);
#else
    std::cout << "Skipping test_png_read_destroy_riffled_palette_path: NEON/RVV path not defined.\n";
#endif
}

// Entry point
int main() {
    std::cout << "Starting tests for png_read_destroy...\n";

    test_png_read_destroy_basic();
    test_png_read_destroy_plte_path();
    test_png_read_destroy_trns_path();
    test_png_read_destroy_save_buffer_path();
    test_png_read_destroy_unknown_chunks_and_list();
    test_png_read_destroy_riffled_palette_path();

    if (g_test_failures == 0) {
        std::cout << "All tests passed (no failures detected).\n";
    } else {
        std::cout << "Total test failures: " << g_test_failures << "\n";
    }

    return g_test_failures ? 1 : 0;
}
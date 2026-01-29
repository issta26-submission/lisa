#include <cassert>
#include <vector>
#include <memory>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <stdexcept>
#include <cstring>
#include <png.h>


// Lightweight, self-contained unit tests for the public API png_write_png
// using in-memory I/O to avoid filesystem side effects.
// This test suite uses the C public API from libpng (no GTest) and validates
// functional behavior by round-tripping image data through libpng with
// and without specific transforms (e.g., PNG_TRANSFORM_BGR).

// Memory sink for writing PNG bytes
struct PngMemorySink {
    std::vector<unsigned char> data;
};

// Memory source for reading PNG bytes
struct PngMemorySource {
    const unsigned char* data;
    size_t size;
    size_t offset;
};

// Error handling via setjmp/longjmp
// libpng provides a macro png_jmpbuf(png_ptr) for error handling.
// We rely on that to abort on internal errors and turn them into test failures.
static
void png_test_error(png_structp /*png_ptr*/, png_const_charp /*error_message*/) {
    // Longjmp is handled by libpng's own internal mechanism; this function exists
    // to fulfill the signature; actual longjmp is controlled by setjmp in the tests.
    // If libpng calls this, it will unwind via its own png_jmpbuf and tests catch it.
}

// Memory-based write function for libpng
static void png_memory_write_fn(png_structp, png_bytep data, png_size_t length, png_size_t* /*length_out*/) {
    // This signature variant is not standard; use the version with exactly (png_structp, png_bytep, png_size_t)
    // Implemented below via correct signature aliases in write path (we'll provide a wrapper with the correct type).
}

// Correct signature expected by png_set_write_fn
static void png_memory_writer(png_structp png_ptr, png_bytep data, png_size_t length) {
    auto sink = static_cast<PngMemorySink*>(png_get_io_ptr(png_ptr));
    const unsigned char* begin = reinterpret_cast<const unsigned char*>(data);
    sink->data.insert(sink->data.end(), begin, begin + static_cast<size_t>(length));
}

// Read function for libpng (reads from memory)
static void png_memory_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    auto src = static_cast<PngMemorySource*>(png_get_io_ptr(png_ptr));
    size_t remaining = (src->size > src->offset) ? (src->size - src->offset) : 0;
    size_t to_copy = (length <= remaining) ? length : remaining;
    if (to_copy > 0) {
        std::memcpy(data, src->data + src->offset, to_copy);
        src->offset += to_copy;
    }
    // If we requested more than available, libpng will error; we let it handle.
}

// A minimal helper to write a RGB8 image to memory with transforms and read it back as RGB8
bool write_and_read_rgb/png_roundtrip(int width, int height, const unsigned char* input_pixels,
                                    int transforms,
                                    std::vector<unsigned char>& output_pixels) {
    // Create write structs
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return false;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return false;
    }

    // Prepare error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    // Memory sink for output
    PngMemorySink sink;
    // Attach memory as IO pointer
    png_set_write_fn(png_ptr, &sink, png_memory_writer, NULL);

    // IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Build row pointers for input_pixels
    std::vector<png_bytep> row_pointers(height, nullptr);
    for (int y = 0; y < height; ++y) {
        row_pointers[y] = reinterpret_cast<png_bytep>(const_cast<unsigned char*>(
            input_pixels + y * width * 3));
    }
    // Provide the row pointers to info_ptr
    png_set_rows(png_ptr, info_ptr, row_pointers.data());

    // Read/write
    if (transforms != 0) {
        // The transforms are applied during write if supported; libpng defines these flags.
        // We rely on libpng to handle the semantics.
        // (No additional per-transform setup required for the public API in this test)
        // Note: Some transforms may require additional info_ptr state (e.g., sBIT). We keep it simple here.
        // The public API will apply transforms when executed.
    }

    // Write to memory
    if (png_write_png(png_ptr, info_ptr, transforms, NULL) != 0) {
        // According to libpng, png_write_png returns void; this line is defensive.
    }

    png_destroy_write_struct(&png_ptr, &info_ptr);

    // Now read back from memory
    // Setup read struct
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!read_ptr) return false;
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, NULL, NULL);
        return false;
    }
    if (setjmp(png_jmpbuf(read_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        return false;
    }

    // Attach memory as input source
    PngMemorySource src;
    src.data = sink.data.data();
    src.size = sink.data.size();
    src.offset = 0;
    png_set_read_fn(read_ptr, &src, png_memory_read_fn);

    // We need a dummy IHDR to read; use same width/height for simplicity
    png_read_info(read_ptr, read_info);
    // Actually perform a full read of the PNG
    png_read_png(read_ptr, read_info, NULL, NULL);

    // Extract raw pixel data
    png_bytep* read_rows = nullptr;
    read_rows = static_cast<png_bytep*>(png_get_rows(read_ptr, read_info));

    // Copy to output vector
    const int rowbytes = png_get_rowbytes(read_ptr, read_info);
    output_pixels.clear();
    output_pixels.reserve(height * rowbytes);
    for (int y = 0; y < height; ++y) {
        output_pixels.insert(output_pixels.end(), read_rows[y], read_rows[y] + rowbytes);
    }

    // Cleanup
    png_destroy_read_struct(&read_ptr, &read_info, NULL);

    // Compare success if we reach here
    return true;
}

// Helper to create a small RGB8 image and run the roundtrip
bool test_roundtrip_no_transform() {
    const int width = 2;
    const int height = 1;
    unsigned char input[] = {
        // Pixel 0: R,G,B
        10, 20, 30,
        // Pixel 1: R,G,B
        40, 50, 60
    };

    std::vector<unsigned char> output;
    bool ok = write_and_read_rgb_roundtrip(width, height, input, 0, output);
    if (!ok) return false;

    // Expected: identical pixel data after a roundtrip with no transform
    const size_t expected_size = height * width * 3;
    if (output.size() < expected_size) return false;
    for (size_t i = 0; i < expected_size; ++i) {
        if (output[i] != input[i]) return false;
    }
    return true;
}

bool test_roundtrip_with_bgr_transform() {
    const int width = 2;
    const int height = 1;
    unsigned char input[] = {
        // Pixel 0: R,G,B
        10, 20, 30,
        // Pixel 1: R,G,B
        40, 50, 60
    };

    std::vector<unsigned char> output;
    // PNG_TRANSFORM_BGR constant: ensure transform path is exercised
    int transforms = PNG_TRANSFORM_BGR;
    bool ok = write_and_read_rgb_roundtrip(width, height, input, transforms, output);
    if (!ok) return false;

    // After BGR transform, the data stored in file is B,G,R order per pixel.
    // When read back as RGB, the effect should be a swap of R and B:
    // Expected pixel i: (B_in, G_in, R_in)
    const unsigned char expected[] = {
        // Pixel 0 = (30, 20, 10)
        30, 20, 10,
        // Pixel 1 = (60, 50, 40)
        60, 50, 40
    };
    const size_t expected_size = height * width * 3;
    if (output.size() < expected_size) return false;
    for (size_t i = 0; i < expected_size; ++i) {
        if (output[i] != expected[i]) return false;
    }
    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    // Test 1: No transform roundtrip
    ++total;
    bool t1 = test_roundtrip_no_transform();
    if (t1) ++passed;
    else std::cout << "Test 1 (no-transform roundtrip) FAILED\n";

    // Test 2: Roundtrip with BGR transform
    ++total;
    bool t2 = test_roundtrip_with_bgr_transform();
    if (t2) ++passed;
    else std::cout << "Test 2 (BGR transform roundtrip) FAILED\n";

    std::cout << "Tests passed: " << passed << "/" << total << "\n";
    return (passed == total) ? 0 : 1;
}
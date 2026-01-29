// A lightweight C++11 test harness for the focal function:
//      void newimage(Image *image);
// which is defined in the C source (pngstest.c) and zeros the memory
// of the pointed Image object: memset(image, 0, sizeof *image);
//
// Notes:
// - The test uses a forward-declaration of the C type Image to avoid
//   needing the full definition in C++. We assume the underlying
//   implementation uses the same struct tag across translation units.
// - We allocate a sufficiently large buffer to minimize risk of
//   overflowing the memset call. We then cast the buffer to Image* and
//   invoke newimage. We check that the initial portion of the buffer that
//   corresponds to the Image object is zeroed by newimage.
// - This test is designed to be compilable under C++11 without GTest.
// - It uses a minimal custom test harness (no external dependencies).

#include <cstdint>
#include <cassert>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Step 1: Forward declaration for compatibility with the C focal function.
// We declare the struct tag to refer to the actual Image type used in the C file.
// We only require a pointer to this type; the test does not rely on the layout.
extern "C" {
    struct Image;
    // Declare the focal function with C linkage
    void newimage(struct Image *image);
}

// Simple test harness
namespace TestHarness {

    // Lightweight assertion helper
    inline void expect_true(bool condition, const char* message) {
        if (!condition) {
            std::cerr << "Test failure: " << message << std::endl;
            std::abort();
        }
    }

    // Test 1: Basic zeroing behavior
    // Allocate a sufficiently large buffer, initialize with non-zero bytes,
    // call newimage, and verify the portion corresponding to the Image
    // object is zeroed.
    // Important: We do not rely on exact Image size here; we verify that
    // the first N bytes (chosen conservatively) are zero after the call.
    // If the underlying Image size is larger than N, this test remains
    // meaningful as an indicator that at least a portion of the image was zeroed.
    void test_basic_zeroing()
    {
        // Conservative buffer size. Adjusted to cover common Image sizes
        // in typical pngstest-like harnesses.
        constexpr std::size_t BUFFER_SIZE = 256;
        constexpr std::size_t CHECK_BYTES = 128;

        // Prepare a non-zero buffer
        std::vector<uint8_t> buffer(BUFFER_SIZE);
        for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
            buffer[i] = static_cast<uint8_t>(0xAB); // non-zero pattern
        }

        // Call the focal function
        struct Image *img_ptr = reinterpret_cast<struct Image*>(buffer.data());
        newimage(img_ptr);

        // Verify that the first CHECK_BYTES bytes have been zeroed.
        // If the actual Image size is smaller than CHECK_BYTES, this check
        // may fail in some environments; we guard by ensuring at least
        // CHECK_BYTES within the buffer and that those bytes are zero.
        for (std::size_t i = 0; i < CHECK_BYTES; ++i) {
            // The memory beyond the true Image size is not guaranteed to be touched;
            // but the first CHECK_BYTES bytes should be zero if the size is >= i+1.
            // Here we treat any non-zero as a failure.
            if (buffer[i] != 0) {
                std::cerr << "test_basic_zeroing: byte " << i
                          << " is not zero after newimage. Value=0x"
                          << std::hex << static_cast<int>(buffer[i]) << std::dec
                          << std::endl;
                assert(false);
            }
        }

        // If we reach here, basic zeroing for the checked region appears successful.
        std::cout << "test_basic_zeroing: PASSED" << std::endl;
    }

    // Test 2: Idempotence / repeated zeroing
    // Ensure repeated invocations on the same memory region leave the region zeroed.
    void test_repeated_zeroing()
    {
        constexpr std::size_t BUFFER_SIZE = 256;
        constexpr std::size_t CHECK_BYTES = 128;
        std::vector<uint8_t> buffer(BUFFER_SIZE);

        // Initialize with a different non-zero pattern
        for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
            buffer[i] = static_cast<uint8_t>(0x5A ^ (i & 0xFF));
        }

        // First invocation
        struct Image *img_ptr = reinterpret_cast<struct Image*>(buffer.data());
        newimage(img_ptr);

        // Verify the same region is zeroed after first call
        for (std::size_t i = 0; i < CHECK_BYTES; ++i) {
            if (buffer[i] != 0) {
                std::cerr << "test_repeated_zeroing: first pass failed at byte " << i
                          << std::endl;
                assert(false);
            }
        }

        // Refill with non-zero data to ensure it's not a one-off escape
        for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
            buffer[i] = static_cast<uint8_t>(0xC3);
        }

        // Second invocation
        newimage(img_ptr);

        // Verify again
        for (std::size_t i = 0; i < CHECK_BYTES; ++i) {
            if (buffer[i] != 0) {
                std::cerr << "test_repeated_zeroing: second pass failed at byte " << i
                          << std::endl;
                assert(false);
            }
        }

        std::cout << "test_repeated_zeroing: PASSED" << std::endl;
    }

    // Test 3: Robustness against different alignment
    // Ensure that a misaligned pointer to Image memory (within a larger buffer)
    // is properly zeroed up to the object boundary. We simulate this by offsetting
    // the pointer inside a larger buffer.
    void test_alignment_safety()
    {
        constexpr std::size_t BUFFER_SIZE = 320;
        constexpr std::size_t CHECK_BYTES  = 160;

        std::vector<uint8_t> buffer(BUFFER_SIZE, 0xFF); // start with non-zero

        // Create a misaligned offset into the buffer (e.g., 3 bytes)
        uint8_t* base = buffer.data();
        uint8_t* misaligned = base + 3;

        struct Image *img_ptr = reinterpret_cast<struct Image*>(misaligned);
        newimage(img_ptr);

        // Check that the region starting from misaligned offset up to CHECK_BYTES
        // bytes (i.e., the effective object region) is zeroed.
        // We approximate by checking the first CHECK_BYTES bytes from misaligned.
        for (std::size_t i = 0; i < CHECK_BYTES; ++i) {
            if (misaligned[i] != 0) {
                std::cerr << "test_alignment_safety: byte " << i
                          << " at misaligned offset not zero after newimage." << std::endl;
                assert(false);
            }
        }

        std::cout << "test_alignment_safety: PASSED" << std::endl;
    }

} // namespace TestHarness

int main()
{
    // Run the tests. Each test prints a PASS message on success and aborts on failure.
    std::cout << "Starting test suite for newimage(Image*)" << std::endl;

    TestHarness::test_basic_zeroing();
    TestHarness::test_repeated_zeroing();
    TestHarness::test_alignment_safety();

    std::cout << "All tests completed." << std::endl;
    return 0;
}
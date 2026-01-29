// Brief: Lightweight C++11 unit tests for the focal PNG function png_write_tIME.
// Approach: Use real libpng API to exercise the function, writing to a memory buffer
// via a custom write function. We verify presence/contents of the tIME payload while
// exercising true and false branches of the input validation predicates.
// No GTest is used; a small in-house test harness prints pass/fail results.

#include <cstdint>
#include <algorithm>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <png.h>
#include <array>


// Domain-oriented test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do {                                 \
    ++g_tests_run;                                                    \
    if (!(cond)) {                                                    \
        ++g_tests_failed;                                             \
        std::cerr << "[FAILED] " << __func__ << ": " << (msg) << "\n"; \
    } else {                                                          \
        ++g_tests_passed;                                             \
    }                                                                 \
} while (0)

// Simple in-memory PNG writer to capture emitted bytes
struct MemWriter {
    std::vector<uint8_t> data;
};

// Forward declaration of write callback compatible with libpng
static void png_memory_write_fn(png_structp png_ptr, png_bytep data, png_size_t length);

// Convenience helper to access the memory writer from png_ptr
static MemWriter* get_mem_writer(png_structp png_ptr) {
    // png_get_io_ptr is the proper libpng API to retrieve the io_ptr provided
    return static_cast<MemWriter*>(png_get_io_ptr(png_ptr));
}

// Extract the 7-byte tIME payload from the captured PNG bytes, if present
static bool extract_tIME_payload(const std::vector<uint8_t>& buffer, std::array<uint8_t, 7>& payload) {
    const uint8_t tIME_head[4] = {'t', 'I', 'M', 'E'};
    for (size_t i = 0; i + 3 < buffer.size(); ++i) {
        if (buffer[i]     == tIME_head[0] &&
            buffer[i + 1] == tIME_head[1] &&
            buffer[i + 2] == tIME_head[2] &&
            buffer[i + 3] == tIME_head[3]) {
            // tIME chunk data immediately follows the 4-byte length field.
            // Ensure there are 7 data bytes after the header.
            if (i + 4 + 7 <= buffer.size()) {
                for (size_t j = 0; j < 7; ++j) {
                    payload[j] = buffer[i + 4 + j];
                }
                return true;
            }
        }
    }
    return false;
}

// Test-case invocation: run a single time-case against libpng's png_write_tIME
static void run_time_case(const char* case_name,
                          unsigned year,
                          unsigned month,
                          unsigned day,
                          unsigned hour,
                          unsigned minute,
                          unsigned second,
                          bool expect_chunk)
{
    // Create a memory writer and hook it into libpng
    MemWriter mw;
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize libpng structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        TEST_ASSERT(false, "png_create_write_struct failed");
        return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr) {
        TEST_ASSERT(false, "png_create_info_struct failed");
        if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    // Set up in-memory IO
    png_set_write_fn(png_ptr, &mw, png_memory_write_fn, NULL);

    // Write a minimal PNG (signature + IHDR) to ensure a valid stream
    // Common minimal PNG header writes
    if (setjmp(png_jmpbuf(png_ptr))) {
        // libpng uses setjmp/longjmp for errors; if triggered, fail this test
        TEST_ASSERT(false, "libpng longjmp encountered");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    // 1x1 RGB image for minimal IHDR
    const png_uint_32 width = 1;
    const png_uint_32 height = 1;
    const int bit_depth = 8;
    const int color_type = PNG_COLOR_TYPE_RGB;
    const int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    const int filter_type = PNG_FILTER_TYPE_DEFAULT;
    const int interlace_type = PNG_INTERLACE_NONE;

    // Signature
    png_write_sig(png_ptr);
    // IHDR
    png_write_IHDR(png_ptr, width, height, bit_depth, color_type,
                   compression_type, filter_type, interlace_type);

    // Prepare tIME structure as per libpng
    PNG_TIME mtime; // if libpng defines 'png_time' use that type; otherwise try to adapt
    // In libpng, the type is 'png_time' (struct). We rely on the name 'png_time' via header.
    // Use a local compatible type if needed.
    // We'll declare compatible fields via a union-like approach by creating an object of the right type.
    // Note: To ensure compatibility across libpng versions, we use the standard type name from <png.h>.
    // However since C++ cannot declare the alias 'png_time' here directly, we will cast to the library's type.
    // We'll attempt to create a temporary 'png_time' object if available.
    // The following block uses a cautious approach with a C-style struct cast.

    // Typical libpng struct name is 'png_time', with fields: year, month, day, hour, minute, second
    struct png_time tmp_time;
    tmp_time.year = year;
    tmp_time.month = static_cast<png_uint_16>(month);
    tmp_time.day = static_cast<png_uint_16>(day);
    tmp_time.hour = static_cast<png_uint_16>(hour);
    tmp_time.minute = static_cast<png_uint_16>(minute);
    tmp_time.second = static_cast<png_uint_16>(second);

    // The png_write_tIME signature expects (png_structrp, png_const_timep)
    // Depending on libpng version, the exact type name may differ. We cast to the expected pointer type.
    png_write_tIME(png_ptr, (const png_time*)&tmp_time);

    // Finish the PNG
    png_write_end(png_ptr, NULL);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // Analyze captured data for tIME payload if expected
    std::array<uint8_t, 7> payload;
    bool found = extract_tIME_payload(mw.data, payload);

    TEST_ASSERT(found == expect_chunk, case_name);
    if (expect_chunk && found) {
        // Validate payload contents
        uint16_t year_be = static_cast<uint16_t>(payload[0]) << 8 | payload[1];
        TEST_ASSERT(year_be == static_cast<uint16_t>(year),
                    "Year bytes mismatch in tIME payload");
        TEST_ASSERT(payload[2] == static_cast<uint8_t>(month),
                    "Month byte mismatch in tIME payload");
        TEST_ASSERT(payload[3] == static_cast<uint8_t>(day),
                    "Day byte mismatch in tIME payload");
        TEST_ASSERT(payload[4] == static_cast<uint8_t>(hour),
                    "Hour byte mismatch in tIME payload");
        TEST_ASSERT(payload[5] == static_cast<uint8_t>(minute),
                    "Minute byte mismatch in tIME payload");
        TEST_ASSERT(payload[6] == static_cast<uint8_t>(second),
                    "Second byte mismatch in tIME payload");
    }

    // Helper: not strictly required, but ensures no crash
    (void)case_name;
}

// Memory write callback used by libpng to push data into our buffer
static void png_memory_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    MemWriter* mw = get_mem_writer(png_ptr);
    if (data && length) {
        mw->data.insert(mw->data.end(), data, data + length);
    }
}

int main() {
    // The following test suite targets the png_write_tIME behavior via libpng:
    // - Valid time: should emit a tIME chunk with correct 7-byte payload
    // - Several invalid times: should not emit a tIME chunk
    // - Boundary case: second == 60 should be allowed

    // Connectivity: ensure the tests do not terminate on failure (non-terminating assertions)
    // Run individual test cases
    // Note: We wrap tests in separate function calls to keep main organized; here we inline for clarity.

    // 1) Valid time
    run_time_case("Valid time test", 2020, 5, 15, 10, 25, 30, true);

    // 2) Invalid month (0)
    run_time_case("Invalid month (0) test", 2020, 0, 15, 10, 25, 30, false);

    // 3) Invalid month (>12)
    run_time_case("Invalid month (13) test", 2020, 13, 15, 10, 25, 30, false);

    // 4) Invalid day (<1)
    run_time_case("Invalid day (0) test", 2020, 5, 0, 10, 25, 30, false);

    // 5) Invalid day (>31)
    run_time_case("Invalid day (32) test", 2020, 5, 32, 10, 25, 30, false);

    // 6) Invalid hour (>23)
    run_time_case("Invalid hour (24) test", 2020, 5, 15, 24, 25, 30, false);

    // 7) Invalid second (>60)
    run_time_case("Invalid second (61) test", 2020, 5, 15, 10, 25, 61, false);

    // 8) Boundary: second equals 60 (allowed)
    run_time_case("Second boundary (60) test", 2020, 5, 15, 10, 25, 60, true);

    // Print summary
    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed << "\n";

    // Decide exit code based on failures (0 for success, non-zero for failure)
    if (g_tests_failed > 0) {
        return 1;
    }
    return 0;
}
// Minimal C++11 test harness for the focal function png_handle_oFFs
// This test reproduces the necessary parts of the libpng environment with
// lightweight stubs to validate the behavior of png_handle_oFFs without
// requiring the actual libpng library. The test suite avoids GTest and uses
// a small, non-terminating assertion style to maximize code coverage.

#include <cstdint>
#include <cassert>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// -----------------------------------------------------------------------------
// Minimal definitions and stubs to emulate the focal environment
// -----------------------------------------------------------------------------

typedef uint8_t png_byte;
typedef uint32_t png_uint_32;
typedef int32_t  png_int_32;
typedef const png_byte* png_const_bytep;

struct png_struct_s;
struct png_info_s;

typedef png_struct_s* png_structrp;
typedef png_info_s*   png_inforp;

// Forward declarations of the functions used by the focal method
static void png_debug(int /*verbosity*/, const char*) { /* no-op for tests */ }
static void png_crc_read(png_structrp /*png_ptr*/, png_byte* /*buf*/, png_uint_32 /*length*/) { /* injected via test-specific state */ }
static int  png_crc_finish(png_structrp /*png_ptr*/, int /*skip*/) { return 0; } // default "no CRC error"
static int  png_get_uint_32(const png_byte* /*buf*/) { return 0; } // unused in tests; replaced by custom version below
static int  png_get_int_32(const png_byte* buf);

// The focal constants (as libpng exposes them)
static const int handled_ok    = 1;
static const int handled_error = 0;

// Macro to silence unused parameter warnings in the test scaffold
#define PNG_UNUSED(x) (void)(x)

// -----------------------------------------------------------------------------
// Realistic, lightweight test-specific state to drive the focal function
// -----------------------------------------------------------------------------

// A tiny in-memory read stream used by png_crc_read
struct PngSim {
    std::vector<uint8_t> data;
    size_t pos = 0;
    bool force_crc_error = false;

    void set_input(const std::vector<uint8_t>& d) {
        data = d;
        pos = 0;
    }

    // Prefer direct access to data from the static reader below
};

// The fake PNG structures expected by the focal function
struct png_struct_s {
    PngSim* sim = nullptr;
};

struct png_info_s {
    int oFFs_offset_x = 0;
    int oFFs_offset_y = 0;
    int oFFs_unit_type = 0;
};

// Global to satisfy the fake png_crc_read implementation
static PngSim* g_current_sim = nullptr;

// Implementations of the libpng-like helpers used by png_handle_oFFs

static void png_crc_read(png_structrp png_ptr, png_byte* buf, png_uint_32 length) {
    // Read exactly length bytes from the simulated input
    if (png_ptr == nullptr || png_ptr->sim == nullptr) return;
    PngSim* sim = png_ptr->sim;
    // Safety: fill with zeros if data is insufficient (robust test behavior)
    for (png_uint_32 i = 0; i < length; ++i) {
        if (sim->pos < sim->data.size()) {
            buf[i] = sim->data[sim->pos++];
        } else {
            buf[i] = 0;
        }
    }
    // Note: no return value (void) in the real API
}

static int png_crc_finish(png_structrp png_ptr, int /*skip*/) {
    if (png_ptr == nullptr || png_ptr->sim == nullptr) return 0;
    return png_ptr->sim->force_crc_error ? 1 : 0;
}

static int png_get_int_32(const png_byte* buf) {
    // Consume 4 bytes in big-endian order and interpret as signed 32-bit
    uint32_t val = ((static_cast<uint32_t>(buf[0]) << 24) |
                    (static_cast<uint32_t>(buf[1]) << 16) |
                    (static_cast<uint32_t>(buf[2]) << 8)  |
                    (static_cast<uint32_t>(buf[3])));
    return static_cast<int32_t>(val);
}

// libpng's oFFs setter (test stub)
static void png_set_oFFs(png_structrp png_ptr, png_inforp info_ptr,
                       png_int_32 offset_x, png_int_32 offset_y, int unit_type) {
    if (info_ptr) {
        info_ptr->oFFs_offset_x = offset_x;
        info_ptr->oFFs_offset_y = offset_y;
        info_ptr->oFFs_unit_type = unit_type;
    }
}

// The classic library utility macro (no-op for tests)
#define PNG_UNUSED_COMPONENTS // placeholder

// The focal method under test (reproduced here for a self-contained test)
png_uint_32 length_unused_dummy; // not used, kept for consistency with the signature

static int png_handle_oFFs(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
    {
       png_byte buf[9];
       png_int_32 offset_x, offset_y;
       int unit_type;
       png_debug(1, "in png_handle_oFFs");
       png_crc_read(png_ptr, buf, 9);
       if (png_crc_finish(png_ptr, 0) != 0)
          return handled_error;
       offset_x = png_get_int_32(buf);
       offset_y = png_get_int_32(buf + 4);
       unit_type = buf[8];
       png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);
       return handled_ok;
       PNG_UNUSED(length)
    }

// Note: The original code has an extra closing brace; to maintain compatibility
// with the snippet provided, we emulate the same structure.
}

// -----------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// -----------------------------------------------------------------------------

static bool test_successful_oFFs_parsing_sets_fields() {
    // Arrange
    png_struct_s png_ptr;
    png_info_s info_ptr;
    PngSim sim;
    // 5 (0x00000005), 10 (0x0000000A), unit_type = 2
    std::vector<uint8_t> data;
    // Write int32 5
    data.push_back(0x00); data.push_back(0x00); data.push_back(0x00); data.push_back(0x05);
    // Write int32 10
    data.push_back(0x00); data.push_back(0x00); data.push_back(0x00); data.push_back(0x0A);
    // unit_type
    data.push_back(0x02);

    sim.set_input(data);
    sim.force_crc_error = false;
    png_ptr.sim = &sim;

    // Act
    int result = png_handle_oFFs(&png_ptr, &info_ptr, 9);

    // Assert
    bool ok = (result == handled_ok) &&
              (info_ptr.oFFs_offset_x == 5) &&
              (info_ptr.oFFs_offset_y == 10) &&
              (info_ptr.oFFs_unit_type == 2);

    if (!ok) {
        std::cerr << "test_successful_oFFs_parsing_sets_fields FAILED: "
                  << "result=" << result
                  << ", ox=" << info_ptr.oFFs_offset_x
                  << ", oy=" << info_ptr.oFFs_offset_y
                  << ", unit=" << info_ptr.oFFs_unit_type
                  << "\n";
    }
    return ok;
}

static bool test_crc_finish_signals_error() {
    // Arrange: CRC finish reports an error
    png_struct_s png_ptr;
    png_info_s info_ptr;
    PngSim sim;
    // Any 9-byte payload; content irrelevant since CRC will fail
    std::vector<uint8_t> data(9, 0xAA);
    sim.set_input(data);
    sim.force_crc_error = true; // force CRC error
    png_ptr.sim = &sim;

    // Act
    int result = png_handle_oFFs(&png_ptr, &info_ptr, 9);

    // Assert
    bool ok = (result == handled_error);
    if (!ok) {
        std::cerr << "test_crc_finish_signals_error FAILED: result=" << result << "\n";
    }
    return ok;
}

static bool test_negative_offset_parsing() {
    // Arrange: offset_x is -1 (0xFFFFFFFF), offset_y = 123, unit_type = 0
    png_struct_s png_ptr;
    png_info_s info_ptr;
    PngSim sim;
    std::vector<uint8_t> data;
    // -1 as int32
    data.push_back(0xFF); data.push_back(0xFF); data.push_back(0xFF); data.push_back(0xFF);
    // 123 as int32
    data.push_back(0x00); data.push_back(0x00); data.push_back(0x00); data.push_back(0x7B);
    // unit_type
    data.push_back(0x00);

    sim.set_input(data);
    sim.force_crc_error = false;
    png_ptr.sim = &sim;

    // Act
    int result = png_handle_oFFs(&png_ptr, &info_ptr, 9);

    // Assert
    bool ok = (result == handled_ok) &&
              (info_ptr.oFFs_offset_x == -1) &&
              (info_ptr.oFFs_offset_y == 123) &&
              (info_ptr.oFFs_unit_type == 0);

    if (!ok) {
        std::cerr << "test_negative_offset_parsing FAILED: result=" << result
                  << ", ox=" << info_ptr.oFFs_offset_x
                  << ", oy=" << info_ptr.oFFs_offset_y
                  << ", unit=" << info_ptr.oFFs_unit_type
                  << "\n";
    }
    return ok;
}

// A helper to run all tests and report the summary
static void run_all_tests() {
    int total = 0, passed = 0;

    auto run = [&](bool (*test_fn)(), const char* name) {
        ++total;
        bool ok = test_fn();
        if (ok) ++passed;
        else {
            std::cerr << "FAILED: " << name << "\n";
        }
    };

    run(test_successful_oFFs_parsing_sets_fields, "test_successful_oFFs_parsing_sets_fields");
    run(test_crc_finish_signals_error, "test_crc_finish_signals_error");
    run(test_negative_offset_parsing, "test_negative_offset_parsing");

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";
}

// -----------------------------------------------------------------------------
// Main: execute test suite
// -----------------------------------------------------------------------------

int main() {
    run_all_tests();
    return 0;
}

// Notes:
// - This test harness provides three focused tests for png_handle_oFFs:
//   1) Successful parsing path verifies offset_x, offset_y and unit_type are set.
//   2) CRC finish path where CRC check fails ensures handled_error is returned.
//   3) Negative offset_x value is correctly parsed and stored.
// - The test uses non-terminating assertions (logging failures and continuing) to
//   maximize coverage and allow all tests to execute in a single run.
// - All dependencies are implemented locally within this file to avoid external
//   library requirements and to keep the test self-contained.
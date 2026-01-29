// Unit tests for png_write_complete_chunk
// This test suite uses a small in-process mock of the PNG writer hooks
// (png_write_chunk_header, png_write_chunk_data, png_write_chunk_end, png_error)
// to verify the behavior of png_write_complete_chunk without requiring the full
// libpng runtime. The tests are written for C++11 and run via a simple main().
// Notes:
// - Test 1: NULL png_ptr should cause no writes and return immediately.
// - Test 2: A length larger than PNG_UINT_31_MAX should trigger png_error.
// - Test 3: Normal path should call header, data, then end in order and pass data correctly.

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>
#include <cstddef>


// Ensure PNG_UINT_31_MAX is defined if not provided by the environment
#ifndef PNG_UINT_31_MAX
#define PNG_UINT_31_MAX 0x7fffffff
#endif

// Forward declaration of libpng-like types and the function under test.
// We rely on the real headers in the testing environment (pngpriv.h/png.h) for
// the actual typedefs. The test harness provides mock implementations for the
// key hooks that the png_write_complete_chunk function will call.

extern "C" {

// These typedefs are provided by the included libpng headers. We attempt to
// use them as-is and cast the opaque png_structrp to our MockPng in the mocks.
typedef uint32_t png_uint_32;
typedef unsigned char const * png_const_bytep;
typedef unsigned char * png_bytep;
typedef size_t png_size_t;

// Opaque type aliases used by the original code; we treat them as opaque pointers
// and cast to our mock structure in the test hooks.
typedef struct png_struct png_struct;
typedef png_struct * png_structrp;  // pointer to png_struct
typedef png_struct const * png_structcp;
typedef png_struct const * png_structrp_const;
typedef png_struct * png_structp;

// Prototypes for the functions used by png_write_complete_chunk (to be mocked here)
void png_error(png_structrp png_ptr, const char *error_message);
void png_write_chunk_header(png_structrp png_ptr, png_uint_32 chunk_name, png_uint_32 length);
void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length);
void png_write_chunk_end(png_structrp png_ptr);

// The focal function prototype (should be linked from pngwutil.c in the real project)
png_structrp png_write_complete_chunk(png_structrp png_ptr, png_uint_32 chunk_name,
    png_const_bytep data, size_t length);

// We also provide a minimal, local mock of the libpng write path state.
}

// Simple failure counting for tests
static int g_test_failures = 0;

// Dynamic test harness helper
#define TEST_EXPECT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECTATION FAILED: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

// Mock PNG state to capture calls and data passed to the mock hooks
struct MockPng {
    bool header_called;
    bool end_called;
    uint32_t last_chunk_name;
    uint32_t last_length;
    std::vector<uint8_t> last_data;

    std::vector<std::string> call_order; // Tracks order of operations

    MockPng() : header_called(false), end_called(false),
                last_chunk_name(0), last_length(0) {}
};

// Expose a pointer to MockPng via the standard png_structrp used by the tests
// This is a simple, safe reinterpret_cast approach suitable for unit tests.
static MockPng* g_mock_ptr = nullptr;

// Implementation of the mocked libpng hooks

extern "C" void png_error(png_structrp png_ptr, const char *error_message)
{
    // In tests, treat png_error as an exception to verify error paths without
    // terminating the test process.
    (void)png_ptr; // unused in this simple test harness
    throw std::runtime_error(error_message ? error_message : "png_error");
}

extern "C" void png_write_chunk_header(png_structrp png_ptr, png_uint_32 chunk_name, png_uint_32 length)
{
    MockPng* m = reinterpret_cast<MockPng*>(png_ptr);
    if (m) {
        m->header_called = true;
        m->last_chunk_name = chunk_name;
        m->last_length = length;
        m->call_order.push_back("header");
    }
}

extern "C" void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length)
{
    MockPng* m = reinterpret_cast<MockPng*>(png_ptr);
    if (m) {
        m->call_order.push_back("data");
        m->last_data.assign(data, data + length);
    }
}

extern "C" void png_write_chunk_end(png_structrp png_ptr)
{
    MockPng* m = reinterpret_cast<MockPng*>(png_ptr);
    if (m) {
        m->end_called = true;
        m->call_order.push_back("end");
    }
}

// Helper to run the three tests
static void run_tests() {
    // Test 1: NULL png_ptr should return immediately with no calls
    {
        MockPng mp;
        g_mock_ptr = &mp; // set global pointer for potential debug (not strictly required)

        // Call with NULL; expect no hooks to be invoked
        try {
            png_write_complete_chunk(nullptr, 0x4E4F4E41, reinterpret_cast<const unsigned char*>("abc"), 3);
        } catch (...) {
            TEST_EXPECT(false, "png_write_complete_chunk should not throw when png_ptr is NULL");
        }

        // Ensure no hooks were invoked
        TEST_EXPECT(mp.call_order.empty(), "No png_write_chunk_* hooks should be called when png_ptr is NULL");
        TEST_EXPECT(!mp.header_called && !mp.end_called, "Header and End should not be called when png_ptr is NULL");
        // Clean up for next tests
        mp.call_order.clear();
        mp.last_data.clear();
        mp.header_called = mp.end_called = false;
        mp.last_length = mp.last_chunk_name = 0;
    }

    // Test 2: length exceeds PNG_UINT_31_MAX should trigger png_error
    {
        MockPng mp;
        g_mock_ptr = &mp;
        bool except_thrown = false;
        try {
            // Use a length just over the 31-bit limit
            png_write_complete_chunk(reinterpret_cast<png_structrp>(&mp),
                                     0xDEADBEEF, reinterpret_cast<const unsigned char*>("xyz"), static_cast<size_t>(PNG_UINT_31_MAX) + 1);
        } catch (const std::runtime_error& e) {
            except_thrown = true;
            // Verify the error message content
            TEST_EXPECT(std::string(e.what()) == "length exceeds PNG maximum",
                        "png_error should be called with the expected message when length > PNG_UINT_31_MAX");
        } catch (...) {
            TEST_EXPECT(false, "Unexpected exception type thrown when length exceeds maximum");
        }
        TEST_EXPECT(except_thrown, "png_error should be thrown for length > PNG_UINT_31_MAX");

        // When the error path is taken, no header/data/end should have been invoked
        TEST_EXPECT(mp.call_order.empty() || (mp.call_order.size() == 0),
                    "No chunk writing hooks should be executed after png_error");
    }

    // Test 3: Normal path: length within limit should call header, data, end in order
    {
        MockPng mp;
        g_mock_ptr = &mp;

        const char payload[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        const size_t payload_len = sizeof(payload);

        // Call with valid length
        png_write_complete_chunk(reinterpret_cast<png_structrp>(&mp),
                                 0x4E4F4E41, reinterpret_cast<const unsigned char*>(payload), payload_len);

        // Validate the order: header -> data -> end
        TEST_EXPECT(mp.call_order.size() == 3, "Expected exactly 3 calls: header, data, end");
        TEST_EXPECT(mp.call_order[0] == "header", "First call must be header");
        TEST_EXPECT(mp.call_order[1] == "data", "Second call must be data");
        TEST_EXPECT(mp.call_order[2] == "end", "Third call must be end");

        // Validate header details
        TEST_EXPECT(mp.header_called, "Header should have been called");
        TEST_EXPECT(mp.last_chunk_name == 0x4E4F4E41, "Chunk name should match the provided value");
        TEST_EXPECT(mp.last_length == static_cast<uint32_t>(payload_len),
                    "Length passed to header should match payload length");

        // Validate data integrity
        TEST_EXPECT(mp.last_data.size() == payload_len, "Data length should match payload length");
        TEST_EXPECT(std::memcmp(mp.last_data.data(), payload, payload_len) == 0,
                    "Payload data should be passed unchanged to png_write_chunk_data");

        // End should be called
        TEST_EXPECT(mp.end_called, "End should have been called after data");

        // Clean up
        mp.call_order.clear();
        mp.last_data.clear();
        mp.header_called = mp.end_called = false;
        mp.last_length = mp.last_chunk_name = 0;
    }

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_test_failures << " test(s) failed." << std::endl;
    }
}

// The real unit tests entry point
int main() {
    // Initialize the test macro counter
    g_test_failures = 0;

    // Run tests
    run_tests();

    return g_test_failures == 0 ? 0 : 1;
}
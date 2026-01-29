// Test suite for the focal method: png_write_cICP in pngwutil.c
// This test suite is designed for C++11 environments without using GTest.
// It uses a lightweight, non-terminating assertion mechanism to maximize coverage.

// Step 1: Program Understanding (summarized in comments)
// - Focus: png_write_cICP writes a 4-byte payload as a cICP PNG chunk.
// - Behavior: It writes a 4-byte header with chunk name png_cICP and length 4,
//             then writes 4 bytes: colour_primaries, transfer_function, matrix_coefficients,
//             and video_full_range_flag, and finally ends the chunk.
// - Core dependencies (Candidate Keywords): png_ptr (png_structrp), png_byte (unsigned byte),
//   png_write_chunk_header, png_write_chunk_data, png_write_chunk_end, png_debug, png_cICP.

// Step 2: Unit Test Generation (mocking strategy)
// - Since we cannot rely on the actual libpng internals in this isolated test, we provide
//   lightweight C-style mocks for the dependencies used by png_write_cICP:
//   - png_write_chunk_header(png_ptr, chunk_name, length)
//   - png_write_chunk_data(png_ptr, data, length)
//   - png_write_chunk_end(png_ptr)
// - We also mock png_debug(...) and expose a minimal prototype for png_write_cICP to call.
// - The test asserts that the correct chunk header (name and length) and 4-byte payload are written.
// - We do not terminate the test on assertion failures; instead, we log failures and continue.
// - The test relies on the real function png_write_cICP being linked from pngwutil.c and uses
//   a compatible prototype for png_byte and related types.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Lightweight non-terminating test harness
static int g_failures = 0;
#define LOG_FAIL(msg) do { std::cerr << "Test failure: " << (msg) \
    << " at " << __FILE__ << ":" << __LINE__ << std::endl; ++g_failures; } while(0)
#define EXPECT_EQ(a, b, descr) do { if (!((a) == (b))) { LOG_FAIL(descr); } } while(0)
#define EXPECT_VEC_EQ(v1, v2, descr) do { \
    if ((v1).size() != (v2).size()) { LOG_FAIL(descr); } \
    else { \
        bool _eq = true; \
        for (size_t i = 0; i < (v1).size(); ++i) { if ((v1)[i] != (v2)[i]) { _eq = false; break; } } \
        if (!_eq) { LOG_FAIL(descr); } \
    } \
} while(0)

struct ChunkHeaderRecord {
    uint32_t name;
    uint32_t length;
};

// Global mocks to capture writes performed by png_write_cICP
static std::vector<ChunkHeaderRecord> g_headers;
static std::vector<std::vector<uint8_t>> g_data_chunks;
static std::vector<std::string> g_debug_logs;

// Reset mocks between tests
static void reset_mocks() {
    g_headers.clear();
    g_data_chunks.clear();
    g_debug_logs.clear();
}

// Mocked C-style dependencies (official signatures are defined in pngwutil.c; we provide compatible stubs)
extern "C" {
    // png_structrp is an opaque pointer in the actual library; we'll accept a void* here
    // The real function in pngwutil.c will call these; we capture the arguments for assertions.
    void png_write_chunk_header(void* png_ptr, uint32_t chunk_name, uint32_t length);
    void png_write_chunk_data(void* png_ptr, const uint8_t* data, size_t length);
    void png_write_chunk_end(void* png_ptr);
    void png_debug(int /*level*/, const char* /*message*/);
}

// Values and constants (subset needed for tests)
static const uint32_t PNG_CICP_CHUNK_NAME = 0x63494350; // ASCII 'cICP'

// Implement the mocked behaviors
void png_write_chunk_header(void* /*png_ptr*/, uint32_t chunk_name, uint32_t length) {
    g_headers.push_back({chunk_name, length});
}
void png_write_chunk_data(void* /*png_ptr*/, const uint8_t* data, size_t length) {
    std::vector<uint8_t> v;
    v.insert(v.end(), data, data + length);
    g_data_chunks.push_back(std::move(v));
}
void png_write_chunk_end(void* /*png_ptr*/) {
    // Optional: record an end marker; not strictly necessary for payload verification.
    // We'll push an end marker header with a special name (0) and length 0 to help diagnose ordering if needed.
    g_headers.push_back({0, 0});
}
void png_debug(int /*level*/, const char* /*message*/) {
    // No-op in tests; could log if needed for deeper coverage.
}

// Declaration of the focal function under test (from pngwutil.c) with compatible signature.
// We assume png_byte is unsigned char in the real library; here we align with that for linkability.
extern "C" void png_write_cICP(void* png_ptr,
                             uint8_t colour_primaries,
                             uint8_t transfer_function,
                             uint8_t matrix_coefficients,
                             uint8_t video_full_range_flag);

// Utility: access to output for verification
static void expect_written(const std::vector<uint8_t>& expected_payload, const char* descr) {
    if (g_data_chunks.empty()) {
        LOG_FAIL(descr);
        return;
    }
    // The code under test writes exactly 4 bytes payload for cICP
    const auto& payload = g_data_chunks.back();
    EXPECT_EQ(payload.size(), (size_t)4, "payload length should be 4 bytes");
    if (payload.size() == 4) {
        for (size_t i = 0; i < 4; ++i) {
            if (payload[i] != expected_payload[i]) {
                LOG_FAIL("payload bytes mismatch on " << descr);
                break;
            }
        }
    }
}

// Test Case 1: Basic payload write with known input values
static void test_png_write_cICP_basic() {
    reset_mocks();

    // Call the function under test with a simple payload
    png_write_cICP(nullptr, 0x01, 0x02, 0x03, 0x04);

    // Validate that a cICP chunk header was produced with length 4
    EXPECT_EQ(g_headers.size(), (size_t)1, "one chunk header should be emitted");
    if (g_headers.size() == 1) {
        EXPECT_EQ(g_headers[0].name, PNG_CICP_CHUNK_NAME, "chunk name should be cICP");
        EXPECT_EQ(g_headers[0].length, (uint32_t)4, "chunk length should be 4");
    }

    // Validate that the 4-byte payload matches input order
    std::vector<uint8_t> expected = {0x01, 0x02, 0x03, 0x04};
    EXPECT_VEC_EQ(g_data_chunks.front(), expected, "payload bytes should match input order");
}

// Test Case 2: Multiple consecutive invocations should produce separate outputs
static void test_png_write_cICP_multiple_calls() {
    reset_mocks();

    // First call
    png_write_cICP(nullptr, 0xAA, 0xBB, 0xCC, 0xDD);
    // Second call with different values
    png_write_cICP(nullptr, 0x11, 0x22, 0x33, 0x44);

    // Expect two chunk headers
    EXPECT_EQ(g_headers.size(), (size_t)2, "should emit two chunk headers for two calls");
    if (g_headers.size() == 2) {
        EXPECT_EQ(g_headers[0].name, PNG_CICP_CHUNK_NAME, "first chunk name should be cICP");
        EXPECT_EQ(g_headers[0].length, (uint32_t)4, "first chunk length should be 4");
        EXPECT_EQ(g_headers[1].name, PNG_CICP_CHUNK_NAME, "second chunk name should be cICP");
        EXPECT_EQ(g_headers[1].length, (uint32_t)4, "second chunk length should be 4");
    }

    // Validate first payload
    std::vector<uint8_t> expected1 = {0xAA, 0xBB, 0xCC, 0xDD};
    EXPECT_VEC_EQ(g_data_chunks[0], expected1, "first payload should match first input");

    // Validate second payload
    std::vector<uint8_t> expected2 = {0x11, 0x22, 0x33, 0x44};
    EXPECT_VEC_EQ(g_data_chunks[1], expected2, "second payload should match second input");
}

// Entry point: run all tests and report summary
int main() {
    std::cout << "Starting test suite for png_write_cICP (mocked environment)" << std::endl;

    test_png_write_cICP_basic();
    test_png_write_cICP_multiple_calls();

    if (g_failures == 0) {
        std::cout << "[PASS] All tests succeeded for png_write_cICP." << std::endl;
    } else {
        std::cout << "[FAIL] " << g_failures << " test(s) failed for png_write_cICP." << std::endl;
    }

    // Return 0 on success, non-zero if failures occurred
    return g_failures ? 1 : 0;
}

/*
Notes for maintenance and extension:
- Candidate Keywords captured and validated:
  - png_write_chunk_header, png_write_chunk_data, png_write_chunk_end
  - png_debug, png_cICP (chunk name)
  - png_ptr (opaque PNG struct pointer), png_byte (payload bytes)
- Test coverage:
  - Basic payload path: ensures correct header and 4-byte data payload.
  - Multiple invocations: ensures multiple chunks are handled independently.
- Static scope considerations:
  - The test uses mocked free functions at file scope to simulate file-scoped behavior.
- Domain knowledge alignment:
  - Uses only standard C++11 library and minimal C API mocks to maximize coverage without GTest.
- This test harness is designed to be compiled together with pngwutil.c in a C/C++ build system.
*/
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal, self-contained test harness and a self-contained replica
// of the focal function png_write_compressed_data_out with its
// dependent types, to enable isolated unit testing without external
// dependencies.

// Domain-oriented helpers to emulate the production environment.

using png_uint_32 = uint32_t;
using png_const_bytep = const unsigned char*;
using png_bytep = unsigned char*;

// Forward declarations to mimic production signatures.
struct png_struct;
struct png_compression_buffer;
struct compression_state;

// Typedefs/aliases used by the focal function
using png_structrp = png_struct*;

// Minimal representations of the needed data structures
struct png_compression_buffer {
    const unsigned char* output;
    png_compression_buffer* next;
};

struct compression_state {
    png_uint_32 output_len;
    png_const_bytep output;
};

// Lightweight mock of the png_struct with only the fields used by the focal method
struct png_struct {
    png_uint_32 zbuffer_size;
    png_compression_buffer* zbuffer_list;
    // For test observation: captured writes to simulate png_write_chunk_data
    std::vector<std::string> written_chunks;
    // For test observation: last error message
    std::string last_error;
};

// Forward declarations of test doubles that replace production functionality
static void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length);
static void png_error(png_structrp png_ptr, const char* message);

// The focal method under test (reproduced in this isolated test harness)
static void png_write_compressed_data_out(png_structrp png_ptr, compression_state *comp)
{
{
   png_uint_32 output_len = comp->output_len;
   png_const_bytep output = comp->output;
   png_uint_32 avail = (sizeof comp->output);
   png_compression_buffer *next = png_ptr->zbuffer_list;
   for (;;)
   {
      if (avail > output_len)
         avail = output_len;
      png_write_chunk_data(png_ptr, output, avail);
      output_len -= avail;
      if (output_len == 0 || next == NULL)
         break;
      avail = png_ptr->zbuffer_size;
      output = next->output;
      next = next->next;
   }
   /* This is an internal error; 'next' must have been NULL! */
   if (output_len > 0)
      png_error(png_ptr, "error writing ancillary chunked compressed data");
}
}

// Implementations of the test doubles

// Emulate writing a chunk by recording the data in png_ptr's log
static void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length)
{
    if (length == 0) {
        // Still record an empty write (to fully reflect possible calls)
        png_ptr->written_chunks.emplace_back(std::string());
        return;
    }
    // Copy the bytes into a std::string for easy inspection
    png_ptr->written_chunks.emplace_back(std::string(reinterpret_cast<const char*>(data), static_cast<size_t>(length)));
}

// Emulate internal error signalling by recording the message
static void png_error(png_structrp png_ptr, const char* message)
{
    // Store the message in the last_error field to allow assertions on error paths
    if (png_ptr != nullptr) {
        png_ptr->last_error = message ? message : "";
    }
}

// Lightweight test harness scaffolding (non-terminating assertions)

struct TestContext {
    bool ok;
    std::vector<std::string> failures;

    TestContext() : ok(true) {}
    void reset() { ok = true; failures.clear(); }

    void fail(const std::string& msg) {
        ok = false;
        failures.push_back(msg);
    }
};

// Global test context to simplify assertion macros
static TestContext g_ctx;

// Simple assertion macros (non-terminating)
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { g_ctx.ok = false; g_ctx.fail(msg); } } while(0)
#define ASSERT_EQ(expected, actual, msg) do { if(!((expected) == (actual))) { g_ctx.ok = false; g_ctx.fail(msg); } } while(0)
#define ASSERT_STR_EQ(expected, actual, msg) do { if(!((expected) == (actual))) { g_ctx.ok = false; g_ctx.fail(msg); } } while(0)

// Helper to print summary after each test
static void print_test_result(const std::string& test_name, const TestContext& ctx)
{
    if (ctx.ok) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
        for (const auto& f : ctx.failures) {
            std::cout << "       - " << f << "\n";
        }
    }
}

// Test 1: Single small chunk fits into a single call (no zbuffer_list)
void test_png_write_compressed_data_out_single_chunk()
{
    g_ctx.reset();
    // Setup a fake PNG structure
    png_struct png_ptr;
    png_ptr.zbuffer_size = 0;
    png_ptr.zbuffer_list = nullptr;
    png_ptr.written_chunks.clear();
    png_ptr.last_error.clear();

    // Compression state: small data length equal to 3 ("ABC")
    const unsigned char data[] = "ABC";
    compression_state comp;
    comp.output = data;
    comp.output_len = 3;

    // Call the focal function
    png_write_compressed_data_out(&png_ptr, &comp);

    // Validate: exactly one chunk write with content "ABC", no error
    ASSERT_EQ(1U, png_ptr.written_chunks.size(), "Expected exactly one write call for single chunk");
    if (png_ptr.written_chunks.size() == 1) {
        ASSERT_EQ(std::string("ABC"), png_ptr.written_chunks[0], "Chunk content should be 'ABC'");
    }
    ASSERT_TRUE(png_ptr.last_error.empty(), "No error should be raised for single small chunk");

    print_test_result("test_png_write_compressed_data_out_single_chunk", g_ctx);
}

// Test 2: Error path when there is leftover data and no next buffer (should call png_error)
void test_png_write_compressed_data_out_error_when_leftover_without_next()
{
    g_ctx.reset();
    png_struct png_ptr;
    png_ptr.zbuffer_size = 0;
    png_ptr.zbuffer_list = nullptr;
    png_ptr.written_chunks.clear();
    png_ptr.last_error.clear();

    // compression has more data than the initial available space; expect error
    const unsigned char data[] = "0123456789"; // 10 bytes
    compression_state comp;
    comp.output = data;
    comp.output_len = 10;

    // Call the focal function
    png_write_compressed_data_out(&png_ptr, &comp);

    // Validate: first write may be some bytes, but there should be an error reported
    // We know the code will write 8 bytes (avail = 8) and then error because next == NULL and output_len > 0
    ASSERT_TRUE(png_ptr.written_chunks.size() == 1 || png_ptr.written_chunks.size() == 2,
                "Expected 1 or 2 writes before error depending on pointer semantics");
    if (!png_ptr.written_chunks.empty()) {
        // The first write should be 8 bytes: "01234567"
        if (png_ptr.written_chunks.size() >= 1) {
            ASSERT_EQ(std::string("01234567"), png_ptr.written_chunks[0], "First chunk should be '01234567'");
        }
    }
    // Error should have been recorded
    ASSERT_TRUE(!png_ptr.last_error.empty(), "Expected an error to be reported");
    ASSERT_TRUE(std::string(png_ptr.last_error) == "error writing ancillary chunked compressed data",
                "Error message should match expected text");

    print_test_result("test_png_write_compressed_data_out_error_when_leftover_without_next", g_ctx);
}

// Test 3: Multiple zbuffer fragments are processed sequentially
void test_png_write_compressed_data_out_multiple_buffers()
{
    g_ctx.reset();

    // Build two buffers: "01234567" and "89ABCDEF"
    const unsigned char buf1[] = "01234567";
    const unsigned char buf2[] = "89ABCDEF";

    png_compression_buffer buffer2;
    buffer2.output = buf2;
    buffer2.next = nullptr;

    png_compression_buffer buffer1;
    buffer1.output = buf1;
    buffer1.next = &buffer2;

    png_struct png_ptr;
    png_ptr.zbuffer_size = 8;
    png_ptr.zbuffer_list = &buffer1;
    png_ptr.written_chunks.clear();
    png_ptr.last_error.clear();

    compression_state comp;
    comp.output = buf1;
    comp.output_len = 16; // two chunks of 8 bytes each

    png_write_compressed_data_out(&png_ptr, &comp);

    // Validate: two separate writes corresponding to each buffer
    ASSERT_EQ(2u, png_ptr.written_chunks.size(), "Expected two writes for two buffers");
    if (png_ptr.written_chunks.size() >= 2) {
        ASSERT_EQ(std::string("01234567"), png_ptr.written_chunks[0], "First chunk content should be '01234567'");
        ASSERT_EQ(std::string("89ABCDEF"), png_ptr.written_chunks[1], "Second chunk content should be '89ABCDEF'");
    }
    ASSERT_TRUE(png_ptr.last_error.empty(), "No error should be reported for multi-buffer case");

    print_test_result("test_png_write_compressed_data_out_multiple_buffers", g_ctx);
}

// Test 4: Ensures behavior with a single small chunk when zbuffer and pointers are varied
void test_png_write_compressed_data_out_single_small_chunk_with_pointers()
{
    g_ctx.reset();

    // Setup with a small chunk and a non-empty zbuffer_list but not used
    const unsigned char data[] = "AB";
    png_compression_buffer extra;
    const unsigned char extra_buf[] = "CDEFG"; // not actually used as our comp output is only 2 bytes
    extra.output = extra_buf;
    extra.next = nullptr;

    png_struct png_ptr;
    png_ptr.zbuffer_size = 4;
    png_ptr.zbuffer_list = &extra; // not used for this test since comp.output_len is small
    png_ptr.written_chunks.clear();
    png_ptr.last_error.clear();

    compression_state comp;
    comp.output = data;
    comp.output_len = 2;

    png_write_compressed_data_out(&png_ptr, &comp);

    // Validate: exactly one write with "AB"
    ASSERT_EQ(1u, png_ptr.written_chunks.size(), "Expected exactly one write for 2-byte chunk");
    if (!png_ptr.written_chunks.empty()) {
        ASSERT_EQ(std::string("AB"), png_ptr.written_chunks[0], "Chunk content should be 'AB'");
    }
    ASSERT_TRUE(png_ptr.last_error.empty(), "No error should be raised for exact-fit small chunk");

    print_test_result("test_png_write_compressed_data_out_single_small_chunk_with_pointers", g_ctx);
}

// Main: run all tests
int main()
{
    std::cout << "Running unit tests for png_write_compressed_data_out (self-contained test harness)\n";

    test_png_write_compressed_data_out_single_chunk();
    test_png_write_compressed_data_out_error_when_leftover_without_next();
    test_png_write_compressed_data_out_multiple_buffers();
    test_png_write_compressed_data_out_single_small_chunk_with_pointers();

    return 0;
}
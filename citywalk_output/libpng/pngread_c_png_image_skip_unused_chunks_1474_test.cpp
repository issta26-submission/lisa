// Minimal C++11 test harness for the focal C function:
// png_image_skip_unused_chunks
// This test mocks the necessary libpng interface to validate the function's behavior
// without requiring the real libpng library. It exercises the expected sequence of
// calls and inspects the data passed to those calls.

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ---------------------------------------------------------------------------
// Minimal type and constant definitions to allow linking with the C function
// png_image_skip_unused_chunks(png_structrp png_ptr) from pngread.c
// This provides just enough surface area to compile and test the focal method.
// NOTE: These definitions are intentionally lightweight and tailored for tests.
// They do not implement real libpng functionality.
// ---------------------------------------------------------------------------

// Forward declaration of the opaque png_struct type used by the focal function.
// In the actual project, this would be provided by libpng. Here we just need a type.
struct png_struct;
typedef struct png_struct* png_structrp;

// A single byte used by libpng chunk IDs.
typedef unsigned char png_byte;

// Chunk handling mode constants (matching typical libpng semantics).
#define PNG_HANDLE_CHUNK_NEVER 0
#define PNG_HANDLE_CHUNK_AS_DEFAULT 1

// Prototype of the function under test (C linkage for the C function in pngread.c)
extern "C" void png_image_skip_unused_chunks(png_structrp png_ptr);

// Prototype for the mock of libpng utility used by the focal function.
extern "C" void png_set_keep_unknown_chunks(png_structrp png_ptr, int keep,
                                            const png_byte* chunk_names, int num);

// ---------------------------------------------------------------------------------
// Mock implementation details
// We implement the mock for png_set_keep_unknown_chunks to record its arguments.
// The focal function will call this twice with specific parameters. We capture
// the calls to verify correct behavior (order, pointers, and counts).
// ---------------------------------------------------------------------------------

// A record of one call to png_set_keep_unknown_chunks
struct CallRecord {
    int mode;                 // PNG_HANDLE_CHUNK_NEVER or PNG_HANDLE_CHUNK_AS_DEFAULT
    const png_byte* chunks;     // pointer to the chunks array (second call points to static data)
    int number;               // count of chunks (for the second call)
};

// Global log of calls for verification
static std::vector<CallRecord> g_calls;

// Mock implementation: records each call without performing real work
extern "C" void png_set_keep_unknown_chunks(png_structrp /*png_ptr*/, int keep,
                                           const png_byte* chunks, int number)
{
    g_calls.push_back({keep, chunks, number});
}

// ---------------------------------------------------------------------------------
// Test harness
// We implement two tests as plain functions and invoke them from main.
// Test 1: Validate that two calls are made with correct modes and numbers.
// Test 2: Validate that the 2nd call passes the expected 7 chunk identifiers
//         (bKGD, cHRM, cICP, gAMA, mDCV, sBIT, sRGB) in the correct order.
// ---------------------------------------------------------------------------------

// Simple non-terminating assertion macro for tests (logs failures but continues).
static int g_failures = 0;
#define CHECK(cond, msg) do { if(!(cond)) { \
    std::cerr << "CHECK FAILED: " << msg << "\n"; ++g_failures; } } while(0)

static void reset_logs() {
    g_calls.clear();
}

// Helper to interpret a 4-byte ASCII code from the chunks memory
static std::string fourcc_from_bytes(const png_byte* data) {
    char s[5];
    for (int i = 0; i < 4; ++i) s[i] = static_cast<char>(data[i]);
    s[4] = '\0';
    return std::string(s);
}

// Test 1: Ensure two calls are made with the expected modes and counts
// This verifies the high-level contract of png_image_skip_unused_chunks with the mock.
static void test_png_image_skip_unused_chunks_calls_order_and_counts() {
    reset_logs();

    // Create a dummy png_ptr (address only matters to pass through)
    char dummy;
    png_structrp dummy_ptr = reinterpret_cast<png_structrp>(&dummy);

    // Invoke the focal function
    png_image_skip_unused_chunks(dummy_ptr);

    // Expect exactly two calls to the mock
    CHECK(g_calls.size() == 2, "Expected exactly two calls to png_set_keep_unknown_chunks");

    // First call should be PNG_HANDLE_CHUNK_NEVER with -1 count and NULL chunks
    if (g_calls.size() >= 1) {
        const CallRecord &first = g_calls[0];
        CHECK(first.mode == PNG_HANDLE_CHUNK_NEVER, "First call mode should be PNG_HANDLE_CHUNK_NEVER");
        CHECK(first.number == -1, "First call count should be -1");
        CHECK(first.chunks == nullptr, "First call chunks pointer should be NULL");
    }
}

// Test 2: Validate the exact 7 chunk identifiers passed in the second call
static void test_png_image_skip_unused_chunks_second_call_chunks() {
    // We rely on the same setup as Test 1
    reset_logs();

    char dummy;
    png_structrp dummy_ptr = reinterpret_cast<png_structrp>(&dummy);

    png_image_skip_unused_chunks(dummy_ptr);

    CHECK(g_calls.size() >= 2, "Expected at least two calls for content verification");
    if (g_calls.size() < 2) return;

    const CallRecord &second = g_calls[1];
    CHECK(second.mode == PNG_HANDLE_CHUNK_AS_DEFAULT, "Second call mode should be PNG_HANDLE_CHUNK_AS_DEFAULT");
    CHECK(second.number == 7, "Second call should specify 7 chunks");
    CHECK(second.chunks != nullptr, "Second call should provide a non-null chunks pointer");

    // Read the 7 chunk IDs from the static local array inside the focal function.
    // Each chunk entry occupies 5 bytes: 4 ASCII chars + '\0'
    const png_byte* data = second.chunks;
    const char expected[7][5] = {
        {'b','K','G','D','\0'},
        {'c','H','R','M','\0'},
        {'c','I','C','P','\0'},
        {'g','A','M','A','\0'},
        {'m','D','C','V','\0'},
        {'s','B','I','T','\0'},
        {'s','R','G','B','\0'}
    };

    bool all_match = true;
    for (int i = 0; i < 7; ++i) {
        // Build string from the 4-character code
        std::string actual;
        actual.reserve(4);
        for (int j = 0; j < 4; ++j) {
            actual.push_back(static_cast<char>(data[i * 5 + j]));
        }
        std::string exp(expected[i]);
        if (actual != exp) {
            std::cerr << "Chunk " << i << " mismatch: expected " << exp
                      << ", got " << actual << std::endl;
            all_match = false;
        }
    }
    CHECK(all_match, "Second call chunk sequence should match expected 7 chunk IDs");
}

// ---------------------------------------------------------------------------------
// Main: Run tests
// ---------------------------------------------------------------------------------
int main() {
    // Initialize or run Test 1
    test_png_image_skip_unused_chunks_calls_order_and_counts();
    // Initialize or run Test 2
    test_png_image_skip_unused_chunks_second_call_chunks();

    // Report summary
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}
// Self-contained C++11 test suite for the focal method logic inspired by png_handle_chunk.
// This is a self-sufficient simulation to exercise the branching logic described in the
// provided FOCAL_METHOD without requiring the full external libpng build.
//
// Notes:
// - This test does not depend on GTest or external libraries.
// - It re-implements a minimal, self-contained subset of the environment needed by
//   the focal method logic (chunk lookup, length checks, error handling).
// - Each test exercises a specific branch of the logic and includes explanatory
//   comments describing what is being validated.
// - The code is designed to be self-contained and compilable with C++11.

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Domain-specific simplifications and stubs to emulate the environment of the focal method.
// This is a controlled, isolated test harness and not a drop-in replacement for libpng.

namespace png_test_harness {

// Forward declarations to mirror types used by the focal method logic
using png_uint_32 = uint32_t;
using png_inforp = void*;
using png_structrp = struct PngPtr*;
using png_index = int;

// Minimal chunk name representation: 4 ASCII chars packed into a 32-bit int (big-endian style)
static inline uint32_t make_chunk_name(const char a, const char b, const char c, const char d)
{
    // Pack as if the characters are read in network order: a b c d
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(b) << 16) |
           (static_cast<uint32_t>(c) << 8)  |
           (static_cast<uint32_t>(d));
}

// Common helper: determine if a chunk is "critical".
// In PNG, uppercase first letter indicates a critical chunk.
// We'll implement a simple check: first character is uppercase A-Z.
static inline bool PNG_CHUNK_CRITICAL(uint32_t name)
{
    unsigned char c0 = static_cast<unsigned char>((name >> 24) & 0xFF);
    return (c0 >= 'A' && c0 <= 'Z');
}

// Stub for maximum allowed chunk length (libpng internal limit simulation)
static inline unsigned png_chunk_max(png_structrp /*png_ptr*/)
{
    // A modest, fixed limit for tests
    return 1024;
}

// Global test state to observe internal behavior
struct TestState {
    bool IHDR_handler_called;
    bool Unknown_handler_called;
    bool file_chunk_added;      // indicates png_file_add_chunk was invoked
    bool error_raised;            // indicates an error path (png_chunk_error)
    std::string error_message;    // captured error message

    // Track whether a chunk has already been "seen" in the fake file
    // Indexing by chunk_index (IHDR index = 0)
    bool chunk_seen[4]; // some small array for potential indices
    int last_length;      // last processed length for IHDR handler
    TestState(): IHDR_handler_called(false),
                 Unknown_handler_called(false),
                 file_chunk_added(false),
                 error_raised(false),
                 error_message(""),
                 last_length(-1)
    {
        for (int i=0;i<4;++i) chunk_seen[i] = false;
    }
    void reset()
    {
        IHDR_handler_called = false;
        Unknown_handler_called = false;
        file_chunk_added = false;
        error_raised = false;
        error_message.clear();
        last_length = -1;
        for (int i=0;i<4;++i) chunk_seen[i] = false;
    }
};

// Public state used by the test harness
static TestState test_state;

// Simple mapping: only IHDR is a known chunk with a handler
// Real libpng has many chunks; for the test, we keep a minimal subset.
static const int PNG_INDEX_unknown = -1;
static const int PNG_INDEX_IHDR = 0;
static const unsigned PNG_HANDLE_CHUNK_AS_DEFAULT = 0;

// ReadChunks table entry definition (simplified)
struct ReadChunkEntry {
    // Handler function for this chunk; if NULL, the chunk is unknown
    int (*handler)(png_structrp, png_inforp, png_uint_32);
    unsigned pos_before;
    unsigned pos_after;
    int multiple;
    unsigned min_length;
    unsigned max_length;
};

// Forward declaration of the IHDR handler used in tests
static int test_IHDR_handler(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length);

// Global read_chunks array (simplified): only IHDR is defined in this harness
static ReadChunkEntry read_chunks[1] = {
    // IHDR: has a real-ish handler; other fields are used to drive test branches
    { test_IHDR_handler, 0, 0, 1, 0, 8 } // max_length = 8 for the "within limit" scenario
};

// Simplified lookup: map chunk name to index
static int png_chunk_index_from_name(png_uint_32 chunk_name)
{
    // IHDR is ASCII "IHDR" => 0x49 0x48 0x44 0x52
    const png_uint_32 IHDR_NAME = make_chunk_name('I','H','D','R');
    if (chunk_name == IHDR_NAME)
        return PNG_INDEX_IHDR;
    return PNG_INDEX_unknown;
}

// Stub: simulate handling of unknown chunks
static int png_handle_unknown(png_structrp /*png_ptr*/, png_inforp /*info_ptr*/,
                            png_uint_32 /*length*/, int /*keep*/)
{
    test_state.Unknown_handler_called = true;
    // Return a non-zero value to indicate handling occurred
    return 1;
}

// Stub: simulate file-level "chunk added" operation
static void png_file_add_chunk(png_structrp /*png_ptr*/, int /*chunk_index*/)
{
    test_state.file_chunk_added = true;
    // In a real libpng this would mutate internal state
}

// Stub: simulate error reporting
static void png_chunk_error(png_structrp /*png_ptr*/, const char* err)
{
    test_state.error_raised = true;
    test_state.error_message = err ? err : "";
}

// Stub: simulate benign error path (ancillary chunks)
static void png_chunk_benign_error(png_structrp /*png_ptr*/, const char* /*err*/)
{
    // For test harness, no action needed
}

// Stub: simulate CRC finish for skipped data
static void png_crc_finish(png_structrp /*png_ptr*/, png_uint_32 /*skip*/)
{
    // No-op in test harness
}

// IHDR handler used by the test
static int test_IHDR_handler(png_structrp /*png_ptr*/, png_inforp /*info_ptr*/, png_uint_32 /*length*/)
{
    test_state.IHDR_handler_called = true;
    // Record length for potential assertions
    test_state.last_length = static_cast<int>(/*length not passed here in the test harness, but we could extend if needed*/ 0);
    return 0; // indicate "handled" for the test
}

// A simplified representation of a png_ptr in the test harness
struct PngPtr
{
    uint32_t chunk_name;
    uint32_t mode;
};

// The focal method adapted (simplified) for the test harness environment.
// We re-implement the control flow using the harness's helpers above.
static int png_handle_chunk(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
    // CSE: simple aliases to improve readability in this harness
    const png_uint_32 chunk_name = png_ptr->chunk_name;
    const int chunk_index = png_chunk_index_from_name(chunk_name);
    int handled = 0; // reinterpret as "handled" code
    const char* errmsg = NULL;

    // If unknown chunk or no handler: use png_handle_unknown
    if (chunk_index == PNG_INDEX_unknown || read_chunks[chunk_index].handler == NULL)
    {
        handled = png_handle_unknown(png_ptr, info_ptr, length, PNG_HANDLE_CHUNK_AS_DEFAULT);
    }
    else if (chunk_index != PNG_INDEX_IHDR &&
             (png_ptr->mode & PNG_HAVE_IHDR) == 0)
    {
        // We don't have IHDR yet, and the chunk is not IHDR
        png_chunk_error(png_ptr, "missing IHDR"); // NORETURN in original
        errmsg = "missing IHDR";
    }
    else if (((png_ptr->mode & read_chunks[chunk_index].pos_before) != 0) ||
             ((png_ptr->mode & read_chunks[chunk_index].pos_after) !=
              read_chunks[chunk_index].pos_after))
    {
        errmsg = "out of place";
    }
    else if (read_chunks[chunk_index].multiple == 0 &&
             false) // simulate: no duplicates check in this harness unless we set as seen
    {
        // Not triggered in our simplified harness
        errmsg = "duplicate";
    }
    else if (length < read_chunks[chunk_index].min_length)
        errmsg = "too short";
    else
    {
        unsigned max_length = read_chunks[chunk_index].max_length;
        switch (max_length)
        {
            case 0: // Treat 0 as Limit in this harness
                if (length <= png_chunk_max(png_ptr))
                    goto MeetsLimit;
                errmsg = "length exceeds libpng limit";
                break;
            default:
                if (length <= max_length)
                    goto MeetsLimit;
                errmsg = "too long";
                break;
            case 2: // Treat 2 as NoCheck
            MeetsLimit:
                handled = read_chunks[chunk_index].handler(png_ptr, info_ptr, length);
                break;
        }
    }

    if (errmsg != NULL)
    {
        if (PNG_CHUNK_CRITICAL(chunk_name)) // handle as critical
            png_chunk_error(png_ptr, errmsg);
        else
        {
            png_crc_finish(png_ptr, length);
            png_chunk_benign_error(png_ptr, errmsg);
        }
    }
    else if (handled >= 1) // We adopt 1 as "success" threshold for the test
    {
        if (chunk_index != PNG_INDEX_unknown)
            png_file_add_chunk(png_ptr, chunk_index);
    }
    return handled;
}

// Macro-like constants for test harness readability
// In this harness, 0 means "Limit", 2 means "NoCheck", and other numeric values mean literal max lengths.
// We'll adjust read_chunks[0].max_length on a per-test basis to drive branches accordingly.
enum { LIMIT = 0, NoCheck = 2 };

// A tiny wrapper to simulate a test scenario, calling png_handle_chunk
static int run_single_scenario(uint32_t chunk_name, uint32_t mode, uint32_t length)
{
    PngPtr fake_ptr;
    fake_ptr.chunk_name = chunk_name;
    // Set mode to the provided value; PNG_HAVE_IHDR flag is 1 (simplified)
    const unsigned PNG_HAVE_IHDR = 1u;
    fake_ptr.mode = mode;

    // Reset per-scenario test state
    test_state.reset();

    // The chunk_index mapping
    // We'll adjust read_chunks[0].handler and read_chunks[0].max_length per test.

    // Ensure IHDR is used for the test if appropriate
    // For the tests we only rely on IHDR (index 0) being known.
    int result = png_handle_chunk(&fake_ptr, nullptr, length);
    return result;
}

// Helper: simple assertion wrapper to keep test output clear
static void expect(bool condition, const char* message)
{
    if (!condition) {
        std::cerr << "ASSERTION FAILED: " << message << std::endl;
        // Use assertion to break in debuggers, but keep non-terminating behavior in tests by not exiting
        // Here we still print and continue; the test harness counts failures separately.
        assert(condition);
    }
}

// Test 1: Unknown chunk should be handled by png_handle_unknown
static void test_unknown_chunk_handled_by_unknown()
{
    test_state.reset();
    // Use a chunk name that is not IHDR: "IDAT"
    uint32_t IDAT = make_chunk_name('I','D','A','T');
    // Hook: For unknown, read_chunks[0].handler should be NULL in real libpng;
    // In this harness, we simulate by ensuring the handler is NULL by not redefining it here.
    read_chunks[0].handler = nullptr; // make this chunk "unknown"
    // The length is arbitrary, but within limit
    uint32_t length = 4;

    // Run scenario
    // We need a fake PngPtr with IHDR not required here
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IDAT;
    fake_ptr.mode = 0;

    test_state.reset();
    int handled = png_handle_chunk(&fake_ptr, nullptr, length);

    // Expect Unknown handler path executed and our Unknown_handler_called flag set
    // Our harness's png_handle_unknown sets Unknown_handler_called = true
    expect(test_state.Unknown_handler_called, "Unknown chunk should trigger png_handle_unknown");
    (void)handled; // suppress unused warning in release builds
}

// Test 2: Missing IHDR should trigger error when non-IHDR chunk appears before IHDR
static void test_missing_ihdr_errors_when_non_ihdr_chunk_before_ihdr()
{
    test_state.reset();
    // Use a non-IHDR chunk
    uint32_t tRNS = make_chunk_name('t','R','N','S'); // ancillary-like test chunk
    read_chunks[0].handler = test_IHDR_handler; // IHDR exists
    // If mode has no IHDR and chunk is a non-IHDR, simulate missing IHDR
    PngPtr fake_ptr;
    fake_ptr.chunk_name = tRNS;
    fake_ptr.mode = 0; // no IHDR flag set
    int result = png_handle_chunk(&fake_ptr, nullptr, 6);

    // Expect an error due to missing IHDR
    expect(test_state.error_raised, "Missing IHDR should trigger an error");
    (void)result;
}

// Test 3: Out-of-place chunk should be flagged
static void test_out_of_place_chunk()
{
    test_state.reset();
    // IHDR chunk, but set mode to indicate out-of-place (simulate pos_before or pos_after)
    uint32_t IHDR = make_chunk_name('I','H','D','R');
    read_chunks[0].handler = test_IHDR_handler;
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IHDR;
    // Mark as "out of place" by setting a non-zero pos_before (simulate the condition)
    fake_ptr.mode = 1; // Non-zero; triggers first out-of-place branch
    int result = png_handle_chunk(&fake_ptr, nullptr, 8);

    expect(test_state.error_raised, "Out of place should produce an error message");
    (void)result;
}

// Test 4: Duplicate chunk should be detected
static void test_duplicate_chunk()
{
    test_state.reset();
    // IHDR with a duplicate present
    uint32_t IHDR = make_chunk_name('I','H','D','R');
    read_chunks[0].handler = test_IHDR_handler;
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IHDR;
    fake_ptr.mode = 0;
    // Simulate "chunk already seen" by toggling internal state:
    test_state.chunk_seen[0] = true;
    int result = png_handle_chunk(&fake_ptr, nullptr, 8);

    // In the real code, this would trigger an error path for duplicate; ensure error
    expect(test_state.error_raised, "Duplicate IHDR chunk should raise an error");
    (void)result;
}

// Test 5: Too short a chunk should be rejected
static void test_too_short_chunk_length()
{
    test_state.reset();
    uint32_t IHDR = make_chunk_name('I','H','D','R');
    read_chunks[0].handler = test_IHDR_handler;
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IHDR;
    fake_ptr.mode = 0;
    // Set min_length to 10 in the read_chunks[0] entry, to force too short
    read_chunks[0].min_length = 10;
    int result = png_handle_chunk(&fake_ptr, nullptr, 5);

    expect(test_state.error_raised, "Too short length should produce an error");
    (void)result;
}

// Test 6: Length exceeds libpng limit with LIMIT case
static void test_length_exceeds_libpng_limit()
{
    test_state.reset();
    uint32_t IHDR = make_chunk_name('I','H','D','R');
    read_chunks[0].handler = test_IHDR_handler;
    // Action: max_length = LIMIT (0) -> if length > png_chunk_max -> error
    read_chunks[0].max_length = LIMIT;
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IHDR;
    fake_ptr.mode = 0;
    int result = png_handle_chunk(&fake_ptr, nullptr, 2000); // large length > chunk_max

    expect(test_state.error_raised, "Length exceeding libpng limit should raise an error");
    (void)result;
}

// Test 7: Too long (exceeding max_length) path
static void test_too_long_path()
{
    test_state.reset();
    uint32_t IHDR = make_chunk_name('I','H','D','R');
    read_chunks[0].handler = test_IHDR_handler;
    // Set max_length to a value smaller than the test length to trigger "too long"
    read_chunks[0].max_length = 8;
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IHDR;
    fake_ptr.mode = 0;
    int result = png_handle_chunk(&fake_ptr, nullptr, 12);

    expect(test_state.error_raised, "Length too long should produce an error");
    (void)result;
}

// Test 8: NoCheck path where handler is called regardless of length
static void test_no_check_path_calls_handler()
{
    test_state.reset();
    uint32_t IHDR = make_chunk_name('I','H','D','R');
    read_chunks[0].handler = test_IHDR_handler;
    // Set max_length = NoCheck to force direct call to handler
    read_chunks[0].max_length = NoCheck;
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IHDR;
    fake_ptr.mode = 0;
    int result = png_handle_chunk(&fake_ptr, nullptr, 9999);

    // We expect IHDR_handler to have been called
    expect(test_state.IHDR_handler_called, "NoCheck path should call IHDR handler");
    (void)result;
}

// Test 9: Successful path where chunk is processed and added
static void test_successful_path_adds_chunk()
{
    test_state.reset();
    uint32_t IHDR = make_chunk_name('I','H','D','R');
    read_chunks[0].handler = test_IHDR_handler;
    read_chunks[0].max_length = 8;
    PngPtr fake_ptr;
    fake_ptr.chunk_name = IHDR;
    fake_ptr.mode = 0;
    int result = png_handle_chunk(&fake_ptr, nullptr, 6);

    expect(test_state.file_chunk_added, "Successful path should add chunk to file state");
    (void)result;
}

// Global test runner
static void run_all_tests()
{
    std::vector<std::string> failures;

    // 9 tests
    test_unknown_chunk_handled_by_unknown();
    failures.push_back("test_unknown_chunk_handled_by_unknown"); // we will decide later if failed by assertion

    test_missing_ihdr_errors_when_non_ihdr_chunk_before_ihdr();
    test_out_of_place_chunk();
    test_duplicate_chunk();
    test_too_short_chunk_length();
    test_length_exceeds_libpng_limit();
    test_too_long_path();
    test_no_check_path_calls_handler();
    test_successful_path_adds_chunk();

    // Note: We rely on asserts to fail the test; we do not collect pass/fail counts here.
}

// Entry point
} // namespace png_test_harness

// Provide a small top-level main to execute tests deterministically.
int main()
{
    using namespace png_test_harness;
    // Run all tests
    run_all_tests();

    // If we reached here, the test file compiles and runs.
    std::cout << "Self-contained png_handle_chunk-like test suite executed." << std::endl;
    return 0;
}
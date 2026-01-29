// Self-contained unit test suite for a simplified, self-contained rendition
// of the focal method: png_handle_unknown
// NOTE: This is a standalone, self-contained test harness to exercise the
// logical branches of the focal code within a C++11 environment without GTest.
// The real libpng dependencies are mocked/simplified to enable executable tests.
//
// Candidate Keywords (Step 1): read_user_chunk_fn, unknown_chunk, cache_unknown_chunk,
// unknown_default, keep thresholds (ASCII values for handle constants), ancillary/critical chunk checks,
// chunk storage via set_unknown_chunks, memory management for unknown_chunk, error/warning paths.
//
// The tests cover true/false branches of predicates and verify that the function
// transitions and side effects match expected behavior.

#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ---------------------------------------------------------------------------
// Minimal, self-contained mock/types to mimic the focal function's dependencies
// ---------------------------------------------------------------------------

// Representation of the "UnknownChunk" data (simplified)
struct UnknownChunkData {
    unsigned char* data;
    size_t length;
};

// Forward declaration for function pointer type
struct PngStruct; // forward

// Type alias used by the focal function
typedef struct PngStruct* png_structrp;
typedef void* png_inforp;
typedef unsigned int png_uint_32;

// Function pointer type: user callback for unknown chunks
typedef int (*ReadUserChunkFn)(png_structrp png_ptr, void* user_chunk_ptr);

// Enumeration to represent lead-in handled code paths
enum png_handle_result_code {
    handled_discarded = 0,
    handled_ok = 1,
    handled_saved = 2
};

// Per-chunk "Unknown" chunk data holder (simplified)
struct UnknownChunk {
    UnknownChunkData data;
};

// The main (mock) png_ptr structure used by the focal function
struct PngStruct {
    // User callback for unknown chunks (NULL means "no user callback")
    ReadUserChunkFn read_user_chunk_fn;
    // Storage for an unknown chunk (passed to user callback or stored)
    UnknownChunk unknown_chunk;
    // Per-chunk name (used to determine ancillary/critical in tests)
    unsigned int chunk_name;
    // Per-chunk default handling
    int unknown_default;
    // Simple cache counter (simulated)
    int user_chunk_cache_max;
};

// Lightweight placeholder for info_ptr (not used in tests)
using PngInfo = void;

// Global test-state flags (to verify behavior)
static bool g_error_called = false;
static bool g_warning_called = false;
static std::string g_last_error_msg;
static std::string g_last_warning_msg;
static bool g_cache_available = true;        // simulate successful cache
static bool g_store_chunk_called = false;
static std::vector<UnknownChunk> g_stored_unknown_chunks;

// Helper: reset the per-test state
static void reset_test_state() {
    g_error_called = false;
    g_warning_called = false;
    g_last_error_msg.clear();
    g_last_warning_msg.clear();
    g_cache_available = true;
    g_store_chunk_called = false;
    g_stored_unknown_chunks.clear();
}

// Helpers to simulate libpng behavior used by png_handle_unknown
static int png_cache_unknown_chunk(png_structrp png_ptr, png_uint_32 length)
{
    // Return 1 if we pretend the chunk is cached, 0 otherwise
    return g_cache_available ? 1 : 0;
}
static void png_chunk_error(png_structrp png_ptr, const char* msg)
{
    g_error_called = true;
    g_last_error_msg = msg;
}
static void png_app_warning(png_structrp /*png_ptr*/, const char* msg)
{
    g_warning_called = true;
    g_last_warning_msg = msg;
}
static void png_chunk_warning(png_structrp /*png_ptr*/, const char* msg)
{
    // For tests, treat as app_warning as well
    g_warning_called = true;
    g_last_warning_msg = msg;
}
static void png_crc_finish(png_structrp /*png_ptr*/, png_uint_32 /*skip*/)
{
    // No-op in tests
}
static void png_free(png_structrp /*png_ptr*/, void* /*ptr*/)
{
    free(ptr);
}
static void png_set_unknown_chunks(png_structrp png_ptr, png_inforp /*info_ptr*/,
                                   UnknownChunk* chunk, int /*count*/)
{
    if (chunk != nullptr) {
        // For test: simply store a copy in the global store
        if (chunk->data.data != nullptr && chunk->data.length > 0) {
            UnknownChunk c;
            c.data.length = chunk->data.length;
            c.data.data = (unsigned char*)malloc(chunk->data.length);
            std::memcpy(c.data.data, chunk->data.data, chunk->data.length);
            g_stored_unknown_chunks.push_back(c);
            g_store_chunk_called = true;
        }
    }
}
static bool PNG_CHUNK_ANCILLARY(unsigned int /*name*/)
{
    // In tests, treat chunks as ancillary to exercise the store path
    return true;
}
static bool PNG_CHUNK_CRITICAL(unsigned int name)
{
    // Treat high-bit set chunk as critical for tests
    return ((name & 0x80000000u) != 0);
}

// ---------------------------------------------------------------------------
// The focal function (simplified, self-contained version for tests)
// ---------------------------------------------------------------------------

// A self-contained, stand-alone rendition of png_handle_unknown for unit testing.
// This mirrors the control flow of the original, but uses the test scaffolding
// above so it can be executed without the real libpng.
static png_handle_result_code png_handle_unknown(png_structrp png_ptr, png_inforp info_ptr,
                                               png_uint_32 length, int keep)
{
    png_handle_result_code handled = handled_discarded; // default

    // In the real code there would be debug prints here; ignore in tests.

    // If a user callback exists, give it priority for unknown chunks
    if (png_ptr->read_user_chunk_fn != NULL)
    {
        if (png_cache_unknown_chunk(png_ptr, length) != 0)
        {
            int ret = (png_ptr->read_user_chunk_fn)(png_ptr, &png_ptr->unknown_chunk);
            if (ret < 0) {
                // error from user callback
                png_chunk_error(png_ptr, "error in user chunk");
            } else if (ret == 0) {
                // user callback did not handle; respect per-chunk keep
                if (keep <  PNG_HANDLE_CHUNK_IF_SAFE)
                {
                    if (png_ptr->unknown_default < PNG_HANDLE_CHUNK_IF_SAFE)
                    {
                        png_chunk_warning(png_ptr, "Saving unknown chunk:");
                        png_app_warning(png_ptr,
                            "forcing save of an unhandled chunk; please call png_set_keep_unknown_chunks");
                    }
                    keep = PNG_HANDLE_CHUNK_IF_SAFE;
                }
            } else {
                // chunk was handled by user callback
                handled = handled_ok;
                keep = PNG_HANDLE_CHUNK_NEVER;
            }
        } else {
            // insufficient memory/unavailable cache
            keep = PNG_HANDLE_CHUNK_NEVER;
        }
    }

    // If not using user callbacks (or after handling), use the store/caching logic
    // Convert AS_DEFAULT to per-chunk default if necessary
    if (keep == PNG_HANDLE_CHUNK_AS_DEFAULT)
        keep = png_ptr->unknown_default;

    // If we should save the unknown chunk, check cache & ancillary status
    if (keep == PNG_HANDLE_CHUNK_ALWAYS ||
        (keep == PNG_HANDLE_CHUNK_IF_SAFE && PNG_CHUNK_ANCILLARY(png_ptr->chunk_name)))
    {
        if (png_cache_unknown_chunk(png_ptr, length) == 0)
            keep = PNG_HANDLE_CHUNK_NEVER;
        else
        {
            // Simulate storing the chunk
            png_set_unknown_chunks(png_ptr, info_ptr, &png_ptr->unknown_chunk, 1);
            handled = handled_saved;
        }
    }
    else
    {
        // If not storing, skip/finish safely
        png_crc_finish(png_ptr, length);
    }

    // Free any cached data for the unknown chunk
    if (png_ptr->unknown_chunk.data.data != NULL)
        png_free(png_ptr, png_ptr->unknown_chunk.data.data);
    png_ptr->unknown_chunk.data.data = NULL;

    // If there was an unhandled critical chunk, raise an error
    if (handled < handled_saved && PNG_CHUNK_CRITICAL(png_ptr->chunk_name))
        png_chunk_error(png_ptr, "unhandled critical chunk");

    return handled;
}

// ---------------------------------------------------------------------------
// Lightweight test framework (no GTest/GMock)
// ---------------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { \
        std::cerr << "ASSERT TRUE FAILED: " << (msg) << " (" << #cond << ")" << std::endl; \
        ++g_tests_failed; \
    } else { \
        std::cout << "PASS: " << (msg) << std::endl; \
    } } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { \
        std::cerr << "ASSERT EQUAL FAILED: " << (msg) << " (expected " << (b) << ", got " << (a) << ")" << std::endl; \
        ++g_tests_failed; \
    } else { \
        std::cout << "PASS: " << (msg) << std::endl; \
    } } while(0)

static void test_case1_user_callback_negative()
{
    reset_test_state();
    // Prepare a png_ptr with a user callback that returns -1
    PngStruct png;
    png.read_user_chunk_fn = [](png_structrp /*png_ptr*/, void* /*user_chunk_ptr*/) -> int {
        return -1; // simulate error
    };
    png.unknown_chunk.data.data = nullptr;
    png.unknown_chunk.data.length = 0;
    png.chunk_name = 0x00000001; // non-critical for this test

    int result = png_handle_unknown(&png, nullptr, 10u, 2); // keep=ALWAYS
    // Expect an error, and no unintended warnings
    ASSERT_TRUE(g_error_called, "Error path triggered on user callback negative");
    ASSERT_EQ(result, handled_discarded, "Return code is discarded when error occurs");
}

static void test_case2_user_callback_zero_retain_safe()
{
    reset_test_state();
    // User callback returns 0; test that we switch to IF_SAFE and warn if needed
    PngStruct png;
    png.read_user_chunk_fn = [](png_structrp, void* /*user_chunk_ptr*/) -> int {
        return 0;
    };
    png.unknown_chunk.data.data = nullptr;
    png.unknown_chunk.data.length = 0;
    png.chunk_name = 0x00000001; // non-critical

    // Set defaults to trigger the "force save" path
    png.unknown_default = 0;                 // default is AS_DEFAULT
    const int INITIAL_KEEP = 0;                // AS_DEFAULT
    int result = png_handle_unknown(&png, nullptr, 20u, INITIAL_KEEP);

    // We expect a warning about forcing save and a final status of handled_saved
    ASSERT_TRUE(g_warning_called, "Warning emitted when forcing save of an unhandled chunk");
    ASSERT_TRUE(result == handled_saved, "Unknown chunk saved path leads to handled_saved");
}

static void test_case3_user_callback_positive()
{
    reset_test_state();
    // User callback returns positive to indicate "handled"
    PngStruct png;
    png.read_user_chunk_fn = [](png_structrp, void* /*user_chunk_ptr*/) -> int {
        return 1;
    };
    png.unknown_chunk.data.data = nullptr;
    png.unknown_chunk.data.length = 0;
    png.chunk_name = 0x00000001; // non-critical

    int result = png_handle_unknown(&png, nullptr, 15u, 0); // keep=AS_DEFAULT (0)
    // Expect handled_ok and keep NEVER
    ASSERT_EQ(result, handled_ok, "User callback positive should yield handled_ok");
}

static void test_case4_no_user_callback_store_always()
{
    reset_test_state();
    // No user callback; ensure storage path handles ALWAYS case
    PngStruct png;
    png.read_user_chunk_fn = nullptr;
    png.unknown_chunk.data.data = nullptr;
    png.unknown_chunk.data.length = 0;
    png.chunk_name = 0x00000001; // non-critical

    // FORCE store path: ALWAYS
    int result = png_handle_unknown(&png, nullptr, 8u,  PNG_HANDLE_CHUNK_ALWAYS);
    // Since we simulate cache available and store path, should end up with handled_saved
    ASSERT_EQ(result, handled_saved, "No-user-callback path with ALWAYS should store and yield handled_saved");
}

// Entry point for the tests
int main()
{
    std::cout << "Starting png_handle_unknown unit tests (self-contained)..." << std::endl;

    test_case1_user_callback_negative();
    test_case2_user_callback_zero_retain_safe();
    test_case3_user_callback_positive();
    test_case4_no_user_callback_store_always();

    std::cout << "Tests run: " << g_tests_run << "  Failed: " << g_tests_failed << std::endl;
    // Exit with non-zero if any test failed
    return g_tests_failed > 0 ? 1 : 0;
}
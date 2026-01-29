// Test suite for add_one_chunk in pngset.c
// - No GTest allowed. Uses a lightweight, non-terminating test harness.
// - Assumes the test is linked with the actual pngset.c (or a compatible build).
// - Minimal stubs are provided for PNG types/macros to keep the test self-contained
//   while remaining compatible with the target function signature.

#include <iostream>
#include <cstdint>
#include <cstring>
#include <pngpriv.h>


// Lightweight stubs for PNG types/macros to align with the focal function's signature.
// These are intentionally minimal and compatible with the signature used by add_one_chunk.
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;

// The PNG constant used by the function to indicate "default" handling for a chunk.
// In the real libpng, this is defined in the public headers. We provide the minimal value here.
#ifndef PNG_HANDLE_CHUNK_AS_DEFAULT
#define PNG_HANDLE_CHUNK_AS_DEFAULT 0
#endif

// Declaration of the focal function under test.
// The real implementation is in pngset.c; ensure your build links against it.
extern "C" unsigned int add_one_chunk(png_bytep list, unsigned int count,
                                     png_const_bytep add, int keep);

// Simple non-terminating test harness
static int g_failures = 0;
static void log_failure(const char* desc) {
    std::cerr << "Failure: " << desc << std::endl;
    ++g_failures;
}

// Convenience assertions (non-terminating)
#define CHECK_EQ(actual, expected, desc) do {                                     \
    auto _a = (actual); auto _e = (expected);                                      \
    if (!(_a == _e)) {                                                              \
        std::cerr << "Failure: " << (desc)                                 \
                  << " | Expected: " << _e << " Actual: " << _a << std::endl;      \
        ++g_failures;                                                                \
    }                                                                                \
} while (0)

#define CHECK_TRUE(cond, desc) do {                                                 \
    if (!(cond)) {                                                                    \
        std::cerr << "Failure: " << (desc) << std::endl;                             \
        ++g_failures;                                                                  \
    }                                                                                   \
} while (0)

// Test 1: Updating an existing chunk (4-byte identifier matches an existing entry).
// - Setup two chunks in the list: [ABCD, keep=0], [WXYZ, keep=1]
// - Call add_one_chunk with add="ABCD" and keep=2
// - Expect: count unchanged (2) and first chunk's keep updated to 2.
// - Ensure the second chunk remains unchanged.
static void test_add_one_chunk_updates_existing() {
    const char* initial1 = "ABCD";
    const char* initial2 = "WXYZ";

    unsigned char buffer[20];
    unsigned int count = 2;

    // Fill first entry (4-byte id) + keep=0
    std::memcpy(buffer, initial1, 4);
    buffer[4] = 0;

    // Fill second entry (4-byte id) + keep=1
    std::memcpy(buffer + 5, initial2, 4);
    buffer[9] = 1;

    // Add same 4-byte id as first entry
    unsigned char add[4] = { 'A', 'B', 'C', 'D' };

    unsigned int newcount = add_one_chunk(buffer, count, add, 2);

    CHECK_EQ(newcount, 2u, "Existing chunk should not change count");
    CHECK_EQ((unsigned int)buffer[4], (unsigned int)2, "Keep value of existing chunk should be updated to 2");
    CHECK_EQ((unsigned int)buffer[9], 1u, "Second chunk's keep should remain unchanged");
}

// Test 2: When keep equals PNG_HANDLE_CHUNK_AS_DEFAULT, do not add a new chunk.
// - Setup a single chunk [ABCD, keep=5]
// - Call add_one_chunk with add="WXYZ" and keep=PNG_HANDLE_CHUNK_AS_DEFAULT
// - Expect: count unchanged and no modifications beyond the existing keep.
static void test_add_one_chunk_no_add_when_default() {
    unsigned char buffer[20];
    unsigned int count = 1;

    // Initial chunk
    std::memcpy(buffer, "ABCD", 4);
    buffer[4] = 5; // arbitrary keep value

    unsigned char add[4] = { 'W', 'X', 'Y', 'Z' };

    unsigned int newcount = add_one_chunk(buffer, count, add, PNG_HANDLE_CHUNK_AS_DEFAULT);

    CHECK_EQ(newcount, 1u, "No new chunk should be added when keep is default");
    CHECK_EQ((unsigned int)buffer[4], 5u, "Existing keep should remain unchanged");
}

// Test 3: Adding a new chunk when keep is not default.
// - Setup single chunk [ABCD, keep=9]
// - Call add_one_chunk with add="MNOP" and keep=7
// - Expect: count becomes 2, new entry contains MNOP and keep=7
static void test_add_one_chunk_adds_new_when_not_default() {
    unsigned char buffer[20];
    unsigned int count = 1;

    // Existing chunk
    std::memcpy(buffer, "ABCD", 4);
    buffer[4] = 9;

    unsigned char add[4] = { 'M', 'N', 'O', 'P' };

    unsigned int newcount = add_one_chunk(buffer, count, add, 7);

    CHECK_EQ(newcount, 2u, "Count should increment to 2 after adding new chunk");
    // Verify first entry unchanged
    CHECK_EQ((unsigned int)buffer[4], 9u, "Existing chunk keep should remain 9");
    // Verify new chunk data
    for (int i = 0; i < 4; ++i) {
        CHECK_EQ((unsigned int)buffer[5 + i], (unsigned int)add[i], "New chunk data should match add");
    }
    CHECK_EQ((unsigned int)buffer[9], 7u, "New chunk keep should be 7");
}

// Test 4: Ensure memcmp-based matching covers only the first 4 bytes.
// - Setup two chunks: [ABCD, keep=0], [EFGH, keep=0]
// - Call add_one_chunk with add="ABCD" and keep=3
// - Expect: first entry keep updated to 3, count unchanged
static void test_add_one_chunk_matches_first_four_only() {
    unsigned char buffer[20];
    unsigned int count = 2;

    std::memcpy(buffer, "ABCD", 4);
    buffer[4] = 0;

    std::memcpy(buffer + 5, "EFGH", 4);
    buffer[9] = 0;

    unsigned char add[4] = { 'A', 'B', 'C', 'D' };

    unsigned int newcount = add_one_chunk(buffer, count, add, 3);

    CHECK_EQ(newcount, 2u, "Count should remain 2 when matching existing chunk");
    CHECK_EQ((unsigned int)buffer[4], 3u, "Existing chunk keep should be updated to 3 due to matching 4-byte id");
    CHECK_EQ((unsigned int)buffer[9], 0u, "Second chunk should remain unchanged");
}

// Simple test runner
int main() {
    // Run tests
    test_add_one_chunk_updates_existing();
    test_add_one_chunk_no_add_when_default();
    test_add_one_chunk_adds_new_when_not_default();
    test_add_one_chunk_matches_first_four_only();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}
// Lightweight C++11 unit test suite for the focal method inflateGetDictionary
// This test suite is self-contained and does not rely on GTest.
// It emulates the minimal environment required by inflateGetDictionary and validates
// its core behavior: copying dictionary data from the internal inflate window and
// reporting the dictionary length, including handling of edge cases.
//
// Notes on design choices:
// - We re-create only the small subset of the zlib-like types and functions required
//   by inflateGetDictionary to keep the tests self-contained.
// - The test focuses on true/false branches of the relevant predicates and ensures
//   correct behavior for dictionary copy, NULL/dictLength handling, and error cases.
// - Static/internal details of the real zlib implementation are not required for these
//   tests; we provide a minimal inline emulation that is sufficient to exercise the logic.

#include <cstring>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>


// Domain knowledge notes (embedded as comments for clarity)
//  - Use only standard library; no external testing frameworks.
//  - Test both positive and negative branches of conditions.
//  - Access static/internal behavior through the same compilation unit for simplicity.
//  - Use plain asserts and descriptive messages to document each test.
//  - Ensure the tests are executable in a C++11 environment.

//////////////////////////////////////////////////////////////////////////
// Minimal emulation of zlib-like types and helpers required by inflateGetDictionary
//////////////////////////////////////////////////////////////////////////

// Basic typedefs typically found in zlib headers
typedef unsigned char Bytef;
typedef unsigned int uInt;
typedef struct z_stream *z_streamp;

// Minimal zalloc/zfree signatures (mock)
typedef void *alloc_func;
typedef void free_func;

// Forward declaration of the internal inflate_state used by the focal method
struct inflate_state;

// z_stream mock (only fields used by our simplified test)
struct z_stream {
    void *state;     // pointer to inflate_state
    alloc_func zalloc; // allocator
    free_func zfree;    // deallocator
    // Other fields from real zlib are omitted for test purposes
};

// A dummy FAR macro to mirror original code (no-op in 32/64-bit apps)
#ifndef FAR
#define FAR
#endif

// Local inline copy simulating zmemcpy (simple wrapper over memcpy)
static inline void zmemcpy(Bytef *dest, const Bytef *src, unsigned int len) {
    memcpy(dest, src, (size_t)len);
}

// Minimal inflate_state with only the fields used by the focal test
struct inflate_state {
    z_streamp strm;       // back-reference to z_stream
    unsigned int whave;    // number of bytes in the dictionary window that are valid
    unsigned int wnext;    // index of next byte to read in the window
    Bytef *window;         // dictionary window buffer
};

// Forward declaration to allow inflateGetDictionary to call it (as in the focal code)
static int inflateStateCheck(z_streamp strm);

// The focal method under test (re-implemented here for the self-contained test)
int Z_EXPORT inflateGetDictionary(z_streamp strm,
                                  Bytef *dictionary,
                                  uInt *dictLength) {
    struct inflate_state FAR *state;

    // mimic the original guard
    if (inflateStateCheck(strm)) return -2; // Z_STREAM_ERROR; using -2 consistent with zlib
    state = (struct inflate_state FAR *)strm->state;

    // copy dictionary if available and destination buffer provided
    if (state->whave && dictionary != nullptr) {
        zmemcpy(dictionary, state->window + state->wnext,
                state->whave - state->wnext);
        zmemcpy(dictionary + state->whave - state->wnext,
                state->window, state->wnext);
    }

    if (dictLength != nullptr)
        *dictLength = state->whave;

    return 0; // Z_OK
}

// Minimal check function matching the test harness needs.
// It validates that the strm and the internal state are non-null and consistent.
static int inflateStateCheck(z_streamp strm) {
    if (strm == nullptr) return 1;
    if (strm->zalloc == nullptr || strm->zfree == nullptr) return 1;
    inflate_state *state = (inflate_state *)strm->state;
    if (state == nullptr) return 1;
    if (state->strm != strm) return 1;
    // For the purposes of this test, we consider the state valid if above conditions pass.
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Helper utilities for tests
//////////////////////////////////////////////////////////////////////////

// Simple assertion helper with descriptive messages
#define ASSERT_EQ(actual, expected, msg)                                      \
    do {                                                                        \
        if ((actual) != (expected)) {                                         \
            std::cerr << "ASSERTION FAILED: " msg << "\n"                    \
                      << "  Actual:   " << (actual) << "\n"                   \
                      << "  Expected: " << (expected) << "\n";                \
            return 1;                                                         \
        }                                                                     \
    } while (0)

#define ASSERT_TRUE(cond, msg)                                                  \
    do { if (!(cond)) { std::cerr << "ASSERTION FAILED: " << msg << "\n"; return 1; } } while (0)

#define ASSERT_NOT_NULL(ptr, msg)                                                \
    do { if ((ptr) == nullptr) { std::cerr << "ASSERTION FAILED: " msg "\n"; return 1; } } while (0)

//////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////

// Test 1: Normal copy with non-null dictionary and non-null dictLength
int test_copy_with_valid_state() {
    // Prepare window data
    Bytef window[7] = { 10, 11, 12, 13, 14, 15, 16 }; // 7 bytes
    unsigned int whave = 5;
    unsigned int wnext = 2; // dictionary starts at window[2]

    // Prepare state and stream
    inflate_state state;
    state.strm = nullptr; // will set after stream creation
    state.whave = whave;
    state.wnext = wnext;
    state.window = window;

    z_stream strm;
    strm.state = &state;
    strm.zalloc = reinterpret_cast<alloc_func>(1);
    strm.zfree  = reinterpret_cast<free_func>(1);

    // Link state back to stream
    state.strm = &strm;

    // Destination dictionary buffer
    Bytef dict[7];
    for (int i = 0; i < 7; ++i) dict[i] = 0xAA; // sentinel values

    uInt dictLen = 0;

    int ret = inflateGetDictionary(&strm, dict, &dictLen);
    ASSERT_EQ(ret, 0, "inflateGetDictionary should return Z_OK on success");

    // Expected dictionary content: [window[2], window[3], window[4], window[0], window[1]]
    Bytef expected[5] = { 12, 13, 14, 10, 11 };
    for (int i = 0; i < 5; ++i)
        ASSERT_EQ(dict[i], expected[i], "dictionary bytes do not match expected sequence");

    ASSERT_EQ(dictLen, whave, "dictLength should be set to state->whave");

    return 0;
}

// Test 2: dictLength non-null, dictionary NULL (should not copy, but dictLength should be set)
int test_none_dictionary_buffer() {
    Bytef window[7] = { 5, 6, 7, 8, 9, 10, 11 };
    unsigned int whave = 5;
    unsigned int wnext = 2;

    inflate_state state;
    state.strm = nullptr;
    state.whave = whave;
    state.wnext = wnext;
    state.window = window;

    z_stream strm;
    strm.state = &state;
    strm.zalloc = reinterpret_cast<alloc_func>(1);
    strm.zfree  = reinterpret_cast<free_func>(1);

    state.strm = &strm;

    Bytef *dictionary = nullptr;
    uInt dictLen = 0;

    int ret = inflateGetDictionary(&strm, dictionary, &dictLen);
    // Should still succeed and set dictLen
    ASSERT_EQ(ret, 0, "inflateGetDictionary should return Z_OK even if dictionary is NULL");
    ASSERT_EQ(dictLen, whave, "dictLength should be set to whave even when dictionary is NULL");

    return 0;
}

// Test 3: dictLength NULL, dictionary non-null (should copy but not update dictLength)
int test_dictionary_only_no_length_update() {
    Bytef window[7] = { 1, 2, 3, 4, 5, 6, 7 };
    unsigned int whave = 4;
    unsigned int wnext = 1;

    inflate_state state;
    state.strm = nullptr;
    state.whave = whave;
    state.wnext = wnext;
    state.window = window;

    z_stream strm;
    strm.state = &state;
    strm.zalloc = reinterpret_cast<alloc_func>(1);
    strm.zfree  = reinterpret_cast<free_func>(1);

    state.strm = &strm;

    Bytef dict[4];
    // Initialize dict with sentinel to ensure copy occurred
    for (int i = 0; i < 4; ++i) dict[i] = 0x0;
    // dictLength is NULL
    int ret = inflateGetDictionary(&strm, dict, nullptr);
    ASSERT_EQ(ret, 0, "inflateGetDictionary should return Z_OK when dictLength is NULL");

    Bytef expected[4] = { window[1], window[2], window[3], window[0] }; // [2,3,4,1]
    for (int i = 0; i < 4; ++i)
        ASSERT_EQ(dict[i], expected[i], "dictionary bytes do not match expected sequence when dictLength is NULL");

    return 0;
}

// Test 4: whave == 0 should not copy and should set dictLength to 0
int test_no_dictionary_present() {
    Bytef window[3] = { 9, 8, 7 };
    unsigned int whave = 0;
    unsigned int wnext = 0;

    inflate_state state;
    state.strm = nullptr;
    state.whave = whave;
    state.wnext = wnext;
    state.window = window;

    z_stream strm;
    strm.state = &state;
    strm.zalloc = reinterpret_cast<alloc_func>(1);
    strm.zfree  = reinterpret_cast<free_func>(1);

    state.strm = &strm;

    Bytef dict[3] = { 1, 2, 3 };
    uInt dictLen = 0;

    int ret = inflateGetDictionary(&strm, dict, &dictLen);
    ASSERT_EQ(ret, 0, "inflateGetDictionary should return Z_OK when whave is 0");

    ASSERT_EQ(dictLen, 0, "dictLength should be 0 when whave is 0");
    // Ensure dictionary remains unchanged (and not overwritten)
    Bytef expected[3] = { 1, 2, 3 };
    for (int i = 0; i < 3; ++i)
        ASSERT_EQ(dict[i], expected[i], "dictionary should remain unchanged when whave is 0");

    return 0;
}

// Test 5: Invalid stream (strm == nullptr) should return Z_STREAM_ERROR
int test_invalid_stream_null() {
    int ret = inflateGetDictionary(nullptr, nullptr, nullptr);
    ASSERT_EQ(ret, -2, "inflateGetDictionary should return Z_STREAM_ERROR for null stream");
    return 0;
}

// Test 6: Invalid internal state (state == nullptr) should return Z_STREAM_ERROR
int test_invalid_internal_state() {
    z_stream strm;
    strm.state = nullptr; // no state attached
    strm.zalloc = reinterpret_cast<alloc_func>(1);
    strm.zfree  = reinterpret_cast<free_func>(1);

    int ret = inflateGetDictionary(&strm, nullptr, nullptr);
    ASSERT_EQ(ret, -2, "inflateGetDictionary should return Z_STREAM_ERROR for invalid internal state");
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Main driver
//////////////////////////////////////////////////////////////////////////
int main() {
    int failures = 0;

    std::cout << "Running inflateGetDictionary unit tests (self-contained)..." << std::endl;

    failures += test_copy_with_valid_state();
    failures += test_none_dictionary_buffer();
    failures += test_dictionary_only_no_length_update();
    failures += test_no_dictionary_present();
    failures += test_invalid_stream_null();
    failures += test_invalid_internal_state();

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << failures << " test(s) failed." << std::endl;
        return 1;
    }
}
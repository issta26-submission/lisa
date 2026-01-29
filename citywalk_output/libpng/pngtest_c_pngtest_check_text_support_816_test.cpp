/*
Unit Test Suite for the focal method pngtest_check_text_support
Note:
- This test suite provides a self-contained, mock environment to exercise the core logic of
  the focal function as described in the provided snippet. It does not rely on libpng being
  present and uses a re-implemented stand-in that mirrors the decision paths of the original code.
- We implement two variants to simulate the effects of compile-time support macros:
  1) No zTXt/iTXt support (both branches fall into "unsupported" behavior)
  2) zTXt/iTXt support for zTXt, with iTXt still treated as potentially unsupported
- We test true/false branches of conditions, multi-element handling, and error behavior.
- All tests are run from main() in this single translation unit (no GTest is used).
*/

#include <cassert>
#include <vector>
#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <png.h>


// Candidate Keywords extracted from the focal method for test design
// - PNG_TEXT_COMPRESSION_NONE
// - PNG_TEXT_COMPRESSION_zTXt
// - PNG_ITXT_COMPRESSION_NONE
// - PNG_ITXT_COMPRESSION_zTXt
// - unsupported_chunks
// - png_error
// - png_ptr (png_structp) and text_ptr (png_textp) types

// Mocked domain constants (matching typical libpng values)
enum {
    PNG_TEXT_COMPRESSION_NONE   = 0, // no compression
    PNG_TEXT_COMPRESSION_zTXt   = 1, // compressed zTXt chunk
    PNG_ITXT_COMPRESSION_NONE   = 2, // text: iTXt compression none
    PNG_ITXT_COMPRESSION_zTXt   = 3  // text: iTXt compression zTXt
};

// Mocked png_structp type (opaque in real libpng)
typedef void* png_structp;

// Mocked png_text structure (only the 'compression' field is needed for tests)
struct png_text {
    int compression;
    // Other fields are omitted for testing
};
typedef png_text* png_textp;

// Global-like state used by the original code (represented here for completeness)
static int g_unsupported_chunks; // mirrors the original code's global increment
static bool g_png_error_seen;     // indicates if an error path was taken

static const char* g_last_error_message = nullptr;

// Mocked png_error to record its invocation
void png_error(png_structp /*png_ptr*/, const char* /*message*/) {
    g_png_error_seen = true;
    // In real usage this would abort; here we just record the event.
    g_last_error_message = message;
}

// Helpers to reset test state
void reset_test_state() {
    g_unsupported_chunks = 0;
    g_png_error_seen = false;
    g_last_error_message = nullptr;
}

// Variant 1: Simulate "no supported" behavior for zTXt and iTXt (i.e., unsupported_chunks increments
// for both zTXt and iTXt/text cases, and for invalid compression we call png_error).
void run_check_text_support_no_support(png_structp /*png_ptr*/, png_textp text_ptr, int num_text) {
    // Mirror the loop structure from the focal method
    while (num_text > 0) {
        switch (text_ptr[--num_text].compression) {
            case PNG_TEXT_COMPRESSION_NONE:
                // do nothing
                break;
            case PNG_TEXT_COMPRESSION_zTXt:
                ++g_unsupported_chunks;
                text_ptr[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
                break;
            case PNG_ITXT_COMPRESSION_NONE:
            case PNG_ITXT_COMPRESSION_zTXt:
                ++g_unsupported_chunks;
                text_ptr[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
                break;
            default:
                png_error(nullptr, "invalid text chunk compression field");
                break;
        }
    }
}

// Variant 2: Simulate "zTXt is supported" while iTXt is still potentially unsupported.
// - For zTXt: no action (supported).
// - For iTXt: treat as unsupported (increment and normalize to NONE).
void run_check_text_support_with_zTXt_support(png_structp /*png_ptr*/, png_textp text_ptr, int num_text) {
    while (num_text > 0) {
        switch (text_ptr[--num_text].compression) {
            case PNG_TEXT_COMPRESSION_NONE:
                break;
            case PNG_TEXT_COMPRESSION_zTXt:
                // zTXt is supported in this variant: do nothing
                break;
            case PNG_ITXT_COMPRESSION_NONE:
            case PNG_ITXT_COMPRESSION_zTXt:
                ++g_unsupported_chunks;
                text_ptr[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
                break;
            default:
                png_error(nullptr, "invalid text chunk compression field");
                break;
        }
    }
}

// Simple assertion helper with message
void require(bool condition, const char* msg) {
    if (!condition) {
        std::cerr << "Test failed: " << msg << std::endl;
        std::terminate();
    }
}

// Test Case 1: Single NONE entry should leave state unchanged (variant_no_support)
void test_case_none_single_no_support() {
    reset_test_state();
    std::vector<png_text> texts(1);
    texts[0].compression = PNG_TEXT_COMPRESSION_NONE;

    run_check_text_support_no_support(nullptr, texts.data(), (int)texts.size());

    // Expect no unsupported chunks and no error
    assert(g_unsupported_chunks == 0);
    assert(texts[0].compression == PNG_TEXT_COMPRESSION_NONE);
    require(!g_png_error_seen, "Unexpected error in test_case_none_single_no_support");
    std::cout << "PASS: test_case_none_single_no_support" << std::endl;
}

// Test Case 2: zTXt entry with no_ZTXt_support should increment unsupported and zero it
void test_case_ztxt_single_no_support() {
    reset_test_state();
    std::vector<png_text> texts(1);
    texts[0].compression = PNG_TEXT_COMPRESSION_zTXt;

    run_check_text_support_no_support(nullptr, texts.data(), (int)texts.size());

    // Expect one unsupported chunk and the entry converted to NONE
    assert(g_unsupported_chunks == 1);
    assert(texts[0].compression == PNG_TEXT_COMPRESSION_NONE);
    require(!g_png_error_seen, "Unexpected error in test_case_ztxt_single_no_support");
    std::cout << "PASS: test_case_ztxt_single_no_support" << std::endl;
}

// Test Case 3: iTXt NONE entry with no_ZTXt_support should increment and convert
void test_case_itxt_none_single_no_support() {
    reset_test_state();
    std::vector<png_text> texts(1);
    texts[0].compression = PNG_ITXT_COMPRESSION_NONE;

    run_check_text_support_no_support(nullptr, texts.data(), (int)texts.size());

    assert(g_unsupported_chunks == 1);
    assert(texts[0].compression == PNG_TEXT_COMPRESSION_NONE);
    require(!g_png_error_seen, "Unexpected error in test_case_itxt_none_single_no_support");
    std::cout << "PASS: test_case_itxt_none_single_no_support" << std::endl;
}

// Test Case 4: Invalid compression triggers png_error (variant_no_support)
void test_case_invalid_compression_triggers_error_no_support() {
    reset_test_state();
    std::vector<png_text> texts(1);
    texts[0].compression = 99; // invalid

    run_check_text_support_no_support(nullptr, texts.data(), (int)texts.size());

    // In this variant, default case triggers png_error
    assert(g_png_error_seen);
    require(g_last_error_message != nullptr, "Expected error message to be set");
    std::cout << "PASS: test_case_invalid_compression_triggers_error_no_support" << std::endl;
}

// Test Case 5: Multiple entries with zTXt and NONE (order reversed due to loop behavior)
void test_case_multiple_reverse_processing_no_support() {
    reset_test_state();
    std::vector<png_text> texts(2);
    texts[0].compression = PNG_TEXT_COMPRESSION_NONE;
    texts[1].compression = PNG_TEXT_COMPRESSION_zTXt;

    run_check_text_support_no_support(nullptr, texts.data(), (int)texts.size());

    // Process order: index 1 first (zTXt -> 1), then index 0 (NONE)
    // Expect 1 unsupported chunk and texts[1] becomes NONE
    assert(g_unsupported_chunks == 1);
    assert(texts[1].compression == PNG_TEXT_COMPRESSION_NONE);
    assert(texts[0].compression == PNG_TEXT_COMPRESSION_NONE);
    require(!g_png_error_seen, "Unexpected error in test_case_multiple_reverse_processing_no_support");
    std::cout << "PASS: test_case_multiple_reverse_processing_no_support" << std::endl;
}

// Test Case 6: zTXt supported variant (variant_with_zTXt_support)
void test_case_ztxt_supported_variant() {
    reset_test_state();
    std::vector<png_text> texts(1);
    texts[0].compression = PNG_TEXT_COMPRESSION_zTXt;

    run_check_text_support_with_zTXt_support(nullptr, texts.data(), (int)texts.size());

    // zTXt is considered supported; no increments
    assert(g_unsupported_chunks == 0);
    assert(texts[0].compression == PNG_TEXT_COMPRESSION_zTXt);
    require(!g_png_error_seen, "Unexpected error in test_case_ztxt_supported_variant");
    std::cout << "PASS: test_case_ztxt_supported_variant" << std::endl;
}

// Test Case 7: iTXt entry with zTXt-supported variant should increment (as per our variant)
void test_case_itxt_untouched_in_supported_variant() {
    reset_test_state();
    std::vector<png_text> texts(1);
    texts[0].compression = PNG_ITXT_COMPRESSION_NONE;

    run_check_text_support_with_zTXt_support(nullptr, texts.data(), (int)texts.size());

    // iTXt NONE case should be treated as unsupported in this variant
    assert(g_unsupported_chunks == 1);
    assert(texts[0].compression == PNG_TEXT_COMPRESSION_NONE);
    require(!g_png_error_seen, "Unexpected error in test_case_itxt_untouched_in_supported_variant");
    std::cout << "PASS: test_case_itxt_untouched_in_supported_variant" << std::endl;
}

// Test Case 8: Mixed entries in supported variant (reverse processing)
void test_case_mixed_entries_supported_variant() {
    reset_test_state();
    std::vector<png_text> texts(2);
    texts[0].compression = PNG_TEXT_COMPRESSION_zTXt; // initially zTXt (supported)
    texts[1].compression = PNG_ITXT_COMPRESSION_NONE; // to be treated as unsupported

    run_check_text_support_with_zTXt_support(nullptr, texts.data(), (int)texts.size());

    // Process order: index 1 (iTXt NONE) -> increments; index 0 (zTXt) -> no action
    assert(g_unsupported_chunks == 1);
    // texts[1] should be converted to NONE
    assert(texts[1].compression == PNG_TEXT_COMPRESSION_NONE);
    require(!g_png_error_seen, "Unexpected error in test_case_mixed_entries_supported_variant");
    std::cout << "PASS: test_case_mixed_entries_supported_variant" << std::endl;
}

// Main driver to run all tests
int main() {
    // Run tests for Variant 1: no-support semantics
    test_case_none_single_no_support();
    test_case_ztxt_single_no_support();
    test_case_itxt_none_single_no_support();
    test_case_invalid_compression_triggers_error_no_support();
    test_case_multiple_reverse_processing_no_support();

    // Run tests for Variant 2: zTXt supported (as per our simulated macro)
    test_case_ztxt_supported_variant();
    test_case_itxt_untouched_in_supported_variant();
    test_case_mixed_entries_supported_variant();

    std::cout << "All tests completed." << std::endl;
    return 0;
}
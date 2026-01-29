// Test harness for HandlePhotoshopAPP13 in jpgicc.c
// This test suite is designed for C++11 compilation without using GTest.
// It mocks minimal JPEG lib types necessary to exercise the focal function's logic.
// Note: A matching jpeglib.h is expected by the actual build; this test provides
// compatible type definitions and the required function prototype to link.

#include <cstring>
#include <iccjpeg.h>
#include <jpeglib.h>
#include <iostream>
#include <cassert>
#include <utils.h>


// ---------------------------------------------------------------------------
// Mock minimal jpeglib.h definitions to allow compilation of jpgicc.c
// These definitions are kept intentionally small; they mirror the usage in
// HandlePhotoshopAPP13 (jpeg_saved_marker_struct, marker, data, next, etc.)
// The goal is to enable unit-test compilation and focus on control flow.
// ---------------------------------------------------------------------------

#ifndef MOCK_JPEGLIB_H
#define MOCK_JPEGLIB_H

typedef unsigned char JOCTET;
#define JPEG_APP0 0xE0
#define GETJOCTET(x) ((JOCTET)(x))

// Minimal representation of a JPEG saved marker in a chain
typedef struct jpeg_saved_marker_struct {
    int marker;
    unsigned int data_length;
    JOCTET* data;
    struct jpeg_saved_marker_struct* next;
} jpeg_saved_marker_struct;

typedef jpeg_saved_marker_struct* jpeg_saved_marker_ptr;

// Basic boolean type used by the focal function
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

// Declaration of the focal method's dependency (as it would appear in the C file)
extern "C" cmsBool HandlePhotoshopAPP13(jpeg_saved_marker_ptr ptr);

#endif // MOCK_JPEGLIB_H

// ---------------------------------------------------------------------------
// Test harness utilities
// ---------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define LOG_ERROR(msg) do { \
    std::cerr << "ERROR: " << msg << std::endl; \
} while (0)

static void log_test_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[OK] " << test_name << "\n";
    } else {
        ++g_failed_tests;
        std::cerr << "[FAILED] " << test_name << "\n";
    }
}

// Helper to safely free a chain of jpeg_saved_marker_struct
static void free_marker_chain(jpeg_saved_marker_ptr head) {
    while (head) {
        jpeg_saved_marker_ptr tmp = head;
        head = head->next;
        delete[] tmp->data;
        delete tmp;
    }
}

// Factory to create a marker node with given parameters
static jpeg_saved_marker_ptr make_marker_node(int marker, const unsigned char* data, unsigned int len, jpeg_saved_marker_ptr next) {
    jpeg_saved_marker_ptr node = new jpeg_saved_marker_struct;
    node->marker = marker;
    node->data_length = len;
    node->data = new JOCTET[len];
    if (data != nullptr && len > 0) {
        std::memcpy(node->data, data, len);
    }
    node->next = next;
    return node;
}

// Helper to build a test data array of given length (filled from provided bytes)
static unsigned char make_buffer(const unsigned char* src, size_t len) {
    unsigned char* dst = new unsigned char[len];
    if (src && len > 0) {
        std::memcpy(dst, src, len);
    }
    return dst[0]; // Just to avoid unused variable warning in some compilers (we won't use this value)
}

// ---------------------------------------------------------------------------
// Tests
// Each test returns true on success and false on failure.


// Test 1: Single node with matching marker and correct "Photoshop" header -> expect TRUE
static bool test_HandlePhotoshopAPP13_SingleMatch() {
    // 9-byte header for "Photoshop"
    unsigned char header9[9] = { 0x50,0x68,0x6F,0x74,0x6F,0x73,0x68,0x6F,0x70 };
    // data_length must be > 9; provide 10 bytes total
    unsigned char data[10] = { 
        0x50,0x68,0x6F,0x74,0x6F,0x73,0x68,0x6F,0x70, 0x00
    };

    jpeg_saved_marker_ptr head = make_marker_node(JPEG_APP0 + 13, data, 10, nullptr);

    // Call the focal method
    cmsBool result = HandlePhotoshopAPP13(head);

    // Clean up
    free_marker_chain(head);

    // Expect TRUE
    bool ok = (result == TRUE);
    if (!ok) log_test_result("test_HandlePhotoshopAPP13_SingleMatch", false);
    else log_test_result("test_HandlePhotoshopAPP13_SingleMatch", true);
    return ok;
}


// Test 2: Single node with non-matching header -> expect FALSE
static bool test_HandlePhotoshopAPP13_SingleNonMatchHeader() {
    unsigned char header9[9] = { 0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88 };
    unsigned char data[10] = { 0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99 };

    jpeg_saved_marker_ptr head = make_marker_node(JPEG_APP0 + 13, data, 10, nullptr);
    // Overwrite header portion by adjusting data to non-matching bytes
    std::memcpy(head->data, header9, 9);

    cmsBool result = HandlePhotoshopAPP13(head);

    free_marker_chain(head);

    bool ok = (result == FALSE);
    if (!ok) log_test_result("test_HandlePhotoshopAPP13_SingleNonMatchHeader", false);
    else log_test_result("test_HandlePhotoshopAPP13_SingleNonMatchHeader", true);
    return ok;
}


// Test 3: Multiple nodes where the matching marker+header occurs on the second node -> expect TRUE
static bool test_HandlePhotoshopAPP13_MatchSecondNode() {
    // First node: non-matching
    unsigned char nonHeader[9] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09 };
    unsigned char data1[10] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xAA };

    // Second node: matching header
    unsigned char header9[9] = { 0x50,0x68,0x6F,0x74,0x6F,0x73,0x68,0x6F,0x70 };
    unsigned char data2[10] = { 0x50,0x68,0x6F,0x74,0x6F,0x73,0x68,0x6F,0x70,0x00 };

    jpeg_saved_marker_ptr second = make_marker_node(JPEG_APP0 + 13, data2, 10, nullptr);
    // Ensure header is correct for the second node
    std::memcpy(second->data, header9, 9);

    jpeg_saved_marker_ptr first = make_marker_node(0, data1, 10, second);

    cmsBool result = HandlePhotoshopAPP13(first);

    free_marker_chain(first);

    bool ok = (result == TRUE);
    if (!ok) log_test_result("test_HandlePhotoshopAPP13_MatchSecondNode", false);
    else log_test_result("test_HandlePhotoshopAPP13_MatchSecondNode", true);
    return ok;
}


// Test 4: No nodes (nullptr) -> expect FALSE
static bool test_HandlePhotoshopAPP13_NullPtr() {
    jpeg_saved_marker_ptr head = nullptr;
    cmsBool result = HandlePhotoshopAPP13(head);
    bool ok = (result == FALSE);
    if (!ok) log_test_result("test_HandlePhotoshopAPP13_NullPtr", false);
    else log_test_result("test_HandlePhotoshopAPP13_NullPtr", true);
    return ok;
}


// Test 5: Node with matching marker but data_length <= 9 -> expect FALSE
static bool test_HandlePhotoshopAPP13_DataLengthTooSmall() {
    unsigned char data[9] = { 0x50,0x68,0x6F,0x74,0x6F,0x73,0x68,0x6F,0x70 };
    // data_length exactly 9, which should fail (>9 required)
    jpeg_saved_marker_ptr head = make_marker_node(JPEG_APP0 + 13, data, 9, nullptr);

    cmsBool result = HandlePhotoshopAPP13(head);

    free_marker_chain(head);

    bool ok = (result == FALSE);
    if (!ok) log_test_result("test_HandlePhotoshopAPP13_DataLengthTooSmall", false);
    else log_test_result("test_HandlePhotoshopAPP13_DataLengthTooSmall", true);
    return ok;
}


// Test 6: Marker present but with non-matching header in all nodes -> FALSE
static bool test_HandlePhotoshopAPP13_NoMatchingHeaderAmongNodes() {
    // Node1: matching marker but non-matching header
    unsigned char badHeader[9] = { 0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE };
    unsigned char data1[10] = { 0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD };

    // Node2: different marker
    unsigned char header9[9] = { 0x50,0x68,0x6F,0x74,0x6F,0x73,0x68,0x6F,0x70 };
    unsigned char data2[10] = { 0x50,0x68,0x6F,0x74,0x6F,0x73,0x68,0x6F,0x70,0x00 };

    jpeg_saved_marker_ptr second = make_marker_node(JPEG_APP0 + 13, data2, 10, nullptr);
    // Overwrite to non-matching header in second as well
    std::memcpy(second->data, header9, 9);

    jpeg_saved_marker_ptr first = make_marker_node(JPEG_APP0 + 13, data1, 10, second);
    // Overwrite first node's header bytes to non-matching
    std::memcpy(first->data, badHeader, 9);

    cmsBool result = HandlePhotoshopAPP13(first);

    free_marker_chain(first);

    bool ok = (result == FALSE);
    if (!ok) log_test_result("test_HandlePhotoshopAPP13_NoMatchingHeaderAmongNodes", false);
    else log_test_result("test_HandlePhotoshopAPP13_NoMatchingHeaderAmongNodes", true);
    return ok;
}

// ---------------------------------------------------------------------------
// Main driver
// ---------------------------------------------------------------------------

int main() {
    bool all_ok = true;

    // Run each test; in case of failure, accumulate results but continue to maximize coverage
    {
        bool r = test_HandlePhotoshopAPP13_SingleMatch();
        all_ok = all_ok && r;
    }
    {
        bool r = test_HandlePhotoshopAPP13_SingleNonMatchHeader();
        all_ok = all_ok && r;
    }
    {
        bool r = test_HandlePhotoshopAPP13_MatchSecondNode();
        all_ok = all_ok && r;
    }
    {
        bool r = test_HandlePhotoshopAPP13_NullPtr();
        all_ok = all_ok && r;
    }
    {
        bool r = test_HandlePhotoshopAPP13_DataLengthTooSmall();
        all_ok = all_ok && r;
    }
    {
        bool r = test_HandlePhotoshopAPP13_NoMatchingHeaderAmongNodes();
        all_ok = all_ok && r;
    }

    // Summary
    std::cout << "\nTest Summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failed, "
              << (g_total_tests - g_failed_tests) << " passed.\n";

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}
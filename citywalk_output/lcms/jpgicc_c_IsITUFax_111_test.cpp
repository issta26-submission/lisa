// Unit test suite for the IsITUFax function (C domain) using a C++11 test harness.
// This test avoids GTest and uses a lightweight in-file test framework.
// It mocks the minimal libjpeg-related types required to exercise IsITUFax
// without requiring the full libjpeg dependency.

#include <cstring>
#include <iccjpeg.h>
#include <jpeglib.h>
#include <iostream>
#include <cassert>
#include <utils.h>


// Domain-mocked definitions to enable compilation of the test harness
// without pulling in the real libjpeg headers. The real IsITUFax in the
// focal code uses these types and constants.

typedef int cmsBool;       // Mimic cmsBool from the original code
#define TRUE 1
#define FALSE 0

// JPEG_APP0 is defined as 0xE0 in libjpeg; APP1 = JPEG_APP0 + 1
#define JPEG_APP0 0xE0

// Forward declaration: the focal IsITUFax function is defined in a C file.
// We declare it with C linkage to match the actual compiled symbol.
extern "C" cmsBool IsITUFax(struct jpeg_saved_marker_struct* ptr);

// Minimal stub for the libjpeg-supplied structure used by IsITUFax
struct jpeg_saved_marker_struct {
    unsigned int marker;
    unsigned int data_length;
    unsigned char* data;
    struct jpeg_saved_marker_struct* next;
};

// Typedef matching the signature used by IsITUFax
typedef jpeg_saved_marker_struct* jpeg_saved_marker_ptr;

// Lightweight non-terminating test reporting framework
static int g_testFailures = 0;

static void report_failure(const char* testName, const char* message) {
    std::cerr << "[TEST FAILURE] " << testName << " - " << message << std::endl;
    ++g_testFailures;
}

#define TEST_NAME (__func__)

// Non-terminating assertions: do not exit on failure; aggregate results
#define EXPECT_TRUE(cond) \
    do { if(!(cond)) report_failure(TEST_NAME, #cond " is false, expected true"); } while(0)

#define EXPECT_FALSE(cond) \
    do { if((cond)) report_failure(TEST_NAME, #cond " is true, expected false"); } while(0)

// Helper to create a linked list marker chain for testing
static jpeg_saved_marker_ptr mkMarker(int marker, const unsigned char* data, unsigned int len, jpeg_saved_marker_ptr next) {
    jpeg_saved_marker_ptr node = new jpeg_saved_marker_struct;
    node->marker = marker;
    node->data_length = len;
    node->next = next;
    if (len > 0 && data != nullptr) {
        node->data = new unsigned char[len];
        memcpy(node->data, data, len);
    } else {
        node->data = nullptr;
    }
    return node;
}

static void freeChain(jpeg_saved_marker_ptr head) {
    while (head) {
        jpeg_saved_marker_ptr t = head->next;
        delete[] head->data;
        delete head;
        head = t;
    }
}

// Test 1: No markers present -> IsITUFax should return FALSE
static void test_IsITUFax_NoMarkers_ReturnFalse() {
    // NULL chain
    jpeg_saved_marker_ptr head = nullptr;
    cmsBool res = IsITUFax(head);
    EXPECT_FALSE(res);
}

// Test 2: Marker present but not APP1 (i.e., not JPEG_APP0 + 1) -> FALSE
static void test_IsITUFax_MarkerNotAPP1_ReturnFalse() {
    // APP0 marker with proper data (data content is irrelevant when marker != APP1)
    const unsigned char data6[6] = { 'G', '3', 'F', 'A', 'X', '\0' }; // "G3FAX" style but marker different
    jpeg_saved_marker_ptr m1 = mkMarker(JPEG_APP0, data6, 6, nullptr);

    cmsBool res = IsITUFax(m1);
    EXPECT_FALSE(res);

    freeChain(m1);
}

// Test 3: APP1 marker with non-G3FAX data and length > 5 -> FALSE
static void test_IsITUFax_APP1_WithNonG3FAXData_ReturnFalse() {
    // APP1 marker with data that is not "G3FAX"
    const unsigned char data7[7] = { 'X', 'Y', 'Z', '1', '2', '3', '\0' }; // "XYZ123"
    jpeg_saved_marker_ptr m1 = mkMarker(JPEG_APP0 + 1, data7, 7, nullptr);

    cmsBool res = IsITUFax(m1);
    EXPECT_FALSE(res);

    freeChain(m1);
}

// Test 4: APP1 marker with data exactly "G3FAX" and length > 5 -> TRUE
static void test_IsITUFax_APP1_WithG3FAX_ReturnTrue() {
    // APP1 marker with data "G3FAX" and a terminator to ensure strcmp succeeds
    const unsigned char data6[6] = { 'G', '3', 'F', 'A', 'X', '\0' }; // "G3FAX"
    jpeg_saved_marker_ptr m1 = mkMarker(JPEG_APP0 + 1, data6, 6, nullptr);

    cmsBool res = IsITUFax(m1);
    EXPECT_TRUE(res);

    freeChain(m1);
}

// Test 5: Chain where the first marker is not APP1 and a subsequent APP1 marker contains "G3FAX" -> TRUE
static void test_IsITUFax_MarkerChain_APP1Later_ReturnsTrue() {
    // First marker: APP0 (ignored)
    const unsigned char dataFirst[6] = { 'A', 'B', 'C', 'D', 'E', '\0' };
    jpeg_saved_marker_ptr m2 = mkMarker(JPEG_APP0 + 1, (const unsigned char*)"G3FAX", 6, nullptr); // will be the APP1 marker
    jpeg_saved_marker_ptr head = mkMarker(JPEG_APP0, dataFirst, 6, m2);

    cmsBool res = IsITUFax(head);
    EXPECT_TRUE(res);

    freeChain(head);
}

// Run all tests and print summary
static void runAllTests() {
    test_IsITUFax_NoMarkers_ReturnFalse();
    test_IsITUFax_MarkerNotAPP1_ReturnFalse();
    test_IsITUFax_APP1_WithNonG3FAXData_ReturnFalse();
    test_IsITUFax_APP1_WithG3FAX_ReturnTrue();
    test_IsITUFax_MarkerChain_APP1Later_ReturnsTrue();

    // Summary
    std::cout << "Total tests executed: 5" << std::endl;
    if (g_testFailures == 0) {
        std::cout << "[ALL TESTS PASSED]" << std::endl;
    } else {
        std::cout << "[TESTS FAILED] Failures: " << g_testFailures << std::endl;
    }
}

// Main entry: run the tests
int main() {
    runAllTests();
    return (g_testFailures == 0) ? 0 : 1;
}
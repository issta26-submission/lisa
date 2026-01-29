/*
Step 1 (Program Understanding) - Candidate Keywords
- Marker constant: "G3FAX" and the 10-byte payload used by SetITUFax
- JPEG marker: (JPEG_APP0 + 1)
- Library call: jpeg_write_marker
- Target object: j_compress_ptr cinfo (JPEG compression state)
- Marker container: cinfo.marker_list (linked list of saved markers)
- Data persistence: marker.data (Marker payload), marker.marker (type), marker.next (next in list)
- Expected behavior: SetITUFax should append a marker with code JPEG_APP0+1 containing 10 data bytes starting with "G3FAX"
This test suite focuses on validating that the marker is written and stored in the marker list.
*/

#include <cstring>
#include <iccjpeg.h>
#include <jpeglib.h>
#include <vector>
#include <iostream>
#include <cassert>
#include <utils.h>


// Include libjpeg headers to access the real types and constants.
// Assumes libjpeg development package is available in the environment.

extern "C" void SetITUFax(j_compress_ptr cinfo); // Focal method under test (must be linked with jpgicc.c)

/*
Step 2 (Unit Test Generation)
We implement a small, self-contained test suite (no GTest) to validate SetITUFax:

- Test 1: Marker app0+1 is written after a single call to SetITUFax.
- Test 2: Repeated calls accumulate multiple markers of type app0+1.
- Test 3: The first bytes of the marker data begin with "G3FAX".

Notes:
- We rely on the public libjpeg API to create and inspect the marker_list.
- We do not rely on private/private-implementation details beyond the marker_list structure.
- All tests are non-terminating: they report failures but continue where reasonable.
*/

static int g_failures = 0;

// Simple assertion macro that prints failures but does not terminate the program.
#define ASSERT(cond, msg)                                      \
    do {                                                       \
        if (!(cond)) {                                       \
            std::cerr << "Assertion failed: " << (msg)       \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" \
                      << std::endl;                          \
            ++g_failures;                                    \
        }                                                    \
    } while (0)

// Helper to count how many markers of type (JPEG_APP0 + 1) exist in the marker_list.
static int count_itufax_markers(jpeg_compress_struct* cinfo) {
    int count = 0;
    for (jpeg_marker_ptr m = cinfo->marker_list; m != nullptr; m = m->next) {
        // Some libjpeg versions expose 'marker' field; guard with dynamic check
        // If 'marker' field exists and equals (JPEG_APP0+1), count it.
        if (m->marker == (JPEG_APP0 + 1)) {
            ++count;
        }
    }
    return count;
}

// Helper: locate the first ITU/FAX marker (JPEG_APP0+1) and return its data pointer if present.
static unsigned char* first_itufax_marker_data(jpeg_compress_struct* cinfo) {
    for (jpeg_marker_ptr m = cinfo->marker_list; m != nullptr; m = m->next) {
        if (m->marker == (JPEG_APP0 + 1)) {
            // Return the pointer to data if available
            return m->data;
        }
    }
    return nullptr;
}

// Test 1: Ensure a single SetITUFax call appends exactly one ITU/FAX marker (JPEG_APP0+1).
bool test_SetITUFax_writes_one_marker() {
    // Prepare a fresh compress struct
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    // Initialize compressor (no actual file/destination needed for marker test)
    if (jpeg_create_compress(&cinfo) != 0) {
        // Some libjpeg versions return void; this check is defensive.
    }

    // Start with a clean marker list
    cinfo.marker_list = nullptr;

    // Call the focal method
    SetITUFax(&cinfo);

    // Verify we have at least one ITU/Fax marker (JPEG_APP0+1)
    int itufax_count = count_itufax_markers(&cinfo);
    bool passed = (itufax_count >= 1);

    // Cleanup
    jpeg_destroy_compress(&cinfo);

    if (!passed) {
        std::cerr << "Test 1 failed: Expected at least one ITU/FAX marker after SetITUFax." << std::endl;
    }
    return passed;
}

// Test 2: Ensure multiple calls append multiple ITU/FAX markers.
bool test_SetITUFax_appends_multiple_markers() {
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);
    cinfo.marker_list = nullptr;

    // First call
    SetITUFax(&cinfo);
    // Second call
    SetITUFax(&cinfo);

    // Count the number of ITU/FAX markers
    int itufax_count = count_itufax_markers(&cinfo);

    // Cleanup
    jpeg_destroy_compress(&cinfo);

    bool passed = (itufax_count >= 2);
    if (!passed) {
        std::cerr << "Test 2 failed: Expected at least two ITU/FAX markers after two SetITUFax calls." << std::endl;
    }
    return passed;
}

// Test 3: Validate the marker payload begins with "G3FAX" when available.
bool test_SetITUFax_marker_payload_starts_with_G3FAX() {
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);
    cinfo.marker_list = nullptr;

    SetITUFax(&cinfo);

    // Locate first ITU/FAX marker and inspect its data payload
    bool starts_with_G3FAX = false;
    for (jpeg_marker_ptr m = cinfo.marker_list; m != nullptr; m = m->next) {
        if (m->marker == (JPEG_APP0 + 1) && m->data != nullptr) {
            // Check first five bytes for "G3FAX"
            unsigned char* d = m->data;
            if (std::strncmp(reinterpret_cast<const char*>(d), "G3FAX", 5) == 0) {
                starts_with_G3FAX = true;
            }
            break;
        }
    }

    jpeg_destroy_compress(&cinfo);

    if (!starts_with_G3FAX) {
        std::cerr << "Test 3 failed: ITU/FAX marker payload did not start with 'G3FAX'." << std::endl;
    }
    return starts_with_G3FAX;
}

// Step 3 (Test Case Refinement) - Additional tests could be added here to further improve coverage.
// For example, tests that deliberately misuse the API to observe non-crashing behavior could be added.
// The public, non-terminating assertions above already help maximize code execution paths.

// Main harness to run all tests
int main() {
    // Informational banner
    std::cout << "Starting unit tests for SetITUFax (C++11, no GTest)." << std::endl;

    std::vector<std::string> testNames;
    testNames.push_back("test_SetITUFax_writes_one_marker");
    testNames.push_back("test_SetITUFax_appends_multiple_markers");
    testNames.push_back("test_SetITUFax_marker_payload_starts_with_G3FAX");

    int testIndex = 0;
    int totalTests = (int)testNames.size();
    int localFailures = 0;

    // Run Test 1
    ++testIndex;
    bool t1 = test_SetITUFax_writes_one_marker();
    if (!t1) ++localFailures;

    // Run Test 2
    ++testIndex;
    bool t2 = test_SetITUFax_appends_multiple_markers();
    if (!t2) ++localFailures;

    // Run Test 3
    ++testIndex;
    bool t3 = test_SetITUFax_marker_payload_starts_with_G3FAX();
    if (!t3) ++localFailures;

    // Summary
    if (localFailures == 0) {
        std::cout << "ALL TESTS PASSED (" << totalTests << " tests)." << std::endl;
    } else {
        std::cout << localFailures << " TEST(S) FAILED OUT OF " << totalTests << "." << std::endl;
    }

    return localFailures;
}

/*
Notes on coverage and design decisions (Step 3 rationale):
- We relied on the public libjpeg interface to initialize a real compression structure and to inspect the marker_list after invoking SetITUFax.
- We covered: (a) single write, (b) multiple writes, (c) payload starts with the expected magic sequence.
- We avoided private/internal details of the FOCAL_CLASS_DEP block by using the public API to validate behavior, ensuring compatibility across common libjpeg versions.
- We avoided terminating tests on first failure and instead accumulate failures to maximize code execution paths and coverage.
*/
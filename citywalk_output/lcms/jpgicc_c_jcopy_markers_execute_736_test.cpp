// This test suite provides a self-contained C++11 test harness
// for the focal function jcopy_markers_execute, using lightweight
// stubs to mimic the minimal libjpeg types and behavior.
// It does not rely on GTest and uses simple EXPECT/ASSERT-style checks
// that log failures but continue execution to maximize coverage.

#include <cstring>
#include <iccjpeg.h>
#include <cstdint>
#include <jpeglib.h>
#include <vector>
#include <iostream>
#include <utils.h>


// -----------------------------------------------------------------------------
// Minimal, self-contained stubs mimicking the required parts of jpeglib.h
// -----------------------------------------------------------------------------

// JPEG marker constants used by the focal method
#define JPEG_APP0 0xE0

// Macro to mimic the behavior of GETJOCTET in the original code
#define GETJOCTET(x) static_cast<unsigned int>(static_cast<unsigned char>(x))

// Forward declarations for the types used by the focal function
struct jpeg_saved_marker_struct;
typedef jpeg_saved_marker_struct* jpeg_saved_marker_ptr;

// Minimal jpeg_decompress_ptr and jpeg_compress_ptr types
struct jpeg_decompress_struct { jpeg_saved_marker_ptr marker_list; };
struct jpeg_compress_struct { int write_JFIF_header; int write_Adobe_marker; };

// Pointers to opaque libjpeg structs in the original code
typedef struct jpeg_decompress_struct* j_decompress_ptr;
typedef struct jpeg_compress_struct* j_compress_ptr;

// Minimal marker node structure (as in libjpeg)
struct jpeg_saved_marker_struct {
    int marker;
    unsigned char* data;
    unsigned int data_length;
    jpeg_saved_marker_ptr next;
};

// Global test harness data: captures writes that would be sent to the encoder
struct MarkerWriteRecord {
    int marker;
    unsigned int data_length;
    std::vector<unsigned char> data;
};
static std::vector<MarkerWriteRecord> g_marker_writes;

// Fake jpeg_write_marker implementation: records the write requests for inspection
void jpeg_write_marker(j_compress_ptr dstinfo, int marker, unsigned char* data, unsigned int data_length)
{
    (void)dstinfo; // in tests we only verify calls through recorded data
    MarkerWriteRecord rec;
    rec.marker = marker;
    rec.data_length = data_length;
    rec.data.assign(data, data + data_length);
    g_marker_writes.push_back(std::move(rec));
}

// -----------------------------------------------------------------------------
// Focal method under test (reproduced here to enable standalone unit tests)
// -----------------------------------------------------------------------------

void jcopy_markers_execute(j_decompress_ptr srcinfo, j_compress_ptr dstinfo)
{
{
  jpeg_saved_marker_ptr marker;
  /* In the current implementation, we don't actually need to examine the
   * option flag here; we just copy everything that got saved.
   * But to avoid confusion, we do not output JFIF and Adobe APP14 markers
   * if the encoder library already wrote one.
   */
  for (marker = srcinfo->marker_list; marker != NULL; marker = marker->next) {
    if (dstinfo->write_JFIF_header &&
        marker->marker == JPEG_APP0 &&
        marker->data_length >= 5 &&
        GETJOCTET(marker->data[0]) == 0x4A &&
        GETJOCTET(marker->data[1]) == 0x46 &&
        GETJOCTET(marker->data[2]) == 0x49 &&
        GETJOCTET(marker->data[3]) == 0x46 &&
        GETJOCTET(marker->data[4]) == 0)
                          continue;         /* reject duplicate JFIF */
    if (dstinfo->write_Adobe_marker &&
        marker->marker == JPEG_APP0+14 &&
        marker->data_length >= 5 &&
        GETJOCTET(marker->data[0]) == 0x41 &&
        GETJOCTET(marker->data[1]) == 0x64 &&
        GETJOCTET(marker->data[2]) == 0x6F &&
        GETJOCTET(marker->data[3]) == 0x62 &&
        GETJOCTET(marker->data[4]) == 0x65)
                         continue;         /* reject duplicate Adobe */
     jpeg_write_marker(dstinfo, marker->marker,
                       marker->data, marker->data_length);
  }
}
 }

// -----------------------------------------------------------------------------
// Test harness (no external testing framework; simple, readable tests)
// -----------------------------------------------------------------------------

// Simple assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TESTCASE(name) void name(); \
    static void __register_##name() { tests().push_back(#name); } \
    static int _dummy_##name = ( [&](){ name(); return 0; }(), 0 );

#define ASSERT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::cerr << "ASSERT_TRUE FAILED: " << (msg) << "\n"; \
            ++g_failed_tests; \
        } \
    } while(0)

#define ASSERT_EQ(a,b, msg) \
    do { \
        ++g_total_tests; \
        if (!((a) == (b))) { \
            std::cerr << "ASSERT_EQ FAILED: " << (msg) \
                      << " (expected " << (b) << ", got " << (a) << ")" << "\n"; \
            ++g_failed_tests; \
        } \
    } while(0)

// List of test function names (for potential iteration/printing)
static std::vector<const char*> g_test_names;

// Forward declaration to access the test list
static std::vector<const char*>& tests() {
    static std::vector<const char*> t;
    return t;
}

// Helper: create a linked list of jpeg_saved_marker_struct nodes
static jpeg_saved_marker_ptr create_marker_chain(int marker, const unsigned char* data, unsigned int length, jpeg_saved_marker_ptr next)
{
    jpeg_saved_marker_ptr node = new jpeg_saved_marker_struct;
    node->marker = marker;
    node->data_length = length;
    if (length > 0) {
        node->data = new unsigned char[length];
        std::memcpy(node->data, data, length);
    } else {
        node->data = nullptr;
    }
    node->next = next;
    return node;
}

// Helper: free the marker chain
static void free_marker_chain(jpeg_saved_marker_ptr head)
{
    while (head) {
        jpeg_saved_marker_ptr cur = head;
        head = head->next;
        delete[] cur->data;
        delete cur;
    }
}

// Test 1: No markers in source; ensure no writes occur
void test_no_markers_in_source()
{
    // Reset global writes
    g_marker_writes.clear();

    // Prepare src and dst
    jpeg_decompress_struct srcinfo;
    srcinfo.marker_list = nullptr;

    jpeg_compress_struct dstinfo;
    dstinfo.write_JFIF_header = 1; // flags set but no markers
    dstinfo.write_Adobe_marker = 1;

    // Execute
    jcopy_markers_execute(&srcinfo, &dstinfo);

    // Assert: no writes produced
    ASSERT_EQ(g_marker_writes.size(), 0, "Expected zero marker writes when source has no markers");
}

// Test 2: JFIF duplicate (APP0 with 'JFIF\0') is skipped when write_JFIF_header is true
void test_jfif_duplicate_is_skipped()
{
    g_marker_writes.clear();

    // Build two markers:
    // m1: APP0 with JFIF header, should be skipped
    unsigned char jfif[5] = { 'J','F','I','F', 0 };
    jpeg_saved_marker_ptr m1 = create_marker_chain(JPEG_APP0, jfif, 5, nullptr);

    // m2: APP0 with non-JFIF header, should be written
    unsigned char payload[3] = { 0x01, 0x02, 0x03 };
    jpeg_saved_marker_ptr m2 = create_marker_chain(JPEG_APP0, payload, 3, nullptr);
    m1->next = m2;

    jpeg_decompress_struct srcinfo;
    srcinfo.marker_list = m1;

    jpeg_compress_struct dstinfo;
    dstinfo.write_JFIF_header = 1; // enable skip for JFIF
    dstinfo.write_Adobe_marker = 0;

    jcopy_markers_execute(&srcinfo, &dstinfo);

    // Expect that only m2 was written
    ASSERT_EQ(g_marker_writes.size(), 1, "Expected exactly one marker write (non-JFIF APP0)"); 
    if (!g_marker_writes.empty()) {
        ASSERT_EQ(g_marker_writes[0].marker, JPEG_APP0, "Written marker should be APP0 (non-JFIF)");
        ASSERT_EQ(g_marker_writes[0].data_length, 3, "Written data length should be 3 for m2");
    }

    // Cleanup
    free_marker_chain(m1);
}

// Test 3: Adobe APP14 duplicate is skipped when write_Adobe_marker is true
void test_adobe_duplicate_is_skipped()
{
    g_marker_writes.clear();

    // m1: APP0+14 (Adobe) with "Adobe" header; should be skipped
    unsigned char adobe_header[5] = { 'A','d','o','b','e' };
    jpeg_saved_marker_ptr m1 = create_marker_chain(JPEG_APP0+14, adobe_header, 5, nullptr);

    // m2: Some other marker; should be written
    unsigned char b[2] = { 0x10, 0x20 };
    jpeg_saved_marker_ptr m2 = create_marker_chain(0x01, b, 2, nullptr);
    m1->next = m2;

    jpeg_decompress_struct srcinfo;
    srcinfo.marker_list = m1;

    jpeg_compress_struct dstinfo;
    dstinfo.write_JFIF_header = 0;
    dstinfo.write_Adobe_marker = 1; // enable Adobe skip

    jcopy_markers_execute(&srcinfo, &dstinfo);

    // Expect that only m2 was written
    ASSERT_EQ(g_marker_writes.size(), 1, "Expected exactly one marker write (non-Adobe)"); 
    if (!g_marker_writes.empty()) {
        ASSERT_EQ(g_marker_writes[0].marker, 0x01, "Written marker should be 0x01");
        ASSERT_EQ(g_marker_writes[0].data_length, 2, "Written data length should be 2 for m2");
    }

    free_marker_chain(m1);
}

// Test 4: Mixed: skipped markers plus written markers
void test_mixed_writes_and_skips()
{
    g_marker_writes.clear();

    // m1: App0 JFIF header (to be skipped)
    unsigned char jfif[5] = { 'J','F','I','F', 0 };
    jpeg_saved_marker_ptr m1 = create_marker_chain(JPEG_APP0, jfif, 5, nullptr);

    // m2: App0+14 Adobe header (to be skipped)
    unsigned char adobe_header[5] = { 'A','d','o','b','e' };
    jpeg_saved_marker_ptr m2 = create_marker_chain(JPEG_APP0+14, adobe_header, 5, nullptr);
    m1->next = m2;

    // m3: normal APP0 marker; should be written
    unsigned char p1[3] = { 0x11, 0x22, 0x33 };
    jpeg_saved_marker_ptr m3 = create_marker_chain(JPEG_APP0, p1, 3, nullptr);
    m2->next = m3;

    // m4: normal marker with longer data; should be written
    unsigned char p2[4] = { 0x44, 0x55, 0x66, 0x77 };
    jpeg_saved_marker_ptr m4 = create_marker_chain(0x01, p2, 4, nullptr);
    m3->next = m4;

    jpeg_decompress_struct srcinfo;
    srcinfo.marker_list = m1;

    jpeg_compress_struct dstinfo;
    dstinfo.write_JFIF_header = 1; // JFIF skip enabled
    dstinfo.write_Adobe_marker = 1; // Adobe skip enabled

    jcopy_markers_execute(&srcinfo, &dstinfo);

    // Expect: m3 and m4 written, m1 and m2 skipped
    ASSERT_EQ(g_marker_writes.size(), 2, "Expected two writes for mixed markers (m3 and m4)");
    if (g_marker_writes.size() >= 2) {
        ASSERT_EQ(g_marker_writes[0].marker, JPEG_APP0, "First written marker should be APP0");
        ASSERT_EQ(g_marker_writes[1].marker, 0x01, "Second written marker should be 0x01");
    }

    free_marker_chain(m1);
}

// Test 5: All markers written when no skip conditions are met
void test_all_markers_written_when_no_skips()
{
    g_marker_writes.clear();

    // m1: APP0 JFIF header but write_JFIF_header is false -> should be written
    unsigned char jfif[5] = { 'J','F','I','F', 0 };
    jpeg_saved_marker_ptr m1 = create_marker_chain(JPEG_APP0, jfif, 5, nullptr);

    // m2: APP0+14 Adobe header but write_Adobe_marker is false -> should be written
    unsigned char adobe_header[5] = { 'A','d','o','b','e' };
    jpeg_saved_marker_ptr m2 = create_marker_chain(JPEG_APP0+14, adobe_header, 5, nullptr);
    m1->next = m2;

    jpeg_decompress_struct srcinfo;
    srcinfo.marker_list = m1;

    jpeg_compress_struct dstinfo;
    dstinfo.write_JFIF_header = 0; // no skips
    dstinfo.write_Adobe_marker = 0;

    // Some other marker to ensure multiple writes
    unsigned char d3[2] = { 0xAA, 0xBB };
    jpeg_saved_marker_ptr m3 = create_marker_chain(0x01, d3, 2, nullptr);
    m2->next = m3;

    jcopy_markers_execute(&srcinfo, &dstinfo);

    // Expect that all three were written (m1, m2, m3)
    ASSERT_EQ(g_marker_writes.size(), 3, "Expected three writes when no skips apply");
    free_marker_chain(m1);
}

// Test 6: Boundary case: data_length exactly 5 with JFIF bytes
void test_boundary_exactly_five_JFIF()
{
    g_marker_writes.clear();

    unsigned char jfif5[5] = { 'J','F','I','F', 0 }; // matches JFIF pattern
    jpeg_saved_marker_ptr m1 = create_marker_chain(JPEG_APP0, jfif5, 5, nullptr);

    jpeg_decompress_struct srcinfo;
    srcinfo.marker_list = m1;

    jpeg_compress_struct dstinfo;
    dstinfo.write_JFIF_header = 1;

    jcopy_markers_execute(&srcinfo, &dstinfo);

    // Must write nothing if only a JFIF marker matches and no non-JFIF exists
    // Depending on the test design, this should be either 0 (no writes)
    // or 1 (if there is no skip for this marker). Here, since only JFIF was present and it would be skipped, expect 0.
    ASSERT_EQ(g_marker_writes.size(), 0, "Expected zero writes when only JFIF marker present and skipped");

    free_marker_chain(m1);
}

// -----------------------------------------------------------------------------
// Test runner
// -----------------------------------------------------------------------------

int main()
{
    // Register & run test cases (in this simple harness, we invoke directly)
    test_no_markers_in_source();
    test_jfif_duplicate_is_skipped();
    test_adobe_duplicate_is_skipped();
    test_mixed_writes_and_skips();
    test_all_markers_written_when_no_skips();
    test_boundary_exactly_five_JFIF();

    // Summary
    std::cout << "Total tests executed: " << g_total_tests << "\n";
    std::cout << "Total failures: " << g_failed_tests << "\n";

    return (g_failed_tests != 0) ? 1 : 0;
}
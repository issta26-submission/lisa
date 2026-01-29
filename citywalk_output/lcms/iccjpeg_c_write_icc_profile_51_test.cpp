// C++11 test suite for write_icc_profile in iccjpeg.c without using GTest
// This test uses a lightweight harness with mock JPEG writer functions to verify
// the behavior of write_icc_profile when writing ICC profile data into JPEG APP2 ICC markers.

// Notes:
// - We rely on the project's iccjpeg.h for type definitions (j_compress_ptr, JOCTET, etc.)
// - We provide C linkage for the mock JPEG writer functions expected by the focal method.
// - We test single-marker behavior (icc_data_len <= MAX_DATA_BYTES_IN_MARKER) and zero-length input.
// - We do not terminate on assertion failures to maximize code coverage; failures are collected and reported at the end.

#include <cstring>
#include <iccjpeg.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cstdint>


// Include the focal header to pull in required declarations

// Lightweight test harness
static int g_test_failures = 0;

// Represent a simple, internal mock JPEG compression context.
// We piggy-back on the project's j_compress_ptr type by storage in a concrete object
// and casting its address to j_compress_ptr when passed to the mocked functions.
struct MockCInfo {
    // Capture the sequence of bytes written via jpeg_write_m_byte
    std::vector<uint8_t> bytes;
    // Capture the sequence of marker headers via jpeg_write_m_header
    struct HeaderEvent {
        int marker;
        unsigned int length;
    };
    std::vector<HeaderEvent> headers;
};

// Global mock context instance
static MockCInfo g_mockContext;

// A simple, C-compatible view of the mock context pointer.
// The production code passes a j_compress_ptr; our mock will interpret it as a pointer
// to MockCInfo.
static inline MockCInfo* toMock(j_compress_ptr cinfo) {
    return reinterpret_cast<MockCInfo*>(cinfo);
}

// Event types for verification (header vs. data byte)
enum EventType { EVT_HEADER, EVT_BYTE };

// A simple event record to verify the sequence of writes
struct WriteEvent {
    EventType type;
    int marker;                 // valid for HEADER events
    unsigned int length;        // valid for HEADER events
    uint8_t value;                // valid for BYTE events
};

// Container for all emitted events during a test
static std::vector<WriteEvent> g_events;

// Helper to reset test state
static void reset_test_state() {
    g_events.clear();
    g_test_failures = 0;
    // Clear mock context data (bytes) to avoid cross-test contamination
    g_mockContext.bytes.clear();
    g_mockContext.headers.clear();
}

// Helpers to report test failures without terminating the test run
static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "Test failure: " << msg << std::endl;
        ++g_test_failures;
    }
}

// Helper to fetch a single mock cinfo pointer
static j_compress_ptr get_mock_cinfo() {
    return reinterpret_cast<j_compress_ptr>(&g_mockContext);
}

// Mock implementations of the JPEG helper routines used by write_icc_profile
// These functions are declared in the project headers (via iccjpeg.h) and will be
// linked against the real implementation during test compilation.

extern "C" {

// Record a header event when an ICC marker header is written
void jpeg_write_m_header(j_compress_ptr cinfo, int marker, unsigned int length) {
    (void)cinfo; // unused in this mock; we could attach to per-marker if needed
    WriteEvent e;
    e.type = EVT_HEADER;
    e.marker = marker;
    e.length = length;
    g_events.push_back(e);

    // Also track header for deeper inspection if needed later
    MockCInfo* mock = toMock(cinfo);
    if (mock) {
        mock->headers.push_back({marker, length});
    }
}

// Record a data byte written as part of the ICC profile payload
void jpeg_write_m_byte(j_compress_ptr cinfo, int val) {
    (void)cinfo; // unused in this mock
    WriteEvent e;
    e.type = EVT_BYTE;
    e.value = static_cast<uint8_t>(val & 0xFF);
    g_events.push_back(e);

    // Also push to the raw bytes sequence for cross-check if desired
    MockCInfo* mock = toMock(cinfo);
    if (mock) {
        mock->bytes.push_back(static_cast<uint8_t>(val & 0xFF));
    }
}

} // extern "C"

// Domain knowledge constants (as used by the focal code)
#ifndef ICC_MARKER
#define ICC_MARKER 0xE2 // APP2 marker
#endif

#ifndef ICC_OVERHEAD_LEN
#define ICC_OVERHEAD_LEN 14 // 12 bytes "ICC_PROFILE" + 0x00 terminator + cur_marker + num_markers (2 bytes)
#endif

#ifndef MAX_DATA_BYTES_IN_MARKER
#define MAX_DATA_BYTES_IN_MARKER 4096 // A reasonable default; the exact value is not critical for single-marker tests
#endif

// Helper: verify the sequence for a single-marker write (icc_data_len = 1)
static void test_single_marker_write() {
    reset_test_state();

    // Prepare a 1-byte ICC profile payload
    uint8_t payload = 0xAA;
    j_compress_ptr cinfo = get_mock_cinfo();

    // Call the focal method under test
    write_icc_profile(cinfo, &payload, 1);

    // Expected: one marker header + 12 bytes for "ICC_PROFILE" + 0x00 terminator +
    // cur_marker (1) + num_markers (1) + 1 data byte (0xAA) = 1 + 12 + 2 + 1 = 16 events
    const size_t expected_events = 16;
    expect_true(g_events.size() == expected_events, "Single-marker: incorrect total event count");

    // Validate the marker header
    if (g_events.size() >= 1) {
        const WriteEvent &he = g_events[0];
        expect_true(he.type == EVT_HEADER, "Single-marker: first event should be HEADER");
        expect_true(he.marker == ICC_MARKER, "Single-marker: header marker should be ICC_MARKER");
        expect_true(he.length == static_cast<unsigned int>(1 + ICC_OVERHEAD_LEN),
                    "Single-marker: header length should be data length + overhead");
    }

    // Validate the ICC_PROFILE string bytes (12 bytes: "ICC_PROFILE" + 0x00)
    // The order is: 12 jpeg_write_m_byte calls following the header
    const uint8_t expected_profile_bytes[12] = {
        0x49, // 'I'
        0x43, // 'C'
        0x43, // 'C'
        0x5F, // '_'
        0x50, // 'P'
        0x52, // 'R'
        0x4F, // 'O'
        0x46, // 'F'
        0x49, // 'I'
        0x4C, // 'L'
        0x45, // 'E'
        0x00  // null terminator
    };

    // Ensure the next 12 events are the expected bytes
    for (size_t i = 0; i < 12; ++i) {
        const WriteEvent &we = g_events[1 + i];
        expect_true(we.type == EVT_BYTE, "Single-marker: profile data should be a BYTE event");
        expect_true(we.value == expected_profile_bytes[i],
                    "Single-marker: profile data byte mismatch at position " << i);
    }

    // The following two bytes are cur_marker and total number of markers
    const WriteEvent &curMarkerEvent = g_events[1 + 12];
    const WriteEvent &totalMarkerEvent = g_events[1 + 13];
    expect_true(curMarkerEvent.type == EVT_BYTE, "Single-marker: cur_marker should be a BYTE event");
    expect_true(totalMarkerEvent.type == EVT_BYTE, "Single-marker: num_markers should be a BYTE event");
    expect_true(curMarkerEvent.value == 1, "Single-marker: cur_marker should start at 1");
    expect_true(totalMarkerEvent.value == 1, "Single-marker: num_markers should be 1 for single chunk");

    // The final event should be the data payload 0xAA
    const WriteEvent &dataEvent = g_events[15];
    expect_true(dataEvent.type == EVT_BYTE, "Single-marker: final data should be a BYTE event");
    expect_true(dataEvent.value == payload, "Single-marker: final data payload mismatch");
}

// Helper: verify behavior with zero-length ICC data (no writes expected)
static void test_zero_length_input() {
    reset_test_state();

    j_compress_ptr cinfo = get_mock_cinfo();

    // Call with zero length
    uint8_t dummy = 0;
    write_icc_profile(cinfo, &dummy, 0);

    // Expect no events written
    expect_true(g_events.empty(), "Zero-length input should produce no writes");
}

// Run all tests
int main() {
    std::cout << "Starting write_icc_profile unit tests (without GTest)..." << std::endl;

    test_single_marker_write();
    test_zero_length_input();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
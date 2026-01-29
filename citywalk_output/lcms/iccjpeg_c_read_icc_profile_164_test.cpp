// C++11 test suite for read_icc_profile in iccjpeg.c
// This test uses a lightweight harness (no Google Test) and prints pass/fail results.
// It constructs fake jpeg_saved_marker_ptr marker_list structures to simulate ICC PROFILE markers
// and validates both success and various failure paths of read_icc_profile.

#include <cstring>
#include <iccjpeg.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>


// Import C declarations from the project's headers
extern "C" {
}

// Lightweight test assertion helper (non-terminating)
static void expect_true(bool cond, const char* msg, int &failures) {
    if (cond) {
        std::cout << "[PASS] " << msg << std::endl;
    } else {
        std::cerr << "[FAIL] " << msg << std::endl;
        ++failures;
    }
}

// Helper to free a chain of jpeg_saved_marker_struct markers
static void free_marker_chain(jpeg_saved_marker_ptr head) {
    while (head) {
        jpeg_saved_marker_ptr next = head->next;
        if (head->data) {
            free(head->data);
        }
        free(head);
        head = next;
    }
}

// Helper to construct an ICC marker with given sequence data and payload
static jpeg_saved_marker_ptr make_icc_marker(int seq, int total, const unsigned char* payload, unsigned int payload_len) {
    unsigned int overhead = ICC_OVERHEAD_LEN; // Provided by iccjpeg.h
    unsigned int data_len = overhead + payload_len;

    jpeg_saved_marker_ptr m = (jpeg_saved_marker_ptr) malloc(sizeof(*m));
    if (!m) return NULL;
    m->data_length = data_len;
    m->data = (JOCTET*) malloc(data_len);
    if (!m->data) {
        free(m);
        return NULL;
    }

    // Header: "ICC_PROFILE" + 0x00 + seq + total
    const char header[11] = { 'I','C','C','_','P','R','O','F','I','L','E' }; // 11 bytes
    for (size_t i = 0; i < 11; ++i) m->data[i] = (JOCTET) header[i];
    m->data[11] = 0;                 // terminator after header
    m->data[12] = (JOCTET) seq;      // sequence number
    m->data[13] = (JOCTET) total;    // total number of markers

    // Payload (data after ICC_OVERHEAD_LEN)
    for (unsigned int i = 0; i < payload_len; ++i) {
        m->data[overhead + i] = payload[i];
    }

    m->next = NULL;
    return m;
}

// Helper to construct a non-ICC marker (for tests where ICC markers are absent)
static jpeg_saved_marker_ptr make_nonicc_marker(int dummy, int total, const unsigned char* payload, unsigned int payload_len) {
    // We'll reuse the same structure but alter the header so marker_is_icc() returns false.
    unsigned int overhead = ICC_OVERHEAD_LEN; // Provided by iccjpeg.h
    unsigned int data_len = overhead + payload_len;

    jpeg_saved_marker_ptr m = (jpeg_saved_marker_ptr) malloc(sizeof(*m));
    if (!m) return NULL;
    m->data_length = data_len;
    m->data = (JOCTET*) malloc(data_len);
    if (!m->data) {
        free(m);
        return NULL;
    }

    // Non-ICC header: "NOTICC" (6 bytes) then pad to reach header size
    const char header[] = { 'N','O','T','I','C','C' }; // 6 bytes
    for (size_t i = 0; i < 6; ++i) m->data[i] = (JOCTET) header[i];
    // Pad remaining header bytes to a consistent length (fill with 'X')
    for (size_t i = 6; i < 11; ++i) m->data[i] = (JOCTET) 'X';
    m->data[11] = 0;                // terminator
    m->data[12] = (JOCTET) 0;         // seq (not used for non-ICC)
    m->data[13] = (JOCTET) 0;         // total (not used)

    // Payload
    for (unsigned int i = 0; i < payload_len; ++i) {
        m->data[overhead + i] = payload[i];
    }

    m->next = NULL;
    return m;
}

// Test 1: Successful assembly from 2 ICC markers
static void test_read_icc_profile_success(int &failures) {
    // Payloads for two markers
    unsigned char p1[] = { 'A', 'A', 'A' }; // 3 bytes
    unsigned int p1_len = sizeof(p1);
    unsigned char p2[] = { 'B', 'B' };      // 2 bytes
    unsigned int p2_len = sizeof(p2);

    jpeg_saved_marker_ptr m1 = make_icc_marker(1, 2, p1, p1_len);
    jpeg_saved_marker_ptr m2 = make_icc_marker(2, 2, p2, p2_len);
    if (!m1 || !m2) {
        expect_true(false, "Test 1: marker allocation failed", failures);
        free_marker_chain(m1);
        free_marker_chain(m2);
        return;
    }
    m1->next = m2;

    jpeg_decompress_struct cinfo_struct;
    std::memset(&cinfo_struct, 0, sizeof(cinfo_struct));
    cinfo_struct.marker_list = m1;
    j_decompress_ptr cinfo = &cinfo_struct;

    JOCTET* icc_data_ptr = NULL;
    unsigned int icc_data_len = 0;

    int res = read_icc_profile(cinfo, &icc_data_ptr, &icc_data_len);

    unsigned char expected[5] = { 'A','A','A','B','B' };
    bool ok = (res != 0) && (icc_data_ptr != NULL) && (icc_data_len == 5) && (std::memcmp(icc_data_ptr, expected, 5) == 0);
    expect_true(ok, "Test 1: successful ICC data assembly", failures);

    if (icc_data_ptr) {
        free(icc_data_ptr);
    }
    free_marker_chain(m1);
    free_marker_chain(m2);
}

// Test 2: Inconsistent num_markers across ICC markers -> FALSE
static void test_read_icc_profile_inconsistent_num(int &failures) {
    unsigned char p1[] = { 'X' }; // 1 byte
    jpeg_saved_marker_ptr m1 = make_icc_marker(1, 2, p1, sizeof(p1));
    unsigned char p2[] = { 'Y' };
    jpeg_saved_marker_ptr m2 = make_icc_marker(2, 3, p2, sizeof(p2)); // inconsistent: num_markers = 3
    if (!m1 || !m2) {
        expect_true(false, "Test 2: marker allocation failed", failures);
        free_marker_chain(m1);
        free_marker_chain(m2);
        return;
    }
    m1->next = m2;

    jpeg_decompress_struct cinfo_struct;
    std::memset(&cinfo_struct, 0, sizeof(cinfo_struct));
    cinfo_struct.marker_list = m1;
    j_decompress_ptr cinfo = &cinfo_struct;

    JOCTET* icc_data_ptr = NULL;
    unsigned int icc_data_len = 0;

    int res = read_icc_profile(cinfo, &icc_data_ptr, &icc_data_len);

    bool ok = (res == 0) && (icc_data_ptr == NULL);
    expect_true(ok, "Test 2: inconsistent num_markers should fail", failures);

    free_marker_chain(m1);
    free_marker_chain(m2);
}

// Test 3: Missing one of the ICC markers -> FALSE
static void test_read_icc_profile_missing_marker(int &failures) {
    unsigned char p1[] = { 'M' };
    jpeg_saved_marker_ptr m1 = make_icc_marker(1, 2, p1, sizeof(p1));
    if (!m1) {
        expect_true(false, "Test 3: marker allocation failed", failures);
        return;
    }
    // Only one marker provided; second marker missing

    jpeg_decompress_struct cinfo_struct;
    std::memset(&cinfo_struct, 0, sizeof(cinfo_struct));
    cinfo_struct.marker_list = m1;
    j_decompress_ptr cinfo = &cinfo_struct;

    JOCTET* icc_data_ptr = NULL;
    unsigned int icc_data_len = 0;

    int res = read_icc_profile(cinfo, &icc_data_ptr, &icc_data_len);

    bool ok = (res == 0) && (icc_data_ptr == NULL);
    expect_true(ok, "Test 3: missing marker should fail", failures);

    free_marker_chain(m1);
}

// Test 4: Duplicate sequence number -> FALSE
static void test_read_icc_profile_duplicate_seq(int &failures) {
    unsigned char p1[] = { 'D' };
    jpeg_saved_marker_ptr m1 = make_icc_marker(1, 2, p1, sizeof(p1));
    unsigned char p2[] = { 'D' };
    // Duplicate seq_no: 1
    jpeg_saved_marker_ptr m2 = make_icc_marker(1, 2, p2, sizeof(p2));
    if (!m1 || !m2) {
        expect_true(false, "Test 4: marker allocation failed", failures);
        free_marker_chain(m1);
        free_marker_chain(m2);
        return;
    }
    m1->next = m2;

    jpeg_decompress_struct cinfo_struct;
    std::memset(&cinfo_struct, 0, sizeof(cinfo_struct));
    cinfo_struct.marker_list = m1;
    j_decompress_ptr cinfo = &cinfo_struct;

    JOCTET* icc_data_ptr = NULL;
    unsigned int icc_data_len = 0;

    int res = read_icc_profile(cinfo, &icc_data_ptr, &icc_data_len);

    bool ok = (res == 0) && (icc_data_ptr == NULL);
    expect_true(ok, "Test 4: duplicate seq numbers should fail", failures);

    free_marker_chain(m1);
    free_marker_chain(m2);
}

// Test 5: No ICC markers present -> FALSE
static void test_read_icc_profile_no_icc_markers(int &failures) {
    unsigned char payload[] = { 'N' };
    // Make markers that are not ICC_PROFILE; marker_is_icc should return false
    jpeg_saved_marker_ptr m1 = make_nonicc_marker(0, 1, payload, sizeof(payload));
    if (!m1) {
        expect_true(false, "Test 5: non-ICC marker allocation failed", failures);
        return;
    }

    jpeg_decompress_struct cinfo_struct;
    std::memset(&cinfo_struct, 0, sizeof(cinfo_struct));
    cinfo_struct.marker_list = m1;
    j_decompress_ptr cinfo = &cinfo_struct;

    JOCTET* icc_data_ptr = NULL;
    unsigned int icc_data_len = 0;

    int res = read_icc_profile(cinfo, &icc_data_ptr, &icc_data_len);

    bool ok = (res == 0) && (icc_data_ptr == NULL);
    expect_true(ok, "Test 5: no ICC markers should fail", failures);

    free_marker_chain(m1);
}

// Test 6: Total ICC data length becomes zero (all markers empty) -> FALSE
static void test_read_icc_profile_zero_total_length(int &failures) {
    unsigned char p1[] = {}; // empty payload
    jpeg_saved_marker_ptr m1 = make_icc_marker(1, 2, p1, 0);
    if (!m1) {
        expect_true(false, "Test 6: marker allocation failed", failures);
        return;
    }

    jpeg_decompress_struct cinfo_struct;
    std::memset(&cinfo_struct, 0, sizeof(cinfo_struct));
    cinfo_struct.marker_list = m1;
    j_decompress_ptr cinfo = &cinfo_struct;

    JOCTET* icc_data_ptr = NULL;
    unsigned int icc_data_len = 0;

    int res = read_icc_profile(cinfo, &icc_data_ptr, &icc_data_len);

    bool ok = (res == 0) && (icc_data_ptr == NULL);
    expect_true(ok, "Test 6: zero total ICC data should fail", failures);

    free_marker_chain(m1);
}

int main() {
    int failures = 0;

    std::cout << "Starting read_icc_profile unit tests (C++ harness)" << std::endl;

    test_read_icc_profile_success(failures);
    test_read_icc_profile_inconsistent_num(failures);
    test_read_icc_profile_missing_marker(failures);
    test_read_icc_profile_duplicate_seq(failures);
    test_read_icc_profile_no_icc_markers(failures);
    test_read_icc_profile_zero_total_length(failures);

    if (failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
    } else {
        std::cerr << failures << " tests FAILED." << std::endl;
    }

    // Exit with 0 if all tests passed; otherwise non-zero
    return (failures == 0) ? 0 : 1;
}
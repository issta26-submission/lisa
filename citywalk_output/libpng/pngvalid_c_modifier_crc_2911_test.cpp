#include <cstdint>
#include <vector>
#include <signal.h>
#include <string.h>
#include <zlib.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Declare the focal function from pngvalid.c (C linkage)
extern "C" void modifier_crc(unsigned char* buffer);

// Helper: read a big-endian 32-bit value from buffer
static uint32_t read_be32(const unsigned char* p) {
    return (static_cast<uint32_t>(p[0]) << 24) |
           (static_cast<uint32_t>(p[1]) << 16) |
           (static_cast<uint32_t>(p[2]) << 8)  |
           (static_cast<uint32_t>(p[3]));
}

// Helper: write a big-endian 32-bit value to buffer
static void write_be32(unsigned char* p, uint32_t v) {
    p[0] = static_cast<unsigned char>((v >> 24) & 0xFF);
    p[1] = static_cast<unsigned char>((v >> 16) & 0xFF);
    p[2] = static_cast<unsigned char>((v >> 8) & 0xFF);
    p[3] = static_cast<unsigned char>(v & 0xFF);
}

// Test 1: Zero-length data chunk
// - Purpose: Validate CRC is computed over chunk type only when datalen == 0.
// - Coverage: verifies correct computation path for datalen = 0 and proper storage position.
static bool test_modifier_crc_zero_length() {
    // Layout: [length(4)] [type(4)] [data(0)] [CRC(4)]
    const uint32_t data_len = 0;
    const size_t total_size = 4 + 4 + data_len + 4; // 12
    std::vector<unsigned char> buffer(total_size, 0);

    // Fill length (big-endian)
    write_be32(buffer.data(), data_len);
    // Chunk type "TEST"
    buffer[4] = 'T'; buffer[5] = 'E'; buffer[6] = 'S'; buffer[7] = 'T';
    // CRC field initially set to a known value to ensure it is overwritten
    buffer[8] = 0xAA; buffer[9] = 0xBB; buffer[10] = 0xCC; buffer[11] = 0xDD;

    // Call the function under test
    modifier_crc(buffer.data());

    // Expected CRC is crc32 over buffer+4, length datalen+4 == 4
    uint32_t expected_crc = static_cast<uint32_t>(
        crc32(0, buffer.data() + 4, data_len + 4)
    );

    uint32_t written_crc = read_be32(buffer.data() + 8); // CRC location
    bool pass = (expected_crc == written_crc);

    // Additional sanity: bytes before CRC should remain unchanged (length+type)
    bool prefix_unchanged = true;
    const unsigned char expected_prefix[8] = { buffer[0], buffer[1], buffer[2], buffer[3],
                                             buffer[4], buffer[5], buffer[6], buffer[7] };
    // Recompute current prefix to compare with saved snapshot
    unsigned char current_prefix[8];
    std::memcpy(current_prefix, buffer.data(), 8);
    prefix_unchanged = (std::memcmp(expected_prefix, current_prefix, 8) == 0);

    if (!pass) {
        std::cerr << "test_modifier_crc_zero_length: expected CRC " << expected_crc
                  << " but got " << written_crc << std::endl;
    }
    if (!prefix_unchanged) {
        std::cerr << "test_modifier_crc_zero_length: prefix bytes modified unexpectedly." << std::endl;
    }

    return pass && prefix_unchanged;
}

// Test 2: Non-zero data length (small payload)
// - Purpose: Validate CRC correctness for non-empty data payload.
// - Coverage: covers typical path with datalen > 0.
static bool test_modifier_crc_nonzero_length() {
    const uint32_t data_len = 5;
    const size_t total_size = 4 + 4 + data_len + 4; // 17
    std::vector<unsigned char> buffer(total_size, 0);

    // Fill length (big-endian)
    write_be32(buffer.data(), data_len);
    // Chunk type "DATA"
    buffer[4] = 'D'; buffer[5] = 'A'; buffer[6] = 'T'; buffer[7] = 'A';
    // Fill data with deterministic pattern
    for (uint32_t i = 0; i < data_len; ++i) {
        buffer[8 + i] = static_cast<unsigned char>(i + 1);
    }
    // CRC field initially zero
    buffer[8 + data_len] = 0;
    buffer[8 + data_len + 1] = 0;
    buffer[8 + data_len + 2] = 0;
    buffer[8 + data_len + 3] = 0;

    // Call the function under test
    modifier_crc(buffer.data());

    // Expected CRC is crc32 over buffer+4, length datalen+4
    uint32_t expected_crc = static_cast<uint32_t>(
        crc32(0, buffer.data() + 4, data_len + 4)
    );

    uint32_t written_crc = read_be32(buffer.data() + 8 + data_len);
    bool pass = (expected_crc == written_crc);

    // Ensure data portion remains intact
    bool data_intact = true;
    for (uint32_t i = 0; i < data_len; ++i) {
        if (buffer[8 + i] != static_cast<unsigned char>(i + 1)) { data_intact = false; break; }
    }

    if (!pass) {
        std::cerr << "test_modifier_crc_nonzero_length: expected CRC " << expected_crc
                  << " but got " << written_crc << std::endl;
    }
    if (!data_intact) {
        std::cerr << "test_modifier_crc_nonzero_length: data payload was modified." << std::endl;
    }

    return pass && data_intact;
}

// Test 3: Buffer overrun protection (extra trailing space should remain untouched)
// - Purpose: Ensure function does not write beyond the CRC field.
// - Coverage: verifies boundary safety beyond required 4 CRC bytes.
static bool test_modifier_crc_no_overflow() {
    const uint32_t data_len = 3;
    const size_t extra_tail = 8; // extra guard space after CRC
    const size_t total_size = 4 + 4 + data_len + 4 + extra_tail;
    std::vector<unsigned char> buffer(total_size, 0xFF); // fill with 0xFF to detect changes

    // Initialize known prefix
    // Fill length
    write_be32(buffer.data(), data_len);
    // Type
    buffer[4] = 'O'; buffer[5] = 'V'; buffer[6] = 'R'; buffer[7] = 'N';
    // Data
    for (uint32_t i = 0; i < data_len; ++i) buffer[8 + i] = static_cast<unsigned char>(i + 0x10);
    // CRC placeholder
    buffer[8 + data_len] = 0;
    buffer[8 + data_len + 1] = 0;
    buffer[8 + data_len + 2] = 0;
    buffer[8 + data_len + 3] = 0;
    // Trailing guard bytes are 0xFF

    modifier_crc(buffer.data());

    uint32_t expected_crc = static_cast<uint32_t>(crc32(0, buffer.data() + 4, data_len + 4));
    uint32_t written_crc = read_be32(buffer.data() + 8 + data_len);

    bool pass = (expected_crc == written_crc);

    // Check trailing bytes untouched (still 0xFF)
    bool trailing_guard_ok = true;
    for (size_t i = 0; i < extra_tail; ++i) {
        if (buffer[total_size - 1 - i] != 0xFF) { trailing_guard_ok = false; break; }
    }

    if (!pass) {
        std::cerr << "test_modifier_crc_no_overflow: expected CRC " << expected_crc
                  << " but got " << written_crc << std::endl;
    }
    if (!trailing_guard_ok) {
        std::cerr << "test_modifier_crc_no_overflow: trailing guard bytes modified (overflow suspected)." << std::endl;
    }

    return pass && trailing_guard_ok;
}

// Test runner
static void run_all_tests() {
    int passed = 0;
    int failed = 0;

    std::cout << "Running test: test_modifier_crc_zero_length" << std::endl;
    if (test_modifier_crc_zero_length()) {
        std::cout << "[PASSED] test_modifier_crc_zero_length" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAILED] test_modifier_crc_zero_length" << std::endl;
        ++failed;
    }

    std::cout << "Running test: test_modifier_crc_nonzero_length" << std::endl;
    if (test_modifier_crc_nonzero_length()) {
        std::cout << "[PASSED] test_modifier_crc_nonzero_length" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAILED] test_modifier_crc_nonzero_length" << std::endl;
        ++failed;
    }

    std::cout << "Running test: test_modifier_crc_no_overflow" << std::endl;
    if (test_modifier_crc_no_overflow()) {
        std::cout << "[PASSED] test_modifier_crc_no_overflow" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAILED] test_modifier_crc_no_overflow" << std::endl;
        ++failed;
    }

    std::cout << "\nTest Summary: " << passed << " passed, " << failed << " failed." << std::endl;
}

int main() {
    run_all_tests();
    return 0;
}
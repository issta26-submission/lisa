#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


extern "C" {
    // Declaration of the focal function under test.
    // Assumes gzip-decode capable environment provided by the project (gzjoin.c).
    void gzcopy(char *name, int clr, unsigned long *crc, unsigned long *tot, FILE *out);
}

/*
 Lightweight test harness (no Google Test). 
 - Keeps tests non-terminating: on failure, logs and continues.
 - Uses a tiny helper set to compress data into a valid gzip file using zlib.
 - Verifies:
   1) For clr = 0 (with trailer): output data equals original payload and trailer CRC/size match.
   2) For clr = 1 (no trailer): output data equals original payload and no trailer appended.
 - All tests are self-contained and rely only on C++11 standard library and zlib.
*/

// Utility: read entire file into a string
static std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return "";
    in.seekg(0, std::ios::end);
    std::size_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::string data(size, '\0');
    in.read(&data[0], size);
    return data;
}

// Utility: get file size
static std::size_t file_size(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return 0;
    return static_cast<std::size_t>(st.st_size);
}

// Utility: write a gzip file from in-memory data using zlib
static bool compress_to_gzip(const std::string& data, const std::string& path) {
    // gzopen/gzwrite/gzclose from zlib
    gzFile gz = gzopen(path.c_str(), "wb9"); // best compression
    if (!gz) return false;
    std::size_t total = 0;
    while (total < data.size()) {
        int written = gzwrite(gz, data.data() + total, static_cast<unsigned int>(data.size() - total));
        if (written <= 0) {
            gzclose(gz);
            return false;
        }
        total += static_cast<std::size_t>(written);
    }
    int ret = gzclose(gz);
    return ret == Z_OK;
}

// Test 1: Round-trip decompression with trailer (clr = 0)
// - Create a gzip file containing payload
// - Run gzcopy with clr = 0 and capture output
// - Verify output payload matches original data
// - Verify trailer is present and matches CRC and total length
static bool test_gzcopy_roundtrip_with_trailer() {
    const std::string payload = "The quick brown fox jumps over the lazy dog";
    const std::string gzipPath = "test_input_roundtrip.gz";
    const std::string outPath  = "out_roundtrip.bin";

    // Compress payload into a gzip file
    if (!compress_to_gzip(payload, gzipPath)) {
        std::cerr << "TEST: Failed to create test gzip file for roundtrip.\n";
        return false;
    }

    // Prepare output
    FILE* out = fopen(outPath.c_str(), "wb");
    if (!out) {
        std::cerr << "TEST: Failed to open output file for roundtrip.\n";
        return false;
    }

    unsigned long crc = 0;
    unsigned long tot = 0;
    // Call focal method with clr = 0 (expect trailer)
    gzcopy(const_cast<char*>(gzipPath.c_str()), 0, &crc, &tot, out);
    fclose(out);

    // Read output and compare payload
    std::string outputData = read_file(outPath);
    bool pass = true;
    if (outputData != payload) {
        std::cerr << "TEST: Roundtrip with trailer - payload mismatch.\n";
        pass = false;
    }

    // Validate trailer: last 8 bytes should be CRC (4 bytes) + total (4 bytes) in little-endian
    std::size_t size = file_size(outPath);
    if (size < payload.size() + 8) {
        std::cerr << "TEST: Roundtrip with trailer - output too small for trailer.\n";
        pass = false;
    } else {
        std::ifstream in(outPath, std::ios::binary);
        in.seekg(-8, std::ios::end);
        unsigned char trailer[8];
        in.read(reinterpret_cast<char*>(trailer), 8);
        unsigned long trailer_crc = trailer[0] | (trailer[1] << 8) | (trailer[2] << 16) | (trailer[3] << 24);
        unsigned long trailer_tot = trailer[4] | (trailer[5] << 8) | (trailer[6] << 16) | (trailer[7] << 24);
        in.close();

        unsigned long expected_crc = crc32(0L, reinterpret_cast<const Bytef*>(payload.data()), static_cast<uInt>(payload.size()));
        unsigned long expected_tot = static_cast<unsigned long>(payload.size());

        if (trailer_crc != expected_crc) {
            std::cerr << "TEST: Roundtrip with trailer - CRC mismatch. expected " << expected_crc
                      << " got " << trailer_crc << ".\n";
            pass = false;
        }
        if (trailer_tot != expected_tot) {
            std::cerr << "TEST: Roundtrip with trailer - total mismatch. expected "
                      << expected_tot << " got " << trailer_tot << ".\n";
            pass = false;
        }
    }

    // Cleanup
    remove(gzipPath.c_str());
    remove(outPath.c_str());

    return pass;
}

// Test 2: No trailer when clr = 1
// - Create a gzip file containing payload
// - Run gzcopy with clr = 1 and capture output
// - Verify output payload matches original data
// - Verify trailer is NOT present (output size should equal payload size)
static bool test_gzcopy_no_trailer_clr_true() {
    const std::string payload = "Sphinx of black quartz, judge my vow";
    const std::string gzipPath = "test_input_notrailer.gz";
    const std::string outPath  = "out_notrailer.bin";

    if (!compress_to_gzip(payload, gzipPath)) {
        std::cerr << "TEST: Failed to create test gzip file for no-trailer scenario.\n";
        return false;
    }

    FILE* out = fopen(outPath.c_str(), "wb");
    if (!out) {
        std::cerr << "TEST: Failed to open output file for no-trailer scenario.\n";
        return false;
    }

    unsigned long crc = 0;
    unsigned long tot = 0;
    gzcopy(const_cast<char*>(gzipPath.c_str()), 1, &crc, &tot, out);
    fclose(out);

    // Read output and compare payload
    std::string outputData = read_file(outPath);
    bool pass = true;
    if (outputData != payload) {
        std::cerr << "TEST: No-trailer with clr=1 - payload mismatch.\n";
        pass = false;
    }

    // Verify that trailer is not present by checking file size
    std::size_t size = file_size(outPath);
    if (size != payload.size()) {
        std::cerr << "TEST: No-trailer with clr=1 - trailer unexpectedly present. size: "
                  << size << "\n";
        pass = false;
    }

    // Cleanup
    remove(gzipPath.c_str());
    remove(outPath.c_str());

    return pass;
}

// Simple test runner
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running gzcopy unit test suite (C++11, no GTest)...\n";

    bool t1 = test_gzcopy_roundtrip_with_trailer();
    total++; if (t1) { ++passed; std::cout << "[PASS] test_gzcopy_roundtrip_with_trailer\n"; } else {
        std::cout << "[FAIL] test_gzcopy_roundtrip_with_trailer\n";
    }

    bool t2 = test_gzcopy_no_trailer_clr_true();
    total++; if (t2) { ++passed; std::cout << "[PASS] test_gzcopy_no_trailer_clr_true\n"; } else {
        std::cout << "[FAIL] test_gzcopy_no_trailer_clr_true\n";
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}
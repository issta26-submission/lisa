// Minimal C++11 test harness for the C function unzRepair (no GTest, self-contained)
#include <cstdio>
#include <cstring>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>
#include <sstream>
#include <cstdlib>


// Include zlib types for Z_OK, Z_STREAM_ERROR, and uLong.
// The focal function is declared with C linkage; declare the prototype accordingly.
extern "C" int ZEXPORT unzRepair(const char* file, const char* fileOut, const char* fileOutTmp, uLong* nRecovered, uLong* bytesRecovered);

// Simple test harness with non-terminating assertions
static int g_failed = 0;

static std::string toString(long long v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_failed; \
        } \
    } while (0)

#define ASSERT_EQ(expected, actual) \
    do { \
        auto _e = (expected); \
        auto _a = (actual); \
        if (!(_e == _a)) { \
            std::cerr << "Assertion failed: expected " << toString(_e) << " == actual " << toString(_a) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_failed; \
        } \
    } while (0)

static void writeHeaderForTest1(std::string path) {
    // Construct a minimal local file header for one entry:
    // Signature: 0x04034b50 -> bytes: 50 4B 03 04
    // version=20 (0x14 0x00), gpflag=0, method=0, times/dates=0, crc=0, cpsize=5, uncpsize=5, fnsize=4, extsize=0
    unsigned char header[30] = {0};
    header[0] = 0x50; header[1] = 0x4B; header[2] = 0x03; header[3] = 0x04; // signature
    header[4] = 0x14; header[5] = 0x00; // version
    header[6] = 0x00; header[7] = 0x00; // gpflag
    header[8] = 0x00; header[9] = 0x00; // method
    header[10] = 0x00; header[11] = 0x00; // filetime
    header[12] = 0x00; header[13] = 0x00; // filedate
    header[14] = 0x00; header[15] = 0x00; header[16] = 0x00; header[17] = 0x00; // crc
    header[18] = 0x05; header[19] = 0x00; header[20] = 0x00; header[21] = 0x00; // cpsize=5
    header[22] = 0x05; header[23] = 0x00; header[24] = 0x00; header[25] = 0x00; // uncpsize=5
    header[26] = 0x04; header[27] = 0x00; // fnsize=4
    header[28] = 0x00; header[29] = 0x00; // extsize=0

    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return;
    fwrite(header, 1, 30, f);
    const char* filename = "test";
    fwrite(filename, 1, 4, f); // fnsize=4
    const char* data = "abcde";
    fwrite(data, 1, 5, f); // data of size 5
    fclose(f);
}

// Creates a valid header with fnsize set to 1024 to trigger Z_STREAM_ERROR in test 2
static void writeHeaderForTest2(std::string path) {
    unsigned char header[30] = {0};
    header[0] = 0x50; header[1] = 0x4B; header[2] = 0x03; header[3] = 0x04;
    header[4] = 0x14; header[5] = 0x00;
    header[6] = 0x00; header[7] = 0x00;
    header[8] = 0x00; header[9] = 0x00;
    header[10] = 0x00; header[11] = 0x00;
    header[12] = 0x00; header[13] = 0x00;
    header[14] = 0x00; header[15] = 0x00; header[16] = 0x00; header[17] = 0x00;
    header[18] = 0x05; header[19] = 0x00; header[20] = 0x00; header[21] = 0x00;
    header[22] = 0x05; header[23] = 0x00; header[24] = 0x00; header[25] = 0x00;
    header[26] = 0x00; header[27] = 0x04; // fnsize set to 0x0400 = 1024 (little-endian 0x00 0x04)
    header[28] = 0x00; header[29] = 0x00; // extsize

    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return;
    fwrite(header, 1, 30, f);
    // Do not write filename or data to keep the test focused on the trigger
    fclose(f);
}

static void test_valid_single_entry() {
    // Prepare input zip-like file with a single entry
    const std::string inPath = "unzRepair_test1_input.zip";
    writeHeaderForTest1(inPath);

    const std::string outPath = "unzRepair_test1_output.bin";
    const std::string tmpPath = "unzRepair_test1_tmp.bin";

    // Ensure no leftovers
    remove(outPath.c_str());
    remove(tmpPath.c_str());

    uLong nRec = 0;
    uLong bytesRec = 0;

    int ret = unzRepair(inPath.c_str(), outPath.c_str(), tmpPath.c_str(), &nRec, &bytesRec);

    // Validate expected outcomes
    ASSERT_EQ(Z_OK, ret);
    ASSERT_EQ((uLong)1, nRec);
    ASSERT_EQ((uLong)5, bytesRec);

    // Basic sanity: output file should exist and be non-empty
    FILE* f = fopen(outPath.c_str(), "rb");
    ASSERT_TRUE(f != NULL, "Output file should be created on success");
    if (f) {
        fseek(f, 0, SEEK_END);
        long sz = ftell(f);
        fclose(f);
        ASSERT_TRUE(sz > 0, "Output file should have non-zero size");
    }

    // Cleanup
    remove(inPath.c_str());
    remove(outPath.c_str());
    remove(tmpPath.c_str());
}

static void test_fnsize_too_large_trigger_error() {
    // Prepare input with fnsize = 1024 (0x0400 little-endian -> bytes 0x00 0x04)
    const std::string inPath = "unzRepair_test2_input.bin";
    writeHeaderForTest2(inPath);

    const std::string outPath = "unzRepair_test2_output.bin";
    const std::string tmpPath = "unzRepair_test2_tmp.bin";

    remove(outPath.c_str());
    remove(tmpPath.c_str());

    uLong nRec = 0;
    uLong bytesRec = 0;

    int ret = unzRepair(inPath.c_str(), outPath.c_str(), tmpPath.c_str(), &nRec, &bytesRec);

    // Expect a stream error due to header violation
    ASSERT_EQ(Z_STREAM_ERROR, ret);

    // Cleanup
    remove(inPath.c_str());
    remove(outPath.c_str());
    remove(tmpPath.c_str());
}

static void test_missing_input_file() {
    // Non-existent input should yield Z_STREAM_ERROR
    const std::string inPath = "unzRepair_nonexistent_input.zip";
    const std::string outPath = "unzRepair_test3_output.bin";
    const std::string tmpPath = "unzRepair_test3_tmp.bin";

    remove(outPath.c_str());
    remove(tmpPath.c_str());

    uLong nRec = 0;
    uLong bytesRec = 0;

    int ret = unzRepair(inPath.c_str(), outPath.c_str(), tmpPath.c_str(), &nRec, &bytesRec);

    ASSERT_EQ(Z_STREAM_ERROR, ret);

    remove(outPath.c_str());
    remove(tmpPath.c_str());
}

static void test_invalid_output_path() {
    // Use a valid input but a bad output path to trigger error on fpOut creation
    const std::string inPath = "unzRepair_test1_input_for_invalid_output.bin";
    writeHeaderForTest1(inPath);

    // Intentionally invalid path (directory does not exist)
#if defined(_WIN32) || defined(_WIN64)
    const std::string outPath = "nonexistent_dir\\unzRepair_output.bin";
#else
    const std::string outPath = "nonexistent_dir/unzRepair_output.bin";
#endif
    const std::string tmpPath = "unzRepair_test_invalid_tmp.bin";

    remove(outPath.c_str());
    remove(tmpPath.c_str());

    uLong nRec = 0;
    uLong bytesRec = 0;

    int ret = unzRepair(inPath.c_str(), outPath.c_str(), tmpPath.c_str(), &nRec, &bytesRec);

    ASSERT_EQ(Z_STREAM_ERROR, ret);

    remove(inPath.c_str());
    remove(tmpPath.c_str());
}

int main() {
    std::cout << "Running unzRepair unit tests (custom harness, no GTest)..." << std::endl;

    test_valid_single_entry();
    test_fnsize_too_large_trigger_error();
    test_missing_input_file();
    test_invalid_output_path();

    if (g_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failed << " test(s) failed." << std::endl;
        return 1;
    }
}
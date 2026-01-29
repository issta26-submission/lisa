// C++11 test suite for the C function: zipRemoveExtraInfoBlock
// This test does not rely on GTest. It uses a small custom test harness.
// It exercises true/false branches and edge cases described in the problem statement.

#include <cstring>
#include <string.h>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


extern "C" {
// Also ensures the C linkage for the function under test
}

// If the header does not declare the function with C linkage, declare it explicitly here.
extern "C" int zipRemoveExtraInfoBlock(char* pData, int* dataLen, short sHeader);

// Simple non-terminating test reporter
static void report(const std::string& name, bool passed, int& failures) {
    if (passed) {
        std::cout << "PASS: " << name << std::endl;
    } else {
        std::cout << "FAIL: " << name << std::endl;
        ++failures;
    }
}

// Helper to interpret byte sequences in little-endian order
static void pushShortLE(std::vector<char>& buf, uint16_t v) {
    buf.push_back(static_cast<char>(v & 0xFF));
    buf.push_back(static_cast<char>((v >> 8) & 0xFF));
}

int main() {
    int failures = 0;

    // Test 1: NULL pData should return ZIP_PARAMERROR
    {
        int dummyLen = 4;
        int ret = zipRemoveExtraInfoBlock(nullptr, &dummyLen, 0x1234);
        bool ok = (ret == ZIP_PARAMERROR);
        report("zipRemoveExtraInfoBlock: NULL pData returns ZIP_PARAMERROR", ok, failures);
    }

    // Test 2: NULL dataLen should return ZIP_PARAMERROR
    {
        char dummy[4] = {0, 0, 0, 0};
        int ret = zipRemoveExtraInfoBlock(dummy, nullptr, 0x1234);
        bool ok = (ret == ZIP_PARAMERROR);
        report("zipRemoveExtraInfoBlock: NULL dataLen returns ZIP_PARAMERROR", ok, failures);
    }

    // Test 3: dataLen < 4 should return ZIP_PARAMERROR
    {
        // Prepare a tiny buffer of 3 bytes
        std::vector<char> buf(3, 0);
        int len = 3;
        int ret = zipRemoveExtraInfoBlock(buf.data(), &len, 0x1234);
        bool ok = (ret == ZIP_PARAMERROR);
        report("zipRemoveExtraInfoBlock: *dataLen < 4 returns ZIP_PARAMERROR", ok, failures);
    }

    // Test 4: Remove a single header block; one block matches sHeader, one does not
    {
        // sHeader = 0x1234
        const uint16_t sHeader = 0x1234;

        std::vector<char> data;

        // Block 1: header = sHeader, size = 2, data = "AB" (to be removed)
        pushShortLE(data, sHeader);
        pushShortLE(data, 2);
        data.push_back('A');
        data.push_back('B');

        // Block 2: header = 0x5678, size = 3, data = "123" (to be kept)
        pushShortLE(data, 0x5678);
        pushShortLE(data, 3);
        data.push_back('1');
        data.push_back('2');
        data.push_back('3');

        int dataLen = static_cast<int>(data.size());
        int ret = zipRemoveExtraInfoBlock(data.data(), &dataLen, sHeader);

        // Expected: Only Block 2 remains; new length = 7; content = [0x78,0x56,0x03,0x00,'1','2','3']
        bool ok = true;
        if (ret != ZIP_OK) ok = false;
        if (dataLen != 7) ok = false;
        // Expected kept block bytes (little-endian header)
        unsigned char expected[7] = { 0x78, 0x56, 0x03, 0x00, '1', '2', '3' };
        if (memcmp(data.data(), expected, 7) != 0) ok = false;

        report("zipRemoveExtraInfoBlock: remove a single matching header block", ok, failures);
    }

    // Test 5: All blocks removed (size becomes 0); input buffer entirely cleared to zeros
    {
        // sHeader = 0x1234
        const uint16_t sHeader = 0x1234;

        std::vector<char> data;

        // Block 1: header = sHeader, size = 2, data = "AA"
        pushShortLE(data, sHeader);
        pushShortLE(data, 2);
        data.push_back('A');
        data.push_back('A');

        // Block 2: header = sHeader, size = 2, data = "BB"
        pushShortLE(data, sHeader);
        pushShortLE(data, 2);
        data.push_back('B');
        data.push_back('B');

        int dataLen = static_cast<int>(data.size());
        int ret = zipRemoveExtraInfoBlock(data.data(), &dataLen, sHeader);

        bool ok = (ret == ZIP_OK) && (dataLen == 0);
        // All bytes should be zeros after operation
        for (size_t i = 0; ok && i < data.size(); ++i) {
            if (data[i] != 0) ok = false;
        }

        report("zipRemoveExtraInfoBlock: remove all blocks results in dataLen 0 and buffer zeroed", ok, failures);
    }

    // Test 6: No blocks removed (none match sHeader); ret should be ZIP_ERRNO and data unchanged
    {
        // sHeader = 0x1234
        const uint16_t sHeader = 0x1234;

        std::vector<char> data;

        // Block 1: header = 0x1111, size = 2, data = "AB"
        pushShortLE(data, 0x1111);
        pushShortLE(data, 2);
        data.push_back('A');
        data.push_back('B');

        // Block 2: header = 0x2222, size = 2, data = "CD"
        pushShortLE(data, 0x2222);
        pushShortLE(data, 2);
        data.push_back('C');
        data.push_back('D');

        std::vector<char> original = data; // copy for later comparison
        int dataLen = static_cast<int>(data.size());

        int ret = zipRemoveExtraInfoBlock(data.data(), &dataLen, sHeader);

        bool ok = (ret == ZIP_ERRNO) && (dataLen == static_cast<int>(original.size()));
        if (ok) {
            if (memcmp(data.data(), original.data(), original.size()) != 0) {
                ok = false;
            }
        }

        report("zipRemoveExtraInfoBlock: no blocks removed (no matching header) leaves data unchanged and returns ZIP_ERRNO", ok, failures);
    }

    // Summary
    if (failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
    } else {
        std::cout << failures << " tests FAILED." << std::endl;
    }

    return failures;
}
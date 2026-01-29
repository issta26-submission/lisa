// Minimal C++11 unit test suite for ReadFileMemory (no GoogleTest, single executable)

// This test suite targets the C-style function declared as:
// extern "C" int ReadFileMemory(const char* filename, long* plFileSize, unsigned char** pFilePtr);

#include <cstdio>
#include <cstring>
#include <windows.h>
#include <string>
#include <functional>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the function under test (assuming it is linked from testzlib.c)
extern "C" int ReadFileMemory(const char* filename, long* plFileSize, unsigned char** pFilePtr);

// Simple test context to accumulate pass/fail without terminating on first failure
class TestContext {
public:
    static TestContext& Instance() {
        static TestContext inst;
        return inst;
    }

    void Pass(const std::string& msg) {
        ++passes;
        std::cout << "[PASS] " << msg << std::endl;
    }

    void Fail(const std::string& msg, const char* file, int line) {
        ++failures;
        std::cerr << "[FAIL] " << msg << " (at " << file << ":" << line << ")" << std::endl;
    }

    int GetPassCount() const { return passes; }
    int GetFailCount() const { return failures; }

private:
    int passes = 0;
    int failures = 0;
};

// Macro for assertions that records results but does not abort the test run
#define ASSERT(cond, msg) do { \
    if(!(cond)) { TestContext::Instance().Fail((msg), __FILE__, __LINE__); } \
    else { TestContext::Instance().Pass((msg)); } \
} while(0)

// Helper utility: write raw bytes to a file in binary mode
static bool WriteBinaryFile(const std::string& path, const unsigned char* data, size_t len) {
    std::ofstream ofs(path, std::ios::binary);
    if(!ofs) return false;
    ofs.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(len));
    return ofs.good();
}

// Helper utility: remove a file if it exists
static void RemoveFileIfExists(const std::string& path) {
    std::remove(path.c_str());
}

// Helper macro to safely free allocated memory
#define SAFE_FREE(ptr) do { if((ptr) != nullptr) { free(ptr); (ptr) = nullptr; } } while(0)

// Test 1: ReadFileMemory should successfully read a small text file
// - Verifies: non-null pointer returned, correct size, and exact content match
static void Test_ReadFileMemory_SmallFile() {
    const std::string fname = "test_small.bin";
    const unsigned char original[] = "abcdefghij"; // 10 bytes
    const size_t len = sizeof(original) - 1;

    RemoveFileIfExists(fname);
    bool wrote = WriteBinaryFile(fname, original, len);
    ASSERT(wrote, "Failed to create small test file");

    long fileSize = 0;
    unsigned char* dataPtr = nullptr;
    int ret = ReadFileMemory(fname.c_str(), &fileSize, &dataPtr);
    ASSERT(ret == 1, "ReadFileMemory should return 1 on success");
    ASSERT(fileSize == static_cast<long>(len), "File size should be 10");
    ASSERT(dataPtr != nullptr, "Data pointer should not be NULL");

    if (dataPtr != nullptr && fileSize > 0) {
        int cmp = std::memcmp(dataPtr, original, static_cast<size_t>(fileSize));
        ASSERT(cmp == 0, "Read data should match original file contents");
    }

    SAFE_FREE(dataPtr);
    RemoveFileIfExists(fname);
}

// Test 2: ReadFileMemory should handle an empty file
// - Verifies: size==0, non-null allocated pointer, and no crash
static void Test_ReadFileMemory_EmptyFile() {
    const std::string fname = "test_empty.bin";

    RemoveFileIfExists(fname);
    // Create an empty file
    {
        std::ofstream ofs(fname, std::ios::binary);
        // write nothing
    }

    long fileSize = -1;
    unsigned char* dataPtr = nullptr;
    int ret = ReadFileMemory(fname.c_str(), &fileSize, &dataPtr);
    ASSERT(ret == 1, "ReadFileMemory should return 1 for empty file");
    ASSERT(fileSize == 0, "File size should be 0 for empty file");
    ASSERT(dataPtr != nullptr, "Data pointer should not be NULL for empty file (allocated 1 byte)");

    SAFE_FREE(dataPtr);
    RemoveFileIfExists(fname);
}

// Test 3: ReadFileMemory should return 0 and not modify output pointer when file does not exist
// - Verifies: false branch handling
static void Test_ReadFileMemory_FileNotFound() {
    const std::string fname = "non_existent_file_12345.bin";

    long fileSize = -1;
    unsigned char* dataPtr = nullptr;
    int ret = ReadFileMemory(fname.c_str(), &fileSize, &dataPtr);
    ASSERT(ret == 0, "ReadFileMemory should return 0 when file is missing");
    // When fopen fails, the function returns early and does not modify *pFilePtr
    ASSERT(dataPtr == nullptr, "Data pointer should remain NULL when file read fails");
}

// Test 4: ReadFileMemory should correctly read binary data (including zero bytes)
static void Test_ReadFileMemory_BinaryContent() {
    const std::string fname = "test_binary.bin";

    const unsigned char sample[] = {
        0x00, 0x01, 0x02, 0x7F, 0x80, 0xFF, 0x0A, 0x0D,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
    };
    const size_t len = sizeof(sample);

    RemoveFileIfExists(fname);
    bool wrote = WriteBinaryFile(fname, sample, len);
    ASSERT(wrote, "Failed to create binary test file");

    long fileSize = -1;
    unsigned char* dataPtr = nullptr;
    int ret = ReadFileMemory(fname.c_str(), &fileSize, &dataPtr);
    ASSERT(ret == 1, "ReadFileMemory should return 1 on success (binary content)");
    ASSERT(fileSize == static_cast<long>(len), "File size should match binary sample length");
    ASSERT(dataPtr != nullptr, "Data pointer should not be NULL for binary content");

    if (dataPtr != nullptr) {
        int cmp = std::memcmp(dataPtr, sample, static_cast<size_t>(fileSize));
        ASSERT(cmp == 0, "Binary content should be read exactly as written");
    }

    SAFE_FREE(dataPtr);
    RemoveFileIfExists(fname);
}

// Entry point: run all tests and report summary
int main() {
    Test_ReadFileMemory_SmallFile();
    Test_ReadFileMemory_EmptyFile();
    Test_ReadFileMemory_FileNotFound();
    Test_ReadFileMemory_BinaryContent();

    int passes = TestContext::Instance().GetPassCount();
    int failures = TestContext::Instance().GetFailCount();

    std::cout << "Test summary: " << passes << " passed, " << failures << " failed." << std::endl;

    return failures > 0 ? 1 : 0;
}
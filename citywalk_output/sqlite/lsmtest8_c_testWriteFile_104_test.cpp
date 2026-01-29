/*
  Unit test suite for the focal method testWriteFile (C style, as used in lsmtest8.c)
  - Target: testWriteFile(const char *zFile, int iOff, void *pOut, int nByte, int *pRc)
  - We implement a small C++11-based test harness (no GTest) that exercises true/false branches
    of the method's predicates as feasibly as possible given the environment.
  - The tests focus on: pRc==0 guard, fopen failure, fseek failure, nByte>=0 path (including nByte==0),
    and non-zero pRc precondition. Partial-write failure is difficult to deterministically simulate here,
    so we cover the rest of the branches with robust checks and cleanups.
  - This file is self-contained and relies on the provided function prototype declared with C linkage.
*/

#include <lsmtest.h>
#include <vector>
#include <lsmInt.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


// Forward declaration of the focal function (C linkage)
extern "C" void testWriteFile(
  const char *zFile, 
  int iOff, 
  void *pOut, 
  int nByte, 
  int *pRc
);

// Helper: create a file with given content (binary mode)
static bool createFileWithContent(const std::string &path, const std::vector<unsigned char> &content) {
    FILE *f = std::fopen(path.c_str(), "wb");
    if(!f) return false;
    size_t written = std::fwrite(content.data(), 1, content.size(), f);
    std::fclose(f);
    return written == content.size();
}

// Helper: read entire file into vector
static bool readFile(const std::string &path, std::vector<unsigned char> &out) {
    FILE *f = std::fopen(path.c_str(), "rb");
    if(!f) return false;
    std::fseek(f, 0, SEEK_END);
    long sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if(sz < 0) { std::fclose(f); return false; }
    out.resize(static_cast<size_t>(sz));
    size_t rd = std::fread(out.data(), 1, out.size(), f);
    std::fclose(f);
    return rd == out.size();
}

// Helper: remove a file if exists
static void removeIfExists(const std::string &path) {
    std::remove(path.c_str());
}

// Simple test harness
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};
static void printSummary(const std::vector<TestResult> &results) {
    int passed = 0;
    for(const auto &r: results){
        std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name;
        if(!r.passed) std::cout << " - " << r.message;
        std::cout << std::endl;
        if(r.passed) ++passed;
    }
    std::cout << "Total: " << results.size() << "  Passed: " << passed << "  Failed: " << (results.size()-passed) << std::endl;
}

// Test 1: Succeeds writing bytes in a valid region; rc remains 0
static TestResult testWriteFile_SucceedsWritesBytes() {
    const std::string path = "lsmtest8_case1.bin";
    // Clean up any previous file
    removeIfExists(path);

    // Prepare initial content: 100 bytes all 'A'
    std::vector<unsigned char> init(100, 'A');
    if(!createFileWithContent(path, init)){
        return { "testWriteFile_SucceedsWritesBytes", false, "Failed to create initial test file" };
    }

    // Prepare output bytes to write
    unsigned char payload[8] = {0xDE,0xAD,0xBE,0xEF,0x01,0x02,0x03,0x04};
    int rc = 0;

    testWriteFile(path.c_str(), 10, payload, 8, &rc);

    if(rc != 0){
        removeIfExists(path);
        return { "testWriteFile_SucceedsWritesBytes", false, "rc non-zero after valid write (expected 0)" };
    }

    // Read back and verify bytes [10..17] match payload
    std::vector<unsigned char> after;
    if(!readFile(path, after)){
        removeIfExists(path);
        return { "testWriteFile_SucceedsWritesBytes", false, "Failed to read back file after write" };
    }

    if(after.size() < 18){
        removeIfExists(path);
        return { "testWriteFile_SucceedsWritesBytes", false, "File too small after write" };
    }

    if(std::memcmp(after.data() + 10, payload, 8) != 0){
        removeIfExists(path);
        return { "testWriteFile_SucceedsWritesBytes", false, "Written bytes do not match expected payload" };
    }

    // Clean up
    removeIfExists(path);
    return { "testWriteFile_SucceedsWritesBytes", true, "" };
}

// Test 2: fopen returns null (no such file); rc becomes 1
static TestResult testWriteFile_FileNotFound() {
    const std::string path = "lsmtest8_case2_nonexistent.bin";
    removeIfExists(path);

    // Ensure file does not exist
    FILE *f = std::fopen(path.c_str(), "wb");
    if(f) { std::fclose(f); std::remove(path.c_str()); } // remove created
    // Now path should not exist
    int rc = 0;
    unsigned char payload[8] = {1,2,3,4,5,6,7,8};

    testWriteFile(path.c_str(), 0, payload, 8, &rc);
    if(rc != 1){
        // If rc is not 1, test failed to trigger fopen failure
        removeIfExists(path);
        return { "testWriteFile_FileNotFound", false, "Expected rc=1 when file can't be opened" };
    }
    removeIfExists(path);
    return { "testWriteFile_FileNotFound", true, "" };
}

// Test 3: fseek fails (negative offset); rc becomes 1
static TestResult testWriteFile_FseekFailure() {
    const std::string path = "lsmtest8_case3.bin";
    removeIfExists(path);

    // Create a valid file
    std::vector<unsigned char> init(16, 0x55); // 16 bytes of 0x55
    if(!createFileWithContent(path, init)){
        removeIfExists(path);
        return { "testWriteFile_FseekFailure", false, "Failed to create initial test file" };
    }

    int rc = 0;
    unsigned char payload[4] = {9,9,9,9};
    // Negative offset to force fseek failure
    testWriteFile(path.c_str(), -5, payload, 4, &rc);

    if(rc != 1){
        removeIfExists(path);
        return { "testWriteFile_FseekFailure", false, "Expected rc=1 when fseek fails" };
    }

    // Optional: verify file content unchanged
    std::vector<unsigned char> after;
    if(!readFile(path, after)){
        removeIfExists(path);
        return { "testWriteFile_FseekFailure", false, "Failed to read back file" };
    }
    if(after != init){
        removeIfExists(path);
        return { "testWriteFile_FseekFailure", false, "File content changed despite fseek failure" };
    }

    removeIfExists(path);
    return { "testWriteFile_FseekFailure", true, "" };
}

// Test 4: nByte == 0 should not modify file and rc remains 0
static TestResult testWriteFile_NByteZero() {
    const std::string path = "lsmtest8_case4.bin";
    removeIfExists(path);

    // Create initial content
    std::vector<unsigned char> init(32, 0xAA);
    if(!createFileWithContent(path, init)){
        removeIfExists(path);
        return { "testWriteFile_NByteZero", false, "Failed to create initial test file" };
    }

    int rc = 0;
    unsigned char payload[8] = {0};

    testWriteFile(path.c_str(), 8, payload, 0, &rc);

    if(rc != 0){
        removeIfExists(path);
        return { "testWriteFile_NByteZero", false, "rc should remain 0 when nByte==0" };
    }

    // Verify content unchanged
    std::vector<unsigned char> after;
    if(!readFile(path, after)){
        removeIfExists(path);
        return { "testWriteFile_NByteZero", false, "Failed to read back file" };
    }
    if(after != init){
        removeIfExists(path);
        return { "testWriteFile_NByteZero", false, "File content changed when nByte==0" };
    }

    removeIfExists(path);
    return { "testWriteFile_NByteZero", true, "" };
}

// Test 5: pRc already non-zero should bypass operation
static TestResult testWriteFile_PrevRcNonZero() {
    const std::string path = "lsmtest8_case5.bin";
    removeIfExists(path);

    // Create initial content
    std::vector<unsigned char> init(20, 0x33);
    if(!createFileWithContent(path, init)){
        removeIfExists(path);
        return { "testWriteFile_PrevRcNonZero", false, "Failed to create initial test file" };
    }

    int rc = 123; // non-zero precondition
    unsigned char payload[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    testWriteFile(path.c_str(), 0, payload, 8, &rc);

    if(rc != 123){
        removeIfExists(path);
        return { "testWriteFile_PrevRcNonZero", false, "rc should remain unchanged when already non-zero" };
    }

    // Verify file unchanged
    std::vector<unsigned char> after;
    if(!readFile(path, after)){
        removeIfExists(path);
        return { "testWriteFile_PrevRcNonZero", false, "Failed to read back file" };
    }
    if(after != init){
        removeIfExists(path);
        return { "testWriteFile_PrevRcNonZero", false, "File content changed despite non-zero rc" };
    }

    removeIfExists(path);
    return { "testWriteFile_PrevRcNonZero", true, "" };
}

// Main: run all tests
int main() {
    std::vector<TestResult> results;

    results.push_back(testWriteFile_SucceedsWritesBytes());
    results.push_back(testWriteFile_FileNotFound());
    results.push_back(testWriteFile_FseekFailure());
    results.push_back(testWriteFile_NByteZero());
    results.push_back(testWriteFile_PrevRcNonZero());

    printSummary(results);

    // Return non-zero if any test failed
    int failed = 0;
    for(const auto &r: results) if(!r.passed) ++failed;
    return failed ? 1 : 0;
}
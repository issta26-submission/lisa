// Unit tests for IsMyFile function in cmscgats.c
// This test suite is designed for a C/C++11 environment without Google Test.
// It uses a lightweight, self-contained test harness and relies on the
// actual cmscgats.c implementation when linked with the rest of the project.
//
// Test focus:
// - Behavior when the target file does not exist (false/0 expected).
// - Behavior when the file exists and starts with a signature that most likely
//   belongs to a valid IT8/CGATS block (expected TRUE).
// - Behavior when the file exists but does not contain a recognizable block
//   (expected FALSE).
//
// Notes:
// - The tests create simple files in the working directory. They do not rely on
//   any private or static members of the focal class/file, and only exercise
//   the public function IsMyFile. They assume the project is linked with cmscgats.c
//   and related dependencies providing IsMyBlock and cmsSignalError.
// - We implement a minimal test harness (no GTest) and use non-terminating checks
//   (return values) to drive the test flow.

#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <lcms2_internal.h>


// Domain-relevant type aliases to match the focal function's API.
// We declare only what is necessary for linking with cmscgats.c.
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
typedef unsigned char cmsUInt8Number;
#define FALSE 0
#define TRUE 1

// Declarations of the focal API (as seen by cmscgats.c).
extern "C" cmsBool IsMyFile(const char* FileName);
extern "C" int IsMyBlock(const cmsUInt8Number* Buffer, cmsUInt32Number n);

// Helper: remove a file if it exists (ignore errors)
static void RemoveIfExists(const std::string& path) {
    std::remove(path.c_str());
}

// Helper: write exactly 132 bytes to a file. The first three bytes are a signature
// "IT8" to mimic a minimal CGATS/IT8 signature scenario.
static void Write132BytesWithSignature(const std::string& path) {
    unsigned char buf[132];
    std::memset(buf, 0, sizeof(buf));
    buf[0] = 'I';
    buf[1] = 'T';
    buf[2] = '8';
    std::ofstream f(path, std::ios::binary);
    f.write(reinterpret_cast<char*>(buf), sizeof(buf));
    f.close();
}

// Helper: write 132 bytes of zeros to a file (no recognizable signature)
static void Write132BytesZeros(const std::string& path) {
    unsigned char buf[132];
    std::memset(buf, 0, sizeof(buf));
    std::ofstream f(path, std::ios::binary);
    f.write(reinterpret_cast<char*>(buf), sizeof(buf));
    f.close();
}

// Test 1: Non-existent file should return FALSE.
// Rationale: IsMyFile should fail gracefully when fopen cannot open the file.
static bool Test_IsMyFile_FileNotFound() {
    const std::string path = "nonexistent_file_for_test_12345.tmp";
    RemoveIfExists(path);
    cmsBool res = IsMyFile(path.c_str());

    // Explanatory comment:
    // Expect FALSE (the file does not exist, so fopen should fail and
    // IsMyFile should return FALSE after signaling an error).
    if (res != FALSE) {
        std::cerr << "[Test 1] FAILED: Expected FALSE for non-existent file, got " << res << "\n";
        return false;
    }
    std::cout << "[Test 1] PASSED: Non-existent file returns FALSE as expected.\n";
    return true;
}

// Test 2: File with IT8-like signature should produce TRUE (best-effort based on signature).
// Rationale: If IsMyBlock recognizes the initial 'IT8' signature, IsMyFile should
// return TRUE for such a block. We craft a 132-byte file with a minimal IT8-like header.
static bool Test_IsMyFile_WithIt8Signature() {
    const std::string path = "test_it8_signature_it8.tmp";
    RemoveIfExists(path);
    Write132BytesWithSignature(path);

    cmsBool res = IsMyFile(path.c_str());

    // Explanatory comment:
    // We expect TRUE if the first bytes form a recognizable IT8 block header
    // that IsMyBlock detects. If IsMyBlock requires more structure, this test
    // might return FALSE, but still documents the intended path coverage.
    if (res != TRUE) {
        std::cerr << "[Test 2] FAILED: Expected TRUE for IT8-signature file, got " << res << "\n";
        return false;
    }
    std::cout << "[Test 2] PASSED: IT8-signature file yields TRUE as expected.\n";
    RemoveIfExists(path);
    return true;
}

// Test 3: File with 132 zero bytes should return FALSE (no recognizable block).
// Rationale: A file devoid of recognizable signature/data should not pass IsMyBlock.
static bool Test_IsMyFile_132BytesZeros() {
    const std::string path = "test_it8_no_signature_132zeros.tmp";
    RemoveIfExists(path);
    Write132BytesZeros(path);

    cmsBool res = IsMyFile(path.c_str());

    // Explanatory comment:
    // Expect FALSE since there is no valid IT8 block signature in the buffer.
    if (res != FALSE) {
        std::cerr << "[Test 3] FAILED: Expected FALSE for 132 zero bytes file, got " << res << "\n";
        return false;
    }
    std::cout << "[Test 3] PASSED: 132 zero bytes file returns FALSE as expected.\n";
    RemoveIfExists(path);
    return true;
}

// Simple test runner
int main() {
    std::cout << "Starting IsMyFile unit tests (no GTest).\n";

    int total = 0;
    int failed = 0;

    auto run = [&](bool result, const char* name) {
        ++total;
        if (!result) ++failed;
        // Non-terminating: we don't exit on first failure to maximize coverage.
        std::cout << name << " " << (result ? "[OK]" : "[FAILED]") << "\n";
    };

    // Run tests
    run(Test_IsMyFile_FileNotFound(), "Test_IsMyFile_FileNotFound");
    run(Test_IsMyFile_WithIt8Signature(), "Test_IsMyFile_WithIt8Signature");
    run(Test_IsMyFile_132BytesZeros(), "Test_IsMyFile_132BytesZeros");

    std::cout << "IsMyFile unit tests finished. Passed " << (total - failed) << " / " << total << ".\n";
    return failed ? 1 : 0;
}
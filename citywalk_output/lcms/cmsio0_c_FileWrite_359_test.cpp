// Test suite for cmsBool FileWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer)
// This test suite is designed for C++11 without GoogleTest. It uses a lightweight
// in-file test harness with plain assertions and descriptive comments for each test.
// Step references (from the prompt):
// Step 1: Understand FileWrite behavior and dependencies (uses iohandler->UsedSpace and iohandler->stream).
// Step 2: Create unit tests for FileWrite with true/false branches.
// Step 3: Refine tests for coverage and portability.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
#include <lcms2_internal.h>


// Import necessary internal dependencies to match the focal method's types.
// We wrap C headers to ensure correct linkage when called from C++.
extern "C" {
}

// Prototype of the focal function under test (extern "C" to avoid C++ name mangling)
extern "C" cmsBool FileWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer);

// Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if ( (cond) ) { \
        ++tests_passed; \
    } else { \
        std::cerr << "TEST FAIL: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    ++tests_run; \
    if (!(cond)) { \
        ++tests_passed; \
    } else { \
        std::cerr << "TEST FAIL: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while (0)

#define EXPECT_EQ(lhs, rhs, msg) do { \
    ++tests_run; \
    if ( (lhs) == (rhs) ) { \
        ++tests_passed; \
    } else { \
        std::cerr << "TEST FAIL: " << msg << " (expected " << (rhs) << ", got " << (lhs) << ") in " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while (0)


// Utility: create a unique temporary file path using tmpnam
static std::string MakeTempPath() {
    char tmpName[L_tmpnam];
    if (std::tmpnam(tmpName)) {
        return std::string(tmpName);
    }
    // Fallback in the unlikely event tmpnam fails
    return "./lcms_tmp_file.bin";
}

// Step 2 tests: cover FileWrite behavior for 0 size, successful write, and failed write via read-only stream.

// 1) Test that size == 0 returns TRUE and writes nothing (UsedSpace unchanged, file unchanged)
static void test_FileWrite_zero_size() {
    // Create an empty temporary file and prepare iohandler
    std::string path = MakeTempPath();
    FILE* f = fopen(path.c_str(), "w+b");
    if (f == nullptr) {
        std::cerr << "Unable to create temp file for zero-size test\n";
        return;
    }

    cmsIOHANDLER ioh;
    // Initialize ioh with the underlying FILE* and zero UsedSpace
    // The real cmsIOHANDLER has a 'stream' member; we set it directly as the focal method uses iohandler->stream
    ioh.stream = (void*)f;
    ioh.UsedSpace = 0;

    const char dummy[] = "DATA";
    cmsBool result = FileWrite(&ioh, 0, dummy);

    // Validate: zero size => result == TRUE, UsedSpace unchanged, no data written
    EXPECT_TRUE((result != 0), "FileWrite with size 0 must return TRUE");
    EXPECT_EQ(ioh.UsedSpace, 0, "UsedSpace should remain 0 after zero-size write");
    // Verify file contents are still empty
    fflush(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    EXPECT_EQ(size, 0, "File should remain empty after zero-size write");

    fclose(f);
    remove(path.c_str());
}

// 2) Test that a normal write writes the correct bytes and updates UsedSpace
static void test_FileWrite_successful_write() {
    std::string path = MakeTempPath();
    FILE* f = fopen(path.c_str(), "w+b");
    if (f == nullptr) {
        std::cerr << "Unable to create temp file for successful write test\n";
        return;
    }

    cmsIOHANDLER ioh;
    ioh.stream = (void*)f;
    ioh.UsedSpace = 0;

    const char payload[] = "Hello";
    cmsUInt32Number size = static_cast<cmsUInt32Number>(strlen(payload));

    cmsBool result = FileWrite(&ioh, size, payload);

    // Validate: result == TRUE, UsedSpace == size, file contains payload
    EXPECT_TRUE((result != 0), "FileWrite should succeed for writable stream");
    EXPECT_EQ(ioh.UsedSpace, size, "UsedSpace should be increased by written size");

    fflush(f);
    fseek(f, 0, SEEK_END);
    long writtenSize = ftell(f);
    EXPECT_EQ(writtenSize, (long)size, "File size should equal written size");

    // Read back to verify content
    fseek(f, 0, SEEK_SET);
    char readBuf[6] = {0};
    size_t readBytes = fread(readBuf, 1, sizeof(readBuf) - 1, f);
    readBuf[readBytes] = '\0';
    EXPECT_TRUE(std::string(readBuf) == "Hello", "File content should match written payload");

    fclose(f);
    remove(path.c_str());
}

// 3) Test that a write fails on a read-only stream but UsedSpace is still updated
static void test_FileWrite_failure_on_readonly_stream() {
    // Create a file with initial content to verify it remains unchanged
    std::string path = MakeTempPath();
    {
        FILE* f = fopen(path.c_str(), "wb");
        if (f == nullptr) {
            std::cerr << "Unable to create temp file for read-only failure test\n";
            return;
        }
        fwrite("abcdef", 1, 6, f);
        fclose(f);
    }

    // Open the file in read-only mode to simulate a write failure
    FILE* ro = fopen(path.c_str(), "rb");
    if (ro == nullptr) {
        std::cerr << "Unable to reopen temp file in read-only mode for test\n";
        remove(path.c_str());
        return;
    }

    cmsIOHANDLER ioh;
    ioh.stream = (void*)ro;
    ioh.UsedSpace = 0;

    const char payload[] = "XYZ";
    cmsUInt32Number size = static_cast<cmsUInt32Number>(strlen(payload));

    cmsBool result = FileWrite(&ioh, size, payload);

    // Validate: result == FALSE, UsedSpace updated, file remains unchanged on disk
    EXPECT_FALSE((result != 0), "FileWrite should fail on read-only stream");
    EXPECT_EQ(ioh.UsedSpace, size, "UsedSpace should be increased by attempted write size even on failure");

    // Close read-only stream and verify the on-disk content is unchanged
    fclose(ro);

    // Reopen to verify content
    FILE* testRead = fopen(path.c_str(), "rb");
    if (testRead == nullptr) {
        std::cerr << "Unable to read temp file for content verification\n";
        remove(path.c_str());
        return;
    }
    char verifyBuf[7] = {0};
    fread(verifyBuf, 1, 6, testRead);
    verifyBuf[6] = '\0';
    fclose(testRead);

    EXPECT_TRUE(std::string(verifyBuf) == "abcdef", "Disk content should remain unchanged after failed write");

    remove(path.c_str());
}

// Main: run all tests
int main() {
    // Step 2/3: Execute the test suite for the focal FileWrite function
    test_FileWrite_zero_size();
    test_FileWrite_successful_write();
    test_FileWrite_failure_on_readonly_stream();

    // Summary
    std::cout << "\nTest results: " << tests_passed << " / " << tests_run << " tests passed.\n";
    return (tests_run == tests_passed) ? 0 : 1;
}
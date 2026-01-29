// gzjoin_bopen_test.cpp
// A small, self-contained test suite for the C API function bopen(char* name)
// No GTest usage; tests are invoked from main() as per domain knowledge requirements.

#include <cstdio>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


extern "C" {
    // The actual bin type is internal to gzjoin.c; we treat the return as an opaque pointer.
    // We only need to know if the function succeeds (non-NULL) or fails (NULL).
    void* bopen(char* name);
}

// Forward declaration to ensure linkage with C code under test.
// We intentionally do not access any internal fields of 'bin' (opaque handle).

struct TestCase {
    const char* name;
    bool (*func)();
};

// Helper: create a temporary file and write optional data.
// Returns the path (std::string) if successful; empty string on failure.
static std::string create_temp_file(const std::string& optional_data = "") {
    char tmpl[] = "/tmp/gzjoin_testXXXXXX";
    int fd = mkstemp(tmpl);
    if (fd == -1) {
        std::cerr << "Failed to create temp file template." << std::endl;
        return "";
    }
    if (!optional_data.empty()) {
        ssize_t written = write(fd, optional_data.data(), optional_data.size());
        (void)written; // not strictly needed
    }
    close(fd);
    // Return the path; keep the file on disk for the duration of the test.
    // The test may not remove the file to avoid interference with the open bin's fd.
    return std::string(tmpl);
}

// Test 1: Valid open with an existing file should return non-NULL.
static bool test_bopen_valid_file() {
    bool passed = true;
    std::string path = create_temp_file("sample content");
    if (path.empty()) {
        std::cerr << "[test_bopen_valid_file] Failed to create temp file." << std::endl;
        return false;
    }

    void* handle = bopen(const_cast<char*>(path.c_str()));
    if (handle == nullptr) {
        std::cerr << "[test_bopen_valid_file] bopen() returned NULL for an existing file: " 
                  << path << std::endl;
        passed = false;
    } else {
        // We intentionally do not attempt to close/free the internal resource here,
        // since the internal bclose() is static/private in gzjoin.c and not exposed.
        // The test focuses on the success path (non-NULL).
    }

    // Best-effort: attempt to unlink the temp file; the opened descriptor may still keep the data.
    // If unlinking fails, we still keep the test logic intact.
    if (unlink(path.c_str()) != 0) {
        // Not fatal; print debug information optionally.
        std::cerr << "[test_bopen_valid_file] Warning: failed to unlink temp file." << std::endl;
    }

    return passed;
}

// Test 2: Open should fail for a non-existent file (returns NULL).
static bool test_bopen_missing_file() {
    bool passed = true;
    // Construct a path that should not exist.
    std::string path = "/tmp/this_file_should_not_exist_hopefully_1234567890.txt";

    void* handle = bopen(const_cast<char*>(path.c_str()));
    if (handle != nullptr) {
        std::cerr << "[test_bopen_missing_file] bopen() unexpectedly returned non-NULL for missing file: "
                  << path << std::endl;
        passed = false;
        // No cleanup needed for actual file, since it doesn't exist.
    }
    return passed;
}

// Test 3: Open should fail for an empty path (returns NULL).
static bool test_bopen_empty_path() {
    bool passed = true;
    std::string path = "";
    void* handle = bopen(const_cast<char*>(path.c_str()));
    if (handle != nullptr) {
        std::cerr << "[test_bopen_empty_path] bopen() unexpectedly returned non-NULL for empty path." << std::endl;
        passed = false;
    }
    return passed;
}

int main() {
    // Register test cases
    std::vector<TestCase> tests = {
        {"bopen_valid_file", test_bopen_valid_file},
        {"bopen_missing_file", test_bopen_missing_file},
        {"bopen_empty_path", test_bopen_empty_path}
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Running " << total << " test(s) for bopen(...) in gzjoin.c\n";

    for (const auto& tc : tests) {
        std::cout << "Running test: " << tc.name << "... ";
        bool ok = tc.func();
        if (ok) {
            std::cout << "PASSED\n";
            ++passed;
        } else {
            std::cout << "FAILED\n";
        }
    }

    std::cout << "Test results: " << passed << "/" << total << " passed.\n";
    return (passed == total) ? 0 : 1;
}
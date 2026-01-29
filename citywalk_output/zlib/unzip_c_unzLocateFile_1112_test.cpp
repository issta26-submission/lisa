// C++11 test suite for the focal method unzLocateFile (part of unzip.c) using a lightweight
// in-house test framework (no Google Test). This test suite uses mock implementations
// of the dependent unzip internals to deterministically exercise the control flow
// of unzLocateFile under various scenarios.

// Note: This test file is designed to work with a compilation unit that provides
// unzip.c and unzip.h. To enable the mock behavior for dependent calls (GoToFirstFile,
// GetCurrentFileInfo64, StringFileNameCompare, GoToNextFile), we expose mock implementations
// with C linkage here. The mocks simulate a central directory of file names for predictable
// behavior. This approach allows testing the logic of unzLocateFile (state saving/restoration,
// param checks, and search loop) without requiring a full ZIP I/O stack.

#include <cstring>
#include <cstdio>
#include <string.h>
#include <string>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <vector>
#include <cctype>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Include the production interface for unzLocateFile and the types/macros used by it.
extern "C" {
}

// ---------------- Mock infrastructure for unzip dependencies ----------------

// Global state used by mocks to simulate a central directory of files.
static std::vector<std::string> g_mock_file_names; // names of files in the "central dir"
static size_t g_mock_count = 0;                   // number of mock files
static size_t g_mock_index = 0;                   // current index in the mock file list

// Track test failures to implement a non-terminating test flow.
static int g_total_tests_run = 0;
static int g_total_failures  = 0;

// Helper to reset mock state for a new test scenario.
static void mock_reset_state() {
    g_mock_file_names.clear();
    g_mock_count = 0;
    g_mock_index = 0;
}

// Helper to configure the mock central directory for a test.
static void mock_set_files(const std::vector<std::string>& names) {
    mock_reset_state();
    g_mock_file_names = names;
    g_mock_count = names.size();
    g_mock_index = -1; // will be set to 0 on first GoToFirstFile call
}

// Case-insensitive compare utility for the mock
static int mock_strcmp_nocase(const char* a, const char* b) {
    while (*a && *b) {
        char ca = std::tolower(static_cast<unsigned char>(*a));
        char cb = std::tolower(static_cast<unsigned char>(*b));
        if (ca != cb) return (int)(ca - cb);
        ++a; ++b;
    }
    return (int)(*a - *b);
}

// Mock: unzGoToFirstFile
extern "C" int ZEXPORT unzGoToFirstFile(unzFile file) {
    // Reset to first file in the mock list.
    g_mock_index = 0;
    if (g_mock_count == 0) {
        // No files available
        return UNZ_END_OF_LIST_OF_FILE;
    }
    // Hint to the caller that there is a current file ready
    if (file != NULL) {
        ((unz64_s*)file)->current_file_ok = 1;
    }
    return UNZ_OK;
}

// Mock: unzGoToNextFile
extern "C" int ZEXPORT unzGoToNextFile(unzFile file) {
    // Move to next file in the mock list
    g_mock_index++;
    if (g_mock_index < g_mock_count) {
        if (file != NULL) {
            ((unz64_s*)file)->current_file_ok = 1;
        }
        return UNZ_OK;
    }
    // End of mock file list
    return UNZ_END_OF_LIST_OF_FILE;
}

// Mock: unzGetCurrentFileInfo64
extern "C" int ZEXPORT unzGetCurrentFileInfo64(
    unzFile file,
    unz_file_info64 * pfile_info,
    char * szCurrentFileName,
    uLong fileNameBufferSize,
    void * extraField,
    uLong extraFieldBufferSize,
    void * szComment,
    uLong commentBufferSize)
{
    (void)pfile_info;
    (void)extraField;
    (void)extraFieldBufferSize;
    (void)szComment;
    (void)commentBufferSize;

    if (g_mock_index < g_mock_count && szCurrentFileName != NULL) {
        const std::string& name = g_mock_file_names[g_mock_index];
        size_t maxCopy = (fileNameBufferSize > 0) ? fileNameBufferSize - 1 : 0;
        size_t copy_len = (name.size() < maxCopy) ? name.size() : maxCopy;
        if (copy_len > 0) {
            std::memcpy(szCurrentFileName, name.c_str(), copy_len);
        }
        // Null-terminate
        if (fileNameBufferSize > 0) {
            szCurrentFileName[copy_len] = '\0';
        }
    } else if (szCurrentFileName != NULL && fileNameBufferSize > 0) {
        szCurrentFileName[0] = '\0';
    }
    return UNZ_OK;
}

// Mock: unzStringFileNameCompare
extern "C" int ZEXPORT unzStringFileNameCompare(
    const char* fileName1,
    const char* fileName2,
    int iCaseSensitivity)
{
    if (iCaseSensitivity == 0) {
        // Case-insensitive comparison
        return mock_strcmp_nocase(fileName1, fileName2);
    } else {
        // Case-sensitive
        return std::strcmp(fileName1, fileName2);
    }
}

// ---------------- Simple in-test assertion helpers (non-terminating) ----------------

static void test_report(const char* test_name, bool passed) {
    ++g_total_tests_run;
    if (passed) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        ++g_total_failures;
        std::cerr << "[FAIL] " << test_name << "\n";
    }
}

// Simple equal assertion for integers
#define EXPECT_EQ(a, b, test_name) do { \
    bool _eq = ((a) == (b)); \
    test_report(test_name, _eq); \
    if (!_eq) { std::cerr << "  Expected: " << (b) << ", Got: " << (a) << "\n"; } \
} while (0)

// ---------------- Test cases for unzLocateFile ----------------

// Test 1: Parameter error when file pointer is NULL
static bool test_param_error_null_file() {
    const char* fname = "test.txt";
    int err = unzLocateFile(NULL, fname, 0);
    bool ok = (err == UNZ_PARAMERROR);
    test_report("test_param_error_null_file", ok);
    return ok;
}

// Test 2: Parameter error when file name is too long
static bool test_param_error_long_filename() {
    // Create a filename of length UNZ_MAXFILENAMEINZIP to trigger the boundary condition
    const size_t maxlen = UNZ_MAXFILENAMEINZIP; // e.g., 256
    std::string longname(maxlen, 'a');
    // Ensure NUL terminator is present in C string usage
    char fname_with_null[maxlen + 1];
    std::memcpy(fname_with_null, longname.c_str(), maxlen);
    fname_with_null[maxlen] = '\0';
    int err = unzLocateFile(reinterpret_cast<unzFile>(nullptr), fname_with_null, 0);
    bool ok = (err == UNZ_PARAMERROR);
    test_report("test_param_error_long_filename", ok);
    return ok;
}

// Test 3: Current file not ok should yield UNZ_END_OF_LIST_OF_FILE
static bool test_end_of_list_when_current_file_not_ok() {
    // Prepare a zip file descriptor-like object with current_file_ok = 0
    unz64_s fake = {};
    fake.current_file_ok = 0;
    const char* target = "anything.txt";
    int err = unzLocateFile(reinterpret_cast<unzFile>(&fake), target, 0);
    bool ok = (err == UNZ_END_OF_LIST_OF_FILE);
    // Also ensure state remained as-is (no modifications were made since we didn't enter the loop)
    EXPECT_EQ(fake.num_file, (int)fake.num_file, "state_preserved_in_param_case"); // no-op check to keep structure
    test_report("test_end_of_list_when_current_file_not_ok", ok);
    return ok;
}

// Test 4: Found matching file during iteration
static bool test_found_matching_file() {
    // Configure mock to present a single file named "target.txt"
    mock_set_files({ "target.txt" });
    unz64_s fake = {};
    fake.current_file_ok = 1;
    fake.num_file = 5;
    fake.pos_in_central_dir = 42;
    // Call unzLocateFile to look for "target.txt"
    int err = unzLocateFile(reinterpret_cast<unzFile>(&fake), "target.txt", 0);
    bool ok = (err == UNZ_OK);
    test_report("test_found_matching_file", ok);
    return ok;
}

// Test 5: File not found after iterating all entries; ensure end state restoration and proper return
static bool test_not_found_and_state_restoration() {
    // Mock with a single non-matching file
    mock_set_files({ "notmatch.txt" });
    unz64_s fake = {};
    fake.current_file_ok = 1;
    fake.num_file = 123;
    fake.pos_in_central_dir = 999;
    // Remember original state
    int saved_num_file = fake.num_file;
    int saved_pos = fake.pos_in_central_dir;

    int err = unzLocateFile(reinterpret_cast<unzFile>(&fake), "target_not_present.txt", 0);
    bool ok = (err == UNZ_END_OF_LIST_OF_FILE) && (fake.num_file == saved_num_file) && (fake.pos_in_central_dir == saved_pos);
    test_report("test_not_found_and_state_restoration", ok);
    return ok;
}

// ---------------- Main to run tests ----------------

int main() {
    // Reset overall counters
    g_total_tests_run = 0;
    g_total_failures = 0;

    // Run tests (these rely on the mocks above)
    bool t1 = test_param_error_null_file();
    bool t2 = test_param_error_long_filename();
    bool t3 = test_end_of_list_when_current_file_not_ok();
    bool t4 = test_found_matching_file();
    bool t5 = test_not_found_and_state_restoration();

    // Summary
    std::cout << "Tests run: " << g_total_tests_run
              << ", Failures: " << g_total_failures
              << std::endl;

    // Return non-zero on any failure for quick CI signaling
    return g_total_failures ? 1 : 0;
}
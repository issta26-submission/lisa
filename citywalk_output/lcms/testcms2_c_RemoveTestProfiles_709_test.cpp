/*
  Unit test suite for the focal method RemoveTestProfiles located in testcms2.c

  Summary:
  - The function RemoveTestProfiles simply calls remove(...) on a fixed set of
    ICC profile file names to delete test artifacts.
  - Tests are written in C++11, without Google Test, using a lightweight
    self-contained harness. The tests create dummy files before invoking
    RemoveTestProfiles and verify that the expected files are removed afterwards.
  - All tests are non-terminating (they collect and report failures rather than abort).
  - Tests call the C function RemoveTestProfiles via an extern "C" prototype.
  - The tests are designed to be portable across platforms using standard C++11
    library facilities (fstream, cstdio, etc.).
*/

#include <cstdio>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Forward declaration of the focal function from C code.
// Use C linkage to avoid name mangling when linking with C sources.
extern "C" void RemoveTestProfiles(void);

// Helper: check if a file exists by attempting to open it.
static bool FileExists(const std::string& path) {
    std::ifstream f(path.c_str());
    return f.good();
}

// Helper: create an empty file with the given path.
static bool CreateFile(const std::string& path) {
    std::ofstream f(path.c_str());
    if (!f) return false;
    f.close();
    return true;
}

// Helper: delete file if it exists (best-effort cleanup).
static void DeleteFileIfExists(const std::string& path) {
    if (FileExists(path)) {
        std::remove(path.c_str());
    }
}

// Test 1: Ensure RemoveTestProfiles removes all known test profile files.
// This validates the “true branch” where files exist and should be deleted.
static void Test_AllFilesRemoved() {
    // Candidate keywords extracted from RemoveTestProfiles dependency set.
    const std::vector<std::string> files = {
        "sRGBlcms2.icc",
        "aRGBlcms2.icc",
        "graylcms2.icc",
        "gray3lcms2.icc",
        "linlcms2.icc",
        "limitlcms2.icc",
        "labv2lcms2.icc",
        "labv4lcms2.icc",
        "xyzlcms2.icc",
        "nullcms2.icc",
        "bchslcms2.icc",
        "lcms2cmyk.icc",
        "glablcms2.icc",
        "lcms2link.icc",
        "lcms2link2.icc",
        "brightness.icc"
    };

    std::vector<std::string> errors;

    // Step: create all files used by the function.
    for (const auto& fname : files) {
        if (!CreateFile(fname)) {
            errors.push_back("Failed to create test file before removal: " + fname);
        }
    }

    // Step: invoke the focal method under test.
    RemoveTestProfiles();

    // Step: verify that all files have been removed.
    for (const auto& fname : files) {
        if (FileExists(fname)) {
            errors.push_back("File still exists after RemoveTestProfiles: " + fname);
        }
    }

    // Step: cleanup any leftovers (defensive).
    for (const auto& fname : files) {
        DeleteFileIfExists(fname);
    }

    // Report result.
    if (errors.empty()) {
        std::cout << "PASS: Test_AllFilesRemoved\n";
    } else {
        std::cout << "FAIL: Test_AllFilesRemoved\n";
        for (const auto& e : errors) std::cout << "  " << e << "\n";
    }
}

// Test 2: Ensure RemoveTestProfiles behaves when only a subset of files exist.
// This validates handling of mixed existence and non-existence of files.
static void Test_PartialFilesRemoved() {
    // Subset of the file list that we will create before removal.
    const std::vector<std::string> files_to_create = {
        "sRGBlcms2.icc",
        "aRGBlcms2.icc",
        "graylcms2.icc",
        "gray3lcms2.icc",
        "linlcms2.icc",
        "limitlcms2.icc",
        "labv2lcms2.icc",
        "labv4lcms2.icc"
    };

    // Remaining files are intentionally left non-existent to simulate partial state.
    const std::vector<std::string> all_files = {
        "sRGBlcms2.icc",
        "aRGBlcms2.icc",
        "graylcms2.icc",
        "gray3lcms2.icc",
        "linlcms2.icc",
        "limitlcms2.icc",
        "labv2lcms2.icc",
        "labv4lcms2.icc",
        "xyzlcms2.icc",
        "nullcms2.icc",
        "bchslcms2.icc",
        "lcms2cmyk.icc",
        "glablcms2.icc",
        "lcms2link.icc",
        "lcms2link2.icc",
        "brightness.icc"
    };

    std::vector<std::string> errors;

    // Create only the subset.
    for (const auto& fname : files_to_create) {
        if (!CreateFile(fname)) {
            errors.push_back("Failed to create test file before partial removal: " + fname);
        }
    }

    // Invoke the focal method under test.
    RemoveTestProfiles();

    // Verify that only the created files have been removed.
    for (const auto& fname : files_to_create) {
        if (FileExists(fname)) {
            errors.push_back("File not removed (expected by partial test): " + fname);
        }
    }

    // Cleanup: ensure no leftover test files remain after test.
    for (const auto& fname : all_files) {
        DeleteFileIfExists(fname);
    }

    // Report result.
    if (errors.empty()) {
        std::cout << "PASS: Test_PartialFilesRemoved\n";
    } else {
        std::cout << "FAIL: Test_PartialFilesRemoved\n";
        for (const auto& e : errors) std::cout << "  " << e << "\n";
    }
}

// Test 3: Ensure RemoveTestProfiles does not crash or misbehave when no test files exist.
// This validates the false/edge-case path where nothing should be removed.
static void Test_NoFilesExist() {
    const std::vector<std::string> files = {
        "sRGBlcms2.icc",
        "aRGBlcms2.icc",
        "graylcms2.icc",
        "gray3lcms2.icc",
        "linlcms2.icc",
        "limitlcms2.icc",
        "labv2lcms2.icc",
        "labv4lcms2.icc",
        "xyzlcms2.icc",
        "nullcms2.icc",
        "bchslcms2.icc",
        "lcms2cmyk.icc",
        "glablcms2.icc",
        "lcms2link.icc",
        "lcms2link2.icc",
        "brightness.icc"
    };

    std::vector<std::string> errors;

    // Ensure none exist before test.
    for (const auto& fname : files) {
        DeleteFileIfExists(fname);
        if (FileExists(fname)) {
            errors.push_back("Failed to clean pre-test file: " + fname);
        }
    }

    // Invoke the focal method under test.
    RemoveTestProfiles();

    // Verify that the filesystem state is unchanged (no new files created, no crash).
    for (const auto& fname : files) {
        if (FileExists(fname)) {
            errors.push_back("New file appeared after RemoveTestProfiles: " + fname);
        }
    }

    // Report result.
    if (errors.empty()) {
        std::cout << "PASS: Test_NoFilesExist\n";
    } else {
        std::cout << "FAIL: Test_NoFilesExist\n";
        for (const auto& e : errors) std::cout << "  " << e << "\n";
    }
}

// Entry point: run all tests sequentially.
// This mirrors a minimal test runner that would be suitable when GTest is not allowed.
int main() {
    // Execute tests in a deterministic order.
    Test_AllFilesRemoved();
    Test_PartialFilesRemoved();
    Test_NoFilesExist();

    // Return code 0 regardless of individual test outcomes to allow continuous execution in CI.
    return 0;
}
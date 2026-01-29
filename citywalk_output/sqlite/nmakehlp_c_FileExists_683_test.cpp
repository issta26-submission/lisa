/*
Step 1 - Program Understanding and Candidate Keywords:
Candidate Keywords for FileExists(szPath):
- szPath (LPCTSTR), path string input
- GetFileAttributes(szPath)
- INVALID_FILE_ATTRIBUTES
- pathAttr (DWORD)
- FILE_ATTRIBUTE_DIRECTORY
- TRUE / FALSE semantics based on (pathAttr != INVALID_FILE_ATTRIBUTES && !(pathAttr & FILE_ATTRIBUTE_DIRECTORY))

Step 2 - Unit Test Generation:
Tests to cover:
- Non-existent path -> FALSE
- Existing file path -> TRUE
- Existing directory path -> FALSE

Step 3 - Test Case Refinement:
Tests implemented as a simple C++11 test harness (no GTest). Uses Windows API to create temp files and directories. Uses non-terminating expectations so tests continue after failures.
*/

#include <math.h>
#include <windows.h>
#include <cstdio>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>


// Declaration for linkage across C/C++ boundaries.
// The focal method is defined in nmakehlp.c as a C function.
extern "C" BOOL FileExists(LPCTSTR szPath);

// Lightweight non-terminating test macros (do not abort on failure, just log).
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_tests_run; \
        if(!(cond)) { \
            ++g_tests_failed; \
            std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")\n"; \
        } \
    } while(false)

#define EXPECT_FALSE(cond, msg) \
    do { \
        ++g_tests_run; \
        if(cond) { \
            ++g_tests_failed; \
            std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")\n"; \
        } \
    } while(false)

#define SUMMARY() \
    do { \
        std::cout << "Test summary: " << g_tests_run << " test(s) run, " \
                  << g_tests_failed << " failure(s).\n"; \
    } while(false)

int main()
{
    // Step 2: Prepare environment and test data
    // Obtain system temp directory (ANSI variant)
    char tempDirBuf[MAX_PATH] = {0};
    DWORD tmpPathLen = GetTempPathA(MAX_PATH, tempDirBuf);
    if (tmpPathLen == 0 || tmpPathLen > MAX_PATH) {
        std::cerr << "Failed to obtain temporary directory.\n";
        SUMMARY();
        return 1;
    }
    std::string tempDir(tempDirBuf, tmpPathLen); // includes trailing backslash

    // 1) Non-existent path: construct a unique name that should not exist
    std::string nonExistPath = tempDir + "nmakehlp_test_nonexist_" + std::to_string(GetTickCount()) + ".tmp";
    BOOL res = FileExists(nonExistPath.c_str());
    EXPECT_FALSE(res, "FileExists should return FALSE for a non-existent path");

    // 2) Existing file: create a temporary file and verify TRUE
    std::string filePath = tempDir + "nmakehlp_test_file_" + std::to_string(GetTickCount()) + ".txt";
    {
        std::ofstream ofs(filePath.c_str(), std::ios::binary);
        if(!ofs) {
            std::cerr << "WARNING: Could not create temporary file for testing: " << filePath << "\n";
        } else {
            ofs << "test data";
        }
    }
    // Recompute path in case of any buffering changes
    res = FileExists(filePath.c_str());
    EXPECT_TRUE(res, "FileExists should return TRUE for an existing regular file");

    // Cleanup the created file
    if (std::remove(filePath.c_str()) != 0) {
        // Non-fatal cleanup issue; log but don't fail test
        std::cerr << "WARNING: Could not remove temporary test file: " << filePath << "\n";
    }

    // 3) Existing directory: create a temporary directory and verify FALSE
    std::string dirPath = tempDir + "nmakehlp_test_dir_" + std::to_string(GetTickCount());
    BOOL dirCreated = CreateDirectoryA(dirPath.c_str(), NULL);
    if (!dirCreated) {
        DWORD err = GetLastError();
        // If directory creation failed, report and continue with an alternative check
        std::cerr << "WARNING: Could not create temporary test directory. Error: " << err << "\n";
        // We'll still attempt to test with a directory path that likely does not exist, which should yield FALSE
    } else {
        // Directory created successfully; verify that FileExists reports FALSE (path is a directory)
        res = FileExists(dirPath.c_str());
        EXPECT_FALSE(res, "FileExists should return FALSE for an existing directory path");
        // Cleanup directory
        if (!RemoveDirectoryA(dirPath.c_str())) {
            std::cerr << "WARNING: Could not remove temporary test directory: " << dirPath << "\n";
        }
    }

    SUMMARY();
    // Return non-zero if any test failed to aid integration in build systems
    return (g_tests_failed == 0) ? 0 : 1;
}
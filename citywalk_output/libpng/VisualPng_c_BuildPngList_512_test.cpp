// VisualPng_Test.cpp
// Lightweight unit tests for BuildPngList from VisualPng.c
// Note: This test suite targets ANSI builds (TCHAR == char). If your project
// uses UNICODE, you will need to adapt the test to use wide strings (wchar_t)
// and the corresponding Windows API variants FindFirstFileW/FindNextFileW, etc.

#include <algorithm>
#include <direct.h>
#include <vector>
#include <string.h>
#include <windows.h>
#include <string>
#include <cstring>
#include <fstream>
#include <zlib.h>
#include <resource.h>
#include <stdlib.h>
#include <cstdio>
#include <pngfile.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <png.h>


// Declaration of the focal function (from VisualPng.c)
extern "C" BOOL BuildPngList (PTSTR pstrPathName, TCHAR **ppFileList, int *pFileCount,
     int *pFileIndex);

// Helper: simple test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
        g_tests_failed++; \
    } \
    g_tests_run++; \
} while(0)


// Helper: create a directory (ANSI)
static bool createDirA(const std::string& dir)
{
    // CreateDirectoryA returns 0 on failure; if already exists, it fails too.
    // We ignore error if dir already exists.
    if (CreateDirectoryA(dir.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
        return true;
    return false;
}

// Helper: create a zero-byte file
static bool createFileA(const std::string& filepath)
{
    std::ofstream ofs(filepath.c_str(), std::ios::binary);
    if (!ofs) return false;
    ofs.close();
    return true;
}

// Helper: remove a file if exists
static void removeFileA(const std::string& filepath)
{
    DeleteFileA(filepath.c_str());
}

// Helper: remove directory (attempt to remove all files first)
static bool removeDirRecursiveA(const std::string& dir)
{
    // Delete all files in directory
    std::string pattern = dir + "\\*.*";

    WIN32_FIND_DATA finddata;
    HANDLE hFind = FindFirstFileA(pattern.c_str(), &finddata);
    if (hFind == INVALID_HANDLE_VALUE) {
        // Empty or no access; attempt to remove directory directly
    } else {
        do {
            // Skip . and .. entries
            if (strcmp(finddata.cFileName, ".") == 0 || strcmp(finddata.cFileName, "..") == 0)
                continue;
            std::string filePath = dir + "\\" + finddata.cFileName;
            DeleteFileA(filePath.c_str());
        } while (FindNextFileA(hFind, &finddata));
        FindClose(hFind);
    }
    // Finally remove directory
    return RemoveDirectoryA(dir.c_str()) != 0;
}

// Helper: list png files in a directory (ANSI) for expected results
static std::vector<std::string> listPngFilesInDirA(const std::string& dir)
{
    std::vector<std::string> files;
    std::string pattern = dir + "\\*.png";
    WIN32_FIND_DATAA finddata;
    HANDLE hFind = FindFirstFileA(pattern.c_str(), &finddata);
    if (hFind == INVALID_HANDLE_VALUE) {
        return files;
    }
    do {
        // skip "." and ".."
        if (strcmp(finddata.cFileName, ".") == 0 || strcmp(finddata.cFileName, "..") == 0)
            continue;
        files.push_back(dir + "\\" + finddata.cFileName);
    } while (FindNextFileA(hFind, &finddata));
    FindClose(hFind);
    std::sort(files.begin(), files.end());
    return files;
}

// Test 1: Simple case with 3 png files, verify sorting and index mapping
static bool test_BuildPngList_SimpleCase()
{
    // Create a unique temporary directory
    char tmpDir[MAX_PATH];
    DWORD len = GetTempPathA(MAX_PATH, tmpDir);
    if (len == 0 || len > MAX_PATH) return false;
    std::string testDir = std::string(tmpDir) + "VisualPngTest_SimpleCase";
    if (!createDirA(testDir)) {
        // If directory exists, still continue (cleanup may fail)
        // Attempt to proceed; could still work if it exists
        // To be robust, try to remove any previous remnants
        removeDirRecursiveA(testDir);
        createDirA(testDir);
    }

    // Create PNG files: apple.png, banana.png, cherry.png; also a non-png file
    createFileA(testDir + "\\apple.png");
    createFileA(testDir + "\\banana.png");
    createFileA(testDir + "\\cherry.png");
    createFileA(testDir + "\\notes.txt"); // should be ignored

    // Input path: full path to banana.png (to test indexing)
    std::string inputPath = testDir + "\\banana.png";

    // Prepare parameters for BuildPngList
    char inputPathC[MAX_PATH];
    strncpy(inputPathC, inputPath.c_str(), MAX_PATH);
    inputPathC[MAX_PATH - 1] = '\0';

    TCHAR *pFileList = NULL;
    int fileCount = 0;
    int fileIndex = -1;

    BOOL res = BuildPngList((PTSTR)inputPathC, &pFileList, &fileCount, &fileIndex);
    TEST_ASSERT(res == TRUE, "BuildPngList should return TRUE on valid directory with png files");

    // Build expected list of png files (sorted)
    std::vector<std::string> expected = listPngFilesInDirA(testDir);
    // Ensure only png files are included in expected
    // The expected vector already contains full paths to files in dir with .png extension
    // It will include apple.png, banana.png, cherry.png in sorted order: apple, banana, cherry

    // Gather actual results from pFileList
    std::vector<std::string> actual;
    if (pFileList != NULL && fileCount > 0) {
        const char* base = (const char*)pFileList;
        for (int i = 0; i < fileCount; ++i) {
            const char* path = base + i * MAX_PATH;
            actual.push_back(std::string(path));
        }
        // Free the allocated memory as done by the API
        free(pFileList);
        pFileList = NULL;
    }

    // Compare counts
    TEST_ASSERT(actual.size() == expected.size(), "FileCount should equal number of png files in directory");

    // Compare each path content (order must be sorted)
    if (actual.size() == expected.size()) {
        for (size_t i = 0; i < expected.size(); ++i) {
            TEST_ASSERT(actual[i] == expected[i], "PNG file list should be sorted alphabetically by full path");
        }
    }

    // Check that pFileIndex corresponds to the index of inputPath within the sorted list
    // Determine expected index of banana.png in the expected vector
    int expectedIndex = -1;
    for (size_t i = 0; i < expected.size(); ++i) {
        if (expected[i] == inputPath) {
            expectedIndex = (int)i;
            break;
        }
    }
    TEST_ASSERT(expectedIndex == fileIndex, "pFileIndex should point to the position of the input file in the sorted list");

    // Cleanup
    removeFileA(testDir + "\\apple.png");
    removeFileA(testDir + "\\banana.png");
    removeFileA(testDir + "\\cherry.png");
    removeFileA(testDir + "\\notes.txt");
    removeDirRecursiveA(testDir);

    return true;
}

// Test 2: Directory with no PNG files -> should return TRUE and FileCount == 0
static bool test_BuildPngList_NoPngFiles()
{
    // Create a new test directory
    char tmpDir[MAX_PATH];
    DWORD len = GetTempPathA(MAX_PATH, tmpDir);
    if (len == 0 || len > MAX_PATH) return false;
    std::string testDir = std::string(tmpDir) + "VisualPngTest_NoPng";
    if (!createDirA(testDir)) {
        removeDirRecursiveA(testDir);
        createDirA(testDir);
    }

    // Create a non-png file to ensure there is content but no PNGs
    createFileA(testDir + "\\readme.txt");

    // Input path: full path to readme.txt (directory path is used for search)
    std::string inputPath = testDir + "\\readme.txt";
    char inputPathC[MAX_PATH];
    strncpy(inputPathC, inputPath.c_str(), MAX_PATH);
    inputPathC[MAX_PATH - 1] = '\0';

    TCHAR *pFileList = NULL;
    int fileCount = 0;
    int fileIndex = -1;

    BOOL res = BuildPngList((PTSTR)inputPathC, &pFileList, &fileCount, &fileIndex);
    TEST_ASSERT(res == TRUE, "BuildPngList should return TRUE even when no PNGs exist");
    TEST_ASSERT(fileCount == 0, "FileCount should be 0 when there are no PNG files");

    // Cleanup: free if allocated
    if (pFileList != NULL) {
        free(pFileList);
        pFileList = NULL;
    }

    // Cleanup directory
    removeFileA(testDir + "\\readme.txt");
    removeDirRecursiveA(testDir);

    return true;
}

// Main: run tests
int main()
{
    std::cout << "Starting BuildPngList unit tests (ANSI build)..." << std::endl;

    // Run tests
    if (test_BuildPngList_SimpleCase()) {
        std::cout << "[PASS] test_BuildPngList_SimpleCase" << std::endl;
    } else {
        std::cout << "[FAIL] test_BuildPngList_SimpleCase" << std::endl;
    }

    if (test_BuildPngList_NoPngFiles()) {
        std::cout << "[PASS] test_BuildPngList_NoPngFiles" << std::endl;
    } else {
        std::cout << "[FAIL] test_BuildPngList_NoPngFiles" << std::endl;
    }

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    return (g_tests_failed == 0) ? 0 : 1;
}
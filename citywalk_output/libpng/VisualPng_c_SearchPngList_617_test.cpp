/*
Step 1: Program Understanding - Candidate Keywords
Candidate Keywords representing the focal method's core dependencies:
- pFileList: contiguous buffer containing FileCount fixed-size MAX_PATH entries.
- FileCount: number of file entries in pFileList.
- pFileIndex: pointer to current index; updated to previous/next cyclically.
- pstrPrevName: destination buffer for previous entry (if non-NULL).
- pstrNextName: destination buffer for next entry (if non-NULL).
- MAX_PATH: fixed block size for each file entry in pFileList.
- BOOL: return type indicating success (TRUE) or failure (FALSE).
- TCHAR, PTSTR: string types used by the function (Windows-like API types).
- The function behavior:
  - If FileCount > 0, compute previous/next entries cyclically and copy their names into pstrPrevName/pstrNextName accordingly, while updating *pFileIndex.
  - If FileCount == 0, return FALSE with no writes.
- Focused logic paths: cycling wrap-around for both previous and next; conditionally writing prev/next; handling NULL pointers for either destination.
*/

/*
Step 2 & 3: Unit Test Generation + Refinement
This test suite targets SearchPngList in VisualPng.c without using GTest.
It assumes a Windows-like environment (BOOL, TCHAR, MAX_PATH, etc.) as used by the focal code.
Tests cover:
- Basic previous/next updates from various starting indices with both prev and next requested.
- Behavior when only next (or only prev) is requested (NULL for the other).
- Wrap-around behavior for both directions.
- Edge case with FileCount == 0.
- Edge case with a single-entry list (FileCount == 1).
- Verification of pFileIndex, prevName, and nextName content after the call.

Compilation note:
- The test code calls the C function with C linkage. Ensure VisualPng.c is compiled and linked with this test.
- Include Windows headers to align type definitions (BOOL, TCHAR, MAX_PATH, PTSTR).
*/

#include <vector>
#include <string.h>
#include <windows.h>
#include <string>
#include <zlib.h>
#include <resource.h>
#include <stdlib.h>
#include <cstdio>
#include <pngfile.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Focal method prototype (C linkage)
extern "C" BOOL SearchPngList (
        TCHAR *pFileList, int FileCount, int *pFileIndex,
        PTSTR pstrPrevName, PTSTR pstrNextName);

// Helper to build a file list buffer: FileCount entries, each MAX_PATH chars long.
static void BuildFileList(int FileCount, std::vector<char>& buffer) {
    buffer.assign(FileCount * MAX_PATH, '\0');
    for (int i = 0; i < FileCount; ++i) {
        char nameBuf[MAX_PATH];
        std::snprintf(nameBuf, MAX_PATH, "image%d.png", i);
        // Copy as C-string into the corresponding block
        char* block = buffer.data() + i * MAX_PATH;
        std::strcpy(block, nameBuf);
        // Remaining bytes in the block are left as '\0' (not strictly required)
    }
}

// Simple test harness helpers
static void logTestResult(const std::string& testName, bool ok, const std::string& details = "") {
    if (ok) {
        printf("[PASS] %s%s\n", testName.c_str(), details.empty() ? "" : (" - " + details).c_str());
    } else {
        printf("[FAIL] %s%s\n", testName.c_str(), details.empty() ? "" : (" - " + details).c_str());
    }
}

// Test 1: Basic previous/next from index 0 with both prev and next requested
static bool test_SearchPngList_BasicPrevNext_FromZero() {
    const int FileCount = 3;
    std::vector<char> fileList;
    BuildFileList(FileCount, fileList);

    int index = 0;
    char prevName[MAX_PATH] = {0};
    char nextName[MAX_PATH] = {0};

    BOOL res = SearchPngList(fileList.data(), FileCount, &index, prevName, nextName);

    bool ok = (res == TRUE) &&
              (index == 0) &&
              (std::strcmp(prevName, "image2.png") == 0) &&
              (std::strcmp(nextName, "image0.png") == 0);

    if (!ok) {
        // Detailed failure reporting
        printf("Debug: res=%d, index=%d, prevName='%s', nextName='%s'\n",
               res, index, prevName, nextName);
    }
    return ok;
}

// Test 2: Basic previous/next from index 1
static bool test_SearchPngList_BasicPrevNext_FromOne() {
    const int FileCount = 3;
    std::vector<char> fileList;
    BuildFileList(FileCount, fileList);

    int index = 1;
    char prevName[MAX_PATH] = {0};
    char nextName[MAX_PATH] = {0};

    BOOL res = SearchPngList(fileList.data(), FileCount, &index, prevName, nextName);

    bool ok = (res == TRUE) &&
              (index == 1) && // after operations: prev -> 0, next -> 1
              (std::strcmp(prevName, "image0.png") == 0) &&
              (std::strcmp(nextName, "image1.png") == 0);

    if (!ok) {
        printf("Debug: res=%d, index=%d, prevName='%s', nextName='%s'\n",
               res, index, prevName, nextName);
    }
    return ok;
}

// Test 3: Only next is requested (pstrPrevName is NULL)
static bool test_SearchPngList_NextOnly() {
    const int FileCount = 3;
    std::vector<char> fileList;
    BuildFileList(FileCount, fileList);

    int index = 2;
    char* prevName = NULL;
    char nextName[MAX_PATH] = {0};

    BOOL res = SearchPngList(fileList.data(), FileCount, &index, prevName, nextName);

    bool ok = (res == TRUE) &&
              (index == 0) &&
              (std::strcmp(nextName, "image0.png") == 0);

    if (!ok) {
        printf("Debug: res=%d, index=%d, nextName='%s'\n", res, index, nextName);
    }
    return ok;
}

// Test 4: FileCount == 0 should return FALSE and not modify buffers
static bool test_SearchPngList_ZeroCount() {
    const int FileCount = 0;
    std::vector<char> fileList;
    BuildFileList(FileCount, fileList);

    int index = 5;
    char prevName[MAX_PATH] = "X"; // seed with known value
    char nextName[MAX_PATH] = "Y";

    BOOL res = SearchPngList(fileList.data(), FileCount, &index, prevName, nextName);

    bool ok = (res == FALSE) && (index == 5) && (std::strcmp(prevName, "X") == 0) && (std::strcmp(nextName, "Y") == 0);
    if (!ok) {
        printf("Debug: res=%d, index=%d, prevName='%s', nextName='%s'\n",
               res, index, prevName, nextName);
    }
    return ok;
}

// Test 5: Single entry (FileCount == 1) ensures wrap/identity behavior
static bool test_SearchPngList_SingleEntry() {
    const int FileCount = 1;
    std::vector<char> fileList;
    BuildFileList(FileCount, fileList);

    int index = 0;
    char prevName[MAX_PATH] = {0};
    char nextName[MAX_PATH] = {0};

    BOOL res = SearchPngList(fileList.data(), FileCount, &index, prevName, nextName);

    bool ok = (res == TRUE) &&
              (index == 0) &&
              (std::strcmp(prevName, "image0.png") == 0) &&
              (std::strcmp(nextName, "image0.png") == 0);

    if (!ok) {
        printf("Debug: res=%d, index=%d, prevName='%s', nextName='%s'\n",
               res, index, prevName, nextName);
    }
    return ok;
}

// Test 6: Next wrap-around from last element (index = FileCount-1) with both dests
static bool test_SearchPngList_NextWrapFromLast() {
    const int FileCount = 3;
    std::vector<char> fileList;
    BuildFileList(FileCount, fileList);

    int index = 2;
    char prevName[MAX_PATH] = {0};
    char nextName[MAX_PATH] = {0};

    BOOL res = SearchPngList(fileList.data(), FileCount, &index, prevName, nextName);

    bool ok = (res == TRUE) &&
              (index == 0) && // next wrap to 0
              (std::strcmp(prevName, "image1.png") == 0) && // previous to index 1
              (std::strcmp(nextName, "image0.png") == 0);

    if (!ok) {
        printf("Debug: res=%d, index=%d, prevName='%s', nextName='%s'\n",
               res, index, prevName, nextName);
    }
    return ok;
}

// Main: Run all tests and report summary
int main() {
    // Candidate Keywords and test intent are described above.
    printf("Running unit tests for SearchPngList (VisualPng.c)\n");

    int testsRun = 0;
    int testsPassed = 0;

#define RUN_TEST(fn) do { \
    bool result = (fn)(); \
    testsRun++; \
    if (result) testsPassed++; \
    logTestResult(#fn, result); \
} while (0)

    RUN_TEST(test_SearchPngList_BasicPrevNext_FromZero);
    RUN_TEST(test_SearchPngList_BasicPrevNext_FromOne);
    RUN_TEST(test_SearchPngList_NextOnly);
    RUN_TEST(test_SearchPngList_ZeroCount);
    RUN_TEST(test_SearchPngList_SingleEntry);
    RUN_TEST(test_SearchPngList_NextWrapFromLast);
#undef RUN_TEST

    printf("Test results: %d/%d tests passed.\n", testsPassed, testsRun);
    return (testsPassed == testsRun) ? 0 : 1;
}
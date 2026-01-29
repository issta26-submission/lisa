// Test suite for the focal method CheckLinking (located in testcms2.c)
// This test suite is written in C++11 (no GTest) and relies on the
// C linkage of the function:
//   extern "C" int CheckLinking(void);
// The tests exercise the success path and a failure path (permission-based)
// to improve coverage of the function under test.
// Explanatory comments are provided for each test case.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <limits.h>
#include <iostream>
#include <testcms2.h>
#include <sys/types.h>
#include <cstdlib>
#include <windows.h>
#include <unistd.h>
#include <direct.h>


#ifdef _WIN32
#define PATH_MAX 260
#define GETCWD(_buf,_len) _getcwd(_buf,_len)
#define MKDIR(path, perm) _mkdir(path)
#define RMDIR _rmdir
#else
#define GETCWD(_buf,_len) getcwd(_buf,_len)
#define MKDIR(path, perm) mkdir(path, perm)
#define RMDIR rmdir
#endif

// Forward declaration of the focal function (C linkage)
extern "C" int CheckLinking(void);

// Helper: simple existence check for cleanup
static bool file_exists(const char* path)
{
    FILE* f = std::fopen(path, "rb");
    if (f) { std::fclose(f); return true; }
    return false;
}

// Helper: remove a file if it exists
static void safe_remove(const char* path)
{
    if (file_exists(path)) std::remove(path);
}

// Helper: retrieve current working directory as string
static std::string current_working_dir()
{
    char buf[PATH_MAX];
    if (GETCWD(buf, sizeof(buf)) != NULL) {
        return std::string(buf);
    }
    return std::string();
}

// Test 1: Verify that CheckLinking succeeds in a normal writable directory.
// Expected result: return value == 1.
// This test cleans up any artifacts produced by the focal method.
static bool test_CheckLinking_Success()
{
    std::cout << "TEST: CheckLinking success path in writable directory\n";

    // Ensure no artifacts exist from a previous run
    safe_remove("lcms2link.icc");
    safe_remove("lcms2link2.icc");

    // Remember current working directory
    std::string oldcwd = current_working_dir();
    if (oldcwd.empty()) {
        std::cerr << "Failed to read current working directory.\n";
        return false;
    }

    // Call the focal method
    int result = CheckLinking();

    // Cleanup artifacts created by the focal method
    safe_remove("lcms2link.icc");
    safe_remove("lcms2link2.icc");

    // Restore state
    // No chdir needed here since we didn't modify the CWD in this test

    if (result == 1) {
        std::cout << "PASS: CheckLinking returned 1 in writable dir.\n";
        return true;
    } else {
        std::cerr << "FAIL: CheckLinking did not return 1 in writable dir (returned " << result << ").\n";
        return false;
    }
}

// Test 2: Verify that CheckLinking returns 0 when it cannot write to the
// current directory due to insufficient permissions.
// This simulates a false/negative path to improve branch coverage.
// Note: This test is skipped on Windows due to differing permission semantics.
static bool test_CheckLinking_PermissionDenied()
{
#if defined(_WIN32)
    // Skipping due to platform differences in permission handling.
    std::cout << "SKIP: Permission-denied test not implemented for Windows.\n";
    return true;
#else
    std::cout << "TEST: CheckLinking failure path due to write-permission denial\n";

    // Create a temporary directory with no write permission
    char dirTemplate[] = "/tmp/CheckLinkingTestXXXXXX";
    char* dirName = mkdtemp(dirTemplate);
    if (dirName == nullptr) {
        std::cerr << "Could not create temporary directory for permission test.\n";
        return false;
    }

    // Change permissions to read+execute only (no write)
    // 0555: rwx? -> for directories, write bit is what we disable
    // Using 0555 ensures we cannot create new files in the directory.
    if (chmod(dirName, 0555) != 0) {
        std::perror("chmod");
        // Best effort cleanup
        // Attempt to remove directory if possible
        RMDIR(dirName);
        return false;
    }

    // Change into the read-only directory
    std::string oldcwd = current_working_dir();
    if (oldcwd.empty()) {
        std::cerr << "Failed to read current working directory.\n";
        // Cleanup and skip
        chmod(dirName, 0755);
        RMDIR(dirName);
        return false;
    }

    if (chdir(dirName) != 0) {
        std::perror("chdir");
        // Cleanup and skip
        chmod(dirName, 0755);
        RMDIR(dirName);
        return false;
    }

    // Attempt to run the focal method; expect it to fail due to permission denied
    int result = CheckLinking();

    // Restore environment
    if (chdir(oldcwd.c_str()) != 0) {
        std::perror("chdir (restore)");
        // best effort
    }
    // Restore permissions to allow deletion
    chmod(dirName, 0755);
    // Remove test directory
    RMDIR(dirName);

    if (result == 0) {
        std::cout << "PASS: CheckLinking returned 0 when write-permission was denied.\n";
        return true;
    } else {
        std::cerr << "FAIL: CheckLinking did not return 0 when write-permission was denied (returned " << result << ").\n";
        return false;
    }
#endif
}

int main()
{
    int total = 0;
    int passed = 0;

    // Run test 1: success path
    ++total;
    if (test_CheckLinking_Success()) ++passed;
    std::cout << "\n";

    // Run test 2: permission-denied path (only on non-Windows)
    ++total;
    if (test_CheckLinking_PermissionDenied()) ++passed;
    std::cout << "\n";

    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";

    // Return code: 0 if all tests passed, non-zero otherwise
    return (passed == total) ? 0 : 1;
}
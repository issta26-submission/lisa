/*
Unit Test Suite for CheckV4gamma (cmsInt32Number CheckV4gamma(void))
Target: Validate behavior of the focal method CheckV4gamma located in testcms2.c
Environment: C++11 compatible, no GTest; simple in-process tests with a minimal harness.

Notes and Rationale:
- CheckV4gamma creates a small tone curve, writes a GrayTRCTag into a profile named "v4gamma.icc",
  closes and frees resources, then removes the created file. It returns 1 on success, 0 on failure.
- We cannot modify the focal method; instead, we exercise both success and failure paths by
  controlling the filesystem permissions and environment.
- Test coverage goals:
  - True path: normal environment where file can be created and written; ensure return value is 1
    and that "v4gamma.icc" is removed afterwards.
  - False path: simulate a non-writable directory, so opening the file for writing fails.
    Ensure return value is 0 and no residual file is left behind.
- Cross-platform considerations:
  - Implement POSIX-oriented tests (mkdtemp, chdir, chmod) suitable for Linux/macOS.
  - Windows-specific code is not included to keep focus on the core behavior in typical test environments.
- Static members / private details:
  - The test accesses the focal function via a forward declaration (extern "C" int CheckV4gamma(void)).
  - We do not rely on internal static state; tests interact only via the public entry point.
- Assertions:
  - Implement a lightweight non-terminating assertion mechanism (EXPECT_TRUE) to maximize code execution and coverage.

Candidate Keywords (Step 1):
- cmsOpenProfileFromFileTHR, cmsSetProfileVersion, cmsWriteTag, cmsCloseProfile, cmsFreeToneCurve,
  remove, cmsBuildTabulatedToneCurve16, DbgThread, cmsSigGrayTRCTag, v4gamma.icc
- File IO and error paths: h == NULL check, !cmsWriteTag return path, cleanup sequences

Usage:
- Compile and link against the project/library that provides CheckV4gamma and the LittleCMS API.
- Run the resulting executable; it will report test results on stdout/stderr.

Code (C++11 test suite):

*/

#include <cstring>
#include <sys/stat.h>
#include <cstdio>
#include <limits.h>
#include <testcms2.h>
#include <cstdlib>
#include <unistd.h>
#include <direct.h>


#ifdef _WIN32
#define mkdir(a) _mkdir(a)
#define rmdir _rmdir
#define chdir _chdir
#define getcwd _getcwd
#define access _access
#define F_OK 0
#define PATH_MAX 260
#endif

// Forward declaration of the focal function under test.
// We declare it with C linkage to match the actual implementation in the project.
extern "C" int CheckV4gamma(void);

// Lightweight test harness macros (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        fprintf(stderr, "Test failure: %s (line %d)\n", msg, __LINE__); \
        ++g_failed_tests; \
    } \
} while(0)

static void Test_CheckV4gamma_Success(void)
{
    // Clean slate: ensure no residual file from previous runs
    remove("v4gamma.icc");

    // Execute the focal method in a normal environment
    int ret = CheckV4gamma();

    // True branch: should return success (1)
    EXPECT_TRUE(ret == 1, "CheckV4gamma should return 1 in a writable environment");

    // Post-condition: the created file should have been removed by the function
    int exists = access("v4gamma.icc", F_OK);
    EXPECT_TRUE(exists != 0, "v4gamma.icc should be removed after CheckV4gamma completes");
}

static void Test_CheckV4gamma_NoWritePermission(void)
{
    // Create a temporary directory and make it non-writable to simulate failure
    char tmpl[] = "/tmp/v4gamma_testXXXXXX";
    char* dir = mkdtemp(tmpl);
    if (dir == NULL) {
        fprintf(stderr, "SKIP: Could not create temporary directory for write-permission test\n");
        return;
    }

    // Restrict permissions to read/execute only (no write)
#ifndef _WIN32
    chmod(dir, 0555);
#else
    // Windows fallback (non-POSIX): skip if chmod isn't meaningful
    (void)dir;
#endif

    // Save current working directory
    char oldcwd[PATH_MAX];
    if (getcwd(oldcwd, sizeof(oldcwd)) == NULL) {
        fprintf(stderr, "SKIP: Could not get current working directory\n");
        // Cleanup
        chmod(dir, 0755);
        rmdir(dir);
        return;
    }

    // Change to the non-writable directory
    if (chdir(dir) != 0) {
        fprintf(stderr, "SKIP: Could not change directory to temporary test directory\n");
        chmod(dir, 0755);
        rmdir(dir);
        return;
    }

    // Ensure no residual file exists in the test directory
    remove("v4gamma.icc");

    // Execute the focal method in a non-writable environment
    int ret = CheckV4gamma();

    // False branch: should fail to open/create the profile and return 0
    EXPECT_TRUE(ret == 0, "CheckV4gamma should return 0 when the output directory is not writable");

    // Cleanup: revert to original directory and remove temporary directory
    if (chdir(oldcwd) != 0) {
        fprintf(stderr, "WARNING: Could not restore original working directory after test\n");
    }

#ifndef _WIN32
    // Restore permissions to allow deletion
    chmod(dir, 0755);
#endif
    rmdir(dir);
}

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    fprintf(stdout, "Starting CheckV4gamma unit tests (C++11 harness)\n");

    Test_CheckV4gamma_Success();
    Test_CheckV4gamma_NoWritePermission();

    fprintf(stdout, "Tests completed. Total: %d, Failures: %d\n", g_total_tests, g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests != 0) ? 1 : 0;
}
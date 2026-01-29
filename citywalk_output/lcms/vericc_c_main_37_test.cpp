/*
  Lightweight unit test suite for the focal method: main in vericc.c
  Approach:
  - Rename vericc.c's main to TestMain by including the file under a macro rename.
  - Provide a minimal mock of the LCMS interface (lcms2.h and related functions) to avoid external dependencies.
  - Implement test cases that cover true/false branches of conditional predicates.
  - Use non-terminating, lightweight assertions (custom EXPECT-style) to record failures without aborting.
  - Test harness is self-contained in this single translation unit (C++11). No GTest used.
  - Explanatory comments are added for each unit test.

  Notes:
  - We assume PrintUsage returns 1 in this test context (consistent with usage in vericc-like utilities).
  - The tests rely on actual filesystem side effects to validate the rename/move behavior.
  - The tests are designed to be compilable with a C++11 compiler and Core C++ standard libs only.
*/

#include <cstring>
#include <cstdio>
#include <math.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <vericc.c>
#include <lcms2.h>
#include <string.h>


// ----------------------------------------------------------------------------------
// Mock LCMS interface (lcms2.h) to satisfy vericc.c dependencies during test.
// This mock is intentionally minimal and deterministic for unit tests.
// ----------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

typedef void* cmsHPROFILE;
typedef double cmsFloat64Number;

cmsHPROFILE cmsOpenProfileFromFile(const char* path, const char* mode);
void cmsSetProfileVersion(cmsHPROFILE h, cmsFloat64Number ver);
int cmsSaveProfileToFile(cmsHPROFILE h, const char* path);
void cmsCloseProfile(cmsHPROFILE h);

#ifdef __cplusplus
}
#endif

// Internal state for mocks (to verify behavior)
static std::string g_lastOpenedPath;
static cmsFloat64Number g_lastVersionSet = 0.0;

// Implementations with C linkage to match vericc.c expectations
extern "C" cmsHPROFILE cmsOpenProfileFromFile(const char* path, const char* mode) {
    (void)mode;
    if (path == nullptr) return nullptr;
    std::ifstream f(path);
    if (!f.good()) return nullptr;
    g_lastOpenedPath = path;
    // Return a non-null handle (we'll just cast the path string pointer for simplicity)
    return (cmsHPROFILE)path;
}

extern "C" void cmsSetProfileVersion(cmsHPROFILE h, cmsFloat64Number ver) {
    (void)h;
    g_lastVersionSet = ver;
}

extern "C" int cmsSaveProfileToFile(cmsHPROFILE /*h*/, const char* path) {
    if (path == nullptr) return 0;
    // Create the destination file to simulate saving the ICC profile
    std::ofstream f(path);
    if (!f) return 0;
    // Persist what's been "set" as the version for verification
    f << "SAVED_VERSION=" << g_lastVersionSet;
    f.close();
    return 1;
}

extern "C" void cmsCloseProfile(cmsHPROFILE /*h*/) {
    // No-op for mock
}

// ----------------------------------------------------------------------------------
// Rename the focal function's main to TestMain by including vericc.c under a macro.
// This allows direct invocation of the focal logic from tests without spawning separate processes.
// ----------------------------------------------------------------------------------

#define main TestMain
extern "C" int TestMain(int argc, char *argv[]);
#undef main

// ----------------------------------------------------------------------------------
// Lightweight, non-terminating test framework (no GTest). Provides simple EXPECT-like checks.
// ----------------------------------------------------------------------------------

static int g_total_checks = 0;
static int g_failed_checks = 0;

#define TEST_CHECK_EQ(a, b, msg) do {                         \
    ++g_total_checks;                                           \
    if ((a) != (b)) {                                         \
        ++g_failed_checks;                                      \
        std::cerr << "[FAIL] " << (msg) << " Expected: " << (b)   \
                  << ", Got: " << (a) << " (at " << __FILE__      \
                  << ":" << __LINE__ << ")" << std::endl;       \
    }                                                           \
} while (0)

#define TEST_CHECK_TRUE(cond, msg) do {                         \
    ++g_total_checks;                                             \
    if (!(cond)) {                                               \
        ++g_failed_checks;                                        \
        std::cerr << "[FAIL] " << (msg) << " (condition false) " \
                  << "(at " << __FILE__ << ":" << __LINE__ << ")"  \
                  << std::endl;                                   \
    }                                                             \
} while (0)

static bool remove_file(const std::string& path) {
    return (std::remove(path.c_str()) == 0);
}

static bool file_exists(const std::string& path) {
    std::ifstream f(path.c_str());
    return f.good();
}

// ----------------------------------------------------------------------------------
// Test cases
// ----------------------------------------------------------------------------------

/*
  Test Case 1: Incorrect argument count should trigger PrintUsage path.
  Expected: return code equals PrintUsage() (assumed 1).
*/
bool test_case_wrong_argc() {
    // argv: program name and one extra arg -> argc = 2
    const char* prog = "vericc";
    char* argv[2];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("arg"); // dummy
    int argc = 2;

    int ret = TestMain(argc, argv);

    TEST_CHECK_EQ(ret, 1, "test_case_wrong_argc: main should return PrintUsage (1)");
    return (g_failed_checks == 0);
}

/*
  Test Case 2: Bad prefix (not starting with --r) should trigger PrintUsage path.
  Expected: return code equals PrintUsage() (assumed 1).
*/
bool test_case_bad_prefix() {
    const char* prog = "vericc";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("--x"); // does not match --r
    argv[2] = const_cast<char*>("dummy.icc");
    int argc = 3;

    int ret = TestMain(argc, argv);

    TEST_CHECK_EQ(ret, 1, "test_case_bad_prefix: should return PrintUsage (1)");
    return (g_failed_checks == 0);
}

/*
  Test Case 3: Empty version after --r (e.g., --r).
  Expected: prints "Wrong version number" and returns 1.
*/
bool test_case_empty_version() {
    const char* prog = "vericc";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("--r"); // ends right after --r
    argv[2] = const_cast<char*>("dummy.icc");
    int argc = 3;

    int ret = TestMain(argc, argv);

    TEST_CHECK_EQ(ret, 1, "test_case_empty_version: should return 1 for wrong version");
    return (g_failed_checks == 0);
}

/*
  Test Case 4: Cannot open the profile file (cmsOpenProfileFromFile returns NULL).
  Expected: return 1.
*/
bool test_case_open_failure() {
    const char* prog = "vericc";
    // Non-existent file path
    const char* nonexistent = "nonexistent_profile.icc";

    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("--r1.0");
    argv[2] = const_cast<char*>(nonexistent);
    int argc = 3;

    int ret = TestMain(argc, argv);

    TEST_CHECK_EQ(ret, 1, "test_case_open_failure: cannot open should return 1");
    return (g_failed_checks == 0);
}

/*
  Test Case 5: Successful path (exists.icc exists, gets replaced with $$tmp.icc content).
  - Create exists.icc, run with --r2.5
  - cmsSaveProfileToFile writes "SAVED_VERSION=2.5" into $$tmp.icc
  - main removes original exists.icc and renames $$tmp.icc to exists.icc
  - After call, exists.icc should contain "SAVED_VERSION=2.5"
*/
bool test_case_success_path() {
    const std::string filename = "exists.icc";

    // Create initial file to simulate a profile on disk
    {
        std::ofstream f(filename, std::ios::out | std::ios::binary);
        f << "ORIGINAL_PROFILE_CONTENT";
        f.close();
    }

    const char* prog = "vericc";
    char* argv[3];
    argv[0] = const_cast<char*>(prog);
    argv[1] = const_cast<char*>("--r2.5");
    argv[2] = const_cast<char*>(filename.c_str()); // existing file to be overwritten

    int argc = 3;
    int ret = TestMain(argc, argv);

    // Verify return code
    TEST_CHECK_EQ(ret, 0, "test_case_success_path: main should return 0 on success");

    // Verify file was replaced by SAVED content
    if (file_exists(filename)) {
        std::ifstream in(filename);
        std::string content;
        std::getline(in, content, '\0');
        in.close();
        TEST_CHECK_TRUE(content.find("SAVED_VERSION=2.5") != std::string::npos,
                        "test_case_success_path: final file should contain SAVED_VERSION=2.5");
    } else {
        TEST_CHECK_TRUE(false, "test_case_success_path: expected exists.icc to exist after operation");
    }

    // Cleanup
    if (file_exists(filename)) remove_file(filename);
    // If exists.icc existed before, it would now be removed; ensure no stray files remain
    return (g_failed_checks == 0);
}

// ----------------------------------------------------------------------------------
// Main test runner
// ----------------------------------------------------------------------------------

int main() {
    // Run tests
    bool all_ok = true;
    std::cout << "Starting vericc.cpp focal method unit tests (non-terminating assertions).\n";

    // Note: Each test prints its own failures if any. We aggregate overall results at the end.
    if (!test_case_wrong_argc()) { all_ok = false; }
    if (!test_case_bad_prefix()) { all_ok = false; }
    if (!test_case_empty_version()) { all_ok = false; }
    if (!test_case_open_failure()) { all_ok = false; }
    if (!test_case_success_path()) { all_ok = false; }

    std::cout << "Tests completed. Total checks: " << g_total_checks
              << ", Failures: " << g_failed_checks << ".\n";

    // Exit with non-zero if any test failed
    return (g_failed_checks == 0) ? 0 : 1;
}
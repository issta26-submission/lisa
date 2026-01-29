// This test suite targets the focal method: png_have_vsx(png_structp png_ptr)
// It uses a custom test harness in C++11 (no Google Test) and relies on a
// lightweight interception of fopen to simulate different /proc/cpuinfo conditions.
// The test suite executes from main() and prints results, without terminating on
// first failure, to maximize coverage.

// Important: This test expects to be linked with the implementation file linux.c
// and to be compiled with -DPNG_WARNINGS_SUPPORTED -ldl (for dynamic symbol lookup).

#include <unistd.h>
#include <vector>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <dlfcn.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <png.h>


// -------------------------
// Candidate Keywords (Step 1): 
// - png_have_vsx, /proc/cpuinfo, fopen, fgets, strstr, "altivec supported"
// - PNG_WARNINGS_SUPPORTED, png_warning
// - png_ptr (png_structp), NULL, MAXLINE
// - File I/O control (fopen interception), dynamic linking (dlsym)
// -------------------------

// Domain knowledge helpers (Step 3)
typedef void* png_structp;

// Forward declaration of focal function (C linkage)
extern "C" int png_have_vsx(png_structp);

// Global test control for the fopen interception
enum TestOpenMode {
    TEST_OPEN_TRUE,   // Make /proc/cpuinfo contain "altivec supported"
    TEST_OPEN_FALSE,  // Make /proc/cpuinfo not contain the string
    TEST_OPEN_FAIL      // Simulate fopen failure
};

static TestOpenMode g_test_open_mode = TEST_OPEN_TRUE;

// Capture warning invocations (when PNG_WARNINGS_SUPPORTED is defined)
static bool g_warning_called = false;
static std::string g_warning_message;

// Track temporary files created for simulated cpuinfo content
static std::vector<std::string> g_tmp_files;

// Real fopen pointer obtained via dlsym(RTLD_NEXT, "fopen")
static FILE* (*g_real_fopen)(const char*, const char*) = nullptr;

// Ensure we have the real system fopen (bypasses our interception)
static void ensure_real_fopen() {
    if (g_real_fopen == nullptr) {
        g_real_fopen = (FILE* (*)(const char*, const char*))dlsym(RTLD_NEXT, "fopen");
        if (g_real_fopen == nullptr) {
            // Fallback (should not normally happen); use standard fopen if available
            g_real_fopen = fopen;
        }
    }
}

// Intercepted fopen for all code under test
extern "C" FILE* fopen(const char* path, const char* mode) {
    if (path && std::strcmp(path, "/proc/cpuinfo") == 0) {
        ensure_real_fopen();

        // Simulate different behaviors for tests
        if (g_test_open_mode == TEST_OPEN_FAIL) {
            // Simulate failure to open /proc/cpuinfo
            return nullptr;
        }

        // Create a temporary file containing desired content
        char tmptemplate[] = "/tmp/proc_cpuinfo_XXXXXX";
        int fd = ::mkstemp(tmptemplate);
        if (fd == -1) {
            return nullptr;
        }

        const char* content = nullptr;
        if (g_test_open_mode == TEST_OPEN_TRUE) {
            // Include the "altivec supported" string somewhere in the content
            content = "processor\t:\t0\naltivec supported\n";
        } else {
            // Content without the target string
            content = "processor\t:\t0\n";
        }

        ssize_t written = ::write(fd, content, std::strlen(content));
        ::close(fd);
        (void)written; // suppress unused warning in some builds

        // Open the temporary file with the real fopen (not intercepted) and return it
        FILE* f = g_real_fopen(tmptemplate, "r");
        if (f) {
            g_tmp_files.push_back(std::string(tmptemplate));
        }
        return f;
    }

    // For all other paths, delegate to the real fopen
    ensure_real_fopen();
    return g_real_fopen(path, mode);
}

// Intercepted png_warning to observe warning behavior (enabled via PNG_WARNINGS_SUPPORTED)
extern "C" void png_warning(png_structp, const char* message) {
    g_warning_called = true;
    g_warning_message = (message ? message : "");
}

// Utilities to set test mode and run assertions (Step 3)
static void set_test_open_mode(TestOpenMode mode) {
    g_test_open_mode = mode;
}

// Simple non-terminating assertion helpers
static bool assert_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "[TEST FAILURE] " << msg << std::endl;
        return false;
    }
    return true;
}

static bool assert_eq(int a, int b, const char* msg) {
    if (a != b) {
        std::cerr << "[TEST FAILURE] " << msg << " Expected " << a << " but got " << b << std::endl;
        return false;
    }
    return true;
}

static bool assert_str_eq(const char* a, const char* b, const char* msg) {
    if ((a == nullptr && b != nullptr) || (a != nullptr && b == nullptr) || (std::strcmp(a, b) != 0)) {
        std::cerr << "[TEST FAILURE] " << msg << " Expected '" << (b ? b : "NULL") << "' but got '" << (a ? a : "NULL") << "'" << std::endl;
        return false;
    }
    return true;
}

// Cleanup temporary files at program exit
static void cleanup_tmp_files() {
    for (const auto& fname : g_tmp_files) {
        std::remove(fname.c_str());
    }
    g_tmp_files.clear();
}

// Test 1: True branch when cpuinfo contains "altivec supported"
static bool test_png_have_vsx_true_branch() {
    set_test_open_mode(TEST_OPEN_TRUE);
    g_warning_called = false;
    g_warning_message.clear();

    int result = png_have_vsx(nullptr);

    bool ok = assert_eq(result, 1, "png_have_vsx should return 1 when 'altivec supported' is present");
    ok = ok && assert_true(!g_warning_called, "png_warning should not be called on success");
    return ok;
}

// Test 2: False branch when cpuinfo does not contain "altivec supported"
static bool test_png_have_vsx_false_branch() {
    set_test_open_mode(TEST_OPEN_FALSE);
    g_warning_called = false;
    g_warning_message.clear();

    int result = png_have_vsx(nullptr);

    bool ok = assert_eq(result, 0, "png_have_vsx should return 0 when 'altivec supported' is absent");
    ok = ok && assert_true(!g_warning_called, "png_warning should not be called when content is read successfully but lacks target string");
    return ok;
}

// Test 3: Open failed branch with warning (PNG_WARNINGS_SUPPORTED defined)
static bool test_png_have_vsx_open_failure_warn() {
    set_test_open_mode(TEST_OPEN_FAIL);
    g_warning_called = false;
    g_warning_message.clear();

    int result = png_have_vsx(nullptr);

    bool ok = assert_eq(result, 0, "png_have_vsx should return 0 when fopen fails");
    ok = ok && assert_true(g_warning_called, "png_warning should be called when fopen fails");
    ok = ok && assert_str_eq(g_warning_message.c_str(), "/proc/cpuinfo open failed",
                              "png_warning should be called with correct message");
    return ok;
}

// -------------------------
// Main: Run tests (Step 3 refinement) 
// -------------------------
int main() {
    // Register cleanup for temp files on exit
    if (std::atexit(cleanup_tmp_files) != 0) {
        std::cerr << "Failed to register cleanup handler" << std::endl;
        return 1;
    }

    bool all_passed = true;

    std::cout << "Running tests for png_have_vsx...\n";

    if (!test_png_have_vsx_true_branch()) {
        std::cerr << "Test 1 failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 1 passed.\n";
    }

    if (!test_png_have_vsx_false_branch()) {
        std::cerr << "Test 2 failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 2 passed.\n";
    }

    if (!test_png_have_vsx_open_failure_warn()) {
        std::cerr << "Test 3 failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 3 passed.\n";
    }

    // Final result
    if (all_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}
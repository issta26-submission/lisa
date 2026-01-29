// C++11 unit test suite for the focal method makedir in untgz.c
// Tests are designed to run without GoogleTest. A lightweight
// in-house test harness is provided via main() and custom ASSERT-like checks.
// The tests rely on the real makedir implementation and use the filesystem
// to verify directory creation behavior.

#include <cstring>
#include <cstdio>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the focal C function under test.
// The C function uses a C linkage symbol 'prog', so we declare it here.
extern "C" int makedir(char *newdir);
// The focal C code references a global 'prog' for error messages; define it with C linkage.
extern "C" char *prog;

// Provide a C linkage definition for the test harness to resolve the symbol.
char *prog = "unit_test_makedir";

// Helper: check if a path exists (file or directory)
static bool pathExists(const std::string& path) {
    return access(path.c_str(), F_OK) == 0;
}

// Helper: recursively remove a directory tree (POSIX-compliant, C++).
static bool removeDirRecursive(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string name = entry->d_name;
            if (name == "." || name == "..") continue;
            std::string full = path + "/" + name;
            struct stat st;
            if (stat(full.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
                removeDirRecursive(full);
            } else {
                unlink(full.c_str());
            }
        }
        closedir(dir);
    }
    // Ignore errors; best-effort cleanup
    rmdir(path.c_str());
    return true;
}

// Test 1: makedir should return 0 for an empty input string.
static bool test_empty_string() {
    char buf[16];
    std::memset(buf, 0, sizeof(buf));
    // Explicitly set to an empty C-string
    buf[0] = '\0';
    int res = makedir(buf);
    bool ok = (res == 0);
    if (!ok) {
        std::cerr << "[FAIL] test_empty_string: expected 0, got " << res << "\n";
    } else {
        std::cout << "[PASS] test_empty_string\n";
    }
    return ok;
}

// Test 2: makedir should create nested directories by progressively creating parents.
// Example: path "./makedirs_nested/a/b" should create "makedirs_nested", "makedirs_nested/a" and "makedirs_nested/a/b".
static bool test_create_nested_dirs() {
    std::string base = "./makedirs_nested_test";
    // Clean up any previous remnants
    if (pathExists(base)) removeDirRecursive(base);

    std::string path = base + "/a/b";
    char buf[1024];
    std::strcpy(buf, path.c_str());

    int res = makedir(buf);
    bool ok = (res == 1) && pathExists(path);
    if (!ok) {
        std::cerr << "[FAIL] test_create_nested_dirs: makedir()=" << res
                  << ", exists(" << path << ")=" << pathExists(path) << "\n";
    } else {
        std::cout << "[PASS] test_create_nested_dirs\n";
    }

    // Cleanup
    removeDirRecursive(base);
    return ok;
}

// Test 3: makedir should handle trailing slash by trimming it and creating the directory.
static bool test_trailing_slash() {
    std::string base = "./makedirs_trailing_test";
    if (pathExists(base)) removeDirRecursive(base);

    std::string path = base + "/subdir/";
    char buf[1024];
    std::strcpy(buf, path.c_str());

    int res = makedir(buf);
    std::string finalPath = base + "/subdir";
    bool ok = (res == 1) && pathExists(finalPath);
    if (!ok) {
        std::cerr << "[FAIL] test_trailing_slash: makedir()=" << res
                  << ", exists(" << finalPath << ")=" << pathExists(finalPath) << "\n";
    } else {
        std::cout << "[PASS] test_trailing_slash\n";
    }

    // Cleanup
    removeDirRecursive(base);
    return ok;
}

// Test 4: If the target path already exists, makedir should return 1 and leave the path existing.
static bool test_existing_path() {
    std::string base = "./makedirs_existing_test";
    if (pathExists(base)) removeDirRecursive(base);
    // Pre-create the directory to simulate "already exists"
    if (mkdir(base.c_str(), 0755) != 0) {
        int saved = errno;
        std::cerr << "[WARN] test_existing_path: could not create base '" << base
                  << "': " << std::strerror(saved) << "\n";
        // Fall back to skipping this test gracefully
        return true;
    }

    char buf[1024];
    std::strcpy(buf, base.c_str());

    int res = makedir(buf);
    bool ok = (res == 1) && pathExists(base);
    if (!ok) {
        std::cerr << "[FAIL] test_existing_path: makedir()=" << res
                  << ", exists(" << base << ")=" << pathExists(base) << "\n";
    } else {
        std::cout << "[PASS] test_existing_path\n";
    }

    // Cleanup
    removeDirRecursive(base);
    return ok;
}

// Simple test runner
int main() {
    int total = 4;
    int passed = 0;

    // Run tests
    if (test_empty_string()) ++passed;
    if (test_create_nested_dirs()) ++passed;
    if (test_trailing_slash()) ++passed;
    if (test_existing_path()) ++passed;

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";
    // Exit with non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
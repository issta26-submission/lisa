// C++11 test suite for the focal function: file_uncompress
// This test suite uses a lightweight, non-GTest style harness suitable
// for environments where only standard C/C++ facilities are allowed.
// It interacts with the C function defined in minigzip_fuzzer.c via C linkage.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <inttypes.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Forward declaration of the focal function from the C source.
// Ensure it uses C linkage to avoid name mangling.
extern "C" int file_uncompress(char *file);

// Helper: Expose the minimal gzip creation capability for test data.

static bool file_exists(const std::string &path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}

static std::string read_file(const std::string &path) {
    FILE *f = fopen(path.c_str(), "rb");
    if (!f) return "";
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::string data;
    data.resize(len);
    size_t read = fread(&data[0], 1, len, f);
    fclose(f);
    if (read != (size_t)len) return "";
    return data;
}

static bool write_raw_file(const std::string &path, const std::string &content) {
    FILE *f = fopen(path.c_str(), "wb");
    if (!f) return false;
    if (fwrite(content.data(), 1, content.size(), f) != content.size()) {
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}

// Create a gzip file at path containing the given content.
// Uses zlib's gzopen/gzwrite for reliability with the tested function.
static bool create_gzip_file(const std::string &path, const std::string &content) {
    gzFile gz = gzopen(path.c_str(), "wb");
    if (!gz) return false;
    int written = gzwrite(gz, content.data(), (unsigned int)content.size());
    gzclose(gz);
    return written == (int)content.size();
}

// Create a unique temporary directory and return its path.
static std::string create_unique_dir() {
    char tmpl[] = "/tmp/minigzip_test_XXXXXX";
    char *dir = mkdtemp(tmpl);
    if (!dir) return "";
    return std::string(dir);
}

// Remove a directory and its contents recursively (best-effort).
static void remove_recursively(const std::string &path) {
    // Simple implementation: attempt to remove file; if directory exists, try to remove recursively.
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return;
    if (S_ISDIR(st.st_mode)) {
        // Remove contents
        DIR *d = opendir(path.c_str());
        if (!d) { rmdir(path.c_str()); return; }
        struct dirent *entry;
        while ((entry = readdir(d)) != NULL) {
            std::string name = entry->d_name;
            if (name == "." || name == "..") continue;
            std::string full = path + "/" + name;
            remove_recursively(full);
        }
        closedir(d);
        rmdir(path.c_str());
    } else {
        unlink(path.c_str());
    }
}

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define RUN_TEST(test_fn) do { \
    const char* _name = #test_fn; \
    bool _ok = (test_fn)(); \
    ++g_tests_run; \
    if (_ok) { \
        printf("[PASS] %s\n", _name); \
    } else { \
        ++g_tests_failed; \
        printf("[FAIL] %s\n", _name); \
    } \
} while (0)


// Test 1: Long filename triggers the "filename too long" error path.
static bool test_filename_too_long() {
    // Build an excessively long filename to trigger the first guard.
    const int LONG_LEN = 1000; // well beyond typical MAX_NAME_LEN
    char *longname = new char[LONG_LEN + 1];
    for (int i = 0; i < LONG_LEN; ++i) longname[i] = 'a';
    longname[LONG_LEN] = '\0';

    int ret = file_uncompress(longname);

    delete[] longname;

    // Expect failure (return 0) due to filename length guard.
    return (ret == 0);
}

// Test 2: Decompress a non-suffixed file by creating input.gz and verifying output.
// File: <tmpdir>/sample.gz -> decompress to <tmpdir>/sample
static bool test_decompress_non_suffix_path() {
    std::string dir = create_unique_dir();
    if (dir.empty()) return false;

    std::string gzpath = dir + "/sample.gz";
    std::string outpath = dir + "/sample";

    // Create gzip with content "HELLO"
    if (!create_gzip_file(gzpath, "HELLO")) {
        remove_recursively(dir);
        return false;
    }

    // Call file_uncompress with non-suffixed filename
    std::string arg = outpath;
    char *carg = new char[arg.size() + 1];
    std::strcpy(carg, arg.c_str());

    int ret = file_uncompress(carg);
    delete[] carg;

    // Verify results
    bool ok = (ret == 0);
    ok = ok && file_exists(outpath);
    if (ok) {
        std::string content = read_file(outpath);
        ok = (content == "HELLO");
    }

    // The input gz should be removed
    ok = ok && !file_exists(gzpath);

    // Cleanup
    remove_recursively(dir);
    return ok;
}

// Test 3: Decompress a file that already ends with .gz (suffix path).
// File: <tmpdir>/data.gz -> decompress to <tmpdir>/data
static bool test_decompress_with_suffix_path() {
    std::string dir = create_unique_dir();
    if (dir.empty()) return false;

    std::string gzpath = dir + "/data.gz";
    std::string outpath = dir + "/data";

    if (!create_gzip_file(gzpath, "WORLD")) {
        remove_recursively(dir);
        return false;
    }

    char *carg = new char[gzpath.size() + 1];
    std::strcpy(carg, gzpath.c_str());

    int ret = file_uncompress(carg);
    delete[] carg;

    bool ok = (ret == 0);
    ok = ok && file_exists(outpath);
    if (ok) {
        std::string content = read_file(outpath);
        ok = (content == "WORLD");
    }
    // In this path, the input gzip should be removed
    ok = ok && !file_exists(gzpath);

    remove_recursively(dir);
    return ok;
}

// Test 4: gzopen (input gzip) failure (non-existent file scenario for suffix/not-suffix).
// We call with a filename that does not exist; gzopen should fail and return 0.
static bool test_gzopen_failure() {
    // Use a clearly non-existent filename; since not-suffixed, it will try to open "<name>.gz"
    const std::string nonexist = "/tmp/minigzip_test_nonexistent_file_xyz";
    char *carg = new char[nonexist.size() + 1];
    std::strcpy(carg, nonexist.c_str());

    int ret = file_uncompress(carg);
    delete[] carg;
    return (ret == 0);
}

// Main entry
int main() {
    // Run all tests
    RUN_TEST(test_filename_too_long);
    RUN_TEST(test_decompress_non_suffix_path);
    RUN_TEST(test_decompress_with_suffix_path);
    RUN_TEST(test_gzopen_failure);

    // Summary
    printf("Test results: %d run, %d failed\n", g_tests_run, g_tests_failed);
    return g_tests_failed == 0 ? 0 : 1;
}
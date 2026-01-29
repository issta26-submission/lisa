/*
Unit test suite for the focal method copymeta in gun.c
- No GTest; a lightweight self-contained test harness is implemented.
- Tests exercise both true and false branches of the metadata-copy path.
- Uses only C/C++ standard facilities and POSIX system calls available on typical UNIX-like systems.
- Accesses the focal function via extern "C" copymeta(char*, char*); assumes the function is linkable (may be static in original; tests rely on the harness linking gun.c with the test).
- Uses non-terminating assertions to allow multiple tests to run in a single pass.
- Test scenarios:
  1) copymeta copies mode bits and access/modify times when from is a regular file.
  2) copymeta returns early when from is a directory (non-regular).
  3) copymeta returns early when from does not exist.
  4) copymeta handles non-existent to gracefully (no crash) and does not create a destination file.
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <utime.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the focal function.
// We assume linkage is available (extern "C" to prevent C++ name mangling).
extern "C" void copymeta(char *from, char *to);

// Simple test harness (no GTest). Provides lightweight assertions that continue on failure.
static int g_test_count = 0;
static int g_fail_count = 0;

#define TEST_BEGIN() do { ++g_test_count; } while(0)
#define ASSERT_TRUE(expr, msg) do { if(!(expr)) { std::cerr << "[TEST " << g_test_count << "] FAIL: " << (msg) << "\n"; ++g_fail_count; } } while(0)
#define ASSERT_EQ(a, b, msg) do { if((a) != (b)) { std::cerr << "[TEST " << g_test_count << "] FAIL: " << (msg) << " (expected " << (a) << " == " << (b) << ")\n"; ++g_fail_count; } } while(0)
#define ASSERT_NE(a, b, msg) do { if((a) == (b)) { std::cerr << "[TEST " << g_test_count << "] FAIL: " << (msg) << " (expected " << (a) << " != " << (b) << ")\n"; ++g_fail_count; } } while(0)

static std::string make_temp_path() {
    // Use tmpnam to obtain a unique path; then use it as a file path.
    // Note: tmpnam is deprecated for security reasons in production code,
    // but acceptable here for test scaffolding.
    char tmp[256];
    if (std::tmpnam(tmp) == nullptr) {
        throw std::runtime_error("Failed to generate temporary path");
    }
    return std::string(tmp);
}

static void remove_path(const std::string& p) {
    if (p.empty()) return;
    unlink(p.c_str());
}

static void write_file(const std::string& path, const std::string& content, mode_t mode) {
    int fd = open(path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (fd < 0) {
        throw std::runtime_error("Failed to create test file: " + path);
    }
    if (!content.empty()) {
        ssize_t w = write(fd, content.data(), content.size());
        (void)w;
    }
    close(fd);
}

static void set_times(const std::string& path, time_t atime, time_t mtime) {
    struct utimbuf ub;
    ub.actime = atime;
    ub.modtime = mtime;
    utime(path.c_str(), &ub);
}

static void stat_times(const std::string& path, time_t &atime, time_t &mtime) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        throw std::runtime_error("stat failed for: " + path);
    }
    atime = st.st_atime;
    mtime = st.st_mtime;
}

static void stat_mode(const std::string& path, mode_t &mode_out) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        throw std::runtime_error("stat failed for: " + path);
    }
    mode_out = st.st_mode & 07777; // compare only permission bits (and related bits)
}

// Test 1: From is a regular file; verify mode bits and times are copied to 'to'.
static void test_copymeta_regular_file_changes_metadata() {
    TEST_BEGIN();
    // Create source file (regular file)
    std::string from = make_temp_path();
    write_file(from, "abc", 0644);
    // Prepare destination file with different mode and times
    std::string to = make_temp_path();
    write_file(to, "dest", 0600);
    // Make sure times differ
    time_t src_atime, src_mtime;
    stat_times(from, src_atime, src_mtime);
    // Set destination times to something else
    time_t dst_atime, dst_mtime;
    stat_times(to, dst_atime, dst_mtime);
    set_times(to, dst_atime - 1000, dst_mtime - 2000);
    // Call focal function
    // Build mutable C strings for passing to C function
    std::vector<char> from_c(from.begin(), from.end()); from_c.push_back('\0');
    std::vector<char> to_c(to.begin(), to.end()); to_c.push_back('\0');
    copymeta(from_c.data(), to_c.data());
    // Validate results
    struct stat st_from, st_to;
    if (stat(from.c_str(), &st_from) != 0 || stat(to.c_str(), &st_to) != 0) {
        std::cerr << "[TEST 1] ERROR: stat failed after copymeta\n";
        ++g_fail_count;
        remove_path(from);
        remove_path(to);
        return;
    }
    // Source must be regular
    ASSERT_TRUE((st_from.st_mode & S_IFMT) == S_IFREG, "Source should remain a regular file for test 1");
    // Destination mode bits should equal source's mode bits (mask 07777)
    ASSERT_EQ((st_to.st_mode & 07777), (st_from.st_mode & 07777),
              "Destination mode bits should copy from source");
    // Destination times should be equal to source times
    ASSERT_EQ(st_to.st_atime, st_from.st_atime,
              "Destination atime should copy from source");
    ASSERT_EQ(st_to.st_mtime, st_from.st_mtime,
              "Destination mtime should copy from source");
    // Cleanup
    remove_path(from);
    remove_path(to);
}

// Test 2: From is a directory; copymeta should return without changes to 'to'.
static void test_copymeta_from_directory_no_change() {
    TEST_BEGIN();
    // Create source as a directory
    std::string from_dir = make_temp_path();
    if (mkdir(from_dir.c_str(), 0755) != 0) {
        std::cerr << "[TEST 2] ERROR: failed to create directory " << from_dir << "\n";
        ++g_fail_count;
        return;
    }
    // Create destination file with known mode/times
    std::string to = make_temp_path();
    write_file(to, "dest2", 0644);
    time_t src_atime, src_mtime;
    // Record initial times of destination
    stat_times(to, src_atime, src_mtime);
    // Call focal function: should be early-return, no changes to 'to'
    std::vector<char> from_c(from_dir.begin(), from_dir.end()); from_c.push_back('\0');
    std::vector<char> to_c(to.begin(), to.end()); to_c.push_back('\0');
    copymeta(from_c.data(), to_c.data());
    // Verify 'to' unchanged
    struct stat st_to;
    if (stat(to.c_str(), &st_to) != 0) {
        std::cerr << "[TEST 2] ERROR: stat failed for destination after copymeta\n";
        ++g_fail_count;
        remove_path(to);
        // cleanup directory
        rmdir(from_dir.c_str());
        return;
    }
    ASSERT_EQ((st_to.st_mode & 07777), 0644, "Destination mode should remain unchanged for test 2");
    ASSERT_EQ(st_to.st_atime, src_atime, "Destination atime should remain unchanged for test 2");
    ASSERT_EQ(st_to.st_mtime, src_mtime, "Destination mtime should remain unchanged for test 2");
    // Cleanup
    remove_path(to);
    rmdir(from_dir.c_str());
}

// Test 3: From does not exist; copymeta should return without modifying 'to'.
static void test_copymeta_from_nonexistent_no_change() {
    TEST_BEGIN();
    // Create destination with known metadata
    std::string to = make_temp_path();
    write_file(to, "exists", 0644);
    time_t dst_atime_before, dst_mtime_before;
    stat_times(to, dst_atime_before, dst_mtime_before);

    // Non-existent source path
    std::string from = make_temp_path(); // Do not create this path

    std::vector<char> from_c(from.begin(), from.end()); from_c.push_back('\0');
    std::vector<char> to_c(to.begin(), to.end()); to_c.push_back('\0');
    copymeta(from_c.data(), to_c.data());

    // Destination should remain unchanged
    struct stat st_to;
    if (stat(to.c_str(), &st_to) != 0) {
        std::cerr << "[TEST 3] ERROR: stat failed for destination after copymeta\n";
        ++g_fail_count;
        remove_path(to);
        return;
    }
    ASSERT_EQ((st_to.st_mode & 07777), (0644 & 07777),
              "Destination mode should remain unchanged when source does not exist (test 3)");
    ASSERT_EQ(st_to.st_atime, dst_atime_before, "Destination atime should remain unchanged when source does not exist (test 3)");
    ASSERT_EQ(st_to.st_mtime, dst_mtime_before, "Destination mtime should remain unchanged when source does not exist (test 3)");
    // Cleanup
    remove_path(to);
}

// Test 4: To does not exist; copymeta should not crash. No new file should be created.
static void test_copymeta_to_not_exist_no_crash() {
    TEST_BEGIN();
    // Create source regular file
    std::string from = make_temp_path();
    write_file(from, "source", 0644);

    // Ensure destination does not exist
    std::string to = make_temp_path();
    remove_path(to);

    std::vector<char> from_c(from.begin(), from.end()); from_c.push_back('\0');
    std::vector<char> to_c(to.begin(), to.end()); to_c.push_back('\0');
    // Call copymeta; it should handle missing 'to' gracefully (no crash)
    copymeta(from_c.data(), to_c.data());

    // Destination should still not exist
    struct stat st;
    ASSERT_TRUE(access(to.c_str(), F_OK) != 0, "Destination should not be created when 'to' does not exist (test 4)");

    // Cleanup
    remove_path(from);
}

// Main runner
int main() {
    // Run tests
    test_copymeta_regular_file_changes_metadata();
    test_copymeta_from_directory_no_change();
    test_copymeta_from_nonexistent_no_change();
    test_copymeta_to_not_exist_no_crash();

    // Summary
    std::cout << "Test run complete. Total: " << g_test_count << ", Failures: " << g_fail_count << "\n";
    return (g_fail_count == 0) ? 0 : 1;
}
// C++11 test suite for the focal method "main" in wpng.c
// This test harness executes the wpng binary with various arguments
// to validate behavior and basic coverage without requiring
// GTest. It uses a lightweight, self-contained approach with
// a small set of integration-style tests that exercise different
// command-line paths and error conditions.
//
// How to run (on a Linux host with libpng installed and a compiled wpng binary):
// 1) Build the WP PNG converter (producing an executable named ./wpng).
// 2) Compile this test suite (e.g., g++ -std=c++11 -O2 -Wall wpng_test.cpp -o wpng_test).
// 3) Run: ./wpng_test
//
// Notes:
// - The tests create small PGM inputs (binary P5 netspbm format) to drive wpng.
// - The tests verify exit codes and the presence/absence of expected output PNGs.
// - This is an integration-style test suite; it assumes the wpng binary links
//   against libpng and a compatible writepng.h/.c source set.
//
// The tests focus on: basic successful conversion, invalid input handling,
// output file collision, gamma option, invalid background color option,
// and input filename length check (to exercise multiple branches).
//
// Explanatory comments accompany each unit test.

// Include necessary C++ standard headers
#include <unistd.h>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <time.h>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <writepng.h>


using namespace std;

// Path to the executable under test.
// If your environment places the binary elsewhere, adjust this path accordingly.
const string WP_EXECUTABLE = "./wpng";

// Directory used to host test artifacts
const string TEST_DIR = "/tmp/wpng_test_suite";

// Utility: create a directory if it doesn't exist
static bool ensure_dir(const string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
        return true;
    }
    // 0777 permissions for test isolation
    return mkdir(path.c_str(), 0777) == 0;
}

// Utility: check file existence
static bool file_exists(const string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

// Utility: remove file if exists
static void remove_file(const string& path) {
    if (file_exists(path)) {
        unlink(path.c_str());
    }
}

// Utility: write a minimal 1x1 binary PGM (P5) image to the given path
static bool write_pgm_p5_1x1(const string& path) {
    ofstream f(path, ios::binary);
    if (!f.is_open()) return false;
    // Header: P5, width=1, height=1, maxval=255
    f << "P5\n1 1\n255\n";
    unsigned char pix = 0; // single pixel
    f.write(reinterpret_cast<char*>(&pix), 1);
    f.close();
    return true;
}

// Execute the wpng binary with given options and input path.
// Returns exit status extracted from the system() call.
// On failure to spawn, returns -1.
static int run_wpng_with(const string& input_path, const string& options) {
    // Basic sanity check for executable presence
    if (WP_EXECUTABLE.empty()) return -1;
    // Build command; quote input path to handle spaces safely
    string cmd = WP_EXECUTABLE;
    if (!options.empty()) {
        cmd += " " + options;
    }
    cmd += " \"" + input_path + "\"";
    int status = system(cmd.c_str());
#if defined(_WIN32)
    if (status == -1) return -1;
    // On Windows, WIFEXITED/WEXITSTATUS semantics differ; approximate
    return (status & 0xFF);
#else
    if (status == -1) return -1;
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    // If terminated by signal or other, consider as error
    return -1;
#endif
}

// Simple test framework macros
#define TEST(name) bool test_##name()
#define ASSERT_TRUE(cond) do { if (!(cond)) { \
    std::cerr << "Assertion failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    return false; } } while(0)
#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))
#define ASSERT_EQ(a, b) do { if ((a) != (b)) { \
    std::cerr << "Assertion failed: " #a " == " #b " (" << (a) << " != " << (b) << ")" \
    << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    return false; } } while(0)
#define RUN_TEST(name) do { \
    std::cout << "Running " #name "..." << std::endl; \
    if (test_##name()) std::cout << "PASS: " #name "\n"; else { std::cout << "FAIL: " #name "\n"; failing++; } \
} while(0)

int failing = 0;

// Test 1: Basic conversion with a valid 1x1 PGM input; expect exit 0 and output PNG created
TEST(basic_conversion) {
    string inpath = TEST_DIR + "/test1.pgm";
    string outpath = TEST_DIR + "/test1.png";

    // Cleanup any remnants
    remove_file(inpath);
    remove_file(outpath);

    // Prepare input
    if (!write_pgm_p5_1x1(inpath)) {
        std::cerr << "Failed to write test input: " << inpath << "\n";
        return false;
    }

    // Run wpng with no extra options
    int rc = run_wpng_with(inpath, "");
    ASSERT_EQ(rc, 0);

    // Expect output PNG exists
    bool outExists = file_exists(outpath);
    ASSERT_TRUE(outExists);

    // Cleanup
    remove_file(inpath);
    remove_file(outpath);
    return true;
}

// Test 2: Invalid input header; expects non-zero exit
TEST(invalid_input_header) {
    string inpath = TEST_DIR + "/test_invalid.pgm";
    string outpath = TEST_DIR + "/test_invalid.png";

    remove_file(inpath);
    remove_file(outpath);

    // Write an invalid header (not a PNM)
    ofstream f(inpath);
    if (!f.is_open()) return false;
    f << "NOT_A_PNM_HEADER\n";
    f.close();

    int rc = run_wpng_with(inpath, "");
    // Expect error exit (non-zero)
    ASSERT_TRUE(rc != 0);

    // No output should be produced
    ASSERT_FALSE(file_exists(outpath));

    remove_file(inpath);
    remove_file(outpath);
    return true;
}

// Test 3: Output file already exists; expect error
TEST(output_exists) {
    string inpath = TEST_DIR + "/test_exists.pgm";
    string outpath = TEST_DIR + "/test_exists.png";

    remove_file(inpath);
    remove_file(outpath);

    // Create input
    if (!write_pgm_p5_1x1(inpath)) return false;

    // Create pre-existing output file
    ofstream f(outpath, ios::binary);
    if (!f.is_open()) {
        remove_file(inpath);
        return false;
    }
    f.put(0);
    f.close();

    int rc = run_wpng_with(inpath, "");
    // Expect error due to existing output file
    ASSERT_TRUE(rc != 0);

    // Cleanup
    remove_file(inpath);
    remove_file(outpath);
    return true;
}

// Test 4: Gamma option provided; expect exit 0 and output PNG exists
TEST(gamma_option) {
    string inpath = TEST_DIR + "/test_gamma.pgm";
    string outpath = TEST_DIR + "/test_gamma.png";

    remove_file(inpath);
    remove_file(outpath);

    if (!write_pgm_p5_1x1(inpath)) return false;

    int rc = run_wpng_with(inpath, "-gamma 0.5");
    ASSERT_EQ(rc, 0);
    ASSERT_TRUE(file_exists(outpath));

    remove_file(inpath);
    remove_file(outpath);
    return true;
}

// Test 5: Invalid background color option format; expect error
TEST(bgcolor_invalid) {
    string inpath = TEST_DIR + "/test_bg_invalid.pgm";

    remove_file(inpath);
    remove_file(TEST_DIR + "/test_bg_invalid.png");

    if (!write_pgm_p5_1x1(inpath)) { remove_file(inpath); return false; }

    // Invalid bgcolor: missing '#' or wrong length
    int rc = run_wpng_with(inpath, "-bgcolor 123456"); // invalid format
    // Expect non-zero exit due to invalid option parsing
    ASSERT_TRUE(rc != 0);

    remove_file(inpath);
    remove_file(TEST_DIR + "/test_bg_invalid.png");
    return true;
}

// Test 6: Input filename too long (> 250 chars) triggers error
TEST(input_filename_too_long) {
    // Build a long filename (> 250 chars)
    string longName(TEST_DIR);
    longName += "/";
    longName += string(251, 'a'); // length 251
    longName += ".pgm";

    remove_file(longName);
    remove_file(TEST_DIR + "/ghost.png");

    // Attempt to run with a non-existent long filename
    int rc = run_wpng_with(longName, "");
    // Expect error due to filename length check
    ASSERT_TRUE(rc != 0);

    remove_file(longName);
    return true;
}

// Main: run all tests and summarize
int main() {
    // Ensure test directory exists
    ensure_dir(TEST_DIR);

    std::cout << "WPng test suite (C++11 integration tests) starting...\n";

    // Run individual tests
    RUN_TEST(basic_conversion);
    RUN_TEST(invalid_input_header);
    RUN_TEST(output_exists);
    RUN_TEST(gamma_option);
    RUN_TEST(bgcolor_invalid);
    RUN_TEST(input_filename_too_long);

    std::cout << "Test run complete. Failures: " << failing << std::endl;
    return (failing == 0) ? 0 : 1;
}
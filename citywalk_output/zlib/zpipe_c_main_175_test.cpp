// zpipe_unit_test_suite.cpp
// Purpose: Provide a lightweight C++11 test harness for the focal method main
// in zpipe.c by executing the compiled zpipe binary as an external process.
// Notes:
// - Tests are designed to cover true/false branches of argument predicates
//   (argc == 1, argc == 2 with "-d", and usage path).
// - IO is exercised via temporary files to avoid blocking on interactive streams.
// - No GTest or other frameworks are used; a small non-terminating assertion system
//   is implemented to accumulate failures and report a summary at the end.
// - This approach avoids redefining main or relying on internal dependencies.
// - The tests assume the zpipe executable is available as ./zpipe in the test run
//   environment.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <fstream>
#include <cstdlib>


// Simple non-terminating assertion framework
static int gFailures = 0;
#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "ASSERT FAILED: " << (msg) << std::endl; ++gFailures; } } while (false)
#define ASSERT_EQ(a, b, msg) \
    do { if (!((a) == (b))) { std::cerr << "ASSERT FAILED: " << (msg) << " (left=" << (a) << ", right=" << (b) << ")" << std::endl; ++gFailures; } } while (false)

// Helper utilities for file-based IO checks
static bool fileExists(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}
static size_t fileSize(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return 0;
    return static_cast<size_t>(st.st_size);
}
static std::string readFileBinary(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return "";
    return std::string((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
}

// Utility to remove a file if present
static void removeIfExists(const std::string& path) {
    if (fileExists(path)) {
        std::remove(path.c_str());
    }
}

// Domain-oriented comments:
// - We use external process invocation (system()) to exercise zpipe.c's main function
//   in isolation, avoiding conflicts with the test executable's own main.
// - This approach ensures true branch coverage of argc-based logic and
//   relies on the actual zlib-based compression/decompression behavior.

//
// Test 1: argc == 1 path (compression). Provide input via a file and capture compressed output.
// Expectation: zpipe returns Z_OK (0) and produces non-empty compressed data in output file.
//
static void test_main_argc1_compression_path() {
    // Prepare input data
    const std::string inputPath = "zpipe_test_input.txt";
    const std::string outputPath = "zpipe_test_output.bin";

    // Clean up any leftovers
    removeIfExists(inputPath);
    removeIfExists(outputPath);

    {
        std::ofstream fout(inputPath, std::ios::binary);
        const char* payload = "Hello, zpipe!";
        fout.write(payload, std::strlen(payload));
    }

    // Execute: ./zpipe < zpipe_test_input.txt > zpipe_test_output.bin
    std::string cmd = "./zpipe < " + inputPath + " > " + outputPath;
    int rc = std::system(cmd.c_str());
#ifdef _WIN32
    // On Windows, system() return semantics may differ; attempt to extract exit code if available
    int exitCode = 0;
#else
    int exitCode = WEXITSTATUS(rc);
#endif

    // Assertions
    ASSERT_TRUE(exitCode == 0, "zpipe should return Z_OK (0) on compression path with valid input.");
    ASSERT_TRUE(fileExists(outputPath), "Compressed output file should be created.");

    if (fileExists(outputPath)) {
        size_t sz = fileSize(outputPath);
        ASSERT_TRUE(sz > 0, "Compressed output should contain data (non-empty).");
    }

    // Cleanup
    removeIfExists(inputPath);
    removeIfExists(outputPath);
}

// Test 2: argc == 2 and argv[1] == "-d" path (decompression). Use previously generated
// compressed data as input and capture decompressed output. Expect "Hello, zpipe!".
static void test_main_argc2d_decompression_path() {
    // Reuse a compressed file generated in Test 1.
    const std::string inputPath = "zpipe_test_input.txt";
    const std::string compressedPath = "zpipe_test_output.bin";  // from Test 1
    const std::string decompressedPath = "zpipe_test_decompressed.txt";

    // Ensure previous test artifacts exist; if not, attempt to create them by running Test 1 steps
    if (!fileExists(compressedPath)) {
        // Try to create a small compressed file quickly
        // Prepare input data for this fallback path
        std::ofstream fout(inputPath, std::ios::binary);
        const char* payload = "Hello, zpipe!";
        fout.write(payload, std::strlen(payload));
        fout.close();

        std::string cmd = "./zpipe < " + inputPath + " > " + compressedPath;
        int rc = std::system(cmd.c_str());
        (void)rc; // ignore for fallback
        removeIfExists(inputPath);
    }

    // Clean destination
    removeIfExists(decompressedPath);

    // Execute: ./zpipe -d < zpipe_test_output.bin > zpipe_test_decompressed.txt
    std::string cmd = "./zpipe -d < " + compressedPath + " > " + decompressedPath;
    int rc = std::system(cmd.c_str());
#ifdef _WIN32
    int exitCode = 0;
#else
    int exitCode = WEXITSTATUS(rc);
#endif

    // Read decompressed data
    std::string decompressed = readFileBinary(decompressedPath);

    // Assertions
    ASSERT_TRUE(exitCode == 0, "zpipe should return Z_OK (0) on decompression path with valid input.");
    ASSERT_TRUE(!decompressed.empty(), "Decompressed output should not be empty.");
    ASSERT_TRUE(decompressed == "Hello, zpipe!", "Decompressed content should match original input.");

    // Cleanup
    removeIfExists(inputPath);
    removeIfExists(compressedPath);
    removeIfExists(decompressedPath);
}

// Test 3: Incorrect usage path (any argument combination not matching the two valid cases).
// Expectation: error message on stderr and return code 1.
static void test_main_usage_path() {
    const std::string usageErrPath = "zpipe_test_usage_err.txt";
    removeIfExists(usageErrPath);

    // Execute: ./zpipe wrongargs 2> zpipe_test_usage_err.txt
    std::string cmd = "./zpipe wrongargs 2> " + usageErrPath;
    int rc = std::system(cmd.c_str());
#ifdef _WIN32
    int exitCode = 0;
#else
    int exitCode = WEXITSTATUS(rc);
#endif

    // Read error output
    std::string errContent = readFileBinary(usageErrPath);

    // Assertions
    ASSERT_TRUE(exitCode == 1, "zpipe should return 1 for invalid usage.");
    ASSERT_TRUE(!errContent.empty(), "Error output should be captured on stderr.");
    ASSERT_TRUE(errContent.find("zpipe usage: zpipe [-d] < source > dest") != std::string::npos,
                "Error output should contain the usage string.");

    // Cleanup
    removeIfExists(usageErrPath);
}

// Simple test runner
static void runAllTests() {
    test_main_argc1_compression_path();
    test_main_argc2d_decompression_path();
    test_main_usage_path();

    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << gFailures << " TEST(S) FAILED" << std::endl;
    }
}

// Entry point for the test harness
int main() {
    std::cout << "Starting zpipe unit test suite (non-GTest, external process invocation)" << std::endl;
    runAllTests();
    return gFailures; // Non-zero indicates failures
}

// End of test suite
// Notes about test design:
// - Tests invoke the zpipe executable externally via system() to exercise main's IO-driven behavior.
// - Each test is designed to be independent, using unique temporary files and cleaning up afterward.
// - We intentionally avoid private/internal API access or mocking. IO behavior is exercised through
//   real compression/decompression as performed by the zpipe.c implementation.
// - The test suite uses a lightweight, non-terminating assertion mechanism to maximize coverage
//   and allow all tests to run even if one fails.
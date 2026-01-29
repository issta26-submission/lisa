/*
  Unit test suite for the focal method: main in testzlib.c
  Approach:
  - Do not link against GTest. Use a lightweight, self-contained test harness.
  - Execute the focal binary (testzlib.exe) as a subprocess for each test case.
  - Capture stdout/stderr via shell redirection and verify exit codes and output content.
  - Create temporary input files to drive the compress/decompress logic.
  - Use non-terminating assertions (collect results and report at end).
  - This test focuses on key behaviors inferred from the focal method:
      - Argument handling (no args vs. with file path)
      - File read path (success vs. error)
      - Normal compression/decompression flow (verify "compare ok" appears)
      - Parameter override paths (BlockSizeCompress/Uncompress and compression level)
  - Note: The focal method relies on Windows-specific timing helpers and zlib; tests drive the binary externally.
  - How to run:
      - Ensure testzlib.exe is built from provided focal code.
      - Compile this test suite with a C++11 compiler as a separate executable.
      - Run the test binary in the same directory as testzlib.exe or adjust paths accordingly.
*/

#include <cstdio>
#include <cstring>
#include <windows.h>
#include <cerrno>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <zlib.h>
#include <fstream>
#include <utility>
#include <unistd.h>
#include <stdlib.h>
#include <array>
#include <sstream>
#include <cstdlib>


#ifdef _WIN32
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

// Helper to write a small temporary file with given content.
// Returns the path to the created file.
static std::string writeTempFile(const std::string& filenameHint, const std::string& content) {
    std::string path = filenameHint;
    // Ensure unique-ish name by appending a simple counter based on clock
    static int counter = 0;
    ++counter;
    path += "_testzlib_input_";
    path += std::to_string(counter);
    path += ".txt";

    std::ofstream ofs(path, std::ios::binary);
    ofs.write(content.data(), content.size());
    ofs.close();
    return path;
}

// Helper to delete a file if possible
static void removeFileIfExists(const std::string& path) {
#ifdef _WIN32
    _chsize_s(_get_osfhandle(_fileno(nullptr)), 0); // noop to avoid unused warning
#endif
    if (path.empty()) return;
    std::remove(path.c_str());
}

// Helper to run the focal binary with given args and capture stdout.
// The function redirects stdout to a temporary file, runs the process, then reads the file.
// Returns pair(exitCode, stdoutContent).
static std::pair<int, std::string> runFocalBinary(const std::string& binPath, const std::vector<std::string>& args) {
    // Prepare command: binPath + args, redirect stdout/stderr to a temp file
    std::string outPath = "tmp_testzlib_out.txt";

    // Build command line safely by quoting args that contain spaces
    std::ostringstream cmd;
    cmd << "\"" << binPath << "\"";
    for (const auto& a : args) {
        cmd << " ";
        // If argument contains spaces, wrap in quotes
        bool needQuote = a.find(' ') != std::string::npos;
        if (needQuote) cmd << "\"" << a << "\"";
        else cmd << a;
    }
    // Redirect stdout and stderr to a file
    cmd << " > " << outPath << " 2>&1";

    // Execute and capture exit code
#ifdef _WIN32
    // Use _popen/_pclose to retrieve exit status
    FILE* pipe = _popen(cmd.str().c_str(), "r");
#else
    FILE* pipe = popen(cmd.str().c_str(), "r");
#endif
    if (!pipe) {
        std::cerr << "Failed to start focal binary: " << cmd.str() << std::endl;
        return std::make_pair(-1, std::string());
    }

    // Read any immediate output (though we redirected to file, there may be nothing)
    // We keep this to ensure non-blocking behavior.
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        // Intentionally ignore here; real output goes to file
        (void)buffer;
    }

#ifdef _WIN32
    int exitCode = _pclose(pipe);
#else
    int exitCode = pclose(pipe);
#endif

    // Read captured stdout from file
    std::ifstream ifs(outPath, std::ios::binary);
    std::string output((std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));
    ifs.close();

    // Cleanup
    removeFileIfExists(outPath);

    return std::make_pair(exitCode, output);
}

// Simple non-terminating assertion helper
static void expect(bool cond, const std::string& msg,
                   int& failCount, int line = -1) {
    if (!cond) {
        std::cerr << "ASSERTION FAILED";
        if (line >= 0) std::cerr << " (line " << line << ")";
        std::cerr << ": " << msg << std::endl;
        ++failCount;
    }
}

// Test 1: No arguments should print usage and exit with code 0
static void test_no_args(int& failCount) {
    std::cout << "Test 1: No arguments -> usage/exit 0" << std::endl;
    std::string binPath = "testzlib.exe"; // adjust for your environment if needed
    auto res = runFocalBinary(binPath, {}); // no additional args
    int exitCode = res.first;
    const std::string& output = res.second;

    // Expect exit code 0
    expect(exitCode == 0, "Expected exit code 0 for no-args run", failCount, __LINE__);

    // Expect usage message
    bool hasUsage = (output.find("run TestZlib <File>") != std::string::npos);
    expect(hasUsage, "Expected usage message in output when no args are provided", failCount, __LINE__);
}

// Test 2: Non-existent file should error and return non-zero exit code
static void test_nonexistent_input(int& failCount) {
    std::cout << "Test 2: Non-existent input file -> error message and exit 1" << std::endl;
    std::string binPath = "testzlib.exe";

    // Non-existent file name
    std::string fakeFile = "this_file_should_not_exist_12345.txt";

    auto res = runFocalBinary(binPath, { fakeFile });
    int exitCode = res.first;
    const std::string& output = res.second;

    // Expect non-zero exit code (1 as per focal code when ReadFileMemory fails)
    expect(exitCode != 0, "Expected non-zero exit code for missing input file", failCount, __LINE__);

    // Expect error message indicating read failure
    bool hasError = (output.find("error reading") != std::string::npos) ||
                    (output.find("error reading ") != std::string::npos);
    expect(hasError, "Expected error reading message in output for missing file", failCount, __LINE__);
}

// Test 3: Valid input file with default block sizes should compress/decompress and say compare ok
static void test_valid_input_basic(int& failCount) {
    std::cout << "Test 3: Valid input with default blocks -> should print 'compare ok' on success" << std::endl;
    std::string binPath = "testzlib.exe";

    // Create a small content
    std::string content = "The quick brown fox jumps over the lazy dog. 0123456789";
    std::string inputPath = writeTempFile("testzlib_input", content);

    // Run with the input file as argument
    auto res = runFocalBinary(binPath, { inputPath });
    int exitCode = res.first;
    const std::string& output = res.second;

    // Cleanup input file
    removeFileIfExists(inputPath);

    // Expect success
    expect(exitCode == 0, "Expected exit code 0 for valid input", failCount, __LINE__);

    // Expect "compare ok" indicating decompressed data matched
    bool hasCompareOk = (output.find("compare ok") != std::string::npos);
    expect(hasCompareOk, "Expected 'compare ok' in output after successful round-trip", failCount, __LINE__);
}

// Test 4: Valid input with overridden block sizes and compression level
static void test_valid_input_with_overrides(int& failCount) {
    std::cout << "Test 4: Valid input with overridden blocks/level -> should still succeed" << std::endl;
    std::string binPath = "testzlib.exe";

    // Create content larger than Test 3 to stress more data
    std::string content(5000, 'A'); // 5000 bytes of 'A'
    std::string inputPath = writeTempFile("testzlib_input_large", content);

    // Use small block sizes to force many iterations and test the loop
    // Arguments: <file> [BlockSizeCompress] [BlockSizeUncompress] [compressionLevel]
    // Choose values that are plausible but force block-wise processing
    auto res = runFocalBinary(binPath, { inputPath, "512", "512", "1" });
    int exitCode = res.first;
    const std::string& output = res.second;

    // Cleanup
    removeFileIfExists(inputPath);

    // Expect success
    expect(exitCode == 0, "Expected exit code 0 for overridden block sizes", failCount, __LINE__);

    // Expect "compare ok" indicating successful round-trip
    bool hasCompareOk = (output.find("compare ok") != std::string::npos);
    expect(hasCompareOk, "Expected 'compare ok' in output after overridden block sizes", failCount, __LINE__);
}

// Utility: Run all tests and report final result
int main() {
    int failCount = 0;
    int testCount = 0;

    test_no_args(failCount);
    ++testCount;

    test_nonexistent_input(failCount);
    ++testCount;

    test_valid_input_basic(failCount);
    ++testCount;

    test_valid_input_with_overrides(failCount);
    ++testCount;

    if (failCount == 0) {
        std::cout << "ALL TESTS PASSED (" << testCount << " tests)" << std::endl;
        return 0;
    } else {
        std::cerr << failCount << " TEST(S) FAILED OUT OF " << testCount << std::endl;
        return 1;
    }
}
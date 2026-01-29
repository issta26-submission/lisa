#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <string>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


// Forward declaration of the focal method (C linkage)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t dataLen);

/*
  This test harness runs the focal method in isolated subprocesses to catch
  any aborts caused by internal asserts in the function. Each test case is
  designed to exercise different execution paths of the checksum.fuzzer.c
  implementation without modifying its code.
*/

// Simple wrapper to invoke the focal function in a separate process.
// Returns true if the test passed (child exited normally with code 0), false otherwise.
static bool runTestCaseInChild(const std::vector<uint8_t>& data, const std::string& testName) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Failed to fork for test: " << testName << "\n";
        return false;
    }
    if (pid == 0) {
        // Child process: invoke the focal function and exit with its return code.
        const uint8_t* d = data.data();
        size_t len = data.size();
        int ret = LLVMFuzzerTestOneInput(d, len);
        // Use _Exit to avoid C/C++ stdio destructors in the child
        _Exit(ret == 0 ? 0 : 1);
    } else {
        // Parent process: wait for child to finish
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "waitpid failed for test: " << testName << "\n";
            return false;
        }
        if (WIFEXITED(status)) {
            int exitCode = WEXITSTATUS(status);
            bool passed = (exitCode == 0);
            std::cout << "Test '" << testName << "' " << (passed ? "PASSED" : "FAILED") 
                      << " (exit code " << exitCode << ")\n";
            return passed;
        } else if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            std::cout << "Test '" << testName << "' ABORTED by signal " << sig << "\n";
            return false;
        } else {
            std::cout << "Test '" << testName << "' ENDED ABNORMALLY\n";
            return false;
        }
    }
}

// Helper to create a data buffer with a given length and a deterministic pattern
static std::vector<uint8_t> createPatternBuffer(size_t length, uint8_t firstByte) {
    std::vector<uint8_t> data(length);
    if (length == 0) return data;
    data[0] = firstByte;
    for (size_t i = 1; i < length; ++i) {
        data[i] = static_cast<uint8_t>(i & 0xFF);
    }
    return data;
}

int main() {
    bool allPassed = true;

    // Test Case 1: Minimal input where dataLen = 1 and buffSize = 1
    // Intention: exercise the simplest non-trivial path where the loop runs once.
    {
        // dataLen = 1, buffSize = data[0] = 1
        std::vector<uint8_t> data = {1}; // single byte payload
        std::string name = "case_minimal_buffsize_1_len_1";
        std::cout << "Running test: " << name << "\n";
        if (!runTestCaseInChild(data, name)) allPassed = false;
    }

    // Test Case 2: buffSize > dataLen path
    // Intention: exercise the branch where the loop does not execute, but remainder handling occurs.
    {
        // dataLen = 5, buffSize = data[0] = 10 (buffSize > dataLen)
        std::vector<uint8_t> data = {10, 0x01, 0x02, 0x03, 0x04}; // length 5
        std::string name = "case_buffsize_greater_than_len";
        std::cout << "Running test: " << name << "\n";
        if (!runTestCaseInChild(data, name)) allPassed = false;
    }

    // Test Case 3: Moderate data length to exercise chunked processing
    // Intention: exercise multiple iterations of the main for-loop with a modest buffSize.
    {
        size_t dataLen = 1024; // 1KiB
        std::vector<uint8_t> data = createPatternBuffer(dataLen, 64); // first byte = 64
        std::string name = "case_moderate_len_chunked_processing";
        std::cout << "Running test: " << name << "\n";
        if (!runTestCaseInChild(data, name)) allPassed = false;
    }

    // Test Case 4: Maximum allowed size to trigger large input path
    // Intention: exercise the upper bound path and ensure performance/behavior remains correct.
    {
        // dataLen = 1,048,576 (1MB); first byte = 1
        size_t dataLen = 1024 * 1024;
        std::vector<uint8_t> data(dataLen);
        data[0] = 1;
        // Fill rest with a deterministic pattern
        for (size_t i = 1; i < dataLen; ++i) {
            data[i] = static_cast<uint8_t>(i & 0xFF);
        }
        std::string name = "case_max_size_one_mb";
        std::cout << "Running test: " << name << "\n";
        if (!runTestCaseInChild(data, name)) allPassed = false;
    }

    if (allPassed) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}
// Test suite for InitUtils function defined in vprf.c
// Target: C++11, no GTest. Standalone test runner with simple EXPECT-like checks.
// The test relies on external linkage of the C function InitUtils and the global
// array ProgramName which are provided by the library under test.

#include <cstring>
#include <iostream>
#include <string>
#include <utils.h>


// Declare the target function and global variable as C linkage to refer
// to the symbols defined in the C file (vprf.c / vprf.c derived objects).
extern "C" void InitUtils(const char* PName);
extern "C" char ProgramName[];

// Simple non-terminating test assertion macro.
// It records total tests and failures, and prints a message on failure.
// This aligns with the domain guidance to use non-terminating assertions.
static int g_total = 0;
static int g_failed = 0;
#define TEST_ASSERT(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "[TEST FAILED] " << msg \
                  << " (File " << __FILE__ << ", Line " << __LINE__ << ")" << std::endl; \
    } \
} while(false)


// Test 1: Short input should be copied into ProgramName with proper null termination.
// Rationale: If the input length is smaller than the destination array, strncpy copies
// the entire input including its terminating null, and the final N-1 byte is explicitly
// nul-terminated by the subsequent line. This test verifies that ProgramName contains
// a prefix of the input and remains null-terminated.
void TestShortInput() {
    // Prepare a short input string
    const char* input = "Hello"; // length 5
    InitUtils(input);

    // Read content of the global ProgramName as std::string (stops at first NUL)
    std::string got(ProgramName);
    std::string expected = std::string(input).substr(0, got.size());

    // Expect that ProgramName equals the corresponding prefix of input
    TEST_ASSERT(got == expected, "TestShortInput: ProgramName should be prefix of input for short input");
}


// Test 2: Long input should exercise truncation behavior.
// Rationale: If the input length exceeds the destination buffer size, the result in
// ProgramName should be a prefix of the input with the last character forcibly nul-terminated.
// We validate by comparing ProgramName to the corresponding prefix of the long input.
void TestLongInput() {
    // Create a long input string (e.g., 1000 'A's)
    std::string input(1000, 'A');
    InitUtils(input.c_str());

    std::string got(ProgramName);
    std::string expected = input.substr(0, got.size());

    TEST_ASSERT(got == expected, "TestLongInput: ProgramName should be a prefix of long input (truncation case)");
}


// Test 3: Consecutive calls to InitUtils should update ProgramName accordingly.
// Rationale: Ensure that subsequent calls overwrite previous contents as expected.
void TestConsecutiveCalls() {
    // First call
    InitUtils("FirstCallValue");
    std::string gotAfterFirst(ProgramName);

    // Second call with a different input
    InitUtils("SecondCallValueThatMayBeLonger");
    std::string gotAfterSecond(ProgramName);

    // Validate that the second call overwrites the first and preserves prefix semantics
    std::string expectedSecond = std::string("SecondCallValueThatMayBeLonger").substr(0, gotAfterSecond.size());
    TEST_ASSERT(gotAfterSecond == expectedSecond,
                "TestConsecutiveCalls: ProgramName should reflect the second InitUtils input as a prefix");
}


// Entrypoint: run all tests and report summary.
// Uses non-terminating assertions to maximize code execution coverage.
int main() {
    // Run individual tests with explanatory comments
    TestShortInput();
    TestLongInput();
    TestConsecutiveCalls();

    // Summary
    std::cerr << "Total tests: " << g_total << ", Failures: " << g_failed << std::endl;
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}
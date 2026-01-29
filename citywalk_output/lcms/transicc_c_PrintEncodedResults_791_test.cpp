// C++11 compatible unit test suite for the focal function PrintEncodedResults
// Note: The target function PrintEncodedResults is defined in transicc.c and relies on
// several global C variables and functions (e.g., OutputColorSpace, OutputColorant, Verbose, InHexa, Width16,
// cmsChannelsOfColorSpace, cmsNamedColorInfo). In this test suite we provide mock
// implementations for the external C dependencies and exercise true/false branches of predicates.
// The tests are designed to be non-terminating and continue after assertion failures.

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utils.h>


// Step 1: Provide C extern interface for the focal function dependencies so the C++ test
// can link with transicc.c without re-declaring the C++-less headers.
// We only declare the symbols that are used by PrintEncodedResults.

extern "C" {

// Typedefs matching the code in transicc.c (these are typically defined in a common header)
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef int cmsBool;                 // using int as cmsBool in many C LCMS-like headers
typedef double cmsFloat64Number;     // commonly double for 64-bit float

// Global state flags (defined in transicc.c). We declare them extern so tests can modify them.
extern int Verbose;
extern int InHexa;
extern int Width16;
extern int OutputColorSpace;
extern void* OutputColorant;

// Mockable external APIs (mock implementations provided below in test file)
int cmsChannelsOfColorSpace(int space);                                   // returns number of channels for a color space
void cmsNamedColorInfo(void* colorant, int i, char* ChannelName, void*, void*, void*, void*); // fills ChannelName

}

// Step 2: Implement mock dependencies to enable standalone testing of PrintEncodedResults.
// We provide simple, deterministic behavior to cover both branches of condition predicates.

extern "C" {

// Mock: return number of channels based on a simple rule to exercise both 2 and 3 channels.
int cmsChannelsOfColorSpace(int space) {
    // Space 0 -> 3 channels, any other space -> 2 channels
    return (space == 0) ? 3 : 2;
}

// Mock: fill ChannelName depending on whether OutputColorant is NULL or not.
void cmsNamedColorInfo(void* colorant, int i, char* ChannelName, void*, void*, void*, void*) {
    if (ChannelName == nullptr) return;
    // If colorant is non-NULL, simulate named color info; else default channel naming
    if (colorant != NULL) {
        // Example: "ColorName_1", "ColorName_2", ...
        std::string s = "NamedColor_";
        s += std::to_string(i + 1);
        std::strcpy(ChannelName, s.c_str());
    } else {
        // When no colorant, provide a fallback like "Channel #1"
        // The original code uses sprintf in the else branch, but we mimic the same output.
        std::string s = "Channel #";
        s += std::to_string(i + 1);
        std::strcpy(ChannelName, s.c_str());
    }
}

} // extern "C"


// Step 3: Lightweight, non-terminating test framework (no GTest). Tests execute from main().
// We implement a tiny assertion mechanism that logs failures but does not abort tests.

static int g_pass = 0;
static int g_fail = 0;

// Non-terminating assertion macro
#define EXPECT_TRUE(cond, desc) do { \
    if (cond) { g_pass++; } \
    else { g_fail++; std::cerr << "Expectation failed: " << (desc) << std::endl; } \
} while(0)

// Helper to capture stdout produced by PrintEncodedResults
static std::string capture_stdout(std::function<void()> func) {
    // Create a pipe to capture stdout
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        // Fallback: return empty string on error
        return "";
    }

    // Save original stdout
    int stdout_fd = dup(fileno(stdout));
    // Redirect stdout to the write end of the pipe
    dup2(pipe_fd[1], fileno(stdout));
    // Close the write end as it's now dup'd to stdout
    close(pipe_fd[1]);

    // Execute function that prints to stdout
    func();

    // Flush and restore stdout
    fflush(stdout);
    dup2(stdout_fd, fileno(stdout));
    close(stdout_fd);

    // Read captured content from the read end of the pipe
    std::string output;
    const size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    ssize_t bytes_read;
    // The read end is pipe_fd[0]
    while ((bytes_read = read(pipe_fd[0], buffer, BUF_SIZE)) > 0) {
        output.append(buffer, (size_t)bytes_read);
    }
    close(pipe_fd[0]);
    return output;
}


// Step 4: Define test cases that exercise PrintEncodedResults under different configurations.

// Test 1: No OutputColorant, decimal output, 16-bit width, verbose enabled, 3 channels
void test_print_encoded_results_no_colorant_decimal_width16_verbose() {
    // Arrange: set up environment
    extern int Verbose;
    extern int InHexa;
    extern int Width16;
    extern int OutputColorSpace;
    extern void* OutputColorant;

    Verbose = 1;
    InHexa = 0;
    Width16 = 1;
    OutputColorSpace = 0;      // Our cmsChannelsOfColorSpace mock will return 3
    OutputColorant = NULL;      // Force ChannelName to be "Channel #i"

    // Encoded values for 3 channels
    cmsUInt16Number Encoded[3] = {0, 512, 1024};

    // Act: capture stdout during call
    std::string out = capture_stdout([&]() {
        PrintEncodedResults(Encoded);
    });

    // Assert: ensure output contains expected channel labels and values in decimal
    EXPECT_TRUE(out.find("Channel #1=0") != std::string::npos, "Channel 1 should be 0 in decimal");
    EXPECT_TRUE(out.find("Channel #2=512") != std::string::npos, "Channel 2 should be 512 in decimal");
    EXPECT_TRUE(out.find("Channel #3=1024") != std::string::npos, "Channel 3 should be 1024 in decimal");
    // Ensure newline at end
    EXPECT_TRUE(!out.empty() && out.back() == '\n', "Output should end with newline");
}

// Test 2: Hexadecimal output, 16-bit width, single-line (no verbose), 3 channels
void test_print_encoded_results_hex_width16_no_verbose() {
    extern int Verbose;
    extern int InHexa;
    extern int Width16;
    extern int OutputColorSpace;
    extern void* OutputColorant;

    Verbose = 0;
    InHexa = 1;
    Width16 = 1;
    OutputColorSpace = 0;      // 3 channels
    OutputColorant = NULL;      // Not using NamedColorInfo

    // Use small values to check 0x0000, 0x0001, 0x0002 formatting
    cmsUInt16Number Encoded[3] = {0, 1, 2};

    std::string out = capture_stdout([&]() {
        PrintEncodedResults(Encoded);
    });

    // Expected substrings for hex formatting with 4-digit hex
    EXPECT_TRUE(out.find("0x0000") != std::string::npos, "Hex value 0 should print as 0x0000");
    EXPECT_TRUE(out.find("0x0001") != std::string::npos, "Hex value 1 should print as 0x0001");
    EXPECT_TRUE(out.find("0x0002") != std::string::npos, "Hex value 2 should print as 0x0002");
    // Ensure spaces after each value
    size_t pos2 = out.find("0x0001");
    if (pos2 != std::string::npos) {
        // ensure there's a space after 0x0001
        EXPECT_TRUE(pos2 + 6 <= out.size() && out[pos2 + 6] == ' ', "Space after 0x0001");
    }
}

// Test 3: OutputColorant non-NULL to exercise cmsNamedColorInfo path (NamedColorInfo fills ChannelName)
void test_print_encoded_results_with_colorant_named_color() {
    extern int Verbose;
    extern int InHexa;
    extern int Width16;
    extern int OutputColorSpace;
    extern void* OutputColorant;

    Verbose = 1;      // show ChannelName
    InHexa = 0;
    Width16 = 1;
    OutputColorSpace = 0; // 3 channels
    OutputColorant = (void*)1; // non-NULL to trigger cmsNamedColorInfo

    cmsUInt16Number Encoded[3] = {7, 8, 9};

    std::string out = capture_stdout([&]() {
        PrintEncodedResults(Encoded);
    });

    // When colorant is non-NULL, ChannelName should be filled by cmsNamedColorInfo:
    // We implemented as "NamedColor_1", "NamedColor_2", etc.
    EXPECT_TRUE(out.find("NamedColor_1=") != std::string::npos, "Channel name should be NamedColor_1 for first channel");
    EXPECT_TRUE(out.find("NamedColor_2=") != std::string::npos, "Channel name should be NamedColor_2 for second channel");
    EXPECT_TRUE(out.find("NamedColor_3=") != std::string::npos, "Channel name should be NamedColor_3 for third channel");
}

// Step 5: Main entry - run all tests and report summary
int main() {
    // Informational header
    std::cout << "Running unit tests for PrintEncodedResults..." << std::endl;

    // Run tests
    test_print_encoded_results_no_colorant_decimal_width16_verbose();
    test_print_encoded_results_hex_width16_no_verbose();
    test_print_encoded_results_with_colorant_named_color();

    // Summary
    std::cout << "Tests completed. Passed: " << g_pass << "  Failed: " << g_fail << std::endl;

    // Return non-zero on failure to help CI detect issues (but keep tests non-terminating)
    return (g_fail == 0) ? 0 : 1;
}

// Notes for reviewers:
// - The tests rely on the presence of the external C function PrintEncodedResults and
//   its dependent global state that are defined in transicc.c. We provide lightweight
//   mock implementations for cmsChannelsOfColorSpace and cmsNamedColorInfo to exercise
//   both branches of the condition predicates in PrintEncodedResults.
// - We capture stdout by redirecting FILE* stdout to a pipe, allowing us to assert on
//   printed content without terminating the test on failures.
// - Static/global state (Verbose, InHexa, Width16, OutputColorSpace, OutputColorant) is
//   manipulated per-test to cover multiple branches. The test environment assumes PS/Linux-like
//   I/O behavior and POSIX pipe support. If porting to non-POSIX platforms, replace
//   capture_stdout with an alternative I/O capture strategy.
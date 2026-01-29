// This is a self-contained C++11 unit test suite for the focal function:
// png_warning_parameter(png_warning_parameters p, int number, png_const_charp string)
// The tests are designed to run without Google Test (no external test framework).
// They rely only on the C/C++ standard library and a minimal, mock implementation
// of the necessary PNG-related types and functions to exercise the function under test.
//
// Step 1 (Conceptual): Candidate Keywords
// - png_warning_parameter
// - png_warning_parameters (array of buffers/pointers used as parameter targets)
// - PNG_WARNING_PARAMETER_COUNT (range bound for 'number')
// - png_safecat (helper to safely concatenate into a fixed-size buffer)
// - Buffer handling via pointers and sizeof behavior on the element
// These keywords guide the tests: verify in-range writes, out-of-range no-ops, and truncation behavior due to buffer size derived from sizeof(p[number-1]).

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge and test harness assumptions (mocked for unit testing):
// We implement minimal, self-contained equivalents for the PNG-related types and
// functions used by the focal method to enable isolated testing without libpng.

#define PNG_WARNING_PARAMETER_COUNT 4  // Small, fixed count for tests

// Type aliases to mimic libpng style used in the focal code
using png_const_charp = const char*;
using png_charp = char*;
using png_warning_parameters = png_charp[PNG_WARNING_PARAMETER_COUNT];

// Lightweight, self-contained implementation of the required helpers:

// Mocked safe-concatenate: appends 'string' into 'buffer' starting at 'pos',
// respecting 'bufsize' and ensuring null-termination.
// Returns the new end position (not strictly required by tests).
size_t png_safecat(png_charp buffer, size_t bufsize, size_t pos, png_const_charp string)
{
    if (buffer == nullptr || string == nullptr) {
        return pos;
    }
    if (pos >= bufsize) {
        return pos;
    }

    size_t space = bufsize - pos; // available space including potential terminator
    if (space <= 0) {
        return pos;
    }

    size_t copy = std::strlen(string);
    if (copy > space - 1) {
        copy = space - 1; // leave room for terminator
    }

    for (size_t i = 0; i < copy; ++i) {
        buffer[pos + i] = string[i];
    }
    buffer[pos + copy] = '\0';
    return pos + copy;
}

// The focal method under test (as provided in the prompt):
void png_warning_parameter(png_warning_parameters p, int number, png_const_charp string)
{
    {
        // If 'number' is in the valid range, append the string to the corresponding buffer.
        if (number > 0 && number <= PNG_WARNING_PARAMETER_COUNT)
            (void)png_safecat(p[number - 1], (sizeof p[number - 1]), 0, string);
    }
}

// Helper function to create a test scenario with N buffers, each sized as sizeof(void*).
// This matches the peculiar usage of sizeof p[number-1] in the focal code (size of a pointer).
static void prepare_buffers(png_warning_parameters &p, std::vector< std::unique_ptr<char[]> > &allocs)
{
    // Determine buffer size equal to pointer size on this platform
    size_t bufsize = sizeof(void*);

    allocs.clear();
    for (int i = 0; i < PNG_WARNING_PARAMETER_COUNT; ++i) {
        allocs.emplace_back(new char[bufsize]);
        allocs.back()[0] = '\0';
        p[i] = allocs.back().get();
    }
}

// Test 1: In-range writes should copy as much as fits into the fixed-size buffers
// given by sizeof(void*). We verify truncation behavior against the computed buffer size.
bool test_png_warning_parameter_in_range_writes_and_truncates()
{
    png_warning_parameters p;
    std::vector< std::unique_ptr<char[]> > allocs;
    prepare_buffers(p, allocs);

    // Determine actual buffer size used by the function (as dictated by the macro and code).
    size_t bufsize = sizeof(void*); // same as used inside the focal function (sizeof p[number-1])

    // Test with a short string that fits entirely
    const std::string s1 = "hello";
    png_warning_parameter(p, 1, s1.c_str());

    // Expectation: buffer[0] == "hello" if it fits (bufsize >= 6, i.e., at least 5+NUL)
    // The actual max copy is bufsize - 1, so compare against min(strlen(s1), bufsize-1)
    size_t expected_len = (s1.length() < (bufsize - 1)) ? s1.length() : (bufsize - 1);
    std::string actual0 = std::string(p[0]);
    std::string expected0 = s1.substr(0, expected_len);
    if (actual0 != expected0) {
        std::cerr << "test_png_warning_parameter_in_range_writes_and_truncates: "
                  << "buffer[0] expected '" << expected0 << "', got '" << actual0 << "'\n";
        return false;
    }

    // Test with a longer string that will be truncated to fit into the buffer
    const std::string s2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    png_warning_parameter(p, 2, s2.c_str());

    expected_len = (s2.length() < (bufsize - 1)) ? s2.length() : (bufsize - 1);
    std::string actual1 = std::string(p[1]);
    std::string expected1 = s2.substr(0, expected_len);
    if (actual1 != expected1) {
        std::cerr << "test_png_warning_parameter_in_range_writes_and_truncates: "
                  << "buffer[1] expected '" << expected1 << "', got '" << actual1 << "'\n";
        return false;
    }

    // Boundary case within range: number == PNG_WARNING_PARAMETER_COUNT
    const std::string s3 = "1234";
    png_warning_parameter(p, PNG_WARNING_PARAMETER_COUNT, s3.c_str());

    expected_len = (s3.length() < (bufsize - 1)) ? s3.length() : (bufsize - 1);
    std::string actual3 = std::string(p[PNG_WARNING_PARAMETER_COUNT - 1]);
    std::string expected3 = s3.substr(0, expected_len);
    if (actual3 != expected3) {
        std::cerr << "test_png_warning_parameter_in_range_writes_and_truncates: "
                  << "buffer[PNG_WARNING_PARAMETER_COUNT-1] expected '" << expected3
                  << "', got '" << actual3 << "'\n";
        return false;
    }

    return true;
}

// Test 2: Out-of-range numbers should not modify any buffers
bool test_png_warning_parameter_out_of_range_no_write()
{
    png_warning_parameters p;
    std::vector< std::unique_ptr<char[]> > allocs;
    prepare_buffers(p, allocs);

    // Pre-fill buffers to known content
    const std::string initial = "INIT";
    // We fill all buffers with an initial known string ensuring space for comparison
    for (int i = 0; i < PNG_WARNING_PARAMETER_COUNT; ++i) {
        // Ensure there is enough space for "INIT" + '\0': The buffer size is sizeof(void*) so
        // maximum copy is at least 3, which accommodates "INI" etc. We'll fill safely.
        std::string fill = initial;
        size_t bufsize = sizeof(void*);
        // Copy up to bufsize-1 to simulate a pre-filled state
        size_t copy = (fill.length() < (bufsize - 1)) ? fill.length() : (bufsize - 1);
        for (size_t j = 0; j < copy; ++j) p[i][j] = fill[j];
        p[i][copy] = '\0';
    }

    // Save copies to compare after no-op calls
    std::vector<std::string> before;
    for (int i = 0; i < PNG_WARNING_PARAMETER_COUNT; ++i) {
        before.emplace_back(std::string(p[i]));
    }

    // Call with an out-of-range number (0)
    png_warning_parameter(p, 0, "SHOULD_NOT_WRITE");
    // Verify no buffers were modified
    for (int i = 0; i < PNG_WARNING_PARAMETER_COUNT; ++i) {
        if (std::string(p[i]) != before[i]) {
            std::cerr << "test_png_warning_parameter_out_of_range_no_write: buffer[" << i
                      << "] changed on out-of-range call when number=0\n";
            return false;
        }
    }

    // Call with an out-of-range number (> PNG_WARNING_PARAMETER_COUNT)
    png_warning_parameter(p, PNG_WARNING_PARAMETER_COUNT + 1, "SHOULD_NOT_WRITE_EITHER");
    for (int i = 0; i < PNG_WARNING_PARAMETER_COUNT; ++i) {
        if (std::string(p[i]) != before[i]) {
            std::cerr << "test_png_warning_parameter_out_of_range_no_write: buffer[" << i
                      << "] changed on out-of-range call when number=" << (PNG_WARNING_PARAMETER_COUNT + 1) << "\n";
            return false;
        }
    }

    return true;
}

int main() {
    int failures = 0;

    std::cout << "Running test: in-range writes and truncation behavior\n";
    if (!test_png_warning_parameter_in_range_writes_and_truncates()) {
        ++failures;
        std::cerr << "FAILED: test_png_warning_parameter_in_range_writes_and_truncates\n";
    } else {
        std::cout << "PASSED: test_png_warning_parameter_in_range_writes_and_truncates\n";
    }

    std::cout << "Running test: out-of-range numbers do not modify buffers\n";
    if (!test_png_warning_parameter_out_of_range_no_write()) {
        ++failures;
        std::cerr << "FAILED: test_png_warning_parameter_out_of_range_no_write\n";
    } else {
        std::cout << "PASSED: test_png_warning_parameter_out_of_range_no_write\n";
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << failures << " TEST(S) FAILED\n";
        return 1;
    }
}
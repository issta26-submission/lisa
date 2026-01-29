// Unit tests for PrintPCSEncoded in transicc.c using a lightweight, framework-free approach.
// This test suite provides minimal mocks for external dependencies, captures stdout output,
// and validates the exact printed PCS/XYZ values when the internal condition is satisfied.
// The tests are designed to be compiled with C++11 and linked against the transicc.c source.

#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utils.h>


// Forward declaration of the focal function from transicc.c
extern "C" void PrintPCSEncoded(double Input[]); // cmsFloat64Number is typically double

// External global dependencies from the focal class dependency file.
// We declare them as extern C with broad types to avoid requiring Little CMS headers.
extern "C" void* hTransXYZ;
extern "C" void* hTransLab;
extern "C" int Verbose;

// Lightweight, C-linkage mock for cmsDoTransform used by the focal method.
// We implement a simple transform that fills the 3-output elements based on the 3-inputs.
// Distinguish transforms by the h pointer value to reproduce XYZ vs Lab outputs in a deterministic way.
extern "C" void cmsDoTransform(void* h, const void* In, void* Out, unsigned int Size)
{
    // If no proper inputs, bail out
    if (!In || !Out || Size == 0) return;

    const double* in = static_cast<const double*>(In);
    unsigned short* out = static_cast<unsigned short*>(Out);

    if (h == reinterpret_cast<void*>(1)) {
        // For XYZ: map input doubles to 16-bit outputs via multiplication by 256
        unsigned int v0 = static_cast<unsigned int>(in[0] * 256.0);
        unsigned int v1 = static_cast<unsigned int>(in[1] * 256.0);
        unsigned int v2 = static_cast<unsigned int>(in[2] * 256.0);
        out[0] = static_cast<unsigned short>(v0 & 0xFFFF);
        out[1] = static_cast<unsigned short>(v1 & 0xFFFF);
        out[2] = static_cast<unsigned short>(v2 & 0xFFFF);
    } else if (h == reinterpret_cast<void*>(2)) {
        // For Lab: map input doubles to 16-bit outputs via multiplication by 128
        unsigned int v0 = static_cast<unsigned int>(in[0] * 128.0);
        unsigned int v1 = static_cast<unsigned int>(in[1] * 128.0);
        unsigned int v2 = static_cast<unsigned int>(in[2] * 128.0);
        out[0] = static_cast<unsigned short>(v0 & 0xFFFF);
        out[1] = static_cast<unsigned short>(v1 & 0xFFFF);
        out[2] = static_cast<unsigned short>(v2 & 0xFFFF);
    } else {
        // Unknown h: write zeros
        out[0] = out[1] = out[2] = 0;
    }
}

// Utility: capture stdout produced by a function call.
// It redirects stdout to a pipe, invokes the function, then restores stdout and returns the captured string.
std::string captureStdout(void (*func)(double[]), double input[3])
{
    // Create a pipe for capturing stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Fallback: return empty on failure
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        close(pipefd[0]); close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the pipe
    dup2(pipefd[1], fileno(stdout));
    close(pipefd[1]); // Not needed after dup2

    // Flush and invoke function
    fflush(stdout);
    func(input);

    // Restore stdout
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Read captured output
    std::string result;
    char buffer[256];
    ssize_t n;
    // Read in a loop until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        result.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return result;
}

// Accessor wrappers to conform to the expected signature by captureStdout.
// The actual PrintPCSEncoded expects a cmsFloat64Number Input[], which is typically double[].
static void wrapperPrintPCSEncoded(double input[3])
{
    PrintPCSEncoded(input);
}

// Test 1: Verify that when Verbose > 1 and both transforms are present, the printed PCS line
// matches the expected Lab and XYZ hex values derived from the mocked cmsDoTransform.
bool test_PrintPCSEncoded_PrintsExpected()
{
    // Arrange
    Verbose = 2;
    hTransXYZ = reinterpret_cast<void*>(1);
    hTransLab = reinterpret_cast<void*>(2);

    double input[3] = {1.0, 2.0, 3.0};

    // Act: capture stdout produced by PrintPCSEncoded
    std::string output = captureStdout(wrapperPrintPCSEncoded, input);

    // Expected based on mock transforms:
    // XYZ: input * 256 -> (256, 512, 768) => 0x0100, 0x0200, 0x0300
    // Lab: input * 128 -> (128, 256, 384) => 0x0080, 0x0100, 0x0180
    const char* expected = "[PCS] Lab=(0x0080,0x0100,0x0180) XYZ=(0x0100,0x0200,0x0300)\n";

    // Assert
    if (output == expected) {
        std::cout << "TEST_PCS_ECODED_PRINTS_EXPECTED: PASS\n";
        return true;
    } else {
        std::cerr << "TEST_PCS_ECODED_PRINTS_EXPECTED: FAIL\n";
        std::cerr << "Expected: " << expected;
        std::cerr << "Actual: " << output;
        return false;
    }
}

// Test 2: Verify that when Verbose <= 1, no output is produced (false branch).
bool test_PrintPCSEncoded_NoPrintWhenVerboseLow()
{
    // Arrange
    Verbose = 1;
    hTransXYZ = reinterpret_cast<void*>(1);
    hTransLab = reinterpret_cast<void*>(2);

    double input[3] = {1.0, 2.0, 3.0};

    // Act
    std::string output = captureStdout(wrapperPrintPCSEncoded, input);

    // Assert: no output should be produced
    if (output.empty()) {
        std::cout << "TEST_PCS_ECODED_NO_PRINT_VERBOSE_LOW: PASS\n";
        return true;
    } else {
        std::cerr << "TEST_PCS_ECODED_NO_PRINT_VERBOSE_LOW: FAIL\n";
        std::cerr << "Unexpected output: " << output;
        return false;
    }
}

// Test 3: Verify that if transforms are missing (both NULL), no output is produced.
bool test_PrintPCSEncoded_NoPrintWhenTransformsNull()
{
    // Arrange
    Verbose = 3;
    hTransXYZ = nullptr;
    hTransLab = nullptr;

    double input[3] = {1.0, 2.0, 3.0};

    // Act
    std::string output = captureStdout(wrapperPrintPCSEncoded, input);

    // Assert: no output should be produced
    if (output.empty()) {
        std::cout << "TEST_PCS_ECODED_NO_PRINT_TRANSFORMS_NULL: PASS\n";
        return true;
    } else {
        std::cerr << "TEST_PCS_ECODED_NO_PRINT_TRANSFORMS_NULL: FAIL\n";
        std::cerr << "Unexpected output: " << output;
        return false;
    }
}

// Test 4: Verify that if only one transform is present, the condition fails and no output is produced.
// (We keep hTransLab NULL to simulate partial transforms)
bool test_PrintPCSEncoded_PartialTransformsNoPrint()
{
    // Arrange
    Verbose = 4;
    hTransXYZ = reinterpret_cast<void*>(1);
    hTransLab = nullptr;

    double input[3] = {1.0, 2.0, 3.0};

    // Act
    std::string output = captureStdout(wrapperPrintPCSEncoded, input);

    // Assert
    if (output.empty()) {
        std::cout << "TEST_PCS_ECODED_PARTIAL_TRANSFORMS_NO_PRINT: PASS\n";
        return true;
    } else {
        std::cerr << "TEST_PCS_ECODED_PARTIAL_TRANSFORMS_NO_PRINT: FAIL\n";
        std::cerr << "Unexpected output: " << output;
        return false;
    }
}

// Entry point: run all tests and report summary.
int main()
{
    int failures = 0;

    if (!test_PrintPCSEncoded_PrintsExpected()) ++failures;
    if (!test_PrintPCSEncoded_NoPrintWhenVerboseLow()) ++failures;
    if (!test_PrintPCSEncoded_NoPrintWhenTransformsNull()) ++failures;
    if (!test_PrintPCSEncoded_PartialTransformsNoPrint()) ++failures;

    if (failures == 0) {
        std::cout << "ALL_PCS_ENCODER_TESTS_PASSED\n";
        return 0;
    } else {
        std::cerr << "PCS_ENCODER_TESTS_FAILED: " << failures << " test(s) failed.\n";
        return 1;
    }
}
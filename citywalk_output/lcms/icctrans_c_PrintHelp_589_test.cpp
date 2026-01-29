/*
Unit test suite for the focal method PrintHelp in icctrans.c
- No GTest; a lightweight, self-contained test harness is used.
- A mock mexPrintf is provided to capture the output emitted by PrintHelp.
- Tests verify that the expected help text content is present in the captured output.
- Tests are written in C++11 and rely on C linkage for the C functions PrintHelp and mexPrintf.
- The test harness calls the tests from main as per domain guidance.

Notes:
- This test assumes the build environment defines SW appropriately for icctrans.c. If not, define SW via compiler flags (e.g., -DSW=' ').
- The test focuses on coverage of the textual output produced by PrintHelp and uses substring checks to validate content.
*/

#include <cstring>
#include <cstdio>
#include <mex.h>
#include <vector>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2.h>
#include <string.h>


// Global buffer to capture output from the mocked mexPrintf
static std::string g_print_buffer;

// Forward declaration of the focal function (C linkage)
extern "C" void PrintHelp(void);

// Mocked mexPrintf to intercept calls from PrintHelp
extern "C" void mexPrintf(const char* fmt, ...)
{
    // Determine required size
    va_list ap;
    va_start(ap, fmt);
    int needed = vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    if (needed <= 0) {
        return;
    }

    // Allocate buffer and format
    std::vector<char> buf(needed + 1);
    va_start(ap, fmt);
    vsnprintf(buf.data(), needed + 1, fmt, ap);
    va_end(ap);

    // Append formatted output to global buffer (excluding additional null terminator)
    g_print_buffer.append(buf.data(), needed);
}

// Simple unit test framework (lightweight)
static void log_result(const std::string& test_name, bool result)
{
    std::cout << (result ? "[PASS] " : "[FAIL] ") << test_name << "\n";
}

// Test 1: Ensure PrintHelp prints the main header and usage sections
static bool test_PrintHelp_content_header_and_usage()
{
    g_print_buffer.clear();
    // Call the function under test
    PrintHelp();

    const std::string& out = g_print_buffer;

    // Core header string
    if (out.find("(MX) little cms ColorSpace conversion tool - v2.0") == std::string::npos) {
        log_result("test_PrintHelp_content_header_and_usage: header missing", false);
        return false;
    }

    // Usage line
    if (out.find("usage: icctrans (mVar, flags)") == std::string::npos) {
        log_result("test_PrintHelp_content_header_and_usage: usage line missing", false);
        return false;
    }

    // Profiles section introducer
    if (out.find("You can use following built-ins as profiles:") == std::string::npos) {
        log_result("test_PrintHelp_content_header_and_usage: profiles intro missing", false);
        return false;
    }

    // A few representative profile entries
    if (out.find("*Lab2") == std::string::npos ||
        out.find("*XYZ") == std::string::npos ||
        out.find("*sRGB") == std::string::npos) {
        log_result("test_PrintHelp_content_header_and_usage: profile samples missing", false);
        return false;
    }

    // Contact information line (as per the text)
    if (out.find("info@littlecms.com") == std::string::npos) {
        log_result("test_PrintHelp_content_header_and_usage: contact line missing", false);
        return false;
    }

    log_result("test_PrintHelp_content_header_and_usage", true);
    return true;
}

// Test 2: Ensure the complete set of built-in profile entries exist
static bool test_PrintHelp_builtin_profiles_present()
{
    g_print_buffer.clear();
    PrintHelp();
    const std::string& out = g_print_buffer;

    // Check for all key built-in profile tokens
    std::vector<const char*> tokens = {
        "*Lab2", "*Lab4", "*Lab", "*XYZ", "*sRGB",
        "*Gray22", "*Gray30", "*null", "*Lin2222"
    };
    for (const char* t : tokens) {
        if (out.find(t) == std::string::npos) {
            std::string msg = "test_PrintHelp_builtin_profiles_present: missing token ";
            msg += t;
            log_result("test_PrintHelp_builtin_profiles_present", false);
            return false;
        }
    }

    log_result("test_PrintHelp_builtin_profiles_present", true);
    return true;
}

// Entry point for running tests
int main()
{
    // Run tests and exit with non-zero code if any fail
    bool r1 = test_PrintHelp_content_header_and_usage();
    bool r2 = test_PrintHelp_builtin_profiles_present();

    int failures = 0;
    if (!r1) ++failures;
    if (!r2) ++failures;

    std::cout << "\nTest summary: " << (2 - failures) << "/2 tests passed." << std::endl;
    return failures;
}
/*
  Test suite for the focal method: PrintColorantTable (as defined in vprf.c)
  This test harness provides lightweight mocks for the Little CMS API used by
  PrintColorantTable and verifies the true/false branches of its logic.

  Notes:
  - This test uses a minimal, self-contained mock of the cms API to drive the
    behavior of cmsIsTag, cmsReadTag, cmsNamedColorCount and cmsNamedColorInfo.
  - The real PrintColorantTable is assumed to be linked in (e.g., via vprf.c)
    and declared as extern "C" to be callable from C++.
  - Tests capture stdout to compare expected versus actual output without
    terminating on failures (non-terminating assertions style).
  - The test assumes a POSIX-like environment for stdout redirection via freopen.
    If ported to Windows, replace the stdout redirection with appropriate APIs.

  Important: This code is provided as a test harness demonstration. Depending on the
  build system, ensure that vprf.c (containing PrintColorantTable) is linked with
  this test and that the mock cms APIs resolve correctly at link time.
*/

#include <cstring>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utils.h>


// Forward declaration of the focal method to be tested (assumed to be linked in).
extern "C" void PrintColorantTable(void* hInput, unsigned int Sig, const char* Title);

// Typedefs matching the minimal subset of the CMS API used by PrintColorantTable.
// We keep these definitions separate from any real library headers to allow mocking.
typedef void* cmsHPROFILE;
typedef unsigned int cmsTagSignature;
#define cmsMAX_PATH 256

// Lightweight mock structure to represent a named color list.
typedef struct cmsNAMEDCOLORLIST {
    int n;                  // number of named colors
    const char** Names;     // array of C-strings with color names
} cmsNAMEDCOLORLIST;

// ------------------------------------------------------------
// Mock CMS API (minimal, behavior driven by test mode)
// ------------------------------------------------------------
static CMS_UNUSED int test_mode = 0; // 0: list unavailable; 1: list with colors; 2: tag absent

static const char* testColorNames[] = { "Blue", "Green", "Red" }; // deterministic order

static cmsNAMEDCOLORLIST testList; // static list instance used by cmsReadTag

extern "C" void set_test_mode(int mode) {
    test_mode = mode;
    // reset list in mode 1
    if (mode == 1) {
        testList.n = 3;
        testList.Names = testColorNames;
    } else {
        testList.n = 0;
        testList.Names = nullptr;
    }
}

// Simple helper to mimic the library's NULL pointer (for readability)
static cmsNAMEDCOLORLIST* NULL_LIST = nullptr;

extern "C" int cmsIsTag(cmsHPROFILE /*hInput*/, cmsTagSignature /*Sig*/) {
    // Mode semantics:
    // 0 -> tag exists but reads as NULL (unavailable)
    // 1 -> tag exists and has 3 colors
    // 2 -> tag does not exist
    if (test_mode == 0) return 1;
    if (test_mode == 1) return 1;
    if (test_mode == 2) return 0;
    return 0;
}

extern "C" cmsNAMEDCOLORLIST* cmsReadTag(cmsHPROFILE /*hInput*/, cmsTagSignature /*Sig*/) {
    // In mode 0: tag exists but content is unavailable (NULL)
    if (test_mode == 0) return NULL_LIST;
    // In mode 1: return a populated list
    if (test_mode == 1) return &testList;
    // In mode 2: tag not present (shouldn't be called if cmsIsTag returns 0)
    return NULL_LIST;
}

extern "C" int cmsNamedColorCount(cmsNAMEDCOLORLIST* list) {
    if (!list) return 0;
    return list->n;
}

extern "C" void cmsNamedColorInfo(cmsNAMEDCOLORLIST* list, int idx, char* Name, void*, void*, void*, void*) {
    if (!list || idx < 0 || idx >= list->n || !Name) {
        if (Name) Name[0] = '\0';
        return;
    }
    const char* src = list->Names[idx];
    std::strncpy(Name, src, cmsMAX_PATH - 1);
    Name[cmsMAX_PATH - 1] = '\0';
}

// The function under test is PrintColorantTable.
// We declare it here to ensure linkage against the real implementation in vprf.c.
extern "C" void PrintColorantTable(void* hInput, unsigned int Sig, const char* Title);

// ------------------------------------------------------------
// Test harness helpers (non-terminating assertions)
// ------------------------------------------------------------
static int g_total_tests = 0;
static int g_failed_tests = 0;
static std::vector<std::string> g_failures;

#define ASSERT_EQ(expected, actual)                                        \
    do {                                                                   \
        ++g_total_tests;                                                   \
        if ((expected) != (actual)) {                                    \
            ++g_failed_tests;                                            \
            char buf[512];                                               \
            std::snprintf(buf, sizeof(buf),                              \
                          "Assertion failed in %s:%d: expected '%s', got '%s'", \
                          __FILE__, __LINE__, (to_string((expected))).c_str(), \
                          (to_string((actual))).c_str());                \
            g_failures.emplace_back(buf);                               \
        }                                                                \
    } while (false)

template <class T>
std::string to_string(const T& value) {
    try {
        return std::to_string(value);
    } catch (...) {
        // Fallback for non-numeric types
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
}
template <>
std::string to_string<std::string>(const std::string& s) { return s; }
template <>
std::string to_string<const char*>(const char* const& s) { return s ? std::string(s) : std::string("(null)"); }

// Redirect stdout to a file for capturing PrintColorantTable output
class StdoutRedirect {
public:
    StdoutRedirect(const char* filename) {
        // Save current stdout
        _savedFd = dup(fileno(stdout));
        // Redirect stdout to file
        freopen(filename, "w", stdout);
    }
    ~StdoutRedirect() {
        // Flush and restore stdout
        fflush(stdout);
        // Restore the original stdout
        dup2(_savedFd, fileno(stdout));
        close(_savedFd);
    }
private:
    int _savedFd;
};

// Read entire file content into string
static std::string read_file(const char* filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in) return "";
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    return contents;
}

// A small wrapper to run PrintColorantTable and capture its output for a given mode
static std::string run_print_for_mode(int mode) {
    set_test_mode(mode);
    const char* outfile = "test_output.txt";

    // Redirect stdout to capture the function's output
    {
        StdoutRedirect redirect(outfile);
        // hInput and Sig are not used by our test mocks for formatting; 0 suffices.
        PrintColorantTable((void*)0x1, 0x1234, "Colorants");
    }
    // Read captured output
    return read_file(outfile);
}

// ------------------------------------------------------------
// Individual test cases (explanation comments included)
// ------------------------------------------------------------

// Test 1: Tag exists and contents are unavailable (cmsReadTag returns NULL)
// Expected behavior: prints Title, then "(Unavailable)" and a trailing newline.
// This exercises the "if (list == NULL) { printf("(Unavailable)\n"); }" branch.
static void test_case_tag_exists_unavailable() {
    // Mode 0: tag exists, but data unavailable
    std::string output = run_print_for_mode(0);

    // Build expected string
    const char* expected = "Colorants:\n(Unavailable)\n\n";

    // Non-terminating assertion
    ASSERT_EQ(std::string(expected), output);
}

// Test 2: Tag exists with a valid named color list (Blue, Green, Red)
// Expected behavior: prints Title, followed by each color name on its own line, then a blank line.
// This exercises the "for" loop calling cmsNamedColorInfo for each color.
static void test_case_tag_exists_with_colors() {
    // Mode 1: tag exists with a list of 3 colors
    std::string output = run_print_for_mode(1);

    // Build expected string in the exact order provided by testColorNames
    const char* expected = "Colorants:\n\tBlue\n\tGreen\n\tRed\n\n";

    // Non-terminating assertion
    ASSERT_EQ(std::string(expected), output);
}

// Test 3: Tag does not exist (no output expected)
// This exercises the "if (cmsIsTag(...)) { ... }" false branch where nothing should be printed.
static void test_case_tag_absent_no_output() {
    // Mode 2: tag absent
    std::string output = run_print_for_mode(2);

    // Expected empty output
    const char* expected = "";

    // Non-terminating assertion
    ASSERT_EQ(std::string(expected), output);
}

// ------------------------------------------------------------
// Main test runner
// ------------------------------------------------------------
int main() {
    // Run tests
    test_case_tag_exists_unavailable();
    test_case_tag_exists_with_colors();
    test_case_tag_absent_no_output();

    // Report summary
    if (g_failed_tests == 0) {
        std::cout << "All tests passed: " << g_total_tests << " tests run." << std::endl;
        return 0;
    } else {
        std::cout << g_failed_tests << " / " << g_total_tests
                  << " tests failed." << std::endl;
        for (const auto& msg : g_failures) {
            std::cerr << msg << std::endl;
        }
        return 1;
    }
}
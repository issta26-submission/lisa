/*
Unit Test Suite for fPutsUtf8 (console_io.c)

Step 1 - Program Understanding ( summarized in comments )
- FOCAL_METHOD is: SQLITE_INTERNAL_LINKAGE int fPutsUtf8(const char *z, FILE *pfO)
- Core behavior:
  - Asserts z != 0
  - Under CIO_WIN_WC_XLATE: uses Windows code path with PST (per-stream) translation to console; otherwise falls back to standard fputs
  - Non-Windows path (most common in POSIX) returns 0 on failure, or strlen(z) on success
- Key dependent components (Candidate Keywords):
  - z (input C-string), pfO (output FILE*)
  - PST_INITIALIZER, PerStreamTags, getEmitStreamInfo
  - CIO_WIN_WC_XLATE, pstReachesConsole, maybeSetupAsConsole
  - conZstrEmit, isKnownWritable, restoreConsoleArb
  - fputs, strlen
  - Assertion on z != 0
- The function is designed to integrate with a larger IO subsystem; test coverage for the Windows translation path may require deeper mocking which is not assumed here. We focus on the portable, non-Windows code path (common and deterministic) to ensure core behavior is correct.

Step 2 - Unit Test Generation (approach)
- Target the portable path (CIO_WIN_WC_XLATE not defined) which uses fputs and strlen.
- Verify:
  1) Basic write: fPutsUtf8 writes the exact string to the provided FILE* and returns its length.
  2) Empty string: fPutsUtf8 returns 0 and writes nothing.
  3) Large string: fPutsUtf8 handles large inputs correctly and returns the correct length; contents match exactly.
- Test structure: Minimal, self-contained C++11 test harness (no Google Test). Tests call the C function via extern "C" declaration and operate on tmpfile() to avoid filesystem side effects.
- Use only standard library facilities.

Step 3 - Test Case Refinement (domain knowledge)
- Use small, deterministic tests with ASCII strings (safe encoding) and an expanded large string to exercise the length computations.
- Ensure test code compiles under C++11 and can be linked with the console_io.c implementation in typical builds.
- Provide explanatory comments on each unit test describing what is being validated and why.

C++11 Test Suite (no GTest, main-based test runner)

*/

// Candidate Keywords (for reference / traceability)
// fPutsUtf8, z, pfO, PST_INITIALIZER, getEmitStreamInfo, CIO_WIN_WC_XLATE, pstReachesConsole,
// maybeSetupAsConsole, conZstrEmit, isKnownWritable, restoreConsoleArb, fputs, strlen, PerStreamTags

#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>


// Forward declaration of the focal function (C linkage)
extern "C" int fPutsUtf8(const char *z, FILE *pfO);

/*
Test harness notes:
- We avoid Windows-specific translation path testing because it requires deeper hooking/mocking
  of internal static/stateful helpers (PerStreamTags, pstReachesConsole, etc.).
- Instead, we validate the portable path (non-CIO_WIN_WC_XLATE) which uses fputs and strlen.
- Each test prints a short summary and returns 0 on pass, non-zero on failure.
*/

// Simple test assertion helpers
static void test_fail(const char* testName, const std::string& detail) {
    std::cerr << "[FAIL] " << testName << ": " << detail << std::endl;
}

static void test_pass(const char* testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

// Utility: read entire contents of a FILE* into a std::string (assumes file is open for reading)
static std::string readAll(FILE* f) {
    if (!f) return "";
    // Seek to end to determine size, then read back
    long cur = std::ftell(f);
    std::fseek(f, 0, SEEK_END);
    long end = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);

    size_t size = static_cast<size_t>(end - 0);
    std::string content(size, '\0');
    size_t readBytes = std::fread(&content[0], 1, size, f);
    // Restore previous position if needed
    (void)cur;
    (void)readBytes;
    return content;
}

// Test 1: Basic write to a temporary file
// - Verifies that fPutsUtf8 writes the exact string to pfO
// - Verifies that the return value equals the string length
static int test_fPutsUtf8_basic_write() {
    const char* testStr = "hello_world";
    FILE* pfO = tmpfile();
    if (!pfO) {
        std::cerr << "[ERROR] Unable to create temporary file for test_fPutsUtf8_basic_write." << std::endl;
        return 1;
    }

    int ret = fPutsUtf8(testStr, pfO);
    if (ret != static_cast<int>(strlen(testStr))) {
        test_fail("test_fPutsUtf8_basic_write",
                  "Return value mismatch. Expected " + std::to_string(strlen(testStr)) +
                  ", got " + std::to_string(ret));
        std::fclose(pfO);
        return 1;
    }

    // Read back contents to verify exact write
    std::fseek(pfO, 0, SEEK_SET);
    std::string written = readAll(pfO);
    if (written != testStr) {
        test_fail("test_fPutsUtf8_basic_write",
                  "Written content mismatch. Expected '" + std::string(testStr) +
                  "', got '" + written + "'");
        std::fclose(pfO);
        return 1;
    }

    std::fclose(pfO);
    test_pass("test_fPutsUtf8_basic_write");
    return 0;
}

// Test 2: Empty string should write nothing and return 0
static int test_fPutsUtf8_empty_string() {
    const char* testStr = "";
    FILE* pfO = tmpfile();
    if (!pfO) {
        std::cerr << "[ERROR] Unable to create temporary file for test_fPutsUtf8_empty_string." << std::endl;
        return 1;
    }

    int ret = fPutsUtf8(testStr, pfO);
    if (ret != 0) {
        test_fail("test_fPutsUtf8_empty_string",
                  "Return value should be 0 for empty string, got " + std::to_string(ret));
        std::fclose(pfO);
        return 1;
    }

    // Ensure file remains empty
    std::fseek(pfO, 0, SEEK_SET);
    int c = std::fgetc(pfO);
    if (c != EOF) {
        test_fail("test_fPutsUtf8_empty_string",
                  "Expected no data written for empty string.");
        std::fclose(pfO);
        return 1;
    }

    std::fclose(pfO);
    test_pass("test_fPutsUtf8_empty_string");
    return 0;
}

// Test 3: Large string write to ensure handling of longer inputs
static int test_fPutsUtf8_large_string() {
    // Create a large string of 1024 'A's
    std::string largeStr(1024, 'A');
    FILE* pfO = tmpfile();
    if (!pfO) {
        std::cerr << "[ERROR] Unable to create temporary file for test_fPutsUtf8_large_string." << std::endl;
        return 1;
    }

    int ret = fPutsUtf8(largeStr.c_str(), pfO);
    if (ret != static_cast<int>(largeStr.size())) {
        test_fail("test_fPutsUtf8_large_string",
                  "Return value mismatch for large string. Expected " +
                  std::to_string(largeStr.size()) + ", got " + std::to_string(ret));
        std::fclose(pfO);
        return 1;
    }

    std::fseek(pfO, 0, SEEK_SET);
    std::string written = readAll(pfO);
    if (written != largeStr) {
        test_fail("test_fPutsUtf8_large_string",
                  "Large string content mismatch. Expected length " +
                  std::to_string(largeStr.size()) +
                  ", got length " + std::to_string(written.size()));
        std::fclose(pfO);
        return 1;
    }

    std::fclose(pfO);
    test_pass("test_fPutsUtf8_large_string");
    return 0;
}

// Main test runner
int main() {
    int failures = 0;

    // Step 2 tests: basic write path (portable/non-Windows translation)
    failures += test_fPutsUtf8_basic_write();
    failures += test_fPutsUtf8_empty_string();
    failures += test_fPutsUtf8_large_string();

    if (failures == 0) {
        std::cout << "[OVERALL PASS] fPutsUtf8 basic portability tests completed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << "[OVERALL FAIL] fPutsUtf8 portability tests failed with "
                  << failures << " failure(s)." << std::endl;
        return 2;
    }
}
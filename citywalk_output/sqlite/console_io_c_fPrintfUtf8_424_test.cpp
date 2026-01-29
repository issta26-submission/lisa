/*
  Unit Test Suite for fPrintfUtf8 (console_io.c)

  Notes:
  - This test targets the non-console path of fPrintfUtf8 (i.e., CIO_WIN_WC_XLATE not defined),
    which ultimately delegates to vfprintf via the given FILE* stream.
  - We do not rely on Google Test. Tests are executed from main().
  - We assume the test environment provides standard C FILE* APIs (tmpfile, fseek, fread, etc.)
  - The test validates that:
      a) Basic formatting with %d and %s is correctly forwarded to the given FILE*.
      b) Additional variadic formatting with multiple specifiers is correctly handled.
  - We avoid triggering the static/hidden dependencies or Windows-specific branches
    (which would require platform-specific behavior or mocks not available here).
*/

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <iostream>


// Declaration of the focal function under test (C linkage)
extern "C" int fPrintfUtf8(FILE *pfO, const char *zFormat, ...);

// Helper to read an entire FILE* into a string (for assertion)
static std::string readAllFromFile(FILE *f) {
    if (!f) return "";
    // Determine length
    long cur = ftell(f);
    if (cur < 0) cur = 0;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read content
    std::string content;
    content.resize(len);
    size_t readBytes = fread(&content[0], 1, len, f);
    content.resize(readBytes);
    // Restore original position (not strictly required here)
    if (cur >= 0) fseek(f, cur, SEEK_SET);
    return content;
}

// Simple helpers to capture test state (using C linkage for compatibility if needed)
static int g_consoleReached = 0; // unused in this non-console test path but reserved for completeness
static int g_forceIsWritableValue = 1; // unused in this non-console test path

extern "C" int pstReachesConsole(void* ppst) {
    // Non-console path in our test scenario
    return 0;
}
extern "C" void maybeSetupAsConsole(void* ppst, int val) { (void)ppst; (void)val; }
extern "C" int conioVmPrintf(void* ppst, const char* zFormat, va_list ap) {
    // In non-console tests, this path isn't used. Provide a harmless stub.
    (void)ppst; (void)zFormat; (void)ap;
    return 0;
}
extern "C" int isKnownWritable(FILE* pf) { return g_forceIsWritableValue; }
extern "C" void restoreConsoleArb(void* ppst) { (void)ppst; }

// The real project defines these, but they are not used by the non-console test path.
// To avoid linkage issues, we provide minimal stubs with C linkage if the linker requires them.
// If the real console_io.c provides these, the linker will choose the first definition encountered.
// Here, we keep them as no-ops to ensure compilation succeeds in environments where they are linked.
extern "C" void getEmitStreamInfo(unsigned chix, void* ppst, FILE**ppf) { (void)chix; (void)ppst; (void)ppf; }

// Simple test harness
int main() {
    int testsPassed = 0;
    int testsTotal = 0;

    // Test 1: Basic formatting using %d and %s forwarded to the provided FILE*
    {
        testsTotal++;
        bool pass = true;

        FILE *out = tmpfile();
        if (!out) {
            std::cerr << "Test1: failed to create temporary file." << std::endl;
            pass = false;
        } else {
            int rv = fPrintfUtf8(out, "Value=%d Name=%s", 123, "Alice");
            // rv should indicate number of characters written by vfprintf
            // Example: "Value=123 Name=Alice" => 20 chars
            const int expectedRv = 20; // 6 + 3 + 6 + 5
            if (rv != expectedRv) {
                pass = false;
            }

            // Flush and read back content
            fflush(out);
            fseek(out, 0, SEEK_SET);
            std::string content = readAllFromFile(out);
            const std::string expected = "Value=123 Name=Alice";
            if (content != expected) {
                pass = false;
            }

            fclose(out);
        }

        if (pass) {
            std::cout << "Test1 (Basic formatting to FILE*): PASS" << std::endl;
            testsPassed++;
        } else {
            std::cout << "Test1 (Basic formatting to FILE*): FAIL" << std::endl;
        }
    }

    // Test 2: Additional specifiers (%c) to ensure variadic formatting is robust
    {
        testsTotal++;
        bool pass = true;

        FILE *out = tmpfile();
        if (!out) {
            std::cerr << "Test2: failed to create temporary file." << std::endl;
            pass = false;
        } else {
            int rv = fPrintfUtf8(out, "A=%d B=%c C=%s", 7, 'Z', "OK");
            // Expected: "A=7 B=Z C=OK" => length 12
            const int expectedRv = 12;
            if (rv != expectedRv) {
                pass = false;
            }

            fflush(out);
            fseek(out, 0, SEEK_SET);
            std::string content = readAllFromFile(out);
            const std::string expected = "A=7 B=Z C=OK";
            if (content != expected) {
                pass = false;
            }

            fclose(out);
        }

        if (pass) {
            std::cout << "Test2 (Additional specifiers): PASS" << std::endl;
            testsPassed++;
        } else {
            std::cout << "Test2 (Additional specifiers): FAIL" << std::endl;
        }
    }

    // Summary
    std::cout << "Summary: " << testsPassed << " / " << testsTotal << " tests passed." << std::endl;
    return (testsPassed == testsTotal) ? 0 : 1;
}
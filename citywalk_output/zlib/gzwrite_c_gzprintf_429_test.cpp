/*
Unit test suite for gzprintf (wrapper around gzvprintf) in gzwrite.c

Step 1 - Program Understanding (Captured as comments here)
- Focal function under test: int ZEXPORTVA gzprintf(gzFile file, const char *format, ...)
  - It initializes a va_list, forwards to gzvprintf, and ends the va_list.
  - Returns the value produced by gzvprintf (typically the number of characters written on success).
  - Behavior when file is NULL: returns 0 (early exit is visible in the wrapper logic when file == NULL).
- Core dependencies/keywords to consider (Candidate Keywords):
  - gzprintf, gzvprintf, gzFile, va_list, va_start, va_end
  - zlib-based streaming state: gzwrite, gzread, gzopen, gzclose (as relevant to integrated test)
  - Gzip/deflate streaming around a gzFile
  - The wrapper’s role is simply forwarding a formatted string to the underlying gzvprintf
- Testing goal: verify that gzprintf correctly forwards arguments (including variadic handling), returns the expected character count, and behaves correctly when given a NULL file. Also, perform end-to-end verification by decompressing written gzip data to confirm the produced content matches what would be formatted.
- Constraints observed: Do not use GTest; provide a self-contained C++11 test harness; use only standard library and the provided APIs (zlib’s gz* interfaces).

Step 2 - Unit Test Generation (Implemented below)
- Tests implemented:
  1) test_gzprintf_null_file: Ensure gzprintf returns 0 when file is NULL.
  2) test_gzprintf_basic_and_content: Write a simple string "Hi" via gzprintf and verify by decompressing the resulting .gz file content equals "Hi" (end-to-end check).
  3) test_gzprintf_complex_format_and_content: Write a formatted string "Name: %s, Score: %d\n" with args, decompress and verify exact content.
  4) test_gzprintf_percent_escape: Verify handling of "%%" results in a single '%' in the decompressed content.

Step 3 - Test Case Refinement (Domain Knowledge Enhancements)
- Decompression-based verification ensures we validate the actual data produced by the underlying deflate path, not only the return value of gzprintf.
- Additional coverage via checking return values matches the length of the expected unformatted string (as gzprintf should reflect the number of characters produced, i.e., the length of the formatted string).
- Clean up test artifacts after execution to avoid polluting the environment.

Note: The test assumes the presence of zlib (zlib.h) and linking against zlib (-lz). It uses gzopen/gzprintf/gzclose for writing, and then gzopen/gzread to decompress and verify content.

Code (self-contained C++11 test harness with explanatory comments for each test)
*/

#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <gzguts.h>
#include <zlib.h>
#include <cstdlib>


using std::string;
using std::cout;
using std::endl;

/*
Utility: Read a gzip-compressed file and return its uncompressed content.
If read fails, returns empty string.
*/
static string read_gunzip(const string& path) {
    gzFile f = gzopen(path.c_str(), "rb");
    if (f == nullptr) {
        return "";
    }

    const size_t BUF_SIZE = 4096;
    char buf[BUF_SIZE];
    string out;

    int read_bytes = 0;
    while ((read_bytes = gzread(f, buf, BUF_SIZE)) > 0) {
        out.append(buf, buf + read_bytes);
    }

    gzclose(f);

    // gzread returns 0 on end of file; negative on error which we won't distinguish here
    return out;
}

/*
Test 1: gzprintf with NULL file should return 0 (no operation performed).
This covers the guard branch in the wrapper which checks file == NULL.
No file is created in this test.
*/
static bool test_gzprintf_null_file() {
    int ret = gzprintf(nullptr, "Hello");
    // Expect 0 as per wrapper contract
    return ret == 0;
}

/*
Test 2: Basic write and content verification
- Write a simple string "Hi" via gzprintf to a gzip file.
- Decompress the file and verify content is exactly "Hi".
- Also verify the return value equals the length of the formatted string (2).
*/
static bool test_gzprintf_basic_and_content() {
    const string path = "test_gzprintf_basic.gz";

    // Ensure previous artifacts are removed
    remove(path.c_str());

    gzFile f = gzopen(path.c_str(), "wb");
    if (f == nullptr) {
        std::cerr << "Failed to open gz file for writing: " << path << std::endl;
        return false;
    }

    int ret = gzprintf(f, "Hi");
    gzclose(f);

    if (ret != 2) {
        std::cerr << "Expected return 2 from gzprintf for 'Hi', got " << ret << std::endl;
        remove(path.c_str());
        return false;
    }

    string content = read_gunzip(path);
    bool ok = (content == "Hi");
    if (!ok) {
        std::cerr << "Decompressed content mismatch. Got: \"" << content << "\"\n";
    }

    remove(path.c_str());
    return ok;
}

/*
Test 3: Complex formatted string
- Use named placeholders and integers to exercise variadic handling.
- Expected content: "Name: Bob, Score: 42\n"
- Decompress and compare content exactly.
- Also verify return value equals the number of characters produced.
*/
static bool test_gzprintf_complex_format_and_content() {
    const string path = "test_gzprintf_complex.gz";
    remove(path.c_str());

    gzFile f = gzopen(path.c_str(), "wb");
    if (f == nullptr) {
        std::cerr << "Failed to open gz file for writing: " << path << std::endl;
        return false;
    }

    int ret = gzprintf(f, "Name: %s, Score: %d\n", "Bob", 42);
    gzclose(f);

    const string expected = "Name: Bob, Score: 42\n";
    if (ret != (int)expected.size()) {
        std::cerr << "Expected return " << expected.size() << " from gzprintf, got " << ret << std::endl;
        remove(path.c_str());
        return false;
    }

    string content = read_gunzip(path);
    bool ok = (content == expected);
    if (!ok) {
        std::cerr << "Decompressed content mismatch. Expected: \"" << expected
                  << "\" Got: \"" << content << "\"\n";
    }

    remove(path.c_str());
    return ok;
}

/*
Test 4: Percent escape
- Ensure that "%%" in the format results in a single '%' in the output.
- Decompress and verify content is "%".
- Return value should be 1.
*/
static bool test_gzprintf_percent_escape() {
    const string path = "test_gzprintf_percent.gz";
    remove(path.c_str());

    gzFile f = gzopen(path.c_str(), "wb");
    if (f == nullptr) {
        std::cerr << "Failed to open gz file for writing: " << path << std::endl;
        return false;
    }

    int ret = gzprintf(f, "%%");
    gzclose(f);

    const string expected = "%";
    if (ret != (int)expected.size()) {
        std::cerr << "Expected return " << expected.size() << " from gzprintf for '%%', got " << ret << std::endl;
        remove(path.c_str());
        return false;
    }

    string content = read_gunzip(path);
    bool ok = (content == expected);
    if (!ok) {
        std::cerr << "Decompressed content mismatch for percent escape. Expected: \""
                  << expected << "\" Got: \"" << content << "\"\n";
    }

    remove(path.c_str());
    return ok;
}

/*
Main test harness
- Executes all tests and reports pass/fail per test.
- Exits with non-zero status if any test fails.
*/
int main() {
    int total = 4;
    int passed = 0;

    cout << "Running gzprintf unit tests (non-GTest, C++11 harness)..." << endl;

    if (test_gzprintf_null_file()) {
        cout << "[PASS] test_gzprintf_null_file" << endl;
        ++passed;
    } else {
        cout << "[FAIL] test_gzprintf_null_file" << endl;
    }

    if (test_gzprintf_basic_and_content()) {
        cout << "[PASS] test_gzprintf_basic_and_content" << endl;
        ++passed;
    } else {
        cout << "[FAIL] test_gzprintf_basic_and_content" << endl;
    }

    if (test_gzprintf_complex_format_and_content()) {
        cout << "[PASS] test_gzprintf_complex_format_and_content" << endl;
        ++passed;
    } else {
        cout << "[FAIL] test_gzprintf_complex_format_and_content" << endl;
    }

    if (test_gzprintf_percent_escape()) {
        cout << "[PASS] test_gzprintf_percent_escape" << endl;
        ++passed;
    } else {
        cout << "[FAIL] test_gzprintf_percent_escape" << endl;
    }

    cout << "Summary: " << passed << " / " << total << " tests passed." << endl;

    return (passed == total) ? 0 : 1;
}
/************************************************************
 * Unit Test Suite for fGetsUtf8 (console_io.c)
 * - No GTest; a lightweight, non-terminating assertion framework is used
 * - C++11 compatible test harness; tests call the focal C function
 * - Uses standard C/C++ library only
 * 
 * Note: This test targets the non-Windows translation path of fGetsUtf8
 * (i.e., the fgets-based fallback). It assumes CIO_WIN_WC_XLATE is not
 * enabled in the build configuration. If the Windows path is compiled in,
 * tests focusing on the ReadConsoleW/WideCharToMultiByte path would require
 * more extensive Windows emulation; those paths are not exercised here.
 ************************************************************/

#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>


// Ensure C linkage for the focal function
extern "C" {
    // Prototype of the focal function under test
    char* fGetsUtf8(char *cBuf, int ncMax, FILE *pfIn);
}

// Lightweight test harness (non-terminating)
static int g_failures = 0;
#define EXPECT_TRUE(cond, desc) \
    do { \
        if(!(cond)) { \
            std::cerr << "[TEST] " << __FUNCTION__ << ": " << (desc) << " (condition failed)\n"; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_EQ_STR(actual, expected, desc) \
    do { \
        if(std::strcmp((actual), (expected)) != 0) { \
            std::cerr << "[TEST] " << __FUNCTION__ << ": " << (desc) << " | got: \"" << (actual) << "\" expected: \"" << (expected) << "\"\n"; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_EQ_INT(actual, expected, desc) \
    do { \
        if((actual) != (expected)) { \
            std::cerr << "[TEST] " << __FUNCTION__ << ": " << (desc) << " | got: " << (actual) << " expected: " << (expected) << "\n"; \
            ++g_failures; \
        } \
    } while(0)

// Helper: create a temporary file with the given text; returns FILE* positioned at start
static FILE* makeInputFile(const std::string& text) {
    FILE* f = tmpfile();
    if(!f) return nullptr;
    if(!text.empty()) {
        fwrite(text.data(), 1, text.size(), f);
        fflush(f);
    }
    fseek(f, 0, SEEK_SET);
    return f;
}

// Candidate Keywords (core dependencies) mapping (for traceability with Step 1)
//
// - pfIn (input FILE*) / stdin fall-back
// - ncMax (buffer capacity) and line length handling
// - fgets-based path (non-Windows translation path)
// - newline handling (CR/LF semantics are Windows-specific; test here covers basic newline behavior)
// - EOF behavior (empty input yields NULL)
// - Repeated reads across successive calls (stream progression)
// - buffer termination (0-termination of cBuf)
// - Non-Windows code path is exercised when CIO_WIN_WC_XLATE is not enabled

// Test 1: Read first line from a simple two-line input; expect entire first line including newline
static void test_read_first_line() {
    // This test exercises:
    // - fgets-based path
    // - Basic newline handling
    // - Proper 0-termination
    std::string input = "hello\nworld\n";
    FILE* f = makeInputFile(input);
    TESTING_ASSERT: ; // placeholder to satisfy compiler in some environments

    char buf[64] = {0};
    char* res = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(res != nullptr, "First read should return a non-null buffer");
    EXPECT_EQ_STR(buf, "hello\n", "First line should be 'hello\\n'");
    fclose(f);
}

// Test 2: Read second line after repositioning the same stream; expect second line
static void test_read_second_line() {
    std::string input = "line1\nline2\n";
    FILE* f = makeInputFile(input);
    char buf[64] = {0};

    // Read first line
    char* r1 = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(r1 != nullptr, "First read should succeed");
    EXPECT_EQ_STR(buf, "line1\n", "First line content");

    // Read second line (rewind to start to get second line in this simple test)
    fseek(f, 0, SEEK_SET);
    // To ensure we read the second line, reposition to after first line
    // Simpler: perform a fresh read of the entire input again and grab the second line
    // Re-reading from start to second line for determinism
    memset(buf, 0, sizeof(buf));
    r1 = fGetsUtf8(buf, (int)sizeof(buf), f);
    // After reset, we are at start; perform a manual step to read to second line would require more complex state
    // Instead, perform a more deterministic approach: create a new file with a single line then read again
    fclose(f);
}

// Test 2 (alternative deterministic): Read second line by consuming the first then reading next on a fresh stream
static void test_read_second_line_deterministic() {
    std::string input = "line1\nline2\n";
    // First read
    FILE* f = makeInputFile(input);
    char buf[64] = {0};
    char* r = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(r != nullptr, "First read should succeed");
    EXPECT_EQ_STR(buf, "line1\n", "First line content");

    // Move to after the first line
    // Re-position to the start of second line by scanning until after first newline
    // We emulate this by closing the current stream and reopening with the same content, then skipping first line
    fclose(f);

    FILE* f2 = makeInputFile(input);
    // Consume the first line manually by reading and discarding
    char discard[64];
    char* rd = fGetsUtf8(discard, (int)sizeof(discard), f2);
    EXPECT_TRUE(rd != nullptr, "Discard first line should succeed");

    // Now read the second line
    memset(buf, 0, sizeof(buf));
    rd = fGetsUtf8(buf, (int)sizeof(buf), f2);
    EXPECT_TRUE(rd != nullptr, "Second read should succeed after discarding first line");
    EXPECT_EQ_STR(buf, "line2\n", "Second line content should be 'line2\\n'");

    fclose(f2);
}

// Test 3: Empty input should yield NULL (no data read)
static void test_empty_input() {
    std::string input = "";
    FILE* f = makeInputFile(input);
    char buf[32] = {0};
    char* res = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(res == nullptr, "Reading from empty input should return NULL");
    fclose(f);
}

// Test 4: Buffer limit handling: input longer than buffer; ensure only ncMax-1 chars are read
static void test_buffer_limit() {
    // 15-char line with a newline; set ncMax to 10 to force max-characters read = 9
    std::string input = "0123456789ABCDEF\n";
    FILE* f = makeInputFile(input);
    char buf[16] = {0};
    char* res = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(res != nullptr, "Buffer-limit read should return non-null");
    EXPECT_EQ_STR(buf, "012345678", "Buffer should contain first 9 characters due to ncMax=10");
    fclose(f);
}

// Test 5: Multiple successive reads on the same stream
static void test_multiple_reads() {
    // Three lines; ensure successive calls return lines in order
    std::string input = "A\nB\nC\n";
    FILE* f = makeInputFile(input);
    char buf[4] = {0};

    // Read first line
    char* res = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(res != nullptr, "First read should succeed");
    EXPECT_EQ_STR(buf, "A\n", "First line content");

    // Prepare buffer for second read
    memset(buf, 0, sizeof(buf));
    res = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(res != nullptr, "Second read should succeed");
    EXPECT_EQ_STR(buf, "B\n", "Second line content");

    // Prepare buffer for third read
    memset(buf, 0, sizeof(buf));
    res = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(res != nullptr, "Third read should succeed");
    EXPECT_EQ_STR(buf, "C\n", "Third line content");

    // Fourth read should be NULL (EOF)
    memset(buf, 0, sizeof(buf));
    res = fGetsUtf8(buf, (int)sizeof(buf), f);
    EXPECT_TRUE(res == nullptr, "Reading past EOF should return NULL");

    fclose(f);
}

// Main: drive tests
int main() {
    std::cout << "[TEST SUITE] Starting unit tests for fGetsUtf8 (console_io.c)\n";

    // Run tests
    test_read_first_line();
    test_read_second_line_deterministic();
    test_empty_input();
    test_buffer_limit();
    test_multiple_reads();

    if(g_failures == 0) {
        std::cout << "[TEST SUITE] All tests passed.\n";
    } else {
        std::cerr << "[TEST SUITE] Total failures: " << g_failures << "\n";
    }

    return g_failures ? 1 : 0;
}
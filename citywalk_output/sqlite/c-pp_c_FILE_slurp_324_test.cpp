/*
  Unit test suite for the focal method: FILE_slurp (located in c-pp.c)

  Summary:
  - Implements a lightweight, self-contained C++11 testing harness (no GTest).
  - Provides minimal C-linkage stubs for external dependencies needed by FILE_slurp
    (notably cmpp_realloc) to enable linking with c-pp.c.
  - Creates four targeted tests that exercise:
      1) Empty input file
      2) Small non-empty input
      3) Large input (to trigger multiple reallocation chunks)
      4) Binary data containing embedded zero bytes
  - Uses non-terminating EXPECT-like assertions to maximize code-execution coverage.
  - Tests are executed from main() as requested by the domain knowledge guidelines.

  Domain knowledge alignment (Step 1-3):
  - Candidate Keywords: FILE_slurp, pFile, pOut, nOut, cmpp_realloc, fread, feof, zBuf, n, nOff, nAlloc
  - Class Dependencies: Minimal stubs for external dependencies used by FILE_slurp (cmpp_realloc, FILE_open/FILE_close stubs, etc.)
  - Test Goals: True/false branch coverage for input states (empty, small, large, binary data); proper memory handling; static-like behavior via in-file stubs; non-terminating assertions; pure C data handling in C++ tests.

  Important: The test harness assumes c-pp.c is compiled and linked with this test file.
  - cmpp_realloc is provided as a small wrapper around realloc to satisfy the external symbol expected by FILE_slurp.
  - Other external symbols from the FOCAL_CLASS_DEP can be added as needed by your environment; the current stubs cover the common needs for FILE_slurp.

  How to build (example):
  - g++ -std=c++11 test_file_slurp.cpp c-pp.c -o test_file_slurp
  - Ensure a C standard library is available and sqlite3 headers (if required by c-pp.c) are installable in your environment.

  Notes on memory management:
  - FILE_slurp allocates a buffer into *pOut which must be freed by the caller (using free).
  - The tests free the allocated buffers after verification.

  Now the test code:
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <cstdarg>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


// Forward declaration of the focal function (C linkage)
extern "C" void FILE_slurp(FILE *pFile, unsigned char **pOut, unsigned * nOut);

// Minimal external dependency stubs to satisfy linking with c-pp.c
extern "C" {

    // Realloc-like helper used by FILE_slurp
    void * cmpp_realloc(void * p, unsigned n) {
        // Simple wrapper around realloc for compatibility with FILE_slurp expectations
        return realloc(p, static_cast<size_t>(n));
    }

    // Fatal stubs (no-op)
    void fatalv(char const *zFmt, va_list va) { (void)zFmt; (void)va; }
    void fatal(char const *zFmt, ...) { (void)zFmt; }

    // FILE helpers (basic wrappers around stdio)
    FILE * FILE_open(char const *zName, const char * zMode) {
        return fopen(zName, zMode);
    }

    void FILE_close(FILE *p) {
        if(p) fclose(p);
    }

    // Optional: keep compatibility in case c-pp.c references FileWrapper-like APIs
    struct FileWrapper { int dummy; };
    void FileWrapper_close(FileWrapper * p) { (void)p; }
    void FileWrapper_slurp(FileWrapper * p) { (void)p; }

    // Placeholder functions for signature completeness (no-ops)
    unsigned count_lines(unsigned char const * zFrom, unsigned char const *zTo) { (void)zFrom; (void)zTo; return 0; }
    void g_stderrv(char const *zFmt, va_list va) { (void)zFmt; (void)va; }
    void g_stderr(char const *zFmt, ...) { (void)zFmt; }
    void cmpp_t_out(void* t, void const *z, unsigned int n) { (void)t; (void)z; (void)n; }
    void CmppLevel_push(void* t) { (void)t; }
    void CmppLevel_pop(void* t) { (void)t; }
    void * CmppLevel_get(void* t) { (void)t; return nullptr; }

    // Database stubs to satisfy potential linkage (if referenced)
    void db_affirm_rc(int rc, const char * zMsg) { (void)rc; (void)zMsg; }
    void db_finalize(void*) { }
    int db_step(void*) { return 0; }
    // sqlite3_str and related stubs (placeholders). If real sqlite3 usage exists in c-pp.c,
    // you should link against sqlite3 library and provide proper headers.
    struct sqlite3_str;
    sqlite3_str * db_str_new(void) { return nullptr; }
    char * db_str_finish(sqlite3_str *s, int * n) { (void)s; if(n) *n = 0; return nullptr; }
    void db_prepare(void**, const char*, ...) { (void)nullptr; (void)0; }
    void db_bind_int(void*, int, int) { }
    void db_bind_textn(void*, int, const char*, int) { (void)0; }
    void db_bind_text(void*, int, const char*) { (void)0; }
    void db_free(void *m) { (void)m; }
    void db_define_add(const char * zKey) { (void)zKey; }
    int db_define_has(const char * zName) { (void)zName; return 0; }
    void db_define_rm(const char * zKey) { (void)zKey; }
    void db_including_add(const char * zKey, const char * zSrc, int srcLine) { (void)zKey; (void)zSrc; (void)srcLine; }
    void db_include_rm(const char * zKey) { (void)zKey; }
    char * db_include_search(const char * zKey) { (void)zKey; return nullptr; }
    int db_including_has(const char * zName) { (void)zName; return 0; }
    void db_include_dir_add(const char * zDir) { (void)zDir; }

    // Static-ish initialization guard (no-op)
    static void cmpp_initdb(void) { }
    static unsigned short cmpp_is_delim(unsigned char const *zBegin,
                                        unsigned char const *zEnd,
                                        unsigned char const *zPos) { (void)zBegin; (void)zEnd; (void)zPos; return 0; }
    static int cmpp_next_keyword_line(void* t) { (void)t; return 0; }
    static void cmpp_kwd__err_prefix(void const * pKw, void* t, char const * zPrefix) { (void)pKw; (void)t; (void)zPrefix; }
    static void cmpp_kwd__misuse(void const * pKw, void* t, char const *zFmt, ...) { (void)pKw; (void)t; (void)zFmt; }
    static void cmpp_kwd_noop(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_error(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_define(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_if(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_else(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_endif(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_include(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_pragma(void const * pKw, void* t) { (void)pKw; (void)t; }
    static void cmpp_kwd_stderr(void const * pKw, void* t) { (void)pKw; (void)t; }
    static int cmp_CmppKeyword(const void *p1, const void *p2) { (void)p1; (void)p2; return 0; }
    const void* CmppKeyword_search(const char *zName) { (void)zName; return nullptr; }
    void cmpp_process_keyword(void* t) { (void)t; }
    void cmpp_process_file(const char * zName) { (void)zName; }
    static void usage(int isErr) { (void)isErr; }
    int main(int argc, char const * const * argv) { (void)argc; (void)argv; return 0; }

    // End of stubs
}

// Expectation helpers (non-terminating assertions)
static int g_test_failures = 0;

static void log_failure(const char* file, int line, const char* msg) {
    std::fprintf(stderr, "Test failure: %s:%d: %s\n", file, line, msg);
    ++g_test_failures;
}

#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { log_failure(__FILE__, __LINE__, "EXPECT_TRUE(" #cond ") failed"); } } while(0)

#define EXPECT_EQ(a,b) \
    do { if(((a) != (b))) { char _buf[128]; std::snprintf(_buf, sizeof(_buf), "EXPECT_EQ(" #a ", " #b ") failed: %lld != %lld", static_cast<long long>(a), static_cast<long long>(b)); log_failure(__FILE__, __LINE__, _buf); } } while(0)

#define EXPECT_MEMEQ(a,b,len) \
    do { if(std::memcmp((a),(b),(len)) != 0) { log_failure(__FILE__, __LINE__, "EXPECT_MEMEQ failed"); } } while(0)

// Test 1: Empty input file should yield null output buffer and zero length
static void test_FILE_slurp_empty() {
    // Create a temporary file (empty)
    char tmpname[] = "/tmp/file_slurp_test_emptyXXXXXX";
    int fd = mkstemp(tmpname);
    if(fd == -1) {
        log_failure(__FILE__, __LINE__, "Failed to create temp file for empty test");
        return;
    }

    // Open as binary read/write, ensure it's empty
    FILE *f = fdopen(fd, "w+b"); // create/overwrite, then we'll seek to 0
    if(!f) {
        close(fd);
        log_failure(__FILE__, __LINE__, "Failed to fdopen temp file");
        return;
    }

    // Ensure file position is at start for reading by FILE_slurp
    fflush(f);
    fseek(f, 0, SEEK_SET);

    unsigned nOut = 0;
    unsigned char* pOut = nullptr;

    FILE_slurp(f, &pOut, &nOut);

    // Clean up file handle; since we opened with "w+b", fclose will also delete content
    fclose(f);
    // Remove the file from FS
    unlink(tmpname);

    // Assertions
    EXPECT_TRUE(pOut == nullptr);
    EXPECT_EQ(nOut, 0u);

    if(pOut) free(pOut); // safety (should be null, but keep clean)
}

// Test 2: Small non-empty input should be slurped exactly
static void test_FILE_slurp_small() {
    const char* text = "Hello";
    size_t len = std::strlen(text);

    char tmpname[] = "/tmp/file_slurp_test_smallXXXXXX";
    int fd = mkstemp(tmpname);
    if(fd == -1) {
        log_failure(__FILE__, __LINE__, "Failed to create temp file for small test");
        return;
    }

    FILE *f = fdopen(fd, "w+b");
    if(!f) {
        close(fd);
        log_failure(__FILE__, __LINE__, "Failed to fdopen temp file for small test");
        return;
    }

    // Write content
    std::fwrite(text, 1, len, f);
    fflush(f);
    fseek(f, 0, SEEK_SET);

    unsigned nOut = 0;
    unsigned char* pOut = nullptr;

    FILE_slurp(f, &pOut, &nOut);

    fclose(f);
    unlink(tmpname);

    // Assertions
    EXPECT_TRUE(pOut != nullptr);
    EXPECT_EQ(nOut, static_cast<unsigned>(len));
    if(pOut) {
        // Compare content (first nOut bytes)
        EXPECT_MEMEQ(pOut, reinterpret_cast<const unsigned char*>(text), len);
        // The stored string should end with a null terminator
        EXPECT_TRUE(pOut[len] == '\0');
        free(pOut);
    }
}

// Test 3: Large input to exercise multiple reallocs
static void test_FILE_slurp_large() {
    size_t len = 10240; // 10 KB - large enough to cross the 8 KB chunk boundary
    std::vector<char> data(len, 'A');

    char tmpname[] = "/tmp/file_slurp_test_largeXXXXXX";
    int fd = mkstemp(tmpname);
    if(fd == -1) {
        log_failure(__FILE__, __LINE__, "Failed to create temp file for large test");
        return;
    }
    FILE *f = fdopen(fd, "w+b");
    if(!f) {
        close(fd);
        log_failure(__FILE__, __LINE__, "Failed to fdopen temp file for large test");
        return;
    }

    // Write large content
    std::fwrite(data.data(), 1, len, f);
    fflush(f);
    fseek(f, 0, SEEK_SET);

    unsigned nOut = 0;
    unsigned char* pOut = nullptr;

    FILE_slurp(f, &pOut, &nOut);

    fclose(f);
    unlink(tmpname);

    // Assertions
    EXPECT_TRUE(pOut != nullptr);
    EXPECT_EQ(nOut, static_cast<unsigned>(len));
    if(pOut) {
        EXPECT_MEMEQ(pOut, reinterpret_cast<const unsigned char*>(data.data()), len);
        // terminator
        EXPECT_TRUE(pOut[len] == '\0');
        free(pOut);
    }
}

// Test 4: Binary data including embedded zero to ensure correct length handling
static void test_FILE_slurp_binary_zero() {
    // Data with embedded zero
    unsigned char payload[] = { 'A', 0u, 'B', 'C' };
    size_t len = sizeof(payload);

    char tmpname[] = "/tmp/file_slurp_test_binXXXXXX";
    int fd = mkstemp(tmpname);
    if(fd == -1) {
        log_failure(__FILE__, __LINE__, "Failed to create temp file for binary test");
        return;
    }

    FILE *f = fdopen(fd, "w+b");
    if(!f) {
        close(fd);
        log_failure(__FILE__, __LINE__, "Failed to fdopen temp file for binary test");
        return;
    }

    // Write binary data
    std::fwrite(payload, 1, len, f);
    fflush(f);
    fseek(f, 0, SEEK_SET);

    unsigned nOut = 0;
    unsigned char* pOut = nullptr;

    FILE_slurp(f, &pOut, &nOut);

    fclose(f);
    unlink(tmpname);

    // Assertions
    EXPECT_TRUE(pOut != nullptr);
    EXPECT_EQ(nOut, static_cast<unsigned>(len));
    if(pOut) {
        // Compare first 4 bytes (including embedded zero)
        EXPECT_MEMEQ(pOut, payload, len);
        // The buffer should be terminated after the data
        EXPECT_TRUE(pOut[len] == '\0');
        free(pOut);
    }
}

// Simple test runner
static void run_all_tests() {
    std::cout << "Starting FILE_slurp test suite (no GTest, lightweight harness)\n";

    test_FILE_slurp_empty();
    test_FILE_slurp_small();
    test_FILE_slurp_large();
    test_FILE_slurp_binary_zero();

    if(g_test_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Tests completed with " << g_test_failures << " failure(s).\n";
    }
}

int main(int argc, char const * const * argv) {
    (void)argc; (void)argv;
    run_all_tests();
    return (g_test_failures == 0) ? 0 : 1;
}
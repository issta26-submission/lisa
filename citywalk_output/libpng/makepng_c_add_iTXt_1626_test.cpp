// Test suite for add_iTXt in makepng.c
// This test uses a custom make_insert hook to capture parameters passed by add_iTXt
// and verifies that the correct internal behavior is invoked.
// Compilation requires linking against the C file providing add_iTXt (as in the project).

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Domain-specific typedefs to match the C interface used by add_iTXt
typedef const char* png_const_charp;
typedef char*       png_charp;
typedef void*       png_structp;
typedef void*       png_infop;
typedef char**      png_charpp;

// Forward declaration of the focal function (assumed to be provided by the C source file).
extern "C" void* add_iTXt(const char *key, const char *language, const char *language_key,
                          const png_const_charp *text);

// We provide a C-linkage override for make_insert so that add_iTXt calls our interceptor.
// We mirror the signature expected by the original code.
extern "C" void* make_insert(png_const_charp what,
                            void (*insert)(png_structp, png_infop, int, png_charpp),
                            int nparams, png_charpp list);

// Global structure to capture details of the last insert invocation
struct LastInsert {
    const char* what;            // points to "iTXt" constant inside the focal code
    void (*insert)(png_structp, png_infop, int, png_charpp);
    int nparams;
    char** list;                   // copied parameter list (4 strings), owned by test
};

// Hold the last insertion data
static LastInsert g_last_insert = { NULL, NULL, 0, NULL };

// Helper: free memory held by g_last_insert
static void clear_last_insert() {
    if (g_last_insert.list) {
        for (int i = 0; i < g_last_insert.nparams; ++i) {
            if (g_last_insert.list[i]) {
                free(g_last_insert.list[i]);
                g_last_insert.list[i] = NULL;
            }
        }
        free(g_last_insert.list);
        g_last_insert.list = NULL;
    }
    g_last_insert.what = NULL;
    g_last_insert.insert = NULL;
    g_last_insert.nparams = 0;
}

// Hook implementation: captures the parameters and returns a pointer to a local LastInsert
extern "C" void* make_insert(png_const_charp what,
                            void (*insert)(png_structp, png_infop, int, png_charpp),
                            int nparams, png_charpp list)
{
    // Store a copy of the input strings to keep them alive after the function returns.
    clear_last_insert();
    g_last_insert.what = what;
    g_last_insert.insert = insert;
    g_last_insert.nparams = nparams;

    // Copy the 4 parameter strings (list is guaranteed to be of length nparams with NULL terminator).
    g_last_insert.list = (char**)malloc(nparams * sizeof(char*));
    for (int i = 0; i < nparams; ++i) {
        if (list && list[i]) {
            g_last_insert.list[i] = strdup(list[i]);
        } else {
            g_last_insert.list[i] = NULL;
        }
    }

    return (void*)&g_last_insert;
}

// Minimal test framework (non-terminating assertions)
static int g_failure_count = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "TEST FAIL: %s\n", msg); \
        ++g_failure_count; \
    } \
} while(0)

static void test_basic_case() {
    // Prepare inputs
    const char* key = "Key1";
    const char* language = "en";
    const char* language_key = "LangKey";
    const png_const_charp text[] = { "Hello", "World", NULL };

    // Call the focal method
    add_iTXt(key, language, language_key, text);

    // Verification
    CHECK(g_last_insert.what != NULL, "what must be non-null");
    if (g_last_insert.what) {
        CHECK(std::strcmp(g_last_insert.what, "iTXt") == 0,
              "what should be 'iTXt'");
    }

    CHECK(g_last_insert.nparams == 4, "nparams should be 4");
    if (g_last_insert.list) {
        CHECK(g_last_insert.list[0] != NULL, "param0 should not be NULL");
        if (g_last_insert.list[0])
            CHECK(std::strcmp(g_last_insert.list[0], key) == 0,
                  "param0 must equal key");

        CHECK(g_last_insert.list[1] != NULL, "param1 should not be NULL");
        if (g_last_insert.list[1])
            CHECK(std::strcmp(g_last_insert.list[1], language) == 0,
                  "param1 must equal language");

        CHECK(g_last_insert.list[2] != NULL, "param2 should not be NULL");
        if (g_last_insert.list[2])
            CHECK(std::strcmp(g_last_insert.list[2], language_key) == 0,
                  "param2 must equal language_key");

        // param3 is the text joined by spaces by strstash_list
        CHECK(g_last_insert.list[3] != NULL, "param3 should not be NULL");
        if (g_last_insert.list[3])
            CHECK(std::strcmp(g_last_insert.list[3], "Hello World") == 0,
                  "param3 must equal 'Hello World'");
    }

    clear_last_insert();
}

static void test_empty_case() {
    const char* key = "";
    const char* language = "";
    const char* language_key = "";
    const png_const_charp text[] = { "", NULL };

    add_iTXt(key, language, language_key, text);

    CHECK(g_last_insert.what != NULL, "what must be non-null (empty case)");
    if (g_last_insert.what) {
        CHECK(std::strcmp(g_last_insert.what, "iTXt") == 0,
              "what should be 'iTXt' in empty case");
    }

    CHECK(g_last_insert.nparams == 4, "nparams should be 4 (empty case)");
    if (g_last_insert.list) {
        CHECK(g_last_insert.list[0] != NULL, "param0 should not be NULL (empty case)");
        if (g_last_insert.list[0])
            CHECK(std::strcmp(g_last_insert.list[0], "") == 0,
                  "param0 must be empty string");

        CHECK(g_last_insert.list[1] != NULL, "param1 should not be NULL (empty case)");
        if (g_last_insert.list[1])
            CHECK(std::strcmp(g_last_insert.list[1], "") == 0,
                  "param1 must be empty string");

        CHECK(g_last_insert.list[2] != NULL, "param2 should not be NULL (empty case)");
        if (g_last_insert.list[2])
            CHECK(std::strcmp(g_last_insert.list[2], "") == 0,
                  "param2 must be empty string");

        CHECK(g_last_insert.list[3] != NULL, "param3 should not be NULL (empty case)");
        if (g_last_insert.list[3])
            CHECK(std::strcmp(g_last_insert.list[3], "") == 0,
                  "param3 must be empty string");
    }

    clear_last_insert();
}

static void test_text_with_special_chars_case() {
    const char* key = "Special Key 123";
    const char* language = "fr";
    const char* language_key = "CléLang";
    const png_const_charp text[] = { "Foo", "Bar", NULL };

    add_iTXt(key, language, language_key, text);

    CHECK(g_last_insert.what != NULL, "what must be non-null (special chars case)");
    if (g_last_insert.what) {
        CHECK(std::strcmp(g_last_insert.what, "iTXt") == 0,
              "what should be 'iTXt' in special chars case");
    }

    CHECK(g_last_insert.nparams == 4, "nparams should be 4 (special chars case)");
    if (g_last_insert.list) {
        CHECK(g_last_insert.list[0] != NULL, "param0 should not be NULL (special chars)");
        if (g_last_insert.list[0])
            CHECK(std::strcmp(g_last_insert.list[0], key) == 0,
                  "param0 must equal key");

        CHECK(g_last_insert.list[1] != NULL, "param1 should not be NULL (special chars)");
        if (g_last_insert.list[1])
            CHECK(std::strcmp(g_last_insert.list[1], language) == 0,
                  "param1 must equal language");

        CHECK(g_last_insert.list[2] != NULL, "param2 should not be NULL (special chars)");
        if (g_last_insert.list[2])
            CHECK(std::strcmp(g_last_insert.list[2], language_key) == 0,
                  "param2 must equal language_key");

        CHECK(g_last_insert.list[3] != NULL, "param3 should not be NULL (special chars)");
        if (g_last_insert.list[3])
            CHECK(std::strcmp(g_last_insert.list[3], "Foo Bar") == 0,
                  "param3 must equal 'Foo Bar'");
    }

    clear_last_insert();
}

int main() {
    test_basic_case();
    test_empty_case();
    test_text_with_special_chars_case();

    if (g_failure_count == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILURES: %d\n", g_failure_count);
        return 1;
    }
}
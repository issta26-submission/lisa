/*
  Unit tests for sqlite3Fts3SimpleTokenizerModule
  - No GTest; a lightweight custom test harness is used.
  - Focus: verify that sqlite3Fts3SimpleTokenizerModule correctly returns a module pointer
    by setting the output parameter ppModule to a non-null value.
  - Tests are written in C++11 and rely only on the standard library plus the focal C API.
  - Explanatory comments accompany each test case.
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cstdio>
#include <fts3_tokenizer.h>
#include <stdio.h>
#include <cstdarg>
#include <cstring>
#include <cstdint>


// Forward declaration to mirror the C types without pulling in SQLite headers.
// This is sufficient for declaring the function signature used in tests.
struct sqlite3_tokenizer_module;

// Declare the focal function with C linkage to match the implementation in fts3_tokenizer1.c
extern "C" void sqlite3Fts3SimpleTokenizerModule(struct sqlite3_tokenizer_module const**ppModule);

// Simple, non-terminating test failure reporter.
// Increments a global counter but keeps running to maximize coverage.
static int g_test_failures = 0;

static void log_failure(const char* file, int line, const char* fmt, ...)
{
    va_list ap;
    fprintf(stderr, "%s:%d: ", file, line);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    ++g_test_failures;
}

// Convenience assertions that do not terminate on failure.
// They print diagnostics and continue executing to improve coverage.
#define ASSERT_TRUE(cond) do { \
    if(!(cond)) log_failure(__FILE__, __LINE__, "ASSERT_TRUE failed: %s", #cond); \
} while(0)

#define ASSERT_NOT_NULL(p) do { \
    if((p) == nullptr) log_failure(__FILE__, __LINE__, "ASSERT_NOT_NULL failed: %s is null", #p); \
} while(0)

#define ASSERT_EQ(a,b) do { \
    if(static_cast<void*>(const_cast<void*>(a)) != static_cast<void*>(const_cast<void*>(b)) && (a) != (b)) \
        log_failure(__FILE__, __LINE__, "ASSERT_EQ failed: %p != %p", (void*)a, (void*)b); \
} while(0)

// Test 1: Basic functionality
// - Domain: Ensure that the focal function assigns a non-null module pointer.
// - Coverage: True-branch of the assignment (ppModule is set to a valid module).
static void test_sqlite3Fts3SimpleTokenizerModule_SetsPointer()
{
    // Output parameter for the module pointer.
    const struct sqlite3_tokenizer_module* mod = nullptr;

    // Call the focal function (C linkage); it should set mod to a valid module pointer.
    sqlite3Fts3SimpleTokenizerModule(&mod);

    // Verify that a non-null pointer was produced.
    ASSERT_NOT_NULL(mod);

    // Additional check: ensure repeated invocations yield a consistent non-null pointer.
    const struct sqlite3_tokenizer_module* mod2 = nullptr;
    sqlite3Fts3SimpleTokenizerModule(&mod2);
    ASSERT_NOT_NULL(mod2);

    // Pointers should be the same across successive calls (static module within the C file).
    ASSERT_EQ(mod, mod2);
}

// Test 2: Pointer consistency across multiple calls
// - Domain: Ensure that repeated invocations do not return different non-null pointers.
// - Coverage: Non-nullness and pointer equality across calls.
static void test_sqlite3Fts3SimpleTokenizerModule_PointerConsistency()
{
    const struct sqlite3_tokenizer_module* a = nullptr;
    const struct sqlite3_tokenizer_module* b = nullptr;

    sqlite3Fts3SimpleTokenizerModule(&a);
    sqlite3Fts3SimpleTokenizerModule(&b);

    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(b);

    // Expect the same module pointer (static internal module).
    ASSERT_EQ(a, b);
}

// Entry point for the test suite.
int main()
{
    // Run tests
    test_sqlite3Fts3SimpleTokenizerModule_SetsPointer();
    test_sqlite3Fts3SimpleTokenizerModule_PointerConsistency();

    // Summary
    if(g_test_failures == 0)
    {
        std::printf("All tests passed\n");
        return 0;
    }
    else
    {
        std::printf("%d test(s) failed\n", g_test_failures);
        return 1;
    }
}
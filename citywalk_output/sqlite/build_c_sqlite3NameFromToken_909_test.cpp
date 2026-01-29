// Unit tests for the sqlite3NameFromToken function
// This test suite is implemented in pure C++11 (no GoogleTest) and uses
// a lightweight test harness with non-terminating EXPECT-like macros.
// The goal is to exercise all branches of sqlite3NameFromToken as described.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Lightweight stubs and minimal implementations to allow compilation
// and to exercise sqlite3NameFromToken logic in a self-contained way.

// Minimal sqlite3 type (opaque in the real project)
struct sqlite3 {};

// Token structure as used by sqlite3NameFromToken
struct Token {
    unsigned char *z; // pointer to the token text
    int n;            // length of the token text
};

// Function under test (copied/adapted from the provided focal method)
char *sqlite3NameFromToken(sqlite3 *db, const Token *pName) {
  char *zName;
  if( pName ){
    // Copy exactly n bytes and ensure NUL termination
    // This mirrors sqlite3DbStrNDup behavior in the real sqlite3 codebase.
    zName = nullptr;
    // Forward declaration to be defined below (in this test file)
    extern char *sqlite3DbStrNDup(sqlite3 *db, const char *z, int n);
    zName = sqlite3DbStrNDup(db, (const char*)pName->z, pName->n);
    // Dequote in-place (as in real implementation)
    extern void sqlite3Dequote(char *z);
    sqlite3Dequote(zName);
  }else{
    zName = 0;
  }
  return zName;
}

// Lightweight in-test implementations mirroring sqlite3 internals

// Allocate and duplicate up to n bytes from z, ensuring NUL termination
char *sqlite3DbStrNDup(sqlite3 *db, const char *z, int n) {
    (void)db; // unused in this lightweight test
    char *p = new char[n + 1];
    if (p && n > 0) {
        std::memcpy(p, z, n);
        p[n] = '\0';
    } else if (p) {
        p[0] = '\0';
    }
    return p;
}

// Remove surrounding quotes if they exist
void sqlite3Dequote(char *z) {
    if (!z) return;
    size_t len = std::strlen(z);
    if (len >= 2) {
        char first = z[0];
        char last  = z[len - 1];
        bool isSingleQuotes = (first == '\'' && last == '\'');
        bool isDoubleQuotes = (first == '\"' && last == '\"');
        if (isSingleQuotes || isDoubleQuotes) {
            // Remove the first and last quote characters
            // Move the middle part to the start
            if (len > 2) {
                std::memmove(z, z + 1, len - 2);
                z[len - 2] = '\0';
            } else {
                // If length is 2 (like ''), result is empty string
                z[0] = '\0';
            }
        }
    }
}

// Simple non-terminating assertion helpers (EXPECT-like)
static int gTestsRun = 0;
static int gTestsFailed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        ++gTestsRun; \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++gTestsFailed; \
        } \
    } while(0)

#define EXPECT_FALSE(cond) \
    do { \
        ++gTestsRun; \
        if (cond) { \
            std::cerr << "EXPECT_FALSE failed at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++gTestsFailed; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        ++gTestsRun; \
        if (!((a) == (b))) { \
            std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " \
                      << #b << " (" << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++gTestsFailed; \
        } \
    } while(0)

#define EXPECT_STR_EQ(a, b) \
    do { \
        ++gTestsRun; \
        if ((a) == nullptr && (b) == nullptr) { \
            ; \
        } else if (!((a) && (b))) { \
            std::cerr << "EXPECT_STR_EQ failed: one null, at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++gTestsFailed; \
        } else if (std::strcmp((a), (b)) != 0) { \
            std::cerr << "EXPECT_STR_EQ failed: \"" << (a) << "\" != \"" << (b) \
                      << "\" at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++gTestsFailed; \
        } \
    } while(0)

#define RUN_TEST(testFunc) \
    do { \
        testFunc(); \
    } while(0)

// Test 1: pName is nullptr -> function should return nullptr
void test_nameFromToken_nullPointerReturnsNull() {
    // Domain knowledge note: The function should handle null input gracefully.
    sqlite3 *db = nullptr;
    const Token *pName = nullptr;
    char *result = sqlite3NameFromToken(db, pName);
    EXPECT_TRUE(result == nullptr);
    // No allocation expected; nothing to free.
}

// Test 2: Basic token without quotes -> should return a copy of the token text
void test_nameFromToken_basicNoQuotes() {
    sqlite3 db;
    const char s[] = "abc";
    Token t;
    t.z = (unsigned char*)s;
    t.n = 3;
    char *result = sqlite3NameFromToken(&db, &t);
    EXPECT_TRUE(result != nullptr);
    EXPECT_STR_EQ(result, "abc");
    delete[] result;
}

// Test 3: Token with single quotes should be dequoted to inner text
void test_nameFromToken_singleQuotesDequoted() {
    sqlite3 db;
    const char s[] = "'abc'";
    Token t;
    t.z = (unsigned char*)s;
    t.n = 5;
    char *result = sqlite3NameFromToken(&db, &t);
    EXPECT_TRUE(result != nullptr);
    EXPECT_STR_EQ(result, "abc");
    delete[] result;
}

// Test 4: Token with double quotes should be dequoted to inner text
void test_nameFromToken_doubleQuotesDequoted() {
    sqlite3 db;
    const char s[] = "\"abc\"";
    Token t;
    t.z = (unsigned char*)s;
    t.n = 5;
    char *result = sqlite3NameFromToken(&db, &t);
    EXPECT_TRUE(result != nullptr);
    EXPECT_STR_EQ(result, "abc");
    delete[] result;
}

// Test 5: Token without surrounding quotes should remain unchanged
void test_nameFromToken_noQuotesRemain() {
    sqlite3 db;
    const char s[] = "xyz";
    Token t;
    t.z = (unsigned char*)s;
    t.n = 3;
    char *result = sqlite3NameFromToken(&db, &t);
    EXPECT_TRUE(result != nullptr);
    EXPECT_STR_EQ(result, "xyz");
    delete[] result;
}

// Entry point to run all tests
int main() {
    std::cout << "Running sqlite3NameFromToken unit tests (C++11, no GTest)...\n";

    RUN_TEST(test_nameFromToken_nullPointerReturnsNull);
    RUN_TEST(test_nameFromToken_basicNoQuotes);
    RUN_TEST(test_nameFromToken_singleQuotesDequoted);
    RUN_TEST(test_nameFromToken_doubleQuotesDequoted);
    RUN_TEST(test_nameFromToken_noQuotesRemain);

    std::cout << "Tests run: " << gTestsRun << ", Failures: " << gTestsFailed << "\n";

    // Return non-zero if any test failed
    return (gTestsFailed > 0) ? 1 : 0;
}
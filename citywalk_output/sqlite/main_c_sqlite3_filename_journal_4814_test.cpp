// Lightweight C++11 test suite for sqlite3_filename_journal
// This test suite avoids GoogleTest and uses a small custom assertion framework.
// It mocks necessary dependencies to isolate sqlite3_filename_journal behavior.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


namespace sqlite3test {

// Forward declaration of the focal function under test.
// We provide minimal mock implementations of the dependencies used inside
// sqlite3_filename_journal to enable unit testing in a standalone fashion.
static const char *databaseName(const char *zName);
static int sqlite3Strlen30(const char *z);
#define ALWAYS(x) ((x) != NULL)

// Mocked dependency: simply returns the input pointer.
// In the real code, this would extract the database part of a multi-string.
// For unit testing, we retain a simple identity mapping.
static const char *databaseName(const char *zName) {
    return zName;
}

// Mocked dependency: safe strlen-like function for non-null strings.
static int sqlite3Strlen30(const char *z) {
    return z ? (int)strlen(z) : 0;
}

// Focal method under test (copied here to enable standalone compilation).
// Signature matches the original: const char *sqlite3_filename_journal(const char *zFilename)
const char *sqlite3_filename_journal(const char *zFilename){
  if( zFilename==0 ) return 0;
  zFilename = databaseName(zFilename);
  zFilename += sqlite3Strlen30(zFilename) + 1;
  while( ALWAYS(zFilename) && zFilename[0] ){
    zFilename += sqlite3Strlen30(zFilename) + 1;
    zFilename += sqlite3Strlen30(zFilename) + 1;
  }
  return zFilename + 1;
}

// Simple test harness (non-terminating assertions)
static int g_failures = 0;
static void logFailure(const char* msg, const char* file, int line) {
    std::cerr << "Test failure: " << msg << " (in " << file << ":" << line << ")" << std::endl;
    ++g_failures;
}
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) logFailure(msg, __FILE__, __LINE__); \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if((const void*)(a) != (const void*)(b)) logFailure(msg, __FILE__, __LINE__); \
} while(0)

// Test 1: Null input should return null
static void test_null_input() {
    const char *res = sqlite3_filename_journal(nullptr);
    EXPECT_TRUE(res == nullptr, "sqlite3_filename_journal(nullptr) should return null");
}

// Test 2: Two-string multi-string layout: "A\0B\0\0" -> expected pointer right after end (buf+6)
static void test_two_strings_case() {
    // Layout: "A" '\0' "B" '\0' '\0' '\0'
    // Indices: 0:'A',1:0,2:'B',3:0,4:0,5:0
    const char buf[] = {'A','\0','B','\0','\0','\0'};
    const char *res = sqlite3_filename_journal(buf);
    const char *expected = buf + 6; // end position after the final '\0'
    EXPECT_PTR_EQ(res, expected, "Two-strings case should return pointer to buf+6");
}

// Test 3: Three-string layout: "A\0B\0C\0\0" -> expected pointer right after end (buf+7)
static void test_three_strings_case() {
    // Layout: "A" '\0' "B" '\0' "C" '\0' '\0'
    // Indices: 0:'A',1:0,2:'B',3:0,4:'C',5:0,6:0
    const char buf[] = {'A','\0','B','\0','C','\0','\0'};
    const char *res = sqlite3_filename_journal(buf);
    const char *expected = buf + 7; // end position after the final '\0'
    EXPECT_PTR_EQ(res, expected, "Three-strings case should return pointer to buf+7");
}

// Runner to execute all tests and report summary
static void run_all_tests() {
    test_null_input();
    test_two_strings_case();
    test_three_strings_case();

    if(g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
    }
}

} // namespace sqlite3test

int main() {
    // Run all tests in the sqlite3test namespace
    sqlite3test::run_all_tests();
    return 0;
}
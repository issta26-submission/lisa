// Minimal C++11 unit test suite for the focal method:
// sqlite3_str_value(sqlite3_str *p)
//
// Purpose: verify behavior for NULL input, zero-length string, and non-zero-length strings.
// This test is self-contained and does not rely on external testing frameworks like GTest.

#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Candidate Keywords (from Step 1): p, nChar, zText, NULL, zero-termination, memory safety, return value, pointer equality

// Minimal reproduction of the sqlite3_str struct used by sqlite3_str_value
typedef struct sqlite3_str {
    char *zText;  // pointer to text buffer
    int nChar;    // index of the last character (before the implicit null terminator)
} sqlite3_str;

// Focal method under test (as provided in the prompt)
char *sqlite3_str_value(sqlite3_str *p){
  if( p==0 || p->nChar==0 ) return 0;
  p->zText[p->nChar] = 0;
  return p->zText;
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)){ \
        std::cerr << "[FAIL] " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a,b, msg) do { \
    if(!((a) == (b))){ \
        std::cerr << "[FAIL] " << (msg) \
                  << " Expected: " << (b) << ", Got: " << (a) \
                  << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a,b, msg) do { \
    if(((void*)(a)) != ((void*)(b))){ \
        std::cerr << "[FAIL] " << (msg) \
                  << " Expected pointer: " << b << ", Got: " << a \
                  << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STR_EQ(a, b, msg) do { \
    const char* _sa = (a); \
    const char* _sb = (b); \
    if(_sa == nullptr || _sb == nullptr){ \
        if(_sa != _sb){ \
            std::cerr << "[FAIL] " << (msg) \
                      << " Null mismatch (line " << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } else { \
        if(std::string(_sa) != std::string(_sb)){ \
            std::cerr << "[FAIL] " << (msg) \
                      << " Expected: " << _sb << ", Got: " << _sa \
                      << " (line " << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } \
} while(0)

//
// Test 1: Null input should return null
// This verifies the guard clause p==0
//
void test_sqlite3_str_value_null_pointer() {
    // When p is null, the function should return 0
    char *res = sqlite3_str_value(nullptr);
    EXPECT_TRUE(res == nullptr, "sqlite3_str_value(nullptr) should return null");
}

// Test 2: Non-null pointer with nChar == 0 should return null
// This validates the nChar==0 branch and that no modification occurs.
void test_sqlite3_str_value_zero_length() {
    sqlite3_str s;
    // Allocate a buffer to simulate a real sqlite3_str object
    s.zText = (char*)std::malloc(6);
    if (s.zText == nullptr) {
        std::cerr << "[WARN] Memory allocation failed in test_sqlite3_str_value_zero_length" << std::endl;
        return;
    }
    std::strcpy(s.zText, "ABCDE"); // fill with some data
    s.nChar = 0;

    char *res = sqlite3_str_value(&s);
    // Expect NULL return and no modification (the function returns early)
    EXPECT_TRUE(res == nullptr, "sqlite3_str_value with nChar==0 should return null");
    // Clean up
    std::free(s.zText);
}

// Test 3: Non-null pointer with nChar > 0 should terminate at zText[nChar] and return zText
// Verifies pointer identity and correct null-termination behavior
void test_sqlite3_str_value_basic_termination() {
    sqlite3_str s;
    // Ensure there is enough space for the operation: at least nChar + 1
    s.zText = (char*)std::malloc(12);
    if (s.zText == nullptr) {
        std::cerr << "[WARN] Memory allocation failed in test_sqlite3_str_value_basic_termination" << std::endl;
        return;
    }
    // Initialize buffer with known content
    std::strcpy(s.zText, "abcdef1234"); // length > 5
    s.nChar = 5; // target index to terminate at (after 'e')

    char *res = sqlite3_str_value(&s);
    // Check that the same buffer is returned
    EXPECT_PTR_EQ(res, s.zText, "sqlite3_str_value should return the original zText pointer");
    // Check that zText[5] has been set to 0 and string is terminated as "abcde"
    EXPECT_STR_EQ(s.zText, "abcde", "zText should be null-terminated at index nChar (5) resulting in 'abcde'");

    // Clean up
    std::free(s.zText);
}

// Driver: run all tests and report summary
int main() {
    // Candidate Keywords context (informational)
    // - Tests cover: null input, zero-length, non-zero-length paths
    // - Static-like behavior: none used; focus on function under test

    test_sqlite3_str_value_null_pointer();
    test_sqlite3_str_value_zero_length();
    test_sqlite3_str_value_basic_termination();

    if(g_failures == 0) {
        std::cout << "[OK] All sqlite3_str_value tests passed.\n";
        return 0;
    } else {
        std::cout << "[ERROR] sqlite3_str_value tests failed: " << g_failures << " failure(s).\n";
        return 1;
    }
}

// Notes for test refinement (Step 3):
// - If the real project provides additional behaviors (e.g., threading concerns, memory ownership),
//   consider expanding tests to exercise these in a future iteration.
// - This suite uses a self-contained sqlite3_str structure to avoid depending on external SQLite internals.
// - Static scope and non-private access rules are respected; tests avoid private members and rely only on
//   public-like behavior of the focal function.
// Test harness for the focal method: lsmStringAppend in lsm_str.c
// This test suite focuses on exercising the N-branch (N<0 vs N>=0),
// and the memory extension/copy path controlled by pStr->nAlloc.
// It uses non-terminating custom EXPECT-like assertions to maximize code coverage
// without introducing a full testing framework (GTest, etc.).

#include <cstdlib>
#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Domain knowledge applied: testing with standard C/C++ facilities only.
// We assume the LsmString structure matches the usage in lsmStringAppend:
//   - int nAlloc: allocated size of z (or 0 if none allocated yet)
//   - char *z: buffer containing the string data
//   - int n: current length (number of characters stored, excluding the null terminator)
typedef struct LsmString {
    int nAlloc;
    char *z;
    int n;
} LsmString;

// Declarations of the focal method and its dependencies (as they would be
// linked from the real library/class dependencies).
extern "C" {
    int lsmStringAppend(LsmString *pStr, const char *z, int N);
    int lsmStringExtend(LsmString *pStr, int nNew);
    // Other dependencies are not required for this focused test, but could be declared if needed.
}

// Simple non-terminating assertion framework (EXPECT-like)
static int g_total = 0;
static int g_failed = 0;
#define EXPECT_TRUE(cond) \
    do { \
        ++g_total; \
        if(!(cond)) { \
            ++g_failed; \
            std::cerr << "[EXPECT_TRUE FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " in " << __FUNCTION__ << " — condition: " #cond << std::endl; \
        } \
    } while(0)


// Test 1: Basic append with explicit N (N >= 0). Ensure extension and copy occur correctly.
bool test_basic_append_with_positive_N() {
    LsmString s;
    s.nAlloc = 0;
    s.z = nullptr;
    s.n = 0;

    const char *src = "abc";
    int rc = lsmStringAppend(&s, src, 3);

    EXPECT_TRUE(rc == 0);
    EXPECT_TRUE(s.n == 3);
    EXPECT_TRUE(s.z != nullptr);
    // Memory should contain "abc" including terminator -> 4 bytes
    EXPECT_TRUE(std::memcmp(s.z, "abc", 4) == 0);

    // Cleanup
    free(s.z);
    return g_failed == 0;
}

// Test 2: Negative N path (N < 0). Should treat N as strlen(z) and append accordingly.
bool test_negative_N_path() {
    LsmString s;
    s.nAlloc = 0;
    s.z = nullptr;
    s.n = 0;

    const char *src = "hello";
    int rc = lsmStringAppend(&s, src, -1); // N < 0 -> N becomes strlen("hello") = 5

    EXPECT_TRUE(rc == 0);
    EXPECT_TRUE(s.n == 5);
    EXPECT_TRUE(s.z != nullptr);
    EXPECT_TRUE(std::memcmp(s.z, "hello", 6) == 0);

    free(s.z);
    return g_failed == 0;
}

// Test 3: Consecutive appends to verify incremental growth and concatenation behavior.
bool test_consecutive_appends() {
    LsmString s;
    s.nAlloc = 0;
    s.z = nullptr;
    s.n = 0;

    int rc1 = lsmStringAppend(&s, "foo", 3);
    int rc2 = lsmStringAppend(&s, "bar", 3);

    EXPECT_TRUE(rc1 == 0);
    EXPECT_TRUE(rc2 == 0);
    // After two appends, the content should be "foobar"
    EXPECT_TRUE(s.n == 6);
    EXPECT_TRUE(s.z != nullptr);
    EXPECT_TRUE(std::memcmp(s.z, "foobar", 7) == 0);

    free(s.z);
    return g_failed == 0;
}

// Test 4: N == 0 should append only the terminator while not increasing the logical length.
bool test_N_zero_appends_only_terminator() {
    LsmString s;
    s.nAlloc = 0;
    s.z = nullptr;
    s.n = 0;

    int rc = lsmStringAppend(&s, "X", 0); // N == 0, should copy 1 byte: '\0'

    EXPECT_TRUE(rc == 0);
    // N should remain 0, as we appended 0 non-terminator characters
    EXPECT_TRUE(s.n == 0);
    EXPECT_TRUE(s.z != nullptr);
    // The first byte should be the null terminator
    EXPECT_TRUE(s.z[0] == '\0');

    free(s.z);
    return g_failed == 0;
}

// Test 5: N negative with empty string should allocate and copy terminator only.
bool test_negative_N_with_empty_string() {
    LsmString s;
    s.nAlloc = 0;
    s.z = nullptr;
    s.n = 0;

    const char *src = "";
    int rc = lsmStringAppend(&s, src, -1); // N < 0 -> strlen("") = 0, copy 1 byte

    EXPECT_TRUE(rc == 0);
    EXPECT_TRUE(s.n == 0);
    EXPECT_TRUE(s.z != nullptr);
    EXPECT_TRUE(s.z[0] == '\0');

    free(s.z);
    return g_failed == 0;
}

// Main entry point to run all tests
int main() {
    std::cout << "Running test suite for lsmStringAppend (lsm_str.c) under C++11" << std::endl;

    bool t1 = test_basic_append_with_positive_N();
    bool t2 = test_negative_N_path();
    bool t3 = test_consecutive_appends();
    bool t4 = test_N_zero_appends_only_terminator();
    bool t5 = test_negative_N_with_empty_string();

    int total = g_total;
    int failures = g_failed;

    std::cout << "Test summary: total=" << total << ", failures=" << failures << std::endl;
    if(failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
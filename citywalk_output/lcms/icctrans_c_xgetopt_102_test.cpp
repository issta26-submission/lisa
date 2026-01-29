// Unit test suite for the focal method xgetopt (as defined in icctrans.c).
// This test suite is written for C++11 without GoogleTest. It uses a lightweight
// non-terminating assertion mechanism to exercise the xgetopt function and its
// global state dependencies (SW, xoptind, letP, xoptarg, xopterr).
// The tests assume the linked implementation provides the required extern
// symbols. We initialize and reset global state between tests to ensure isolation.

#include <cstring>
#include <mex.h>
#include <stdarg.h>
#include <iostream>
#include <lcms2.h>
#include <string.h>


// Forward declaration of the function under test (C linkage).
extern "C" int xgetopt(int argc, char *argv[], char *optionS);

// Global/state variables used by xgetopt in the focal implementation.
// We declare them as extern to reference the definitions from icctrans.c.
// Note: The actual types in the focal implementation may differ slightly.
// For linking purposes, these declarations are sufficient as the symbols are
// resolved by the linker and only their addresses are used by xgetopt.
extern "C" int SW;
extern "C" int xoptind;
extern "C" char *letP;
extern "C" char *xoptarg;
extern "C" int xopterr;

// Optional: FatalError is defined in icctrans.c (or its headers). We declare it
// here to satisfy linker resolution, though our tests avoid triggering the error path.
extern "C" void FatalError(const char *frm, ...);

// Lightweight test framework: non-terminating assertions.
static int gTotalTests = 0;
static int gFailedTests = 0;

#define TEST_PASSED() do { \
    std::cout << "  [OK] " << __func__ << "\n"; \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        ++gFailedTests; \
        std::cerr << "  [FAIL] " << __func__ << ": " << (msg) << "\n"; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        ++gFailedTests; \
        std::cerr << "  [FAIL] " << __func__ << ": " << (msg) \
                  << " | Expected: " << (b) << ", Got: " << (a) << "\n"; \
    } \
} while (0)

#define RUN_TEST(test) do { \
    gTotalTests++; \
    test(); \
} while (0)

// Test 1: When no arguments are provided (argc <= xoptind), xgetopt should return EOF
// and not touch xoptarg. This exercises the EOF path at the top of the function.
void test_no_args_returns_EOF()
{
    // Reset global state
    SW = 0;
    xoptind = 0;
    letP = nullptr;
    xoptarg = nullptr;
    xopterr = 0; // Do not trigger FatalError path in tests

    int argc = 0;
    char **argv = nullptr;
    int ret = xgetopt(argc, argv, "a:");

    EXPECT_EQ(ret, EOF, "Expected EOF when no arguments are provided");
    EXPECT_TRUE(xoptarg == nullptr, "Expected xoptarg to be NULL on EOF path");
    TEST_PASSED();
}

// Test 2: Arg in the form /a with option string "a:" (requires argument).
// argv = { "/a", "value" }. Expect to receive option 'a', and xoptarg points to "value".
void test_required_argument_parsed()
{
    // Reset global state
    SW = 0;
    xoptind = 0;
    letP = nullptr;
    xoptarg = nullptr;
    xopterr = 0;

    // Prepare argv
    char *arg0 = (char*)"/a";
    char *arg1 = (char*)"value";
    char *argv2[] = { arg0, arg1, nullptr };
    int argc = 2;

    int ret = xgetopt(argc, argv2, "a:");

    EXPECT_EQ(ret, 'a', "Expected option 'a' to be parsed");
    EXPECT_TRUE(xoptarg == arg1, "Expected xoptarg to point to the argument string 'value'");
    EXPECT_EQ(xoptind, 2, "Expected xoptind to be advanced to 2 after processing");
    EXPECT_TRUE(letP == nullptr, "Expected letP to be NULL after processing argument");
    TEST_PASSED();
}

// Test 3: Arg in the form /b with option string "b" (no required argument).
// argv = { "/b" }. Expect to receive option 'b' and xoptarg remains NULL.
void test_no_argument_not_required()
{
    // Reset global state
    SW = 0;
    xoptind = 0;
    letP = nullptr;
    xoptarg = nullptr;
    xopterr = 0;

    char *arg0 = (char*)"/b";
    char *argv3[] = { arg0, nullptr };
    int argc = 1;

    int ret = xgetopt(argc, argv3, "b");

    EXPECT_EQ(ret, 'b', "Expected option 'b' to be parsed");
    EXPECT_TRUE(xoptarg == nullptr, "Expected xoptarg to be NULL when option does not require an argument");
    EXPECT_EQ(xoptind, 1, "Expected xoptind to be advanced to 1 after processing");
    EXPECT_TRUE(letP == nullptr, "Expected letP to be NULL after processing");
    TEST_PASSED();
}

// Main function to run all tests
int main()
{
    std::cout << "Starting unit tests for xgetopt...\n";

    RUN_TEST(test_no_args_returns_EOF);
    RUN_TEST(test_required_argument_parsed);
    RUN_TEST(test_no_argument_not_required);

    std::cout << "Test results: " << gTotalTests << " tests, "
              << gFailedTests << " failures.\n";

    if (gFailedTests > 0) {
        std::cout << "Some tests FAILED.\n";
        return 1;
    } else {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
}
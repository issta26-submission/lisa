// Test suite for the focal method: matchname (int arg, int argc, char **argv, char *fname)
// This test harness provides a controlled environment with a lightweight
// ExprMatch implementation to exercise the true/false branches of matchname.
// The tests avoid GTest and use non-terminating assertions that log results.

#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>


// Forward declaration of the function under test (as in the provided code).
int matchname (int arg,int argc,char **argv,char *fname);

// Lightweight, test-oriented implementation of ExprMatch.
// This function is used by matchname to decide whether a given argument matches the filename.
int ExprMatch (char *string,char *expr)
{
    // Test mapping to exercise true branches in various scenarios.

    // Case 1: fname == "foo" and argv entry == "match1" -> should be a match.
    if (strcmp(string, "foo") == 0 && strcmp(expr, "match1") == 0) return 1;

    // Case 2: fname == "target.txt" and argv entry == "pattern" -> should be a match.
    if (strcmp(string, "target.txt") == 0 && strcmp(expr, "pattern") == 0) return 1;

    // Case 3: fname == "bar" and argv entry == "x" -> should be a match.
    if (strcmp(string, "bar") == 0 && strcmp(expr, "x") == 0) return 1;

    // Default: no match
    return 0;
}

// Minimal test framework (non-terminating assertions).
static int total_tests = 0;
static int failure_count = 0;

#define EXPECT_TRUE(cond, msg) do { \
    total_tests++; \
    if (!(cond)) { \
        std::cerr << "FAIL: " << msg << std::endl; \
        failure_count++; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while (0)

// Test Case 1: Arg equals argc -> matchname should return 1 (no args processed).
void test_arg_equals_argc()
{
    // Prepare dummy argv; not used in this path, but provided for realism.
    char *argv[] = { (char*)"ignored", (char*)"dummy", (char*)"also" };
    int arg = 3;
    int argc = 3;
    char fname[] = "anyfile";

    int result = matchname(arg, argc, argv, fname);

    EXPECT_TRUE(result == 1, "Case 1 - arg == argc => matchname should return 1");
}

// Test Case 2: First provided argv entry matches ExprMatch -> immediate success.
void test_first_arg_matches()
{
    // fname "foo" with argv[1] "match1" should trigger ExprMatch true.
    char *argv[] = { (char*)"ignored", (char*)"match1" };
    int arg = 1;
    int argc = 2;
    char fname[] = "foo";

    int result = matchname(arg, argc, argv, fname);

    EXPECT_TRUE(result == 1, "Case 2 - first argv entry matches (foo, match1) => 1");
}

// Test Case 3: No matches across all argv entries -> should return 0.
void test_no_match()
{
    // fname "bar" with argv entries that do not trigger a match.
    char *argv[] = { (char*)"ignored", (char*)"n", (char*)"m" };
    int arg = 1;
    int argc = 3;
    char fname[] = "bar";

    int result = matchname(arg, argc, argv, fname);

    EXPECT_TRUE(result == 0, "Case 3 - no matching argv entries for (bar, n/m) => 0");
}

// Test Case 4: Last argv entry matches (non-initial match after earlier non-matches).
void test_last_arg_matches()
{
    // fname "target.txt" with argv[1] "xx" (no match) and argv[2] "pattern" (match)
    char *argv[] = { (char*)"ignored", (char*)"xx", (char*)"pattern" };
    int arg = 1;
    int argc = 3;
    char fname[] = "target.txt";

    int result = matchname(arg, argc, argv, fname);

    EXPECT_TRUE(result == 1, "Case 4 - last argv entry matches after prior non-match => 1");
}

// Implement a standalone copy of matchname from the focal method for testing.
// The braces are kept identical to the provided snippet to ensure comparable behavior.
int matchname (int arg,int argc,char **argv,char *fname)
{
{
  if (arg == argc)      
    return 1;
  while (arg < argc)
    if (ExprMatch(fname,argv[arg++]))
      return 1;
  return 0; 
}
}

// main() to run tests.
// According to <DOMAIN_KNOWLEDGE> point 10, call test methods from main when not using a test framework.
int main()
{
    test_arg_equals_argc();
    test_first_arg_matches();
    test_no_match();
    test_last_arg_matches();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total tests: " << total_tests
              << ", Failures: " << failure_count << std::endl;
    // Return non-zero if any test failed to signal issues to the harness.
    return failure_count;
}
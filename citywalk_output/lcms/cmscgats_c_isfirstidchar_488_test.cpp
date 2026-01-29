/*
Unit test suite for cmsBool isfirstidchar(int c)

Step 1 - Program Understanding summary (embedded as comments):
- Core predicates involved:
  - c != '-'  (first character cannot be a dash)
  - !isdigit(c) (first character cannot be a digit)
  - ismiddle(c) (character must be acceptable as a middle character in an identifier)
- The test targets the public (extern "C") function isfirstidchar which relies on ismiddle.
- We assume typical semantics for ismiddle: letters and underscore are acceptable; digits are not (but digits are already excluded by isdigit).
- Candidate Keywords (derived from the focal method and dependencies): isfirstidchar, ismiddle, isdigit, '-', cmsBool, false/true outcomes.

Step 2 - Generated test suite plan:
- Provide tests that exercise true and false branches of the predicate:
  - c == '-' should be false
  - c is a digit should be false
  - c is a letter (e.g., 'A', 'G') should be true (assuming ismiddle allows letters)
  - c is '_' (underscore) should be true (commonly allowed as a middle identifier character)
  - c is another symbol like '$' should be false (assuming ismiddle disallows it)
- Use a small, self-contained C++11 test harness (no GTest) with non-terminating checks that log failures and continue.
- Access the C function via extern "C" prototype, linking against the existing cmscgats.c object file in the build.

Step 3 - Test Case Refinement:
- The tests verify both branches of the predicate (true/false outcomes) for representative values.
- Tests are designed to be robust against typical ismiddle semantics (letters/underscore allowed for middle, unusual symbols not allowed).
- Static members and private details are not involved; the test calls the function through its external interface.
- A lightweight test harness prints per-test results and a final summary.

Note: This test file is designed to be compiled as C++11 and linked with the C source cmscgats.c (or its object) that provides cmsBool, ismiddle, and isfirstidchar.

*/

// Include standard I/O and character checks for test scaffolding
#include <cctype>
#include <lcms2_internal.h>
#include <iostream>


// Include the project header(s) to ensure cmsBool is defined (if available)
extern "C" {
}

// Declare the function under test (C linkage)
extern "C" cmsBool isfirstidchar(int c);

// Simple non-terminating test assertion framework
static int gFailures = 0;

static void logFailure(const char* testName, const char* detail) {
    std::cerr << "FAIL: " << testName << " - " << detail << std::endl;
    ++gFailures;
}

#define ASSERT_TRUE(cond, testName, detail) do { if(!(cond)) logFailure(testName, detail); } while(0)
#define ASSERT_FALSE(cond, testName, detail) do { if((cond)) logFailure(testName, detail); } while(0)


// Candidate Test 1: dash '-' should be rejected as first id char
static void test_dash_false() {
    cmsBool res = isfirstidchar('-');
    // Expected: false (0)
    ASSERT_FALSE(res, "test_dash_false", "isfirstidchar('-') should be false since c == '-'");
}

// Candidate Test 2: digit '0' should be rejected as first id char
static void test_digit_false() {
    cmsBool res = isfirstidchar('0');
    // Expected: false (0) due to isdigit constraint
    ASSERT_FALSE(res, "test_digit_false", "isfirstidchar('0') should be false because it's a digit");
}

// Candidate Test 3: uppercase letter should be accepted as first id char (typical case)
static void test_letter_true() {
    cmsBool res = isfirstidchar('A');
    // Expected: true (non-zero)
    ASSERT_TRUE(res, "test_letter_true", "isfirstidchar('A') should be true for a valid starting letter");
}

// Candidate Test 4: underscore '_' should be accepted as first id char (common middle/first allow)
static void test_underscore_true() {
    cmsBool res = isfirstidchar('_');
    // Expected: true (underscore commonly allowed as id start/middle)
    ASSERT_TRUE(res, "test_underscore_true", "isfirstidchar('_') should be true if underscore is allowed");
}

// Candidate Test 5: other symbol like '$' should be rejected as first id char
static void test_symbol_false() {
    cmsBool res = isfirstidchar('$');
    // Expected: false (0) since it's not a valid id character
    ASSERT_FALSE(res, "test_symbol_false", "isfirstidchar('$') should be false as '$' is not a valid id char");
}


// Optional: Run all tests and report summary
static void runAllTests() {
    test_dash_false();
    test_digit_false();
    test_letter_true();
    test_underscore_true();
    test_symbol_false();
}

// Entry point
int main() {
    std::cout << "Running cmsBool isfirstidchar unit tests (C function via C linkage in C++11 test harness)" << std::endl;
    runAllTests();
    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << gFailures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
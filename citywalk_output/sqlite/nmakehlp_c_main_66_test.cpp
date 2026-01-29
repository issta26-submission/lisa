// Test suite for the focal method main in nmakehlp.c
// This test harness intentionally focuses on safe, non-UB execution paths of the main function.
// It uses a minimal, custom test runner (no GTest) as requested.
// Note: The focal code relies on Windows API; this test is designed to be run in an environment
// where the focal code can be linked (e.g., Windows). We only exercise branches that do not
// require the internal feature-detection functions (which have stubbed/unsafe bodies in the class dep).

#include <math.h>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>


// Declare the focal C entry point with C linkage so we can call it from C++.
extern "C" int main(int argc, char **argv);

// Helper: run the focal program's main with given arguments.
// The first element should be the program name (argv[0]).
int run_main_with_args(const std::vector<std::string>& args) {
    // Build a contiguous argv array for the focal main.
    // We keep storage alive for the duration of the call.
    std::vector<std::string> storage;
    storage.reserve(args.size());
    for (const auto& a : args) storage.push_back(a);

    // Build the array of char* pointers to the C-strings stored in storage.
    std::vector<char*> c_argv;
    c_argv.reserve(storage.size());
    for (auto &s : storage) {
        c_argv.push_back(const_cast<char*>(s.c_str()));
    }

    // Call the focal main.
    return main(static_cast<int>(storage.size()), c_argv.data());
}

// Simple assertion helper
bool assert_eq(int expected, int actual, const std::string& test_desc) {
    if (expected == actual) {
        std::cout << "[PASS] " << test_desc << " (expected " << expected << ", got " << actual << ")\n";
        return true;
    } else {
        std::cout << "[FAIL] " << test_desc << " (expected " << expected << ", got " << actual << ")\n";
        return false;
    }
}

int main() {
    // List of test cases for Step 2: Unit Test Generation
    // We intentionally cover true/false branches of each safe predicate in the main function.
    // Each test includes a description, arguments, and the expected exit code.

    struct TestCase {
        std::string description;
        std::vector<std::string> args;
        int expected;
    };

    // Test 1: No arguments provided (argc == 1)
    // Description: Should show usage and return exit code 2 as per the final fallback.
    TestCase t1 = {
        "No arguments: program should print usage and return 2",
        {"nmakehlp"},
        2
    };

    // Test 2: -c without the required compiler option (argc != 3)
    // Description: Should print usage for -c and return 2 without invoking the feature check.
    TestCase t2 = {
        "-c with insufficient args: should return 2",
        {"nmakehlp","-c"},
        2
    };

    // Test 3: -l without a required option (argc < 3)
    // Description: Should print usage for -l and return 2.
    TestCase t3 = {
        "-l with insufficient args: should return 2",
        {"nmakehlp","-l"},
        2
    };

    // Test 4: -f with only the flag (argc == 2)
    // Description: Should print usage for -f and return 2.
    TestCase t4 = {
        "-f with only flag: should return 2",
        {"nmakehlp","-f"},
        2
    };

    // Test 5: -f with argc == 3 (subcase: string present but no substring)
    // Description: Should return 0 (no match when string is present but substring missing).
    TestCase t5 = {
        "-f with argc == 3: should return 0",
        {"nmakehlp","-f","abc"},
        0
    };

    // Test 6: -s with insufficient args (argc == 2)
    // Description: Should print usage for -s and return 2.
    TestCase t6 = {
        "-s with insufficient args: should return 2",
        {"nmakehlp","-s"},
        2
    };

    // Test 7: -V with argc != 4 (argc == 2)
    // Description: Should print usage-like message for -V and return 0.
    TestCase t7 = {
        "-V with argc != 4: should return 0",
        {"nmakehlp","-V"},
        0
    };

    // Test 8: -Q with insufficient args (argc != 3)
    // Description: Should print usage for -Q and return 2.
    TestCase t8 = {
        "-Q with insufficient args: should return 2",
        {"nmakehlp","-Q"},
        2
    };

    // Test 9: -L with insufficient args (argc != 3)
    // Description: Should print usage for -L and return 2.
    TestCase t9 = {
        "-L with insufficient args: should return 2",
        {"nmakehlp","-L"},
        2
    };

    // Test 10: Non-dash second argument (no recognized option)
    // Description: Should reach the final usage and return 2.
    TestCase t10 = {
        "Non-dash second argument: should return 2",
        {"nmakehlp","foo"},
        2
    };

    // Aggregate tests
    std::vector<TestCase> tests = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10};

    int passed = 0;
    for (size_t i = 0; i < tests.size(); ++i) {
        int result = run_main_with_args(tests[i].args);
        if (assert_eq(tests[i].expected, result, "Test " + std::to_string(i + 1) + ": " + tests[i].description)) {
            ++passed;
        }
    }

    std::cout << "\nTest suite finished: " << passed << " / " << tests.size() << " tests passed.\n";

    return (passed == static_cast<int>(tests.size())) ? 0 : 1;
}
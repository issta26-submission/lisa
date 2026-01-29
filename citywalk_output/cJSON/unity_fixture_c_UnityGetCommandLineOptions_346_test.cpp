/*
Unit test suite for UnityGetCommandLineOptions (unity_fixture.c)
- This test suite is written in C++11 (no GTest) and relies on the Unity testing framework
  behavior compiled in the project (unity_fixture.c and unity_fixture.h).
- The test cases exercise various command-line argument combinations and verify the
  side effects on UnityFixture fields as well as the return codes.
- Each test includes comments describing the intent and expected behavior.
- Build guidance (not part of code) is provided separately; this file should be linked
  with unity_fixture.c and any other project sources.
*/

#include <unity_internals.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unity_fixture.h>


// Forward declaration of the focal C function (C linkage)
extern "C" int UnityGetCommandLineOptions(int argc, const char* argv[]);

// Include Unity fixture header (defines UnityFixture and related types)

// Access to the UnityFixture global defined in unity_fixture.c
extern "C" struct UNITY_FIXTURE_T UnityFixture;

/*
Test case 1: No additional arguments (argc == 1)
Expected: return 0 and all defaults set (Verbose=0, GroupFilter=null, NameFilter=null, RepeatCount=1)
*/
static bool test_empty_args_defaults()
{
    const char* program = "test_program";
    const char* argv[] = { program };
    int argc = 1;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.Verbose == 0)
        && (UnityFixture.GroupFilter == 0)
        && (UnityFixture.NameFilter == 0)
        && (UnityFixture.RepeatCount == 1);

    if (!ok) {
        std::cout << "[FAIL] test_empty_args_defaults: "
                  << "result=" << result
                  << ", Verbose=" << UnityFixture.Verbose
                  << ", GroupFilter=" << (UnityFixture.GroupFilter ? UnityFixture.GroupFilter : "NULL")
                  << ", NameFilter=" << (UnityFixture.NameFilter ? UnityFixture.NameFilter : "NULL")
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 2: Enable verbose (-v)
Expected: return 0, Verbose=1, others unchanged (GroupFilter=null, NameFilter=null, RepeatCount=1)
*/
static bool test_verbose_flag()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-v" };
    int argc = 2;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.Verbose == 1)
        && (UnityFixture.GroupFilter == 0)
        && (UnityFixture.NameFilter == 0)
        && (UnityFixture.RepeatCount == 1);

    if (!ok) {
        std::cout << "[FAIL] test_verbose_flag: "
                  << "result=" << result
                  << ", Verbose=" << UnityFixture.Verbose
                  << ", GroupFilter=" << (UnityFixture.GroupFilter ? UnityFixture.GroupFilter : "NULL")
                  << ", NameFilter=" << (UnityFixture.NameFilter ? UnityFixture.NameFilter : "NULL")
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 3: Missing argument after -g (expected error)
- -g without a following value should return 1 (error)
*/
static bool test_missing_group_arg_error()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-g" };
    int argc = 2;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 1)
        && (UnityFixture.GroupFilter == 0); // should remain default

    if (!ok) {
        std::cout << "[FAIL] test_missing_group_arg_error: "
                  << "result=" << result
                  << ", GroupFilter=" << (UnityFixture.GroupFilter ? UnityFixture.GroupFilter : "NULL")
                  << "\n";
    }
    return ok;
}

/*
Test case 4: -g with a value sets the group filter
- -g GroupA
*/
static bool test_group_flag_sets_group()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-g", "GroupA" };
    int argc = 3;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.GroupFilter != 0)
        && (strcmp(UnityFixture.GroupFilter, "GroupA") == 0)
        && (UnityFixture.Verbose == 0)
        && (UnityFixture.NameFilter == 0)
        && (UnityFixture.RepeatCount == 1);

    if (!ok) {
        std::cout << "[FAIL] test_group_flag_sets_group: "
                  << "result=" << result
                  << ", GroupFilter=" << (UnityFixture.GroupFilter ? UnityFixture.GroupFilter : "NULL")
                  << ", Verbose=" << UnityFixture.Verbose
                  << ", NameFilter=" << (UnityFixture.NameFilter ? UnityFixture.NameFilter : "NULL")
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 5: -n with a value sets the name filter
- -n TestName
*/
static bool test_name_flag_sets_name()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-n", "TestName" };
    int argc = 3;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.NameFilter != 0)
        && (strcmp(UnityFixture.NameFilter, "TestName") == 0)
        && (UnityFixture.Verbose == 0)
        && (UnityFixture.GroupFilter == 0)
        && (UnityFixture.RepeatCount == 1);

    if (!ok) {
        std::cout << "[FAIL] test_name_flag_sets_name: "
                  << "result=" << result
                  << ", NameFilter=" << (UnityFixture.NameFilter ? UnityFixture.NameFilter : "NULL")
                  << ", Verbose=" << UnityFixture.Verbose
                  << ", GroupFilter=" << (UnityFixture.GroupFilter ? UnityFixture.GroupFilter : "NULL")
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 6: -r without following digits should set RepeatCount to 2 (default when -r is present)
*/
static bool test_repeat_flag_without_digits_sets_two()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-r" };
    int argc = 2;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.RepeatCount == 2)
        && (UnityFixture.Verbose == 0)
        && (UnityFixture.GroupFilter == 0)
        && (UnityFixture.NameFilter == 0);

    if (!ok) {
        std::cout << "[FAIL] test_repeat_flag_without_digits_sets_two: "
                  << "result=" << result
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 7: -r with digits parses the number
- -r 12
*/
static bool test_repeat_flag_with_digits_sets_value()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-r", "12" };
    int argc = 3;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.RepeatCount == 12)
        && (UnityFixture.Verbose == 0)
        && (UnityFixture.GroupFilter == 0)
        && (UnityFixture.NameFilter == 0);

    if (!ok) {
        std::cout << "[FAIL] test_repeat_flag_with_digits_sets_value: "
                  << "result=" << result
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 8: Unknown parameter is ignored
- -x should not affect any settings and return 0
*/
static bool test_unknown_param_ignored()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-x" };
    int argc = 2;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.Verbose == 0)
        && (UnityFixture.GroupFilter == 0)
        && (UnityFixture.NameFilter == 0)
        && (UnityFixture.RepeatCount == 1);

    if (!ok) {
        std::cout << "[FAIL] test_unknown_param_ignored: "
                  << "result=" << result
                  << ", Verbose=" << UnityFixture.Verbose
                  << ", GroupFilter=" << (UnityFixture.GroupFilter ? UnityFixture.GroupFilter : "NULL")
                  << ", NameFilter=" << (UnityFixture.NameFilter ? UnityFixture.NameFilter : "NULL")
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 9: Combination of options
- -v -g Group -n Name -r 5
*/
static bool test_combined_options()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-v", "-g", "Group", "-n", "Name", "-r", "5" };
    int argc = 7;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.Verbose == 1)
        && (UnityFixture.GroupFilter != 0 && strcmp(UnityFixture.GroupFilter, "Group") == 0)
        && (UnityFixture.NameFilter != 0 && strcmp(UnityFixture.NameFilter, "Name") == 0)
        && (UnityFixture.RepeatCount == 5);

    if (!ok) {
        std::cout << "[FAIL] test_combined_options: "
                  << "result=" << result
                  << ", Verbose=" << UnityFixture.Verbose
                  << ", GroupFilter=" << (UnityFixture.GroupFilter ? UnityFixture.GroupFilter : "NULL")
                  << ", NameFilter=" << (UnityFixture.NameFilter ? UnityFixture.NameFilter : "NULL")
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test case 10: -r with non-digit following
- -r abc -> RepeatCount should remain the default 2 (since digits were not found)
*/
static bool test_repeat_with_non_digits_following()
{
    const char* program = "test_program";
    const char* argv[] = { program, "-r", "abc" };
    int argc = 3;

    int result = UnityGetCommandLineOptions(argc, argv);

    bool ok = (result == 0)
        && (UnityFixture.RepeatCount == 2)
        && (UnityFixture.Verbose == 0)
        && (UnityFixture.GroupFilter == 0)
        && (UnityFixture.NameFilter == 0);

    if (!ok) {
        std::cout << "[FAIL] test_repeat_with_non_digits_following: "
                  << "result=" << result
                  << ", RepeatCount=" << UnityFixture.RepeatCount
                  << "\n";
    }
    return ok;
}

/*
Test harness: define test cases and run them in main().
Each test prints PASS/FAIL and a brief description.
*/
struct TestCase {
    const char* name;
    bool (*fn)();
};

static TestCase tests[] = {
    { "test_empty_args_defaults", test_empty_args_defaults },
    { "test_verbose_flag", test_verbose_flag },
    { "test_missing_group_arg_error", test_missing_group_arg_error },
    { "test_group_flag_sets_group", test_group_flag_sets_group },
    { "test_name_flag_sets_name", test_name_flag_sets_name },
    { "test_repeat_flag_without_digits_sets_two", test_repeat_flag_without_digits_sets_two },
    { "test_repeat_flag_with_digits_sets_value", test_repeat_flag_with_digits_sets_value },
    { "test_unknown_param_ignored", test_unknown_param_ignored },
    { "test_combined_options", test_combined_options },
    { "test_repeat_with_non_digits_following", test_repeat_with_non_digits_following }
};

int main()
{
    const size_t total = sizeof(tests) / sizeof(tests[0]);
    size_t passed = 0;

    std::cout << "Running UnityGetCommandLineOptions unit tests (C++11) ...\n";

    for (size_t i = 0; i < total; ++i)
    {
        bool result = tests[i].fn();
        if (result)
            ++passed;
        // Each test prints its own PASS/FAIL message
        // The per-test function already outputs detailed info on failure
    }

    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
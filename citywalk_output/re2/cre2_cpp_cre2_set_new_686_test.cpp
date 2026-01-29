// Unit test suite for cre2_set_new in cre2.cpp
// This test suite is written for C++11, does not rely on Google Test,
// and uses a lightweight, non-terminating assertion approach.
// It exercises basic creation paths of cre2_set_new using cre2_opt_new
// and two different anchor values. It also checks that multiple sets can be created
// with the same options and are distinct.

// Note: The tests assume the presence of the cre2.h header and related cre2_*
// functions as described in the provided class dependencies.

#include <cre2.h>
#include <cstddef>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


extern "C" {
}

// Simple non-terminating assertion helper
static bool ASSERT(bool condition, const char* message)
{
    if (!condition) {
        if (message) {
            std::cerr << "Assertion failed: " << message << std::endl;
        } else {
            std::cerr << "Assertion failed" << std::endl;
        }
        return false;
    }
    return true;
}

// Test 1: Basic cre2_set_new creation with anchor value 0
// This validates that a valid options object can be used to create a Set
// and that the function returns a non-null pointer.
static bool test_cre2_set_new_basic_creation()
{
    bool all_ok = true;

    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!ASSERT(opt != nullptr, "cre2_opt_new() should return non-null")) {
        return false;
    }

    // Create set with anchor 0 (cast to cre2_anchor_t)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!ASSERT(set != nullptr, "cre2_set_new() should return non-null for anchor 0")) {
        all_ok = false;
    }

    // Cleanup
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return all_ok;
}

// Test 2: cre2_set_new creation with a different anchor value (1)
// This ensures another code path (anchor value) is handled without crashing.
static bool test_cre2_set_new_another_anchor()
{
    bool all_ok = true;

    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!ASSERT(opt != nullptr, "cre2_opt_new() should return non-null")) {
        return false;
    }

    // Create set with anchor 1
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)1);
    if (!ASSERT(set != nullptr, "cre2_set_new() should return non-null for anchor 1")) {
        all_ok = false;
    }

    // Cleanup
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return all_ok;
}

// Test 3: Creating multiple sets with the same options should yield distinct pointers
static bool test_cre2_set_new_multiple_distinct_sets()
{
    bool all_ok = true;

    // Create single options object
    cre2_options_t *opt = cre2_opt_new();
    if (!ASSERT(opt != nullptr, "cre2_opt_new() should return non-null")) {
        return false;
    }

    // Create first set
    cre2_set *set1 = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!ASSERT(set1 != nullptr, "First cre2_set_new() should return non-null")) {
        all_ok = false;
    }

    // Create second set with same options
    cre2_set *set2 = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!ASSERT(set2 != nullptr, "Second cre2_set_new() should return non-null")) {
        all_ok = false;
    }

    // Ensure distinct pointers
    if (set1 && set2) {
        ASSERT(set1 != set2, "Two sets created with the same options should be distinct objects");
    }

    // Cleanup
    if (set1) cre2_set_delete(set1);
    if (set2) cre2_set_delete(set2);
    if (opt) cre2_opt_delete(opt);

    return all_ok;
}

// Simple test runner
static int run_tests()
{
    int failures = 0;

    if (!test_cre2_set_new_basic_creation()) {
        std::cerr << "[Test] test_cre2_set_new_basic_creation failed" << std::endl;
        ++failures;
    } else {
        std::cout << "[Test] test_cre2_set_new_basic_creation passed" << std::endl;
    }

    if (!test_cre2_set_new_another_anchor()) {
        std::cerr << "[Test] test_cre2_set_new_another_anchor failed" << std::endl;
        ++failures;
    } else {
        std::cout << "[Test] test_cre2_set_new_another_anchor passed" << std::endl;
    }

    if (!test_cre2_set_new_multiple_distinct_sets()) {
        std::cerr << "[Test] test_cre2_set_new_multiple_distinct_sets failed" << std::endl;
        ++failures;
    } else {
        std::cout << "[Test] test_cre2_set_new_multiple_distinct_sets passed" << std::endl;
    }

    return failures;
}

int main()
{
    int result = run_tests();
    if (result != 0) {
        std::cout << "Unit tests completed with " << result << " failure(s)." << std::endl;
    } else {
        std::cout << "All unit tests passed." << std::endl;
    }
    return result;
}
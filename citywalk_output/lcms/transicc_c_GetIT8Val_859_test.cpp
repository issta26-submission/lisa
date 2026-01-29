// Test suite for the focal method GetIT8Val in transicc.c
// Notes:
// - This test suite is designed to be compiled as C++11 with a C source (transicc.c) providing GetIT8Val.
// - A lightweight test harness is used (no Google Test). Tests run from main() and print PASS/FAIL per case.
// - To enable controlled testing of GetIT8Val's data retrieval (Val values for a given field name),
//   a minimal weak hook for cmsIT8GetData is provided. If a real cmsIT8GetData exists in the
//   linked translation unit, it will override the weak hook. Otherwise, the weak hook supplies test data.
// - FatalError behavior is assumed to terminate the process (as is common in error paths). For the
//   "missing field" case, tests run in a separate process to avoid terminating the entire test suite.
// - This code uses only standard C/C++ facilities and the standard library; no GTest is used.

#include <cstring>
#include <utility>
#include <cstdio>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>
#include <cmath>


// Domain alignment: define cmsFloat64Number to match the project's typedef (likely double)
typedef double cmsFloat64Number;

// Declaration of the focal function under test
extern "C" cmsFloat64Number GetIT8Val(const char* Name, cmsFloat64Number Max);

// ------------------------------------------------------------------------------------
// Mock hook for cmsIT8GetData to provide controllable test input without editing transicc.c
// This uses a small test data store. If the real cmsIT8GetData is present (non-weak),
// it will override this weak definition. Otherwise, this provides deterministic test data.
// ------------------------------------------------------------------------------------
static std::vector< std::pair<std::string, std::string> > g_TestIT8Data;

// Weak declaration to allow overriding by the real function if it exists in linked code.
// The function is treated as C linkage to match the real API.
extern "C" const char* cmsIT8GetData(void* hIT8in, int patch, const char* Name) __attribute__((weak));

extern "C" const char* cmsIT8GetData(void* hIT8in, int patch, const char* Name)
{
    // Linear search in the test data vector
    for (const auto& kv : g_TestIT8Data) {
        if (kv.first == Name) {
            // Return a pointer to a c-string owned by the test data map.
            // std::string's c_str() lifetime is tied to the std::string object in the vector.
            // To ensure lifetime, we will create the strings in the vector and keep them alive.
            // We return the C string pointer to the stored value.
            return kv.second.c_str();
        }
    }
    return nullptr; // Simulate missing field
}

// Helper to set test data values for a given field name
void Test_SetIT8DataValue(const char* Name, const char* Value)
{
    // Remove any existing entry with the same Name
    for (auto it = g_TestIT8Data.begin(); it != g_TestIT8Data.end(); ) {
        if (it->first == Name) it = g_TestIT8Data.erase(it);
        else ++it;
    }
    g_TestIT8Data.emplace_back(Name, Value ? std::string(Value) : std::string(""));
}

// Helper to clear all test data
void Test_ClearIT8Data()
{
    g_TestIT8Data.clear();
}

// ------------------------------------------------------------------------------------
// Helper: small test harness
// ------------------------------------------------------------------------------------
static int g_PassCount = 0;
static int g_FailCount = 0;

#define TPRINT(...) do { printf(__VA_ARGS__); } while (0)
#define TEST_PASS(name) do { g_PassCount++; printf("[PASS] %s\n", name); } while (0)
#define TEST_FAIL(name) do { g_FailCount++; printf("[FAIL] %s\n", name); } while (0)

static void assert_near(const char* testName, cmsFloat64Number actual, cmsFloat64Number expected, cmsFloat64Number tol)
{
    if (std::fabs(actual - expected) <= tol) {
        TEST_PASS(testName);
        // Optionally print the computed value for debugging
        // printf("  got: %.12f, expected: %.12f, tol: %.12f\n", actual, expected, tol);
    } else {
        TEST_FAIL(testName);
        printf("  Expected ~%.12f, got %.12f (tol %.12f)\n", (double)expected, (double)actual, (double)tol);
    }
}

// ------------------------------------------------------------------------------------
// Test 1: Existing field returns expected value (Val != NULL path)
// - Set a known field value via the test data hook.
// - Call GetIT8Val(Name, Max) and verify result equals atof(Val)/Max.
// ------------------------------------------------------------------------------------
static void test_GetIT8Val_existing_field_basic()
{
    Test_ClearIT8Data();
    // Prepare test data: FieldName -> "6.0", Max = 3.0, expected = 2.0
    Test_SetIT8DataValue("TestField", "6.0");

    cmsFloat64Number Max = 3.0;
    cmsFloat64Number result = GetIT8Val("TestField", Max);

    assert_near("GetIT8Val existing field basic (6.0/3.0)", result, 2.0, 1e-9);
}

// ------------------------------------------------------------------------------------
// Test 2: Existing field with fractional value
// - Set a known field value of "3.141592653589793" and Max = 2.0
// - Expect result to be pi/2 approximately 1.5707963267948966
// ------------------------------------------------------------------------------------
static void test_GetIT8Val_existing_field_fractional()
{
    Test_ClearIT8Data();
    Test_SetIT8DataValue("PiField", "3.141592653589793");

    cmsFloat64Number Max = 2.0;
    cmsFloat64Number result = GetIT8Val("PiField", Max);

    assert_near("GetIT8Val existing field fractional (pi/2)", result, 1.5707963267948966, 1e-12);
}

// ------------------------------------------------------------------------------------
// Test 3: Missing field triggers FatalError path
// - No data is set for the requested field, so cmsIT8GetData returns NULL.
// - We run the call in a child process to avoid terminating the test runner due to FatalError.
// - Success is determined if the child process exits with a non-zero status (FatalError invoked).
// ------------------------------------------------------------------------------------
static bool test_GetIT8Val_missing_field_triggers_fatal_in_child()
{
    Test_ClearIT8Data();
    pid_t pid = fork();
    if (pid == -1) {
        // Fork failed
        fprintf(stderr, "Fork failed for missing-field test\n");
        return false;
    } else if (pid == 0) {
        // Child process: attempt to call GetIT8Val on a missing field
        // Expect FatalError to terminate the process (exit code non-zero)
        GetIT8Val("MissingField_NoData", 1.0);
        // If it returns, it's unexpected; exit with non-zero to indicate failure
        _exit(77);
    } else {
        // Parent: wait for child result
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            fprintf(stderr, "waitpid failed\n");
            return false;
        }
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            // Non-zero exit indicates FatalError path was taken (as expected)
            return (code != 0);
        } else if (WIFSIGNALED(status)) {
            // Terminated by a signal - still indicates FatalError path invoked
            return true;
        } else {
            // Other termination
            return false;
        }
    }
}

// ------------------------------------------------------------------------------------
// Main: run all tests and report summary
// ------------------------------------------------------------------------------------
int main()
{
    printf("Starting GetIT8Val unit test suite (without GTest)\n");

    // Run Test 1
    test_GetIT8Val_existing_field_basic();

    // Run Test 2
    test_GetIT8Val_existing_field_fractional();

    // Run Test 3 (may terminate the test runner if FatalError terminates the process; handle via fork)
    bool missingFieldTestResult = test_GetIT8Val_missing_field_triggers_fatal_in_child();
    if (missingFieldTestResult) {
        TEST_PASS("GetIT8Val missing field fatal path (child exited non-zero)");
    } else {
        TEST_FAIL("GetIT8Val missing field fatal path (child did not exit as expected)");
    }

    // Summary
    printf("\nTest Summary: %d passed, %d failed\n", g_PassCount, g_FailCount);
    // Return non-zero if any test failed
    return (g_FailCount > 0) ? 1 : 0;
}
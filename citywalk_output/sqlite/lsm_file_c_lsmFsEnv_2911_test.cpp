// C++11 test suite for the focal method: lsmFsEnv(FileSystem *pFS)
// This test harness uses a lightweight, non-terminating EXPECT-style assertion
// system (no Google Test). It assumes the project provides lsmInt.h and lsm_file.h
// with the actual FileSystem and lsm_env definitions.
// The tests are designed to be compiled together with the existing C sources.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstddef>
#include <lsm_file.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdint>


// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_TRUE] Condition failed: " #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_EQ] Expected equality: " #a " == " #b \
                  << " (actual: " << (a) << ", " << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b) do { \
    ++g_tests_run; \
    if(((void*)(a)) != ((void*)(b))) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_PTR_EQ] Pointers are not equal: " \
                  << (void*)(a) << " != " << (void*)(b) \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_NOT_NULL(a) do { \
    ++g_tests_run; \
    if((a) == nullptr) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_NOT_NULL] Pointer is null: " #a \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)


// Import C headers (lsmInt.h / lsm_file.h) to obtain real project types.
// We wrap in extern "C" to preserve C linkage for the focal function.
extern "C" {
}

// If the headers are not in the include path, the test won't compile here.
// The test suite relies on the actual project's struct layout for FileSystem and lsm_env.


// Test 1: When pFS.pEnv is a valid lsm_env*, lsmFsEnv should return the exact same pointer.
// This verifies the basic accessor behavior of the focal method.
void test_lsmFsEnv_ReturnsSamePointer_WhenEnvExists() {
    // Prepare a concrete lsm_env instance as expected environment
    lsm_env expectedEnv;
    std::memset(&expectedEnv, 0, sizeof(expectedEnv)); // initialize to deterministic state

    // Prepare a FileSystem instance with pEnv pointing to our expectedEnv
    FileSystem fs;
    std::memset(&fs, 0, sizeof(fs)); // zero initialize to avoid incidental padding issues
    fs.pEnv = &expectedEnv;

    // Call the function under test
    lsm_env *actualEnv = lsmFsEnv(&fs);

    // Assertions
    EXPECT_NOT_NULL(actualEnv);
    EXPECT_PTR_EQ(actualEnv, &expectedEnv);
    // Also ensure the pointer value is unchanged relative to the struct member
    EXPECT_EQ(fs.pEnv, &expectedEnv);
}

// Test 2: When pFS.pEnv is NULL, lsmFsEnv should return NULL.
// This validates the function handles a null environment gracefully.
void test_lsmFsEnv_ReturnsNull_WhenEnvIsNull() {
    FileSystem fs;
    std::memset(&fs, 0, sizeof(fs));
    fs.pEnv = nullptr;

    lsm_env *actualEnv = lsmFsEnv(&fs);

    EXPECT_EQ(actualEnv, nullptr);
}

// Test 3: Multiple FileSystem instances with distinct environments.
// Each lsmFsEnv call should return the corresponding environment pointer,
// not interfere with other instances.
void test_lsmFsEnv_MultipleInstances_DoNotShareEnvs() {
    lsm_env envA;
    lsm_env envB;
    std::memset(&envA, 0, sizeof(envA));
    std::memset(&envB, 0, sizeof(envB));

    FileSystem fsA;
    FileSystem fsB;
    std::memset(&fsA, 0, sizeof(fsA));
    std::memset(&fsB, 0, sizeof(fsB));

    fsA.pEnv = &envA;
    fsB.pEnv = &envB;

    lsm_env *resA = lsmFsEnv(&fsA);
    lsm_env *resB = lsmFsEnv(&fsB);

    EXPECT_PTR_EQ(resA, &envA);
    EXPECT_PTR_EQ(resB, &envB);
    EXPECT_NOT_NULL(resA);
    EXPECT_NOT_NULL(resB);
    // Ensure cross-instance independence
    EXPECT_PTR_EQ(resA, &envA);
    EXPECT_PTR_EQ(resB, &envB);
}

// Test 4: The call should not modify the FileSystem's pEnv field.
// This confirms the function has no side effects on the input structure.
void test_lsmFsEnv_NoSideEffectsOnFS() {
    lsm_env env;
    std::memset(&env, 0, sizeof(env));

    FileSystem fs;
    std::memset(&fs, 0, sizeof(fs));
    fs.pEnv = &env;

    // Take a snapshot of pEnv before call
    lsm_env *before = fs.pEnv;

    lsm_env *after = lsmFsEnv(&fs);

    // The returned pointer should be equal to before, and pEnv should remain unchanged
    EXPECT_PTR_EQ(after, before);
    EXPECT_EQ(fs.pEnv, before);
}


// Simple runner to execute tests and report results
void run_all_tests() {
    std::cout << "Starting lsmFsEnv unit tests...\n";

    test_lsmFsEnv_ReturnsSamePointer_WhenEnvExists();
    test_lsmFsEnv_ReturnsNull_WhenEnvIsNull();
    test_lsmFsEnv_MultipleInstances_DoNotShareEnvs();
    test_lsmFsEnv_NoSideEffectsOnFS();

    std::cout << "lsmFsEnv tests finished. "
              << "Total: " << g_tests_run
              << ", Passed: " << (g_tests_run - g_tests_failed)
              << ", Failed: " << g_tests_failed << "\n";

    // Exit with non-zero if any test failed to aid automation
    if (g_tests_failed > 0) {
        std::exit(1);
    }
}


// Entry point
int main() {
    run_all_tests();
    return 0;
}
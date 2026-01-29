// Test suite for lsmMutexNew in lsm_mutex.c
// - Uses a lightweight, non-terminating test framework (no Google Test).
// - Tests focus on the behavior of lsmMutexNew, which delegates to pEnv->xMutexNew.
// - The tests create a mock lsm_env with a controllable xMutexNew to verify pass-through
//   semantics, return codes, and proper argument propagation.
//
// Note: This test is designed to be linked with the real production code (lsm_mutex.c and lsmInt.h).

#include <lsmInt.h>
#include <iostream>
#include <cstdlib>
#include <cstring>


// Include the production header to obtain the type definitions (lsm_env, lsm_mutex, etc.)
// This header is expected to declare the lsmMutexNew function prototype as well.

extern "C" int lsmMutexNew(lsm_env *pEnv, lsm_mutex **ppNew); // forward declaration of the focal method

// Lightweight non-terminating test framework
static int g_test_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_EQ_INT(a, b) \
    do { \
        if((int)(a) != (int)(b)) { \
            std::cerr << "EXPECT_EQ_INT failed: " #a " != " #b " (" << (int)(a) << " != " << (int)(b) << ") at " \
                      << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_EQ_PTR(a, b) \
    do { \
        if((void*)(a) != (void*)(b)) { \
            std::cerr << "EXPECT_EQ_PTR failed: " #a " != " #b " (ptrs " << (void*)(a) << " != " << (void*)(b) << ") at " \
                      << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_NOT_NULL(p) \
    do { \
        if((p) == nullptr) { \
            std::cerr << "EXPECT_NOT_NULL failed: " #p " is null at " __FILE__ ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(0)

// Mock environment to simulate lsm_env with controllable xMutexNew
// The lsm_env struct is assumed to be the first member in MockEnv.base to allow
// passing MockEnv through as lsm_env*.
struct MockEnv {
    // The production lsm_env (as the first member)
    lsm_env base;

    // Mock controls
    int return_code;        // value to return from xMutexNew
    bool allocate;            // whether to allocate a new lsm_mutex on success
    int call_count;           // number of times xMutexNew was invoked
    lsm_mutex *last_allocated; // last allocated mutex pointer
    lsm_env *last_env_ptr;     // last env pointer that was passed into xMutexNew

    MockEnv() : return_code(0), allocate(true), call_count(0),
                last_allocated(nullptr), last_env_ptr(nullptr) {
        // Initialize function pointer to our mock
        base.xMutexNew = Mock_xMutexNew;
    }

    // Provide a static-like access point for the function (defined below)
    // (We are using a free function as the actual hook; this constructor ensures
    // xMutexNew points to the correct function.)
};

// Forward declaration of the mock function
static int Mock_xMutexNew(lsm_env *env, lsm_mutex **ppNew);

// Implementation of the mock xMutexNew
static int Mock_xMutexNew(lsm_env *env, lsm_mutex **ppNew) {
    // env is expected to be the address of MockEnv.base
    MockEnv *self = reinterpret_cast<MockEnv*>(env);

    // Record passed environment
    self->last_env_ptr = env;
    ++self->call_count;

    if (self->allocate) {
        // Allocate a placeholder lsm_mutex object (opaque to tests)
        // Use malloc to avoid C++ constructors.
        *ppNew = reinterpret_cast<lsm_mutex*>(malloc(sizeof(lsm_mutex)));
        self->last_allocated = *ppNew;
        // For safety, zero-initialize
        if (*ppNew) {
            std::memset(*ppNew, 0, sizeof(lsm_mutex));
        }
    } else {
        *ppNew = nullptr;
    }

    return self->return_code;
}

// Test 1: Successful mutex creation via xMutexNew
// - Ensures that lsmMutexNew returns the same code as pEnv->xMutexNew
// - Verifies that a non-null ppNew is returned when allocation is requested
// - Verifies that the environment pointer is passed through correctly
static void test_lsmMutexNew_success() {
    // Arrange
    MockEnv mock;
    mock.return_code = 0;
    mock.allocate = true;
    mock.call_count = 0;
    mock.last_allocated = nullptr;
    mock.last_env_ptr = nullptr;

    lsm_mutex *pNew = nullptr;

    // Act
    int rc = lsmMutexNew(reinterpret_cast<lsm_env*>(&mock.base), &pNew);

    // Assert
    EXPECT_EQ_INT(rc, 0);
    EXPECT_NOT_NULL(pNew);
    EXPECT_TRUE(mock.call_count == 1);
    EXPECT_NOT_NULL(mock.last_env_ptr);
    EXPECT_EQ_PTR(mock.last_env_ptr, reinterpret_cast<lsm_env*>(&mock.base));
    EXPECT_NOT_NULL(mock.last_allocated);
    EXPECT_EQ_PTR(mock.last_allocated, pNew);

    // Cleanup
    if (pNew) {
        std::free(pNew);
        pNew = nullptr;
    }
}

// Test 2: Failure path when xMutexNew reports an error
// - Verifies that lsmMutexNew propagates the error code
// - Verifies that when allocation is disabled, ppNew remains null
static void test_lsmMutexNew_failure() {
    // Arrange
    MockEnv mock;
    mock.return_code = -1;
    mock.allocate = false;
    mock.call_count = 0;
    mock.last_allocated = nullptr;
    mock.last_env_ptr = nullptr;

    lsm_mutex *pNew = reinterpret_cast<lsm_mutex*>(0xDEADBEEF); // sentinel to detect modification

    // Act
    int rc = lsmMutexNew(reinterpret_cast<lsm_env*>(&mock.base), &pNew);

    // Assert
    EXPECT_EQ_INT(rc, -1);
    EXPECT_TRUE(pNew == nullptr);
    EXPECT_TRUE(mock.call_count == 1);
    EXPECT_NOT_NULL(mock.last_env_ptr);
    EXPECT_EQ_PTR(mock.last_env_ptr, reinterpret_cast<lsm_env*>(&mock.base));
    EXPECT_EQ_PTR(mock.last_allocated, reinterpret_cast<lsm_mutex*>(nullptr));

    // Cleanup (nothing allocated in this path)
}

// Entry point to run tests
int main() {
    std::cout << "Running test_lsmMutexNew_success..." << std::endl;
    test_lsmMutexNew_success();
    std::cout << "Running test_lsmMutexNew_failure..." << std::endl;
    test_lsmMutexNew_failure();

    if (g_test_failures > 0) {
        std::cerr << "Unit tests completed with " << g_test_failures << " failure(s)." << std::endl;
        return 1;
    } else {
        std::cout << "All unit tests passed." << std::endl;
        return 0;
    }
}
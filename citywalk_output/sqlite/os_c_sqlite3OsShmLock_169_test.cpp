// This test suite targets the focal method:
// int sqlite3OsShmLock(sqlite3_file *id, int offset, int n, int flags) { 
//   return id->pMethods->xShmLock(id, offset, n, flags); 
// }

// Step 1 (Test Design Notes):
// - Candidate Keywords derived from the focal method's dependencies:
//   sqlite3_file, pMethods, xShmLock, sqlite3_io_methods, sqliteInt.h, os.c, ShmLock, offset, n, flags
// - We will mock the xShmLock callback via a test double registered by sqlite3_file's pMethods.
// - We assume the project exposes sqliteInt.h (as in the provided class dependency block) and sqlite3OsShmLock is linkable.
// - We implement tests in C++11 without GTest, using a tiny in-house assertion logger to maximize coverage while keeping tests executable.

#include <vector>
#include <sstream>
#include <cstring>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>


// Import C definitions from SQLite's internal headers (as used by os.c in the focal code).
extern "C" {
}

// Ensure the focal function is visible for linking (the project provides its definition in os.c).
extern "C" int sqlite3OsShmLock(sqlite3_file *id, int offset, int n, int flags);

// -----------------------------------------------------------------------------
// Test Harness (lightweight, non-terminating assertions)
// -----------------------------------------------------------------------------

static std::vector<std::string> gFailures;  // accumulate failures, do not abort on first failure

static void logFailure(const std::string &msg) {
    gFailures.push_back(msg);
    std::cerr << "Test failure: " << msg << std::endl;
}

// Helpers to stringify integer values
template <typename T>
static std::string to_string_t(const T &val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

// Lightweight assertion helpers (non-terminating)
#define ASSERT_EQ(a, b, msg) do { \
    auto _a = (a); auto _b = (b); \
    if (!(_a == _b)) { \
        logFailure((std::string("ASSERT_EQ failed: ") + (msg) + " | " #a " != " #b + " [actual: " + to_string_t(_a) + " vs " + to_string_t(_b) + "]")); \
    } \
} while (0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { logFailure(std::string("ASSERT_TRUE failed: ") + (msg)); } \
} while (0)

#define ASSERT_FALSE(cond, msg) do { \
    if (cond) { logFailure(std::string("ASSERT_FALSE failed: ") + (msg)); } \
} while (0)

// -----------------------------------------------------------------------------
// Test Infrastructure to mock xShmLock via sqlite3_io_methods
// -----------------------------------------------------------------------------

// Context attached to each sqlite3_file instance to verify that sqlite3OsShmLock
// correctly forwards parameters to xShmLock.
struct ShmLockContext {
    int lastOffset;
    int lastN;
    int lastFlags;
    bool called;
    int returnValue; // what the mocked xShmLock should return

    ShmLockContext() : lastOffset(0), lastN(0), lastFlags(0), called(false), returnValue(0) {}
};

// Global registry mapping a sqlite3_file* to its per-file test context
static std::unordered_map<sqlite3_file*, ShmLockContext*> gShmLockContextMap;

// Mock xShmLock implementation that sqlite3OsShmLock will dispatch to through the above pMethods
static int MockXShmLock(sqlite3_file *id, int offset, int n, int flags) {
    auto it = gShmLockContextMap.find(id);
    if (it == gShmLockContextMap.end()) {
        // If no context registered for this file, return a sentinel error to indicate misconfiguration
        return -1;
    }
    ShmLockContext *ctx = it->second;
    ctx->called = true;
    ctx->lastOffset = offset;
    ctx->lastN = n;
    ctx->lastFlags = flags;
    return ctx->returnValue;
}

// -----------------------------------------------------------------------------
// Individual tests
// -----------------------------------------------------------------------------

// Test 1: Basic propagation and parameter forwarding
// Verifies that sqlite3OsShmLock forwards all parameters to the underlying xShmLock
// and returns the exact value provided by the callback.
static void test_basic_propagation() {
    // Prepare a sqlite3_file with a vfs/methods structure exposing xShmLock.
    sqlite3_file file;
    std::memset(&file, 0, sizeof(file));

    // Prepare the io_methods structure with only xShmLock initialized.
    sqlite3_io_methods methods;
    std::memset(&methods, 0, sizeof(methods));
    methods.iVersion = 1;
    methods.xShmLock = MockXShmLock;
    // Attach methods to the file
    file.pMethods = &methods;

    // Create a context and register it for this file
    ShmLockContext *ctx = new ShmLockContext();
    ctx->returnValue = 0; // success
    gShmLockContextMap[&file] = ctx;

    // Perform the call
    int res = sqlite3OsShmLock(&file, /*offset*/ 5, /*n*/ 10, /*flags*/ 3);

    // Assertions
    ASSERT_EQ(res, 0, "sqlite3OsShmLock should return value from xShmLock (0)");
    ASSERT_TRUE(ctx->called, "xShmLock should have been called");
    ASSERT_EQ(ctx->lastOffset, 5, " forwarded offset should be 5");
    ASSERT_EQ(ctx->lastN, 10, " forwarded n should be 10");
    ASSERT_EQ(ctx->lastFlags, 3, " forwarded flags should be 3");

    // Cleanup
    gShmLockContextMap.erase(&file);
    delete ctx;
}

// Test 2: Propagate non-zero/negative return values from xShmLock
// Ensures that sqlite3OsShmLock propagates error codes correctly.
static void test_propagate_error_return() {
    sqlite3_file file;
    std::memset(&file, 0, sizeof(file));

    sqlite3_io_methods methods;
    std::memset(&methods, 0, sizeof(methods));
    methods.iVersion = 1;
    methods.xShmLock = MockXShmLock;
    file.pMethods = &methods;

    ShmLockContext *ctx = new ShmLockContext();
    ctx->returnValue = -7; // some error code
    gShmLockContextMap[&file] = ctx;

    int res = sqlite3OsShmLock(&file, 0, 0, 0);

    ASSERT_EQ(res, -7, "sqlite3OsShmLock should propagate xShmLock's negative return value");
    ASSERT_TRUE(ctx->called, "xShmLock should have been called");

    gShmLockContextMap.erase(&file);
    delete ctx;
}

// Test 3: Concurrent usage with multiple sqlite3_file instances
// Verifies that two independent files each with their own context are handled correctly.
static void test_multiple_files_isolated_contexts() {
    // File A
    sqlite3_file fileA;
    std::memset(&fileA, 0, sizeof(fileA));
    sqlite3_io_methods methodsA;
    std::memset(&methodsA, 0, sizeof(methodsA));
    methodsA.iVersion = 1;
    methodsA.xShmLock = MockXShmLock;
    fileA.pMethods = &methodsA;
    ShmLockContext *ctxA = new ShmLockContext();
    ctxA->returnValue = 1;
    gShmLockContextMap[&fileA] = ctxA;

    // File B
    sqlite3_file fileB;
    std::memset(&fileB, 0, sizeof(fileB));
    sqlite3_io_methods methodsB;
    std::memset(&methodsB, 0, sizeof(methodsB));
    methodsB.iVersion = 1;
    methodsB.xShmLock = MockXShmLock;
    fileB.pMethods = &methodsB;
    ShmLockContext *ctxB = new ShmLockContext();
    ctxB->returnValue = 2;
    gShmLockContextMap[&fileB] = ctxB;

    // Call for A
    int resA = sqlite3OsShmLock(&fileA, 1, 2, 3);
    // Call for B
    int resB = sqlite3OsShmLock(&fileB, 4, 5, 6);

    ASSERT_EQ(resA, ctxA->returnValue, "File A should forward its own return value");
    ASSERT_EQ(resB, ctxB->returnValue, "File B should forward its own return value");

    ASSERT_TRUE(ctxA->called, "File A's xShmLock should have been called");
    ASSERT_TRUE(ctxB->called, "File B's xShmLock should have been called");

    ASSERT_EQ(ctxA->lastOffset, 1, "File A offset forwarded as 1");
    ASSERT_EQ(ctxA->lastN, 2, "File A n forwarded as 2");
    ASSERT_EQ(ctxA->lastFlags, 3, "File A flags forwarded as 3");

    ASSERT_EQ(ctxB->lastOffset, 4, "File B offset forwarded as 4");
    ASSERT_EQ(ctxB->lastN, 5, "File B n forwarded as 5");
    ASSERT_EQ(ctxB->lastFlags, 6, "File B flags forwarded as 6");

    // Cleanup
    gShmLockContextMap.erase(&fileA);
    gShmLockContextMap.erase(&fileB);
    delete ctxA;
    delete ctxB;
}

// Test 4: Edge case - zero-length lock and zeroed parameters
static void test_zero_length_and_params() {
    sqlite3_file file;
    std::memset(&file, 0, sizeof(file));

    sqlite3_io_methods methods;
    std::memset(&methods, 0, sizeof(methods));
    methods.iVersion = 1;
    methods.xShmLock = MockXShmLock;
    file.pMethods = &methods;

    ShmLockContext *ctx = new ShmLockContext();
    ctx->returnValue = 123; // arbitrary success code
    gShmLockContextMap[&file] = ctx;

    int res = sqlite3OsShmLock(&file, 0, 0, 0);

    ASSERT_EQ(res, 123, "Zero-length parameters should still forward to xShmLock and return its value");
    ASSERT_TRUE(ctx->called, "xShmLock should have been called for zero-length lock");
    ASSERT_EQ(ctx->lastOffset, 0, "Offset forwarded as 0");
    ASSERT_EQ(ctx->lastN, 0, "N forwarded as 0");
    ASSERT_EQ(ctx->lastFlags, 0, "Flags forwarded as 0");

    gShmLockContextMap.erase(&file);
    delete ctx;
}

// -----------------------------------------------------------------------------
// Test runner
// -----------------------------------------------------------------------------

static void run_all_tests() {
    test_basic_propagation();
    test_propagate_error_return();
    test_multiple_files_isolated_contexts();
    test_zero_length_and_params();
}

// -----------------------------------------------------------------------------
// Main entry
// -----------------------------------------------------------------------------

int main() {
    run_all_tests();

    if (gFailures.empty()) {
        std::cout << "All tests PASSED. SQLite3OsShmLock forwards parameters correctly and propagates return values as expected." << std::endl;
        return 0;
    } else {
        std::cout << gFailures.size() << " test(s) FAILED." << std::endl;
        for (const auto &f : gFailures) {
            std::cout << "  - " << f << std::endl;
        }
        return 1;
    }
}
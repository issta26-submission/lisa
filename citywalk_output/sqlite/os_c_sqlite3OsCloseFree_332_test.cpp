// Lightweight C++11 test harness for sqlite3OsCloseFree
// This test suite targets the focal method:
// void sqlite3OsCloseFree(sqlite3_file *pFile){
//
//   assert( pFile );
//   sqlite3OsClose(pFile);
//   sqlite3_free(pFile);
// }
//
// The test uses weakly-defined behavior overrides for sqlite3OsClose and sqlite3_free
// so that we can observe that sqlite3OsCloseFree calls into its dependencies with the
// same pointer value. The actual sqlite3OsCloseFree implementation is expected to be
// provided by the project (os.c). We compile this test with that translation unit available.

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>


// Forward declaration of the function under test (to be provided by os.c during linking)
typedef struct sqlite3_file sqlite3_file;
extern "C" void sqlite3OsCloseFree(sqlite3_file *pFile);

// Mocks to observe interactions inside sqlite3OsCloseFree.
// We override the dependency functions to record the pointer passed.

// Global observers
static sqlite3_file* g_last_closed_ptr = nullptr;
static void*         g_last_freed_ptr  = nullptr;

// Override of sqlite3OsClose to observe its argument.
// This function will intercept the call from sqlite3OsCloseFree.
extern "C" void sqlite3OsClose(sqlite3_file *pId) {
    g_last_closed_ptr = pId;
}

// Override of sqlite3_free to observe and actually free the memory to avoid leaks.
extern "C" void sqlite3_free(void *p) {
    g_last_freed_ptr = p;
    // Use the real free to avoid leaking memory in tests
    std::free(p);
}

// Basic test: ensure that sqlite3OsCloseFree calls sqlite3OsClose and sqlite3_free
// with the exact same pointer that was provided to sqlite3OsCloseFree.
static int test_sqlite3OsCloseFree_basic() {
    // Reset observers
    g_last_closed_ptr = nullptr;
    g_last_freed_ptr  = nullptr;

    // Allocate a small dummy sqlite3_file object. sqlite3_file is an incomplete type here,
    // so we allocate raw memory and reinterpret_cast to the type.
    sqlite3_file *p = reinterpret_cast<sqlite3_file*>(std::malloc(1));
    if (p == nullptr) {
        std::fprintf(stderr, "Test setup failed: malloc returned NULL\n");
        return 1;
    }

    // Call the focal function (implementation provided by os.c in the actual project)
    sqlite3OsCloseFree(p);

    // Validate that sqlite3OsClose was invoked with the exact same pointer
    if (g_last_closed_ptr != p) {
        std::fprintf(stderr, "FAIL: sqlite3OsClose was not called with the original pointer\n");
        return 1;
    }

    // Validate that sqlite3_free was invoked with the exact same pointer
    if (g_last_freed_ptr != p) {
        std::fprintf(stderr, "FAIL: sqlite3_free was not called with the original pointer\n");
        return 1;
    }

    // If we reached here, the test passes.
    return 0;
}

// Entry point: run tests and report results.
// Note: The environment should link this test with the actual os.c (or equivalent)
// so that sqlite3OsCloseFree is resolved.
int main() {
    int failures = 0;

    if (test_sqlite3OsCloseFree_basic() != 0) {
        std::printf("Test sqlite3OsCloseFree_basic: FAILED\n");
        ++failures;
    } else {
        std::printf("Test sqlite3OsCloseFree_basic: PASSED\n");
    }

    // Return non-zero if any test failed to integrate with CI expectations.
    return failures ? 1 : 0;
}
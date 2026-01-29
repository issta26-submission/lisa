// C++11 test harness for sqlite3VListAdd (Step 2 & Step 3 accordance with the provided <FOCAL_METHOD>).
// This test suite assumes the focal function is linked from the project's C/C++ sources
// and uses minimal external dependencies (no GTest). It uses a lightweight, non-terminating
// assertion style and prints a summary at the end.

#include <math.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// The focal method sqlite3VListAdd is defined in the project under test.
// We declare the minimal external interface required for linking and calling the function.
// We assume VList is compatible with an int-based array layout used by sqlite3VListAdd.

extern "C" {
    // Forward declaration of an opaque sqlite3 struct used by the function.
    struct sqlite3;
    // The VList type is treated as an alias to an integer element array for the test harness.
    typedef int VList;
    // Prototype of the focal function under test.
    VList *sqlite3VListAdd(struct sqlite3 *db,
                          VList *pIn,
                          const char *zName,
                          int nName,
                          int iVal);

    // Memory allocator hook used by sqlite3VListAdd to (re)allocate memory.
    // The real project should provide this; we declare it here for linkage.
    void *sqlite3DbRealloc(struct sqlite3 *db, void *pOld, long long nBytes);
}

// Lightweight non-terminating test harness
static int g_total = 0;
static int g_passed = 0;

// Helper: non-terminating assertion with message
#define ASSERT_TRUE(cond, msg) do { \
    g_total++; \
    if (cond) { ++g_passed; } else { \
        std::cerr << "ASSERT_TRUE FAILED: " << (msg) << "  [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    g_total++; \
    if ((a) == (b)) { ++g_passed; } else { \
        std::cerr << "ASSERT_EQ FAILED: " << (msg) << "  Expected " << (b) << " got " << (a) \
                  << "  [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    } \
} while(0)

#define ASSERT_NEQ(a, b, msg) do { \
    g_total++; \
    if ((a) != (b)) { ++g_passed; } else { \
        std::cerr << "ASSERT_NEQ FAILED: " << (msg) << "  Both are " << (a) \
                  << "  [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    } \
} while(0)

#define ASSERT_STRNCMP(p, q, n, msg) do { \
    g_total++; \
    if (std::strncmp((p), (q), (n)) == 0) { ++g_passed; } else { \
        std::cerr << "ASSERT_STRNCMP FAILED: " << (msg) << "  got '"
                  << std::string(p, (n)) << "' vs expected '" << std::string(q, (n)) \
                  << "'  [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    } \
} while(0)

// Test 1: pIn != 0 and no reallocation should occur (pIn[1] + nInt <= pIn[0])
bool test_VListAdd_NoRealloc() {
    // Prepare a pre-sized VList-like buffer with enough capacity
    // Layout: [0] = capacity, [1] = next index, [2..] data...
    const int nName = 4;      // "NAME"
    const int nInt  = nName/4 + 3; // 4
    const int initialCap = 10;      // >= 3 and >= (pIn[1] + nInt) => 2 + 4 = 6 <= 10
    int *buf = (int*)std::malloc(initialCap * sizeof(int));
    if(!buf) return false;

    buf[0] = initialCap; // capacity
    buf[1] = 2;          // next index (i)
    // buf[2], buf[3] will be overwritten by the function as per contract
    // The rest is uninitialized.

    // Call: db = nullptr, pIn non-null, zName with nName, iVal
    VList *pIn = (VList*)buf;
    const char *zName = "NAME";

    VList *res = sqlite3VListAdd(nullptr, pIn, zName, nName, 7);

    // Validate
    bool ok = true;
    ASSERT_TRUE(res == pIn, "NoRealloc: Returned pointer should be original buffer when no realloc occurs");
    // After call:
    // nInt = 4; i = 2; pIn[2] = iVal; pIn[3] = nInt; pIn[1] = i+nInt = 6
    ASSERT_EQ(buf[0], initialCap, "NoRealloc: capacity should remain unchanged");
    ASSERT_EQ(buf[1], 6, "NoRealloc: updated next index should be i+nInt (6)");
    ASSERT_EQ(buf[2], 7, "NoRealloc: first value entry (pIn[i] == iVal) should be at index 2");
    ASSERT_EQ(buf[3], nInt, "NoRealloc: nInt should be stored at index 3");
    // zName should be copied after i+2 = 4
    const char *zCopy = (const char*)&buf[4];
    ASSERT_STRNCMP(zCopy, zName, nName, "NoRealloc: zName should be copied after numeric fields");
    // Ensure null terminator at zName end
    ASSERT_EQ(zCopy[nName], '\0', "NoRealloc: zName should be zero-terminated");

    std::free(buf);
    return ok;
}

// Test 2: pIn != 0 but there is not enough space; reallocation should occur
bool test_VListAdd_WithRealloc() {
    const int nName = 4; // "NAME"
    const int nInt  = nName/4 + 3; // 4
    // Create a small buffer to force reallocation: capacity 5, next idx 2
    int *buf = (int*)std::malloc(5 * sizeof(int));
    if(!buf) return false;
    buf[0] = 5;   // capacity
    buf[1] = 2;   // next index (i)
    // buf[2]..buf[3] undefined

    VList *pIn = (VList*)buf;
    const char *zName = "NAME";

    // Call: this should trigger reallocation
    VList *res = sqlite3VListAdd(nullptr, pIn, zName, nName, 99);

    // Validate
    bool ok = true;
    ASSERT_NEQ(res, pIn, "WithRealloc: Returned pointer should be different after realloc");
    int *a = (int*)res;

    // Expect new capacity nAlloc = 2*(old cap) + nInt = 2*5 + 4 = 14
    ASSERT_EQ(a[0], 14, "WithRealloc: new capacity (a[0]) should be 14");
    // i remains pIn[1] (which was 2)
    ASSERT_EQ(a[2], 99, "WithRealloc: a[2] should hold iVal (99)");
    ASSERT_EQ(a[3], nInt, "WithRealloc: a[3] should hold nInt (4)");
    // pIn[1] updated to i+nInt = 6
    ASSERT_EQ(a[1], 6, "WithRealloc: updated next index should be i+nInt (6)");
    // zName copied after index 4
    const char *zCopy = (const char*)&a[4];
    ASSERT_STRNCMP(zCopy, zName, nName, "WithRealloc: zName should be copied after numeric fields");
    ASSERT_EQ(zCopy[nName], '\0', "WithRealloc: zName should be zero-terminated");

    std::free(res);
    return ok;
}

// Test 3: pIn == 0 path (initial allocation)
bool test_VListAdd_InputNull() {
    const int nName = 3; // "ABC" (non-null-terminated length considered 3)
    const int nInt = nName/4 + 3; // 0 + 3 = 3

    VList *res = sqlite3VListAdd(nullptr, nullptr, "ABC", nName, 5);
    bool ok = true;
    // We expect a new allocation; res should not be null
    ASSERT_TRUE(res != nullptr, "InputNull: Result should be non-null when starting from NULL pIn");
    int *a = (int*)res;

    // a[0] = nAlloc = 10 + nInt = 13
    ASSERT_EQ(a[0], 13, "InputNull: new capacity should be 13");
    // a[1] = i+nInt, with i starting at 2 -> 2+3 = 5
    ASSERT_EQ(a[1], 5, "InputNull: next index after operation should be 5");
    // a[2] = iVal
    ASSERT_EQ(a[2], 5, "InputNull: iVal should be written at a[2]");
    // a[3] = nInt
    ASSERT_EQ(a[3], nInt, "InputNull: nInt should be stored at a[3]");
    // zName copied after index i+2 = 4
    const char *zCopy = (const char*)&a[4];
    ASSERT_STRNCMP(zCopy, "ABC", nName, "InputNull: zName should be copied after numeric fields");
    ASSERT_EQ(zCopy[nName], '\0', "InputNull: zName should be zero-terminated");

    std::free(res);
    return ok;
}

int main() {
    std::cout << "Running sqlite3VListAdd unit tests (C++11, no GTest) ..." << std::endl;

    // Execute tests, each with explicit test-case explanations in code comments.
    bool t1 = test_VListAdd_NoRealloc();
    bool t2 = test_VListAdd_WithRealloc();
    bool t3 = test_VListAdd_InputNull();

    int total = g_total;
    int passed = g_passed;
    std::cout << "Tests run: " << total << ", Passed: " << passed << std::endl;
    if (passed == total) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << (total - passed) << " test(s) FAILED." << std::endl;
        return 1;
    }
}
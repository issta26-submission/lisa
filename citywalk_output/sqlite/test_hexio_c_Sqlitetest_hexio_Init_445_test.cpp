/*
  Unit test suite for Sqlitetest_hexio_Init in test_hexio.c
  - Uses a minimal C-style mock of the Tcl command registration API to verify
    that the Init function registers the expected commands.
  - No GTest; a small in-house test harness is used. Tests are run from main().
  - Focuses on ensuring the registration of the 8 commands and basic behavior.
  - This file is written to be compiled with C++11 and linked against the project
    that provides Sqlitetest_hexio_Init and the Tcl-like interface.
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>


// Lightweight, test-oriented mock of the Tcl types and API used by Sqlitetest_hexio_Init.
// We provide just enough surface to compile and verify command registrations.

extern "C" {

// Forward declarations to match the actual signature used by the focal code.
// We assume the real environment provides these definitions; we provide lightweight
// stubs here so the test can link and run against the focal function.

typedef struct Tcl_Interp Tcl_Interp;
typedef struct Tcl_Obj Tcl_Obj;

// Function pointer type for Tcl command handlers.
// In real Tcl: typedef int Tcl_ObjCmdProc(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
// We approximate with a compatible signature for testing.
typedef int (*Tcl_ObjCmdProc)(void*, Tcl_Interp*, int, Tcl_Obj*[]);

// Mock of Tcl_CreateObjCommand used by Sqlitetest_hexio_Init.
// We simply record the registered command names and the provided handler pointers.
int Tcl_CreateObjCommand(Tcl_Interp *interp, const char *zName, Tcl_ObjCmdProc *xProc, void *clientData, void *deleteProc);

// The function under test is declared here for linking.
// The real implementation is in test_hexio.c as per the project's focal method.
int Sqlitetest_hexio_Init(Tcl_Interp *interp);

} // extern "C"

// Minimal, test-time global state to capture registrations performed by Sqlitetest_hexio_Init
static const char *g_registeredNames[128];
static int g_registeredCount = 0;
static Tcl_Interp *g_lastInterp = nullptr;

// A simple stub object to satisfy potential uses in the mock environment.
// Not strictly necessary, but helps readability if the test code references the type.
struct Tcl_Interp { int dummy; };

// Implementation of the mock Tcl_CreateObjCommand.
// Records the command name and stores the last interp pointer.
int Tcl_CreateObjCommand(Tcl_Interp *interp, const char *zName, Tcl_ObjCmdProc *xProc, void *clientData, void *deleteProc) {
    (void)clientData;
    (void)deleteProc;
    if (g_registeredCount < (int)(sizeof(g_registeredNames)/sizeof(g_registeredNames[0]))) {
        g_registeredNames[g_registeredCount++] = zName;
    }
    g_lastInterp = interp;
    // In a real Tcl, this would return a command token; we just indicate success.
    return 0;
}

// Simple lightweight assertion utilities (non-terminating).
static int g_test_failures = 0;

#define EXPECT_EQ(a,b) do { \
    if ((a) != (b)) { \
        ++g_test_failures; \
        fprintf(stderr, "EXPECT_EQ failed: %s == %s (actual: %lld, expected: %lld) at %s:%d\n", #a, #b, (long long)(a), (long long)(b), __FILE__, __LINE__); \
    } \
} while(0)

#define EXPECT_STR_EQ(a,b) do { \
    if ((a) == nullptr && (b) == nullptr) { /* both null, treat as equal */ } \
    else if ((a) == nullptr || (b) == nullptr || strcmp((a), (b)) != 0) { \
        ++g_test_failures; \
        fprintf(stderr, "EXPECT_STR_EQ failed: \"%s\" != \"%s\" at %s:%d\n", (a), (b), __FILE__, __LINE__); \
    } \
} while(0)

#define EXPECT_TRUE(x) do { \
    if (!(x)) { \
        ++g_test_failures; \
        fprintf(stderr, "EXPECT_TRUE failed: %s is false at %s:%d\n", #x, __FILE__, __LINE__); \
    } \
} while(0)

#define ASSERT_NOT_REACHED() do { \
    ++g_test_failures; \
    fprintf(stderr, "ASSERT_NOT_REACHED failed. Unexpected path taken at %s:%d\n", __FILE__, __LINE__); \
} while(0)

// Convenience to reset the mock state between tests.
static void reset_mock() {
    g_registeredCount = 0;
    g_lastInterp = nullptr;
    for (int i = 0; i < 128; ++i) g_registeredNames[i] = nullptr;
}

// First test: verify that Init registers exactly 8 commands with the expected names
static void test_InitRegistersEightCommands_once() {
    reset_mock();

    // Create a dummy interpreter pointer and call the Init function once.
    Tcl_Interp dummyInterp;
    int rc = Sqlitetest_hexio_Init(&dummyInterp);

    // Basic return value check (TCL_OK assumed to be 0 in the test environment)
    EXPECT_EQ(rc, 0);

    // Validate that exactly 8 commands were registered and in the expected order.
    const int kExpectedCount = 8;
    const char *expectedNames[kExpectedCount] = {
        "hexio_read",
        "hexio_write",
        "hexio_get_int",
        "hexio_render_int16",
        "hexio_render_int32",
        "utf8_to_utf8",
        "read_fts3varint",
        "make_fts3record"
    };

    EXPECT_EQ(g_registeredCount, kExpectedCount);
    for (int i = 0; i < kExpectedCount; ++i) {
        EXPECT_STR_EQ(g_registeredNames[i], expectedNames[i]);
    }

    // The interpreter pointer should have been passed through to the mock.
    EXPECT_TRUE(g_lastInterp == &dummyInterp);
}

// Second test: calling Init twice should register 16 commands total (repeat)
static void test_InitRegistersEightCommands_twice() {
    reset_mock();

    Tcl_Interp dummyInterp;
    int rc1 = Sqlitetest_hexio_Init(&dummyInterp);
    int rc2 = Sqlitetest_hexio_Init(&dummyInterp);

    EXPECT_EQ(rc1, 0);
    EXPECT_EQ(rc2, 0);
    EXPECT_EQ(g_registeredCount, 16);

    // Check that the first 8 names equal the expected sequence (repeat)
    const char *expectedNames[8] = {
        "hexio_read",
        "hexio_write",
        "hexio_get_int",
        "hexio_render_int16",
        "hexio_render_int32",
        "utf8_to_utf8",
        "read_fts3varint",
        "make_fts3record"
    };

    for (int i = 0; i < 8; ++i) {
        EXPECT_STR_EQ(g_registeredNames[i], expectedNames[i]);
        // Also ensure the second batch matches
        EXPECT_STR_EQ(g_registeredNames[i + 8], expectedNames[i]);
    }
}

// Third test: ensure behavior with a NULL interpreter does not crash and returns TCL_OK (0)
static void test_InitWithNullInterp() {
    reset_mock();

    int rc = Sqlitetest_hexio_Init(nullptr);

    EXPECT_EQ(rc, 0);
    // Still should have registered 8 commands even if interp is NULL
    EXPECT_EQ(g_registeredCount, 8);
}

// Runner for the test suite
static void run_all_tests() {
    test_InitRegistersEightCommands_once();
    test_InitRegistersEightCommands_twice();
    test_InitWithNullInterp();
}

// Entry point
int main() {
    run_all_tests();

    if (g_test_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s)\n", g_test_failures);
        return 1;
    }
}
// Unit test suite for sqlite3_value_blob (vdbeapi.c) adapted for a C++11 test harness
// - No GTest/GMock usage; tests are self-contained and executed via main()
// - Focuses on core logic branches in sqlite3_value_blob
// - Uses the project’s internal Mem type and related constants via included headers
// - Utilizes non-terminating assertions (custom macros) to maximize code coverage
// - Static internals are accessed through the provided headers; no private members are accessed directly

#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Import necessary internal dependencies to match the focal class/file context.
// These headers should define Mem, MEM_Blob, MEM_Str, MEM_Null, sqlite3_value_text, and SQLITE_OK.
// The test harness assumes a build environment where these headers are available.
extern "C" {
}

// Declaration of the focal function under test
// The actual implementation resides in vdbeapi.c; link against the project to provide this symbol.
extern "C" const void *sqlite3_value_blob(sqlite3_value *pVal);

// Lightweight non-terminating assertion macro for test continuity
static int g_test_failures = 0;
#define TEST_ASSERT(cond, message) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (message) << "  [Line: " << __LINE__ << "]\n"; \
        ++g_test_failures; \
    } \
} while(0)

// Helper to run all tests and report overall status
static void RunAllTests() {
    std::cout << "Starting sqlite3_value_blob unit tests...\n";

    // Test 1: Blob path taken with n > 0 should return the blob pointer (p->z)
    // and should set MEM_Blob flag on p (idempotent for the blob path).
    {
        Mem p;
        std::memset(&p, 0, sizeof(Mem));
        p.flags = MEM_Blob;                  // trigger blob path
        const char blobData[] = "blobdata";
        p.z = (char*)blobData;
        p.n = (int)sizeof(blobData) - 1;     // length excluding null terminator

        sqlite3_value *pVal = (sqlite3_value*)&p;

        const void *ret = sqlite3_value_blob(pVal);

        // Expectation: ret points to original blob data, and MEM_Blob flag is set
        TEST_ASSERT(ret == blobData, "Blob path: expected return to equal blob data pointer");
        TEST_ASSERT((p.flags & MEM_Blob) != 0, "Blob path: MEM_Blob flag should be set after call");
    }

    // Test 2: Blob path taken with n == 0 should return NULL (0)
    // Still should set MEM_Blob flag on p
    {
        Mem p;
        std::memset(&p, 0, sizeof(Mem));
        p.flags = MEM_Blob;                  // trigger blob path
        const char blobData[] = "zero_len_blob";
        p.z = (char*)blobData;
        p.n = 0;                               // zero length

        sqlite3_value *pVal = (sqlite3_value*)&p;

        const void *ret = sqlite3_value_blob(pVal);

        // Expectation: NULL/0 returned because n == 0, MEM_Blob is set
        TEST_ASSERT(ret == 0, "Blob path (n==0): expected return to be 0");
        TEST_ASSERT((p.flags & MEM_Blob) != 0, "Blob path (n==0): MEM_Blob flag should be set after call");
    }

    // Test 3: Non-blob path (no MEM_Blob or MEM_Str) should delegate to sqlite3_value_text
    // The returned pointer should match sqlite3_value_text(pVal)
    {
        Mem p;
        std::memset(&p, 0, sizeof(Mem));
        p.flags = 0;                           // not blob/str
        const char textData[] = "plain text";
        p.z = (char*)textData;
        p.n = (int)sizeof(textData) - 1;

        sqlite3_value *pVal = (sqlite3_value*)&p;

        const void *ret = sqlite3_value_blob(pVal);
        const void *expected = sqlite3_value_text(pVal);

        TEST_ASSERT(ret == expected, "Non-blob path: blob() return should equal text() result");
    }

    // Summary
    if(g_test_failures == 0) {
        std::cout << "All sqlite3_value_blob tests passed.\n";
    } else {
        std::cout << g_test_failures << " sqlite3_value_blob test(s) FAILED.\n";
    }
}

// Entry point
int main() {
    // Run all tests
    RunAllTests();

    // Return non-zero if any test failed
    return g_test_failures;
}
// High-quality C++11 unit test suite for sqlite3_wasm_db_serialize
// This test suite does not use GoogleTest. It provides a lightweight
// harness with non-fatal assertions to maximize coverage and execution.
// The test relies on providing a stub for sqlite3_serialize to control
// its return value, allowing us to validate the behavior of
// sqlite3_wasm_db_serialize under various scenarios.

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Include SQLite header for types and constants

// Forward-declare the sqlite3_serialize stub to shadow the real function
// The real implementation would be provided by SQLite; for tests, we
// override it with a controllable stub.
extern "C" unsigned char* sqlite3_serialize(sqlite3* pDb, const char* zSchema,
                                           sqlite3_int64* pOut, unsigned int mFlags);

// Global controls for the sqlite3_serialize stub
static int g_stub_return_null = 0;        // If non-zero, stub returns NULL
static const char* g_last_schema = nullptr; // Last zSchema observed by the stub

// The sqlite3_serialize stub used by tests. It mimics a simplified serialization
// behavior sufficient for testing sqlite3_wasm_db_serialize.
extern "C" unsigned char* sqlite3_serialize(sqlite3* pDb, const char* zSchema,
                                           sqlite3_int64* pOut, unsigned int /*mFlags*/) {
    // Record the schema argument for verification
    g_last_schema = zSchema;

    // If test asks for NULL, return NULL
    if (g_stub_return_null) {
        if (pOut) *pOut = 0;
        return nullptr;
    }

    // Non-NULL result: populate pOut and return a small buffer
    if (pOut) *pOut = 12345;
    unsigned char* p = (unsigned char*)malloc(4);
    if (p) {
        p[0] = 0xAA;
        p[1] = 0xBB;
        p[2] = 0xCC;
        p[3] = 0xDD;
    }
    return p;
}

// Simple test harness helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT(cond, msg)                                        \
    do {                                                              \
        ++g_total_tests;                                             \
        if (!(cond)) {                                               \
            std::cerr << "TEST FAIL: " << msg << " (" << __FILE__      \
                      << ":" << __LINE__ << ")" << std::endl;       \
            ++g_failed_tests;                                        \
        } else {                                                       \
            std::cout << "TEST PASS: " << msg << std::endl;          \
        }                                                              \
    } while (0)

static void reset_stub() {
    g_stub_return_null = 0;
    g_last_schema = nullptr;
}

// Test 1: Valid path - zSchema provided, pOut non-null, stub returns non-null
static void test_serialize_nonnull_result_sets_out_and_nOut() {
    reset_stub();
    g_stub_return_null = 0; // Ensure stub returns non-null
    void* dummyDb = reinterpret_cast<void*>(0x1); // arbitrary non-null pointer
    unsigned char* out = nullptr;
    sqlite3_int64 nOut = -1;

    int rc = sqlite3_wasm_db_serialize((sqlite3*)dummyDb, "custom_schema",
                                       &out, &nOut, 0);

    TEST_ASSERT(rc == 0, "rc should be SQLITE_OK (0) when serialization returns non-null");
    TEST_ASSERT(out != nullptr, "pOut should be set to non-null pointer when serialization returns data");
    TEST_ASSERT(nOut == 0, "nOut should be initialized to 0 by function when provided");

    // Verify that the stub observed the correct schema
    TEST_ASSERT(g_last_schema && std::strcmp(g_last_schema, "custom_schema") == 0,
                "sqlite3_serialize should be invoked with provided zSchema");

    // Cleanup
    if (out) free(out);
    reset_stub();
}

// Test 2: zSchema NULL but SQLITE_SERIALIZE_NOCOPY flag set; should still succeed
static void test_serialize_null_with_nocopy_flag_should_succeed_and_set_null_out() {
    reset_stub();
    g_stub_return_null = 1; // Stub will return NULL
    void* dummyDb = reinterpret_cast<void*>(0x2);
    unsigned char* out = (unsigned char*)0x1; // non-null initial value (should be overwritten)
    sqlite3_int64 nOut = 0;

    const unsigned int nocopyFlag = SQLITE_SERIALIZE_NOCOPY;
    int rc = sqlite3_wasm_db_serialize((sqlite3*)dummyDb, NULL,
                                       &out, &nOut, nocopyFlag);

    TEST_ASSERT(rc == 0, "rc should be SQLITE_OK (0) when SQLITE_SERIALIZE_NOCOPY flag is set even if zSerialize returns NULL");
    TEST_ASSERT(out == nullptr, "pOut should be set to NULL when stub returns NULL and NOCOPY flag is set");
}

// Test 3: zSchema provided, stub returns NULL and NOCOPY not set -> should return SQLITE_NOMEM
static void test_serialize_null_without_nocopy_should_return_nOMEM() {
    reset_stub();
    g_stub_return_null = 1; // Stub will return NULL
    void* dummyDb = reinterpret_cast<void*>(0x3);
    unsigned char* out = nullptr;
    sqlite3_int64 nOut = 0;

    int rc = sqlite3_wasm_db_serialize((sqlite3*)dummyDb, "main_schema",
                                       &out, &nOut, 0); // no NOCOPY
    TEST_ASSERT(rc == SQLITE_NOMEM, "rc should be SQLITE_NOMEM when serialization returns NULL and no NOCOPY flag");
}

// Test 4: Invalid arguments lead to SQLITE_MISUSE
static void test_serialize_invalid_arguments_should_return_misuse() {
    reset_stub();
    unsigned char* out = nullptr;
    sqlite3_int64 nOut = 0;

    // Case: pDb is NULL
    int rc1 = sqlite3_wasm_db_serialize(nullptr, "main", &out, &nOut, 0);
    TEST_ASSERT(rc1 == SQLITE_MISUSE, "rc should be SQLITE_MISUSE when pDb is NULL");

    // Case: pOut is NULL
    int rc2 = sqlite3_wasm_db_serialize((sqlite3*)0x10, "main", nullptr, &nOut, 0);
    TEST_ASSERT(rc2 == SQLITE_MISUSE, "rc should be SQLITE_MISUSE when pOut is NULL");
}

// Test 5: Ensure zSchema value is forwarded to the underlying serializer
static void test_serialize_forwards_schema_argument() {
    reset_stub();
    g_stub_return_null = 0; // Stub returns non-null
    void* dummyDb = reinterpret_cast<void*>(0x4);
    unsigned char* out = nullptr;
    sqlite3_int64 nOut = 0;

    int rc = sqlite3_wasm_db_serialize((sqlite3*)dummyDb, "forward_schema",
                                       &out, &nOut, 0);

    TEST_ASSERT(rc == 0, "rc should be SQLITE_OK when serialization returns non-null");
    // The stub should have recorded the schema string
    TEST_ASSERT(g_last_schema != nullptr &&
                std::strcmp(g_last_schema, "forward_schema") == 0,
                "sqlite3_serialize should be invoked with the exact zSchema provided");

    if (out) free(out);
}

// Utility to run all tests
static void run_all_tests() {
    test_serialize_nonnull_result_sets_out_and_nOut();
    test_serialize_null_with_nocopy_flag_should_succeed_and_set_null_out();
    test_serialize_null_without_nocopy_should_return_nOMEM();
    test_serialize_invalid_arguments_should_return_misuse();
    test_serialize_forwards_schema_argument();
}

// Entry point
int main() {
    // Informative header
    std::cout << "Starting tests for sqlite3_wasm_db_serialize..." << std::endl;

    run_all_tests();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}
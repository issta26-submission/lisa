// This test harness provides a minimal C/C++ compatibility layer to exercise the
// focal function: sqlite3_result_error16 as defined in the prompt.
// The goal is to validate the function's core behavior without requiring the full
// SQLite/Vdbe build environment. We provide lightweight stubs for the involved types
// and functions, then run a small, self-contained test suite (no Google Test).

#include <vector>
#include <cstdio>
#include <opcodes.h>
#include <sstream>
#include <cstddef>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain knowledge hints: create a tiny, self-contained environment to exercise
// the function's logic paths, focusing on the true branches and ensuring non-crashing
// behavior when API_ARMOR is enabled.

//////////////////////////////////////////////////////////////
// Lightweight, self-contained definitions to mock the API //
//////////////////////////////////////////////////////////////

// Enable API_ARMOR path (to exercise the #ifdef SQLITE_ENABLE_API_ARMOR branch)
#define SQLITE_ENABLE_API_ARMOR 1

// Minimal macro placeholders to resemble sqlite3 constants used in the focal method.
#define SQLITE_UTF16NATIVE 0
#define SQLITE_TRANSIENT 0
#define SQLITE_ERROR 1

// Forward-declare lightweight "sqlite3_context" related structures used by the focal method.
struct sqlite3_mutex {
    bool held; // whether the mutex is considered "held" by the current thread
};

struct sqlite3_db {
    sqlite3_mutex mutex;
};

struct sqlite3_out_dummy {
    // This object mimics the "Mem" used by the real API in a very small subset:
    // It holds a reference to the database and the last string data written.
    sqlite3_db *db;
    const void *z;
    int n;
};

// The context object passed to sqlite3_result_error16 in the focal code.
// We only implement the fields that are accessed by the function under test.
struct sqlite3_context {
    sqlite3_out_dummy *pOut;
    int isError;
};

// Minimal helper to "hold" a mutex (as per the real API assertion).
static int sqlite3_mutex_held(sqlite3_mutex *mutex) {
    return mutex != nullptr && mutex->held;
}

// Minimal representation of the internal helper used by sqlite3_result_error16:
static void sqlite3VdbeMemSetStr(sqlite3_out_dummy *pOut, const void *z, int n,
                                 int enc, int nDel) {
    // In the real API, this would allocate and/or fill a Mem object.
    // For testing, we simply store the pointer and length so tests can verify the write.
    if (pOut) {
        pOut->z = z;
        pOut->n = n;
    }
}

// The focal method under test, replicated here in C++ for the test harness.
// We replicate the exact structure and logic described in the prompt.
extern "C" void sqlite3_result_error16(sqlite3_context *pCtx, const void *z, int n) {
#if defined(SQLITE_ENABLE_API_ARMOR)
  if( pCtx==0 ) return;
#endif
  // In the real SQLite, this would assert that the current thread holds the mutex.
  // Our test harness provides a compatible assertion via sqlite3_mutex_held.
  assert( sqlite3_mutex_held(pCtx->pOut->db->mutex) );
  pCtx->isError = SQLITE_ERROR;
  sqlite3VdbeMemSetStr(pCtx->pOut, z, n, SQLITE_UTF16NATIVE, SQLITE_TRANSIENT);
}

// Lightweight assert handling (we include <cassert> for the assertion used in the focal method)

///////////////////////////////
// Lightweight test harness  //
///////////////////////////////

static std::vector<std::string> g_failures;
static int g_test_count = 0;
static int g_pass_count = 0;

static void log_failure(const std::string &msg) {
    g_failures.push_back(msg);
}

template<typename T>
static std::string to_string_val(const T &v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

// Special handling for pointer types to show addresses
static std::string to_string_ptr(const void *p) {
    std::ostringstream oss;
    oss << p;
    return oss.str();
}

// Expect equality for integral types
template<typename A, typename B>
static void expect_eq_int(const A &a, const B &b, const std::string &note) {
    if (!(a == b)) {
        std::ostringstream oss;
        oss << note
            << " | expected: " << to_string_val(b)
            << " | actual: " << to_string_val(a);
        log_failure(oss.str());
    } else {
        g_pass_count++;
    }
    g_test_count++;
}

// Expect equality for pointers
static void expect_eq_ptr(const void *a, const void *b, const std::string &note) {
    if (a != b) {
        std::ostringstream oss;
        oss << note
            << " | expected ptr: " << to_string_ptr(b)
            << " | actual ptr: " << to_string_ptr(a);
        log_failure(oss.str());
    } else {
        g_pass_count++;
    }
    g_test_count++;
}

// A helper to report overall result
static void report_results() {
    std::cout << "Tests run: " << g_test_count << ", Passed: " << g_pass_count << "\n";
    if (!g_failures.empty()) {
        std::cout << "Failures:\n";
        for (size_t i = 0; i < g_failures.size(); ++i) {
            std::cout << "  " << i+1 << ". " << g_failures[i] << "\n";
        }
    } else {
        std::cout << "All tests passed.\n";
    }
}

// Test Case 1: API Armor enabled path with null context should return without crashing.
// This covers the true-branch of the pCtx == 0 check.
static void test_case_api_armor_null_ctx() {
    // When API armor is enabled, passing a null context should cause an early return.
    // The absence of a crash is the intended behavior.
    sqlite3_result_error16(nullptr, (void*)"dummy", 4);
    // If we reached here, the early return path did not crash.
}

// Test Case 2: Normal operation path where mutex is held and we write a string.
// This verifies that isError is set and the memory is populated with the input data.
static void test_case_normal_operation() {
    // Set up a minimal environment that the focal function expects.
    sqlite3_mutex mutex;
    mutex.held = true;

    sqlite3_db db;
    db.mutex = mutex;

    sqlite3_out_dummy out;
    out.db = &db;
    out.z = nullptr;
    out.n = 0;

    sqlite3_context ctx;
    ctx.pOut = &out;
    ctx.isError = 0;

    const char *str = "hello_world";
    int n = 11; // length of "hello_world"

    // Call the focal function
    sqlite3_result_error16(&ctx, (const void*)str, n);

    // Validate results: isError should be SQLITE_ERROR, and the string should be written.
    expect_eq_int(ctx.isError, SQLITE_ERROR, "ctx.isError should be SQLITE_ERROR after sqlite3_result_error16");
    expect_eq_ptr(ctx.pOut->z, str, "pCtxOut.z should point to the input string");
    expect_eq_int(ctx.pOut->n, n, "pCtxOut.n should equal the input length n");
}

// Test Case 3: Zero-length input should not crash and should set n to 0 in the storage.
// This checks handling of zero-length data.
static void test_case_zero_length_input() {
    sqlite3_mutex mutex;
    mutex.held = true;

    sqlite3_db db;
    db.mutex = mutex;

    sqlite3_out_dummy out;
    out.db = &db;
    out.z = nullptr;
    out.n = 0;

    sqlite3_context ctx;
    ctx.pOut = &out;
    ctx.isError = 0;

    const void *z = (const void*)"abc"; // some data
    int n = 0;                          // zero-length write

    sqlite3_result_error16(&ctx, z, n);

    expect_eq_int(ctx.isError, SQLITE_ERROR, "ctx.isError should be SQLITE_ERROR for zero-length input");
    expect_eq_ptr(ctx.pOut->z, z, "pCtxOut.z should still reflect the input pointer even for zero length");
    expect_eq_int(ctx.pOut->n, n, "pCtxOut.n should be 0 for zero-length input");
}

// Entry point to run all tests
int main() {
    // Initialize test cases
    test_case_api_armor_null_ctx();
    test_case_normal_operation();
    test_case_zero_length_input();

    // Report results
    report_results();
    return g_failures.empty() ? 0 : 1;
}
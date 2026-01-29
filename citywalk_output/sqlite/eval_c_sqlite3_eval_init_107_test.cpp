// High-quality C++11 unit test suite for sqlite3_eval_init (no GTest, no GMock)

// This test suite uses a lightweight, self-contained mocking strategy to validate
// the behavior of sqlite3_eval_init as implemented in eval.c. It mocks the
// underlying sqlite3_create_function API to verify calls, order, and return value
// handling in both success and failure scenarios. The test is designed to compile
// in a C++11 environment and link against the provided C source (eval.c).

#include <iostream>
#include <sqlite3ext.h>
#include <string.h>
#include <cstring>


// Minimal, self-contained prototype and constants to align with the focal code.
// We rely on forward declarations for sqlite3 types since the real implementation
// (eval.c) expects these types via the included sqlite3ext.h header in the
// actual project. Here we only need pointer semantics for the test harness.

#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Forward declare the relevant SQLite-like types used by sqlite3_eval_init's signature.
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// Forward declare the function under test (implemented in eval.c)
extern "C" int sqlite3_eval_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// Forward declare the mocked API that eval.c will call.
// The real project links to the actual SQLite, but for unit testing we provide
// a controlled mock in this translation unit.
extern "C" int sqlite3_create_function(sqlite3*,
                                     const char*,
                                     int,
                                     int,
                                     void*,
                                     void (*)(sqlite3_context*, int, sqlite3_value**),
                                     void (*)(sqlite3_context*, int, sqlite3_value**),
                                     void (*)(sqlite3_context*));

// Internal state for the mock to control return values and capture registrations.
static int g_seq[20];
static int g_seq_len = 0;
static int g_seq_pos = 0;

static const char* g_regs[20];
static int g_regs_narg[20];
static int g_regs_count = 0;

// Helper: reset mock state before each test
static void mock_reset()
{
    g_seq_len = 0;
    g_seq_pos = 0;
    g_regs_count = 0;
    for (int i = 0; i < 20; ++i) {
        g_regs[i] = nullptr;
        g_regs_narg[i] = 0;
    }
}

// Helper: set the expected return sequence for sqlite3_create_function calls
static void mock_set_seq(const int seq[], int len)
{
    for (int i = 0; i < len && i < 20; ++i) {
        g_seq[i] = seq[i];
    }
    g_seq_len = (len > 20) ? 20 : len;
    g_seq_pos = 0;
}

// Helper: simple reporting
static int g_total = 0;
static int g_passed = 0;
static void test_ok(bool cond, const char* msg)
{
    g_total++;
    if (cond) {
        g_passed++;
        // Lightweight success message; keep runtime small.
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cerr << "[FAIL] " << msg << "\n";
    }
}

// Mock implementation of sqlite3_create_function
extern "C" int sqlite3_create_function(sqlite3* /*db*/,
                                    const char* zFunctionName,
                                    int nArg,
                                    int /*eTextRep*/,
                                    void* /*pApp*/,
                                    void (* /*xFunc*/)(sqlite3_context*, int, sqlite3_value**),
                                    void (* /*xStep*/)(sqlite3_context*, int, sqlite3_value**),
                                    void (* /*xFinal*/)(sqlite3_context*))
{
    // Record the registration name and arity for verification
    if (g_regs_count < 20) {
        g_regs[g_regs_count] = zFunctionName;
        g_regs_narg[g_regs_count] = nArg;
    }
    ++g_regs_count;

    // Return the next value from the predefined sequence, or SQLITE_OK by default
    if (g_seq_pos < g_seq_len) {
        return g_seq[g_seq_pos++];
    }
    return SQLITE_OK;
}

// Helper to call the function under test (db and pApi can be anything non-null/ignored)
static int run_eval_init_with_seq(const int seq[], int len)
{
    mock_reset();
    mock_set_seq(seq, len);
    // Call the function under test with a dummy db pointer and null error/message pointers
    int rc = sqlite3_eval_init(reinterpret_cast<sqlite3*>(0x1), nullptr, nullptr);
    return rc;
}

// Test 1: Both arities registered successfully (first 2 calls return SQLITE_OK)
static void test_both_ok()
{
    // Expect two registrations: eval(1) and eval(2) and an overall SQLITE_OK
    const int seq[] = { SQLITE_OK, SQLITE_OK };
    int rc = run_eval_init_with_seq(seq, 2);

    test_ok(rc == SQLITE_OK, "test_both_ok: rc should be SQLITE_OK on both successful registrations");
    test_ok(g_regs_count == 2, "test_both_ok: should have registered two functions");
    test_ok(g_regs[0] != nullptr && std::strcmp(g_regs[0], "eval") == 0, "test_both_ok: first registration name should be 'eval'");
    test_ok(g_regs_narg[0] == 1, "test_both_ok: first registration should have arity 1");
    test_ok(g_regs[1] != nullptr && std::strcmp(g_regs[1], "eval") == 0, "test_both_ok: second registration name should be 'eval'");
    test_ok(g_regs_narg[1] == 2, "test_both_ok: second registration should have arity 2");
}

// Test 2: Failure on first call should not attempt second registration
static void test_first_fail()
{
    // First call returns SQLITE_ERROR, so only one attempt occurred
    const int seq[] = { SQLITE_ERROR };
    int rc = run_eval_init_with_seq(seq, 1);

    test_ok(rc == SQLITE_ERROR, "test_first_fail: rc should propagate error from first sqlite3_create_function call");
    test_ok(g_regs_count == 1, "test_first_fail: should have attempted only one registration");
    test_ok(g_regs[0] != nullptr && std::strcmp(g_regs[0], "eval") == 0, "test_first_fail: first registration name should be 'eval'");
    test_ok(g_regs_narg[0] == 1, "test_first_fail: first registration should have arity 1");
}

// Test 3: First success, second failure should propagate error after second call
static void test_second_fail()
{
    // First call OK, second call returns SQLITE_ERROR
    const int seq[] = { SQLITE_OK, SQLITE_ERROR };
    int rc = run_eval_init_with_seq(seq, 2);

    test_ok(rc == SQLITE_ERROR, "test_second_fail: rc should be SQLITE_ERROR when second call fails");
    test_ok(g_regs_count == 2, "test_second_fail: should have attempted two registrations");
    test_ok(g_regs[0] != nullptr && std::strcmp(g_regs[0], "eval") == 0, "test_second_fail: first registration name should be 'eval'");
    test_ok(g_regs_narg[0] == 1, "test_second_fail: first registration should have arity 1");
    test_ok(g_regs[1] != nullptr && std::strcmp(g_regs[1], "eval") == 0, "test_second_fail: second registration name should be 'eval'");
    test_ok(g_regs_narg[1] == 2, "test_second_fail: second registration should have arity 2");
}

// Main: run all tests and report summary
int main()
{
    // Run each test, ensuring we isolate their mock state between tests
    std::cout << "Starting sqlite3_eval_init unit tests (no GTest)..." << std::endl;

    test_both_ok();
    // Reset for isolation
    mock_reset();

    test_first_fail();
    mock_reset();

    test_second_fail();
    mock_reset();

    int total = g_total;
    int passed = g_passed;
    int failed = total - passed;
    std::cout << "Test summary: total=" << total << ", passed=" << passed
              << ", failed=" << failed << std::endl;

    return (failed == 0) ? 0 : 1;
}
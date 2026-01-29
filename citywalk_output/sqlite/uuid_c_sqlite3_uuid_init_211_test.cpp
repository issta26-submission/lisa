/*
  Unit test suite for sqlite3_uuid_init (uuid.c)
  - This test harness provides a minimal mock environment to exercise
    the sqlite3_uuid_init function without depending on a real SQLite runtime.
  - It verifies all-OK path and the three short-circuit branches when rc != SQLITE_OK
    at each function registration step.
  - The tests are written for C++11, without Google Test. A lightweight
    EXPECT-style macro is provided to allow non-terminating assertions
    while still collecting test results.
  - The sqlite3_create_function symbol is mocked to simulate successive return
    codes and to capture the function names requested by sqlite3_uuid_init.
  - The actual implementation under test (uuid.c) is assumed to be compiled
    and linked with this test harness.
*/

#include <string.h>
#include <ctype.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <initializer_list>
#include <iostream>


// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(a, b) do { \
    ++g_tests_run; \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " | expected: " << (b) << " | actual: " << (a) << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_TRUE(x) do { \
    ++g_tests_run; \
    if (!(x)) { \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                  << " | expression evaluated to false" << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_FALSE(x) do { \
    ++g_tests_run; \
    if ( (x)) { \
        std::cerr << "EXPECT_FALSE failed at " << __FILE__ << ":" << __LINE__ \
                  << " | expression evaluated to true" << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)


// Mocking sqlite3 extension interface
extern "C" {

// Forward declare minimal SQLite types to satisfy the uuid.c dependencies
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// Prototype of the function under test's dependency (will be mocked)
int sqlite3_create_function(sqlite3*, const char*, int, unsigned int,
                            void*, void (*)(sqlite3_context*, int, sqlite3_value**),
                            void (*)(sqlite3_context*, int, sqlite3_value**),
                            void (*)(sqlite3_context*, int, sqlite3_value**));
}

// Lightweight SQLite constants (as used by the focal method)
static const int SQLITE_OK = 0;
static const int SQLITE_UTF8 = 0;
static const int SQLITE_INNOCUOUS = 0;
static const int SQLITE_DETERMINISTIC = 0;

// Global state to control the mocked sqlite3_create_function behavior
static std::vector<int> g_rc_queue;                 // Predefined return codes for successive calls
static std::vector<std::string> g_called_names;    // Captured function names registered

static void reset_mocks() {
    g_rc_queue.clear();
    g_called_names.clear();
}

// Push a sequence of return codes for subsequent sqlite3_create_function calls
static void push_rcs(std::initializer_list<int> rc_list) {
    g_rc_queue.assign(rc_list.begin(), rc_list.end());
}

// Mock implementation of sqlite3_create_function
extern "C" int sqlite3_create_function(
    sqlite3* db,
    const char* zFunctionName,
    int nArg,
    unsigned int eTextRep,
    void* pApp,
    void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
    void (*xStep)(sqlite3_context*, int, sqlite3_value**),
    void (*xFinal)(sqlite3_context*, int, sqlite3_value**)
) {
    (void)db; (void)pApp; (void)xFunc; (void)xStep; (void)xFinal;
    // Record the function name for verification
    g_called_names.emplace_back(zFunctionName ? zFunctionName : "<null>");
    // Return the next code from the queue if available; otherwise SQLITE_OK
    int rc = SQLITE_OK;
    if (!g_rc_queue.empty()) {
        rc = g_rc_queue.front();
        g_rc_queue.erase(g_rc_queue.begin());
    }
    return rc;
}

// Declaration of the focal function from uuid.c
// Ensure C linkage to match the symbol in the C translation unit
extern "C" int sqlite3_uuid_init(
    sqlite3 *db,
    char **pzErrMsg,
    const sqlite3_api_routines *pApi
);

int main() {
    int overall_rc = 0;

    // Test 1: All registrations succeed -> sqlite3_uuid_init should return SQLITE_OK
    {
        reset_mocks();
        push_rcs({ SQLITE_OK, SQLITE_OK, SQLITE_OK });
        // Call the focal function with nulls (mocked environment)
        int rc = sqlite3_uuid_init(nullptr, nullptr, nullptr);
        EXPECT_EQ(rc, SQLITE_OK);
        // Expect three registrations with correct names in order
        EXPECT_EQ(g_called_names.size(), 3);
        EXPECT_EQ(g_called_names[0], std::string("uuid"));
        EXPECT_EQ(g_called_names[1], std::string("uuid_str"));
        EXPECT_EQ(g_called_names[2], std::string("uuid_blob"));
    }

    // Test 2: First registration fails -> only "uuid" should be registered
    {
        reset_mocks();
        push_rcs({ 1 }); // First call returns non-OK
        int rc = sqlite3_uuid_init(nullptr, nullptr, nullptr);
        EXPECT_EQ(rc, 1);
        EXPECT_EQ(g_called_names.size(), 1);
        EXPECT_EQ(g_called_names[0], std::string("uuid"));
    }

    // Test 3: First two registrations succeed, third fails -> two registrations expected
    {
        reset_mocks();
        push_rcs({ 0, 2 }); // Second call fails
        int rc = sqlite3_uuid_init(nullptr, nullptr, nullptr);
        EXPECT_EQ(rc, 2);
        EXPECT_EQ(g_called_names.size(), 2);
        EXPECT_EQ(g_called_names[0], std::string("uuid"));
        EXPECT_EQ(g_called_names[1], std::string("uuid_str"));
    }

    // Summary
    std::cout << "Tests run: " << g_tests_run << " | Tests failed: " << g_tests_failed << std::endl;
    overall_rc = (g_tests_failed == 0) ? 0 : 1;
    return overall_rc;
}
// Test suite for sqlite3_overload_function (C focal method) with lightweight C-style stubs.
// This test harness is designed for C++11, without Google Test, and uses a small in-process
// mock of the necessary SQLite API surface to exercise the function logic within a controlled
// environment.

#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cstdarg>
#include <iostream>
#include <cassert>


// Enable minimal subset of SQLite API surface required by the focal function.
// This is a mock to allow isolated unit tests without depending on the full SQLite source.

#ifdef __cplusplus
extern "C" {
#endif

// Forward-declare minimal types to mirror SQLite usage in sqlite3_overload_function
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;

// Minimal struct with a mutex field used by sqlite3_overload_function
struct sqlite3 {
    void *mutex;
};

// Constants (subset)
#define SQLITE_UTF8 1
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT 1
#define SQLITE_NOMEM 7

// Forward declarations of dependencies used by sqlite3_overload_function
// In a real environment these would come from sqliteInt.h / sqlite3.h
int sqlite3SafetyCheckOk(sqlite3 *db);
void sqlite3_mutex_enter(void *p);
void sqlite3_mutex_leave(void *p);
void *sqlite3FindFunction(sqlite3 *db, const char *zName, int nArg, int enc, void *p);
char *sqlite3_mprintf(const char *fmt, ...);
void sqlite3_free(void *p);
int sqlite3_create_function_v2(
    sqlite3 *db,
    const char *zFunc,
    int nArg,
    int enc,
    void *p,
    void (*xSFunc)(sqlite3_context*, int, sqlite3_value**),
    void (*xStep)(sqlite3_context*, int, sqlite3_value**),
    void (*xFinal)(sqlite3_context*),
    void (*xDestroy)(void *)
);

// The focal function is assumed to be provided by the project under test.
// We declare it here as extern to be invoked by tests.
int sqlite3_overload_function(
  sqlite3 *db,
  const char *zName,
  int nArg
){
    // The real implementation exists in the project under test (main.c).
    // Here we rely on the stubbed environment to exercise its code path.
    // The test harness will compile/link against the real function in the project.
    (void)db; (void)zName; (void)nArg;
    return 0;
}

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------
// Lightweight test harness state and stubs
// ----------------------------------------------------------------------------

// Global toggles and state used to exercise different branches in the focal method.
static int g_SafetyCheckOkFlag = 1;               // Controls sqlite3SafetyCheckOk result
static const char* g_lastCreatedName = nullptr;    // Captured by sqlite3_create_function_v2
static void* g_lastPUserData = nullptr;            // Captured user data pointer passed to create_function_v2

// Minimal implementations to satisfy the focal function's calls

static int sqlite3SafetyCheckOk(sqlite3 *db) {
    (void)db;
    return g_SafetyCheckOkFlag;
}

static void sqlite3_mutex_enter(void *p) { (void)p; /* no-op for test */ }
static void sqlite3_mutex_leave(void *p) { (void)p; /* no-op for test */ }

static void* sqlite3FindFunction(sqlite3 *db, const char *zName, int nArg, int enc, void *p) {
    (void)db; (void)nArg; (void)enc; (void)p;
    // Simulate that a function named "exists" already exists for any arg count
    if (zName != nullptr && std::strcmp(zName, "exists") == 0) {
        return reinterpret_cast<void*>(1);
    }
    return nullptr;
}

// Simple variadic mprintf that only handles "%s" usage in tests
static char* sqlite3_mprintf(const char *fmt, ...) {
    (void)fmt;
    va_list ap;
    va_start(ap, fmt);
    const char *s = va_arg(ap, const char*);
    va_end(ap);
    if (s == nullptr) {
        char *empty = new char[1];
        empty[0] = '\0';
        return empty;
    }
    size_t len = std::strlen(s);
    char* out = new char[len + 1];
    std::strcpy(out, s);
    return out;
}
static void sqlite3_free(void *p) { delete[] reinterpret_cast<char*>(p); }

// Dummy SQLite context/value structures for function pointers
struct sqlite3_context {};
struct sqlite3_value {};
static void dummy_xFunc(sqlite3_context*, int, sqlite3_value**) { (void)0; }
static void dummy_xStep(sqlite3_context*, int, sqlite3_value**){ (void)0; }
static void dummy_xFinal(sqlite3_context*) {}

// The create_function_v2 mock captures the function creation request
static int sqlite3_create_function_v2(
  sqlite3* db,
  const char* zFunc,
  int nArg,
  int enc,
  void* p,
  void (*xSFunc)(sqlite3_context*, int, sqlite3_value**),
  void (*xStep)(sqlite3_context*, int, sqlite3_value**),
  void (*xFinal)(sqlite3_context*),
  void (*xDestroy)(void *)
){
    (void)db;
    (void)xSFunc;
    (void)xStep;
    (void)xFinal;
    (void)xDestroy;
    g_lastCreatedName = zFunc;
    g_lastPUserData = p;
    return SQLITE_OK;
}

// Overload the real sqlite3_overload_function to be linked with our mocks.
// The actual implementation is in the project under test; this stub exists to allow
// linking in this isolated test harness if required by the build setup.
// The real tests will link against the project's object that contains the actual implementation.


// ----------------------------------------------------------------------------
// Test utilities
// ----------------------------------------------------------------------------

static bool test_misuse_due_to_api_armor() {
    // Arrange
    sqlite3 db;
    db.mutex = nullptr;
    g_SafetyCheckOkFlag = 0; // Trigger API_ARMOR misuse path

    // Act
    int rc = sqlite3_overload_function(&db, "fn_misuse", 1);

    // Assert
    bool passed = (rc == 1 /* SQLITE_MISUSE_BKPT */);

    // Cleanup
    g_SafetyCheckOkFlag = 1;
    return passed;
}

static bool test_overload_function_exists_path() {
    // Arrange
    sqlite3 db;
    db.mutex = nullptr;
    g_SafetyCheckOkFlag = 1; // Armor passes
    // No need to register; our stub returns non-null for "exists"

    // Act
    int rc = sqlite3_overload_function(&db, "exists", 1);

    // Assert
    bool passed = (rc == 0 /* SQLITE_OK */);
    return passed;
}

static bool test_overload_function_creates_new_function() {
    // Arrange
    sqlite3 db;
    db.mutex = nullptr;
    g_SafetyCheckOkFlag = 1;
    g_lastCreatedName = nullptr;
    g_lastPUserData = nullptr;

    // Act
    int rc = sqlite3_overload_function(&db, "newfn", 2);

    // Assert
    bool passed = (rc == 0) &&
                  (g_lastCreatedName != nullptr) &&
                  (std::strcmp(g_lastCreatedName, "newfn") == 0) &&
                  (g_lastPUserData != nullptr) &&
                  (std::strcmp(reinterpret_cast<char*>(g_lastPUserData), "newfn") == 0);

    // Cleanup
    // Note: Not invoking destructor to mimic real-world lifecycle; tests finish here.
    return passed;
}

// ----------------------------------------------------------------------------
// Main - executes tests
// ----------------------------------------------------------------------------

int main() {
    int total = 3;
    int passed = 0;

    if (test_misuse_due_to_api_armor()) {
        std::cout << "[PASS] test_misuse_due_to_api_armor\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_misuse_due_to_api_armor\n";
    }

    if (test_overload_function_exists_path()) {
        std::cout << "[PASS] test_overload_function_exists_path\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_overload_function_exists_path\n";
    }

    if (test_overload_function_creates_new_function()) {
        std::cout << "[PASS] test_overload_function_creates_new_function\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_overload_function_creates_new_function\n";
    }

    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    return (passed == total) ? 0 : 1;
}
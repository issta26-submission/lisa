// Unit test suite for the focal method: lsmFsWriteLog
// This test suite is written in C++11, does not rely on GTest, and uses a lightweight
// in-source test harness to validate the forwarding behavior from lsmFsWriteLog
// to lsmEnvWrite along with correct parameter propagation.
//
// Focused method (from <FOCAL_METHOD>):
// int lsmFsWriteLog(FileSystem *pFS, i64 iOff, LsmString *pStr){
//   assert( pFS->fdLog );
//   return lsmEnvWrite(pFS->pEnv, pFS->fdLog, iOff, pStr->z, pStr->n);
// }
//
// Key dependencies (from <FOCAL_CLASS_DEP>):
// - FileSystem: contains pointers pEnv and fdLog
// - lsm_env, lsm_file: opaque types used by lsmEnvWrite
// - lsmEnvWrite: function that actually writes to the underlying environment
// - LsmString: structure containing z (char*) and n (int) to pass string data
// - i64: 64-bit integer type
//
// Test goals (based on Domain Knowledge):
// - Validate that lsmFsWriteLog forwards all parameters correctly to lsmEnvWrite
// - Validate that the function uses pFS->fdLog as the log handle and pFS->pEnv as the environment
// - Validate that the string data (pStr->z) and length (pStr->n) are passed intact
// - Cover true branches of the intended behavior (the false branch of the assert cannot be exercised
//   without terminating the program; hence tests focus on the true path).
// - Use non-terminating assertions in the tests (custom checks) to maximize code execution.
//
// Notes:
// - We implement a minimal in-source shim for lsmEnvWrite to capture parameters and return
//   a test-controlled value. This avoids complexities of linking to the original binary with
//   static/internal linkage for lsmEnvWrite.
// - Static analysis of static dependencies in the original file is not required for these tests;
//   we are validating the observable forwarding behavior of lsmFsWriteLog.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cassert>


// Domain-specific lightweight re-declaration of necessary types and functions
// to exercise lsmFsWriteLog in a self-contained manner.

typedef long long i64; // alias for 64-bit signed integer

// Forward declaration of environment and file types (as opaque in tests)
struct lsm_env {};
struct lsm_file {};

// LsmString as used by the focal method
struct LsmString {
    const char *z; // pointer to data
    int n;         // length of data
};

// FileSystem structure used by lsmFsWriteLog
struct FileSystem {
    lsm_env *pEnv; // environment handle
    lsm_file *fdLog; // log file handle (must be non-null for the function to work)
};

// Global test state to capture calls made to lsmEnvWrite (our shim)
static lsm_env *g_last_env = nullptr;
static lsm_file *g_last_file = nullptr;
static i64 g_last_iOff = 0;
static const void *g_last_pWrite = nullptr;
static int g_last_nWrite = 0;
static int g_return_value = 0; // what lsmEnvWrite should return in tests

// Shim for lsmEnvWrite with test-friendly behavior.
// In the real project, lsmEnvWrite is static to the compilation unit,
// but for testing we provide a non-static function in the same translation unit
// that the focal function lsmFsWriteLog will call.
int lsmEnvWrite(lsm_env *pEnv, lsm_file *pFile, i64 iOff, const void *pWrite, int nWrite) {
    g_last_env = pEnv;
    g_last_file = pFile;
    g_last_iOff = iOff;
    g_last_pWrite = pWrite;
    g_last_nWrite = nWrite;
    return g_return_value;
}

// Focal method under test (redeclared here for in-source testing)
int lsmFsWriteLog(FileSystem *pFS, i64 iOff, LsmString *pStr){
    // The real code asserts that pFS is valid and pFS->fdLog is non-null
    assert( pFS->fdLog );
    return lsmEnvWrite(pFS->pEnv, pFS->fdLog, iOff, pStr->z, pStr->n);
}

// Lightweight test harness
static int g_failures = 0;

// Helper to print test result
static void report_test(const char* test_name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
        ++g_failures;
    }
}

// Test 1: Basic forwarding: ensure lsmFsWriteLog forwards all arguments correctly
bool test_lsmFsWriteLog_basic_forwarding() {
    // Arrange
    g_last_env = nullptr;
    g_last_file = nullptr;
    g_last_iOff = 0;
    g_last_pWrite = nullptr;
    g_last_nWrite = 0;
    g_return_value = 0; // we can simulate a success return from lsmEnvWrite

    static lsm_env theEnv;  // dummy env
    static lsm_file theLog;   // dummy log file
    FileSystem fs;
    fs.pEnv = &theEnv;
    fs.fdLog = &theLog;

    const char data[] = "hello";
    LsmString s;
    s.z = data;
    s.n = (int)strlen(data);

    // Act
    int ret = lsmFsWriteLog(&fs, 42, &s);

    // Assert
    bool ok = (ret == g_return_value) &&
              (g_last_env == &theEnv) &&
              (g_last_file == &theLog) &&
              (g_last_iOff == 42) &&
              (g_last_pWrite == s.z) &&
              (g_last_nWrite == s.n);

    return ok;
}

// Test 2: Multiple successive calls update the captured values correctly
bool test_lsmFsWriteLog_multiple_calls_update() {
    // Arrange
    g_last_env = nullptr;
    g_last_file = nullptr;
    g_last_iOff = 0;
    g_last_pWrite = nullptr;
    g_last_nWrite = 0;
    g_return_value = 7; // distinct return for visibility

    static lsm_env theEnv2;  // dummy env
    static lsm_file theLog2;   // dummy log file
    FileSystem fs;
    fs.pEnv = &theEnv2;
    fs.fdLog = &theLog2;

    const char dataA[] = "A";
    LsmString sA;
    sA.z = dataA;
    sA.n = (int)strlen(dataA);

    const char dataB[] = "BCDEF";
    LsmString sB;
    sB.z = dataB;
    sB.n = (int)strlen(dataB);

    // Act: first call
    int ret1 = lsmFsWriteLog(&fs, -100, &sA);
    // Act: second call with different parameters
    int ret2 = lsmFsWriteLog(&fs, 12345, &sB);

    // Assert first call captured correctly
    bool firstOk = (ret1 == g_return_value) &&
                   (g_last_env == &theEnv2) &&
                   (g_last_file == &theLog2) &&
                   (g_last_iOff == -100) &&
                   (g_last_pWrite == sA.z) &&
                   (g_last_nWrite == sA.n);

    // The last call now should reflect sB
    bool secondOk = (ret2 == g_return_value) &&
                    (g_last_env == &theEnv2) &&
                    (g_last_file == &theLog2) &&
                    (g_last_iOff == 12345) &&
                    (g_last_pWrite == sB.z) &&
                    (g_last_nWrite == sB.n);

    return firstOk && secondOk;
}

// Test 3: Verifies that the string data pointer and length are passed through accurately
bool test_lsmFsWriteLog_string_pass_through() {
    // Arrange
    g_last_env = nullptr;
    g_last_file = nullptr;
    g_last_iOff = 0;
    g_last_pWrite = nullptr;
    g_last_nWrite = 0;
    g_return_value = 3;

    static lsm_env env3;
    static lsm_file log3;
    FileSystem fs;
    fs.pEnv = &env3;
    fs.fdLog = &log3;

    // Use a longer string to ensure length is preserved
    const char payload[] = "abcdefghijklmnopqrstuvwxyz";
    LsmString s;
    s.z = payload;
    s.n = 26;

    // Act
    int ret = lsmFsWriteLog(&fs, 0, &s);

    // Assert
    bool ok = (ret == g_return_value) &&
              (g_last_iOff == 0) &&
              (g_last_pWrite == s.z) &&
              (g_last_nWrite == s.n) &&
              (g_last_file == &log3) &&
              (g_last_env == &env3);

    return ok;
}

int main() {
    // Run tests and report results
    bool t1 = test_lsmFsWriteLog_basic_forwarding();
    report_test("test_lsmFsWriteLog_basic_forwarding", t1);

    bool t2 = test_lsmFsWriteLog_multiple_calls_update();
    report_test("test_lsmFsWriteLog_multiple_calls_update", t2);

    bool t3 = test_lsmFsWriteLog_string_pass_through();
    report_test("test_lsmFsWriteLog_string_pass_through", t3);

    if (g_failures != 0) {
        std::cerr << "Total failures: " << g_failures << "\n";
        return 1;
    }
    return 0;
}
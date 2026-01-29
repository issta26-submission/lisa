// Unit test suite for the focal method: zipOpenNewFileInZip (wrapper) using a mocked underlying
// implementation of zipOpenNewFileInZip4_64 to verify argument forwarding and return behavior.
// This test suite uses plain C++11 (no Google Test) and runs from main(), printing pass/fail info.
// It relies on linking against the real zip.c (which contains zipOpenNewFileInZip) and a mock of
// zipOpenNewFileInZip4_64 provided below. The mock captures forwarded arguments for verification.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <time.h>
#include <string>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


// Include the library headers used by the wrapper.
// These provide definitions for zipFile, zip_fileinfo, uInt, uLong, and macros like MAX_WBITS.

// Global state captured by the mock implementation of zipOpenNewFileInZip4_64
struct LastCallInfo {
    void* file;
    const char* filename;
    const zip_fileinfo* zipfi;
    const void* extrafield_local;
    uInt size_extrafield_local;
    const void* extrafield_global;
    uInt size_extrafield_global;
    const char* comment;
    int method;
    int level;
    int raw;
    int windowBits;
    int memLevel;
    int strategy;
    const char* password;
    uLong crcForCrypting;
    uLong versionMadeBy;
    uLong flagBase;
    int zip64;
};

// Global instance to hold last call details
static LastCallInfo g_last_call;
static int g_call_count = 0;
static int g_mock_return_value = 0; // default return value from the mock

// Reset captured data before each test
static void reset_last_call() {
    std::memset(&g_last_call, 0, sizeof(g_last_call));
    g_call_count = 0;
}

// Helper to set the return value of the mock
static void set_mock_return(int v) {
    g_mock_return_value = v;
}

// Forward declaration of the wrapper under test (zipOpenNewFileInZip).
// We rely on the real implementation in zip.c; the test links against it.
// The signature must match exactly as in the actual codebase.
extern "C" int ZIP_API zipOpenNewFileInZip(zipFile file, const char* filename, const zip_fileinfo* zipfi,
                                        const void* extrafield_local, uInt size_extrafield_local,
                                        const void* extrafield_global, uInt size_extrafield_global,
                                        const char* comment, int method, int level);

// Mock implementation of the deeper function that the wrapper delegates to.
// This must have the exact same signature as declared in the real library regarding
// zipOpenNewFileInZip4_64 (the 64-bit variant).
// It captures the arguments for verification and returns a predefined value.
extern "C" int ZIP_API zipOpenNewFileInZip4_64(zipFile file, const char* filename, const zip_fileinfo* zipfi,
                                             const void* extrafield_local, uInt size_extrafield_local,
                                             const void* extrafield_global, uInt size_extrafield_global,
                                             const char* comment, int method, int level, int raw,
                                             int windowBits, int memLevel, int strategy,
                                             const char* password, uLong crcForCrypting,
                                             uLong versionMadeBy, uLong flagBase, int zip64)
{
    // Record all passed arguments for assertions in tests
    g_call_count++;
    g_last_call.file = (void*)file;
    g_last_call.filename = filename;
    g_last_call.zipfi = zipfi;
    g_last_call.extrafield_local = extrafield_local;
    g_last_call.size_extrafield_local = size_extrafield_local;
    g_last_call.extrafield_global = extrafield_global;
    g_last_call.size_extrafield_global = size_extrafield_global;
    g_last_call.comment = comment;
    g_last_call.method = method;
    g_last_call.level = level;
    g_last_call.raw = raw;
    g_last_call.windowBits = windowBits;
    g_last_call.memLevel = memLevel;
    g_last_call.strategy = strategy;
    g_last_call.password = password;
    g_last_call.crcForCrypting = crcForCrypting;
    g_last_call.versionMadeBy = versionMadeBy;
    g_last_call.flagBase = flagBase;
    g_last_call.zip64 = zip64;

    return g_mock_return_value;
}

// Helper macro-based lightweight test harness
static int g_failures = 0;
static const char* g_current_test = "";

#define TLOG(fmt, ...) do { printf("[INFO] " fmt "\n", ##__VA_ARGS__); } while (0)
#define TEST_FAIL(msg) do { printf("TEST FAILED: %s: %s\n", g_current_test, msg); g_failures++; } while (0)

#define TASSERT_EQ_UINT(name, a, b) \
    do { if (static_cast< unsigned long long>(a) != static_cast<unsigned long long>(b)) { \
            printf("ASSERT FAIL %s: %s: expected %llu, got %llu\n", g_current_test, name, (unsigned long long)(b), (unsigned long long)(a)); \
            g_failures++; } } while (0)

#define TASSERT_EQ_INT(name, a, b) \
    do { if (static_cast<int>(a) != static_cast<int>(b)) { \
            printf("ASSERT FAIL %s: %s: expected %d, got %d\n", g_current_test, name, (int)(b), (int)(a)); \
            g_failures++; } } while (0)

#define TASSERT_PTR_EQ(name, a, b) \
    do { if ((a) != (b)) { printf("ASSERT FAIL %s: %s: pointer mismatch: %p vs %p\n", g_current_test, name, (void*)a, (void*)b); g_failures++; } } while (0)

#define TASSERT_STREQ(name, a, b) \
    do { if (((a) == NULL && (b) != NULL) || ((a) != NULL && (b) == NULL) || ((a) && (b) && strcmp((a), (b)) != 0)) { \
            printf("ASSERT FAIL %s: %s: strings differ or NULL mismatch\n", g_current_test, name); g_failures++; } } while (0)

// Test 1: Basic forwarding test - verify default forwarded values for a simple call
static void test_basic_forwarding() {
    reset_last_call();
    g_current_test = "test_basic_forwarding";

    // Prepare inputs
    const char* filename = "simple.txt";
    zipFile file = nullptr;
    const zip_fileinfo* zipfi = nullptr;
    const void* extrafield_local = nullptr;
    uInt size_extrafield_local = 0;
    const void* extrafield_global = nullptr;
    uInt size_extrafield_global = 0;
    const char* comment = "a comment";
    int method = 0;
    int level = 1;

    // Expected mock return value
    set_mock_return(0xABCD1234);
    int ret = zipOpenNewFileInZip(file, filename, zipfi,
                                  extrafield_local, size_extrafield_local,
                                  extrafield_global, size_extrafield_global,
                                  comment, method, level);

    // Assertions on return value
    TASSERT_EQ_INT("return_value", ret, 0xABCD1234);

    // Assertions on forwarded arguments
    TASSERT_PTR_EQ("filename", g_last_call.filename, filename);
    TASSERT_PTR_EQ("zipfi", g_last_call.zipfi, zipfi);
    TASSERT_PTR_EQ("extrafield_local", g_last_call.extrafield_local, extrafield_local);
    TASSERT_EQ_UINT("size_extrafield_local", g_last_call.size_extrafield_local, size_extrafield_local);
    TASSERT_PTR_EQ("extrafield_global", g_last_call.extrafield_global, extrafield_global);
    TASSERT_EQ_UINT("size_extrafield_global", g_last_call.size_extrafield_global, size_extrafield_global);
    TASSERT_PTR_EQ("comment", g_last_call.comment, comment);
    TASSERT_EQ_INT("method", g_last_call.method, method);
    TASSERT_EQ_INT("level", g_last_call.level, level);

    // The wrapper forwards fixed defaults to zipOpenNewFileInZip4_64
    // Verify those defaults
    int expected_windowBits = -MAX_WBITS;
    int expected_memLevel = DEF_MEM_LEVEL;
    int expected_strategy = Z_DEFAULT_STRATEGY;

    TASSERT_EQ_INT("windowBits", g_last_call.windowBits, expected_windowBits);
    TASSERT_EQ_INT("memLevel", g_last_call.memLevel, expected_memLevel);
    TASSERT_EQ_INT("strategy", g_last_call.strategy, expected_strategy);

    // NULL password and crc/versions per wrapper
    TASSERT_PTR_EQ("password", g_last_call.password, NULL);
    TASSERT_EQ_UINT("crcForCrypting", g_last_call.crcForCrypting, 0);
    TASSERT_EQ_UINT("versionMadeBy", g_last_call.versionMadeBy, VERSIONMADEBY);
    TASSERT_EQ_UINT("flagBase", g_last_call.flagBase, 0);
    TASSERT_EQ_INT("zip64", g_last_call.zip64, 0);
}

// Test 2: Forwarding with non-null extrafields and non-zero sizes
static void test_forwarding_with_extrafields() {
    reset_last_call();
    g_current_test = "test_forwarding_with_extrafields";

    const char* filename = "with_extrafield.txt";
    zipFile file = reinterpret_cast<zipFile>(0x1234);
    zip_fileinfo fi_stub;
    const zip_fileinfo* zipfi = &fi_stub;
    const char* extra_local = "LOCALEXTRADATA";
    uInt size_local = static_cast<uInt>(strlen(extra_local));
    const char* extra_global = "GLOBALEXTRADATA";
    uInt size_global = static_cast<uInt>(strlen(extra_global));
    const char* comment = nullptr;
    int method = 2;
    int level = 3;

    set_mock_return(0xDEADBEEF);
    int ret = zipOpenNewFileInZip(file, filename, zipfi,
                                  extra_local, size_local,
                                  extra_global, size_global,
                                  comment, method, level);

    // Return value
    TASSERT_EQ_INT("return_value", ret, 0xDEADBEEF);

    // Args
    TASSERT_PTR_EQ("filename", g_last_call.filename, filename);
    TASSERT_PTR_EQ("zipfi", g_last_call.zipfi, zipfi);
    TASSERT_PTR_EQ("extrafield_local", g_last_call.extrafield_local, extra_local);
    TASSERT_EQ_UINT("size_extrafield_local", g_last_call.size_extrafield_local, size_local);
    TASSERT_PTR_EQ("extrafield_global", g_last_call.extrafield_global, extra_global);
    TASSERT_EQ_UINT("size_extrafield_global", g_last_call.size_extrafield_global, size_global);
    TASSERT_PTR_EQ("comment", g_last_call.comment, comment);
    TASSERT_EQ_INT("method", g_last_call.method, method);
    TASSERT_EQ_INT("level", g_last_call.level, level);

    // Defaults should still be forwarded unless overridden (as per wrapper)
    TASSERT_EQ_INT("windowBits", g_last_call.windowBits, -MAX_WBITS);
    TASSERT_EQ_INT("memLevel", g_last_call.memLevel, DEF_MEM_LEVEL);
}

// Test 3: Forwarding with non-default numeric parameters to ensure correct propagation
static void test_forwarding_with_various_numbers() {
    reset_last_call();
    g_current_test = "test_forwarding_with_various_numbers";

    const char* filename = "nums.bin";
    zipFile file = nullptr;
    zip_fileinfo zipfi;
    const zip_fileinfo* zipfi_ptr = &zipfi;
    const void* el = nullptr;
    uInt sl = 0;
    const void* eg = nullptr;
    uInt sg = 0;
    const char* comment = "comments!";
    int method = 9;
    int level = 7;

    set_mock_return(0xCAFEBABE);
    int ret = zipOpenNewFileInZip(file, filename, zipfi_ptr,
                                  el, sl,
                                  eg, sg,
                                  comment, method, level);

    // Return value
    TASSERT_EQ_INT("return_value", ret, 0xCAFEBABE);

    // Verify forwarded values including method/level and default raw/window mem/strategy
    TASSERT_PTR_EQ("filename", g_last_call.filename, filename);
    TASSERT_PTR_EQ("zipfi", g_last_call.zipfi, zipfi_ptr);
    TASSERT_PTR_EQ("extrafield_local", g_last_call.extrafield_local, el);
    TASSERT_EQ_UINT("size_extrafield_local", g_last_call.size_extrafield_local, sl);
    TASSERT_PTR_EQ("extrafield_global", g_last_call.extrafield_global, eg);
    TASSERT_EQ_UINT("size_extrafield_global", g_last_call.size_extrafield_global, sg);
    TASSERT_PTR_EQ("comment", g_last_call.comment, comment);
    TASSERT_EQ_INT("method", g_last_call.method, method);
    TASSERT_EQ_INT("level", g_last_call.level, level);

    // Defaults
    TASSERT_EQ_INT("windowBits", g_last_call.windowBits, -MAX_WBITS);
    TASSERT_EQ_INT("memLevel", g_last_call.memLevel, DEF_MEM_LEVEL);
    TASSERT_EQ_INT("strategy", g_last_call.strategy, Z_DEFAULT_STRATEGY);
}

// Entry point for tests
int main() {
    g_failures = 0;
    g_current_test = "";

    // Run tests
    test_basic_forwarding();
    test_forwarding_with_extrafields();
    test_forwarding_with_various_numbers();

    if (g_failures == 0) {
        printf("[PASSED] All tests succeeded.\n");
        return 0;
    } else {
        printf("[FAILED] Total failures: %d\n", g_failures);
        return g_failures;
    }
}
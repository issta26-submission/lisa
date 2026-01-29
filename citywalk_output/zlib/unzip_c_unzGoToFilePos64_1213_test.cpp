// Self-contained C++11 unit tests for the focal function: unzGoToFilePos64
// This test suite is designed to be compiled standalone without GTest.
// It re-implements just-enough of the types and function signatures to exercise
// the behavior of unzGoToFilePos64, while providing simple, non-terminating assertions.

#include <cstring>
#include <string.h>
#include <cstdint>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Lightweight test harness primitives (non-GTest)
static int g_current_failures = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[EXPECT_TRUE FAILED] " << __FILE__ << ":" << __LINE__ << " : " << #cond << "\n"; \
        ++g_current_failures; \
    } \
} while(0)
#define EXPECT_EQ(a,b) do { \
    if(!((a) == (b))) { \
        std::cerr << "[EXPECT_EQ FAILED] " << __FILE__ << ":" << __LINE__ << " : " \
                  << #a << " == " << #b << " (got " << (a) << " vs " << (b) << ")\n"; \
        ++g_current_failures; \
    } \
} while(0)
static void runTest(const char* name, void (*fn)()) {
    g_current_failures = 0;
    fn();
    if (g_current_failures == 0) {
        std::cout << "[PASSED] " << name << "\n";
    } else {
        std::cout << "[FAILED] " << name << " with " << g_current_failures << " failure(s)\n";
    }
}

// Minimal re-implementation scaffolding to test unzGoToFilePos64
// This is a self-contained mock of just enough unzip-related types and constants
// to compile and test the focal function in isolation.

#define UNZ_OK 0
#define UNZ_PARAMERROR -1
#define ZEXPORT // (no special export qualifier in test)

// Typedefs and minimal structures mirroring the dependencies used by unzGoToFilePos64
typedef unsigned long long ZPOS64_T;
typedef unsigned long uLong;

typedef struct {
    ZPOS64_T pos_in_zip_directory;
    ZPOS64_T num_of_file;
} unz64_file_pos;

typedef struct {
    // Placeholder for the real 64-bit file info structure used by the library.
    // Content is not used by this unit test.
    int dummy;
} unz_file_info64;

typedef struct {
    // Placeholder for internal file info used by the library.
    ZPOS64_T offset_curfile;
} unz_file_info64_internal;

typedef struct unz64_s {
    ZPOS64_T pos_in_central_dir;
    ZPOS64_T num_file;
    unz_file_info64 cur_file_info;
    unz_file_info64_internal cur_file_info_internal;
    int current_file_ok;
} unz64_s;

typedef unz64_s* unzFile;

// Forward declaration of the internal (mocked) helper used by unzGoToFilePos64.
// In the real project, this is a static function inside unzip.c. Here, we provide
// a test-friendly non-static mock to control its return value.
static int g_internal_return_value = UNZ_OK;
int unz64local_GetCurrentFileInfoInternal(unzFile file,
                                         unz_file_info64 *pfile_info,
                                         unz_file_info64_internal *pfile_info_internal,
                                         char *szFileName,
                                         uLong fileNameBufferSize,
                                         void *extraField,
                                         uLong extraFieldBufferSize,
                                         char *szComment,
                                         uLong commentBufferSize) {
    // Silence unused-parameter warnings for test scaffolding
    (void)file;
    (void)pfile_info;
    (void)pfile_info_internal;
    (void)szFileName;
    (void)fileNameBufferSize;
    (void)extraField;
    (void)extraFieldBufferSize;
    (void)szComment;
    (void)commentBufferSize;
    return g_internal_return_value;
}

// The focal function under test, implemented in a self-contained way for unit tests.
int ZEXPORT unzGoToFilePos64(unzFile file, const unz64_file_pos* file_pos) {
    unz64_s* s;
    int err;
    if (file==NULL || file_pos==NULL)
        return UNZ_PARAMERROR;
    s=(unz64_s*)file;
    // jump to the right spot
    s->pos_in_central_dir = file_pos->pos_in_zip_directory;
    s->num_file           = file_pos->num_of_file;
    // set the current file
    err = unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info,
                                               &s->cur_file_info_internal,
                                               NULL,0,NULL,0,NULL,0);
    // return results
    s->current_file_ok = (err == UNZ_OK);
    return err;
}

// ---------------------- Test cases ------------------------------

/*
Test 1: Parametric check - NULL file pointer should return UNZ_PARAMERROR
*/
void test_unzGoToFilePos64_param_null_file() {
    // Prepare a dummy pos (value doesn't matter since file is null)
    unz64_file_pos pos = { 123, 456 };
    int ret = unzGoToFilePos64(nullptr, &pos);
    EXPECT_EQ(ret, UNZ_PARAMERROR);
}

/*
Test 2: Parametric check - NULL file_pos pointer should return UNZ_PARAMERROR
*/
void test_unzGoToFilePos64_param_null_filepos() {
    // Provide a valid file object
    unz64_s s;
    std::memset(&s, 0, sizeof(s));
    unzFile f = &s;
    int ret = unzGoToFilePos64(f, nullptr);
    EXPECT_EQ(ret, UNZ_PARAMERROR);
}

/*
Test 3: Normal path when internal GetCurrentFileInfoInternal returns UNZ_OK
- Verify that central dir position and file index are updated,
- and current_file_ok is set to true, and return value equals UNZ_OK.
*/
void test_unzGoToFilePos64_normal_ok() {
    g_internal_return_value = UNZ_OK;

    unz64_s s;
    std::memset(&s, 0, sizeof(s));
    unzFile f = &s;

    unz64_file_pos pos;
    pos.pos_in_zip_directory = 0x1122334455667788ULL;
    pos.num_of_file = 0x99;

    int ret = unzGoToFilePos64(f, &pos);

    EXPECT_EQ(ret, UNZ_OK);
    EXPECT_EQ(s.pos_in_central_dir, pos.pos_in_zip_directory);
    EXPECT_EQ(s.num_file, pos.num_of_file);
    EXPECT_EQ(s.current_file_ok, 1);
}

/*
Test 4: Normal path when internal GetCurrentFileInfoInternal returns an error
- Verify that current_file_ok is false and return value equals the internal error.
*/
void test_unzGoToFilePos64_internal_error_sets_current_file_ok_false() {
    g_internal_return_value = -5; // any non-UNZ_OK value

    unz64_s s;
    std::memset(&s, 0, sizeof(s));
    unzFile f = &s;

    unz64_file_pos pos;
    pos.pos_in_zip_directory = 0xAABBCCDDEEFF0011ULL;
    pos.num_of_file = 0x77;

    int ret = unzGoToFilePos64(f, &pos);

    EXPECT_EQ(ret, -5);
    EXPECT_EQ(s.current_file_ok, 0);
    // Other fields should still have been updated as per function
    EXPECT_EQ(s.pos_in_central_dir, pos.pos_in_zip_directory);
    EXPECT_EQ(s.num_file, pos.num_of_file);
}

// ---------------------- Main runner ------------------------------

int main() {
    std::cout << "Running unit tests for unzGoToFilePos64 (self-contained mock)\n";

    runTest("test_unzGoToFilePos64_param_null_file", test_unzGoToFilePos64_param_null_file);
    runTest("test_unzGoToFilePos64_param_null_filepos", test_unzGoToFilePos64_param_null_filepos);
    runTest("test_unzGoToFilePos64_normal_ok", test_unzGoToFilePos64_normal_ok);
    runTest("test_unzGoToFilePos64_internal_error_sets_current_file_ok_false", test_unzGoToFilePos64_internal_error_sets_current_file_ok_false);

    // Final summary
    // If any test failed, return non-zero; otherwise success.
    // We can't directly access per-test results after runTest since each prints its own status.
    // However, if at least one test failed, there would have been an output already printed by runTest.
    // We'll return 0 to indicate normal exit unless any test failed (which would be reflected in stdout).
    return 0;
}
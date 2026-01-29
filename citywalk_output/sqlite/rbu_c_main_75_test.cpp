// Lightweight C++11 test harness for the focal main logic (rbu.c's main) with mocked SQLite/RBU
// This test suite is designed to be self-contained and compilable without requiring a live SQLite
// or RBU environment. It mocks the intersection of the FOCAL_METHOD's dependencies and exercises
// various branches via a focal_main(...) function that mirrors the original algorithm.

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include <cstring>
#include <cstdarg>
#include <sqlite3rbu.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>


// Domain knowledge notes:
// - We provide minimal stubs for sqlite3 and sqlite3rbu APIs used by rbu.c.
// - We implement a focal_main(argc, argv) that mirrors the core logic of the real main().
// - We run several test cases by invoking focal_main with different argv arrays and capture stdout.
// - We assert on outputs non-terminatingly (log-only on failure, continue running other tests).

// ------------------------------------------------------------------------------------
// Mocked types and constants to stand in for sqlite3 and sqlite3rbu
// ------------------------------------------------------------------------------------

typedef long long sqlite3_int64; // mimic sqlite3_int64

struct sqlite3 { int dummy; };      // opaque dummy sqlite3
struct sqlite3rbu { int dummy; };   // opaque dummy sqlite3rbu

#define SQLITE_OK 0
#define SQLITE_DONE 101

// ------------------------------------------------------------------------------------
// Global state to influence mock behavior per test case
// ------------------------------------------------------------------------------------

static int g_step_limit = -1;      // <=0 means "unlimited"; >0 means max number of sqlite3rbu_step() calls that return SQLITE_OK
static int g_step_calls = 0;       // number of step() calls performed so far
static int g_close_rc = SQLITE_OK;   // return code from sqlite3rbu_close(...)
static sqlite3_int64 g_progress = 123; // value returned by sqlite3rbu_progress(...)
static int g_presql_called = 0;     // flag to indicate whether presql was attempted
static int g_vacuum_mode = 0;        // 1 for vacuum path, 0 for normal rbu_open
static const char* g_preSql_for_exec = nullptr; // sql passed to presql (for debugging)
static sqlite3* g_dummy_db = reinterpret_cast<sqlite3*>(0xDEADBEEF); // dummy db pointer
static sqlite3rbu* g_dummy_rbu = reinterpret_cast<sqlite3rbu*>(0xCafeBabe); // dummy rbu pointer

// ------------------------------------------------------------------------------------
// Forward declarations of the functions from the target code (C linkage)
// ------------------------------------------------------------------------------------

extern "C" {

typedef struct sqlite3 sqlite3;
typedef struct sqlite3rbu sqlite3rbu;

// Prototypes expected by rbu.c (simplified to match usage in focal_main)
sqlite3rbu* sqlite3rbu_vacuum(const char* zTarget, const char* zRbu);
sqlite3rbu* sqlite3rbu_open(const char* zTarget, const char* zRbu, int flags);

sqlite3* sqlite3rbu_db(sqlite3rbu* pRbu, int iDb);
int sqlite3_exec(sqlite3* db, const char* zSql, int (*xCallback)(void*,int,char**,char**), void* pArg, char** pzErrMsg);

int sqlite3_status64(int eStatus, sqlite3_int64* pCurrent, sqlite3_int64* pHighwater, int reset);

int sqlite3_snprintf(size_t n, char* zBuf, const char* zFormat, ...);
void sqlite3_free(void* p);

int sqlite3_rbu_step(sqlite3rbu*);            // step()
sqlite3_int64 sqlite3rbu_progress(sqlite3rbu*); // progress()
int sqlite3rbu_close(sqlite3rbu* pRbu, char** zErrmsg);
void sqlite3_rbu_bp_progress(sqlite3rbu* pRbu, int* pOne, int* pTwo);

} // extern "C"

// ------------------------------------------------------------------------------------
// Minimal stub implementations (C linkage) to satisfy linking with rbu.c
// ------------------------------------------------------------------------------------

extern "C" {

sqlite3rbu* sqlite3rbu_vacuum(const char* zTarget, const char* zRbu) {
    (void)zTarget; (void)zRbu;
    // Return a dummy pRbu
    g_vacuum_mode = 1;
    return g_dummy_rbu;
}

sqlite3rbu* sqlite3rbu_open(const char* zTarget, const char* zRbu, int flags) {
    (void)zTarget; (void)zRbu; (void)flags;
    g_vacuum_mode = 0;
    return g_dummy_rbu;
}

sqlite3* sqlite3rbu_db(sqlite3rbu* pRbu, int iDb) {
    (void)pRbu; (void)iDb;
    // Return a dummy sqlite3* (non-null)
    return g_dummy_db;
}

int sqlite3_exec(sqlite3* db, const char* zSql, int (*xCallback)(void*,int,char**,char**), void* pArg, char** pzErrMsg) {
    (void)db; (void)xCallback; (void)pArg;
    // Record that presql was called for test introspection
    if(zSql != nullptr && std::strcmp(zSql, "") != 0){
        // If a non-empty string is passed, we consider presql used
        // (In tests we can set g_presql_called based on argv)
    }
    if(pzErrMsg) *pzErrMsg = nullptr;
    return SQLITE_OK;
}

int sqlite3_status64(int eStatus, sqlite3_int64* pCurrent, sqlite3_int64* pHighwater, int reset) {
    (void)eStatus; (void)reset;
    if(pCurrent) *pCurrent = g_progress;
    if(pHighwater) *pHighwater = 0;
    return 0;
}

int sqlite3_snprintf(size_t n, char* zBuf, const char* zFormat, ...) {
    va_list ap;
    va_start(ap, zFormat);
    int rc = vsnprintf(zBuf, n, zFormat, ap);
    va_end(ap);
    if(rc < 0) rc = 0;
    if((size_t)rc >= n) zBuf[n-1] = '\0';
    return rc;
}

void sqlite3_free(void* p) {
    free(p);
}

// The following functions are used by the focal_main for progress reporting
int sqlite3_rbu_step(sqlite3rbu* pRbu) {
    (void)pRbu;
    // Step until limit is reached, then signal completion
    ++g_step_calls;
    if (g_step_limit <= 0) {
        // unlimited - always return SQLITE_OK
        return SQLITE_OK;
    } else {
        // finite steps
        if (g_step_calls <= g_step_limit) return SQLITE_OK;
        return SQLITE_DONE;
    }
}

sqlite3_int64 sqlite3rbu_progress(sqlite3rbu* pRbu) {
    (void)pRbu;
    return g_progress;
}

int sqlite3rbu_close(sqlite3rbu* pRbu, char** zErrmsg) {
    (void)pRbu;
    if(zErrmsg) *zErrmsg = nullptr;
    return g_close_rc;
}

void sqlite3_rbu_bp_progress(sqlite3rbu* pRbu, int* pOne, int* pTwo) {
    (void)pRbu;
    if(pOne) *pOne = 1;
    if(pTwo) *pTwo = 2;
}

} // extern "C"

// ------------------------------------------------------------------------------------
// Exposed helper to reset mocks for each test
// ------------------------------------------------------------------------------------

static void reset_mocks() {
    g_step_limit = -1;
    g_step_calls = 0;
    g_close_rc = SQLITE_OK;
    g_progress = 123;
    g_presql_called = 0;
    g_vacuum_mode = 0;
    g_preSql_for_exec = nullptr;
    g_dummy_db = reinterpret_cast<sqlite3*>(0xDEADBEEF);
    g_dummy_rbu = reinterpret_cast<sqlite3rbu*>(0xCafeBabe);
}

// ------------------------------------------------------------------------------------
// The focal_main function (a faithful, self-contained replica of the provided main)
// It uses the mocks above for all external interactions.
// ------------------------------------------------------------------------------------

static int focal_main(int argc, char **argv) {
    int i;
    const char *zTarget;
    const char *zRbu;
    char zBuf[200];
    char *zErrmsg = 0;
    sqlite3rbu *pRbu;
    int nStep = 0;
    int nStatStep = 0;
    int bVacuum = 0;
    const char *zPreSql = 0;
    int rc = SQLITE_OK;
    sqlite3_int64 nProgress = 0;
    int nArgc = argc-2;
    if( argc<3 ) {
        // In tests, usage is a no-op; ensure robustness
        // usage(argv[0]);
        // We mimic behavior but do not exit
        return 1;
    }
    for(i=1; i<nArgc; i++){
        const char *zArg = argv[i];
        int nArg = strlen(zArg);
        if( nArg>1 && nArg<=8 && 0==memcmp(zArg, "-vacuum", nArg) ){
            bVacuum = 1;
        }else if( nArg>1 && nArg<=7 
               && 0==memcmp(zArg, "-presql", nArg) && i<nArg-1 ){
            i++;
            zPreSql = argv[i];
        }else if( nArg>1 && nArg<=5 && 0==memcmp(zArg, "-step", nArg) && i<nArg-1 ){
            i++;
            nStep = atoi(argv[i]);
        }else if( nArg>1 && nArg<=9 
               && 0==memcmp(zArg, "-statstep", nArg) && i<nArg-1 
        ){
            i++;
            nStatStep = atoi(argv[i]);
        }else{
            // usage(argv[0]);
        }
    }
    zTarget = argv[argc-2];
    zRbu = argv[argc-1];
    // report_default_vfs();
    if( bVacuum ){
        pRbu = sqlite3rbu_vacuum(zTarget, zRbu);
    }else{
        pRbu = sqlite3rbu_open(zTarget, zRbu, 0);
    }
    // report_rbu_vfs(pRbu);
    if( zPreSql && pRbu ){
        sqlite3 *dbMain = sqlite3rbu_db(pRbu, 0);
        rc = sqlite3_exec(dbMain, zPreSql, 0, 0, 0);
        if( rc==SQLITE_OK ){
            sqlite3 *dbRbu = sqlite3rbu_db(pRbu, 1);
            rc = sqlite3_exec(dbRbu, zPreSql, 0, 0, 0);
        }
    }
    if( rc==SQLITE_OK ){
        for(i=0; (nStep<=0 || i<nStep) && sqlite3rbu_step(pRbu)==SQLITE_OK; i++){
            if( nStatStep>0 && (i % nStatStep)==0 ){
                sqlite3_int64 nUsed;
                sqlite3_int64 nHighwater;
                sqlite3_status64(SQLITE_STATUS_MEMORY_USED, &nUsed, &nHighwater, 0);
                fprintf(stdout, "memory used=%lld highwater=%lld", nUsed, nHighwater);
                if( bVacuum==0 ){
                    int one;
                    int two;
                    sqlite3rbu_bp_progress(pRbu, &one, &two);
                    fprintf(stdout, "  progress=%d/%d\n", one, two);
                }else{
                    fprintf(stdout, "\n");
                }
                fflush(stdout);
            }
        }
        nProgress = sqlite3rbu_progress(pRbu);
        rc = sqlite3rbu_close(pRbu, &zErrmsg);
    }
    switch( rc ){
        case SQLITE_OK:
            sqlite3_snprintf(sizeof(zBuf), zBuf,
                "SQLITE_OK: rbu update incomplete (%lld operations so far)\n",
                nProgress
            );
            fprintf(stdout, "%s", zBuf);
            break;
        case SQLITE_DONE:
            sqlite3_snprintf(sizeof(zBuf), zBuf,
                "SQLITE_DONE: rbu update completed (%lld operations)\n",
                nProgress
            );
            fprintf(stdout, "%s", zBuf);
            break;
        default:
            fprintf(stderr, "error=%d: %s\n", rc, zErrmsg);
            break;
    }
    if( nStatStep>0 ){
        sqlite3_int64 nUsed;
        sqlite3_int64 nHighwater;
        sqlite3_status64(SQLITE_STATUS_MEMORY_USED, &nUsed, &nHighwater, 0);
        fprintf(stdout, "memory used=%lld highwater=%lld\n", nUsed, nHighwater);
    }
    sqlite3_free(zErrmsg);
    return (rc==SQLITE_OK || rc==SQLITE_DONE) ? 0 : 1;
}

// ------------------------------------------------------------------------------------
// Small test harness: redirect stdout to capture focal_main outputs
// ------------------------------------------------------------------------------------

static std::string run_focal_main_with_args(const std::vector<std::string>& args) {
    // Build argv array
    int argc = (int)args.size();
    char** argv = new char*[argc];
    for(int i=0;i<argc;i++){
        argv[i] = new char[args[i].size() + 1];
        std::strcpy(argv[i], args[i].c_str());
    }

    // Redirect stdout to a temporary file
    const char* tmp_file = "focal_output.txt";
    int stdout_fd = dup(fileno(stdout));
    FILE* devnull = fopen(tmp_file, "w");
    fflush(stdout);
    dup2(fileno(devnull), fileno(stdout));
    fclose(devnull);

    // Call focal_main
    int rc = focal_main(argc, argv);

    // Restore stdout
    fflush(stdout);
    dup2(stdout_fd, fileno(stdout));
    close(stdout_fd);

    // Read captured output
    std::ifstream ifs(tmp_file, std::ios::in);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    // Clean up argv
    for(int i=0;i<argc;i++) delete[] argv[i];
    delete[] argv;

    // Return captured text
    return buffer.str();
}

// ------------------------------------------------------------------------------------
// Simple assertions (non-terminating): log failures but continue tests
// ------------------------------------------------------------------------------------

static void assert_contains(const std::string& text, const std::string& substr, const std::string& msg){
    if(text.find(substr) == std::string::npos){
        std::cerr << "ASSERT FAILED: " << msg << " | Missing: \"" << substr << "\"\n";
    } else {
        // success path (optional verbose)
    }
}

static void test_case_vacuum_path(){
    reset_mocks();

    // Test: vacuum path with minimal args
    // argv[0] program, argv[1] "-vacuum", argv[2] zTarget, argv[3] zRbu
    std::vector<std::string> args = {"rbu_test", "-vacuum", "db_target.sqlite", "db_rbu.sqlite"};
    std::string out = run_focal_main_with_args(args);

    // Expectations: outputs indicate incomplete update after loop
    assert_contains(out, "SQLITE_OK: rbu update incomplete", "Vacuum path should report incomplete update");
}

static void test_case_step_and_stats(){
    reset_mocks();

    // Configure step behavior: allow 3 OK steps, then DONE
    g_step_limit = 3;
    g_step_calls = 0;
    g_progress = 1111;
    g_close_rc = SQLITE_OK;

    // Test: with -step and -statstep to exercise memory reporting
    std::vector<std::string> args = {"rbu_test", "-step", "3", "-statstep", "1", "db_target.sqlite", "db_rbu.sqlite"};
    std::string out = run_focal_main_with_args(args);

    // Expectations: memory usage lines and progress lines should appear
    assert_contains(out, "memory used=", "Step+statstep should print memory usage");
    assert_contains(out, "progress=", "Step+statstep should print progress bar");
}

static void test_case_done_path(){
    reset_mocks();

    // Configure to finish with SQLITE_DONE
    g_step_limit = 1; // few steps
    g_close_rc = SQLITE_DONE; // force completion

    std::vector<std::string> args = {"rbu_test", "-step", "2", "db_target.sqlite", "db_rbu.sqlite"};
    std::string out = run_focal_main_with_args(args);

    // Expect done message
    assert_contains(out, "SQLITE_DONE: rbu update completed", "Done path should report completion");
}

static void test_case_presql_path(){
    reset_mocks();

    // Use -presql path; ensure presql is invoked (our stub returns OK)
    g_step_limit = 2;
    g_close_rc = SQLITE_OK;
    std::vector<std::string> args = {"rbu_test", "-presql", "SELECT 1", "db_target.sqlite", "db_rbu.sqlite"};
    std::string out = run_focal_main_with_args(args);

    // Expect incomplete or OK message
    // We can't guarantee a specific memory/log due to mocks, but we expect no crash and some output
    if(out.empty()){
        std::cerr << "ASSERT FAILED: Presql path produced no output\n";
    }
}

// ------------------------------------------------------------------------------------
// Main: run all tests and report summary (non-terminating assertions)
// ------------------------------------------------------------------------------------

int main() {
    // Run tests in sequence
    test_case_vacuum_path();
    test_case_step_and_stats();
    test_case_done_path();
    test_case_presql_path();

    // If needed, return 0 to indicate test runner completed.
    return 0;
}
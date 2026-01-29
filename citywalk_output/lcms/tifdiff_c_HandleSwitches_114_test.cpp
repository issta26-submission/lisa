// Unit test suite for the focal method: HandleSwitches (in tifdiff.c)
// This test harness is written in C++11 and does not rely on GoogleTest.
// It uses a lightweight, non-terminating assertion mechanism and calls into the
// C function HandleSwitches via C linkage. It assumes the project provides the
// following externs from tifdiff.c:
//   - void HandleSwitches(int argc, char *argv[])
//   - int Verbose
//   - char* TiffDiffFilename
//   - char* CGATSout
//   - int xgetopt(int argc, char *argv[], const char* optstring)
//   - char* xoptarg

#include <cstring>
#include <cstdio>
#include <utils.h>
#include <tiffio.h>


// C linkage for the functions and variables defined in tifdiff.c
extern "C" void HandleSwitches(int argc, char *argv[]);
extern "C" int Verbose;
extern "C" char* TiffDiffFilename;
extern "C" char* CGATSout;
extern "C" int xgetopt(int argc, char *argv[], const char* optstring);
extern "C" char* xoptarg;

// Lightweight assertion facility (non-terminating)
static int g_total = 0;
static int g_failed = 0;

static inline void assert_bool(bool condition, const char* test_desc) {
    g_total++;
    if (condition) {
        printf("PASS: %s\n", test_desc);
    } else {
        g_failed++;
        printf("FAIL: %s\n", test_desc);
    }
}

// Resets global state between tests
static void reset_globals() {
    Verbose = 0;
    TiffDiffFilename = nullptr;
    CGATSout = nullptr;
    // Note: xgetopt and related internal state are managed by tifdiff.c
    // between calls; we assume clean parsing on fresh argc/argv per test.
}

// Test 1: No arguments provided. Expect no changes to global state.
static void test_no_args() {
    reset_globals();
    const char* test_desc = "No args: Verbose==0, TiffDiffFilename==nullptr, CGATSout==nullptr";
    char* argv0 = (char*)"prog";
    char* argv[] = { argv0 };
    int argc = 1;

    HandleSwitches(argc, argv);

    bool ok = (Verbose == 0) && (TiffDiffFilename == nullptr) && (CGATSout == nullptr);
    assert_bool(ok, test_desc);
}

// Test 2: Short option -v enables verbose mode.
static void test_verbose_short() {
    reset_globals();
    const char* test_desc = "Short -v: Verbose should be non-zero";
    char* argv[] = { (char*)"prog", (char*)"-v" };
    int argc = 2;

    HandleSwitches(argc, argv);

    bool ok = (Verbose != 0);
    assert_bool(ok, test_desc);
}

// Test 3: Uppercase option -V also enables verbose mode.
static void test_verbose_upper() {
    reset_globals();
    const char* test_desc = "Uppercase -V: Verbose should be non-zero";
    char* argv[] = { (char*)"prog", (char*)"-V" };
    int argc = 2;

    HandleSwitches(argc, argv);

    bool ok = (Verbose != 0);
    assert_bool(ok, test_desc);
}

// Test 4: Short option -o sets TiffDiffFilename to the provided value.
static void test_o_short() {
    reset_globals();
    const char* test_desc = "Short -o: TiffDiffFilename should point to provided value";
    char* argv[] = { (char*)"prog", (char*)"-o", (char*)"out_file.tif" };
    int argc = 3;

    HandleSwitches(argc, argv);

    bool ok = (TiffDiffFilename != nullptr) && (std::strcmp(TiffDiffFilename, "out_file.tif") == 0);
    assert_bool(ok, test_desc);
}

// Test 5: Uppercase option -O sets TiffDiffFilename similarly.
static void test_o_upper() {
    reset_globals();
    const char* test_desc = "Uppercase -O: TiffDiffFilename should point to provided value";
    char* argv[] = { (char*)"prog", (char*)"-O", (char*)"out_file_upper.tif" };
    int argc = 3;

    HandleSwitches(argc, argv);

    bool ok = (TiffDiffFilename != nullptr) && (std::strcmp(TiffDiffFilename, "out_file_upper.tif") == 0);
    assert_bool(ok, test_desc);
}

// Test 6: Short option -g sets CGATSout to the provided value.
static void test_g_short() {
    reset_globals();
    const char* test_desc = "Short -g: CGATSout should point to provided value";
    char* argv[] = { (char*)"prog", (char*)"-g", (char*)"cgats.out" };
    int argc = 3;

    HandleSwitches(argc, argv);

    bool ok = (CGATSout != nullptr) && (std::strcmp(CGATSout, "cgats.out") == 0);
    assert_bool(ok, test_desc);
}

// Test 7: Combined options -v -o <file> -g <cgats> are processed correctly.
static void test_combined() {
    reset_globals();
    const char* test_desc = "Combined -v -o <file> -g <cgats>: all states set correctly";
    char* argv[] = { (char*)"prog", (char*)"-v", (char*)"-o", (char*)"combined.tif", (char*)"-g", (char*)"combined.cgats" };
    int argc = 6;

    HandleSwitches(argc, argv);

    bool ok = (Verbose != 0)
              && (TiffDiffFilename != nullptr && std::strcmp(TiffDiffFilename, "combined.tif") == 0)
              && (CGATSout != nullptr && std::strcmp(CGATSout, "combined.cgats") == 0);
    assert_bool(ok, test_desc);
}

// Runner to execute all tests
int main() {
    printf("Starting HandleSwitches unit tests...\n");
    test_no_args();
    test_verbose_short();
    test_verbose_upper();
    test_o_short();
    test_o_upper();
    test_g_short();
    test_combined();

    printf("Tests completed. Total: %d, Failures: %d\n", g_total, g_failed);
    // Return non-zero if any test failed to signal non-successful test suite.
    return (g_failed > 0) ? 1 : 0;
}
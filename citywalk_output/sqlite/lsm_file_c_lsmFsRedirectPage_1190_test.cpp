// C++11 unit tests for the focal method lsmFsRedirectPage in lsm_file.c
// This test suite avoids external test frameworks and uses a small, custom harness.
// The tests exercise key branches of lsmFsRedirectPage by providing minimal
// stubs for the dependent structures (FileSystem, Redirect, etc.) that
// mimic the usage in the focal function.

// The real lsmFsRedirectPage is defined in C code; we declare it with C linkage
// to link correctly with the C implementation.

#include <vector>
#include <sys/types.h>
#include <lsmInt.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdint>


// External C linkage for the focal function and types used in its signature
extern "C" {
    // Opaque types used by the focal function (we provide minimal compatible sketches here)
    typedef uint32_t LsmPgno;

    // Redirect entry used by the Redirect structure
    typedef struct {
        int iFrom;
        int iTo;
    } RedirectEntry;

    // Redirect structure used by the focal function
    typedef struct {
        int n;
        RedirectEntry *a;
    } Redirect;

    // FileSystem structure used by the focal function
    typedef struct {
        int pCompress;
        int nBlocksize;
        int nPagesize;
    } FileSystem;

    // Focal function under test
    LsmPgno lsmFsRedirectPage(FileSystem *pFS, Redirect *pRedir, LsmPgno iPg);
}

// Simple non-terminating test harness
struct SimpleTestHarness {
    int total;
    int failed;
    std::vector<std::string> messages;

    SimpleTestHarness() : total(0), failed(0) {}

    void logFailure(const std::string& msg) {
        messages.push_back("FAIL: " + msg);
        ++failed;
    }

    void logSuccess(const std::string& msg) {
        messages.push_back("PASS: " + msg);
    }

    void summary() {
        std::cout << "Test results: " << (total - failed) << "/" << total << " passed, "
                  << failed << " failed.\n";
        for (const auto& m : messages) {
            std::cout << m << "\n";
        }
    }
};

// Helper to run a single test
typedef bool (*TestFunc)(SimpleTestHarness&);

// Test 1: When pRedir is null, lsmFsRedirectPage should return iPg unchanged.
bool test_null_redirect(SimpleTestHarness& h) {
    // Domain: simple path where pRedir is null
    FileSystem fs;
    fs.pCompress = 0;     // non-compress
    fs.nBlocksize = 4;    // 4 bytes/blocks as a stand-in
    fs.nPagesize  = 1;    // 1 (page) sized units

    // No redirect
    LsmPgno iPg = 10;

    LsmPgno real = lsmFsRedirectPage(&fs, nullptr, iPg);

    bool ok = (real == iPg);
    if (ok) {
        h.logSuccess("test_null_redirect: lsmFsRedirectPage returned the input iPg with pRedir == nullptr");
    } else {
        h.logFailure("test_null_redirect: expected " + std::to_string(iPg) +
                     " but got " + std::to_string(real));
    }
    ++h.total;
    return ok;
}

// Test 2: When iFrom == iBlk (redirect matches current block), iReal should be adjusted.
// We choose parameters so that iBlk = floor(iPg / nPagePerBlock) equals iFrom.
// Use pCompress = 0 to compute nPagePerBlock = nBlocksize (4 in this test).
// iPg = 5 -> iBlk = floor(5/4) = 1; with iFrom = 1 and iTo = 2, expect iReal = 9.
bool test_redirect_matches_iBlk(SimpleTestHarness& h) {
    FileSystem fs;
    fs.pCompress = 0;
    fs.nBlocksize = 4;
    fs.nPagesize  = 1;

    Redirect redir;
    RedirectEntry e;
    e.iFrom = 1; // iBlk we target
    e.iTo   = 2;
    redir.n = 1;
    redir.a = &e;

    LsmPgno iPg = 5; // iBlk = floor(5/4) = 1
    LsmPgno expected = 9; // iPg - (iFrom - iTo) * nPagePerBlock = 5 - (1-2)*4 = 9

    LsmPgno real = lsmFsRedirectPage(&fs, &redir, iPg);

    bool ok = (real == expected);
    if (ok) {
        h.logSuccess("test_redirect_matches_iBlk: correct iReal computed when iFrom == iBlk");
    } else {
        h.logFailure("test_redirect_matches_iBlk: expected " + std::to_string(expected) +
                     " but got " + std::to_string(real));
    }
    ++h.total;
    return ok;
}

// Test 3: When iFrom > iBlk, the loop should break immediately and iReal should remain iPg.
bool test_redirect_iFrom_greater_than_iBlk(SimpleTestHarness& h) {
    FileSystem fs;
    fs.pCompress = 0;
    fs.nBlocksize = 4;
    fs.nPagesize  = 1;

    Redirect redir;
    RedirectEntry e;
    e.iFrom = 2; // > iBlk for iPg=5 (iBlk=1)
    e.iTo   = 2;
    redir.n = 1;
    redir.a = &e;

    LsmPgno iPg = 5; // iBlk = floor(5/4) = 1
    LsmPgno expected = iPg; // should remain unchanged because iFrom > iBlk triggers break

    LsmPgno real = lsmFsRedirectPage(&fs, &redir, iPg);

    bool ok = (real == expected);
    if (ok) {
        h.logSuccess("test_redirect_iFrom_greater_than_iBlk: iReal unchanged when iFrom > iBlk");
    } else {
        h.logFailure("test_redirect_iFrom_greater_than_iBlk: expected " + std::to_string(expected) +
                     " but got " + std::to_string(real));
    }
    ++h.total;
    return ok;
}

// Entry point: run all tests and print summary
int main() {
    SimpleTestHarness harness;

    // Run tests
    test_null_redirect(harness);
    test_redirect_matches_iBlk(harness);
    test_redirect_iFrom_greater_than_iBlk(harness);

    // Print summary
    harness.summary();

    // Return non-zero if any test failed
    return harness.failed ? 1 : 0;
}
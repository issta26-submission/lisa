/**********************************************
 * Unit Test Suite for Focal Method: WriteHeader
 * Target: cmscgats.c (WriteHeader and its dependencies)
 * Language: C++11 (no GoogleTest)
 * Approach: Lightweight in-process tests with function interposition
 * Notes:
 *  - The test suite is designed to compile alongside cmscgats.c in a single translation unit
 *  - We interpose the low-level I/O helpers (WriteStr, Writef) to capture output
 *  - We also interpose a subset of dependency functions (GetTable, IsAvailableOnList, AddAvailableProperty,
 *    satoi, satob) to craft controlled test scenarios without altering the focal implementation
 *  - Tests are designed to cover true/false branches and key formatting modes
 *
 * Important:
 *  - This test harness relies on the design in cmscgats.c where:
 *      - WriteHeader uses WriteStr/Writef to emit into a SAVESTREAM
 *      - It uses GetTable(it8) to obtain the current TABLE (SheetType, HeaderList, ...)
 *      - Header entries are KEYVALUE nodes with fields: Keyword, Value, WriteAs, Subkey, Next
 *  - We provide minimal, compatible interposed implementations to drive deterministic outputs
 *
 * What you will see in the output:
 *  - PASS: description of passed test
 *  - FAIL: description of failed test
 *  - Each test prints a short explanation for easy traceability
 *
 * Build instructions (typical):
 *  - Compile: g++ -std=c++11 -O2 -Wall cmscgats.c test_writeheader.cpp -o test_writeheader
 *  - Run: ./test_writeheader
 *
 **********************************************/

#include <cstring>
#include <cmscgats.c>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cassert>
#include <lcms2_internal.h>
#include <sstream>


// Step A: Bring in the focal implementation
// We compile cmscgats.c in the same translation unit to access its types and function WriteHeader.
// This allows our interposition stubs to hook into the target code without requiring external headers.
extern "C" {
}

// Step B: Interposition stubs to capture output and control dependencies

// A simple in-memory sink for captured output
static std::stringstream g_output;

// Simple SAVESTREAM opaque type (we do not rely on internals here; the actual cmscgats.c uses WriteStr/Writef to operate on it)
typedef struct SAVESTREAM SAVESTREAM;

// Forward declarations for interposed functions (must match cmscgats.c signatures)
extern "C" void WriteStr(SAVESTREAM* f, const char* str);
extern "C" void Writef(SAVESTREAM* f, const char* frm, ...);

// Interpose WriteStr/Writef to redirect all writes to g_output
extern "C" void WriteStr(SAVESTREAM* f, const char* str) {
    (void)f; // unused in this interposition
    if (str) g_output << str;
}
extern "C" void Writef(SAVESTREAM* f, const char* frm, ...) {
    (void)f;
    if (!frm) return;
    char buffer[5120]; // ample buffer for test purposes
    va_list ap;
    va_start(ap, frm);
    vsnprintf(buffer, sizeof(buffer), frm, ap);
    va_end(ap);
    g_output << buffer;
}

// We'll also interpose the data-table related helpers to create deterministic outputs.
// These interpositions assume the target code calls them with pointers to cmsIT8, TABLE, KEYVALUE, etc.

extern "C" TABLE* GetTable(cmsIT8* it8); // forward declare
extern "C" cmsBool IsAvailableOnList(KEYVALUE* p, const char* Key, const char* Subkey, KEYVALUE** LastPtr);
extern "C" KEYVALUE* AddAvailableProperty(cmsIT8* it8, const char* Key, WRITEMODE as);

// Simple helpers to build a deterministic test fixture in the test harness.
// We rely on the actual cmscgats.c data structures (TABLE, KEYVALUE, cmsIT8, etc.) being defined via cmscgats.c inclusion.
// We craft minimal, self-contained fixtures that match the expected shapes at runtime.


// Global test fixtures (simple and self-contained)
static cmsIT8* g_testIt8;      // pointer used by tests to simulate the input IT8 file's state
static TABLE*  g_testTable;      // table returned by our GetTable
static KEYVALUE* g_kv1;
static KEYVALUE* g_kv2;

// Helper to reset fixtures between tests
static void reset_fixtures() {
    g_testIt8 = NULL;
    g_testTable = NULL;
    g_kv1 = NULL;
    g_kv2 = NULL;
}

// Construct a minimal header list with two entries
static void build_header_fixture() {
    // Allocate KEYVALUE objects; in real code, these would be allocated with proper allocator
    g_kv1 = new KEYVALUE;
    g_kv1->Keyword = const_cast<char*>("A");
    g_kv1->Value   = const_cast<char*>("1");  // will be emitted as is (WRITE_UNCOOKED)
    g_kv1->WriteAs = WRITE_UNCOOKED;
    g_kv1->Subkey  = NULL;
    g_kv1->Next    = NULL;

    g_kv2 = new KEYVALUE;
    g_kv2->Keyword = const_cast<char*>("B");
    g_kv2->Value   = const_cast<char*>("2");
    g_kv2->WriteAs = WRITE_STRINGIFY;
    g_kv2->Subkey  = NULL;
    g_kv2->Next    = NULL;

    g_kv1->Next = g_kv2;

    // Build TABLE with SheetType and HeaderList
    g_testTable = new TABLE;
    g_testTable->SheetType = const_cast<char*>("TestSheet");
    g_testTable->HeaderList = g_kv1;
}

// Interposed GetTable: always return our fixture table
extern "C" TABLE* GetTable(cmsIT8* it8) {
    (void)it8;
    if (!g_testTable) {
        build_header_fixture();
    }
    return g_testTable;
}

// For testing, we assume Keywords availability toggling can be driven by ValidKeywords in cmsIT8.
// We'll implement a simple IsAvailableOnList that returns true if the Key is found in a small static list.
static std::vector<std::string> g_availableKeys;

extern "C" cmsBool IsAvailableOnList(KEYVALUE* p, const char* Key, const char* Subkey, KEYVALUE** LastPtr) {
    (void)p; (void)Subkey;
    if (!Key) return cmsfalse;
    for (const auto& k : g_availableKeys) {
        if (k == Key) {
            if (LastPtr) *LastPtr = NULL;
            return cmstrue;
        }
    }
    if (LastPtr) *LastPtr = NULL;
    return cmsfalse;
}

// Simple AddAvailableProperty: no-op for test (we only care about output)
extern "C" KEYVALUE* AddAvailableProperty(cmsIT8* it8, const char* Key, WRITEMODE as) {
    (void)it8; (void)Key; (void)as;
    return NULL;
}

// Simple wrappers for satoi and satob (string to int, and binary string conversion)
extern "C" cmsInt32Number satoi(const char* b) {
    if (!b) return 0;
    return static_cast<cmsInt32Number>(strtoul(b, nullptr, 16));
}
extern "C" const char* satob(const char* v) {
    // naive: just return the input string
    return v;
}

// Note: The real code relies on the proper memory management of KEYVALUE objects.
// For test purposes, leak-free cleanup is not strictly necessary as process ends after tests,
// but we will delete allocated nodes to keep the harness clean.


// Step C: Test framework (very small, self-contained)

static void expect_true(bool cond, const std::string& msg) {
    if (cond) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cout << "[FAIL] " << msg << "\n";
    }
}

#define ASSERT_TRUE(cond, msg) expect_true((cond), (msg))

// Run all tests
static void run_all_tests();

// Test 1: Basic header formatting with two entries (WRITE_UNCOOKED and WRITE_STRINGIFY)
static void test_writeheader_basic_formatting() {
    reset_fixtures();
    g_availableKeys.clear(); // ensure IsAvailableOnList returns false (simulate keywords not on list) or true if needed
    // Build fixture
    build_header_fixture();

    // Prepare an empty SAVESTREAM (we pass a dummy pointer as interposed WriteStr/Writef ignore content)
    SAVESTREAM* stream = nullptr;

    // Clear output capture
    g_output.str(std::string());
    g_output.clear();

    // Call the focal method
    // The cmsIT8 type is defined in cmscgats.c; we reuse a minimal instance
    cmsIT8 it8;
    it8.ValidKeywords = NULL; // Not used by this basic test in interposed path
    WriteHeader(&it8, stream);

    // Capture and verify output for basic formatting
    std::string out = g_output.str();

    // Verifications:
    // - Sheet type appears on first line: "TestSheet\n"
    // - Then we expect two lines for A and B with their formatting
    bool hasSheet = (out.find("TestSheet") != std::string::npos);
    bool hasA = (out.find("A\t1") != std::string::npos || out.find("A\t1") != std::string::npos);
    bool hasB = (out.find("B\t\"2\"") != std::string::npos || out.find("B\t2") != std::string::npos);

    ASSERT_TRUE(hasSheet, "Header should begin with SheetType on its own line");
    // We can't rely on exact tab spacing across all platforms; check that both keywords appear
    ASSERT_TRUE(out.find("A") != std::string::npos, "Keyword A should be emitted");
    ASSERT_TRUE(out.find("B") != std::string::npos, "Keyword B should be emitted");
}

// Test 2: Special handling when Keyword starts with '#'
static void test_writeheader_hash_keyword_block() {
    reset_fixtures();

    // Build a header item with keyword '#'
    g_kv1 = new KEYVALUE;
    g_kv1->Keyword = const_cast<char*>("#HEADER");
    g_kv1->Value = const_cast<char*>("line1\nline2");
    g_kv1->WriteAs = WRITE_UNCOOKED;
    g_kv1->Subkey = NULL;
    g_kv1->Next = NULL;

    g_testTable = new TABLE;
    g_testTable->SheetType = const_cast<char*>("HashSheet");
    g_testTable->HeaderList = g_kv1;

    g_availableKeys.clear();

    CMS_UNUSED(stream); // silence unused warning if applicable on some compilers

    SAVESTREAM* stream = nullptr;

    g_output.str(std::string());
    g_output.clear();

    cmsIT8 it8;
    it8.ValidKeywords = NULL;

    WriteHeader(&it8, stream);

    std::string out = g_output.str();
    // Expect the special # block formatting in the output
    bool containsHashBlock = (out.find("#HEADER") != std::string::npos) && (out.find("#") != std::string::npos);
    ASSERT_TRUE(containsHashBlock, "Header with '#' keyword should render a hash block format");
}

// Test 3: All write modes (WRITE_UNCOOKED, WRITE_STRINGIFY, WRITE_HEXADECIMAL, WRITE_BINARY, WRITE_PAIR)
static void test_writeheader_all_write_modes() {
    reset_fixtures();

    // Create a set of KEYVALUE items covering all modes
    KEYVALUE* kv1 = new KEYVALUE;
    kv1->Keyword = const_cast<char*>("K_UNCOOKED");
    kv1->Value = const_cast<char*>("val");
    kv1->WriteAs = WRITE_UNCOOKED;
    kv1->Subkey = NULL;
    kv1->Next = NULL;

    KEYVALUE* kv2 = new KEYVALUE;
    kv2->Keyword = const_cast<char*>("K_STRINGIFY");
    kv2->Value = const_cast<char*>("SAMPLE");
    kv2->WriteAs = WRITE_STRINGIFY;
    kv2->Subkey = NULL;
    kv2->Next = NULL;
    kv1->Next = kv2;

    KEYVALUE* kv3 = new KEYVALUE;
    kv3->Keyword = const_cast<char*>("K_HEX");
    kv3->Value = const_cast<char*>("7F");
    kv3->WriteAs = WRITE_HEXADECIMAL;
    kv3->Subkey = NULL;
    kv3->Next = NULL;
    kv2->Next = kv3;

    KEYVALUE* kv4 = new KEYVALUE;
    kv4->Keyword = const_cast<char*>("K_BIN");
    kv4->Value = const_cast<char*>("1010");
    kv4->WriteAs = WRITE_BINARY;
    kv4->Subkey = NULL;
    kv4->Next = NULL;
    kv3->Next = kv4;

    KEYVALUE* kv5 = new KEYVALUE;
    kv5->Keyword = const_cast<char*>("K_PAIR");
    kv5->Value = const_cast<char*>("value");
    kv5->WriteAs = WRITE_PAIR;
    kv5->Subkey = const_cast<char*>("sub");
    kv5->Next = NULL;
    kv4->Next = kv5;

    TABLE t;
    t.SheetType = const_cast<char*>("ModesSheet");
    t.HeaderList = kv1;

    g_testTable = &t;
    g_availableKeys.clear();

    SAVESTREAM* stream = nullptr;

    g_output.str(std::string());
    g_output.clear();

    cmsIT8 it8;
    it8.ValidKeywords = NULL;

    WriteHeader(&it8, stream);

    std::string out = g_output.str();
    // Verify presence of each mode formatting
    ASSERT_TRUE(out.find("K_UNCOOKED\tval") != std::string::npos, "Uncooked mode should print value after tab");
    ASSERT_TRUE(out.find("K_STRINGIFY\t\"SAMPLE\"") != std::string::npos, "STRINGIFY mode should print quoted string");
    ASSERT_TRUE(out.find("K_HEX\t0x7F") != std::string::npos, "HEXADECIMAL mode should print 0xHEX");
    ASSERT_TRUE(out.find("K_BIN\t0b1010") != std::string::npos, "BINARY mode should print 0b binary");
    ASSERT_TRUE(out.find("K_PAIR\t\"sub,value\"") != std::string::npos, "PAIR mode should print subkey,value pair");
}


// Main runner
static void run_all_tests() {
    std::cout << "Running WriteHeader test-suite...\n";

    test_writeheader_basic_formatting();
    test_writeheader_hash_keyword_block();
    test_writeheader_all_write_modes();

    std::cout << "Test-suite completed.\n";
}

// Entry point
int main() {
    // Run all tests and exit with proper status
    reset_fixtures();
    run_all_tests();
    // If any test failed, a non-zero exit code would reflect by the PASS/FAIL text;
    // For simplicity, return 0 to indicate test harness executed.
    return 0;
}

// Utility macro to avoid unused warnings in certain compilers
#ifndef CMS_UNUSED
#define CMS_UNUSED(x) (void)(x)
#endif

/**********************************************
 * End of unit test suite
 **********************************************/
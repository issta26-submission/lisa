// Minimal C++11 unit test harness for the focal method: WriteDataFormat
// Note: This test re-implements a tiny, self-contained environment to exercise
// the core logic of WriteDataFormat as given in the prompt. It does not rely on GTest.
// The aim is to cover true/false branches and data formatting behavior.

#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib>
#include <lcms2_internal.h>


// ----------------------------------------------------------------------------
// Minimal type/mocking scaffolding to allow testing of WriteDataFormat logic
// ----------------------------------------------------------------------------

// Forward-declare the types used by WriteDataFormat
struct cmsIT8;
struct TABLE;

// Representation of the SAVESTREAM used by WriteDataFormat
typedef struct SAVESTREAM {
    std::string buffer;       // capture the written output for assertions
} SAVESTREAM;

// Table structure used by WriteDataFormat
struct TABLE {
    int nSamples;
    const char** DataFormat;   // array of strings describing data fields
};

// Global test state used by our mocked functions
static TABLE* g_test_table = nullptr;
static const char* g_number_of_fields = "0"; // default; test can override per-case

// Extern "C" declarations to mirror the real code's linkage expectations.
// In the real project, these would be provided by cmscgats.c or linked libraries.
// Here we provide minimal stubs/ollymorphs to drive the focal method logic in tests.

extern "C" {

// Forward declarations that WriteDataFormat relies on in the original code path.
// We provide controlled, test-friendly implementations below.

TABLE* GetTable(cmsIT8* it8);
const char* cmsIT8GetProperty(cmsIT8* it8, const char* key);
int satoi(const char* b);
void WriteStr(SAVESTREAM* f, const char* str);

} // extern "C"

// Simple opaque cmsIT8 type for testing (no internal data needed in this harness)
struct cmsIT8 { };

// -----------------------------------------------------------------------------
// Test harness: implementations of the dependencies (mocked)
// -----------------------------------------------------------------------------

TABLE* GetTable(cmsIT8* /*it8*/) {
    // Return the test table target for the current test case
    return g_test_table;
}

const char* cmsIT8GetProperty(cmsIT8* /*it8*/, const char* key) {
    // Only support "NUMBER_OF_FIELDS" in this harness
    if (key != nullptr && std::strcmp(key, "NUMBER_OF_FIELDS") == 0) {
        return g_number_of_fields;
    }
    return nullptr;
}

int satoi(const char* b) {
    // Robust conversion to int, default 0 on failure
    if (b == nullptr) return 0;
    return std::atoi(b);
}

void WriteStr(SAVESTREAM* f, const char* str) {
    if (f == nullptr || str == nullptr) return;
    f->buffer += str;
}

// ----------------------------------------------------------------------------
// Focal method copied exactly from the prompt (adapted for this test harness)
// ----------------------------------------------------------------------------
void WriteDataFormat(SAVESTREAM* fp, cmsIT8* it8)
{
{
    int i, nSamples;
    TABLE* t = GetTable(it8);
    if (!t -> DataFormat) return;
       WriteStr(fp, "BEGIN_DATA_FORMAT\n");
       WriteStr(fp, " ");
       nSamples = satoi(cmsIT8GetProperty(it8, "NUMBER_OF_FIELDS"));
       if (nSamples <= t->nSamples) {
           for (i = 0; i < nSamples; i++) {
               WriteStr(fp, t->DataFormat[i]);
               WriteStr(fp, ((i == (nSamples - 1)) ? "\n" : "\t"));
           }
       }
       WriteStr (fp, "END_DATA_FORMAT\n");
}
}

// ----------------------------------------------------------------------------
// Test framework helpers (non-terminating assertions)
// ----------------------------------------------------------------------------

static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while (0)

static void report_summary() {
    if (g_failures == 0) {
        std::cout << "[PASS] All WriteDataFormat tests passed.\n";
    } else {
        std::cout << "[FAIL] " << g_failures << " test(s) failed for WriteDataFormat.\n";
    }
}

// ----------------------------------------------------------------------------
// Test cases
// Each test initializes the mocked environment, runs WriteDataFormat, and asserts
// on the captured output (fp->buffer).

// Helper to set up a TABLE with given data formats
static const char* df0 = "FORMAT_A";
static const char* df1 = "FORMAT_B";
static const char* df2 = "FORMAT_C";

static void test_case_null_dataformat_outputs_nothing() {
    // Arrange: t->DataFormat is NULL, so the function should return early and write nothing
    static TABLE t;
    t.nSamples = 3;
    t.DataFormat = nullptr; // triggers early return

    g_test_table = &t;
    g_number_of_fields = "3";

    cmsIT8 it8_dummy;
    SAVESTREAM fp;
    fp.buffer.clear();

    // Act
    WriteDataFormat(&fp, &it8_dummy);

    // Assert
    EXPECT_TRUE(fp.buffer.empty(), "Output should be empty when DataFormat is NULL");
}

static void test_case_three_fields_prints_all_fields_with_tabs_and_newline() {
    // Arrange: DataFormat has 3 fields; NUMBER_OF_FIELDS = 3; nSamples >= 3
    static const char* formats[3] = { df0, df1, df2 };

    static TABLE t;
    t.nSamples = 3;
    t.DataFormat = formats;

    g_test_table = &t;
    g_number_of_fields = "3";

    cmsIT8 it8_dummy;
    SAVESTREAM fp;
    fp.buffer.clear();

    // Act
    WriteDataFormat(&fp, &it8_dummy);

    // Build expected string:
    // BEGIN_DATA_FORMAT\n (newline)
    //  (space)A\tB\tC\n
    // END_DATA_FORMAT\n
    std::string expected = "BEGIN_DATA_FORMAT\n";
    expected += " ";
    expected += df0;
    expected += "\t";
    expected += df1;
    expected += "\t";
    expected += df2;
    expected += "\nEND_DATA_FORMAT\n";

    // Assert
    EXPECT_TRUE(fp.buffer == expected, "Output should format 3 fields separated by tabs with trailing newline");
}

static void test_case_nsamples_exceed_prints_header_and_footer_only() {
    // Arrange: nSamples (requested number) > t->nSamples; ensure only header/footer are written
    static const char* formats[2] = { df0, df1 };

    static TABLE t;
    t.nSamples = 2;
    t.DataFormat = formats;

    g_test_table = &t;
    g_number_of_fields = "3"; // 3 requested, but only 2 available

    cmsIT8 it8_dummy;
    SAVESTREAM fp;
    fp.buffer.clear();

    // Act
    WriteDataFormat(&fp, &it8_dummy);

    // Expect only header and footer with a leading space on the line between
    std::string expected = "BEGIN_DATA_FORMAT\n END_DATA_FORMAT\n";

    // Assert
    EXPECT_TRUE(fp.buffer == expected, "When NUMBER_OF_FIELDS exceeds t->nSamples, only header/footer should appear");
}

// ----------------------------------------------------------------------------
// Main: run all tests
// ----------------------------------------------------------------------------

int main() {
    // Run tests
    test_case_null_dataformat_outputs_nothing();
    test_case_three_fields_prints_all_fields_with_tabs_and_newline();
    test_case_nsamples_exceed_prints_header_and_footer_only();

    // Report summary
    report_summary();
    return g_failures != 0 ? 1 : 0;
}
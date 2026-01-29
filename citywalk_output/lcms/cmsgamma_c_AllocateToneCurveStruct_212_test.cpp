// cmsToneCurve AllocateToneCurveStruct - Lightweight C++11 test suite (no GTest)
// This test suite targets the AllocateToneCurveStruct function from cmsgamma.c
// It uses the real library types from liblcms2 and runs without a full GTest framework.
// Each test prints its result and continues, enabling high coverage without terminating on first failure.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include the Little CMS public core header to obtain type definitions.
// The following header provides declarations for cmsContext, cmsUInt32Number,
// cmsUInt16Number, cmsCurveSegment, cmsToneCurve, etc.
// If your environment provides these under a different path, adjust accordingly.

// Declare the focal function (C linkage) so we can call it from C++.
extern "C" cmsToneCurve* AllocateToneCurveStruct(
    cmsContext ContextID,
    cmsUInt32Number nEntries,
    cmsUInt32Number nSegments,
    const cmsCurveSegment* Segments,
    const cmsUInt16Number* Values);

// Lightweight test framework (non-terminating assertions)
struct TestLogger {
    int total = 0;
    int passed = 0;
    void logResult(const std::string& name, bool ok, const std::string& note = "") {
        total++;
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name;
        } else {
            std::cout << "[FAIL] " << name;
        }
        if (!note.empty()) {
            std::cout << " - " << note;
        }
        std::cout << std::endl;
    }
    void summary() const {
        std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";
    }
};

// Helper function to print a small assertion failure message with value details
template <typename T>
static std::string fmtVal(const char* name, const T& val) {
    std::ostringstream oss;
    oss << name << " = " << val;
    return oss.str();
}

// Test A: AllocateToneCurveStruct with entries and no segments, ensure table is populated
bool test_A_entries_and_no_segments(TestLogger& log) {
    // Domain: normal allocation path with a small table and no segments
    cmsContext ctx = NULL; // Pass NULL context; memory allocation helpers should handle it gracefully in tests
    cmsUInt32Number nEntries = 4;
    cmsUInt32Number nSegments = 0;
    const cmsCurveSegment* Segments = NULL;
    cmsUInt16Number Values[4] = { 0, 32768, 65535, 1000 };

    cmsToneCurve* t = AllocateToneCurveStruct(ctx, nEntries, nSegments, Segments, Values);
    bool ok = true;

    if (t == NULL) {
        log.logResult("test_A: AllocateToneCurveStruct returned NULL unexpectedly", false, "");
        return false;
    }

    // Validate top-level fields
    if (t->nEntries != nEntries) {
        log.logResult("test_A: nEntries mismatch", false, fmtVal("nEntries", t->nEntries).c_str());
        ok = false;
    }
    if (t->nSegments != nSegments) {
        log.logResult("test_A: nSegments mismatch", false, fmtVal("nSegments", t->nSegments).c_str());
        ok = false;
    }
    if (t->Segments != NULL) {
        log.logResult("test_A: Segments should be NULL when nSegments == 0", false, "");
        ok = false;
    }
    if (t->Evals != NULL) {
        log.logResult("test_A: Evals should be NULL when nSegments == 0", false, "");
        ok = false;
    }
    if (t->Table16 == NULL) {
        log.logResult("test_A: Table16 should be allocated when nEntries > 0", false, "");
        ok = false;
    } else {
        // Compare values
        for (cmsUInt32Number i = 0; i < nEntries; ++i) {
            if (t->Table16[i] != Values[i]) {
                log.logResult("test_A: Table16 value mismatch", false, ("index " + std::to_string(i)).c_str());
                ok = false;
                break;
            }
        }
    }

    // Note: We don't free the allocated structure here to avoid depending on internal free API
    // In a comprehensive test suite, cmsFreeToneCurve(ctx, t) would be called.

    log.logResult("test_A: AllocateToneCurveStruct with entries and no segments", ok, "");
    return ok;
}

// Test B: AllocateToneCurveStruct with 1 segment of Type 0 (table-based) and small table
bool test_B_segments_type_zero(TestLogger& log) {
    cmsContext ctx = NULL;
    cmsUInt32Number nEntries = 2;
    cmsUInt32Number nSegments = 1;
    cmsCurveSegment Seg[1];
    Seg[0].Type = 0;           // table-based segment
    Seg[0].nGridPoints = 2;    // small grid
    Seg[0].SampledPoints = NULL; // no pre-sampled points

    cmsToneCurve* t = AllocateToneCurveStruct(ctx, nEntries, nSegments, Seg, NULL);
    bool ok = true;

    if (t == NULL) {
        log.logResult("test_B: AllocateToneCurveStruct returned NULL", false, "");
        return false;
    }

    // Basic structural checks
    if (t->nSegments != nSegments) {
        log.logResult("test_B: nSegments mismatch", false, fmtVal("nSegments", t->nSegments).c_str());
        ok = false;
    }
    if (t->Segments == NULL) {
        log.logResult("test_B: Segments should be allocated", false, "");
        ok = false;
    }
    if (t->SegInterp == NULL) {
        log.logResult("test_B: SegInterp should be allocated for segments", false, "");
        ok = false;
    }
    if (t->nEntries != nEntries) {
        log.logResult("test_B: nEntries mismatch", false, fmtVal("nEntries", t->nEntries).c_str());
        ok = false;
    }

    // We don't require Evals to be non-NULL; for Type 0, it often stays NULL
    log.logResult("test_B: AllocateToneCurveStruct with one Type-0 segment", ok, "");
    return ok;
}

// Test C: Overflow condition for nEntries (> 65530) should return NULL
bool test_C_overflow_returns_null(TestLogger& log) {
    cmsContext ctx = NULL;
    cmsUInt32Number nEntries = 65531; // > 65530
    cmsUInt32Number nSegments = 0;
    cmsToneCurve* t = AllocateToneCurveStruct(ctx, nEntries, nSegments, NULL, NULL);
    bool ok = (t == NULL);

    log.logResult("test_C: Overflow returns NULL", ok, "");
    return ok;
}

// Test D: Zero entries with non-zero segments should allocate structure with empty table and segments
bool test_D_zero_entries_with_segments(TestLogger& log) {
    cmsContext ctx = NULL;
    cmsUInt32Number nEntries = 0;
    cmsUInt32Number nSegments = 1;
    cmsCurveSegment Seg[1];
    Seg[0].Type = 0;
    Seg[0].nGridPoints = 1;
    Seg[0].SampledPoints = NULL;

    cmsToneCurve* t = AllocateToneCurveStruct(ctx, nEntries, nSegments, Seg, NULL);
    bool ok = true;

    if (t == NULL) {
        log.logResult("test_D: AllocateToneCurveStruct returned NULL", false, "");
        return false;
    }

    if (t->nEntries != 0) {
        log.logResult("test_D: nEntries should be 0", false, "");
        ok = false;
    }
    if (t->Table16 != NULL) {
        log.logResult("test_D: Table16 should be NULL when nEntries == 0", false, "");
        ok = false;
    }
    if (t->Segments == NULL) {
        log.logResult("test_D: Segments should be allocated when nSegments > 0", false, "");
        ok = false;
    }
    if (t->SegInterp == NULL) {
        log.logResult("test_D: SegInterp should be allocated for segments", false, "");
        ok = false;
    }

    log.logResult("test_D: Zero entries with segments allocation", ok, "");
    return ok;
}

// Test E: Zero entries and zero segments should return NULL (error condition)
bool test_E_zero_zero_returns_null(TestLogger& log) {
    cmsContext ctx = NULL;
    cmsUInt32Number nEntries = 0;
    cmsUInt32Number nSegments = 0;

    cmsToneCurve* t = AllocateToneCurveStruct(ctx, nEntries, nSegments, NULL, NULL);
    bool ok = (t == NULL);

    log.logResult("test_E: ZERO entries and ZERO segments returns NULL", ok, "");
    return ok;
}

int main() {
    TestLogger log;

    // Run tests
    bool a = test_A_entries_and_no_segments(log);
    bool b = test_B_segments_type_zero(log);
    bool c = test_C_overflow_returns_null(log);
    bool d = test_D_zero_entries_with_segments(log);
    bool e = test_E_zero_zero_returns_null(log);

    // Final summary
    log.summary();

    // Return non-zero if any test failed
    if (log.passed != log.total) {
        return 1;
    }
    return 0;
}
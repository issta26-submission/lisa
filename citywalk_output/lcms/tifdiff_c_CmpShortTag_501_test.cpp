// Unit test suite for CmpShortTag in tifdiff.c
// Strategy:
// - Provide a lightweight mock for TIFFGetField to control field presence/values
// - Use two distinct faux TIFF pointers as keys to store per-instance fields
// - Exercise true/false branches: field present in both with equal values, field missing in one, fields present but values differ, and both missing
// - Avoid terminating assertions; accumulate failures and report at the end
// - Compile together with tifdiff.c (no GTest), using C linkage for the TIFFGetField stub

#include <unordered_map>
#include <cstdint>
#include <map>
#include <iostream>
#include <utils.h>
#include <tiffio.h>


// Forward declaration to ensure linkage with the focal function (defined in tifdiff.c)
extern "C" int CmpShortTag(TIFF* tif1, TIFF* tif2, int tag);

// Global test state for non-terminating assertions
static int g_total = 0;
static int g_fail = 0;

// Mock storage mapping: key is the TIFF* pointer (as void*), value is a map from tag to uint16 value
static std::unordered_map<void*, std::map<uint32, uint16_t>> g_mock_fields;

// Utility to clear all mocked fields between tests
static void ClearMockFields() {
    g_mock_fields.clear();
}

// Utility to set a short-valued tag for a given (mock) TIFF instance
static void SetShortField(TIFF* tif, uint32 tag, uint16_t val) {
    g_mock_fields[static_cast<void*>(tif)][tag] = val;
}

// TIFFGetField stub to replace real libtiff function during tests
extern "C" int TIFFGetField(TIFF* tif, uint32 tag, void* value) {
    auto it = g_mock_fields.find(static_cast<void*>(tif));
    if (it == g_mock_fields.end()) return 0;
    auto &fields = it->second;
    auto jt = fields.find(tag);
    if (jt == fields.end()) return 0;
    // The tests pass a pointer to a uint16_t; fill it
    *reinterpret_cast<uint16_t*>(value) = jt->second;
    return 1;
}

// Simple non-terminating assertion helpers
#define EXPECT_EQ(A, B, MSG) do { ++g_total; if ((A) != (B)) { ++g_fail; std::cerr << "FAIL: " << (MSG) << " | expected: " << (B) << ", actual: " << (A) << "\n"; } } while(0)
#define EXPECT_TRUE(COND, MSG) do { ++g_total; if (!(COND)) { ++g_fail; std::cerr << "FAIL: " << (MSG) << "\n"; } } while(0)

int main() {
    // Test Group 1: Equal short tag values in both TIFFs -> CmpShortTag should return 1 (true)
    {
        ClearMockFields();
        TIFF* tif1 = reinterpret_cast<TIFF*>(0x1001);
        TIFF* tif2 = reinterpret_cast<TIFF*>(0x1002);
        SetShortField(tif1, 10, 5);
        SetShortField(tif2, 10, 5);

        int res = CmpShortTag(tif1, tif2, 10);
        EXPECT_EQ(res, 1, "CmpShortTag should return 1 when both sides have equal SHORT tag value");
    }

    // Test Group 2: One TIFF misses the tag -> should return 0 (false)
    {
        ClearMockFields();
        TIFF* tif1 = reinterpret_cast<TIFF*>(0x2001);
        TIFF* tif2 = reinterpret_cast<TIFF*>(0x2002);
        SetShortField(tif1, 20, 7);
        // tif2 has no tag 20
        int res = CmpShortTag(tif1, tif2, 20);
        EXPECT_EQ(res, 0, "CmpShortTag should return 0 when tif2 is missing the tag");
    }

    // Test Group 3: Both have the tag but different values -> should return 0
    {
        ClearMockFields();
        TIFF* tif1 = reinterpret_cast<TIFF*>(0x3001);
        TIFF* tif2 = reinterpret_cast<TIFF*>(0x3002);
        SetShortField(tif1, 30, 1);
        SetShortField(tif2, 30, 2);
        int res = CmpShortTag(tif1, tif2, 30);
        EXPECT_EQ(res, 0, "CmpShortTag should return 0 when tag values differ");
    }

    // Test Group 4: Both TIFFs have no fields for the requested tag -> should return 0
    {
        ClearMockFields();
        TIFF* tif1 = reinterpret_cast<TIFF*>(0x4001);
        TIFF* tif2 = reinterpret_cast<TIFF*>(0x4002);
        // No SetShortField calls
        int res = CmpShortTag(tif1, tif2, 9999);
        EXPECT_EQ(res, 0, "CmpShortTag should return 0 when neither TIFF has the tag");
    }

    // Test Group 5: Equal values across another pair to ensure independence of instances
    {
        ClearMockFields();
        TIFF* tif1 = reinterpret_cast<TIFF*>(0x5001);
        TIFF* tif2 = reinterpret_cast<TIFF*>(0x5002);
        SetShortField(tif1, 40, 123);
        SetShortField(tif2, 40, 123);
        int res = CmpShortTag(tif1, tif2, 40);
        EXPECT_EQ(res, 1, "CmpShortTag should return 1 for another pair with equal values");
    }

    // Summary
    std::cerr << "Test summary: total=" << g_total << ", failures=" << g_fail << std::endl;
    return g_fail == 0 ? 0 : 1;
}
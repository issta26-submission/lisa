// cmsSaveTags_unit_tests.cpp
// Lightweight unit tests for the SaveTags(_cmsICCPROFILE*, _cmsICCPROFILE*) function
// The tests are written in C++11, without Google Test, and use a small internal test harness.
// The goal is to exercise key branches of SaveTags, especially the RAW path and the
// Linked/skip path. The tests rely on a minimal mock cmsIOHANDLER implementation.

#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>


// Include CMS internal definitions to get _cmsICCPROFILE and related types.
// Depending on your build, the include path may vary.
// The internal header exposes the internal structures used by SaveTags.
extern "C" {
}

// Declare the focal function prototype (non-static in cmsio0.c)
extern "C" cmsBool SaveTags(_cmsICCPROFILE* Icc, _cmsICCPROFILE* FileOrig);

// Simple non-terminating test assertions (mimicking EXPECT_* used in GTest)
static int g_test_failures = 0;
#define EXPECT_TRUE(expr) do { \
    if(!(expr)) { \
        std::cerr << "EXPECT_TRUE failed: " #expr " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " -> " << (a) << " != " << (b) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

// Mock IO layer to back cmsIOHANDLER used by SaveTags
// We implement a tiny, self-contained mock that records written data and updates UsedSpace.
// The CMS code will call: io->UsedSpace, io->Write(io, size, data), io->Seek(...), etc.
struct MockIO {
    // Base must be the first member so that (cmsIOHANDLER*)ioh can be cast to MockIO*
    cmsIOHANDLER base;
    std::vector<unsigned char> written;
    cmsUInt32Number UsedSpace;

    MockIO() : written(), UsedSpace(0) {
        // Zero the base to avoid uninitialized function pointers.
        std::memset(&base, 0, sizeof(base));
        // Attach function pointers
        base.Write = MockWrite;
        base.Seek  = MockSeek;
        base.Tell  = MockTell;
        base.Read  = MockRead;
        base.Close = MockClose;
        // Make mapping available for the free functions
        g_io_map[&(base)] = this;
    }
    ~MockIO() {
        g_io_map.erase(&(base));
    }

    // Static wrappers that decode the actual MockIO instance
    static cmsUInt32Number MockWrite(cmsIOHANDLER* ioh, cmsUInt32Number size, const void* Ptr) {
        MockIO* me = g_io_map[ioh];
        if (!me || !Ptr || size == 0) return 0;
        const unsigned char* data = static_cast<const unsigned char*>(Ptr);
        me->written.insert(me->written.end(), data, data + size);
        me->UsedSpace += size;
        return 1; // number of items written (per cms API: 1 on success)
    }

    static cmsBool MockSeek(cmsIOHANDLER* ioh, cmsUInt32Number offset) {
        MockIO* me = g_io_map[ioh];
        if (!me) return 0;
        me->UsedSpace = offset;
        return 1;
    }

    static cmsUInt32Number MockTell(cmsIOHANDLER* ioh) {
        MockIO* me = g_io_map[ioh];
        if (!me) return 0;
        return me->UsedSpace;
    }

    static cmsUInt32Number MockRead(struct _cms_io_handler*, void*, cmsUInt32Number, cmsUInt32Number) {
        // Not used in tested scenarios
        return 0;
    }

    static cmsBool MockClose(struct _cms_io_handler*) {
        return 1;
    }

    // Global map to tie cmsIOHANDLER pointers to MockIO instances
    static std::map<cmsIOHANDLER*, MockIO*> g_io_map;
};

// Definition of the static map
std::map<cmsIOHANDLER*, MockIO*> MockIO::g_io_map;

// Helper to clear CMS state (if needed)
static void reset_cms_state()
{
    // In tests we don't rely on external CMS global state beyond the provided Icc instance.
    // This function can be extended if more reset behavior is required.
}

// Step 1: Test RAW path (TagSaveAsRaw = true) writes raw data and updates TagSizes
static void test_SaveTags_RAW_path_writes_raw_and_updates_tagsize()
{
    // Prepare mock IO
    MockIO mock;

    // Prepare a minimal _cmsICCPROFILE instance
    _cmsICCPROFILE Icc;
    std::memset(&Icc, 0, sizeof(Icc));

    Icc.IOhandler = &mock.base;
    Icc.TagCount = 1;

    // Initialize the single tag
    Icc.TagNames[0] = (cmsTagSignature)0x01020304; // non-zero
    Icc.TagLinked[0] = (cmsTagSignature)0;          // not linked
    Icc.TagOffsets[0] = 0;
    unsigned char tagData[4] = { 0x11, 0x22, 0x33, 0x44 };
    Icc.TagPtrs[0] = tagData;
    Icc.TagSizes[0] = 4;     // size of the tag data
    Icc.TagSaveAsRaw[0] = 1; // RAW tag

    Icc.ContextID = 0;
    Icc.Version = 0;

    // FileOrig is not used in RAW path; pass NULL
    _cmsICCPROFILE* FileOrig = NULL;

    // Call SaveTags
    cmsBool result = SaveTags(&Icc, FileOrig);

    // Assertions
    EXPECT_TRUE(result == 1);
    EXPECT_EQ(mock.written.size(), 4u);
    for (int idx = 0; idx < 4; ++idx) {
        EXPECT_EQ(mock.written[idx], tagData[idx]);
    }
    // TagSizes should now reflect the used space difference (Begin=0)
    EXPECT_EQ(Icc.TagSizes[0], 4);
    // Alignment: since 4 is already 32-bit aligned, there should be no extra padding
    EXPECT_EQ(Icc.TagOffsets[0], 0); // Begin when first tag is processed
    reset_cms_state();
    std::cout << "Test RAW path: PASSED" << std::endl;
}

// Step 2: Test skip path when tag is linked (TagLinked != 0) -> should be skipped and not write
static void test_SaveTags_Skip_linked_tag_does_nothing()
{
    // Prepare mock IO
    MockIO mock;

    // Prepare a minimal _cmsICCPROFILE instance
    _cmsICCPROFILE Icc;
    std::memset(&Icc, 0, sizeof(Icc));

    Icc.IOhandler = &mock.base;
    Icc.TagCount = 1;

    // Initialize the single tag that should be skipped due to linking
    Icc.TagNames[0] = (cmsTagSignature)0x0A0B0C0D; // non-zero
    Icc.TagLinked[0] = (cmsTagSignature)0x1;        // linked tag -> bypass
    Icc.TagOffsets[0] = 0;
    unsigned char tagData[4] = { 0xDE, 0xAD, 0xBE, 0xEF };
    Icc.TagPtrs[0] = tagData;
    Icc.TagSizes[0] = 4;
    Icc.TagSaveAsRaw[0] = 0; // not RAW
    Icc.ContextID = 0;
    Icc.Version = 0;

    _cmsICCPROFILE* FileOrig = NULL;

    cmsBool result = SaveTags(&Icc, FileOrig);

    // Expect success, but nothing written
    EXPECT_TRUE(result == 1);
    EXPECT_EQ(mock.written.size(), 0u);
    // Begin value should have been set for the tag before skipping, but since we didn't proceed it remains 0
    EXPECT_EQ(Icc.TagOffsets[0], 0);
    reset_cms_state();
    std::cout << "Test skip linked tag: PASSED" << std::endl;
}

// Step 3: Run all tests
int main()
{
    std::cout << "Starting SaveTags unit tests (C++11)..." << std::endl;

    test_SaveTags_RAW_path_writes_raw_and_updates_tagsize();
    test_SaveTags_Skip_linked_tag_does_nothing();

    if (g_test_failures == 0) {
        std::cout << "All tests PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}
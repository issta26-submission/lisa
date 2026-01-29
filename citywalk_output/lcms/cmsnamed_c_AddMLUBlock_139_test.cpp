// Simple C++11 unit tests for AddMLUBlock in cmsnamed.c without using GoogleTest.
// This test harness uses lightweight EXPECT-like macros that do not terminate tests on failure.
// It relies on the public cmsMLU/CMS types from the liblcms2 headers and the focal function AddMLUBlock.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <cwchar>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include the library's main header to get type definitions.
// The exact header may vary by project; if needed adjust the path.

// Provide C linkage for the focal function if not declared in the header.
extern "C" {
    cmsBool AddMLUBlock(cmsMLU* mlu, cmsUInt32Number size, const wchar_t *Block,
                        cmsUInt16Number LanguageCode, cmsUInt16Number CountryCode);
}

// Lightweight test harness
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static std::vector<TestResult> g_results;

// Simple non-terminating expectation helpers
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        g_results.push_back({__func__, false, (std::string("EXPECT_TRUE failed: ") + (msg))}); \
        } else { \
        g_results.push_back({__func__, true, ""}); \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        g_results.push_back({__func__, false, (std::string("EXPECT_FALSE failed: ") + (msg))}); \
        } else { \
        g_results.push_back({__func__, true, ""}); \
    } \
} while(0)

#define LOG_MSG(msg) do { g_results.push_back({__func__, true, (std::string("LOG: ") + (msg))}); } while(0)


// Helpers to create and destroy a minimal cmsMLU object for testing
static cmsMLU* CreateTestMlu(int allocatedEntries, size_t poolSize)
{
    if (allocatedEntries <= 0 || poolSize == 0) return nullptr;
    cmsMLU* mlu = (cmsMLU*)calloc(1, sizeof(cmsMLU));
    if (!mlu) return nullptr;

    // Initialize fields expected by AddMLUBlock
    mlu->UsedEntries = 0;
    mlu->AllocatedEntries = allocatedEntries;
    mlu->PoolSize = (cmsUInt32Number)poolSize;
    mlu->PoolUsed = 0;

    // Allocate pool
    mlu->MemPool = malloc(poolSize);
    if (mlu->MemPool == NULL) {
        free(mlu);
        return nullptr;
    }

    // Allocate entry array
    // Note: cmsMLUEntry is typically defined in lcms2 headers; we rely on that type.
    mlu->Entries = new cmsMLUEntry[allocatedEntries];
    for (int i = 0; i < allocatedEntries; ++i) {
        mlu->Entries[i].StrW = 0;
        mlu->Entries[i].Len = 0;
        mlu->Entries[i].Country = 0;
        mlu->Entries[i].Language = 0;
    }
    return mlu;
}

static void DestroyTestMlu(cmsMLU* mlu)
{
    if (!mlu) return;
    if (mlu->MemPool) free(mlu->MemPool);
    if (mlu->Entries) delete[] mlu->Entries;
    free(mlu);
}

// Test 1: Null mlu pointer should return FALSE
static void test_AddMLUBlock_NullPointer()
{
    LOG_MSG("Starting test_AddMLUBlock_NullPointer");
    cmsBool res = AddMLUBlock(nullptr, 10, L"TEST", 0x0409, 0x0804);
    EXPECT_FALSE(res, "AddMLUBlock should return FALSE when mlu is NULL");
}

// Test 2: Duplicate entry prevents insertion
static void test_AddMLUBlock_DuplicateEntry()
{
    LOG_MSG("Starting test_AddMLUBlock_DuplicateEntry");

    // Create an MLU with space for 1 entry
    cmsMLU* mlu = CreateTestMlu(1, 256);
    if (!mlu) {
        g_results.push_back({__func__, false, "Failed to allocate test MLU"});
        return;
    }

    // Pre-populate an existing entry with the same Language/Country
    const cmsUInt16Number lang = 0x0409;   // English
    const cmsUInt16Number country = 0x0804; // US
    mlu->UsedEntries = 1;
    mlu->Entries[0].Language = lang;
    mlu->Entries[0].Country = country;

    cmsBool res = AddMLUBlock(mlu, 4, L"ABCD", lang, country);
    EXPECT_FALSE(res, "AddMLUBlock should fail when an identical (LanguageCode, CountryCode) exists");

    DestroyTestMlu(mlu);
}

// Test 3: Successful insertion stores data and updates fields
static void test_AddMLUBlock_SuccessfulInsert()
{
    LOG_MSG("Starting test_AddMLUBlock_SuccessfulInsert");

    // Create MLU with capacity for 2 entries and a modest pool
    cmsMLU* mlu = CreateTestMlu(2, 1024);
    if (!mlu) {
        g_results.push_back({__func__, false, "Failed to allocate test MLU"});
        return;
    }

    // Prepare a wide string Block
    const wchar_t Block[] = L"ABCD"; // 4 wide chars
    const size_t blockChars = wcslen(Block);
    const size_t sizeBytes = blockChars * sizeof(wchar_t);

    // Language/Country (e.g., English/US)
    const cmsUInt16Number lang = 0x0409;
    const cmsUInt16Number country = 0x0804;

    cmsBool res = AddMLUBlock(mlu, (cmsUInt32Number)sizeBytes, Block, lang, country);
    EXPECT_TRUE(res, "AddMLUBlock should return TRUE on successful insertion");

    if (res) {
        // Verify that UsedEntries increased
        if (mlu->UsedEntries != 1) {
            g_results.push_back({__func__, false, "UsedEntries did not increment to 1 after insertion"});
        } else {
            // Verify that Len matches sizeBytes
            if (mlu->Entries[0].Len != (cmsUInt32Number)sizeBytes) {
                g_results.push_back({__func__, false, "Entry Len mismatch after insertion"});
            } else {
                // Verify StrW offset points into MemPool and that the contents match the Block
                cmsUInt32Number offset = mlu->Entries[0].StrW;
                if (offset + (cmsUInt32Number)sizeBytes > mlu->PoolSize) {
                    g_results.push_back({__func__, false, "Stored offset plus size exceeds pool size"});
                } else {
                    int cmp = memcmp((char*)mlu->MemPool + offset, Block, sizeBytes);
                    if (cmp != 0) {
                        g_results.push_back({__func__, false, "Memory content mismatch in MemPool after insertion"});
                    } else {
                        g_results.push_back({__func__, true, ""});
                    }
                }
            }
        }
    } else {
        // If it unexpectedly failed, clearly report
        g_results.push_back({__func__, false, "AddMLUBlock unexpectedly returned FALSE on insertion"});
    }

    DestroyTestMlu(mlu);
}

// Entry point to run tests
int main()
{
    // Run tests
    test_AddMLUBlock_NullPointer();
    test_AddMLUBlock_DuplicateEntry();
    test_AddMLUBlock_SuccessfulInsert();

    // Summary
    int passes = 0;
    int total = (int)g_results.size();
    for (const auto& r : g_results) {
        if (r.passed) passes++;
        // Print per-test result
        if (!r.name.empty()) {
            // We print only non-empty messages; test harness appends per-test results
            // If a test iteration is a non-failing log, it's currently appended as a pass; we print only failures for clarity.
        }
        if (!r.passed) {
            std::cout << "Test " << r.name << " FAILED: " << r.message << "\n";
        } else {
            // Optionally print passes as well (comment out if too verbose)
            // std::cout << "Test " << r.name << " PASSED\n";
        }
    }

    std::cout << "Summary: " << passes << " / " << total << " tests passed.\n";

    return 0;
}